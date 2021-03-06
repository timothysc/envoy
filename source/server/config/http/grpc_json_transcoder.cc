#include "server/config/http/grpc_json_transcoder.h"

#include "envoy/registry/registry.h"

#include "common/grpc/json_transcoder_filter.h"

namespace Envoy {
namespace Server {
namespace Configuration {

HttpFilterFactoryCb
GrpcJsonTranscoderFilterConfig::createFilterFactory(const Json::Object& config_json,
                                                    const std::string&, FactoryContext&) {
  Grpc::JsonTranscoderConfigSharedPtr config =
      std::make_shared<Grpc::JsonTranscoderConfig>(config_json);

  return [config](Http::FilterChainFactoryCallbacks& callbacks) -> void {
    callbacks.addStreamFilter(Http::StreamFilterSharedPtr{new Grpc::JsonTranscoderFilter(*config)});
  };
}

/**
 * Static registration for the grpc transcoding filter. @see RegisterNamedHttpFilterConfigFactory.
 */
static Registry::RegisterFactory<GrpcJsonTranscoderFilterConfig, NamedHttpFilterConfigFactory>
    register_;

} // namespace Configuration
} // namespace Server
} // namespace Envoy
