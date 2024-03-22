#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "shared_memory.h"

int shmid;
struct shared_data* data;

void handle_alarm(int sig) {
    exit(0); // Завершение работы процесса при получении сигнала от таймера
}

int main(int argc, char *argv[]) {
    // Создание сегмента разделяемой памяти
    shmid = shmget(SHM_ID, sizeof(struct shared_data), 0666|IPC_CREAT);
    if (shmid == -1) {
        perror("Не удалось создать разделяемую память");
        return -1;
    }

    // Прикрепление разделяемой памяти к адресному пространству процесса
    data = (struct shared_data*) shmat(shmid, NULL, 0);
    if (data == (void*) -1) {
        perror("Shmat не удался");
        return -1;
    }

    // Установка таймера, если был передан соответствующий аргумент командной строки
    if (argc > 1 && strcmp(argv[1], "timer") == 0) {
        signal(SIGALRM, handle_alarm);
        alarm(10);
    }

    // Чтение чисел из разделяемой памяти, пока клиент не завершит работу
    while (!data->is_finished) {
        if (data->is_new_data) {
            printf("Получено число: %d\n", data->number); // Вывод числа
            data->is_new_data = 0; // Сброс флага, что данные прочитаны
        }
    }

    // Отсоединение от разделяемой памяти
    if (shmdt(data) == -1) {
        perror("Shmdt не удался");
        return -1;
    }

    // Удаление сегмента разделяемой памяти
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("Shmctl не удался");
        return -1;
    }

    return 0;
}