# RootKit_hide_processes
An implementation for a rootkit to hide processes by their name in linux (Ubuntu) works on ps and pstree.
The rootkit checks of the read file is a directory of a process. 
The name of the process can be found in syscalls.c as "process name here".
if it is he checks for the name of the process, if the name is matching he hides the process directory
Note this rootkit is unstable and can cuase problems to the OS.
