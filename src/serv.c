#include "../headers/serv.h"

void ServInit(void **seg, int *shmid)
{
    key_t key = getKey();
    *seg = createMemory(key, shmid);
    char *tmp = *seg;
    for (int i = 0; i < SHARED_MEMORY_SIZE; ++i) tmp[i] = 0;
}

int ServSend(record Rec, char *msg, void *seg)
{
    int id = Rec.idTo;
    char *client = (char *) (seg) + (id * SIZE_ONE);

    if (client[USED_ID] == UNUSED) return -1;
    if (swriteTo(Rec, id, seg) == UNREADY) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

int ServRecv(record *Rec, int id, void *seg)
{
    char *client = (char *) (seg) + (id * SIZE_ONE);
    if (client[USED_ID] == UNUSED) return -1;
    if (sread(Rec, id, seg) == UNREADY) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

void deleteServ(void *seg, int shmid)
{
    deleteMemory(seg, shmid);
}
