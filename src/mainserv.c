#include "../headers/serv.h"

int main()
{
    void *seg = NULL;
    int shmid;
    ServInit(&seg, &shmid);

    deleteServ(seg, shmid);
    return 0;
}