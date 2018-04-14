/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <roq/api.h>

#include <string>
#include <unordered_map>

namespace examples {
namespace common {

class Gateway final {
 public:
  Gateway(roq::Strategy::Dispatcher& dispatcher, const std::string& name);
  const std::string& get_order_template(bool close) const {
    return close ? _gtc_close : _gtc_open;
  }
  bool is_ready() const { return _download == false && _order_manager_ready; }
  void on(const roq::DownloadBeginEvent& event);
  void on(const roq::DownloadEndEvent& event);
  void on(const roq::GatewayStatusEvent& event);
  void on(const roq::OrderUpdateEvent& event);
  bool parse_open_close(const char *order_template) const;
  double get_fill_quantity(const roq::OrderUpdate& order_update);
  uint32_t create_order(
      const std::string& exchange,
      const std::string& instrument,
      roq::TradeDirection direction,
      double quantity,
      double price,
      const std::string& order_template);

 private:
  void update_max_order_id(uint32_t order_id);

 private:
  roq::Strategy::Dispatcher& _dispatcher;
  const std::string _name;
  const std::string _gtc_open;
  const std::string _gtc_close;
  bool _download = false;
  bool _order_manager_ready = false;
  uint32_t _max_order_id = 0;
  std::unordered_map<uint32_t, double> _order_traded_quantity;
};

}  // namespace common
}  // namespace examples
