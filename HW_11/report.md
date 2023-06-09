# Задание

Необходимо разработать клиент-серверное приложение, использующее TCP/IP и реализующее передачу сообщений от клиента №1 серверу, который перенаправляет их клиенту №2. Клиент №1 передает сообщения серверу, а сервер перенаправляет их клиенту №2. Приложение должно поддерживать передачу нескольких сообщений и завершаться при получении сообщения "The End". Приложение должно работать в конфигурациях от одного до трех компьютеров с различными комбинациями размещения клиентов и сервера.

## Описание работы программы

### Программа-сервер (server.c)

- Создает серверный сокет и ожидает входящие соединения от клиентов.
- После установления соединения с клиентом №1 и клиентом №2, начинает основной цикл обработки.
- В цикле сервер читает данные из сокета клиента №1 и пересылает их в сокет клиента №2.
- Если получено сообщение "The End" или происходит ошибка при чтении данных, сервер отправляет сообщение "The End" клиенту №2 и выходит из цикла.
- Закрывает сокеты клиентов и серверный сокет.

### Программа-клиент (client.c)

- Создает клиентский сокет и устанавливает соединение с сервером.
- Если клиент является отправителем (указана роль "sender"), вводит сообщения с клавиатуры и отправляет их на сервер.
- Если клиент является получателем (указана роль "receiver"), ожидает получение сообщений от сервера и выводит их на экран.
- Если получено сообщение "The End" или происходит ошибка при чтении данных, клиент закрывает соединение и завершает работу.

## Запуск программы

1. Компилируйте программы `server.c` и `client.c` следующим образом:

   ```
   gcc -o server server.c
   gcc -o client client.c
   ```

2. Запустите сервер, указав IP-адрес и порт:

   ```
   ./server 127.0.0.1 8080
   ```

3. Запустите клиент №1 в другом терминале, указав IP-адрес и порт сервера, а также роль "sender":

   ```
   ./client 127.0.0.1 8080 sender
   ```

4. Введите сообщения для отправки на сервер. Для завершения передачи введите "The End".

5. Запустите клиент №2 в третьем терминале, указав IP-адрес и порт сервера, а также роль "receiver":

   ```
   ./client 127.0.0.1 8080 receiver
   ```

6. Клиент №2 будет получать сообщения, перенаправленные сервером, и выводить их на экран. При получении сообщения "The End" клиент завершит работу.

7. Завершите выполнение программы, введя "The End" в клиенте №1 или нажав Ctrl+C в терминале сервера или клиента №2.

Примечание: При необходимости вы можете запустить клиент №1 и клиент №2 на разных компьютерах, указав соответствующие IP-адреса и порты сервера при запуске клиентов.