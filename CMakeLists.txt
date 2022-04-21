cmake_minimum_required(VERSION 3.17.3)

project(pandora)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(pandora)

# LLVM
find_package(LLVM REQUIRED CONFIG)

llvm_map_components_to_libnames(REQ_LLVM_LIBRARIES mcjit interpreter native ipo)

target_link_libraries(pandora PUBLIC
  ${LLVM_LIBRARIES}
  {REQ_LLV_LIBRARIES})

target_include_directories(pandora PUBLIC
  ${LLVM_INCLUDE_DIRS}
  ${PROJECT_SOURCE_DIR})

target_compile_definitions(pandora PUBLIC 
  ${LLVM_DEFINITIONS})

target_compile_features(pandora PRIVATE cxx_std_20)

target_compile_options(pandora PRIVATE
	"$<$<COMPILE_LANG_AND_ID:CXX,Clang,AppleClang>:"
    -std=c++20
		-Wall
		-Wshadow
		-Wextra
		-Werror
		-Wpedantic
		-pedantic-errors
		-Walloca 
		-Wvla 
    -fdiagnostics-color=always
    -Wno-sign-compare           # flex and bison
    -Wno-unused-function        # flex and bison
    -Wno-unused-parameter       # flex and bison
    -Wno-unused-private-field  # flex and bison
  ">"
	"$<$<AND:$<COMPILE_LANG_AND_ID:CXX,Clang,AppleClang>,$<CONFIG:Debug>>:"
		-O0
		-g
		-fsanitize=address
		-fsanitize=undefined
		-fsanitize=bounds
  ">"
	"$<$<AND:$<COMPILE_LANG_AND_ID:CXX,Clang,AppleClang>,$<CONFIG:Release>>:"
		-O3
  ">")

target_link_options(pandora PUBLIC
	"$<$<AND:$<COMPILE_LANG_AND_ID:CXX,Clang,AppleClang>,$<CONFIG:Debug>>:"
		-fsanitize=address
		-fsanitize=undefined
		-fsanitize=bounds
  ">")

