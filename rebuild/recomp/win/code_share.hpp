// Code share (code_share.cpp): the share of game-code time in FullDecomp's native code vs the
// original (lifted) code, sampled at 1 kHz and shown as one line by the overlay / Present.
#pragma once

#include <cstdint>
#include <string>

namespace host::codeshare {
void sample(uintptr_t pc);  // a sampled program counter (async-signal-safe)
bool on();                  // shown?
void toggle();
std::string line();         // the line to draw (rolled once a second)
#ifndef _WIN32
void profilerOwnsSigprof();  // the profiler's SIGPROF handler calls sample(); no second sampler
#else
void setGameThread(void* thread);  // the game's main thread, for the Windows sampler
#endif
}  // namespace host::codeshare
