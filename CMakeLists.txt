cmake_minimum_required(VERSION 3.5)
project(scopeapp)

# Create an option to switch between a system sdl library and a vendored SDL library
option(SCOPEAPP_VENDORED "Use vendored libraries" ON)

if(SCOPEAPP_VENDORED)
    # This assumes you have added SDL as a submodule in vendored/SDL
    add_subdirectory(vendored/SDL3-3.2.8 EXCLUDE_FROM_ALL)
else()
    # 1. Look for a SDL3 package,
    # 2. look for the SDL3-shared component, and
    # 3. fail if the shared component cannot be found.
    find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
endif()

if(SCOPEAPP_VENDORED)
    # This assumes you have added SDL as a submodule in vendored/SDL
    add_subdirectory(vendored/SDL3_image-3.2.4 EXCLUDE_FROM_ALL)
endif()

# Create your game executable target as usual
add_executable(scopeapp WIN32 src/scope.c)

# Link to the actual SDL3 library.
target_link_libraries(scopeapp PRIVATE SDL3::SDL3 SDL3_image::SDL3_image)
