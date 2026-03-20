#include <iostream>

struct Allocation {
    double tech;
    double industrials;
    double energy;
    double cash;
};

Allocation allocate(double alpha){
    Allocation a;

    if(alpha > 0.2){
        a.industrials = 0.4;
        a.energy = 0.3;
        a.tech = 0.2;
        a.cash = 0.1;
    }
    else if(alpha < -0.2){
        a.cash = 0.6;
        a.energy = 0.2;
        a.tech = 0.1;
        a.industrials = 0.1;
    }
    else{
        a.tech = 0.3;
        a.industrials = 0.3;
        a.energy = 0.2;
        a.cash = 0.2;
    }

    return a;
}

void print_alloc(const Allocation& a){
    std::cout << "\n=== PORTFOLIO ===\n";
    std::cout << "Tech: " << a.tech*100 << "%\n";
    std::cout << "Industrials: " << a.industrials*100 << "%\n";
    std::cout << "Energy: " << a.energy*100 << "%\n";
    std::cout << "Cash: " << a.cash*100 << "%\n";
}
