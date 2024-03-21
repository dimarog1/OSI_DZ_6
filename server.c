#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include "shared_memory.h" // Убедитесь, что этот файл находится в том же каталоге, что и server.c

int main() {
    // Создаем сегмент разделяемой памяти
    int shmid = shmget(SHM_ID, sizeof(struct shared_data), 0666|IPC_CREAT);
    if (shmid == -1) {
        perror("Не удалось создать разделяемую память");
        return -1;
    }

    // Присоединяем сегмент разделяемой памяти к адресному пространству процесса
    struct shared_data* data = (struct shared_data*) shmat(shmid, NULL, 0);
    if (data == (void*) -1) {
        perror("Shmat не удался");
        return -1;
    }

    // В бесконечном цикле проверяем наличие новых данных и выводим их
    while (!data->is_finished) {
        if (data->is_new_data) {
            printf("Получено число: %d\n", data->number);
            data->is_new_data = 0;
        }
    }

    // Отсоединяем сегмент разделяемой памяти от адресного пространства процесса
    if (shmdt(data) == -1) {
        perror("Shmdt не удался");
        return -1;
    }

    // Удаляем сегмент разделяемой памяти
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("Shmctl не удался");
        return -1;
    }

    return 0;
}