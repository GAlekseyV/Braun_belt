cmake_minimum_required(VERSION 3.12)
project(Red_belt)

set(CMAKE_CXX_STANDARD 17)

MESSAGE(STATUS "Compiler flags:" ${CMAKE_CXX_COMPILE_FLAGS})
MESSAGE(STATUS "Compiler cxx debug flags:" ${CMAKE_CXX_FLAGS_DEBUG})
MESSAGE(STATUS "Compiler cxx debug flags:" ${CMAKE_CXX_FLAGS_RELEASE})

set(PARTS
    Part_1
    Part_2
    Part_3
    Part_4
    Part_5
    Final_task
)

set(Part_1
        0_priority_container
        1_hash_set
        2_set_iterator++
        3_hash_point
        4_hash_person
        #5_secondary_index
)

set(Part_2
        0_spendings_xml
)

set(Part_3

)

set(Part_4

)

set(Part_5

)

set(Final_task

)

# create a project file per tutorial
foreach(PART ${PARTS})
    foreach(TASK ${${PART}})
        file(GLOB SOURCE
                "${PART}/${TASK}/*.h"
                "${PART}/${TASK}/*.cpp"
                )
        set(NAME "${PART}_${TASK}")
        add_executable(${NAME} ${SOURCE})
        target_include_directories(${NAME} PRIVATE "test_framework/")
    endforeach(TASK)
endforeach(PART)