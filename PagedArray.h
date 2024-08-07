#ifndef PAGEDARRAY_H
#define PAGEDARRAY_H

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <deque>

class PagedArray {
public:
    PagedArray(const std::string &filePath, size_t pageSize);
    ~PagedArray();

    int& operator[](size_t index);
    size_t size() const;
    size_t getPageFaults() const;
    size_t getPageHits() const;

private:
    void loadPage(size_t pageIndex);
    void unloadPage(size_t pageIndex);

    std::string filePath;
    size_t pageSize;
    size_t numPages;
    std::unordered_map<size_t, std::vector<int>> pages;
    std::deque<size_t> pageOrder;
    size_t numElements;
    std::ifstream inputFile;
    std::ofstream outputFile;

    static const size_t maxPagesInMemory = 4;
    size_t pageFaults;
    size_t pageHits;
};

#endif // PAGEDARRAY_H
