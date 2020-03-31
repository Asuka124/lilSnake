cmake_minimum_required(VERSION 3.10)
PROJECT(snake)

include_directories(/usr/local/include)


link_directories(/usr/local/lib)
link_libraries(curses)
link_libraries(collectc)

set(CMAKE_BUILD_TYPE "Debug")


AUX_SOURCE_DIRECTORY(. DIR_SRCS)
ADD_EXECUTABLE(snake ${DIR_SRCS})