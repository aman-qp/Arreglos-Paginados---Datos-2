#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <stdexcept>
#include "PagedArray.h"

enum SortAlgorithm { QUICK_SORT, INSERTION_SORT, BUBBLE_SORT };

SortAlgorithm getSortAlgorithm(const std::string &alg) {
    if (alg == "QS") return QUICK_SORT;
    if (alg == "IS") return INSERTION_SORT;
    if (alg == "BS") return BUBBLE_SORT;
    throw std::invalid_argument("Invalid sorting algorithm");
}

template <typename T>
void quickSort(T &array, size_t left, size_t right) {
    if (left < right) {
        size_t pivotIndex = left + (right - left) / 2;
        int pivotValue = array[pivotIndex];
        std::swap(array[pivotIndex], array[right]);
        size_t storeIndex = left;
        for (size_t i = left; i < right; ++i) {
            if (array[i] < pivotValue) {
                std::swap(array[i], array[storeIndex]);
                ++storeIndex;
            }
        }
        std::swap(array[storeIndex], array[right]);

        if (storeIndex > left) quickSort(array, left, storeIndex - 1);
        if (storeIndex < right) quickSort(array, storeIndex + 1, right);
    }
}

template <typename T>
void insertionSort(T &array, size_t size) {
    for (size_t i = 1; i < size; ++i) {
        int key = array[i];
        size_t j = i;
        while (j > 0 && array[j - 1] > key) {
            array[j] = array[j - 1];
            --j;
        }
        array[j] = key;
    }
}

template <typename T>
void bubbleSort(T &array, size_t size) {
    for (size_t i = 0; i < size - 1; ++i) {
        for (size_t j = 0; j < size - i - 1; ++j) {
            if (array[j] > array[j + 1]) {
                std::swap(array[j], array[j + 1]);
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 7) {
        std::cerr << "Usage: " << argv[0] << " -input <INPUT FILE PATH> -output <OUTPUT FILE PATH> -alg <ALGORITHM>" << std::endl;
        return 1;
    }

    std::string inputPath;
    std::string outputPath;
    std::string algStr;

    // Procesar los argumentos
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-input") {
            if (i + 1 < argc) {
                inputPath = argv[++i];
            } else {
                std::cerr << "Error: Missing value for -input" << std::endl;
                return 1;
            }
        } else if (std::string(argv[i]) == "-output") {
            if (i + 1 < argc) {
                outputPath = argv[++i];
            } else {
                std::cerr << "Error: Missing value for -output" << std::endl;
                return 1;
            }
        } else if (std::string(argv[i]) == "-alg") {
            if (i + 1 < argc) {
                algStr = argv[++i];
            } else {
                std::cerr << "Error: Missing value for -alg" << std::endl;
                return 1;
            }
        } else {
            std::cerr << "Invalid argument: " << argv[i] << std::endl;
            return 1;
        }
    }

    SortAlgorithm algorithm;
    try {
        algorithm = getSortAlgorithm(algStr);
    } catch (const std::invalid_argument &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    const size_t pageSize = 4096; // Ajusta según tus necesidades
    PagedArray pagedArray(inputPath, pageSize);

    auto start = std::chrono::high_resolution_clock::now();

    switch (algorithm) {
        case QUICK_SORT:
            quickSort(pagedArray, 0, pagedArray.size() - 1);
            break;
        case INSERTION_SORT:
            insertionSort(pagedArray, pagedArray.size());
            break;
        case BUBBLE_SORT:
            bubbleSort(pagedArray, pagedArray.size());
            break;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    // Crear el archivo binario ordenado
    std::ofstream outputFile(outputPath, std::ios::binary);
    if (!outputFile) {
        std::cerr << "Error opening output file" << std::endl;
        return 1;
    }

    for (size_t i = 0; i < pagedArray.size(); ++i) {
        int value = pagedArray[i];
        outputFile.write(reinterpret_cast<const char*>(&value), sizeof(value));
    }
    outputFile.close();

    // Convertir el archivo binario a un archivo de texto legible
    std::ofstream readableOutputFile(outputPath + ".txt");
    if (!readableOutputFile) {
        std::cerr << "Error opening readable output file" << std::endl;
        return 1;
    }

    for (size_t i = 0; i < pagedArray.size(); ++i) {
        if (i > 0) {
            readableOutputFile << ",";
        }
        readableOutputFile << pagedArray[i];
    }
    readableOutputFile.close();

    std::cout << "Sorting completed in " << elapsed.count() << " seconds." << std::endl;
    std::cout << "Algorithm used: " << algStr << std::endl;
    std::cout << "Page faults: " << pagedArray.getPageFaults() << std::endl;
    std::cout << "Page hits: " << pagedArray.getPageHits() << std::endl;

    return 0;
}
