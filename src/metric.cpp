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
#include "metric_sink.hpp"

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

static MetricSink *sink = nullptr;
static std::string error = "";

extern "C" {
#include "MetricInterface.h"

static int32_t init() {
  if (sink != nullptr) {
    sink = new MetricSink();
    while (!sink->ready()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
  }

  return EXIT_SUCCESS;
}

static int32_t fini() {
  if (sink != nullptr) {
    delete sink;
    sink = nullptr;
  }

  return EXIT_SUCCESS;
}

static int32_t get_reading(double *value) {
  if (sink == nullptr) {
    error = "no init";
    return EXIT_FAILURE;
  }

  if (sink->error()) {
    error = sink->errorString();
    return EXIT_FAILURE;
  }

  *double = sink->value();

  return EXIT_SUCCESS;
}

static const char *get_error() {
  // this is probably not a good coding practice.
  const char *errorCString = error.c_str();
  return errorCString;
}

static metric_type_t metric = {.name = "metricq",
                               .type = METRIC_ABSOLUTE,
                               .unit = "W",
                               .callback_time = 0,
                               .callback = nullptr,
                               .init = init,
                               .fini = fini,
                               .get_reading = get_reading,
                               .get_error = get_error};
}
