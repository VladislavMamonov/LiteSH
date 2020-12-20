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


int NetWorking_client(char *command)
{
  // структура для сокета
  struct sockaddr_in server;
  int sock = socket(AF_INET, SOCK_STREAM, 0);

  server.sin_family = AF_INET;
  server.sin_port = htons(2020);
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
