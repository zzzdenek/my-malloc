#ifndef MALLOC_H_
#define MALLOC_H_

#include "unistd.h"
#include "stdlib.h"

//enum state { FREE, ALLOCATED };

typedef struct _MagicValue_ magic_value;
struct _MagicValue_
{
    //enum state state_;
    magic_value* next_block_;
    void* data_;
};

magic_value root;

//magic_value constructMagicValue();

void *malloc(size_t size);

void free(void *ptr);

#endif /* MALLOC_H_ */
