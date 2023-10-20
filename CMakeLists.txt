cmake_minimum_required(VERSION 3.2)

project(AppForTonyStark)

set(SOURCES 
    src/main.cpp
    src/input.cpp
    src/logic.cpp
    src/file_entry.cpp
)

add_executable(${PROJECT_NAME}
    ${SOURCES}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE "./include"
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        CXX_STANDARD 20
        OUTPUT_NAME "app"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)
