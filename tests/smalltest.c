#include "stdio.h"
#include "malloc.h"
#include "stdlib.h"
#include "assert.h"
 
int main(int argc, char* argv[]){
    //printf("hello\n");
    //size_t m_value_size;
    //m_value_size = getSizeMagicValue();
    //m_value_size = sizeof(char);
    //printf("size = %d bytes!", m_value_size);
    //int* number = malloc(0);
    //printf("number = %p", number);

    //printf("size of int* = %d bytes!\n", sizeof(char*));
    //printf("size of int = %d bytes!\n", sizeof(char));

    void* temp = malloc(1);
    free(temp);
    void* temp0 = malloc(1);
    void* temp1 = malloc(1);
    free(temp1);
    free(temp0);
    
    

    int* data[5];
    data[0] = (int*)malloc(5*sizeof(int));
    assert(data[0] != 0);
    data[1] = (int*)malloc(20*sizeof(int));
    data[2] = (int*)malloc(40*sizeof(int));
    data[3] = (int*)malloc(100*sizeof(int));
    data[4] = (int*)malloc(500*sizeof(int));

    //printf("num = %d\n", num);

    printf("sizeof(temp) =  %d\n", sizeof(temp));
    printf("address of temp = %d\n", temp);
    printf("address of temp0 = %d\n", temp0);
    printf("address of temp1 = %d\n", temp1);


    printf("address of data = %p\n", data);
    printf("address of data[0] = %d\n", data[0]);
    printf("address of data[1] = %d\n", data[1]);
    printf("address of data[2] = %d\n", data[2]);
    printf("address of data[3] = %d\n", data[3]);
    printf("address of data[4] = %d\n", data[4]);


    int i = 0;
    for(i = 0;i<40;i++){
        data[2][i] = 0x99999999;
    }
    for(i = 0;i<500;i++){
        data[4][i] = 666;
    }

    free(data[0]);
    free(data[1]);
    free(data[3]);
    for(i = 0;i<40;i++){
        if(data[2][i] != 0x99999999){
            printf("Error: content changed");
            assert(0);
            
        }
    }
    data[0] = (int*)malloc(25*sizeof(int));
    free(data[0]);
    free(data[2]);
    free(data[4]);
    printf("passed?\n");

    return 0;
}

