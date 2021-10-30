# CMakeList.txt : CMake project for DemoSerializeFunctions, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
find_package(Threads)
add_executable (DemoSerializeFunctions "main.cpp" "serializeFunctions.h" "serializeFunctions.cpp")
target_link_libraries(DemoSerializeFunctions ${CMAKE_THREAD_LIBS_INIT} )

# TODO: Add tests and install targets if needed.
