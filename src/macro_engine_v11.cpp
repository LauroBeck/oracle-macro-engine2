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

struct Allocation {
    double tech, industrials, energy, cash;
};

class Engine {
private:
    std::vector<Row> data;

    double ret(double a,double b){
        if(a==0) return 0;
        return (b-a)/a;
    }

    double clamp(double x){
        if(x > 1) return 1;
        if(x < -1) return -1;
        return x;
    }

    double compute_vol(){
        double s=0;
        for(size_t i=1;i<data.size();i++){
            double r = ret(data[i-1].tech, data[i].tech);
            s += r*r;
        }
        return std::sqrt(s/data.size());
    }

    Allocation allocate(double alpha, double orcl_signal){
        Allocation a{0,0,0,0};

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

        // 🔥 ORACLE BOOST (AI / CLOUD)
        if(orcl_signal > 0){
            a.tech += 0.1;
            a.cash -= 0.1;
        } else {
            a.tech -= 0.1;
            a.cash += 0.1;
        }

        return a;
    }

public:

    void load(const std::string& file){
        std::ifstream f(file);
        std::string line;
        std::getline(f,line);

        while(std::getline(f,line)){
            std::stringstream ss(line);
            std::string val;
            Row r;

            try {
                std::getline(ss,val,','); r.tech = std::stod(val);
                std::getline(ss,val,','); r.capex = std::stod(val);
                std::getline(ss,val,','); r.oil = std::stod(val);
                std::getline(ss,val,','); r.gold = std::stod(val);
                std::getline(ss,val,','); r.vix = std::stod(val);
                std::getline(ss,val,','); r.orcl = std::stod(val);

                data.push_back(r);
            } catch(...) { continue; }
        }

        std::cout << "Loaded rows: " << data.size() << "\n";
    }

    void run(){
        int n = data.size();
        if(n < 5){
            std::cout << "Not enough data\n";
            return;
        }

        // ===== CORE SIGNALS =====
        double tech_a = ret(data[n-2].tech, data[n-1].tech) - ret(data[n-3].tech, data[n-2].tech);
        double capex_a = ret(data[n-2].capex, data[n-1].capex) - ret(data[n-3].capex, data[n-2].capex);
        double oil_a = ret(data[n-2].oil, data[n-1].oil) - ret(data[n-3].oil, data[n-2].oil);

        double alpha = clamp(capex_a - tech_a - oil_a);
        double confidence = std::abs(alpha)*100;

        // ===== ORACLE SIGNAL =====
        double orcl_a = ret(data[n-2].orcl, data[n-1].orcl) - ret(data[n-3].orcl, data[n-2].orcl);

        // ===== VOL =====
        double vol = compute_vol();

        // ===== BACKTEST =====
        double equity = 1.0;
        for(int i = n-10; i < n-1; i++){
            double r = ret(data[i].capex, data[i+1].capex);

            double a = (ret(data[i].capex,data[i+1].capex)
                       -ret(data[i].tech,data[i+1].tech)
                       -ret(data[i].oil,data[i+1].oil));

            int signal = 0;
            if(a > 0.05) signal = 1;
            else if(a < -0.05) signal = -1;

            equity *= (1 + signal*r);
        }

        // ===== ALLOCATION =====
        Allocation alloc = allocate(alpha, orcl_a);

        // ===== OUTPUT =====
        std::cout << std::fixed << std::setprecision(4);

        std::cout << "\n=== MACRO ENGINE v11 ===\n";
        std::cout << "Alpha: " << alpha << "\n";
        std::cout << "Confidence: " << confidence << "%\n";

        // 🔥 ORACLE BLOCK
        std::cout << "\n=== ORACLE SIGNAL ===\n";
        std::cout << "ORCL accel: " << orcl_a << "\n";

        if(orcl_a > 0)
            std::cout << "🚀 AI / Cloud bullish\n";
        else
            std::cout << "⚠️ AI / Cloud weakening\n";

        // CORE REGIME
        if(alpha > 0.2) std::cout << "\n🟢 RISK-ON\n";
        else if(alpha < -0.2) std::cout << "\n🔴 RISK-OFF\n";
        else std::cout << "\n⚖️ ROTATION\n";

        std::cout << "\n=== RISK ===\n";
        std::cout << "Volatility: " << vol << "\n";

        std::cout << "\n=== BACKTEST ===\n";
        std::cout << "Equity: " << equity << "\n";

        std::cout << "\n=== PORTFOLIO ===\n";
        std::cout << "Tech: " << alloc.tech*100 << "%\n";
        std::cout << "Industrials: " << alloc.industrials*100 << "%\n";
        std::cout << "Energy: " << alloc.energy*100 << "%\n";
        std::cout << "Cash: " << alloc.cash*100 << "%\n";
    }
};

int main(){
    Engine e;
    e.load("data/macro_data_v4.csv");
    e.run();
}
