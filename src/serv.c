#include "../headers/serv.h"

void ServInit(void **seg, int *shmid)
{
    key_t key = getKey();
    *seg = createMemory(key, shmid);
    memset(*seg, 0, SHARED_MEMORY_SIZE);
}

int ServSend(record Rec, char *msg, void *seg)
{
    int id = Rec.idTo;
    char *client = (char *) (seg) + (id * SIZE_ONE);

    if (client[USED_ID] == UNUSED) return -1;
    if (swriteTo(Rec, id, seg, msg) == UNREADY) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

int ServRecv(record *Rec, int id, void *seg, char *msg)
{
    char *client = (char *) (seg) + (id * SIZE_ONE);
    if (client[USED_ID] == UNUSED) return -1;
    if (sread(Rec, id, seg, msg) == UNREADY) return -1;//EXIT_FAILURE;
//    printf("%d %d", Rec->idFrom, Rec->idTo);
    return EXIT_SUCCESS;
}

void deleteServ(void *seg, int shmid)
{
    deleteMemory(seg, shmid);
}
