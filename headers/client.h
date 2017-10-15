#ifndef INTERPROCESS_CHAT_CLIENT_H
#define INTERPROCESS_CHAT_CLIENT_H

#include "sharedMemory.h"

int init(void **seg, int *id);

int send(record Rec, int id, char *msg, void *seg);

int recv(record *Rec, int id, void *seg);

void disconnect(void *seg, int id);

#endif //INTERPROCESS_CHAT_CLIENT_H
