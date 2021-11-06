#pragma warning(disable : 4996)

#define WINVER 0x0A00
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>

using namespace std;

#define BUFSIZE MAX_PATH
#define FILESYSNAMEBUFSIZE MAX_PATH
#define MAX_KEY_LENGTH 255


#define INFO_BUFFER_SIZE 32767


double PCFreq = 0.0;
__int64 CounterStart = 0;



int main() {

    LARGE_INTEGER li;
    LARGE_INTEGER t;

    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
    QueryPerformanceFrequency(&t);
    QueryPerformanceCounter(&t);
    cout << "2.2.  CPU clock count: " << double((t.QuadPart - CounterStart) * 1000000) / PCFreq << "  us \n";
    PCFreq = double(li.QuadPart);
    printf("\n2.1.  CPU frequency: %ll  Hz\n", li);
}
