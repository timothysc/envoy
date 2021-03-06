#pragma once

#include "exe/hot_restart.h"

#include "server/options_impl.h"

namespace Envoy {
/**
 * This is the real main body that executes after site-specific
 * main() runs.
 *
 * @param options Options object initialized by site-specific code
 * @param restarter HotRestart object initialized in site-specific code.
 * @return int Return code that should be returned from the actual main()
 */
int main_common(Envoy::OptionsImpl& options, Envoy::Server::HotRestartImpl& restarter);

} // namespace Envoy
