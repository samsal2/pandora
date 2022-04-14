cmake_minimum_required(VERSION 3.17.3)

project(pandora)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(pandora)

# LLVM
find_package(LLVM REQUIRED CONFIG)
target_link_libraries(pandora PUBLIC ${LLVM_LIBRARIES})
target_include_directories(pandora PUBLIC ${LLVM_INCLUDE_DIRS})
target_compile_definitions(pandora PUBLIC ${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(REQ_LLVM_LIBRARIES mcjit interpreter native ipo)

target_link_libraries(pandora PUBLIC ${REQ_LLVM_LIBRARIES})

set(PANDORA_CXX_CLANG_BASE_COMPILE_FLAGS 
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
    -Wno-unused-private-field)  # flex and bison

set(PANDORA_CXX_CLANG_DEBUG_COMPILE_FLAGS
		-O0
		-g
		-fsanitize=address
		-fsanitize=undefined
		-fsanitize=bounds)

set(PANDORA_CXX_CLANG_RELEASE_COMPILE_FLAGS
		-O3)

set(PANDORA_CXX_CLANG_DEBUG_LINK_FLAGS
		-fsanitize=address
		-fsanitize=undefined
		-fsanitize=bounds)
	
target_include_directories(pandora PUBLIC ${PROJECT_SOURCE_DIR})

target_compile_features(pandora PRIVATE cxx_std_20)

target_compile_options(pandora PRIVATE
	"$<$<COMPILE_LANG_AND_ID:CXX,Clang,AppleClang>:"
    "${PANDORA_CXX_CLANG_BASE_COMPILE_FLAGS}>"
	"$<$<AND:$<COMPILE_LANG_AND_ID:CXX,Clang,AppleClang>,$<CONFIG:Debug>>:"
    "${PANDORA_CXX_CLANG_DEBUG_COMPILE_FLAGS}>"
	"$<$<AND:$<COMPILE_LANG_AND_ID:CXX,Clang,AppleClang>,$<CONFIG:Release>>:"
    "${PANDORA_CXX_CLANG_RELEASE_COMPILE_FLAGS}>")

target_link_options(pandora PUBLIC
	"$<$<AND:$<COMPILE_LANG_AND_ID:CXX,Clang,AppleClang>,$<CONFIG:Debug>>:"
    "${PANDORA_CXX_CLANG_DEBUG_LINK_FLAGS}>")

