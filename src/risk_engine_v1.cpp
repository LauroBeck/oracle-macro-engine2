#include <iostream>
#include <vector>
#include <cmath>

double compute_vol(std::vector<double> r){
    double s = 0;
    for(double x : r) s += x*x;
    return std::sqrt(s / r.size());
}

int main(){
    std::vector<double> returns = {0.01, -0.005, 0.002, -0.003, 0.004};

    double vol = compute_vol(returns);

    std::cout << "Volatility: " << vol << "\n";

    if(vol < 0.01)
        std::cout << "LOW VOL → TREND\n";
    else
        std::cout << "HIGH VOL → RISK\n";
}
