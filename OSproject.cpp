#include <iostream>

#include <queue>

#include <thread>

#include <semaphore.h>

#include <chrono>

#include <random>

using namespace std;

sem_t runway, qvar;
queue < int > landingQ;
queue < int > departingQ;
int planesLandedNo = 0;
int planesDepartedNo = 0;

void planeCreation() {
  while (1) {
    	
    int random1 = rand() % 5;
    		
    this_thread::sleep_for(chrono::seconds(random1));
   
    int random2 = rand() % 2;
    
    if (random2 == 0) {
     
      sem_trywait( & qvar);
      landingQ.push(random1);

      
    } else {
     
      sem_trywait( & qvar);
      departingQ.push(random1);
      
    }
  }
}
void arrivalService() {
  while (1) { 		
    int random = rand() % 10 + 10; 		
    this_thread::sleep_for(chrono::seconds(random)); 
    if (sem_trywait( & runway) == 0 && !landingQ.empty()) {
      planesLandedNo++;
      int flight = landingQ.front();
      landingQ.pop();
      
      cout << "Flight# A" << flight << " arrived" << endl; 
      
      sem_post( & runway);
    }
  }
}
void departureService() {
  while (1) { 

    int random = rand() % 15 + 10; 		
    this_thread::sleep_for(chrono::seconds(random)); 	
    if (sem_trywait( & runway) == 0 && !departingQ.empty()) {
      planesDepartedNo++;
      int flight = departingQ.front();
      departingQ.pop();
      sem_post( & qvar);
      cout << "Flight# D" << flight << " departed" << endl; 
      
      sem_post( & runway);

    }
  }

}

int main() {
  	
  sem_init( & runway, 0, 1);
  sem_init( & qvar, 0, 1);
  	
  thread planeCreationThread(planeCreation);
  thread arrivalServiceThread(arrivalService);
  thread departureServiceThread(departureService);
  cout << "choose an option" << endl; 	
  cout << "1. Print statistics" << endl;
  cout << "2. Stop simulation" << endl; 	
  
  while (1) {

    int input;
    cin >> input; 	
    if (input == 1) {
      cout << "Planes Serviced:" << endl;
      cout << "----------------" << endl;
      cout << "Arrival Serviced: " << planesLandedNo << endl;
      cout << "Departure Serviced:: " << planesDepartedNo << endl;
      cout << "Planes waiting to land: " << landingQ.size() << endl;
      cout << "Planes waiting to depart: " << departingQ.size() << endl;

    } 		
    else if (input == 2) {
      		
      planeCreationThread.detach();

    }

  }

  	
  arrivalServiceThread.detach();
  departureServiceThread.detach();

  planeCreationThread.join();
  arrivalServiceThread.join();
  departureServiceThread.join(); 
  sem_destroy(&runway);
  sem_destroy(&qvar);

  		
  return 0;

}