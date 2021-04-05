#include <stdio.h>
// #include <sys/sysinfo.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <time.h>
#include <regex>
#include <vector>
#include <unistd.h>
#include <stdlib.h>
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
    int numComms = 13;
    std::string validCommands[numComms] = {"exit", "ls", "cd", "cat", "clear", "pwd", "mkdir", "rmdir", "touch", "prompt", "sysinf", "meminf", "man"}; 

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

bool isValChar(std::string character) {
    if (character.length() > 1) {
        std::cout << "Invalid switch, [Character] argument given is more than 1 character\n";
        return false;
    }

    else {
        return true;
    }
}

/*  
*    Prompt switch with congifugrations:
*   'prompt' -s [string] -c [character]
*   'prompt' -sc [string] [character]   
*   'prompt' -d 
*/ 
int swtichPrompt(std::vector<std::string> args, std::string *promptStr) {
    int statusCode = 0;
    std::string endChar = ">";
    std::string newPrompt = "";
    std::string invalConfMsg("\nInvalid configuration of swtiches\n"
        "Usage:\n'prompt' -s [string] -c [character]\n"
        "'prompt' -sc [string] [character]\n'prompt' -s [string]\n'prompt' -d\n");
    
    if (args.size() == 3 || args.size() == 4 || args.size() == 5) {

        if (args.size() == 5) {

            std::regex strReg("(-s)");
            std::regex charReg("(-c)");

            // -s [string] -c [character]
            if (std::regex_match(args[1], strReg) && std::regex_match(args[3], charReg)) {
                newPrompt = args[2];
                
                if (isValChar(args[4])) {
                    endChar = args[4];
                    *promptStr = newPrompt + endChar;
                    statusCode = 0;
                }
                else {
                    statusCode = 2;
                }
            }

            // -c [character] -s [string]
            else if (std::regex_match(args[1], charReg) && std::regex_match(args[3], strReg)) {
                newPrompt = args[4];

                if (isValChar(args[2])) {
                    endChar = args[2];
                    *promptStr = newPrompt + endChar;
                    statusCode = 0;
                }

                else {
                    statusCode = 2;
                }
            }

            else {
                std::cout << invalConfMsg << std::endl;
                statusCode = 1;
            }
            
        }

        else if (args.size() == 4) {
            std::regex conf1("(-sc)");
            std::regex conf2("(-cs)");

            // -sc [string] [character]
            if (std::regex_match(args[1], conf1)) {
                newPrompt = args[2];
                
                if (isValChar(args[3])) {
                    endChar = args[3];
                    *promptStr = newPrompt + endChar;
                    statusCode = 0;
                }
                else {
                    statusCode = 2;
                }
            }

            // -cs [character] [string]
            else if (std::regex_match(args[1], conf2)) {
                newPrompt = args[3];

                if (isValChar(args[2])) {
                    endChar = args[2];
                    *promptStr = newPrompt + endChar;
                    statusCode = 0;
                }
                else {
                    statusCode = 2;
                }
            }

            else {
                std::cout << invalConfMsg << std::endl;
                statusCode = 1; 
            }
        }

        // -s [string]
        else {
            std::regex conf3("(-s)");
            
            if (std::regex_match(args[1], conf3)) {
                newPrompt = args[2];
                *promptStr = newPrompt + endChar;
                statusCode = 0;
            }

            else {
                std::cout << invalConfMsg << std::endl;
                statusCode = 1;
            }
        }
    }

    // Date configuration: 'prompt' -d
    else if (args.size() == 2) {

        std::regex dateConf("(-d)");
        
        if (std::regex_match(args[1], dateConf)) {

            // Help with formatting time correctly https://www.geeksforgeeks.org/strftime-function-in-c/
            time_t t;
            struct tm *tmp;
            char MY_TIME[50];
            time(&t);

            tmp = localtime(&t);

            std::strftime(MY_TIME, sizeof(MY_TIME), "%x", tmp);

            newPrompt = MY_TIME;
            endChar = ">";
            *promptStr = newPrompt + endChar;
            statusCode = 0;
        }

        else {
            std::cout << "Invalid configuration, only valid single argument is 'prompt' -d\n";
            statusCode = 3;
        }
    }

    // Invalid configuration of switches
    else {
        std::cout << invalConfMsg << std::endl;
        statusCode = 1;
    }

    return statusCode;
}

// int getManPage(std::string commName, std::vector<std::string> args, ) {

//     std::regex helpReg1("(-h)");
//     std::regex helpReg2("(-help)");

//     if (args[0].find("-") == std::string::npos) {
//         std::cout << "Invalid syntax in arguments, use '-' to begin any argument" << "\n";
//         exitCode = 2;
//     }

