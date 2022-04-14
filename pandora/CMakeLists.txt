find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

BISON_TARGET(
  Parser 
  parser.y 
  ${CMAKE_CURRENT_SOURCE_DIR}/parser.cpp
  DEFINES_FILE
  ${CMAKE_CURRENT_SOURCE_DIR}/parser.hpp)

FLEX_TARGET(
  Scanner 
  tokens.l 
  ${CMAKE_CURRENT_SOURCE_DIR}/tokens.cpp)

ADD_FLEX_BISON_DEPENDENCY(Scanner Parser)

add_executable(pandora 
  main.cpp
  ${BISON_Parser_OUTPUTS} 
  ${FLEX_Scanner_OUTPUTS})

target_sources(pandora PRIVATE
  main.cpp
  ast.cpp
  ast.hpp)

# target_link_libraries(pandora ${FLEX_LIBRARIES})

