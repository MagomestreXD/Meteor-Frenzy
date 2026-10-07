cmake_minimum_required(VERSION 3.20)

project(MeuJogo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_mixer REQUIRED)

add_executable(MeuJogo
    src/main.cpp
    src/rasterizer.cpp
    src/spriteManager.cpp
    src/game.cpp
)

target_include_directories(MeuJogo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(MeuJogo PRIVATE 
	SDL3::SDL3
	SDL3_image::SDL3_image-shared
	SDL3_mixer::SDL3_mixer-shared	
)

add_custom_command(TARGET MeuJogo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/resources
    $<TARGET_FILE_DIR:MeuJogo>/resources
)