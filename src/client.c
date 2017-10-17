#include <stdio.h>
#include "../headers/client.h"

int init(void **seg, int *id, int *semid)
{

    //lock
    key_t key = getKey();
    char *ptr = getMemory(key);
    semGet(semid);
    *id = -1;
    lock(*semid, LOCK_MEMORY);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (ptr[i * SIZE_ONE + USED_ID] == UNUSED) {
            memset(ptr + i * SIZE_ONE, 0, SIZE_ONE);
            ptr[i * SIZE_ONE + USED_ID] = USED;
            *id = i;
            ptr[i * SIZE_ONE + ID] = (char) i;
            break;
        }
    }
    unlock(*semid, LOCK_MEMORY);
    pthread_mutex_init(&mutex, NULL);
    if (*id < 0) return EXIT_FAILURE;
    *seg = ptr;
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

int recv(record **Rec, int id, void *seg)
{
    int res = cread(Rec, id, seg);
    if (res == UNREADY) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

void disconnect(void *seg, int id, int semid)
{
    char *cur = seg;
    cur = cur + id * SIZE_ONE;
    if (id >= 0) {
        lock(semid, id);
        cur[USED_ID] = 0;
        memset(cur, 0, SIZE_ONE);
        unlock(semid, id);
    }

    if (shmdt(seg) == -1) {
        perror("shmdt:");
        exit(EXIT_FAILURE);
    }
    pthread_mutex_destroy(&mutex);
}

void *read_msg(void *arg)
{
    record *Record = NULL;
    data *p = arg;
    char *seg = p->seg;
    int myid = p->myid;
    int semid = p->semid;
    int res;
    while (1) {
        lock(semid, p->myid);
        res = recv(&Record, myid, seg);
        unlock(semid, p->myid);
        if (res == EXIT_SUCCESS) {
            pthread_mutex_lock(&mutex);
            printf("From[%d] : [%s]\n", Record->idFrom, Record->message);
            pthread_mutex_unlock(&mutex);
        }
        sleep(1);
    }
}

void *write_msg(void *arg)
{
    record Record;
    data *passport = arg;
    Record.idFrom = passport->myid;
    int semid = passport->semid;
    char message[ENABLE_MEMORY];
    char *seg = passport->seg;
    int res;
    int i;
    char c;
    while (1) {
        i = 0;
        pthread_mutex_lock(&mutex);
        printf("\nWhom do you want to write?\n");
        pthread_mutex_unlock(&mutex);
        scanf("%d", &res);
        if (res == passport->myid) {
            pthread_mutex_lock(&mutex);
            printf("Error id! You cann't send messange yourself\n");
            pthread_mutex_unlock(&mutex);
            continue;
        }
        Record.idTo = (char) res;
        getchar();
        pthread_mutex_lock(&mutex);
        printf("\nWhat do you want to send?\n");
        pthread_mutex_unlock(&mutex);
        while ((c = (char) getchar()) != '\n') {
            if (i == (ENABLE_MEMORY - MSG_SIZE)) {
                pthread_mutex_lock(&mutex);
                printf("\nYour message is too long. We send current message without the last symbol\n");
                pthread_mutex_unlock(&mutex);
                break;
            }
            message[i++] = c;
        }
        message[i] = '\0';
        Record.length = (char) (strlen(message) + 1);
        lock(semid, passport->myid);
        res = send(Record, Record.idFrom, message, seg);
        unlock(semid, passport->myid);
        while (res == EXIT_FAILURE) {
            lock(semid, passport->myid);
            res = send(Record, Record.idTo, message, seg);
            unlock(semid, passport->myid);
            sleep(1);
        }
    }
}


