# Рогатнев Дмитрий Александрович БПИ229

## Запуск программы

Для компиляции программы выполните следующие команды:

```bash
gcc server.c -o server
gcc client.c -o client
```

Далее запустите сервер и клиент в разных терминалах:


Для сервера:

```bash
./server
```

Для клиента:

```bash
./client
```

## Структура данных

Структура `shared_data`, которая хранится в разделяемой памяти, содержит следующие поля:

- `number`: число, которое передается от клиента к серверу.
- `is_new_data`: флаг, указывающий на то, что клиент записал новое число в разделяемую память.
- `is_finished`: флаг, указывающий на то, что клиент завершил генерацию чисел.

## Работа программы

Клиент генерирует 10 случайных чисел и записывает их по одному в разделяемую память, устанавливая флаг `is_new_data` в 1 
после каждой записи. После записи всех чисел клиент устанавливает флаг `is_finished` в 1 и завершает свою работу.

Сервер в бесконечном цикле проверяет флаг `is_new_data`. Если этот флаг установлен в 1, сервер считывает число из разделяемой 
памяти и выводит его, затем сбрасывает флаг `is_new_data` в 0. Если сервер обнаруживает, что флаг `is_finished` установлен в
1, он завершает свой цикл и удаляет сегмент разделяемой памяти.

Таким образом, корректное завершение работы программы достигается путем установки флага `is_finished` в 1 со стороны 
клиента и проверки этого флага со стороны сервера. После завершения работы сервера сегмент разделяемой памяти удаляется,
что предотвращает утечку памяти.

## Дополнительные варианты завершения работы

### Вариант 1: Использование сигналов

В этом варианте клиент отправляет сигнал серверу, когда он завершает генерацию чисел. Сервер, получив сигнал, завершает 
свою работу и удаляет сегмент разделяемой памяти. Это обеспечивает корректное завершение работы обоих процессов.

### Вариант 2: Использование тайм-аута

В этом варианте сервер автоматически завершает работу через определенное время после последнего полученного числа. Это 
может быть полезно, если клиент по какой-то причине не смог установить флаг `is_finished` в 1. После завершения работы 
сервер удаляет сегмент разделяемой памяти.