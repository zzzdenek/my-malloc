#include "malloc.h"
#include "pthread.h"
#include "semaphore.h"

int init;

pthread_mutex_t mutex_heap;

void initialize()
{
    init = 1;
    root.next_block_ = 0;
    root.data_ = (void*) -1;
}

size_t calcBlockSize(magic_value* mv_ptr)
{
    size_t begin = (size_t) mv_ptr;
    size_t end = (size_t) mv_ptr->next_block_;
    size_t size = end - begin;
    return size;
}

magic_value* findLeft(size_t size)
{
    magic_value* mv_ptr = &root;
    while(1)
    {
        if (!mv_ptr->next_block_) break;
        if (!mv_ptr->next_block_->data_ &&
            size <= calcBlockSize(mv_ptr->next_block_))
            break;
        mv_ptr = mv_ptr->next_block_;
    }
    return mv_ptr;
}

void *malloc(size_t size)
{
    if (!size) exit(-1);
    pthread_mutex_init(&mutex_heap, 0);
    if (!init) initialize();
    pthread_mutex_lock(&mutex_heap);
    size_t multiplier = 0;
    if (size <= 16) multiplier = 2;
    else
    {
        multiplier = size / 8;
        multiplier++;
        if (size % 8) multiplier++;
    }
    size = 8 * multiplier;
    magic_value* left_neighbour = findLeft(size);
    void* destination;

    if (!left_neighbour->next_block_)
    {
        destination = sbrk(size);
        if (destination == (void*) -1)
        {
            pthread_mutex_unlock(&mutex_heap);
            return 0;
        }
        magic_value new_mv;
        new_mv.next_block_ = 0;
        new_mv.data_ = 0;
        *(magic_value*)destination = new_mv;
        left_neighbour->next_block_ = (magic_value*) destination;
        left_neighbour->next_block_->data_ = destination + sizeof(magic_value);
    }
    else
    {
        size_t size_block = calcBlockSize(left_neighbour->next_block_);
        size_t difference = size_block - size;
        if (difference >= sizeof(magic_value))
        {
            magic_value new_mv;
            new_mv.next_block_ = left_neighbour->next_block_->next_block_;
            new_mv.data_ = 0;
            void* temp = (void*) left_neighbour->next_block_;
            temp += size;
            magic_value* mv_ptr = (magic_value*) temp;
            *mv_ptr = new_mv;
            left_neighbour->next_block_->next_block_ = mv_ptr;
        }
        void* temp = (void*) left_neighbour->next_block_;
        temp += sizeof(magic_value);
        left_neighbour->next_block_->data_ = temp;
    }
    void* temp = (void*) left_neighbour->next_block_->data_;
    pthread_mutex_unlock(&mutex_heap);
    return temp;
}

magic_value* find(void* ptr)
{
    magic_value* mv_ptr = &root;
    void* temp;
    while(1)
    {
        if (!mv_ptr->next_block_) break;
        temp = (void*) mv_ptr->next_block_;
        temp += sizeof(magic_value);
        if (temp == ptr) return mv_ptr;
        mv_ptr = mv_ptr->next_block_;
    }
    return 0;
}

magic_value* merge(magic_value* left_neigh)
{
    if (left_neigh->next_block_->next_block_)
    {
        if (!left_neigh->next_block_->next_block_->data_)
        {
            left_neigh->next_block_->next_block_ =
                left_neigh->next_block_->next_block_->next_block_;
        }
    }
    if (!left_neigh->data_)
    {
        left_neigh->next_block_ = left_neigh->next_block_->next_block_;
        void* temp = (void*) left_neigh;
        temp += sizeof(magic_value);
        left_neigh = find(temp);
        if (!left_neigh) left_neigh = &root;
    }
    return left_neigh;
}

void free(void *ptr)
{
    pthread_mutex_init(&mutex_heap, 0);
    if (!init) initialize();
    pthread_mutex_lock(&mutex_heap);
    if (!ptr)
    {
        pthread_mutex_unlock(&mutex_heap);
        return;
    }
    magic_value* left_neigh = find(ptr);
    if (!left_neigh)
    {
        pthread_mutex_unlock(&mutex_heap);
        return;
    }
    left_neigh->next_block_->data_ = 0;
    left_neigh = merge(left_neigh);
    if (!left_neigh->next_block_->next_block_)
    {
        void* addr = (void*) left_neigh->next_block_;
        left_neigh->next_block_ = 0;
        brk(addr);
    }
    pthread_mutex_unlock(&mutex_heap);
}

