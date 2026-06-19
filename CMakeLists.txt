cmake_minimum_required(VERSION 3.14)
project(better-lists C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options(-Wall)

add_library(list STATIC src/list.c)
target_include_directories(list PUBLIC src)

include(FetchContent)
FetchContent_Declare(
  unity
  GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
  GIT_TAG v2.6.0
)
FetchContent_MakeAvailable(unity)
target_compile_definitions(unity PUBLIC UNITY_INCLUDE_DOUBLE)

add_executable(tests src/tests.c)
target_link_libraries(tests PRIVATE list unity)

option(ENABLE_SANITIZERS "Build with AddressSanitizer and UndefinedBehaviorSanitizer" OFF)
if(ENABLE_SANITIZERS)
  set(SANITIZER_FLAGS -fsanitize=address,undefined -fno-omit-frame-pointer -O0 -g)
  target_compile_options(list PRIVATE ${SANITIZER_FLAGS})
  target_link_options(list PRIVATE ${SANITIZER_FLAGS})
  target_compile_options(tests PRIVATE ${SANITIZER_FLAGS})
  target_link_options(tests PRIVATE ${SANITIZER_FLAGS})
endif()

option(ENABLE_COVERAGE "Build with coverage instrumentation" OFF)
if(ENABLE_COVERAGE)
  target_compile_options(list PRIVATE --coverage -O0 -g)
  target_link_options(list PRIVATE --coverage)
  target_compile_options(tests PRIVATE --coverage -O0 -g)
  target_link_options(tests PRIVATE --coverage)

  find_program(GCOVR_PATH gcovr REQUIRED)
  add_custom_target(coverage
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/coverage
    COMMAND tests
    COMMAND ${GCOVR_PATH} --root ${CMAKE_SOURCE_DIR} --filter ${CMAKE_SOURCE_DIR}/src
            --html --html-details -o ${CMAKE_BINARY_DIR}/coverage/index.html
            --print-summary
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS tests
    COMMENT "Running tests and generating coverage report"
  )
endif()
