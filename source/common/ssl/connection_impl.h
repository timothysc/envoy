#pragma once

#include <cstdint>
#include <string>

#include "common/network/connection_impl.h"
#include "common/ssl/context_impl.h"

namespace Envoy {
namespace Ssl {

class ConnectionImpl : public Network::ConnectionImpl, public Connection {
public:
  enum class InitialState { Client, Server };

  ConnectionImpl(Event::DispatcherImpl& dispatcher, int fd,
                 Network::Address::InstanceConstSharedPtr remote_address,
                 Network::Address::InstanceConstSharedPtr local_address, Context& ctx,
                 InitialState state);
  ~ConnectionImpl();

  // Network::Connection
  std::string nextProtocol() override;
  Ssl::Connection* ssl() override { return this; }

  // Ssl::Connection
  bool peerCertificatePresented() override;
  std::string uriSanLocalCertificate() override;
  std::string sha256PeerCertificateDigest() override;
  std::string subjectPeerCertificate() override;
  std::string uriSanPeerCertificate() override;

private:
  PostIoAction doHandshake();
  void drainErrorQueue();
  std::string getUriSanFromCertificate(X509* cert);

  // Network::ConnectionImpl
  void closeSocket(uint32_t close_type) override;
  IoResult doReadFromSocket() override;
  IoResult doWriteToSocket() override;
  void onConnected() override;

  ContextImpl& ctx_;
  bssl::UniquePtr<SSL> ssl_;
  bool handshake_complete_{};
};

class ClientConnectionImpl final : public ConnectionImpl, public Network::ClientConnection {
public:
  ClientConnectionImpl(Event::DispatcherImpl& dispatcher, Context& ctx,
                       Network::Address::InstanceConstSharedPtr address);

  // Network::ClientConnection
  void connect() override;
};

} // namespace Ssl
} // namespace Envoy
