cmake_minimum_required(VERSION 3.2)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{CMAKE_TOOLCHAIN_FILE})
       set(CMAKE_TOOLCHAIN_FILE $ENV{CMAKE_TOOLCHAIN_FILE})
endif()

project(Snowflake_build)

set(TargetName Snowflaker)

find_package(NGL CONFIG REQUIRED)
set(CMAKE_AUTOMOC ON)
find_package(Qt6 COMPONENTS OpenGL Widgets QUIET )
if ( Qt6_FOUND )
    message("Found Qt6: Using that.")
else()
    message("Found Qt5: Using that.")
    find_package(Qt5 COMPONENTS OpenGL Widgets REQUIRED)
endif()

set(CMAKE_CXX_STANDARD 17)

add_executable(${TargetName})
include_directories(include $ENV{HOME}/NGL/include)
target_sources(${TargetName} PRIVATE 
${PROJECT_SOURCE_DIR}/src/main.cpp
${PROJECT_SOURCE_DIR}/src/snowflake.cpp
${PROJECT_SOURCE_DIR}/src/NGLScene.cpp
${PROJECT_SOURCE_DIR}/include/NGLScene.h
${PROJECT_SOURCE_DIR}/include/snowflake.h
${PROJECT_SOURCE_DIR}/include/WindowParams.h
)

target_link_libraries(${TargetName} PRIVATE Qt::Widgets Qt::OpenGL NGL)

add_custom_target(${TargetName}CopyShaders ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders
    $<TARGET_FILE_DIR:${TargetName}>/shaders
    
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/fonts
    $<TARGET_FILE_DIR:${TargetName}>/fonts
)