#ifndef WEBCC_CLIENT_H_
#define WEBCC_CLIENT_H_

#include "webcc/blocking_client_base.h"

namespace webcc {

class Client final : public BlockingClientBase {
public:
  explicit Client(boost::asio::io_context& io_context)
      : BlockingClientBase(io_context, "80"), socket_(io_context) {
  }

  ~Client() override = default;

  void Close() override;

protected:
  SocketType& GetSocket() override {
    return socket_;
  }

  void AsyncWrite(const std::vector<boost::asio::const_buffer>& buffers,
                  AsyncRWHandler&& handler) override;

  void AsyncReadSome(boost::asio::mutable_buffer buffer,
                     AsyncRWHandler&& handler) override;

private:
  boost::asio::ip::tcp::socket socket_;
};

}  // namespace webcc

#endif  // WEBCC_CLIENT_H_
