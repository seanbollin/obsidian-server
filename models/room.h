// Copyright 2017 Sean Bollin

#ifndef MODELS_ROOM_H_
#define MODELS_ROOM_H_

#include <string>

class Room {
 public:
    Room(std::string title, std::string description)
            : title(title),
              description(description),
              titleAndDescription(title + ": " + description) {}

    const std::string& getTitle() const;
    const std::string& getDescription() const;

    operator const char*() const {
        return titleAndDescription.c_str();
    }

 private:
    std::string title;
    std::string description;
    std::string titleAndDescription;
};

#endif  // MODELS_ROOM_H_
