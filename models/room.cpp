// Copyright 2017 Sean Bollin

#include "../models/room.h"

const std::string& Room::getTitle() const { // NOLINT (build/include_what_you_use)
    return this->title;
}

const std::string& Room::getDescription() const { // NOLINT (build/include_what_you_use)
    return this->description;
}
