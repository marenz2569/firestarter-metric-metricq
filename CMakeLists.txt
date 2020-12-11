cmake_minimum_required(VERSION 3.9)

set(METRICQ_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_subdirectory(lib/metricq)

project(metric-metricq)

add_library(${PROJECT_NAME} SHARED
	src/metric.cpp
	)

add_executable(${PROJECT_NAME}-dumper
	src/metric_sink.cpp
	src/main.cpp
	)

find_package(Threads REQUIRED)

target_link_libraries(${PROJECT_NAME}
	PRIVATE
		Threads::Threads
	)

target_link_libraries(${PROJECT_NAME}-dumper
	PRIVATE
	        metricq::sink
)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wl,-nostartfiles)
