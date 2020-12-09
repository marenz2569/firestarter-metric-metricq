#include "metric_sink_wrapper.h"
#include "metric_sink.hpp"

#include <cassert>

metric_sink_t *metric_sink_create() {
  metric_sink_t *m;
  MetricSink *obj;

  m = (typeof(m))malloc(sizeof(*m));
  obj = new MetricSink();
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
