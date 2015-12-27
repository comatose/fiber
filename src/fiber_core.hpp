#ifndef __FIBER_CORE_HPP__
#define __FIBER_CORE_HPP__

#include <memory>
#include <functional>

#include <cstdint>

#include <boost/context/all.hpp>

namespace glow {

using fiber_result_t = intptr_t;

class FiberManager;

class FiberCore {
  friend class FiberManager;

 public:
  FiberCore(FiberManager& manager, std::size_t stack_size,
            std::function<fiber_result_t()>&& runner);

  FiberCore() = delete;
  FiberCore(const FiberCore&) = delete;
  FiberCore(FiberCore&&) = delete;

  FiberCore& operator=(const FiberCore&) = delete;
  FiberCore& operator=(FiberCore&&) = delete;

  static void callRunner(FiberCore*);

#ifdef __TEST__
 private:
#endif

  void exit();

  std::unique_ptr<char[]> stack_;
  std::function<fiber_result_t()> runner_;
  boost::context::fcontext_t ctx_;
  FiberManager* manager_;
};

} // namespace glow

#endif // __FIBER_CORE_HPP__
