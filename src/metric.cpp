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
static std::string error = "";
static void *callback_arg = nullptr;
static void (*callback_func)(void *, uint64_t, int64_t, double) = nullptr;

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

  const char *cmd = std::getenv("FIRESTARTER_METRIC_METRICQ_DUMPER");

  if (!cmd) {
    error = "failed to read env variable \"FIRESTARTER_METRIC_METRICQ_DUMPER\"";
    pthread_exit(NULL);
  }

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
      (*callback_func)(callback_arg, ROOT_METRIC_INDEX, time, value);
    }
  }
  free(line);

  pclose(pipe);
  error = "FILE closed";
  pthread_exit(NULL);
}

static int32_t register_insert_callback(void (*callback)(void *, uint64_t,
                                                         int64_t, double),
                                        void *obj) {
  callback_func = callback;
  callback_arg = obj;

  thread = (pthread_t *)malloc(sizeof(pthread_t));

  pthread_create(thread, NULL, inserter_thread, NULL);

  return EXIT_SUCCESS;
}

static const char *get_error() { return ""; }

MetricInterface metric = {.Name = "metricq",
                          .Type = {.Absolute = 1,
                                   .Accumalative = 0,
                                   .DivideByThreadCount = 0,
                                   .InsertCallback = 1,
                                   .IgnoreStartStopDelta = 0,
                                   .Reserved = 0},
                          .Unit = "W",
                          .CallbackTime = 0,
                          .Callback = nullptr,
                          .Init = init,
                          .Fini = fini,
                          .GetSubmetricNames = nullptr,
                          .GetReading = nullptr,
                          .GetError = get_error,
                          .RegisterInsertCallback = register_insert_callback};
