#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>

extern "C" {
#include <pthread.h>

#include "MetricInterface.h"
}

static pthread_t *thread;
static const char *metricName = "metricq";
// TODO: get this via env variable
static const char *cmd =
    "/home/s2599166/firestarter-metric-metricq/build/metric-metricq-dumper";
static std::string error = "";
static void *callback_arg = nullptr;
static void (*callback_func)(void *, const char *, int64_t, double) = nullptr;

static int32_t init() { return EXIT_SUCCESS; }

static int32_t fini() {
  if (callback_func != nullptr) {
    pthread_cancel(*thread);

    pthread_join(*thread, NULL);
  }

  callback_arg = nullptr;
  callback_func = nullptr;

  return EXIT_SUCCESS;
}

static void *inserter_thread(void *input) {
  (void)input;

  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

  FILE *pipe = popen(cmd, "r");
  if (!pipe) {
    error = "popen() failed";
    pthread_exit(NULL);
  }

  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  while ((read = getline(&line, &len, pipe)) != -1) {
    int64_t time;
    double value;
    if (std::sscanf(line, "[DATA] %ld %lf", &time, &value) > 0) {
      (*callback_func)(callback_arg, metricName, time, value);
    }
  }
  free(line);

  pclose(pipe);
  error = "FILE closed";
  pthread_exit(NULL);
}

static int32_t register_insert_callback(void (*callback)(void *, const char *,
                                                         int64_t, double),
                                        void *obj) {
  callback_func = callback;
  callback_arg = obj;

  thread = (pthread_t *)malloc(sizeof(pthread_t));

  pthread_create(thread, NULL, inserter_thread, NULL);

  return EXIT_SUCCESS;
}

static const char *get_error() { return ""; }

metric_interface_t metric = {.name = metricName,
                             .type = {.absolute = 1,
                                      .accumalative = 0,
                                      .divide_by_thread_count = 0,
                                      .insert_callback = 1,
                                      .__reserved = 0},
                             .unit = "W",
                             .callback_time = 0,
                             .callback = nullptr,
                             .init = init,
                             .fini = fini,
                             .get_reading = nullptr,
                             .get_error = get_error,
                             .register_insert_callback =
                                 register_insert_callback};
