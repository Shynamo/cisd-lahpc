set( HEADERS my_lapack.h util.h Mat.h)
set( FLAGS_DEBUG /DEBUG /Od ) 
set( FLAGS_RELEASE /O2 )

add_library( my_lapack 
    my_lapack.cpp
    util.cpp
    Mat.cpp
    ${HEADERS} )

target_include_directories(
    my_lapack
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions ( "-DLAHPC_DEBUG" )
endif()

target_compile_features(
    my_lapack
    PUBLIC
        cxx_std_11
)

target_compile_options( 
    my_lapack
    PRIVATE
        "$<$<CONFIG:Debug>:${FLAGS_DEBUG}>"
        "$<$<CONFIG:Release>:${FLAGS_RELEASE}>"
)