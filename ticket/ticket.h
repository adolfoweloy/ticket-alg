//
//  ticket.h
//  ticket-algorithm
//
//  Created by Adolfo Eloy on 22/04/18.
//

#ifndef ticket_h
#define ticket_h

#include <stdlib.h>
#include <pthread.h>

/**
 Ticket Algorithm data structure and functions definitions
 **/
typedef struct ProcessInfo {
    int id;
    int start;
    int end;
    void* (*cs)(void*); // function to run within a critical section
} ProcessInfo_t;

/**
 initializes mutex and thread related features
 **/
void co_init(int number_of_threads);

/**
 Defines the process CS[i=1 to n], where i is defined by
 the range (start and end attributes from ProcessInfo data structure).
 **/
void co_process(ProcessInfo_t* process_info);

/**
 This function defines a slice of work that will be executed
 concurrently. It's depicting the while(true) from parallel algorithms.
 **/
void* co_process_loop(void* process_info_param);

void co_join(int number_of_threads);

#endif /* ticket_h */

