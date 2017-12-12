# cmake init
cmake_minimum_required(VERSION 3.10.0)

# project init
project(PTGen)

# library packages
#	init
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules) 
#	SFML
find_package(SFML REQUIRED COMPONENTS graphics window system)

# library directories
#	SFML
include_directories(${SFML_INCLUDE_DIR})

# project directories
include_directories(src)

# project sources
file(GLOB SRC src/*.cpp)
add_executable(PTGen ${SRC})

# library linking
#	SFML
target_link_libraries(PTGen ${SFML_LIBRARIES} ${SFML_DEPENDECIES})
