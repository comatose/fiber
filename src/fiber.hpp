#ifndef __FIBER_HPP__
#define __FIBER_HPP__

#include <memory>

#include <cstdint>

#include "fiber_core.hpp"

namespace glow {

class FiberManager;

class Fiber {
 public:
  template<class Func, class... Args>
  Fiber(FiberManager& manager, std::size_t stack_size, Func&& runner,
        Args&&... args) {
    using namespace std;

    auto core = make_shared<FiberCore>(manager, stack_size,
                                       bind(runner, forward<Args>(args)...));
    core_ = core;
    addCoreToManager(manager, std::move(core));
  }

  Fiber() = default;
  Fiber(const Fiber&) = delete;
  Fiber(Fiber&&) = default;

  Fiber& operator=(const Fiber&) = delete;
  Fiber& operator=(Fiber&&) = default;

  bool valid() const;

#ifdef __TEST__
 private:
#endif

  void addCoreToManager(FiberManager&, std::shared_ptr<FiberCore>&&) const;

  std::weak_ptr<FiberCore> core_;
};

} // namespace glow

#endif // __FIBER_HPP__
