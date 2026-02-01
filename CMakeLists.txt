cmake_minimum_required(VERSION 3.12)

project(Red_belt
    VERSION 1.0.0
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

set(PARTS "Part_1" "Part_2")

set(Part_1
        0_priority_container
        1_hash_set
        2_set_iterator++
        3_hash_point
        4_hash_person
)

set(Part_2
        0_spendings_xml
        1_spendings_json
        2_ini
)

function(add_task TARGET_NAME PART TASK)
    file(GLOB SOURCES
        "${PART}/${TASK}/*.h"
        "${PART}/${TASK}/*.cpp"
    )
    
    add_executable(${TARGET_NAME} ${SOURCES})
    target_include_directories(${TARGET_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/test_framework")
    
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set_target_properties(${TARGET_NAME} PROPERTIES
            CXX_CLANG_TIDY "clang-tidy;-header-filter=${CMAKE_SOURCE_DIR}/*"
        )
    endif()
endfunction()

foreach(PART ${PARTS})
    foreach(TASK ${${PART}})
        set(TARGET_NAME "${PART}_${TASK}")
        add_task(${TARGET_NAME} ${PART} ${TASK})
    endforeach()
endforeach()