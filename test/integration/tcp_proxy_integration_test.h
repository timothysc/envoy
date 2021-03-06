#pragma once

#include <memory>
#include <string>

#include "test/integration/integration.h"

#include "gtest/gtest.h"

namespace Envoy {
class TcpProxyIntegrationTest : public BaseIntegrationTest,
                                public testing::TestWithParam<Network::Address::IpVersion> {
public:
  TcpProxyIntegrationTest() : BaseIntegrationTest(GetParam()) {}

  void SetUp() override {
    fake_upstreams_.emplace_back(new FakeUpstream(0, FakeHttpConnection::Type::HTTP1, version_));
    registerPort("upstream_0", fake_upstreams_.back()->localAddress()->ip()->port());
    fake_upstreams_.emplace_back(new FakeUpstream(0, FakeHttpConnection::Type::HTTP1, version_));
    registerPort("upstream_1", fake_upstreams_.back()->localAddress()->ip()->port());
    createTestServer("test/config/integration/tcp_proxy.json",
                     {"tcp_proxy", "tcp_proxy_with_tls_termination"});
  }

  void TearDown() override {
    test_server_.reset();
    fake_upstreams_.clear();
  }
};
} // namespace Envoy
