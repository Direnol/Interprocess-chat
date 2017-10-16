#ifndef INTERPROCESS_CHAT_CLIENT_H
#define INTERPROCESS_CHAT_CLIENT_H

#include "sharedMemory.h"
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex;
typedef struct {
    void *seg;
    char myid;
} data;

int init(void **seg, int *id);

int send(record Rec, int id, char *msg, void *seg);

int recv(record *Rec, int id, void *seg, char *getmsg);

void disconnect(void *seg, int id);

void *read_msg(void *seg);

void *write_msg(void *seg);

#endif //INTERPROCESS_CHAT_CLIENT_H
