#include <iostream>
using namespace std;

#define AND_CMD +" & "+



int minimalTest(){
    int retCode = system("bit");
    return retCode;
}

int testHelp(){
    int retCode = system("bit help");
    return retCode;
}

int testAdd(){
    int retCode = system("bit add");
    return retCode;
}

int testClear(){
    int retCode = system("bit clear");
    return retCode;
}

int main()
{
    // Run the tests we have
    cout << minimalTest() << "\n";
    cout << testHelp() << "\n";
    
    return 0;
} 