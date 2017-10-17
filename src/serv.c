#include "../headers/serv.h"

void ServInit(void **seg, int *shmid, int *semid)
{
    key_t key = getKey();
    semInit(semid);
    *seg = createMemory(key, shmid);
    lock(*semid, LOCK_MEMORY);
    memset(*seg, 0, SHARED_MEMORY_SIZE);
    unlock(*semid, LOCK_MEMORY);
}

int ServSend(record *Rec, void *seg)
{
    int id = Rec->idTo;
    char *client = (char *) (seg) + (id * SIZE_ONE);

    if (client[USED_ID] == UNUSED) return -1;
    if (swriteTo(Rec, id, seg) == UNREADY) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

int ServRecv(record **Rec, int id, void *seg)
{
    char *client = (char *) (seg) + (id * SIZE_ONE);
    if (client[USED_ID] == UNUSED) return -1;
    if (sread(Rec, id, seg) == UNREADY) return -1;//EXIT_FAILURE;
//    printf("%d %d", Rec->idFrom, Rec->idTo);
    return EXIT_SUCCESS;
}

void deleteServ(void *seg, int shmid, int semid)
{
    deleteMemory(seg, shmid);
    semDel(semid);
}
