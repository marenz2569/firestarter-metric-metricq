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

#include <metricq/ostream.hpp>
#include <metricq/types.hpp>

#include <cmath>
#include <stdexcept>

MetricSink::MetricSink() : metricq::Sink("", true) {
  const char *server = std::getenv("METRICQ_SERVER");
  if (server == nullptr) {
	  throw std::runtime_error("ENV varibale METRICQ_SERVER does not exsist");
  }
  this->server = server;
  const char *metrics = std::getenv("METRICQ_METRICS");
  if (metrics == nullptr) {
	  throw std::runtime_error("ENV varibale METRICQ_METRICS does not exsist");
  }
  this->metric = metrics;
  metrics_.push_back(this->metric);
  connect(this->server);
}

// TODO: we should probably send an sink unsubscribe somewhere, but i think the
// server should not have a problem with it, maybe.

void MetricSink::on_connected() { this->subscribe(metrics_); }

void MetricSink::on_data_channel_ready() { ready_ = true; }

void MetricSink::on_error(const std::string &message) {
  error_ = true;
  errorString_ = message;
}

void MetricSink::on_closed() {
  error_ = true;
  errorString_ = "Connection closed";
}

void MetricSink::on_data(const AMQP::Message &message, uint64_t delivery_tag,
                         bool redelivered) {
  if (message.typeName() == "end") {
    data_channel_->ack(delivery_tag);
    // We used to close the data connection here, but this should not be
    // necessary. It will be closed implicitly from the response callback.
    rpc("sink.release", [this](const auto &) { close(); },
        {{"dataQueue", data_queue_}});
    return;
  }

  Sink::on_data(message, delivery_tag, redelivered);
}

void MetricSink::on_data(const std::string &name, metricq::TimeValue tv) {
  last_value_ = tv.value;
}
