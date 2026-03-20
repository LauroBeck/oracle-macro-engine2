#include <iostream>
#include <cmath>

double clamp(double x){
    if(x > 1) return 1;
    if(x < -1) return -1;
    return x;
}

int main(){
    double alpha;
    std::cout << "Enter alpha: ";
    std::cin >> alpha;

    alpha = clamp(alpha);

    double tech = 0, ind = 0, energy = 0, cash = 0;

    if(alpha > 0.2){
        ind = 0.4;
        energy = 0.3;
        tech = 0.2;
        cash = 0.1;
    }
    else if(alpha < -0.2){
        cash = 0.6;
        energy = 0.2;
        tech = 0.1;
        ind = 0.1;
    }
    else{
        tech = 0.3;
        ind = 0.3;
        energy = 0.2;
        cash = 0.2;
    }

    std::cout << "\n=== ALLOCATION ===\n";
    std::cout << "Tech: " << tech*100 << "%\n";
    std::cout << "Industrials: " << ind*100 << "%\n";
    std::cout << "Energy: " << energy*100 << "%\n";
    std::cout << "Cash: " << cash*100 << "%\n";
}
