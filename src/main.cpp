#include "LiteSH.hpp"


int pid_SaveFile()
{
  ofstream pid_file("/run/LiteSH.pid");
  if (pid_file.is_open() == 0) return 1;

  pid_file << getpid() << endl;

  pid_file.close();
  return 0;
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

  pid_t server_pid = net_proc();
  if (pid_SaveFile() == 1)
    cout << "Unable to create pid file, restart the program as administrator" << endl << endl;

  char *error;
  void *handle = dlopen("/home/vladislav/OS/LiteSH/lib/libabout.so", RTLD_LAZY);
  if (!handle) {
    fputs (dlerror(), stderr);
    return 1;
  }

  typedef void (*p_func_commands_guide)();
  typedef void (*p_func_about)();
  p_func_commands_guide commands_guide = (p_func_commands_guide)dlsym(handle, "commands_guide");
  p_func_about about = (p_func_about)dlsym(handle, "about");

  if ((error = dlerror()) != NULL) {
    fprintf (stderr, "%s\n", error);
    return 1;
  }

  commands_guide();
  bool isExit = false;

  while (isExit != true)
  {
    int selection = 0;
    char *command = new char[100];

    cout << endl << endl;
    cout << "command to server: ";
    cin >> command;
    cout << endl << endl;

    cout << "--------------------------" << endl;
    selection = NetWorking_client(command);
    cout << "--------------------------" << endl << endl;

    switch (selection) {
      case 1: {
        char *ProcName = new char[100];
        cout << "process name: ";
        cin >> ProcName;

        if (CreateProcess(ProcName) == 1)
          cout << "failed to create process" << endl;

        delete [] ProcName;
        break;
      }

      case 2: {
        char *ProcName = new char[100];
        cout << "process name: ";
        cin >> ProcName;

        if (CreateDaemon(ProcName) == 1)
          cout << "failed to create daemon" << endl;

        delete [] ProcName;
        break;
      }

      case 3: {
        pid_t pid, signum;
        cout << "pid: ";
        cin >> pid;
        cout << "signal to send: ";
        cin >> signum;

        if (SendSignal(pid, signum) == 1)
          cout << "failed to send signal";

        break;
      }

      case 4: {
        int signum;
        cout << "signal to receive: ";
        cin >> signum;

        if (RecvSignal(signum) == 1)
          cout << "failed to receive signal";

        break;
      }

      case 5:
        CreateProcess("/home/vladislav/OS/FileManager/bin/File_Manager"); //поменять на свой
        break;

      case 6:
        about();
        break;

      case 7:
        isExit = true;
        break;
    }
  }

  SendSignal(server_pid, SIGKILL);  // завершаем работу сервера
  dlclose(handle);
  return 0;
}
