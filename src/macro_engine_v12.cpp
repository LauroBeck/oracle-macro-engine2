#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

struct MacroSignals {
    std::string sector;
    double rates_vol;
    double oil_regime;
    double equity_beta;
    double ai_cloud_score; 
};

struct GlobalExposure {
    double total_nvda = 0;
    double total_msft = 0;
    double total_orcl = 0;
    int client_count = 0;
};

class InstitutionalAllocator {
public:
    void process_client(const MacroSignals& sig, GlobalExposure& global) {
        double nvda = 0.33, msft = 0.33, orcl = 0.34;
        double gross = (sig.equity_beta > 1.2) ? 0.5 : 1.0;

        if (sig.sector == "BANK") {
            nvda = 0.15; msft = 0.35; orcl = 0.50;
            if (sig.rates_vol > 1.5) gross *= 0.8;
        } else if (sig.sector == "BIG_OIL") {
            nvda = 0.45; msft = 0.25; orcl = 0.30;
            if (sig.oil_regime > 100.0) gross *= 1.2;
        } else if (sig.sector == "MINING") {
            nvda = 0.20; msft = 0.20; orcl = 0.60;
            if (sig.oil_regime > 110.0) gross *= 0.4;
        } else { // BIG_TECH
            nvda = 0.50; msft = 0.30; orcl = 0.20;
        }

        // Apply Gross & Track Global
        double final_nvda = nvda * gross;
        double final_msft = msft * gross;
        double final_orcl = orcl * gross;

        global.total_nvda += final_nvda;
        global.total_msft += final_msft;
        global.total_orcl += final_orcl;
        global.client_count++;

        std::cout << std::left << std::setw(10) << sig.sector 
                  << " | GRS: " << std::fixed << std::setprecision(0) << std::setw(3) << gross * 100 << "% "
                  << "| NVDA: " << std::setprecision(2) << final_nvda 
                  << " | MSFT: " << final_msft 
                  << " | ORCL: " << final_orcl << std::endl;
    }
};

int main() {
    InstitutionalAllocator engine;
    GlobalExposure global;
    std::ifstream file("data/macro_data_v4.csv");

    if (!file.is_open()) return 1;

    std::string line, word;
    std::getline(file, line); // Skip Header

    while (std::getline(file, line)) {
        if(line.empty()) continue;
        std::stringstream ss(line);
        std::vector<std::string> row;
        while (std::getline(ss, word, ',')) row.push_back(word);

        if (row.size() >= 5) {
            try {
                MacroSignals sig = {row[0], std::stod(row[1]), std::stod(row[2]), std::stod(row[3]), std::stod(row[4])};
                engine.process_client(sig, global);
            } catch (...) { continue; }
        }
    }

    std::cout << "\n-----------------------------------------------" << std::endl;
    std::cout << "--- FINAL v12.6 AGGREGATE SUMMARY (n=" << global.client_count << ") ---" << std::endl;
    std::cout << "TOTAL NVDA UNIT EXPOSURE: " << global.total_nvda << std::endl;
    std::cout << "TOTAL MSFT UNIT EXPOSURE: " << global.total_msft << std::endl;
    std::cout << "TOTAL ORCL UNIT EXPOSURE: " << global.total_orcl << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;

    return 0;
}
