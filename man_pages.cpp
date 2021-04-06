#include "man_pages.h"
#include <string>
#include <iostream>

int man_pages::getManual() {
    std::string man("[--------------- CWU SHELL ---------------]\n\n"
    "ABOUT\n"
    "\tThe CWU Shell is a micro shell which allows common UNIX commands to be executed.\n"
    "\tIt is developed and tested within the Debian Ubuntu 20.04 linux terminal and\n"
    "\tas a lab assignment for CS470 Operating Systems.\n\n"
    "AUTHOR\n"
    "\tKyle Brown\n\n"
    "COMMANDS\n"
    "\t[cat] - Concatenates files to standard output.\n\n"
    "\t[cd] - Changes directory.\n\n"
    "\t[clear] - Clears the terminal window.\n\n"
    "\t[exit] - Terminates the shell process.\n\n"
    "\t[ls] - Lists directory contents.\n\n"
    "\t[man] - Manual, an interface to the system reference manuals.\n\n"
    "\t[meminf] - Prints system memory information depending on switches.\n\n"
    "\t[mkdir] - Makes directory.\n\n"
    "\t[prompt] - Changes the prompt or if given no parameter, changes to default.\n\n"
    "\t[pwd] - Prints current/working directory.\n\n"
    "\t[rm] - Removes a specified file.\n\n"
    "\t[rmdir] - Removes directory.\n\n"
    "\t[sysinf] - Lists system information depending on switches.\n\n"
    "\t[touch] - Creates a new file.\n\n"
    "[HELP]\n"
    "\tAny of the commands listed above may be entered followed by '--help' to display an\n"
    "\tindividual manual page detailing the command and all available switches it has.\n\n"
    "[EXIT CODES]\n"
    "\t0	is OK,\n\n"
    "\t1	if command entered was not found,\n\n"
    "\t-1	if error occurs retrieving a manual page.\n");

    std::cout << man << "\n";
    return 0;
}

int man_pages::getMeminfHelp() {
    std::string man("[--------------- CWU SHELL ---------------]\n\n"	
    "NAME\n"
    "\tmeminf - prints system memory information depending on switches.\n\n"
    "SYNOPSIS\n"
    "\tmeminf [OPTION]...\n"
    "\tmeminf [OPTION 1]... [OPTION 2]... [OPTION N]...\n\n"	
    "DESCRIPTION\n"
    "\tLists information about the system's memory according to the linux /proc/meminfo\n"
    "\tfile. Information for RAM or other volatile memory will fluctuate with restarting\n"
    "\tthe shell but not while within the shell.\n\n"
    "\tList of availible switches are:\n\n"
    "\t-a (List all memory information availible)\n\n"
    "\t-s (Lists the available free swap space in bytes)\n\n"
    "\t-S (Lists the available total shared memory in bytes)\n\n"
    "\t-u (Lists the available free memory in bytes)\n\n"
    "EXIT CODES\n"
    "\t0	is OK,\n\n"
    "\t1	if an unknown switch given,\n\n"
    "\t2	if invalid syntax in parameters.\n\n");

    std::cout << man << "\n";
    return 0;
}

int man_pages::getSysinfHelp() {
    std::string man("[--------------- CWU SHELL ---------------]\n\n"	
    "NAME\n"
	"\tsysinf - prints system and network information depending on switches.\n\n"
    "SYNOPSIS\n"
	"\tsysinf [OPTION]...\n"
	"\tsysinf [OPTION 1]... [OPTION 2]... [OPTION N]...\n\n"
    "DESCRIPTION\n"
	"\tLists information about the system network properties and cpu information.\n"
	"\tcommands -h and -i will display network information while -p lists all processors\n"
	"\tdetected and the type of processor they are.\n\n"
	"\tList of availible switches are:\n\n"
	"\t-h (Lists the system hostname)\n\n"
	"\t-i (Lists the ip address of the host)\n\n"
	"\t-p (Lists the number of processors and their type)\n\n"
    "EXIT CODES\n"
	"\t0	is OK,\n\n"
	"\t1	if an unknown switch given,\n\n"
	"\t2	if invalid syntax in parameters.\n");

    std::cout << man << "\n";
    return 0;
}

int man_pages::getPromptHelp() {
    std::string man("[--------------- CWU SHELL ---------------]\n\n"	
    "NAME\n"
	"\tprompt - Changes the prompt or if given no parameter, changes to default.\n\n"
    "SYNOPSIS\n"
	"\tprompt -s [string] -c [character] (-s and -c are exchangeable)\n"
	"\tprompt -sc [string] [character] (-s and -c are exchangeable)\n"
	"\tprompt -s [string]\n"
	"\tprompt -d\n"
	"\tprompt\n\n"
    "DESCRIPTION\n"
	"\tAllows for the changing of the shell prompt which is by default 'cwushell>'.\n"
	"\tIf the prompt command is invoked without any arguments the prompt changes to the\n"
	"\tdefault. Using -s and -c allow changing the prompt such as in the following example\n"
	"\t'prompt -s CS470 -c $' whill change the prompt to 'CS470$'. If -s is provided\n"
	"\twithout -c then the default character '>' will be appended. Optionally using -d\n"
	"\twill display the prompt as the current date followed by a '>'.\n\n"
	"\tList of availible switches are:\n\n"
	"\t-s (Allows passing a string to change the prompt to as the next argument)\n\n" 
	"\t-c (Allows passing a single character to change the end charcater of the prompt)\n\n"
	"\t-d (Allows the prompt to display the current date followed by a '>')\n\n"
    "EXIT CODES\n"
	"\t0	is OK,\n\n"
	"\t1	invalid argument configuration given,\n\n"
	"\t2	invalid switch, -c argument given is more than 1 character.\n");

    std::cout << man << "\n";
    return 0;
}

int man_pages::getPWDHelp() {
    std::string man("[NOTE] This manual page is copy/pasted from Linux as calling 'pwd --help' from the CWUSHELL\n"
    "is not possible for this command specifically, all other commands do not have an issue.\n\n"
    "pwd: pwd [-LP]\n"
    "\t\tPrint the name of the current working directory.\n\n"
    "\t\tOptions:\n"
    "\t\t\t-L        print the value of $PWD if it names the current working\n"
    "\t\t\t            directory\n"
    "\t\t\t-P        print the physical directory, without any symbolic links\n\n"
    "\t\tBy default, `pwd' behaves as if `-L' were specified.\n\n"
    "\t\tExit Status:\n"
    "\t\tReturns 0 unless an invalid option is given or the current directory\n"
    "\t\tcannot be read.\n");

    std::cout << man << "\n";
    return 0;
}