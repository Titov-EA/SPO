#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <sstream>
#include <string>
#include <map>

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

    std::string kom (MAX_BUFFER_SIZE, '\0');
    std::string keyword,key,value,response {};
    DWORD bytes;
    std::map<std::string, std::string> data {};
    while (true)
    {
        std::cout << "Waiting for a client...\n";
        if (!ConnectNamedPipe(pipe, nullptr))
        {
           std::cout << "Error:"<< GetLastError()<<'\n';;
           CloseHandle(pipe);
           return false;
        }
        else
        {
           std::cout << "connection successful"<<'\n';;
           return false;
        }
       while(true)
       {

            std::cout << "Wait for the client's com...\n";
            auto fr = ReadFile(pipe, &kom[0], kom.size(), &bytes, nullptr);
            if (!fr)
            {
                auto err = GetLastError();
                std::cout<<"Error:"<<err;
            }
            kom.resize(kom.find('\0'));
            std::cout << kom;
            std::istringstream parser {kom};
            parser >> std::ws >> keyword;
            if (keyword == "set")
                {
                    parser >> key >> value;
                    data[key] = value;
                    response = "acknowledged";
                }
            else if (keyword == "get")
                {
                    parser >> key;
                    if (data.find(key) != data.end())
                        response = "found " + data[key];
                    else
                        response = "missing";
                }
            else if (keyword == "list")
                {
                    for (auto i = data.begin(); i != data.end(); ++ i)
                        response += i->first + " ";
                }
            else if (keyword == "delete")
                {
                    parser >> key;
                    auto del = data.find(key);
                    if (del != data.end())
                    {
                        data.erase(del);
                        response = "deleted";
                    }
                    else
                        response = "missing";
                }
            else if (keyword == "quit")
            {
                DisconnectNamedPipe(pipe);
                kom.replace(0, kom.size(), kom.size(), '\0');
                kom.resize(MAX_BUFFER_SIZE, '\0');
                break;
            }
            else
            {
                 std::cerr << "Incorrect command! (command: \"" << command << "\")\n";
                response = "incorrect command";
            }


            auto fr1 = WriteFile(pipe, response.c_str(), response.size(), &bytes, nullptr);
            if (!fr1)
            {
                auto err = GetLastError();
                std::cout<<"Error:"<<err;
            }


             kom.replace(0, kom.size(), kom.size(), '\0');
             kom.resize(MAX_BUFFER_SIZE, '\0');
             response.clear();
             keyword.clear();
             key.clear();
             value.clear();
    }

        char answer;
        bool exit = false;
        std::cout << "Do you want to delete pipe \"" << pipe_name << "\" (y/n)?: ";
        while (std::cin >> answer)
        {
            if (answer == 'yes')
            {
                (CloseHandle(pipe),
                                  "occurred while closing pipe",
                                  pipe_name);
                exit = true;
                break;
            }
            else if (answer == 'no')
                break;
            else
            {
                std::cout << "(y/n): ";
                continue;
            }
        }
        if (exit)
            break;
    }
}
