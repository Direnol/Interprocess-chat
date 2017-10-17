#ifndef INTERPROCESS_CHAT_SHAREDMEMORY_H
#define INTERPROCESS_CHAT_SHAREDMEMORY_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>
#include <memory.h>

typedef struct {
    char idTo;
    char idFrom;
    char length;
    char message[0];
} record;

#define PROJ_ID 1997
#define PATH_KEY "/"
#define SHARED_MEMORY_SIZE (4096)
#define SIZE_ONE 128
#define ENABLE_MEMORY (SIZE_ONE - 4 / 2)
#define PART_FIRST 4
#define PART_SECOND ((SIZE_ONE + 4) / 2)
#define MAX_CLIENTS ((SHARED_MEMORY_SIZE) / SIZE_ONE)
#define MSG_SIZE sizeof(record)
#define READY 1
#define UNREADY 0

#define USED_ID 0
#define ID 1
#define MSG_READY_PUT 2
#define MSG_READY_GET 3

#define UNUSED 0
#define USED 1

key_t getKey();

void *createMemory(key_t key, int *shmid);

void *getMemory(key_t key);

int cwriteTo(record Record, char *message, void *segment, int myid);

int cread(record **Record, int myid, void *segment);

int swriteTo(record *Record, int id, void *segment);

int sread(record **Record, int id, void *segment);

void deleteMemory(void *segment, int shmid);


#endif //INTERPROCESS_CHAT_SHAREDMEMORY_H
