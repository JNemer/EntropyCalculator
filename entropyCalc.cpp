#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <map>
#include <filesystem> // C++17 filesystem header

namespace fs = std::filesystem;

double calculateEntropy(const std::string& filePath) {
    std::ifstream file(filePath, std::ifstream::binary);
    if (!file.is_open()) {
        std::cerr << "File open error - '" << filePath << "'" << std::endl;
        return 0.0;
    }

    std::map<unsigned char, long long> frequencies;
    char buffer[1024];
    while (file.read(buffer, sizeof(buffer)) || file.gcount()) {
        for (std::streamsize i = 0; i < file.gcount(); ++i) {
            frequencies[static_cast<unsigned char>(buffer[i])]++;
        }
    }

    file.close();

    long long totalSize = 0;
    for (const auto& pair : frequencies) {
        totalSize += pair.second;
    }

    double entropy = 0.0;
    for (const auto& pair : frequencies) {
        double frequency = static_cast<double>(pair.second) / totalSize;
        entropy += frequency * std::log2(frequency);
    }
    return -entropy;
}

void calculateEntropyForAllFiles(const std::string& directoryPath) {
    std::vector<std::pair<std::string, double>> fileEntropies;

    try {
        for (const auto& entry : fs::directory_iterator(directoryPath)) {
            if (entry.is_regular_file()) {
                double entropy = calculateEntropy(entry.path().string());
                fileEntropies.push_back({entry.path().string(), entropy});
                std::cout << "Entropy of '" << entry.path().string() << "' is: " << entropy << std::endl;
            }
        }

        std::cout << "Entropies >= 7.2:" << std::endl;
        for (const auto& pair : fileEntropies) {
            if (pair.second >= 7.2) {
                std::cout << "File: " << pair.first << ", Entropy: " << pair.second << std::endl;
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main(int argc, char* argv[]) {
    std::string directoryPath;

    if (argc >= 2) {
        directoryPath = argv[1];
    } else {
        directoryPath = fs::current_path().string();
        std::cout << "Using the current directory: " << directoryPath << std::endl;
    }

    calculateEntropyForAllFiles(directoryPath);

    // Wait for user input before closing the console window
    std::getchar();

    return 0;
}