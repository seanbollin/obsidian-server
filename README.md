[![Build Status](https://travis-ci.org/seanbollin/obsidian-server.svg?branch=master)](https://travis-ci.org/seanbollin/obsidian-server)

# obsidian-server

ObsidianMUD server.  Written in C++ using websockets.  Web client is the [obsidian-client](https://github.com/seanbollin/obsidian-client).

## Prerequisite installations
sudo apt-get install python-pip  
sudo apt-get install cmake  
PATH=$HOME/.local/bin:$PATH;  
pip install conan  
git submodule init  
git submodule update  

## Build instructions
mkdir build && cd build/  
conan install ..  
cmake ..  
make  

## Pylint instructions
After following the build instructions, run:  
make cpplint  

## Running tests
After following the build instructions, run:  
make test  
