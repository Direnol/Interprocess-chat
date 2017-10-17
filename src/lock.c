//
// Created by direnol on 15.10.17.
//

#include "../headers/lock.h"

struct sembuf _lock = {0, -1, IPC_NOWAIT};
struct sembuf _unlock = {0, 1, IPC_NOWAIT};

void semInit(int *semid)
{
    *semid = semget(getKey(), MAX_CLIENTS + 1, 0666 | IPC_CREAT);
    if (*semid < 0) {
        perror("Sem init");
        exit(1);
    }
    semun arg;
    arg.val = 1;
    for (int i = 0; i <= MAX_CLIENTS; ++i) {
        semctl(*semid, i, SETVAL, arg);
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
    semop(semid, &_lock, 1);
}

int unlock(int semid, int id)
{
    semop(semid, &_unlock, 1);
}

void semDel(int semid)
{
    semctl(semid, 0, IPC_RMID);
}


