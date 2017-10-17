#include <signal.h>
#include <zconf.h>
#include "../headers/serv.h"
#include "../headers/sharedMemory.h"

void sig(int num);

void *seg = NULL;
int shmid;
int semid;

int main()
{
    signal(SIGINT, sig);
    ServInit(&seg, &shmid, &semid);
    int res;
    record Rec;
    char msg[64];
    while (1) {
        for (int i = 0; i < MAX_CLIENTS; ++i) {
            do {
                lock(semid, i);
                res = ServRecv(&Rec, i, seg, msg);
                if (res == EXIT_SUCCESS) {
                    printf("From[%d] : To[%d] = [%s]\n", Rec.idFrom, Rec.idTo, msg);
                    do {
                        lock(semid, Rec.idTo);
                        res = ServSend(Rec, msg, seg);
                        unlock(semid, Rec.idTo);
                        if (res < 0) {
                            sprintf(msg, "This client is not found[%d]", Rec.idTo);
                            Rec.idTo = Rec.idFrom;
                            Rec.length = (char) (strlen(msg) + 1);
                        }
                        usleep(50000);
                    } while (res == EXIT_FAILURE);
                }
                unlock(semid, i);
            } while (res == EXIT_FAILURE);
        }
    }

}

void sig(int num)
{
    printf("Exit\n");
    deleteServ(seg, shmid, semid);
    printf("Server is delted\n");
    exit(0);
}