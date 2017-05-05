#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <errno.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>

void usage ();
void waitfor(int);
void terminate(int);

//Klassenvariablen
pid_t child;
pid_t watchdog;

using namespace std;
int main (int argc, char *argv[]){
    
    //Variablen
    int hour, min, sec, duration, start, fin, status, calling;
    
    //Wenn keine Parameter dann Usage
    if (argc < 2){
        // Aufruf der Funktion
        usage();
        // Notwendig da sonst Segmentation fault (Beendet ein Process wenn nicht terminiert werden kann)
        exit (EXIT_FAILURE);
    }
    //speichern eines vorhanden Parameters
    string param1 = argv[1] ;
    
    //Usage aufruf durch Parametereingabe
    if (param1 == "-h" || param1 == "-help" ){
        usage();
        //terminiert einen erfolgreichen Prozess um eine weiter Ausführung des programms zu verhindern
        exit (EXIT_SUCCESS);
    }
    
   // prüft ob ein Process durch fork kopiert wurde
    //switch1 start
    switch( child = fork()){
        case -1:
            cout << "fork konnte nicht durchgeführt werden" << endl;
            //terminiert einen erfolgreichen Prozess um eine weiter Ausführung des programmes zu verhindern
            exit (EXIT_FAILURE);
            break;
        case 0:
            //ein Systemcall wird aufgerufen und somit wird Kind zu einem Prozess
            execvp(argv[1], &argv[1]);
            break;
            
        default:
            
            //Prozess für Watchdogs erstellen
            //switch2 start
            switch(watchdog = fork()){
                case -1:
                    cout << "fork konnte nicht durchgeführt werden" << endl;
                    exit (EXIT_FAILURE);
                    break;
                case 0:
                    
                    //Dauerhaftes Abfangen von Kind1
                    while(true){
                        //Signal ankündigung was bei einem Signalaufruf gemacht wird
                        //SIGALRM gibt ein Signal aus das eine Methode aufruft
                        signal (SIGALRM, waitfor);
                        //nach 3 sek. wird ein Signal aufgerufen
                        alarm(6);
                        pause();// Process paussiert
                        signal(SIGALRM, terminate);
                        alarm(3);
                        
                        //beliebige Eingabe
                        calling = getchar();
                        //Prüft ob eingabe Leer ist
                        if(calling != 0)
                        {
                            //Process von Kind wird vortgesetzt
                            kill(child, SIGCONT);
                            //Programm läuft bis zum ende durch
                            alarm(0);
                            cout << "Programm still running"<< endl;
                        }
                    }
                    
                    
                    
                    break;
                    
                default:
                    
                    //speichern der aktuellen Zeit beim Start von Programm A
                    start=time(NULL);
                    
                    //WAIT wartet auf status
                    wait(&status);
                    
                    //speichert der aktuellen Zeit beim beenden von Programm A
                    fin=time(NULL);
                    
                    //Kindprocess wird ausgeführt
                    if(status != 0){
                        cout << argv[1] << " konnte nicht ausgeführt weren "<< endl;
                        //Gibt einen Wert zurück --> Über den Status
                        cout << "Status= " << status << endl;
                        exit (EXIT_FAILURE);
                    }
                    else{
                        
                        cout << " " << endl;
                        cout << argv[1] << " executed!" << endl;
                    }
                    
                    //PID-Nummern
                    cout <<"PID-parent:                 " << getppid()<< endl; //getppid für Vater
                    cout <<"PID-child:                  " << getpid() << endl; // getpid für Kind
           
                    //bereichung der Laufzeit
                    duration=(fin-start);
                    
                    //berechnen der Stunden
                    hour=duration/3600;
                    
                    //berechnen der Minuten
                    min=(duration%3600)/60;
                    
                    //berechnen der Sekunden
                    sec=duration%60;
                    
                    //Die Dauer wird Ausgegeben
                    cout << "Dauer in Stunden:Minuten:Sekunden: " << hour <<":" << min <<":" << sec << endl;
                    
                    //SatusNummer
                    cout <<"Status_Number_Child: "<< status <<endl;
                    
                    break;
                    
                    //switch2 ende
            }
            //switch1 ende
    }
    //beendet den Process
    kill(watchdog, SIGKILL);
    
}

void usage (){
    cout << "Usage of programmA:"<< endl;
    cout << "programmA -h         usage of the programm"<< endl;
    cout << "programmA -help      usage of the programm"<< endl;
    cout << "programmA var1 var2  execute and check runtime of a second Programm with parameter var2"<< endl;
    cout << "                     Example programmA programmB Hello"<<endl;
    cout << "                     The programm ask every 3 seconds for a button action. If there is no action the programm will be terminated" << endl;
    
}

void waitfor(int sigNr)
{
    //SIGSTOP hält den Process an (keine terminierung)
    kill(child, SIGSTOP);
    cout << "Please press a button between 3 seconds or the process will be killed" << endl;
    
}
void terminate(int sigNr)
{
    cout <<"Programm will be terminated" <<endl;
    // SIGKILL beendet den Process umgehend
    kill(child, SIGKILL);
    kill(watchdog, SIGKILL);
    
}
