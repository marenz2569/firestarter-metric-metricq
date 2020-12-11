#include "metric_sink.hpp"

int main(int argc, const char **argv) {
  (void)argc;
  (void)argv;

  MetricSink sink;

  sink.main_loop();

  return EXIT_SUCCESS;
}
