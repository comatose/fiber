#include "fiber_manager.hpp"

#include <boost/context/all.hpp>

using namespace std;
using namespace boost::context;

namespace glow {

void FiberManager::run() {
  if(ready_fibers_.empty())
    return;

  auto& first_fiber = *ready_fibers_.front();
  jump_fcontext(&ctx_, first_fiber.ctx_,
                reinterpret_cast<intptr_t>(&first_fiber));

  destroyDoneFibers();
}

void FiberManager::addCore(shared_ptr<FiberCore>&& core) {
  ready_fibers_.push(move(core));
}

void FiberManager::yieldCurrentFiber() {
  if(ready_fibers_.size() == 1)
    return;

  ready_fibers_.push(move(ready_fibers_.front()));
  ready_fibers_.pop();

  destroyDoneFibers();

  auto& next_fiber = *ready_fibers_.front();
  auto& curr_fiber = *ready_fibers_.back();

  jump_fcontext(&curr_fiber.ctx_, next_fiber.ctx_,
                reinterpret_cast<intptr_t>(&next_fiber));
}

void FiberManager::removeCurrentFiber() {
  done_fibers_.push(move(ready_fibers_.front()));
  ready_fibers_.pop();

  auto& curr_fiber = *done_fibers_.back();
  if(ready_fibers_.empty()) {
    jump_fcontext(&curr_fiber.ctx_, ctx_, 0);
  }

  auto& next_fiber = *ready_fibers_.front();
  jump_fcontext(&curr_fiber.ctx_, next_fiber.ctx_,
                reinterpret_cast<intptr_t>(&next_fiber));
}

void FiberManager::destroyDoneFibers() {
  auto tmp = move(done_fibers_);
}

}
