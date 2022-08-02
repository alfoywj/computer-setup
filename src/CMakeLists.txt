add_subdirectory(offscreen)
add_subdirectory(math)

add_custom_command(
        OUTPUT out.c
        COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/in.txt out.c
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/in.txt
        VERBATIM)

add_custom_command(
        OUTPUT head.txt.h
        COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/head.txt head.txt.h
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/head.txt
        VERBATIM)

add_executable(myLib out.c head.txt.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR})