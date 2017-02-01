#include <iostream>
#include <windows.h>
#include "archive.h"
using namespace std;

void renderMenu();
char getCharInput();
void executeComand(char, char*, archive&);

int main(){
    string input;
    archive arch;
    while(true){
        system("CLS");
        renderMenu();

        cout << "Command code: ";
        char command = getCharInput();
        char fileName[100];

        if(command != 'd' && command != 'r'){
            cout << "File name: ";
            cin >> fileName;
        }

        executeComand(command, fileName, arch);

        // Block console
        cin.get();
        cin.ignore();
    }

    return 0;
}

void executeComand(char command, char* fileName, archive& arch){
    switch(command){
    case 'o':
        arch.importArchive(fileName);
        cout << "Archived imported from - " << fileName << endl;
        break;
    case 'e':
        arch.exportExtract(fileName);
        cout << "Archived extracted at - " << fileName << endl;
        break;
    case 'c':
        arch.compressFile(fileName);
        cout << "Archived compressed at - " << fileName << endl;
        break;
    case 'd':
        cout << arch.decompress() << endl;
        break;
    case 'r':
        cout << arch.compressionRate() << endl;
        break;
    case '>':
        arch.exportArchive(fileName);
        cout << "Archived exported at - " << fileName << endl;
        break;
    default:
        cout << "Error - unknown command" << endl;
    }
}

char getCharInput(){
    char input;
    cin >> input;
    return input;
}

void renderMenu(){
    cout << "***************************************************" << endl;
    cout << " # MENU" << endl;
    cout << " 1) o [file_name] - Open archive file" << endl;
    cout << " 2) e [file_name] - Extract current open archive" << endl;
    cout << " 3) c [file_name] - Archive file" << endl;
    cout << " 4) d [file_name] - Decompress current file" << endl;
    cout << " 5) r             - Compression rate" << endl;
    cout << " 6) > [file_name] - Export current archive file" << endl;
    cout << "***************************************************" << endl;
}
