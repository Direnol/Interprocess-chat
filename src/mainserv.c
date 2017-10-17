#include <signal.h>
#include <unistd.h>
#include "../headers/serv.h"

void sig(int num);

void *seg = NULL;
int shmid;
int semid;

int main()
{
    signal(SIGINT, sig);
    ServInit(&seg, &shmid, &semid);
    int res;
    record *Rec = NULL;
    printf("Server is started\n");
    while (1) {
        for (int i = 0; i < MAX_CLIENTS; ++i) {
            do {
                lock(semid, i);
                res = ServRecv(&Rec, i, seg);
                if (res == EXIT_SUCCESS) {
                    printf("From[%d] : To[%d] = [%s]\n", Rec->idFrom, Rec->idTo, Rec->message);
                    do {
                        if (i != Rec->idTo)
                            lock(semid, Rec->idTo);
                        res = ServSend(Rec, seg);
                        if (i != Rec->idTo)
                            unlock(semid, Rec->idTo);
                        if (res < 0) {
                            sprintf(Rec->message, "This client is not found[%d]", Rec->idTo);
                            printf("id[%d] send to incorrect id[%d] : [%s]", i, Rec->idTo, Rec->message);
                            Rec->idTo = Rec->idFrom;
                            Rec->length = (char) (strlen(Rec->message) + 1);
                            res = EXIT_FAILURE;
                        }
                        sleep(1);
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