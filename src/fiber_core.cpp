#include "fiber_core.hpp"

#include "fiber_manager.hpp"

using namespace boost::context;

namespace glow {

FiberCore::FiberCore(FiberManager& manager, std::size_t stack_size,
                     std::function<fiber_result_t()>&& runner)
    : stack_(std::make_unique<char[]>(stack_size)),
      runner_(std::move(runner)),
      ctx_(make_fcontext(stack_.get() + stack_size,
                         stack_size,
                         reinterpret_cast<void (*)(intptr_t)>(callRunner))),
      manager_(&manager) {
}

void FiberCore::exit() {
  manager_->removeCurrentFiber();
}

void FiberCore::callRunner(FiberCore* self) {
  self->runner_();

  self->exit();
}

}
