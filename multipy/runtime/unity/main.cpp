// Copyright (c) Meta Platforms, Inc. and affiliates.
// All rights reserved.
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <memory>
#include "deploy.h"
#include "unity/xar_environment.h"

namespace torch {
namespace deploy {

// the way we lookup main module follows how an xar file is setup
std::string lookupMainModule(InterpreterManager& m) {
  auto I = m.acquireOne();
  auto mainModule =
      I.global("__manifest__", "fbmake").attr("get")({"main_module"});
  std::ostringstream ss;
  ss << mainModule.toIValue();
  LOG(INFO) << "main module is " << ss.str();
  return ss.str();
}

int doMain(int /* argc */, char** argv) {
  std::shared_ptr<Environment> env = std::make_shared<XarEnvironment>(argv[0]);
  InterpreterManager m(2, env);

  auto mainModule = lookupMainModule(m);
  auto I = m.acquireOne();
  I.global("runpy", "run_module")({mainModule});
  return 0;
}

} // namespace deploy
} // namespace torch

// NOLINTNEXTLINE(bugprone-exception-escape)
int main(int argc, char** argv) {
  return torch::deploy::doMain(argc, argv);
}
