#include "PagedArray.h"

// Constructor de PagedArray
PagedArray::PagedArray(const std::string &filePath, size_t pageSize)
    : filePath(filePath), pageSize(pageSize), pageFaults(0), pageHits(0) {
    // Abrir el archivo en modo binario y posicionar el puntero al final
    inputFile.open(filePath, std::ios::binary | std::ios::ate);
    if (!inputFile.is_open()) {
        throw std::runtime_error("No se puede abrir el archivo");
    }

    // Calcular el número de elementos y páginas en el archivo
    numElements = inputFile.tellg() / sizeof(int);
    numPages = (numElements + pageSize - 1) / pageSize;
    inputFile.seekg(0, std::ios::beg);
}

// Destructor de PagedArray
PagedArray::~PagedArray() {
    inputFile.close();
    outputFile.close();
}

// Devuelve el tamaño del arreglo
size_t PagedArray::size() const {
    return numElements;
}

// Operador de índice para acceder a los elementos
int& PagedArray::operator[](size_t index) {
    if (index >= numElements) {
        throw std::out_of_range("Índice fuera de los límites");
    }

    // Calcular el índice de la página
    size_t pageIndex = index / pageSize;
    // Verificar si la página está cargada
    if (pages.find(pageIndex) == pages.end()) {
        loadPage(pageIndex);
        ++pageFaults; // Incrementar fallos de página
    } else {
        ++pageHits; // Incrementar aciertos de página
    }

    // Calcular el índice local dentro de la página
    size_t localIndex = index % pageSize;
    return pages[pageIndex][localIndex];
}

// Cargar una página en memoria
void PagedArray::loadPage(size_t pageIndex) {
    // Si la cantidad de páginas en memoria excede el máximo, descargar la más antigua
    if (pages.size() >= maxPagesInMemory) {
        unloadPage(pageOrder.front());
        pageOrder.pop_front();
    }

    // Leer la página del archivo
    std::vector<int> page(pageSize);
    inputFile.seekg(pageIndex * pageSize * sizeof(int), std::ios::beg);
    inputFile.read(reinterpret_cast<char*>(page.data()), pageSize * sizeof(int));

    // Almacenar la página en el mapa y actualizar el orden de las páginas
    pages[pageIndex] = std::move(page);
    pageOrder.push_back(pageIndex);
}

// Descargar una página de la memoria
void PagedArray::unloadPage(size_t pageIndex) {
    if (pages.find(pageIndex) != pages.end()) {
        pages.erase(pageIndex);
    }
}

// Devuelve el número de fallos de página
size_t PagedArray::getPageFaults() const {
    return pageFaults;
}

// Devuelve el número de aciertos de página
size_t PagedArray::getPageHits() const {
    return pageHits;
}
