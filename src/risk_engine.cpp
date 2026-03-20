#include <vector>
#include <cmath>

double compute_vol(const std::vector<double>& returns){
    if(returns.empty()) return 0;

    double sum = 0;
    for(double r : returns)
        sum += r*r;

    return std::sqrt(sum / returns.size());
}

const char* risk_regime(double vol){
    if(vol < 0.01) return "LOW VOL (TREND)";
    if(vol < 0.02) return "MID VOL (ROTATION)";
    return "HIGH VOL (RISK-OFF)";
}
