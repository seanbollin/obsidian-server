project(tests)

add_executable(tests test.cpp ../models/room.cpp ../models/room.h ../models/exit.cpp ../models/exit.h)
target_link_libraries(tests Catch)