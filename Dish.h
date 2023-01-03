#pragma once
#include <string>

class Dish {
private:
    enum Name {Pizza, Soup, Steak, Salad, Sushi};
public:
    bool ready = false;
    std::string name = "unknown";

    Dish() {
        int temp = rand() % 5;

        if (temp == Soup) name = "Soup";
        else if (temp == Steak) name = "Steak";
        else if (temp == Salad) name = "Salad";
        else if (temp == Sushi) name = "Sushi";
        else name = "Pizza";
    }
};