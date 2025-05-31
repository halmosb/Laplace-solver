# == Include Directories ==
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# == Source Files ==
set(SOURCES 
    main.cpp
    parser.cpp
    calculator.cpp
    filehandler.cpp
    utilities.cpp
)

# == Library for reuse in tests ==
add_library(laplace_lib
    parser.cpp
    calculator.cpp
    filehandler.cpp
    utilities.cpp
    HDF5Writer.cpp
)

target_include_directories(laplace_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${HDF5_INCLUDE_DIRS})
target_link_libraries(laplace_lib PUBLIC ${Boost_LIBRARIES} ${HDF5_CXX_LIBRARIES})

# == Main Executable with OpenMP ==
add_executable(laplace main.cpp)
target_link_libraries(laplace PRIVATE laplace_lib)

if(OpenMP_FOUND)
    target_link_libraries(laplace PRIVATE OpenMP::OpenMP_CXX)
    target_compile_definitions(laplace PRIVATE -DUSE_OPENMP)
endif()

# == Main Executable without OpenMP ==
add_executable(laplace_serial main.cpp)
target_link_libraries(laplace_serial PRIVATE laplace_lib)
