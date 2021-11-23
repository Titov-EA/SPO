#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <sstream>
#include <string>

#define MAX_BUFFER_SIZE 64

int main()
{
    std::string pipe_name;
    std::cout<<"Enter pipe name: ";
    std::cin >> pipe_name;
    auto pipe_path = "\\\\.\\pipe\\" + pipe_name;
    auto pipe = CreateFile(pipe_path.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (pipe == INVALID_HANDLE_VALUE)
    {
        std::cout << "Error:"<< GetLastError()<<std::endl;
    }

    std::string kom (64,'\0');
    DWORD written;
    DWORD read;
    while (true)
    {
        std::cout << "> ";
        std::getline(std::cin >> std::ws, kom);
        WriteFile(pipe, kom.c_str(), kom.size(), &written, nullptr);
        if (kom == "quit")
            break;

        char msg_buf[MAX_BUFFER_SIZE];
        ReadFile(pipe, msg_buf, MAX_BUFFER_SIZE, &read, nullptr);
        msg_buf[read] = 0;
        std::cout << msg_buf << '\n';

    CloseHandle(pipe);
    return EXIT_SUCCESS;
}
