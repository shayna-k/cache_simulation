A cache simulation written in C for CMSC 15400 (UChicago). 

Instructions for running the code follow:

************************
Running the autograders:
************************

Before running the autograders, compile your code:
    linux> make
    macbook> make

Check the correctness of your simulator:
    linux> ./test-csim
    macbook> ./test-csim-mac

Check everything at once (this is the program that your instructor runs):
    linux> ./driver.py
    macbook> ./driver-mac.py

******
Files:
******

# You will modify this file and only this files
csim.c       Your cache simulator

# Tools for evaluating your simulator
Makefile        Builds the simulator
README          This file
driver.py*      The driver program, runs test-csim
driver-mac.py*  The driver program for mac users, runs test-csim-mac
cachelab.c      Required helper functions
cachelab.h      Required header file
csim-ref*       The executable reference cache simulator
csim-ref-mac*   The executable reference cache simulator for Mac users
test-csim*      Tests your cache simulator
test-csim-mac*  Tests your cache simulator on mac
traces/         Trace files used by test-csim.c
