cmake_minimum_required(VERSION 3.2)

project(AppForTonyStark)

set(SOURCES 
    src/main.cpp
)

add_executable(${PROJECT_NAME}
    ${SOURCES}
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        OUTPUT_NAME "app"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)
