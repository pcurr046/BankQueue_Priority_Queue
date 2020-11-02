//Name: Patricia Curry
//Class: CS10C
//Instructor: Dave Harden
//File name: Assignment10_2.cpp
/*Implement the event-driven simulation of a bank that this chapter described.
A queue of arrival events will represent the line of customers in the bank.
Maintain the arrival events and departure events in a priority queue, sorted by
 the time of the event. Use a the STL priority queue for the event list.
*/
#include <iostream>
#include <fstream>
#include <queue>

using namespace std;

class Event{
public:
    int customerID;
    int arrivalTime;
    int duration;
    bool arrival;

    Event(int arrivalTime, int duration, int customerID, bool arrival);
    friend bool operator < (const Event &event1, const Event &event2);

};

class Customer{
public:
    Customer(Event event) : customerID(event.customerID), duration(event
    .duration), arrivaltime(event.arrivalTime) {}

    int customerID;
    int arrivaltime;
    int duration;
};


bool operator < (const Event &event1, const Event &event2) {
    return event1.arrivalTime > event2.arrivalTime;
}

Event::Event(int arrTime, int dur, int custID, bool arrival) : arrivalTime
(arrTime), duration(dur), customerID(custID), arrival(arrival) {}

void simulate();

void processArival(Event event, priority_queue<Event> &pQueue, queue<Customer>
        &queue, bool &tellerAvailable, int currentTime);

void processDeparture(Event event, priority_queue<Event> &pQueue,
                      queue<Customer> &queue,  bool &tellerAvailable, int
                      currentTime );

int main() {
    simulate();
    return 0;
}

void simulate(){
    bool tellerAvailable;
    cout << "Simulation Begins" << endl;
    queue<Customer> bankQueue;
    priority_queue<Event> eventListPQueue;
    tellerAvailable = true;

    ifstream inFile;
    string filename = "in1.txt";
    inFile.open(filename);
    if(inFile.is_open()){

        int arrT;
        int dur;
        int custID = 0;
        while( inFile >> arrT && inFile >> dur){
            custID++;
            Event arrival(arrT, dur, custID, true);
            eventListPQueue.push(arrival);
        }

        while(!eventListPQueue.empty()){

            Event newEvent = eventListPQueue.top();

            int currentTime = newEvent.arrivalTime;
            if(newEvent.arrival){
                processArival(newEvent, eventListPQueue, bankQueue,
                        tellerAvailable, currentTime);
            }
            else{
                processDeparture(newEvent, eventListPQueue, bankQueue,
                        tellerAvailable, currentTime);
            }
        }
    }else{
        cout << "File didn't open properly" << endl;
    }

    inFile.close();

}

void processDeparture(Event event, priority_queue<Event> &pQueue,
                      queue<Customer> &queue, bool &tellerAvailable, int
                      currentTime) {

    pQueue.pop();
    if(!queue.empty()){
        Customer customer(queue.front());
        queue.pop();
        int departureTime = currentTime + event.duration;
        Event newDeparture(departureTime, 0, customer.customerID, false);
        pQueue.push(newDeparture);
        cout << "Processing customer #" << customer.customerID << " departure "
                                                                 "at "
                                                                "time: " <<
                                                                departureTime
                                                                << endl;
    }
    else{
        tellerAvailable = true;
    }
}

void processArival(Event event, priority_queue<Event> &pQueue,
        queue<Customer> &queue, bool &tellerAvailable, int currentTime) {

    pQueue.pop();

    Customer customer(event);

    if(queue.empty() && tellerAvailable){
        int departureTime = currentTime + event.duration;
        Event newDeparture(departureTime, 0, customer.customerID, false );
        pQueue.push(newDeparture);
        cout << "Processing customer #" << customer.customerID << " arrival time: " <<customer.arrivaltime << endl;
        tellerAvailable = false;
    }
    else{
        queue.push(customer);
    }
}
/*Simulation Begins
Processing customer #1 arrival at time: 1
Processing customer #2 arrival at time: 2
Processing customer #3 arrival at time: 4
Processing customer #1 departure at time: 6
Processing customer #2 departure at time: 11
Processing customer #3 departure at time: 16
Processing customer #4 arrival at time: 20
Processing customer #5 arrival at time: 22
Processing customer #6 arrival at time: 24
Processing customer #4 departure at time: 25
Processing customer #7 arrival at time: 26
Processing customer #8 arrival at time: 28
Processing customer #5 departure at time: 30
Processing customer #9 arrival at time: 30
Processing customer #6 departure at time: 35
Processing customer #7 departure at time: 40
Processing customer #8 departure at time: 45
Processing customer #9 departure at time: 50
Processing customer #10 arrival at time: 88
Processing customer #10 departure at time: 91
Final Statistics:
    Total number of people processed: 10
    Average amount of time spent waiting: 5.6*/