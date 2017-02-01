#ifndef Archive_H
#define Archive_H

#include <string>
#include <list>
#include <stack>
#include <vector>
#include <map>
#include <cmath>
#include <fstream>
using namespace std;

#include "tree.h"

#define histogram map<char, int>
#define histogramUnit pair<char, int>

class archive{

    map<char, string>* mapTable;
    histogram* freqTable;

    vector<char> data;
    tree* huffTree;
    int size;
    int oSize;

public:
    archive(string input = "") : mapTable(NULL), freqTable(NULL), huffTree(NULL){ compress(input); }

    ~archive(){ erase(); }

    // Export archive object to archive file
    void exportArchive(const char* dstName);

    // Parse archive file to archive object
    void importArchive(const char* dstName);

    // Decompress and extract to destination file
    void exportExtract(const char* dstName);

    // Compress file using the compress function
    bool compressFile(const char* fileName);

    string decompress();

    // Compress the input string (using Huffman tree)
    void compress(string& input);

    // Return the compression rate
    double compressionRate();

    //Prints the bits of the compressed string
    void printBits();

    // Prints the bits of the compressed string as integers
    void printNums();

    // Prints the replacement map
    void printMap();

    // Prints the frequency histogram
    void printHist();

    // Prints the Huffman tree
    void printTree();

private:
    // Import bitArray from file
    vector<char> importBitArray(ifstream& file);

    // Import freqTable from file
    histogram* importFreqTable(ifstream& file);

    // Export bitArray to file
    void exportBitArray(vector<char>& data, ofstream& file);

    // Export freqTable to file
    void exportFreqTable(histogram* freqTable, ofstream& file);

    // Add '0' to every char
    string toChar(string& str);

    // Bet the bit in the pos position of the byte
    bool getBit(int byte, int pos);

    // Delete the allocated memory
    void erase();

    // Convert the input to byte array (vector<char>)
    vector<char> getBitVector(string& input, map<char, string>* mapTable);

    // Convert the bit string to bit vector (not currently used)
    vector<char> getBitVector(string& bitString);

    // Covert byte string to char (real 1 byte)
    char binToNum(string& byte);

    // Convert the input string to bit string (0/1) (not currently used)
    string getBitString(string& input, map<char, string> mapTable);

    // Calls mapBuilder with the root of the Huffman tree
    map<char, string>* getMapTable(tree* huffTree);

    // Recursively build the map table
    void mapBuilder(node* huffTree, map<char, string>& mapper, stack<char>& bitStack);

    // Add all the bits recorded in the stack as a new entry in the mapper (leaf was reached)
    void addStackToMapper(map<char, string>& mapper, stack<char> bitStack, char value);

    // Return the Huffman tree corresponding to the frequency table
    tree* getHuffTree(histogram* freqTable);

    // Pop the tree with the smallest weight
    tree* popSmallestTree(list<tree*>& trees);

    // Convert frequency map table into vector of tees
    list<tree*> initTrees(histogram* freqTable);

    // New tree weight = t1.weight + t2 weight
    tree* mergeTrees(tree* t1, tree* t2);

    // Frequency table
    histogram* getHistogram(string& input);
};

#endif // Archive_H

