#ifndef Archive_CPP
#define Archive_CPP

#include "archive.h"

void archive::exportArchive(const char* dstName){
    ofstream file(dstName, ios::binary | ios::out | fstream::trunc);
    exportFreqTable(freqTable, file);
    exportBitArray(data, file);
    file.write(reinterpret_cast<char*>(&size), sizeof(int));
    file.write(reinterpret_cast<char*>(&oSize), sizeof(int));

    file.close();
}

void archive::importArchive(const char* dstName){
    erase();
    ifstream file(dstName, ios::binary | ios::in);

    freqTable = importFreqTable(file);
    huffTree = getHuffTree(freqTable);
    mapTable = getMapTable(huffTree);
    data = importBitArray(file);

    file.read((char*)(&size), sizeof(int));
    file.read((char*)(&oSize), sizeof(int));

    file.close();
}

void archive::exportExtract(const char* dstName){
    ofstream file(dstName, ios::binary | ios::out);
    string original = decompress();

    for(unsigned i = 0;i < original.size();i++){
        char byte = original[i];
        file.write(reinterpret_cast<char*>(&byte), sizeof(char));
    }
}

bool archive::compressFile(const char* fileName){
    ifstream file(fileName, ios::binary | ios::in);
    if(!file.is_open()){
        return false;
    }else{
        int cnt = 0;
        char byte;
        string input;

        while(!file.eof()){
            file.seekg(cnt++);
            file.read(reinterpret_cast<char*>(&byte), 1);
            input += byte;
        }
        file.close();

        compress(input);
        return true;
    }

    return false;
}

string archive::decompress(){
    string result;
    if(huffTree == NULL)
        return result;

    node* traverse = huffTree -> getRoot();
    int counter = 0;

    for(unsigned i = 0;i < data.size();i++){
        for(int j = 7;j >= 0;j--){
            if(traverse == NULL || counter  >= size)
                return result;

            if(getBit(data[i], j))
                traverse = traverse -> right;
            else
                traverse = traverse -> left;

            if(traverse -> leaf()){
                result += traverse -> value;
                traverse = huffTree -> getRoot();
            }

            counter++;
        }
    }

    return result;
}

void archive::compress(string& input){
    erase();
    histogram* freqTable = getHistogram(input);
    tree* huffTree = getHuffTree(freqTable);
    map<char, string>* mapTable = getMapTable(huffTree);
    vector<char> data = getBitVector(input, mapTable);

    this -> oSize = input.size() * 8;
    this -> huffTree = huffTree;
    this -> data = data;
    this -> mapTable = mapTable;
    this -> freqTable = freqTable;
}

double archive::compressionRate(){
    return oSize == 0 ? 0 : (double) size / oSize;
}

void archive::printBits(){
    for(unsigned i = 0;i < data.size();i++)
        for(int j = 7;j >= 0;j--){
            cout << (int) getBit(data[i], j);
            if(j == 0)
                cout << ' ';
        }
}

void archive::printNums(){
    for(unsigned i = 0;i < data.size();i++)
            cout << data[i] + '0' << ' ';
}

void archive::printMap(){
    if(mapTable != NULL)
        for (map<char, string>::iterator it = mapTable -> begin();it != mapTable -> end();it++)
            cout << it -> first << " => " << toChar(it -> second) << endl;
}

void archive::printHist(){
    if(mapTable != NULL)
        for (histogram::iterator it = freqTable -> begin();it != freqTable -> end();it++)
            cout << it -> first << " => " << it -> second << endl;
}

void archive::printTree(){
    if(huffTree != NULL)
        huffTree -> print();
}

vector<char> archive::importBitArray(ifstream& file){
    vector<char> result;
    unsigned count;

    file.read((char*)(&count), sizeof(unsigned));
    for(unsigned i = 0;i < count;i++){
        char byte;
        file.read((char*)(&byte), sizeof(char));
        result.push_back(byte);
    }

    return result;
}

histogram* archive::importFreqTable(ifstream& file){
    histogram* freqTable = new histogram();
    unsigned count;

    file.read((char*)(&count), sizeof(unsigned));
    for(unsigned i = 0;i < count;i++){
        char first;
        int second;
        file.read((char*)(&first), sizeof(char));
        file.read((char*)(&second), sizeof(int));

        freqTable -> insert(histogramUnit(first, second));
    }

    return freqTable;
}

void archive::exportBitArray(vector<char>& data, ofstream& file){
    unsigned bitArrSize = data.size();
    file.write(reinterpret_cast<char*>(&bitArrSize), sizeof(unsigned));
    for(unsigned i = 0;i < data.size();i++){
        char byte = data[i];
        file.write(reinterpret_cast<char*>(&byte), sizeof(char));
    }
}

void archive::exportFreqTable(histogram* freqTable, ofstream& file){
    if(freqTable != NULL){
        int freqTableSize = freqTable -> size();
        file.write(reinterpret_cast<char*>(&freqTableSize), sizeof(int));

        for(histogram::iterator it = freqTable -> begin();it != freqTable -> end();it++){
            char first = it -> first;
            int second = it -> second;

            file.write(reinterpret_cast<char*>(&first), sizeof(char));
            file.write(reinterpret_cast<char*>(&second), sizeof(int));
        }
    }
}

