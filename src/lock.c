//
// Created by direnol on 15.10.17.
//

#include "../headers/lock.h"

struct sembuf _lock = {0, -1, 0};
struct sembuf _unlock = {0, 1, 0};
semun arg;

void semInit(int *semid)
{
    *semid = semget(getKey(), MAX_CLIENTS + 1, 0666 | IPC_CREAT);
    if (*semid < 0) {
        perror("Sem init");
        exit(1);
    }

    arg.val = 1;
    for (int i = 0; i <= MAX_CLIENTS; ++i) {
        semctl(*semid, i, SETVAL, arg.val);
    }
}

void semGet(int *semid)
{
    *semid = semget(getKey(), MAX_CLIENTS + 1, 0666);
    if (*semid < 0) {
        perror("Connect to sem");
        exit(1);
    }
}

int lock(int semid, int id)
{
    _lock.sem_num = (unsigned short) id;
    semop(semid, &_lock, 1);
}

int unlock(int semid, int id)
{
    _unlock.sem_num = (unsigned short) id;
    semop(semid, &_unlock, 1);
}

void semDel(int semid)
{
    for (int i = 0; i <= MAX_CLIENTS; ++i) semctl(semid, i, IPC_RMID);
}


