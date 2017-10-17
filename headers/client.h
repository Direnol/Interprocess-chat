#ifndef INTERPROCESS_CHAT_CLIENT_H
#define INTERPROCESS_CHAT_CLIENT_H

#include "lock.h"
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex;
typedef struct {
    void *seg;
    char myid;
    int semid;
} data;

int init(void **seg, int *id, int *semid);

int send(record Rec, int id, char *msg, void *seg);

int recv(record *Rec, int id, void *seg, char *getmsg);

void disconnect(void *seg, int id, int semid);

void *read_msg(void *seg);

void *write_msg(void *seg);

#endif //INTERPROCESS_CHAT_CLIENT_H
