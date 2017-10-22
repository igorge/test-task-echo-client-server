cmake_minimum_required(VERSION 3.7 FATAL_ERROR)
project(echo-client)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


if(WIN32)
    set(SOCKSLINKLIB "Ws2_32.lib")
    set(THREADLIB "")

    add_definitions(-DNOMINMAX)

    set(PLATFORM_INCLUDE ${PROJECT_SOURCE_DIR}/../common-win/include)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
    set(SOCKSLINKLIB "")
    set(THREADLIB "pthread")

    set(SANITAZERS "-fsanitize=address -fsanitize=undefined -fsanitize=float-cast-overflow")

    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -fno-omit-frame-pointer ${SANITAZERS}")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG -O3")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -DNDEBUG -O2 -fno-omit-frame-pointer")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -DNDEBUG -Os")

    set(PLATFORM_INCLUDE ${PROJECT_SOURCE_DIR}/../common-linux/include)
endif ()

include_directories(
        ${CMAKE_CURRENT_BINARY_DIR} ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/../common/include ${PLATFORM_INCLUDE})

set(SOURCE_FILES
        src/main.cpp)


add_executable(echo-client ${SOURCE_FILES})

target_link_libraries(echo-client ${SOCKSLINKLIB} ${THREADLIB})
