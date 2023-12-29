#include <filesystem>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map> // for hashing https://stackoverflow.com/questions/8029121/how-to-hash-stdstring

using namespace std;
namespace fs = filesystem;

// 
bool runDefault = false;
string defaultCommand = "help";
vector<string> ignoredFiles;

// 
const fs::path currentPath = fs::current_path();
const string currentPathString = currentPath.string();

// Split string via delimiter
vector<string> split(string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;
    while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }
    res.push_back (s.substr (pos_start));
    return res;
}

// Generate hash from file
string hashFromFile(const string& pathString){

    return "";
}


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

// 
vector<string> fileSizes(const vector<string> pathStrings) {
    vector<string> fileSizes;
    string fileSize;
    for (const auto& fileLoc : pathStrings) {
        fileSize = fs::file_size(fileLoc);
        fileSizes.push_back(fileSize);
    }
    return fileSizes;
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

// Initialize timeline
void generateTimeline(){
    fs::create_directories("./.bit/filestore");
    ofstream ("./.bit/timeline");
    ofstream ("./.bit/queue");
    ofstream ("./.bit/settings");
    return;
}


// Save queue to timeline
void queueToTimeline(){
    if (!fs::exists("./.bit/timeline")) return generateTimeline(); //If it doesn't exist, generate.
    
    // if it does exist, load timeline
    // vector<string> rlines = fileLineData("./.bit/timeline");

    // Get queue

    // iterate through files

        // if file exists in filestore, remove from queue and add to timeline

    return;
}

// Save files to queue
void currentToQueue(){
    if (!fs::exists("./.bit/timeline")) generateTimeline(); //If it doesn't exist, generate.
    
    // get files.

    // iterate through files.

        // get hash for each file.

        // if hash exists in queue, skip.

        // if hash does not exist in queue, push to queue and filestore.

        // for every queue item that exists in filestore but not in new queue or timeline, remove it.

    return;
} 



// Load in app data
void loadAppData(){
    if (!fs::exists("./.bit/settings")) return; //If it doesn't exist.
    
    // if it does exist, load data in
    vector<string> rlines = fileLineData("./.bit/settings");
    string line;
    string appDataVar;
    string appDataVal;
    for (const auto& l:rlines) {
        vector<string> vec = split(l,":");
        if (!vec.empty()) {
            appDataVar = vec.front();
            appDataVal = vec.back();
            if (appDataVal == "defaultCommand") {
                defaultCommand = appDataVal;
            } else if (appDataVal == "ignore") {
                ignoredFiles.push_back(appDataVal);
            }
        }
    }
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
    cout << "Help:\n"
            "-----\n"
            "bit help : runs this command.\n"
            "bit add : adds current history to timeline.\n"
            "bit tag : allows for jumping back to most recently add.\n"
            "bit at : runs add and tag consecutively.\n"
            "bit jump [relative index] : jumps relative to current, default is -1.\n"
            "bit look [max view] : lists history of changes.\n"
            "bit clear [minimum relative index] : clears history, default is 0.\n"
            "bit delete : removes all traces of bit at this location.\n"
            "bit default [command] : the default bit command, default is 'help'.";
    return 0;
}

int add(){
    // vector<string> files = listFiles();
    // printVS(files);
    
    currentToQueue();

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
    loadAppData();
    if(argc == 1) return commandLookup(defaultCommand);
    if(argc != 3) runDefault = true; // means that no atributes were used for this command, run with default values.
    
    commandLookup(argv[1]);
    return 0;
} 