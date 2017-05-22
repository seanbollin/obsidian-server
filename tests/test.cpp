#define CATCH_CONFIG_MAIN
#include <catch.h>

#include "../models/room.h"

TEST_CASE("Rooms have a title and description", "[Room][title][description]" ) {
    unsigned int roomId = 1;
    std::string title = "Village Center";
    std::string description = "The center of the village.";

    Room room(roomId, title, description);

    REQUIRE(room.getId() == roomId);
    REQUIRE(room.getTitle() == title);
    REQUIRE(room.getDescription() == description);
}

TEST_CASE("Rooms have exits", "[Room][exits]" ) {
    unsigned int id = 1;
    std::string title = "Village Center";
    std::string description = "The center of the village.";
    Room villageCenter(id, title, description);

    Exit exitEast("e", 2);
    Exit exitSouth("s", 3);
    std::vector<Exit> exits{exitEast, exitSouth};
    villageCenter.setExits(exits);

    REQUIRE(villageCenter.getExitsString() == "Obvious exits: east, south");
}

TEST_CASE("Exits have a full cardinal name", "[Exit]") {
    Exit exitSouthEast("se", 1);
    REQUIRE(exitSouthEast.getExitDisplay() == "southeast");
}
