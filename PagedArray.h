#ifndef PAGEDARRAY_H
#define PAGEDARRAY_H

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <deque>

class PagedArray {
public:
    // Constructor que inicializa el arreglo paginado con la ruta del archivo y el tamaño de página
    PagedArray(const std::string &filePath, size_t pageSize);

    // Destructor que cierra los archivos abiertos
    ~PagedArray();


    int& operator[](size_t index);

    // Devuelve el tamaño del arreglo
    size_t size() const;

    // Devuelve el número de fallos de página
    size_t getPageFaults() const;

    // Devuelve el número de aciertos de página
    size_t getPageHits() const;

private:
    // Carga una página desde el archivo
    void loadPage(size_t pageIndex);

    // Descarga una página de la memoria
    void unloadPage(size_t pageIndex);

    std::string filePath;
    size_t pageSize;
    size_t numPages;
    std::unordered_map<size_t, std::vector<int>> pages;
    std::deque<size_t> pageOrder;
    size_t numElements;
    std::ifstream inputFile;
    std::ofstream outputFile;

    static const size_t maxPagesInMemory = 4; // Máximo número de páginas en memoria
    size_t pageFaults; // Contador de fallos de página
    size_t pageHits; // Contador de aciertos de página
};

#endif // PAGEDARRAY_H
