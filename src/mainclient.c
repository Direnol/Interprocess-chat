#include <signal.h>
#include "../headers/client.h"

void sig(int num);

pthread_t writer, reader;
int myid;
void *seg = NULL;
int semid;

int main()
{
    signal(SIGINT, sig);
    if (init(&seg, &myid, &semid) == EXIT_FAILURE) {
        perror("Connect");
        disconnect(seg, myid, 0);
        return 1;
    }
    data passport = {seg, (char) myid, semid};
    printf("Connection with id[%d]\n", myid);
    pthread_create(&writer, NULL, (void *(*)(void *)) write_msg, &passport);
    pthread_create(&reader, NULL, (void *(*)(void *)) read_msg, &passport);
    pthread_join(writer, NULL);
    pthread_join(reader, NULL);
    return 0;
}

void sig(int num)
{
    pthread_cancel(writer);
    pthread_cancel(reader);
    printf("Exit\n");
    pthread_join(writer, NULL);
    pthread_join(reader, NULL);

    disconnect(seg, myid, semid);
    exit(0);
}