string archive::toChar(string& str){
    string result;
    for(unsigned i = 0;i < str.size();i++)
        result += str[i] + '0';

    return result;
}

bool archive::getBit(int byte, int pos){
    return ((byte >> pos) & 1) == 1;
}

void archive::erase(){
    if(huffTree != NULL)
        delete huffTree;

    if(mapTable != NULL)
        delete mapTable;

    if(freqTable != NULL)
        delete freqTable;

    size = 0;
    oSize = 0;
}

vector<char> archive::getBitVector(string& input, map<char, string>* mapTable){
    vector<char> result;
    string currentByte;
    size = 0;

    for(unsigned i = 0;i < input.size();i++){
        string code = mapTable -> operator[](input[i]);
        for(unsigned j = 0;j < code.size();j++){
            currentByte += code[j];
            if(currentByte.size() == 8 || (i == input.size() - 1 && j == code.size() - 1)){
                result.push_back(binToNum(currentByte));
                currentByte = "";
            }
            size++;
        }
    }

    return result;
}

vector<char> archive::getBitVector(string& bitString){
    vector<char> result;
    unsigned length = (bitString.size() / 8 + 1) * 8;
    string byte;

    for(unsigned i = 0;i < length;i++){
        char current = 0;
        if(i < bitString.size())
            current = bitString[i];

        byte += current;
        if((i + 1) % 8 == 0){
            result.push_back(binToNum(byte));
            byte = "";
        }
    }

    return result;
}

char archive::binToNum(string& byte){
    char result = 0;
    for(unsigned i = 0;i < byte.size();i++)
        result += (byte[i]) * (int) pow(2, 7 - i);

    return result;
}

string archive::getBitString(string& input, map<char, string> mapTable){
    string result;
    for(unsigned i = 0;i < input.size();i++)
        result += mapTable[input[i]];

    return result;
}

map<char, string>* archive::getMapTable(tree* huffTree){
    map<char, string>* mapper = new map<char, string>();
    stack<char> bitStack;

    if(huffTree != NULL)
        mapBuilder(huffTree -> getRoot(), *mapper, bitStack);

    return mapper;
}

void archive::mapBuilder(node* huffTree, map<char, string>& mapper, stack<char>& bitStack){
    if(huffTree == NULL){
        if(!bitStack.empty())
            bitStack.pop();
        return;
    }

    if(huffTree -> leaf()){
        addStackToMapper(mapper, bitStack, huffTree -> value);
        if(!bitStack.empty())
            bitStack.pop();
        return;
    }

    bitStack.push(0);
    mapBuilder(huffTree -> left, mapper, bitStack);
    bitStack.push(1);
    mapBuilder(huffTree -> right, mapper, bitStack);

    if(!bitStack.empty())
        bitStack.pop();
}

void archive::addStackToMapper(map<char, string>& mapper, stack<char> bitStack, char value){
    string newKey;
    while(!bitStack.empty()){
        newKey = (char) (bitStack.top()) + newKey;
        bitStack.pop();
    }

    mapper.insert(pair<char, string>(value, newKey));
}

tree* archive::getHuffTree(histogram* freqTable){
    list<tree*> trees = initTrees(freqTable);
    if(trees.empty())
        return NULL;

    if(trees.size() == 1)
        return mergeTrees(trees.front(), new tree());

    while(trees.size() > 1){
        tree* smallest = popSmallestTree(trees);
        tree* secondSmallest = popSmallestTree(trees);
        tree* result = mergeTrees(smallest, secondSmallest);
        trees.push_back(result);
    }

    return trees.front();
}

tree* archive::popSmallestTree(list<tree*>& trees){
    if(trees.empty())
        return NULL;

    list<tree*>::iterator result = trees.begin();
    for(list<tree*>::iterator it = trees.begin();it != trees.end();it++)
        if((*it) -> getWeight() < (*result) -> getWeight())
            result = it;

    trees.remove(*result);

    return *result;
}

list<tree*> archive::initTrees(histogram* freqTable){
    list<tree*> result;
    for(histogram::iterator it = freqTable -> begin();it != freqTable -> end();it++)
        result.push_back(new tree(it -> first, it -> second));

    return result;
}

tree* archive::mergeTrees(tree* t1, tree* t2){
    tree* result = new tree(t1 -> getWeight() + t2 -> getWeight(), t1, t2);
    delete t1;
    delete t2;
    return result;
}

// Frequency table
histogram* archive::getHistogram(string& input){
    histogram* result = new histogram();
    for(unsigned i = 0;i < input.size();i++){
        char c = input[i];
        if(result -> find(c) == result -> end())
            result -> insert(histogramUnit(c, 1));
        else
            result -> operator[](c)++;
    }

    return result;
}

#endif // Archive_CPP
