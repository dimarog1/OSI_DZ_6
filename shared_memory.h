#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>

#define SHM_ID  0x777     // ключ разделяемой памяти
#define PERMS   0666      // права доступа

// коды сообщений
#define MSG_TYPE_EMPTY  0     // сообщение о завершении обмена при пустой строке
#define MSG_TYPE_STRING 1     // сообщение о передаче строки
#define MSG_TYPE_FINISH 2     // сообщение о том, что пора завершать обмен
#define MAX_STRING      120   // максимальная длина текстового сообщения

struct shared_data {
    int number; // Число, которое будет передаваться между клиентом и сервером
    int is_new_data; // Флаг, указывающий на то, что появились новые данные
    int is_finished; // Флаг, указывающий на то, что клиент завершил работу
};