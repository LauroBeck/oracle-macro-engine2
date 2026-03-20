#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <iomanip>

struct Row {
    double tech, capex, oil;
    double gold, vix, orcl;
};

class Engine {
private:
    std::vector<Row> data;

public:

    void load(const std::string& file){
        std::ifstream f(file);
        std::string line;

        std::getline(f,line);

        while(std::getline(f,line)){
            std::stringstream ss(line);
            std::string val;
            Row r;

            std::getline(ss,val,','); r.tech = std::stod(val);
            std::getline(ss,val,','); r.capex = std::stod(val);
            std::getline(ss,val,','); r.oil = std::stod(val);
            std::getline(ss,val,','); r.gold = std::stod(val);
            std::getline(ss,val,','); r.vix = std::stod(val);
            std::getline(ss,val,','); r.orcl = std::stod(val);

            data.push_back(r);
        }

        std::cout << "Loaded rows: " << data.size() << "\n";
    }

    double ret(double a,double b){
        if(a==0) return 0;
        return (b-a)/a;
    }

    double vol(){
        double s=0;
        for(size_t i=1;i<data.size();i++){
            double r = ret(data[i-1].tech, data[i].tech);
            s += r*r;
        }
        return std::sqrt(s/data.size());
    }

    void run(){
        int n=data.size();
        if(n<3){
            std::cout<<"Not enough data\n";
            return;
        }

        double tech_r1 = ret(data[n-3].tech, data[n-2].tech);
        double tech_r2 = ret(data[n-2].tech, data[n-1].tech);

        double capex_r1 = ret(data[n-3].capex, data[n-2].capex);
        double capex_r2 = ret(data[n-2].capex, data[n-1].capex);

        double oil_r1 = ret(data[n-3].oil, data[n-2].oil);
        double oil_r2 = ret(data[n-2].oil, data[n-1].oil);

        double tech_accel = tech_r2 - tech_r1;
        double capex_accel = capex_r2 - capex_r1;
        double oil_accel = oil_r2 - oil_r1;

        double alpha = capex_accel - tech_accel - oil_accel;

        if(alpha > 1) alpha = 1;
        if(alpha < -1) alpha = -1;

        double conf = std::abs(alpha)*100;
        double v = vol();

        std::cout << "\n=== MACRO ENGINE v10 ===\n";
        std::cout << std::fixed << std::setprecision(4);
        std::cout << "Alpha: " << alpha << "\n";
        std::cout << "Confidence: " << conf << "%\n";

        if(alpha > 0.2) std::cout << "RISK-ON\n";
        else if(alpha < -0.2) std::cout << "RISK-OFF\n";
        else std::cout << "ROTATION\n";

        std::cout << "\nVolatility: " << v << "\n";
    }
};

int main(){
    Engine e;
    e.load("data/macro_data_v4.csv");
    e.run();
}
