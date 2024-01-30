cmake_minimum_required(VERSION 3.20)

project(WeatherForecast)

set(CMAKE_CXX_STANDARD 11)

find_package(CURL REQUIRED)
find_package(cpr REQUIRED)
find_package(nlohmann_json REQUIRED)

add_executable(WeatherForecast src/main.cpp)

target_include_directories(WeatherForecast PRIVATE ${CURL_INCLUDE_DIRS})
target_include_directories(WeatherForecast PRIVATE ${CPR_INCLUDE_DIRS})
target_include_directories(WeatherForecast PRIVATE ${nlohmann_json_INCLUDE_DIRS})

target_link_libraries(WeatherForecast PRIVATE ${CURL_LIBRARIES})
target_link_libraries(WeatherForecast PRIVATE ${CPR_LIBRARIES})
target_link_libraries(WeatherForecast PRIVATE nlohmann_json::nlohmann_json)
