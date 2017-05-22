// Copyright 2017 Sean Bollin

#include <string>
#include <vector>

#include "../models/room.h"
#include "../models/exit.h"

unsigned int Room::getId() const {
    return id_;
}

const std::string& Room::getTitle() const { // NOLINT (build/include_what_you_use)
    return this->title_;
}

const std::string& Room::getDescription() const { // NOLINT (build/include_what_you_use)
    return this->description_;
}

const std::string Room::getExitsString() const { // NOLINT (build/include_what_you_use)
    return std::string(Room::EXITS_PREFIX) + " " + this->listExits();
}

void Room::setExits(std::vector<Exit> exits) {
    exits_ = exits;
}

const std::string Room::listExits() const {
    std::string listOfExits = "";

    for (const auto& exit : exits_) {
        if (listOfExits.length() != 0) listOfExits += ", ";
        listOfExits += exit.getExitDisplay();
    }

    return listOfExits;
}
