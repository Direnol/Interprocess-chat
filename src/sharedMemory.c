#include "../headers/sharedMemory.h"

key_t getKey()
{
    return ftok(PATH_KEY, PROJ_ID);
}

void *createMemory(key_t key, int *shmid)
{
    if ((*shmid = shmget(key, SHARED_MEMORY_SIZE, 0666 | IPC_CREAT)) < 0) {
        perror("Cannot create server");
        exit(EXIT_FAILURE);
    }
    void *shmem_segment = NULL;
    if ((shmem_segment = shmat(*shmid, NULL, 0)) == (void *) -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    return shmem_segment;
}

int cwriteTo(record Record, char *message, void *segment, int myid)
{
    char *msg = segment + myid * SIZE_ONE;
    if (msg[MSG_READY_PUT] == USED) return UNREADY;
    record *tmp = (record *) (msg + PART_FIRST);
    tmp->idFrom = Record.idFrom;
    tmp->idTo = Record.idTo;
    tmp->length = Record.length;
    for (int i = 0; i < tmp->length; ++i) {
        tmp->message[i] = message[i];
    }
    msg[MSG_READY_PUT] = USED;
    return READY;
}

int cread(record **Record, int myid, void *segment)
{
    char *msg = segment + myid * SIZE_ONE;
    if (msg[MSG_READY_GET] == UNUSED) return UNREADY;
    *Record = (record *) (msg + PART_SECOND);
    msg[MSG_READY_GET] = UNUSED;
    return READY;
}

int swriteTo(record *Record, int id, void *segment)
{
    char *cur = segment + id * SIZE_ONE;
    if (cur[USED_ID] == UNUSED) return UNUSED; // UNUSED
    if (cur[ID] != id) return UNUSED;
    if (cur[MSG_READY_GET] == USED) return UNREADY;
    record *tmp = (record *) (cur + PART_SECOND);
    tmp->idFrom = Record->idFrom;
    tmp->idTo = Record->idTo;
    tmp->length = Record->length;
    for (int i = 0; i < tmp->length; ++i) {
        tmp->message[i] = Record->message[i];
    }
    cur[MSG_READY_GET] = USED;
    return READY;
}

int sread(record **Record, int id, void *segment)
{
    char *cur = (char *) (segment) + id * SIZE_ONE;
    if (cur[USED_ID] == UNUSED) return UNUSED;
    if (cur[MSG_READY_PUT] == UNREADY) return UNREADY;
    *Record = (record *) (cur + PART_FIRST);
    cur[MSG_READY_PUT] = UNUSED;
    return READY;
}

void deleteMemory(void *segment, int shmid)
{
    if (shmdt(segment) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    shmctl(shmid, IPC_RMID, NULL);
}

void *getMemory(key_t key)
{
    int shmid;
    if ((shmid = shmget(key, SHARED_MEMORY_SIZE, 0666)) < 0) {
        perror("Server is off");
        exit(EXIT_FAILURE);
    }
    void *shmem_segment = NULL;
    if ((shmem_segment = shmat(shmid, NULL, 0)) == (void *) -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    return shmem_segment;
}


