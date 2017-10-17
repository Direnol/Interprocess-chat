#ifndef INTERPROCESS_CHAT_SERV_H
#define INTERPROCESS_CHAT_SERV_H

#include "lock.h"

void ServInit(void **seg, int *shmid, int *semid);

int ServSend(record *Rec, void *seg);

int ServRecv(record **Rec, int id, void *seg);

void deleteServ(void *seg, int shmid, int semid);

#endif //INTERPROCESS_CHAT_SERV_H
