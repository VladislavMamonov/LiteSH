#include "LiteSH.hpp"


int CreateProcess(char *ProcName)
{
  int rv;
  char *arg[] = {ProcName, 0};

  switch (fork()) {
    case -1:
      perror("fork");
      return 1;
    case 0:
      cout << "pid: " << getpid() << endl;
      cout << "ppid: " << getppid() << endl;
      execv(arg[0], arg);
    default:
      wait(&rv);
  }

  return 0;
}


int CreateDaemon(char *ProcName)
{
  int rv;
  char *arg[] = {ProcName, 0};

  switch (fork()) {
    case -1:
      perror("fork");
      return 1;
    case 0:
      setsid();
      cout << "pid: " << getpid() << endl;
      cout << "ppid: " << getppid() << endl;
      chdir("/");
      fclose(stdin);
      fclose(stdout);
      fclose(stderr);
      execv(arg[0], arg);
      //exit(0);
    default:
      wait(&rv);
  }

  return 0;
}


int SendMssg(const char *ProcName, const char *mssg)
{
  key_t ipckey;
  int mq_id;

  struct { long type; char text[100]; } mymsg;
  ipckey = ftok(ProcName, 42);
  cout << "my key is " << ipckey << endl;

  mq_id = msgget(ipckey, IPC_CREAT | 0666);
  cout << "message identifier is " << mq_id << endl;

  memset(mymsg.text, 0, 100);
  strcpy(mymsg.text, mssg);
  mymsg.type = 1;
  msgsnd(mq_id, &mymsg, sizeof(mymsg), 0);

  return 0;
}


int RecvMssg(const char *ProcName)
{
  key_t ipckey;
  int mq_id;

  struct { long type; char text[100]; } mymsg;
  int received;

  ipckey = ftok(ProcName, 42);
  cout << "My key is " << ipckey << endl;

  /* Подключение к очереди */
  mq_id = msgget(ipckey, 0);
  cout << "Message identifier is " << mq_id << endl;

  /* Считывание сообщения */
  received = msgrcv(mq_id, &mymsg, sizeof(mymsg), 0, 0);
  cout << mymsg.text << "(" << received << ")" << endl;

  return 1;
}


void sighandler(int signo)
{
  cout << "Signal received successfully" << endl;
}


int SendSignal(pid_t pid, int signum)
{
  if (kill(pid, signum) == -1)
    return 1;

  return 0;
}


int RecvSignal(int signum)
{
  if (signal(signum, sighandler) == SIG_ERR)
    return 1;

  return 0;
}


void NetWorking_server()
{
  struct sockaddr_in server, client;
  int sock = socket(AF_INET, SOCK_STREAM, 0); // создание сокета

  // структура для сервера
  server.sin_family = AF_INET;  // ipv4
  server.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
  server.sin_port = htons(2019); // порт сервера

  bind(sock, (struct sockaddr*)&server, sizeof(server));  // связка с сокетом
  listen(sock, 5);  // ожидаем очередь размером 5

  while (1) {
    char buf[100] = "";
    int newsock, size;
    socklen_t clnlen;

    newsock = accept(sock, (struct sockaddr*)&client, &clnlen);   // появление нового клиента
    cout << "new client: " << inet_ntoa(client.sin_addr) << endl;

    while ((size = recv(newsock, buf, sizeof(buf), 0)) != 0) {  // пока получаем от клиента
      if (strcmp(buf, "CreateProcess") == 0) {
        strcpy(buf, "1");
        send(newsock, buf, sizeof(buf), 0);
      } else if (strcmp(buf, "CreateDaemon") == 0) {
        strcpy(buf, "2");
        send(newsock, buf, sizeof(buf), 0);
      } else if (strcmp(buf, "SendSignal") == 0) {
        strcpy(buf, "3");
        send(newsock, buf, sizeof(buf), 0);
      } else if (strcmp(buf, "RecvSignal") == 0) {
        strcpy(buf, "4");
        send(newsock, buf, sizeof(buf), 0);
      } else if (strcmp(buf, "FileManager") == 0) {
        strcpy(buf, "5");
        send(newsock, buf, sizeof(buf), 0);
      } else if (strcmp(buf, "about") == 0) {
        strcpy(buf, "6");
        send(newsock, buf, sizeof(buf), 0);
      } else if (strcmp(buf, "exit") == 0) {
        strcpy(buf, "7");
        send(newsock, buf, sizeof(buf), 0);
      } else {
        strcpy(buf, "unknown command");
        send(newsock, buf, sizeof(buf), 0);
      }
    }
    close(newsock);
  }

  close(sock);
}


int NetWorking_client(char *command)
{
  // структура для сокета
  struct sockaddr_in server;
  int sock = socket(AF_INET, SOCK_STREAM, 0);

  server.sin_family = AF_INET;
  server.sin_port = htons(2019);
  server.sin_addr.s_addr = inet_addr("127.0.0.1");

  if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {   // соединяемся с сервером
    cout << "connection failed" << endl;
    return -1;
  }

  char buf[100] = "";
  send(sock, command, sizeof(buf), 0);
  recv(sock, buf, sizeof(buf), 0);
  cout << "received from server: " << buf << endl;

  close(sock);

  int selection = atoi(buf);
  return selection;
}


pid_t net_proc()
{
  pid_t pid;

  switch (pid = fork()) {
    case -1:
      perror("net fork");
      return -1;
    case 0:
      NetWorking_server();
  }

  return pid;
}
