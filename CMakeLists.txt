cmake_minimum_required(VERSION 3.9)

project(metric-metricq VERSION 1.0)

add_library(metric-metricq
	src/metric.cpp
	src/metric_sink.cpp
	)
target_compile_features(metric-metricq PRIVATE cxx_std_17)
target_compile_options(metric-metricq PRIVATE -Wall -Wextra -pedantic)

target_link_libraries(metric-metricq
    PRIVATE
        metricq::sink
)
