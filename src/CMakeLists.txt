add_subdirectory(offscreen)
add_subdirectory(math)
set(GLSL_LIST cube.frag;cube.vert)

find_program(GLSLANG_VALIDATORR names glslangValidator HINTS ${PLATFORM_DIRR})
set(SHADERS "")
foreach(X IN LISTS GLSL_LIST)
    message(STATUS "GLSL compile ${X}")
    add_custom_command(
            COMMENT "Compiling ${X}"
            OUTPUT ${X}.h
            COMMAND ${GLSLANG_VALIDATORR} -V -x -o ${X}.h
            ${CMAKE_CURRENT_SOURCE_DIR}/${X}
            MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/${X}
            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${X}
    )
    list(APPEND SHADERS ${X}.h)
endforeach()
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

add_executable(myLib out.c head.txt.h ${SHADERS})

include_directories(${CMAKE_CURRENT_BINARY_DIR})