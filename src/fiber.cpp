#include "fiber.hpp"

#include "fiber_manager.hpp"

using namespace std;

namespace glow {

void Fiber::addCoreToManager(FiberManager& manager, shared_ptr<FiberCore>&& core) const {
  manager.addCore(move(core));
}

bool Fiber::valid() const {
  return !core_.expired();
}

}
