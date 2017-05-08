project(tests)

add_executable(tests test.cpp ../models/room.cpp ../models/room.h)
target_link_libraries(tests Catch)