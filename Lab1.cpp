#include <stdio.h>
// #include <sys/sysinfo.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <regex>
#include <vector>
#include <unistd.h>
#include <algorithm>

// Tokenizes the user input to parse commands
std::vector<std::string> tokenizeArray(std::string line) {

    std::vector<std::string> tokens;

    char char_array[line.size() + 1];
    strcpy(char_array, line.c_str());

    // Remaining lines in function from:
    // https://www.educative.io/edpresso/splitting-a-string-using-strtok-in-c
    
    char * token = strtok(char_array, " ");

    while (token != NULL) {
       tokens.push_back(token);
       token = strtok(NULL, " ");
    }

    return tokens;
}

// Validates the entered command
int checkCommand(std::string comm) {
    
    int exitCode = 0;
    bool isValid = false;
    int numComms = 12;
    std::string validCommands[numComms] = {"exit", "ls", "cd", "cat", "clear", "pwd", "mkdir", "rmdir", "prompt", "sysinf", "meminf", "man"}; 

    for (int i = 0; i < numComms; i++) {

        if (validCommands[i] == comm) {
            isValid = true;
        }
    }

    // Displays error message and tries to guess the command if it's close to a valid one
    if(! isValid) {

        std::vector<std::string> guesses;
        for (int i = 0; i < numComms; i++) {
            
            if (validCommands[i].find(comm) != std::string::npos) {
                guesses.push_back(validCommands[i]);
            }
        }

        if (! guesses.empty()) {
            std::cout << "Command '" << comm << "' not found, did you mean ";
             
            for(int i = 0; i < guesses.size(); i++) {
                std::cout << "'" << guesses[i] << "'"; 

                if (i < guesses.size() -1) {
                    std::cout << " or ";
                }
            }
            std::cout << "?" << std::endl;
            exitCode = 1;
        }

        else {
            std::cout << "Command '" << comm << "' not found" << std::endl;
            exitCode = 1;
        }
    }

    return exitCode;
}

// Gets system memory information given different parameters as switches
int getMemInfo(std::string params) {

    if (params.find("a") != std::string::npos) {
        
        std::string line;
        std::ifstream file("/proc/meminfo", std::ifstream::in);

        while (std::getline(file, line)) {
            std::cout << line << std::endl;
        }
    }

    else {

        std::vector<std::string> infoArr;

        if (params.find("s") != std::string::npos) {
            
            std::string line;
            std::ifstream file("/proc/meminfo", std::ifstream::in);

            while (std::getline(file, line)) {
                
                if (line.find("SwapFree") != std::string::npos) {

                    std::vector<std::string> memInfArr = tokenizeArray(line);

                    std::istringstream iss(memInfArr[1]);
                    long long int swapFree = 0;

                    // SwapFree in kb originally; converts to bytes
                    iss >> swapFree;
                    swapFree = swapFree * 1024;

                    std::string swapStr = memInfArr[0] + " " + std::to_string(swapFree) + " bytes";
                    infoArr.push_back(swapStr);
                }
            }
        }

        if (params.find("u") != std::string::npos) {
            
            std::string line;
            std::ifstream file("/proc/meminfo", std::ifstream::in);

            while (std::getline(file, line)) {
                
                if (line.find("MemFree") != std::string::npos) {

                    std::vector<std::string> memInfArr = tokenizeArray(line);

                    std::istringstream iss(memInfArr[1]);
                    long int memFree = 0;

                    // MemFree in kb originally; converts to bytes
                    iss >> memFree;
                    memFree = memFree * 1024;

                    std::string memStr = memInfArr[0] + " " + std::to_string(memFree) + " bytes";
                    infoArr.push_back(memStr);
                }
            }
        }

        /* 
        *   "Shmem: Total used shared memory (shared between several processes, 
        *   thus including RAM disks, SYS-V-IPC and BSD like SHMEM)"
        *   https://access.redhat.com/solutions/406773
        */
        if (params.find("S") != std::string::npos) {

            std::string line;
            std::ifstream file("/proc/meminfo", std::ifstream::in);

            while (std::getline(file, line)) {
                
                if (line.find("Shmem") != std::string::npos) {

                    std::vector<std::string> memInfArr = tokenizeArray(line);

                    std::istringstream iss(memInfArr[1]);
                    long int shmem = 0;

                    // Shmem in kb originally; converts to bytes
                    iss >> shmem;
                    shmem = shmem * 1024;

                    std::string shmemStr = memInfArr[0] + " " + std::to_string(shmem) + " bytes";
                    infoArr.push_back(shmemStr);
                }
            }
        }

        // Display all 
        for (int i = 0; i < infoArr.size(); i++) {
            std::cout << infoArr[i] << std::endl;
        }

    } // End of else

    std::cout << std::endl;
    return 0;

} // End of getMemInfo()


