#ifndef INTERPROCESS_CHAT_SERV_H
#define INTERPROCESS_CHAT_SERV_H

#include "sharedMemory.h"

void ServInit(void **seg, int *shmid);

int ServSend(record Rec, char *msg, void *seg);

int ServRecv(record *Rec, int id, void *seg);

void deleteServ(void *seg, int shmid);

#endif //INTERPROCESS_CHAT_SERV_H
