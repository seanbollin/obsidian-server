#define CATCH_CONFIG_MAIN
#include "catch/catch.h"

#include "../room.h"

TEST_CASE("Rooms have a title and description", "[Room][title][description]" ) {
    std::string title = "Village Center";
    std::string description = "The center of the village.";

    Room room(title, description);

    REQUIRE(room.getTitle() == title);
    REQUIRE(room.getDescription() == description);
}
