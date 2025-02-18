#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>

sem_t barman;
sem_t tobacco, paper, matches;

void smoker_tobacco() {
    while (1) {
        sem_wait(&tobacco);
        printf("TOBACCO\n");
        sleep(1);
        sem_post(&barman);
    }
}

void smoker_paper() {
    while (1) {
        sem_wait(&paper);
        printf("PAPER\n");
        sleep(1);
        sem_post(&barman);
    }
}

void smoker_matches() {
    while (1) {
        sem_wait(&matches);
        printf("MATCHES\n");
        sleep(1);
        sem_post(&barman);
    }
}

void barman_func() {
    char input[10];
    while (scanf("%s", input) != EOF) {
        sem_wait(&barman);
        if (strcmp(input, "tobacco") == 0) sem_post(&tobacco);
        else if (strcmp(input, "paper") == 0) sem_post(&paper);
        else if (strcmp(input, "matches") == 0) sem_post(&matches);
    }
    kill(0, SIGTERM);
}

int main() {
    sem_init(&barman, 0, 1);
    sem_init(&tobacco, 0, 0);
    sem_init(&paper, 0, 0);
    sem_init(&matches, 0, 0);

    pthread_t smokers[3], barman_thread;
    pthread_create(&smokers[0], NULL, (void *)smoker_tobacco, NULL);
    pthread_create(&smokers[1], NULL, (void *)smoker_paper, NULL);
    pthread_create(&smokers[2], NULL, (void *)smoker_matches, NULL);
    pthread_create(&barman_thread, NULL, (void *)barman_func, NULL);

    pthread_join(barman_thread, NULL);
    return 0;
}
