#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <stdexcept>

enum Size { SMALL, MEDIUM, LARGE };

Size getSize(const std::string& sizeStr) {
    if (sizeStr == "SMALL") return SMALL;
    if (sizeStr == "MEDIUM") return MEDIUM;
    if (sizeStr == "LARGE") return LARGE;
    throw std::invalid_argument("Invalid size argument");
}

size_t getFileSize(Size size) {
    switch (size) {
        case SMALL: return 512 * 1024 * 1024;  // 512 MB
        case MEDIUM: return 1024 * 1024 * 1024; // 1 GB
        case LARGE: return 2 * 1024 * 1024 * 1024; // 2 GB
    }
    throw std::invalid_argument("Invalid size");
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " -size <SIZE> -output <OUTPUT FILE PATH>" << std::endl;
        return 1;
    }

    std::string sizeStr;
    std::string outputPath;

    // Procesar los argumentos
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-size") {
            if (i + 1 < argc) {
                sizeStr = argv[++i];
            } else {
                std::cerr << "Error: Missing value for -size" << std::endl;
                return 1;
            }
        } else if (std::string(argv[i]) == "-output") {
            if (i + 1 < argc) {
                outputPath = argv[++i];
            } else {
                std::cerr << "Error: Missing value for -output" << std::endl;
                return 1;
            }
        } else {
            std::cerr << "Invalid argument: " << argv[i] << std::endl;
            return 1;
        }
    }

    Size size;
    try {
        size = getSize(sizeStr);
    } catch (const std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    size_t fileSize = getFileSize(size);
    size_t numIntegers = fileSize / sizeof(int);

    std::ofstream outFile(outputPath, std::ios::binary);
    if (!outFile) {
        std::cerr << "Error opening output file" << std::endl;
        return 1;
    }

    std::srand(std::time(nullptr));
    for (size_t i = 0; i < numIntegers; ++i) {
        int randomInt = std::rand();
        outFile.write(reinterpret_cast<const char*>(&randomInt), sizeof(randomInt));
    }

    outFile.close();
    std::cout << "File generated successfully!" << std::endl;

    return 0;
}
