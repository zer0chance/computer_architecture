# Simple Computer

This is the virtual machine for Simple Computer. There is a Simple Assembler Translator, that can help you to programm on this computer, and also compiler for high-level language: Simple Basic.

## How to build and use

    make
    ./sbc <filename>
    ./asmt <filename>
    ./simple_computer
    

You need to compile your program first, so file with extension .sb will be compiled in .sa. Than you need to translate Assembly file in binary file with extension .o and after that it can be loaded into Simple Computer and launched.

    make debug 
    
Debug build will show you some workflow statistics, so it will be easier to identify, where is your mistake occur.

    make clean
    
To delete all binary files.
