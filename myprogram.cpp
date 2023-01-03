#include "Dish.h"
#include "Restaurant.h"
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <vector>

std::vector <Dish> dishes;
std::mutex scoreboard;

void Courier(Restaurant* restaurant) {
    for (;;) {
        std::this_thread::sleep_for(std::chrono::seconds(30));
            
        if (dishes.front().ready) {
            
            scoreboard.lock(); 
            std::cout << dishes.front().name << " dish delivery." << '\n';
            scoreboard.unlock();
                
            dishes.erase(dishes.begin());
            restaurant->deliveriesCount++;
        }
    }
}

void Waiting() {
    std::srand(std::time(nullptr));

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds((rand() % 11 + 5)));
        
        bool found = false;
        for(int i = 0; i < dishes.size() && !found; i++) 
            if (!dishes[i].ready) {
                dishes[i].ready = true;

                scoreboard.lock();
                std::cout << "Dish " << dishes[i].name << " ready.\n";
                scoreboard.unlock();

                found = true;
            } 
    }
}

int main() { 
    std::srand(std::time(nullptr));
    
    Restaurant* restaurant = new Restaurant;
    
    //-----------курьер------------------------
    std::thread delivery(Courier, restaurant);
    delivery.detach();
    //-----------курьер------------------------

    //-----------кухня-------------------------
    std::thread cooking(Waiting);
    cooking.detach(); 
    //-----------кухня-------------------------

    while (restaurant->deliveriesCount < 10) {
        Dish dish;
        dishes.push_back(dish);
        
        scoreboard.lock();
        std::cout << "Online order " << dishes.back().name << " dish." << '\n'; 
        scoreboard.unlock();
        
        std::this_thread::sleep_for(std::chrono::seconds((rand() % 6 + 5)));
    }
    delete restaurant; restaurant = nullptr;
}