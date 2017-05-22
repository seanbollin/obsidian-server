#include "exit.h"

const std::unordered_map<std::string, std::string> Exit::FULL_CARDINALS{
    {"n", "north"},
    {"ne", "northeast"},
    {"e", "east"},
    {"se", "southeast"},
    {"s", "south"},
    {"sw", "southwest"},
    {"w", "west"},
    {"nw", "northwest"}
};

std::string Exit::getExitDisplay() const {
    return Exit::FULL_CARDINALS.at(this->direction_);
}