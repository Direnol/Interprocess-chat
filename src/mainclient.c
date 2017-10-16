#include <signal.h>
#include "../headers/client.h"

pthread_t writer, reader;
int myid;
void *seg = NULL;

void sig(int num)
{
    pthread_cancel(writer);
    pthread_cancel(reader);
    printf("Exit\n");
    pthread_join(writer, NULL);
    pthread_join(reader, NULL);


    disconnect(seg, myid);
    exit(0);
}

int main()
{
    signal(SIGINT, sig);


    if (init(&seg, &myid) == EXIT_FAILURE) {
        perror("Connect");
        disconnect(seg, myid);
        return 1;
    }
    data passport = {seg, (char) myid};
    printf("Connection with id[%d]\n", myid);
    pthread_create(&writer, NULL, (void *(*)(void *)) write_msg, &passport);
    pthread_create(&reader, NULL, (void *(*)(void *)) read_msg, &passport);
    pthread_join(writer, NULL);
    pthread_join(reader, NULL);
    return 0;
}