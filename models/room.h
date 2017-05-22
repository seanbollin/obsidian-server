// Copyright 2017 Sean Bollin

#ifndef MODELS_ROOM_H_
#define MODELS_ROOM_H_

#include <string>
#include <vector>
#include "exit.h"

class Room {
 public:
    Room(unsigned int id, std::string title, std::string description)
            : id_(id),
              title_(title),
              description_(description),
              titleAndDescription_(title + ": " + description) {}

    static const std::string EXITS_PREFIX;

    unsigned int getId() const;
    const std::string& getTitle() const;
    const std::string& getDescription() const;
    const std::string getExitsString() const;

    void setExits(std::vector<Exit> exits);

    operator const char*() const {
        return titleAndDescription_.c_str();
    }

 private:
    unsigned int id_;
    std::string title_;
    std::string description_;
    std::string titleAndDescription_;
    std::vector<Exit> exits_;

    const std::string listExits() const;
};

#endif  // MODELS_ROOM_H_
