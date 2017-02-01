# Implementation of the Huffman compression algorithm

## Interface
 - importArchive
 - exportExtract
 - compressFile
 - decompress
 - compressionRate
 - exportArchive
 
# Example
```cpp
#include "archive.h"

int main() {
    archive arch;
    arch.importArchive(fileName);
    cout << arch.decompress() << endl;
    cout << arch.compressionRate() << endl;
    
    return 0;
}
```
