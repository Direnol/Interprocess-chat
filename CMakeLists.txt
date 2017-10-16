cmake_minimum_required(VERSION 3.5)
project(Interprocess_chat)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c11")

set(SOURCE_FILES
        src/sharedMemory.c headers/sharedMemory.h src/lock.c headers/lock.h)

set(SOURCE_CLIENT src/mainclient.c src/client.c headers/client.h ${SOURCE_FILES})
set(SOURCE_SERV src/mainserv.c src/serv.c headers/serv.h ${SOURCE_FILES})

add_executable(client ${SOURCE_CLIENT})
target_link_libraries(client pthread)

add_executable(serv ${SOURCE_SERV})