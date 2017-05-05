#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <errno.h>
#include <sys/types.h>
#include <time.h>
#include <signal.h>
void usage ();
using namespace std;
int main (int argc, char *argv[]){
  
    //Variablen
    int hour, min, sec, whole_time,start,end,status;

    int father_pid = getppid();

	//Überprüft ob Argumente vorhanden sind 
    if (argc < 2){
    	    usage(); // Aufruf der Funktion
	    exit (EXIT_FAILURE);// Notwendig da sonst Segmentation fault (Beendet ein Process wenn nicht terminiert werden kann)
    }else{

    //speichern eines vorhanden Parameters
    string param1 = argv[1] ;

    //Usage aufruf durch Parametereingabe
    if (param1 == "-h" || param1 == "-help" ){
	    usage();
	    exit (EXIT_SUCCESS);//terminiert einen erfolgreichen Prozess um eine weiter Ausführung des programms zu verhindern 
    }else{

   
	//Elternprozess wird kopiert und als Kindprozess angelegt
	  pid_t processID; // erstellt eine processID variable

	  // prüft ob ein Process kopiert wurde
	  switch( processID = fork()){
	    case -1:
	      cout << "fork konnte nicht durchgeführt werden" << endl;
	      exit (EXIT_FAILURE);
	      break;
	    case 0:
	      //ein Systemcall wird aufgerufen
	      execvp(argv[1], &argv[1]);
              break;
	    default: 	
	  
	  //speichern der aktuellen Zeit beim Start von Programm A
	  start=time(NULL);
	  //WAIT wartet auf status
          wait(&status);
	  //speichert der aktuellen Zeit beim beenden von Programm B
          end=time(NULL);
	  
	  //Kindprocess wird ausgeführt
	  if(status != 0){
                cout << argv[1] << "fork konnte nicht durchgeführt werden"<< endl;
		//Gibt einen Wert zurück --> Über den Status
                cout << "Status= " << status << endl;
		exit (EXIT_FAILURE);
            }
            else{
		    
	      cout << " " << endl;
	      cout << argv[1] << " executed!" << endl;
	    }

	    //PID-Nummern
	    cout <<"PID-parent:                 " << father_pid << endl; //getppid für Vater
	    cout <<"PID-child:                  " << getpid() << endl; // getpid für Kind

	    //bereichung der Dauer
            whole_time=(end-start);
	    
	    //berechnen der Stunden
	    hour = whole_time/3600;
	    //berechnen der Minuten
            min  = (whole_time%3600)/60;
	    //berechnen der Sekunden
            sec  = whole_time%60;
	    //Die Dauer wird Ausgegeben
	    cout << "RunTime in h:min:sec           " << hour <<":" << min <<":" << sec << endl;


	    //SatusNummer	   
	    cout <<"StatusNumber_child"<<argv[1]<<":     "<<status <<endl;


    break;
	  }
    }

    
    }

}
void usage (){
cout << "Usage of programmA:"<< endl;
cout << "programmA -h         usage of the programm"<< endl;
cout << "programmA -help      usage of the programm"<< endl;
cout << "programmA var1 var2  execute and check runtime of a second Programm with parameter var2"<< endl;
cout << "                     Example programmA programmB Hello"<<endl;

}