// Compile me with ---> g++ -o lab1 Lab1.cpp
/*
    CWU SHELL: 

    Description:
        First validates if command entered is known. If command is valid then all arguments
        are passed to the appropriate command where the arguments (switches) are also checked
        for syntax and validity. Upon success appropriate commands will be executed. If any
        errors arise in the shell or a specific command, they will pass corresponding error codes
        as the terminating exit code such as "Proccess exited with code 1".
*/
int main() {

    int exitCode = 0;
    std::string command = "";
    std::string prompt = "cwushell> ";
    std::string line = "";

    while (command != "exit") {
        std::cout << prompt;

        std::getline(std::cin, line);
        std::vector<std::string> argsArr = tokenizeArray(line);

        command = argsArr[0];

        // Shortcut to lowercase from https://www.tutorialspoint.com/how-to-convert-std-string-to-lower-case-in-cplusplus
        std::transform(command.begin(), command.end(), command.begin(), ::tolower);

        if (checkCommand(command) == 0) {
            
            if (command == "exit") {
                
                if (argsArr.size() <= 2) {
                    if (argsArr.size() == 2) {
                        
                        std::istringstream iss(argsArr[1]);

                        if (iss >> exitCode) {
                            iss >> exitCode;
                        }

                        else {
                            std::cout << "Invalid argument: second arguement for 'exit' command should be an integer" << "\n";
                        }
                    }
                } 

                else {
                    std::cout << "Invalid number of arguments" << "\n"
                    << "Usage: [exit] or [exit] [number]" << "\n";
                }
            }

            if (command == "ls") {}

            // Credit to Stack Overflow for the correct escape code 
            // https://stackoverflow.com/questions/5367068/clear-a-terminal-screen-for-real
            else if (command == "clear") {
                std::cout << "\033c";
            }

            else if (command == "cd") {}

            else if (command == "cat") {

            }

            else if (command == "pwd") {
                std::cout << get_current_dir_name() << "\n";
            }

            // else if (command == "mkdir") {}
            // else if (command == "rmdir") {}

            else if (command == "prompt") {}

            else if (command == "sysinf") {}

            // [Note]: should combine variable number of switches together as one string to pass
            // ie. meminf -s -u -S --> becomes getMemInfo("-suS")
            else if (command == "meminf") {
                
                std::string switches = "";

                if (argsArr.size() < 2 || argsArr[1].find("-help") != std::string::npos 
                    || argsArr[1].find("-h") != std::string::npos) {

                    // Display manual page here
                }

                else {

                    std::string switchOptons = "-suSa";
                    bool isValid = true;

                    // Checks for any errors in arguments (switches)
                    for (int i = 1; i < argsArr.size(); i++) {
                        
                        std::regex valArgs ("(-[sSua]+)");

                        if (argsArr[i].find("-") == std::string::npos) {
                            
                            std::cout << "Invalid syntax in arguments, use '-' to begin any argument" << "\n";
                            exitCode = 2;

                            isValid = false;
                            break;
                        }

                        if (! std::regex_match(argsArr[i], valArgs)) {
                            
                            std::cout << "Unknown argument given, valid options are 's', 'S', 'a', and 'u'" << "\n";
                            exitCode = 1;

                            isValid = false;
                            break;
                        }
                    }

                    // If switch arguments given are valid then all arguments are joined together
                    if (isValid) {

                        for (int i = 1; i < argsArr.size(); i++) {
                            
                            std::string curArg = argsArr[i];
                            switches.append(curArg.substr(1));
                        }

                        exitCode = getMemInfo(switches);
                    }
                }

            } // End of meminf command  

        } // End command checks

    } // End of main while loop

    std::cout << "\n" << "Process exited with code " << exitCode << "\n";
    return exitCode;
}

