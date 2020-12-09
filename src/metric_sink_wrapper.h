#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

struct metric_sink {
  void *obj;
};

typedef struct metric_sink metric_sink_t;

metric_sink_t *metric_sink_create();

bool metric_sink_error(metric_sink_t *m);

bool metric_sink_ready(metric_sink_t *m);

double metric_sink_value(metric_sink_t *m);

void metric_sink_destroy(metric_sink_t *m);

#ifdef __cplusplus
}
#endif
