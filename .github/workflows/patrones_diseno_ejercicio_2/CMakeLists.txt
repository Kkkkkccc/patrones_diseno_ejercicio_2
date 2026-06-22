cmake_minimum_required(VERSION 4.2)
project(expenses_patterns LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(expenses_app
        src/main.cpp
        src/expenses.cpp
)
target_include_directories(expenses_app PRIVATE include)

add_executable(expenses_tests
        tests/test_expenses.cpp
        src/expenses.cpp
)
target_include_directories(expenses_tests PRIVATE include)

enable_testing()
add_test(NAME expenses_tests COMMAND expenses_tests)