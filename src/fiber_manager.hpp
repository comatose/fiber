#ifndef __FIBER_MANAGER_HPP__
#define __FIBER_MANAGER_HPP__

#include <queue>
#include <memory>

#include "fiber_core.hpp"

namespace glow {

class FiberManager {
  friend class Fiber;

 public:
  void run();
  void yieldCurrentFiber();

#ifdef __TEST__
 private:
#endif

  void addCore(std::shared_ptr<FiberCore>&&);
  void removeCurrentFiber();
  void destroyDoneFibers();

  std::queue<std::shared_ptr<FiberCore>> ready_fibers_;
  std::queue<std::shared_ptr<FiberCore>> done_fibers_;
  boost::context::fcontext_t ctx_;
};

}

#endif // __FIBER_MANAGER_HPP__
