//
//  ticket.c
//  ticket-algorithm
//
//  Created by Adolfo Eloy on 22/04/18.
//
#include "ticket.h"

int co_next;
int* co_turn;

pthread_t* processes;
pthread_mutex_t mutex;  // TODO: remove it when input and output protocol was defined

void co_init(int number_of_threads) {
    processes = malloc(sizeof(pthread_t) * number_of_threads);
    pthread_mutex_init(&mutex, NULL);
}

void co_process(ProcessInfo_t* process_info_param) {
    ProcessInfo_t* process_info = (ProcessInfo_t*) process_info_param;
    int i = process_info->id;
    pthread_create((processes+i), NULL, co_process_loop, (void*) process_info);
}

void* co_process_loop(void* process_info_param) {
    ProcessInfo_t* process_info = (ProcessInfo_t*) process_info_param;
    void* (*f)(void*);
    f = process_info->cs;
    
    for (int index = process_info->start; index < process_info->end; index++) {
        // protocolo de entrada
        __atomic_fetch_add(co_turn+index, 1, __ATOMIC_SEQ_CST); // <turn[i] = number; number++>
        while(co_next != co_turn[index]) {}                     // <await(turn[i] == next)>
        
        // start of CS
        f((void*)&index);

        // protocolo de saida
        __atomic_fetch_add(&co_next, 1, __ATOMIC_SEQ_CST);      // <next=next+1>
    }
    
    return NULL;
}

void co_join(int number_of_threads) {
    for (int i=0; i<number_of_threads; i++) {
        pthread_join(*(processes+i), NULL);
    }
}

