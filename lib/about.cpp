#include <iostream>
#include <fstream>

using namespace std;

extern "C" {

  void about()
  {
    cout << "\t\t**********LiteSH**********" << endl;
    cout << endl;
    cout << "Using this utility you can easily create processes and daemons," << endl;
    cout << "receive signals from external programs and the operating system" << endl;
    cout << "and also send signals to other processes" << endl << endl;
    cout <<  "Authors: Semyon Karulev, Vladislav Mamonov, Roman Suksin" << endl << endl;

    ifstream signals("../bin/signals.txt");
    string SigName;

    cout << "Available signal numbers: " << endl;
    cout << "---------------------------" << endl;

    for (int i = 1; getline(signals, SigName); i++) {
      cout << "\E[36m" << i << " - ";
      cout << "\E[39m";
      cout << SigName << endl;
    }

    signals.close();
  }


  void commands_guide()
  {
    cout << "Print 'CreateProcess' to create process" << endl;
    cout << "Print 'CreateDaemon' to create daemon" << endl;
    cout << "Print 'SendSignal' to send signal to the process" << endl;
    cout << "Print 'RecvSignal' to receive signal" << endl;
    cout << "Print 'FileManager' to start FileManager" << endl;
    cout << "Print 'about' to learn about project" << endl;
    cout << "Print 'exit' to close program" << endl << endl;
  }

}


int main()
{
  return 0;
}
