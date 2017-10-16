#include <signal.h>
#include "../headers/serv.h"

void *seg = NULL;
int shmid;

void sig(int num)
{
    printf("Exit\n");
    deleteServ(seg, shmid);
    printf("Server is delted\n");
    exit(0);
}

int main()
{
    signal(SIGINT, sig);

    ServInit(&seg, &shmid);
    int res;
    record Rec;
    char msg[64];
    while (1) {
        for (int i = 0; i < MAX_CLIENTS; ++i) {
            do {
                // lock
                res = ServRecv(&Rec, i, seg, msg);
                if (res == EXIT_SUCCESS) {
                    printf("From[%d] : To[%d] = [%s]\n", Rec.idFrom, Rec.idTo, msg);
                    do {
                        //lock
                        res = ServSend(Rec, msg, seg);
                        //unlock
                        if (res < 0) {
                            sprintf(msg, "This client is not found[%d]", Rec.idTo);
                            Rec.idTo = Rec.idFrom;
                            Rec.length = (char) (strlen(msg) + 1);
                        }
                    } while (res == EXIT_FAILURE);
                }
                //unlock
            } while (res == EXIT_FAILURE);
        }
    }

}