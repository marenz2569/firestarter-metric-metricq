// Copyright (c) 2018, ZIH, Technische Universitaet Dresden, Federal Republic of
// Germany
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright notice,
//       this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//     notice,
//       this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of metricq nor the names of its contributors
//       may be used to endorse or promote products derived from this software
//       without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "metric_sink_wrapper.h"

#include "MetricInterface.h"

#include <stdlib.h>

static metric_sink_t *sink = NULL;

static int32_t init() {
  if (sink == NULL) {
    sink = metric_sink_create();
    while (!metric_sink_ready(sink))
	    ;
  }

  return EXIT_SUCCESS;
}

static int32_t fini() {
  if (sink != NULL) {
    metric_sink_destroy(sink);
    sink = NULL;
  }

  return EXIT_SUCCESS;
}

static int32_t get_reading(double *value) {
  if (sink == NULL) {
 //   error = "no init";
    return EXIT_FAILURE;
  }

  if (metric_sink_error(sink)) {
//    error = sink->errorString();
    return EXIT_FAILURE;
  }

  *value = metric_sink_value(sink);

  return EXIT_SUCCESS;
}

static const char *get_error() {
	return "";
}

metric_interface_t metric = {
	.name = "metricq",
	.type = METRIC_ABSOLUTE,
	.unit = "W",
	.callback_time = 0,
	.callback = NULL,
	.init = init,
	.fini = fini,
	.get_reading = get_reading,
	.get_error = get_error,
};

static void initi(void) __attribute__((constructor));
static void initi(void) {}

static void shutdown(void) __attribute__((destructor));
static void shutdown(void) {}
