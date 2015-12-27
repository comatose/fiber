#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_COLOUR_NONE

#include <catch.hpp>

#include <iostream>
#include <vector>

#include <boost/context/all.hpp>

#define __TEST__

#include "fiber.hpp"
#include "fiber_manager.hpp"

using namespace std;
using namespace glow;

TEST_CASE("Fiber constructor should do right.") {
  FiberManager fiber_manager;

  Fiber fiber0(fiber_manager, 1024*1024, [] {
      return 0;
    });

  SECTION("Fiber can be move-constructed.") {
    // auto copied_fiber = fiber0; // shouldn't be compiled

    auto moved_fiber = move(fiber0);
    REQUIRE(moved_fiber.valid());

    REQUIRE(!fiber0.valid());
  }

  SECTION("Fiber can be move-assigned.") {
    Fiber moved_fiber;
    REQUIRE(!moved_fiber.valid());

    // copied_fiber = fiber0; // shouldn't be compiled
    moved_fiber = move(fiber0);
    REQUIRE(moved_fiber.valid());

    REQUIRE(!fiber0.valid());
  }
}

SCENARIO("Fibers should run correctly.") {
  GIVEN("Two fibers") {
    FiberManager fiber_manager;

    Fiber fiber1(fiber_manager, 1024*1024, [&fiber_manager] {
        cout << "fiber 1 stared." << endl;
        fiber_manager.yieldCurrentFiber();
        cout << "fiber 1 resumed." << endl;
        return 0;
      });

    Fiber fiber2(fiber_manager, 1024*1024, [&fiber_manager] {
        cout << "fiber 2 stared." << endl;
        fiber_manager.yieldCurrentFiber();
        cout << "fiber 2 resumed." << endl;
        return 0;
      });

    fiber_manager.run();
  }
}