//     if (std::regex_match(argsArr[1], helpReg1) || std::regex_match(argsArr[1], helpReg2)) {
//         // Display manual page here
//         std::cout <<" MANUAL PAGE FOR PWD" << "\n";
//     }

//     else {
//         std::cout << "Unknown argument given, valid options are '-h' and '-help'\n";
//         exitCode = 1;
//     }
// }

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
    std::string prompt = "cwushell>";
    std::string line = "";

    while (command != "exit") {
        std::cout << prompt << " ";

        std::getline(std::cin, line);
        std::vector<std::string> argsArr = tokenizeArray(line);

        if (argsArr.empty()) {
            continue;
        }
        else {
            command = argsArr[0];
        }

        // Shortcut to lowercase from https://www.tutorialspoint.com/how-to-convert-std-string-to-lower-case-in-cplusplus
        std::transform(command.begin(), command.end(), command.begin(), ::tolower);

        if (checkCommand(command) == 0) {
            
            // Used to concatenate command + arguments for direct system calls
            // (Only used for ls, cat, mkdir, rmdir, touch)
            std::string sysCommStr = "";
            for (int i = 0; i < argsArr.size(); i++) {
                    
                if (i == argsArr.size() - 1) {
                    sysCommStr.append(argsArr[i]);
                }
                else {
                    sysCommStr.append(argsArr[i] + " ");
                }
            }

            char linuxComm[sysCommStr.size()+1];
            strcpy(linuxComm, sysCommStr.c_str());

            if (command == "ls") {
                int statusCode = system(linuxComm);
                exitCode = statusCode;
            }

            else if (command == "cat") {
                int statusCode = system(linuxComm);
                exitCode = statusCode;
                std::cout << "\n";
            }

            else if (command == "mkdir") {
                int statusCode = system(linuxComm);
                exitCode = statusCode;
            }

            else if (command == "rmdir") {
                int statusCode = system(linuxComm);
                exitCode = statusCode;
            }

            else if (command == "touch") {
                int statusCode = system(linuxComm);
                exitCode = statusCode;
            }

            // Credit to Stack Overflow for the correct escape code 
            // https://stackoverflow.com/questions/5367068/clear-a-terminal-screen-for-real
            else if (command == "clear") {
                std::cout << "\033c";
            }

            else if (command == "cd") {
                std::string directArgs = "";

                for (int i = 1; i < argsArr.size(); i++) {
                    
                    if (i == argsArr.size() - 1) {
                        directArgs.append(argsArr[i]);
                    }
                    
                    else {
                        directArgs.append(argsArr[i] + " ");
                    }
                }

                char directComm[directArgs.size()+1];
                strcpy(directComm, directArgs.c_str());

                int statusCode = chdir(directComm);
                exitCode = statusCode;

                if (statusCode != 0) {
                    std::cout << "Error: directory path not found" << "\n";
                }
            }

            else if (command == "pwd") {

                if (argsArr.size() < 3) {
                    
                    if (argsArr.size() == 1) {
                        std::cout << get_current_dir_name() << "\n";
                    }

                    else {
                        std::regex helpReg1("(-h)");
                        std::regex helpReg2("(-help)");

                        if (argsArr[1].find("-") == std::string::npos) {
                            std::cout << "Invalid syntax in arguments, use '-' to begin any argument" << "\n";
                            exitCode = 2;
                        }

                        if (std::regex_match(argsArr[1], helpReg1) || std::regex_match(argsArr[1], helpReg2)) {
                            // Display manual page here
                            std::cout << "MANUAL PAGE FOR PWD" << "\n";
                        }

                        else {
                            std::cout << "Unknown argument given, valid options are '-h' and '-help'\n";
                            exitCode = 1;
                        }
                    }
                }
                
                else {
                    std::cout << "Inavalid number of arguments for 'pwd'\n" << "Usage: 'pwd' or 'pwd' [-help / -h]\n";
                }
            }


            /* 
            *   [----- Below are commands which do not use any system calls -----]
            */  
            else if (command == "exit") {
                
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
                    << "Usage: 'exit' or 'exit' [number]" << "\n";
                }
            }

            // Switches are -s -c -d
            else if (command == "prompt") {

                if (argsArr.size() == 1) {
                    prompt = "cwushell>";
                }
                else {

                    if (argsArr[1].find("-help") != std::string::npos 
                        || argsArr[1].find("-h") != std::string::npos) {

                        // Display manual page here                            
                    }
                    else {
                        exitCode = swtichPrompt(argsArr, &prompt);
                    }
                }
            }

            else if (command == "sysinf") {

            }

            else if (command == "meminf") {
                
                if (argsArr.size() < 2 || argsArr[1].find("-help") != std::string::npos 
                    || argsArr[1].find("-h") != std::string::npos) {

                    
                    // Display manual page here
                }

                else {
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
                        std::string switches = "";

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

