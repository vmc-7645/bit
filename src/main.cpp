#include <filesystem>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
// #include <unordered_map> // for hashing https://stackoverflow.com/questions/8029121/how-to-hash-stdstring

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
    while (getline(inputFile, line)) fileLines.push_back(line); // get lines
    inputFile.close();
    return fileLines;
}

// Read timeline
// TODO read most recent timeline
// TODO read specified timeline relative to current entry

string hashVecStr(vector<string> const& vecOfStr) {

    // Convert string to uint32_t
    vector<uint32_t> vec;
    for (string str : vecOfStr) vec.push_back(static_cast<uint32_t>(stoul(str)));

    // Generate hash : https://stackoverflow.com/questions/20511347/a-good-hash-function-for-a-vector/72073933#72073933 
    size_t seed = vec.size();
    for(auto x : vec) {
        x = ((x >> 16) ^ x) * 0x45d9f3b;
        x = ((x >> 16) ^ x) * 0x45d9f3b;
        x = (x >> 16) ^ x;
        seed ^= x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return to_string(seed);
}

// Generate hash from file
string hashFromFile(const string& pathString){

    // read file data from location
    vector<string> linesFromDoc = fileLineData(pathString);

    // TODO: add path to linesfromdoc so that hash is more unique
    // might actually change this to save space. i.e. no need to create a completely new file if you're just renaming it.
    
    // get hash of vector<string>
    string fileHash = hashVecStr(linesFromDoc);

    return fileHash;
}

// Initialize timeline
void generateTimeline(){
    fs::create_directories("./.bit/filestore");
    ofstream ("./.bit/timeline");
    ofstream ("./.bit/queue");
    ofstream ("./.bit/settings");
    return;
}

string genQueueID(const string& pathString, string fileHash = "NA"){
    if (fileHash=="NA") fileHash = hashFromFile(pathString);// if filehash is not available, generate it.
    string ret = pathString+"::"+fileHash;
    return ret;
}

vector<string> getQueue(){
    vector<string> queueItems = fileLineData("./.bit/queue");
    return queueItems;
}

// Clear queue
vector<string> clearQueue(){
    // get queue items
    vector<string> queueItems = getQueue();

    // Deletes file references, but not files themselves.
    ofstream ofs;
    ofs.open("./.bit/queue", ofstream::out | ofstream::trunc);
    ofs.close();

    return queueItems;
}

// Clear queue
vector<string> deleteQueue(vector<string> queueItems = clearQueue()){
    // remove queue items from filestore
    // TODO
    string toDelete;
    for (const auto& queueItem : queueItems) {
        toDelete = "./.bit/filestore/"+queueItem;
        const int result = filesystem::remove(toDelete);
        if( result == 0 ){
            printf( "success\n" );
        } else {
            printf( "%s\n", strerror( errno ) ); // No such file or directory
        }
    }

    return queueItems;
}



void addQueue(const string& pathString, string fileHash = "NA"){
    
    // if filehash is not available, generate it.
    if (fileHash=="NA") fileHash = hashFromFile(pathString);
    
    // get contents from pathString
    // TODO 

    // add filename & hash to queue
    // TODO

    return;
}

bool inQueue(const string& pathString, string fileHash = "NA", vector<string> queueItems = getQueue()){
    
    // if filehash is not available, generate it.
    if (fileHash=="NA") fileHash = hashFromFile(pathString);
    
    // iterate through items in queue
    for (const auto& queueItem : queueItems) {
        if (queueItem==genQueueID(pathString,fileHash)) return true;
    }
    return false;
}

bool inFilestore(string fileHash){

    // pull files in filestore
    // TODO

    // iterate through filenames

        // if filename matches fileHash, return true
        // TODO

    return false;
}

void addFilestore(const string& pathString, string fileHash = "NA"){

    // if filehash is not available, generate it.
    if (fileHash=="NA") fileHash = hashFromFile(pathString);

    // if item not in filestore, add to filestore
    // TODO

    return;
}


// Save queue to timeline
void queueToTimeline(){
    if (!fs::exists("./.bit/timeline")) return generateTimeline(); //If it doesn't exist, generate.
    vector<string> queueItems = getQueue();
    string fileLoc;
    string fileHash;

    // iterate through files in queue
    for (const auto& queueItem : queueItems) {
        
        // if file exists in filestore, add to timeline. 
        vector<string> vec = split(queueItem,"::");
        if (!vec.empty()) {
            fileLoc = vec.front();
            fileHash = vec.back();
            if (inQueue(fileLoc,fileHash)){
                
                // Add item to filestore
                addFilestore(fileLoc,fileHash);
            }
        }
    }

    // Clear queue
    clearQueue();

    return;
}

// Save files to queue
void currentToQueue(){
    if (!fs::exists("./.bit/timeline")) generateTimeline(); //If it doesn't exist, generate.
    
    // get files.
    vector<string> toQueue = listFiles();
    
    // get queue items.
    vector<string> queueItems = clearQueue();

    // iterate through files.
    for (const auto& fileLoc : toQueue) {
        
        // get hash for each file.
        string fileHash = hashFromFile(fileLoc);

        // if hash exists in queue, with the same name, skip.
        if (inQueue(fileLoc,fileHash,queueItems)) continue;

        // if hash does not exist in queue, push to queue and filestore.
        addQueue(fileLoc,fileHash);

        // for every queue not here, delete from filestore
        // TODO HIGH PRIORITY
    }
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
    queueToTimeline();
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
    } else if (cmdname=="default" || cmdname=="d"){
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