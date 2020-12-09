#include "metric_sink_wrapper.h"
#include "metric_sink.hpp"

#include <cassert>

metric_sink_t *metric_sink_create() {
  metric_sink_t *m;
  MetricSink *obj;

  m = static_cast<metric_sink_t *>(malloc(sizeof(metric_sink_t)));
  try {
  obj = new MetricSink();
  } catch(std::exception &e) {
	std::cout << e.what() << std::flush;
  }
  m->obj = obj;

  return m;
}

bool metric_sink_error(metric_sink_t *m) {
  assert(m != NULL);

  MetricSink *obj = static_cast<MetricSink *>(m->obj);

  return obj->error();
}

bool metric_sink_ready(metric_sink_t *m) {
  assert(m != NULL);

  MetricSink *obj = static_cast<MetricSink *>(m->obj);

  return obj->ready();
}

double metric_sink_value(metric_sink_t *m) {
  assert(m != NULL);

  MetricSink *obj = static_cast<MetricSink *>(m->obj);

  return obj->value();
}

void metric_sink_destroy(metric_sink_t *m) {
  assert(m != NULL);

  MetricSink *obj = static_cast<MetricSink *>(m->obj);

  delete obj;
  free(m);
}
