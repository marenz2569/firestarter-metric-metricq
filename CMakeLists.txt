cmake_minimum_required(VERSION 3.9)

set(METRICQ_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_subdirectory(lib/metricq)

project(metric-metricq)

add_library(${PROJECT_NAME} SHARED
	src/metric_sink_wrapper.cpp
	src/metric_sink.cpp
	src/metric.c
	)

target_link_libraries(${PROJECT_NAME}
	PRIVATE
	        metricq::sink
)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wl,-nostartfiles)
