# Huffman
Implementation of the Huffman compression algorithm

## Interface
 - void exportArchive(const char* dstName);
 - void importArchive(const char* dstName);
 - void exportExtract(const char* dstName);
 - bool compressFile(const char* fileName);
 - string decompress();
 - void compress(string& input);
 - double compressionRate();
 
# Example
```cpp
#include "archive.h"

int main() {
    archive arch;
    arch.importArchive(fileName);
    std::cout << arch.decompress() << std::endl;
    std::cout << arch.compressionRate() << std::endl;
    
    return 0;
}
```
