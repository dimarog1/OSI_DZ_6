#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include "shared_memory.h"

int main(int argc, char *argv[]) {
    srand(time(NULL)); // Инициализация генератора случайных чисел

    // Получение доступа к разделяемой памяти
    int shmid = shmget(SHM_ID, sizeof(struct shared_data), 0666);
    if (shmid == -1) {
        perror("Не удалось получить доступ к разделяемой памяти");
        return -1;
    }

    // Прикрепление разделяемой памяти к адресному пространству процесса
    struct shared_data* data = (struct shared_data*) shmat(shmid, NULL, 0);
    if (data == (void*) -1) {
        perror("Shmat не удался");
        return -1;
    }

    // Генерация и запись 10 случайных чисел в разделяемую память
    for (int i = 0; i < 10; ++i) {
        while (data->is_new_data) { } // Ожидание, пока сервер прочитает предыдущее число

        data->number = random() % 1000; // Генерация случайного числа
        data->is_new_data = 1; // Установка флага, что появились новые данные
    }

    data->is_finished = 1; // Установка флага, что клиент завершил работу

    // Отсоединение от разделяемой памяти
    if (shmdt(data) == -1) {
        perror("Shmdt не удался");
        return -1;
    }

    return 0;
}