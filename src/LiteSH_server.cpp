#include "LiteSH.hpp"


void NetWorking_server()
{
  struct sockaddr_in server, client;
  int sock = socket(AF_INET, SOCK_STREAM, 0); // создание сокета

  // структура для сервера
  server.sin_family = AF_INET;  // ipv4
  server.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
  server.sin_port = htons(2020); // порт сервера

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


int main(int argc, char const *argv[])
{
  if (argc == 2) {
    if (strcmp(argv[1], "-silent") == 0) {
      setsid();
      chdir("/");
      fclose(stdout);
      fclose(stderr);
    }
  }

  NetWorking_server();
  return 0;
}
