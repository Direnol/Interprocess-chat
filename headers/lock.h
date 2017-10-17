#ifndef INTERPROCESS_CHAT_LOCK_H
#define INTERPROCESS_CHAT_LOCK_H

#include "sharedMemory.h"
#include <sys/sem.h>

typedef union {
    int val;
    struct semid_ds *buf;
    unsigned short array [MAX_CLIENTS + 1];
} semun;

#define LOCK_MEMORY MAX_CLIENTS


void semInit(int *semid);

void semGet(int *semid);

int lock(int semid, int id);

int unlock(int semid, int id);

void semDel(int semid);


#endif //INTERPROCESS_CHAT_LOCK_H
