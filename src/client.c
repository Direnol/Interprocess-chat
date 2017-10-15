#include "../headers/client.h"

int init(void **seg, int *id)
{
    //lock
    key_t key = getKey();
    char *ptr = getMemory(key);
    *id = -1;
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (ptr[i * SIZE_ONE + USED_ID] == UNUSED) {
            ptr[i * SIZE_ONE + USED_ID] = USED;
            *id = i;
            ptr[i * SIZE_ONE + ID] = (char) i;
            break;
        }
    }
    //unlock
    if (*id < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

int send(record Rec, int id, char *msg, void *seg)
{
    //lock
    int res = cwriteTo(Rec, msg, seg, id);
    //unlock
    if (res == UNREADY) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

int recv(record *Rec, int id, void *seg)
{
    // lock
    int res = cread(Rec, id, seg);
    //unlock
    if (res == UNREADY) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

void disconnect(void *seg, int id)
{
    char *cur = seg;
    cur = cur + id * SIZE_ONE;
    cur[USED_ID] = 0;

    if (shmdt(seg) == -1) {
        perror("shmdt:");
        exit(EXIT_FAILURE);
    }
}


