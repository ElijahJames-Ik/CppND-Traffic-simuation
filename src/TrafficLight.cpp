#include <iostream>
#include <random>
#include <functional>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */


template <typename T>
T MessageQueue<T>::receive()
{
    
    std::unique_lock<std::mutex> unique_lck(_mutex);
    _condVar.wait(unique_lck,[this]{return !_queue.empty();});
    T msg = std::move(_queue.back());
    _queue.pop_back();
    return msg;

}


template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    
    std::lock_guard<std::mutex> lck(_mutex);
    _queue.push_back(std::move(msg));
    _condVar.notify_one();
}


/* Implementation of class "TrafficLight" */


TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
   
    
    while(true)
    {
       TrafficLightPhase state =  _msgQueue.receive();
       if(state == TrafficLightPhase::green)
       {
           break;
       }
    }
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}


// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{

    std::cout << " 1Current state" << std::endl;

    std::uniform_int_distribution<int> distributor(4000,6000);
    std::random_device rd;
    std::mt19937 randEng(rd());
    
    int randomNum = distributor(randEng);
    auto startTime = std::chrono::high_resolution_clock::now();
    while(true)
    {
        auto timeNow = std::chrono::high_resolution_clock::now();
        int64_t elaspedTime = std::chrono::duration_cast<std::chrono::milliseconds>(timeNow - startTime).count();
        if(elaspedTime > randomNum)
        {
             auto state = _currentPhase == TrafficLightPhase::green ? TrafficLightPhase::red : TrafficLightPhase::green;
            _currentPhase = state;
            _msgQueue.send(std::move(state));
            startTime = std::chrono::high_resolution_clock::now();
            randomNum = distributor(randEng);
            
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        
    }
}

