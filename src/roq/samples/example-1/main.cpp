/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include <gflags/gflags.h>

#include <vector>

#include "roq/application.h"
#include "roq/client.h"

#include "roq/logging.h"
#include "roq/format.h"

// command line options

DEFINE_string(deribit_exchange,
    "deribit",
    "name of the deribit exchange");

DEFINE_string(deribit_symbols,
    "BTC-\\d{2}\\D{3}\\d{2}",  // e.g. "BTC-27DEC19"
    "regex used to subscribe deribit symbols");

DEFINE_string(coinbase_pro_exchange,
    "coinbase-pro",
    "name of the coinbase-pro exchange");

DEFINE_string(coinbase_pro_symbols,
    "BTC-.*",  // e.g. "BTC-USD"
    "regex used to subscribe coinbase-pro symbols");

namespace roq {
namespace samples {
namespace example_1 {

// client::Config is an interface allowing a strategy factory
// to query for relevant configuration
// the design uses dispatch to allow the factory to use interfaces
// without imposing any kind of requirement on containers and storage

class Config final : public client::Config {
 public:
  Config() {}
  Config(Config&&) = default;

 protected:
  void dispatch(Handler& handler) const override {
    // callback for each subscription pattern
    handler.on(
        client::Symbol {
          .exchange = FLAGS_deribit_exchange,
          .regex = FLAGS_deribit_symbols,
        });
    handler.on(
        client::Symbol {
          .exchange = FLAGS_coinbase_pro_exchange,
          .regex = FLAGS_coinbase_pro_symbols,
        });
  }

 private:
  Config(const Config&) = delete;
  void operator=(const Config&) = delete;
};

// client::Handler is the base class managing the life-time of
// a strategy (could be in-process simulation or connectivity
// to one or more market access gateways)
// the constructor must *always* accept a client::Dispatcher as the
// first argument
// the construct *may* accept more arguments from the 2nd position
// event handlers *may* be overriden

class Strategy final : public client::Handler {
 public:
  explicit Strategy(client::Dispatcher& dispatcher)
      : _dispatcher(dispatcher) {
  }

  Strategy(Strategy&&) = default;

 protected:
  // the following event handlers log every update as-is
  // an important lesson here is to only use verbose logging in
  // the "hot path" (i.e. inside the event handlers)
  // using LOG(INFO) is not disallowed, but you should only use
  // this logging facility very minimally as it may have significant
  // latency impact
  // note!
  //   the ROQ_v environment variable defines the verbosity level
  //   for example, "export ROQ_v=1"
  void operator()(const ConnectionStatusEvent& event) override {
    LOG(INFO)("event={}", event);
  }
  void operator()(const DownloadBeginEvent& event) override {
    LOG(INFO)("event={}", event);
  }
  void operator()(const DownloadEndEvent& event) override {
    LOG(INFO)("event={}", event);
  }
  void operator()(const MarketDataStatusEvent& event) override {
    LOG(INFO)("event={}", event);
  }
  void operator()(const OrderManagerStatusEvent& event) override {
    LOG(INFO)("event={}", event);
  }
  void operator()(const ReferenceDataEvent& event) override {
    LOG(INFO)("event={}", event);
  }
  void operator()(const MarketStatusEvent& event) override {
    LOG(INFO)("event={}", event);
  }
  void operator()(const MarketByPriceEvent& event) override {
    VLOG(1)("event={}", event);  // see comment above
  }
  void operator()(const TradeSummaryEvent& event) override {
    VLOG(1)("event={}", event);  // see comment above
  }

 private:
  Strategy(const Strategy&) = delete;
  void operator=(const Strategy&) = delete;

 private:
  client::Dispatcher& _dispatcher;
};

// Application is a base class used to initialize the environment
// it is not required, only for your convenience

class Controller final : public Application {
 public:
  using Application::Application;

 protected:
  int main(int argc, char **argv) override {
    if (argc == 1)
      throw std::runtime_error("Expected arguments");
    Config config;
    std::vector<std::string> connections(argv + 1, argv + argc);
    // this strategy factory uses direct connectivity to one or more
    // market access gateways
    client::Trader(config, connections).dispatch<Strategy>();
    return EXIT_SUCCESS;
  }
};

}  // namespace example_1
}  // namespace samples
}  // namespace roq

namespace {
constexpr const char *DESCRIPTION = "Example 1 (Roq Samples)";
}  // namespace

int main(int argc, char **argv) {
  return roq::samples::example_1::Controller(
      argc,
      argv,
      DESCRIPTION).run();
}