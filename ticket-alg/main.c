//
//  main.c
//  ticket-alg
//
//  Created by Adolfo Eloy on 22/04/18.
//

#include <stdio.h>
#include <time.h>
#include "ticket.h"

int N; // vector size
int T; // amount of threads

long max_number = 0;
long* vector;

void* max(void* params);
void prepare_vector(void);
void print_vector(void);

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("usage: max <vector_size> <thread_numbers>\n");
        return -1;
    }
    
    N = atoi(argv[1]);
    T = atoi(argv[2]);
    
    if (T > N) {
        printf("number of threads must be less or equals to vector size\n");
        return -1;
    }
    
    prepare_vector();
    
    // creating threads
    int range_size = N / T;
    int offset = 0;
    
    co_init(T); // preparing for ticket Algorithm main loop
    for (int i=0; i<T; i++) {
        
        // process CS[i = 0 to n - 1]
        ProcessInfo_t* process_info = malloc(sizeof(ProcessInfo_t));
        process_info->id = i;
        process_info->start = offset;
        process_info->end = offset + range_size;
        process_info->cs = max;

        co_process(process_info);
        
        offset += range_size;
    }

    co_join(T);

    print_vector();
    printf("max %ld\n", max_number);
    exit(0);
}

/**
 this is the declaration of what defines a max value from an array
 **/
void* max(void* params) {
    int* index = (int*) params;
    
    if (vector[*index] > max_number) {
        max_number = vector[*index];
    }
    return NULL;
}

void prepare_vector() {
    srand((unsigned) time(0));
    vector = malloc(sizeof(long) * N);
    for (int i=0; i<N; i++) {
        *(vector+i) = rand();
    }
}

void print_vector() {
    for (int i=0; i<N; i++) {
        printf("v[%d]=%ld\n", i, vector[i]);
    }
}

