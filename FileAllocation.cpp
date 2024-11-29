#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <memory>
#include <algorithm>

class Block {
public:
    int blockNumber;
    bool isAllocated;

    Block(int number) : blockNumber(number), isAllocated(false) {}
};

class File {
public:
    std::string fileName;
    int fileSize;
    int indexBlockNumber;
    std::vector<int> allocatedBlocks;

    File(const std::string& name, int size) 
        : fileName(name), fileSize(size), indexBlockNumber(-1) {}
};

class FileSystem {
private:
    std::vector<Block> diskBlocks;
    std::vector<File> files;
    int totalBlocks;
    int blockSize;

public:
    FileSystem(int totalBlockCount, int blockSizeInBytes) 
        : totalBlocks(totalBlockCount), blockSize(blockSizeInBytes) {
        // Initialize disk blocks
        for (int i = 0; i < totalBlocks; ++i) {
            diskBlocks.emplace_back(i);
        }
    }

    int findFreeIndexBlock() {
        for (int i = 0; i < totalBlocks; ++i) {
            if (!diskBlocks[i].isAllocated) {
                diskBlocks[i].isAllocated = true;
                return i;
            }
        }
        throw std::runtime_error("No free index block available");
    }

    std::vector<int> allocateBlocks(int fileSize) {
        std::vector<int> allocatedBlocks;
        int blocksNeeded = (fileSize + blockSize - 1) / blockSize;

        for (int i = 0; i < totalBlocks && allocatedBlocks.size() < blocksNeeded; ++i) {
            if (!diskBlocks[i].isAllocated) {
                diskBlocks[i].isAllocated = true;
                allocatedBlocks.push_back(i);
            }
        }

        if (allocatedBlocks.size() < blocksNeeded) {
            // Free previously allocated blocks
            for (int block : allocatedBlocks) {
                diskBlocks[block].isAllocated = false;
            }
            throw std::runtime_error("Insufficient free blocks for file allocation");
        }

        return allocatedBlocks;
    }

    void createFile(const std::string& fileName, int fileSize) {
        // Find an index block
        int indexBlockNumber = findFreeIndexBlock();

        // Allocate blocks for the file
        std::vector<int> allocatedBlocks = allocateBlocks(fileSize);

        // Create file object
        File newFile(fileName, fileSize);
        newFile.indexBlockNumber = indexBlockNumber;
        newFile.allocatedBlocks = allocatedBlocks;

        files.push_back(newFile);
    }

    void printFileAllocation() {
        std::cout << "File Allocation Details:\n";
        for (const auto& file : files) {
            std::cout << "File: " << file.fileName 
                      << ", Size: " << file.fileSize 
                      << " bytes\n";
            std::cout << "  Index Block: " << file.indexBlockNumber << "\n";
            std::cout << "  Allocated Blocks: ";
            for (int block : file.allocatedBlocks) {
                std::cout << block << " ";
            }
            std::cout << "\n\n";
        }
    }

    void deleteFile(const std::string& fileName) {
        auto it = std::find_if(files.begin(), files.end(), 
            [&fileName](const File& f) { return f.fileName == fileName; });

        if (it != files.end()) {
            // Free index block
            diskBlocks[it->indexBlockNumber].isAllocated = false;

            // Free allocated blocks
            for (int block : it->allocatedBlocks) {
                diskBlocks[block].isAllocated = false;
            }

            // Remove file from list
            files.erase(it);
        }
    }
};

int main() {
    // Create a file system with 100 blocks, each 1024 bytes
    FileSystem fs(100, 1024);

    try {
        // File 1: Large file
        fs.createFile("document.txt", 5120);  // 5 blocks

        // File 2: Small file
        fs.createFile("notes.txt", 2048);  // 2 blocks

        // File 3: Single block file
        fs.createFile("readme.md", 500);  // 1 block

        // Print allocation details
        fs.printFileAllocation();

        // Delete a file
        fs.deleteFile("notes.txt");

        // Print updated allocation
        std::cout << "After deleting notes.txt:\n";
        fs.printFileAllocation();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
