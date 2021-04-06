# CS470-Lab1
A mini UNIX shell for lab 1 of CS470 Operating Systems.

This mini shell was developed in Linux Ubuntu 20.04 but should work in similar Debian based Linux environments or ones that share the same system file structure.

In this shell you will find several normal linux commands which essentially call their corresponding system commands (except clear and pwd). These commands are ls, cd, cat, mkdir, rmdir, touch, rm, pwd, and clear.

The custom commands for the lab assignment "exit", "meminf", "sysinf", and "prompt" do not use any system calls and only derive information from linux files and C/C++ libraries which linux itself uses and documents in it's man pages.

Compiling & Executing:
    The code base while large was kept in a single .cpp file for simplicity. The only required library in linux should be the standard g++ compiler for c++. Besides that the shell is only usable in linux/Unix but keep in mind it was developed for Ubuntu 20.04.

    Simply compile with the command 'g++ -o lab1.exe man_pages.cpp Lab1.cpp' and run with './lab1.exe'.

Help/Manual:
    For a general manual and list of all available commands use the 'man' command in the shell. 
    
    For all other commands (except for cd, clear, and exit for obvious reasons) a corresponding manual page will come up when [command] --help is entered. Some manual pages are derived from their native linux manual pages (all except the custom commands and pwd).

Custom Commands:

    [exit] - Terminates the shell process and may also be given an exit code 'n'
        such as 'exit 2'.

    [meminf] - Lists information about the system's memory according to linux's 
        /proc/meminfo file. Note that information for volatile memory will fluctuate every time the shell is restarted. Switches for meminf include: -s for free swap space, -u for available free memory, -S for total shared memory (all of which are in bytes). Alternatively using -a for "all" will display all memory info from /proc/meminfo.

    [sysinf] - Lists information about the system's cpu and some network info. More 
        specifically it will list all processors and their type as well as the
        system hostname and ip address. Use -h for hostname, -i for host's IP address, and -p for number of processors and their type. 

    [prompt] - Allows changing the displayed prompt of the cwu shell which is by
        default 'cwushell>'. If given a string -s and a character -c then the prompt may be changed to for example 'Kyle%' if no character is given then the default '>' is appended to the string. Alternatively -d may be used to replace the prompt with the current date ending with '>'. If only prompt is given it will return to the default 'cwushell>'.
