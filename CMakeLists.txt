cmake_minimum_required(VERSION 3.10)
project(PerfumeBazaar)
set(CMAKE_CXX_STANDARD 17)
find_package(SQLite3 REQUIRED)

add_executable(app main.cpp db.cpp auth.cpp deal.cpp)
target_include_directories(app PRIVATE .)
target_link_libraries(app SQLite::SQLite3)

enable_testing()

add_executable(test_db test_db.cpp db.cpp)
target_include_directories(test_db PRIVATE .)
target_link_libraries(test_db SQLite::SQLite3)
add_test(NAME DB COMMAND test_db)

add_executable(test_auth test_auth.cpp auth.cpp db.cpp)
target_include_directories(test_auth PRIVATE .)
target_link_libraries(test_auth SQLite::SQLite3)
add_test(NAME Auth COMMAND test_auth)

add_executable(test_deal test_deal.cpp deal.cpp db.cpp)
target_include_directories(test_deal PRIVATE .)
target_link_libraries(test_deal SQLite::SQLite3)
add_test(NAME Deal COMMAND test_deal)