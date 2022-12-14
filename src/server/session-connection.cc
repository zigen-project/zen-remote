#include "server/session-connection.h"

#include "core/logger.h"

namespace zen::remote::server {

SessionConnection::SessionConnection(int control_fd, std::string host_port)
    : control_fd_(control_fd)
{
  grpc::ChannelArguments args;

  args.SetString(GRPC_ARG_OPTIMIZATION_TARGET, "latency");
  args.SetInt(GRPC_ARG_KEEPALIVE_TIME_MS, 1000);
  args.SetInt(GRPC_ARG_KEEPALIVE_TIMEOUT_MS, 3000);
  args.SetInt(GRPC_ARG_HTTP2_MAX_PINGS_WITHOUT_DATA, 0);
  args.SetInt(GRPC_ARG_KEEPALIVE_PERMIT_WITHOUT_CALLS, 1);

  grpc_channel_ = grpc::CreateCustomChannel(
      host_port, grpc::InsecureChannelCredentials(), args);
}

void
SessionConnection::Disable()
{
  std::lock_guard<std::mutex> lock(mtx_);
  disabled_ = true;
  control_fd_ = -1;
}

void
SessionConnection::NotifyDisconnection()
{
  std::lock_guard<std::mutex> lock(mtx_);
  if (disabled_) return;
  uint8_t msg = kDisconnect;

  if (control_fd_ > 0) {
    write(control_fd_, &msg, sizeof(msg));
  }
}

}  // namespace zen::remote::server
