#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "shared_memory.h" // Убедитесь, что этот файл находится в том же каталоге, что и client.c

int main() {
    // Инициализируем генератор случайных чисел
    srand(time(NULL));

    // Получаем доступ к сегменту разделяемой памяти
    int shmid = shmget(SHM_ID, sizeof(struct shared_data), 0666);
    if (shmid == -1) {
        perror("Не удалось получить доступ к разделяемой памяти");
        return -1;
    }

    // Присоединяем сегмент разделяемой памяти к адресному пространству процесса
    struct shared_data* data = (struct shared_data*) shmat(shmid, NULL, 0);
    if (data == (void*) -1) {
        perror("Shmat не удался");
        return -1;
    }

    // В цикле генерируем 10 случайных чисел и передаем их серверу
    for (int i = 0; i < 10; ++i) {
        while (data->is_new_data) { } // ждем, пока сервер обработает предыдущее число

        data->number = random() % 1000; // генерируем случайное число
        data->is_new_data = 1; // устанавливаем флаг, что появились новые данные
    }

    data->is_finished = 1; // устанавливаем флаг, что клиент завершил работу

    // Отсоединяем сегмент разделяемой памяти от адресного пространства процесса
    if (shmdt(data) == -1) {
        perror("Shmdt не удался");
        return -1;
    }

    return 0;
}