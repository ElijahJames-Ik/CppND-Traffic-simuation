#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include "TrafficObject.h"

// forward declarations to avoid include cycle
class Vehicle;




template <class T>
class MessageQueue
{
public:
    void send(T&& t);
    T receive();
private:
    std::deque<T> _queue;
    std::condition_variable _condVar;
    std::mutex _mutex;
    
};


enum TrafficLightPhase{red, green};

class TrafficLight : public TrafficObject
{
public:
    // constructor / desctructor
    TrafficLight();
    

    // getters / setters
    TrafficLightPhase getCurrentPhase();


    // typical behaviour methods
    void waitForGreen();
    void simulate();
    


private:
    // typical behaviour methods
    void cycleThroughPhases();

    

    std::condition_variable _condition;
    std::mutex _mutex;
    TrafficLightPhase _currentPhase;
    MessageQueue<TrafficLightPhase> _msgQueue;
};

#endif