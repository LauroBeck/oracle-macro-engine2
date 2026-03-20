#include <vector>
#include <cmath>

struct Signal {
    double alpha;
    double confidence;
};

double clamp(double x){
    if(x > 1) return 1;
    if(x < -1) return -1;
    return x;
}

Signal compute_signal(double tech, double capex, double oil){
    double alpha = capex - tech - oil;
    alpha = clamp(alpha);

    Signal s;
    s.alpha = alpha;
    s.confidence = std::abs(alpha) * 100.0;

    return s;
}
