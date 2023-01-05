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
    while (restaurant->deliveriesCount < 10) {
        std::this_thread::sleep_for(std::chrono::seconds(30));
        
        scoreboard.lock();       
        if (dishes.front().ready) {
            std::cout << dishes.front().name << " dish delivery." << '\n';
            dishes.erase(dishes.begin());
            restaurant->deliveriesCount++;
        }
        scoreboard.unlock();
    }
}

void Waiting(Restaurant* restaurant) {
    std::srand(std::time(nullptr));

    while (restaurant->deliveriesCount < 10) {
        std::this_thread::sleep_for(std::chrono::seconds((rand() % 11 + 5)));
        
        bool found = false;
        
        scoreboard.lock();
        for(int i = 0; i < dishes.size() && !found; i++) 
            if (!dishes[i].ready) {
                dishes[i].ready = true;
                std::cout << "Dish " << dishes[i].name << " ready.\n";
                found = true;
            } 
        scoreboard.unlock();
    }
}

int main() { 
    std::srand(std::time(nullptr));
    
    Restaurant* restaurant = new Restaurant;
    
    std::thread cooking(Waiting, restaurant);
    std::thread delivery(Courier, restaurant);                                   
    cooking.detach();                                                           
    delivery.detach();

    while (restaurant->deliveriesCount < 10) {
        Dish dish;
        
        scoreboard.lock();
        dishes.push_back(dish);
        std::cout << "Online order " << dishes.back().name << " dish." << '\n'; 
        scoreboard.unlock();
        
        std::this_thread::sleep_for(std::chrono::seconds((rand() % 6 + 5)));
    }
    delete restaurant; restaurant = nullptr;
}