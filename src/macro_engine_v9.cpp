#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

struct Row {
    double tech, capex, oil;
    double us10y, us2y, dxy, gold;
    double hy_spread, vix, orcl;
};

class Engine {
private:
    std::vector<Row> data;

public:
    void load(const std::string& file){
        std::ifstream f(file);
        std::string line;

        if(!f){
            std::cout << "File not found\n";
            return;
        }

        std::getline(f,line);

        while(std::getline(f,line)){
            std::stringstream ss(line);
            std::string val;
            Row r;

            try {
                std::getline(ss,val,','); r.tech = std::stod(val);
                std::getline(ss,val,','); r.capex = std::stod(val);
                std::getline(ss,val,','); r.oil = std::stod(val);
                std::getline(ss,val,','); r.us10y = std::stod(val);
                std::getline(ss,val,','); r.us2y = std::stod(val);
                std::getline(ss,val,','); r.dxy = std::stod(val);
                std::getline(ss,val,','); r.gold = std::stod(val);
                std::getline(ss,val,','); r.hy_spread = std::stod(val);
                std::getline(ss,val,','); r.vix = std::stod(val);
                std::getline(ss,val,','); r.orcl = std::stod(val);

                data.push_back(r);
            } catch(...) {
                continue;
            }
        }

        std::cout << "Loaded rows: " << data.size() << "\n";
    }

    double accel(double a,double b,double c){
        return (c-b)-(b-a);
    }

    void run(){
        int n=data.size();
        if(n<3){
            std::cout<<"Not enough data\n";
            return;
        }

        double tech_accel  = accel(data[n-3].tech,data[n-2].tech,data[n-1].tech);
        double capex_accel = accel(data[n-3].capex,data[n-2].capex,data[n-1].capex);
        double oil_accel   = accel(data[n-3].oil,data[n-2].oil,data[n-1].oil);

        double alpha = (capex_accel - tech_accel - oil_accel)/3.0;

        std::cout << "\n=== MACRO ENGINE v9 ===\n";
        std::cout << "Alpha: " << alpha << "\n";
    }
};

int main(){
    Engine e;
    e.load("data/macro_data_v4.csv");
    e.run();
}
