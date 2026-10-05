// Main-thread video loop (window and presentation). Placeholder until the GPU lands.
#include "xhost.hpp"

#include <chrono>
#include <thread>

namespace xb {

void videoMain() {
    for (;;) std::this_thread::sleep_for(std::chrono::seconds(1));
}

} // namespace xb
