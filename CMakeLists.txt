cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0048 NEW)

project(kdeconnect-applet
    VERSION 0.1
    LANGUAGES CXX)

#----------------------------
# Target definition

# Populates SOURCES/HEADERS/etc variables.
add_subdirectory(src)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${UI_FILES} ${RC_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF)

#----------------------------
# Dependencies

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Widgets DBus REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Widgets Qt5::DBus)

#----------------------------
# Strict warnings

option(WSTRICT "Build with strict warning flags (Clang only)" NO)

if(WSTRICT)
    if(NOT CMAKE_CXX_COMPILER_ID MATCHES "(Clang|AppleClang)")
        message(FATAL_ERROR "Clang is required for WSTRICT.")
    endif()

    target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra -pedantic -Wdocumentation
        -Wconversion -Wdeprecated -Wformat -Weffc++)
endif()

#----------------------------
# Linters and style checkers

find_program(clangtidy_path NAMES clang-tidy)
if(clangtidy_path)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    add_custom_target(tidy
        COMMAND "${clangtidy_path}" -p "${PROJECT_BINARY_DIR}" ${SOURCES}
        COMMENT "Running clang-tidy tool"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/"
        SOURCES ${PROJECT_SOURCE_DIR}/.clang-tidy
        )
endif()

find_program(clangformat_path NAMES clang-format)
if(clangformat_path)
    add_custom_target(format
        COMMAND "${clangformat_path}" -i -style=file ${SOURCES} ${HEADERS}
        COMMENT "Running clang-format tool"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/src"
        SOURCES ${PROJECT_SOURCE_DIR}/.clang-format
        )
endif()

#----------------------------
# Installation

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})