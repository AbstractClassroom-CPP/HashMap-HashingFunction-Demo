#include "Dictionary/Dictionary.hpp"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

using Clock = std::chrono::steady_clock;

struct RunResult {
    std::size_t initialKeys;
    std::size_t ops;
    std::size_t inserts;
    std::size_t updates;
    std::chrono::nanoseconds elapsed;
};

RunResult runScenario(std::size_t initialKeys, std::size_t ops, double updateProbability) {
    Dictionary dict;
    std::vector<std::string> keys;
    keys.reserve(initialKeys + ops / 10);

    // Seed initial keys
    for (std::size_t i = 0; i < initialKeys; ++i) {
        std::string k = "key_" + std::to_string(i);
        dict.setValue(k, "value_" + std::to_string(i));
        keys.push_back(k);
    }

    std::mt19937 rng(12345);
    std::uniform_real_distribution<double> chance(0.0, 1.0);
    std::uniform_int_distribution<std::size_t> pickIndex(0, keys.empty() ? 0 : keys.size() - 1);

    std::size_t inserts = 0;
    std::size_t updates = 0;
    std::size_t nextNewKeyId = initialKeys;

    const auto start = Clock::now();
    for (std::size_t i = 0; i < ops; ++i) {
        const bool doUpdate = !keys.empty() && (chance(rng) < updateProbability);

        if (doUpdate) {
            // Update an existing key.
            pickIndex = std::uniform_int_distribution<std::size_t>(0, keys.size() - 1);
            const std::string& k = keys[pickIndex(rng)];
            dict.setValue(k, "updated_" + std::to_string(i));
            ++updates;
        } else {
            // Insert a new key. Entire linked list was scanned
            std::string k = "key_" + std::to_string(nextNewKeyId++);
            dict.setValue(k, "value_" + std::to_string(i));
            keys.push_back(k);
            ++inserts;
        }
    }
    const auto end = Clock::now();

    return RunResult{initialKeys, ops, inserts, updates, std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)};
}

void printResult(const RunResult& r) {
    const double seconds = std::chrono::duration<double>(r.elapsed).count();
    const double nsPerOp = r.ops == 0 ? 0.0 : static_cast<double>(r.elapsed.count()) / static_cast<double>(r.ops);

    std::cout << "initialKeys=" << std::setw(6) << r.initialKeys
              << "  ops=" << std::setw(9) << r.ops
              << "  inserts=" << std::setw(9) << r.inserts
              << "  updates=" << std::setw(9) << r.updates
              << "  time=" << std::fixed << std::setprecision(3) << seconds << "s"
              << "  (" << std::setprecision(0) << nsPerOp << " ns/op)\n";
}

} // namespace

int main(int argc, char** argv) {
    // Examples:
    //   ./profile_dictionary 200000
    //   ./profile_dictionary 200000 0.7 20000
    std::size_t ops = 15000;
    double updateProbability = 0.70; // 70% updates, 30% inserts
    std::size_t maxInitialKeys = 10000;

    try {
        if (argc >= 2) {
            ops = static_cast<std::size_t>(std::stoull(argv[1]));
        }
        if (argc >= 3) {
            updateProbability = std::stod(argv[2]);
        }
        if (argc >= 4) {
            maxInitialKeys = static_cast<std::size_t>(std::stoull(argv[3]));
        }
    } catch (const std::exception&) {
        std::cerr << "Usage: " << argv[0] << " [ops] [updateProbability] [maxInitialKeys]\n";
        return 2;
    }

    std::cout << "Profiling Dictionary (linked list)\n";
    std::cout << "ops per run: " << ops
              << ", updateProbability: " << updateProbability
              << ", maxInitialKeys: " << maxInitialKeys << "\n\n";

    // As initialKeys increases, each operation tends to get slower due to O(n) scans.
    for (std::size_t initialKeys : {1000u, 5000u, 10000u, 20000u}) {
        if (initialKeys > maxInitialKeys) {
            continue;
        }
        printResult(runScenario(initialKeys, ops, updateProbability));
    }

    return 0;
}




