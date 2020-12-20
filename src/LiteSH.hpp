#pragma once
#include <iostream>
#include <cstring>
#include <fstream>
#include <arpa/inet.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <dlfcn.h>
#include <unistd.h>

using namespace std;


int CreateProcess(char *ProcName);
int CreateDaemon(char *ProcName);
int SendMssg(const char *ProcName, const char *mssg);
int RecvMssg(const char *ProcName);

void sighandler();
int SendSignal(pid_t pid, int signum);
int RecvSignal(int signum);

int NetWorking_client(char *command);
void NetWorking_server();
pid_t net_proc();
