[![Build Status](https://travis-ci.org/seanbollin/obsidian-server.svg?branch=master)](https://travis-ci.org/seanbollin/obsidian-server)

# obsidian-server

ObsidianMUD server.  Written in C++ using websockets.  Web client is the [obsidian-client](https://github.com/seanbollin/obsidian-client).

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