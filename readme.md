# Physical Design Pa1
  partition homework for physical design
# Indexes
  - [Contents](#contents)
  - [Compiling](#compiling)
  - [Execution](#execution)
  - [Makefile Targets](#makefile-targets)
  - [Known Bugs](#known-bugs)
# Contents
  - PA files
    - checker   : program to check the validity of output file
    - input_pa1 : the input patterns
  - source code
    - main.cpp           : main function of fm program
    - data.h             : net, cell data structure
    - FMPartiter.{h,cpp} : FM partition core
    - parser.{h,cpp}     : input file parser
  - course report file
    - readme.txt : describe the project environment and usage
# Compiling
  ```
  make release
  ```
# Execution
  ```
  fm <input_file> <output_file>
  ```
# Makefile Targets
  - default target : debug
  - release : program without debug information
  - debug : program with debug information
  - tags : ctags tags
  - clean : remove object file and program
# Known Bugs
  - can not parse multi-line NET format
  - can not resolve dummy cell