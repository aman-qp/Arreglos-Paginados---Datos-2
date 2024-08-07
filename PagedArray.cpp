#include "PagedArray.h"

PagedArray::PagedArray(const std::string &filePath, size_t pageSize)
    : filePath(filePath), pageSize(pageSize), pageFaults(0), pageHits(0) {
    inputFile.open(filePath, std::ios::binary | std::ios::ate);
    if (!inputFile.is_open()) {
        throw std::runtime_error("Cannot open file");
    }

    numElements = inputFile.tellg() / sizeof(int);
    numPages = (numElements + pageSize - 1) / pageSize;
    inputFile.seekg(0, std::ios::beg);
}

PagedArray::~PagedArray() {
    inputFile.close();
    outputFile.close();
}

size_t PagedArray::size() const {
    return numElements;
}

int& PagedArray::operator[](size_t index) {
    if (index >= numElements) {
        throw std::out_of_range("Index out of bounds");
    }

    size_t pageIndex = index / pageSize;
    if (pages.find(pageIndex) == pages.end()) {
        loadPage(pageIndex);
        ++pageFaults;
    } else {
        ++pageHits;
    }

    size_t localIndex = index % pageSize;
    return pages[pageIndex][localIndex];
}

void PagedArray::loadPage(size_t pageIndex) {
    if (pages.size() >= maxPagesInMemory) {
        unloadPage(pageOrder.front());
        pageOrder.pop_front();
    }

    std::vector<int> page(pageSize);
    inputFile.seekg(pageIndex * pageSize * sizeof(int), std::ios::beg);
    inputFile.read(reinterpret_cast<char*>(page.data()), pageSize * sizeof(int));

    pages[pageIndex] = std::move(page);
    pageOrder.push_back(pageIndex);
}

void PagedArray::unloadPage(size_t pageIndex) {
    if (pages.find(pageIndex) != pages.end()) {
        pages.erase(pageIndex);
    }
}

size_t PagedArray::getPageFaults() const {
    return pageFaults;
}

size_t PagedArray::getPageHits() const {
    return pageHits;
}
