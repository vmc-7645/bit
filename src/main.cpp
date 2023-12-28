#include <filesystem>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
namespace fs = filesystem;

// 
bool runDefault = false;
string defaultCommand = "help";

// 
const fs::path currentPath = fs::current_path();
const string currentPathString = currentPath.string();

// List files at directory
vector<string> listFiles(const string& pathString = currentPathString) {
    vector<string> pathsVector;
    if (!fs::exists(pathString)) {
        cerr << "Error: Directory does not exist\n";
        return pathsVector;
    }
    for (const auto& entry : fs::directory_iterator(pathString)) {
        fs::path tempPath = entry.path();
        pathsVector.push_back(tempPath.string());
    }
    return pathsVector;
}

// Read file data
vector<string> fileLineData(const string& pathString) {
    vector<string> fileLines;
    if (!fs::exists(pathString)) {
        cerr << "Error: File does not exist\n";
        return fileLines;
    }
    ifstream inputFile(pathString);
    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open file: " << pathString << "\n";
        return fileLines;
    }
    string line;
    while (getline(inputFile, line)) {
        fileLines.push_back(line);
    }
    inputFile.close();
    return fileLines;
}

// Print vector<string>
void printVS(vector<string> toPrint){
    for (const auto& line : toPrint) {
        cout << line << endl;
    }
    return;
}



// User facing functions
int help(){
    cout << "Help command ran.\n"
            "-----\n"
            "bit help : runs this command.\n"
            "bit add : adds current history to timeline.\n"
            "bit tag : allows for jumping back to most recently add.\n"
            "bit at : runs add and tag consecutively.\n"
            "bit jump [relative index] : jumps relative to current, default is -1.\n"
            "bit look [max view] : lists history of changes.\n"
            "bit clear [minimum relative index] : clears history, default is 0.\n"
            "bit default [command] : the default bit command, default is 'help'.";
    return 0;
}

int add(){
    vector<string> files = listFiles();
    vector<string> fileData = fileLineData("C:\\Users\\vinmc\\Documents\\Programing\\Projects\\bit\\bin\\test.txt");

    printVS(files);
    printVS(fileData);
    
    cout << "Added to timeline";
    return 0;
}

int tag(){
    cout << "Tagged in timeline";
    return 0;
}

int jump(){
    cout << "Jumped";
    return 0;
}

int look(){
    cout << "View timeline";
    return 0;
}

int clear(){
    cout << "Cleared items from timeline";
    return 0;
}

int def(){
    cout << "Defined new default command";
    return 0;
}

// Command lookup
int commandLookup(string cmdname){
    if (cmdname=="help" || cmdname=="h"){
        return help();
    } else if (cmdname=="add" || cmdname=="a"){
        return add();
    } else if (cmdname=="tag" || cmdname=="t"){
        return tag();
    } else if (cmdname=="at"){
        add();
        tag();
        return 0;
    } else if (cmdname=="jump" || cmdname=="j"){
        return jump();
    } else if (cmdname=="look" || cmdname=="l"){
        return look();
    } else if (cmdname=="clear" || cmdname=="c"){
        return clear();
    }else if (cmdname=="default" || cmdname=="d"){
        return def();
    }

    cout << "No valid bit command specified, running 'help' as default.\n";
    help();
    return 0;
}

int main(int argc, char *argv[])
{
    // TODO load in program data

    if(argc == 1) return commandLookup(defaultCommand);
    if(argc != 3) runDefault = true; // means that no atributes were used for this command, run with default values.
    
    commandLookup(argv[1]);
    return 0;
} 