// "Code share": of the time the game spends in its own code, how much runs in FullDecomp's
// byte-matched C++ (the native-decomp objects, rebuild/recomp/native) and how much in the original
// code (the lifted Fable.exe). A sampler classifies the interrupted program counter: inside the range
// the native objects register (nd_register_range, from their constructor) is "FullDecomp", inside the
// lifted functions' range is "original", anything else (renderer, driver, libc, host code) is left
// out. The share of the last second is shown as one line: the RG34XX / Linux overlay (host menu item,
// fed by the profiler's 1 kHz SIGPROF samples or its own sampler), or on Windows drawn over the frame
// with Direct3D 9 (F9 toggles; a sampler thread reads the game thread's context at 1 kHz).
// FABLE_CODE_SHARE=1 starts it shown.
#include "host.hpp"
#include "code_share.hpp"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include <string>

#ifndef _WIN32
#include <csignal>
#include <sys/time.h>
#include <ucontext.h>
#endif

extern "C" {
typedef struct RecompEntry { uint32_t addr; GuestFn fn; } RecompEntry;
const RecompEntry* recomp_table_entries(uint32_t i, uint32_t* n);
#if defined(FABLE_CODELAYOUT)
void fable_hot_text_begin(void);  // posix/codelayout.cpp: the hot lifted functions, ordered first
void fable_hot_text_end(void);
#endif
}

namespace host::codeshare {
namespace {
std::atomic<uintptr_t> g_ndLo{0}, g_ndHi{0};
std::atomic<uint32_t> g_ndCount{0};
uintptr_t g_liftLo = 0, g_liftHi = 0, g_hotLo = 0, g_hotHi = 0;
std::atomic<uint64_t> g_native{0}, g_lifted{0};
std::atomic<bool> g_on{false};
std::once_flag g_init;
std::mutex g_lineLock;
std::string g_line = "Code share: measuring...";
std::chrono::steady_clock::time_point g_last;
uint64_t g_lastNative = 0, g_lastLifted = 0;

// the lifted functions' host range: the lowest and highest function in the recomp tables (the
// lifted objects are linked together; the end is padded by the largest function's likely size).
// The RG34XX build orders the hot lifted functions first (between the codelayout markers), with
// host code between them and the rest: two ranges then.
void findLifted() {
#if defined(FABLE_CODELAYOUT)
    g_hotLo = reinterpret_cast<uintptr_t>(&fable_hot_text_begin), g_hotHi = reinterpret_cast<uintptr_t>(&fable_hot_text_end);
    if (g_hotHi <= g_hotLo) g_hotLo = g_hotHi = 0;
#endif
    uintptr_t lo = ~uintptr_t{0}, hi = 0;
    uint32_t n = 0;
    for (uint32_t t = 0; const RecompEntry* e = recomp_table_entries(t, &n); ++t)
        for (uint32_t i = 0; i < n; ++i) {
            const uintptr_t f = reinterpret_cast<uintptr_t>(e[i].fn);
            if (!f || (f >= g_hotLo && f < g_hotHi)) continue;
            lo = std::min(lo, f), hi = std::max(hi, f);
        }
    if (hi) g_liftLo = lo, g_liftHi = hi + 0x10000;
}

void init() {
    findLifted();
    if (const char* e = std::getenv("FABLE_CODE_SHARE"); e && std::atoi(e) != 0) g_on = true;
    g_last = std::chrono::steady_clock::now();
}

#ifndef _WIN32
// a sampler of our own when the profiler is not running (FABLE_PROFILE=0)
bool g_profilerOwnsSigprof = false;
void onSigprof(int, siginfo_t*, void* ctx) {
    auto* uc = static_cast<ucontext_t*>(ctx);
#if defined(__aarch64__)
    sample(uc->uc_mcontext.pc);
#elif defined(__x86_64__)
    sample(static_cast<uintptr_t>(uc->uc_mcontext.gregs[REG_RIP]));
#else
    (void)uc;
#endif
}
void ensureSampler() {
    static std::once_flag once;
    std::call_once(once, [] {
        if (g_profilerOwnsSigprof) return;
        struct sigaction sa{};
        sa.sa_sigaction = onSigprof;
        sa.sa_flags = SA_SIGINFO | SA_RESTART;
        sigemptyset(&sa.sa_mask);
        sigaction(SIGPROF, &sa, nullptr);
        itimerval tv{};
        tv.it_interval.tv_usec = 1000;
        tv.it_value.tv_usec = 1000;
        setitimer(ITIMER_PROF, &tv, nullptr);
    });
}
#else
HANDLE g_gameThread = nullptr;
DWORD WINAPI samplerMain(void*) {
    timeBeginPeriod(1);
    for (;;) {
        Sleep(1);
        if (!g_on.load(std::memory_order_relaxed) || !g_gameThread) continue;
        if (SuspendThread(g_gameThread) == static_cast<DWORD>(-1)) continue;
        CONTEXT cx{};
        cx.ContextFlags = CONTEXT_CONTROL;
        if (GetThreadContext(g_gameThread, &cx)) sample(static_cast<uintptr_t>(cx.Rip));
        ResumeThread(g_gameThread);
    }
}
void ensureSampler() {
    static std::once_flag once;
    std::call_once(once, [] { CloseHandle(CreateThread(nullptr, 0, samplerMain, nullptr, 0, nullptr)); });
}
#endif

// the share of the last window, once a second
void roll() {
    const auto now = std::chrono::steady_clock::now();
    if (now - g_last < std::chrono::seconds(1)) return;
    g_last = now;
    const uint64_t n = g_native.load(), l = g_lifted.load();
    const uint64_t dn = n - g_lastNative, dl = l - g_lastLifted;
    g_lastNative = n, g_lastLifted = l;
    char buf[200];
    const uint32_t fns = g_ndCount.load();
    if (!g_ndHi.load())
        std::snprintf(buf, sizeof buf, "Code share: no FullDecomp code in this build (100%% original)");
    else if (dn + dl == 0)
        std::snprintf(buf, sizeof buf, "Code share: no game-code samples yet (%u FullDecomp functions)", fns);
    else
        std::snprintf(buf, sizeof buf, "Your code (FullDecomp) %.1f%%  |  original %.1f%%  of game-code time  (%u functions native)", 100.0 * dn / (dn + dl),
                      100.0 * dl / (dn + dl), fns);
    std::lock_guard<std::mutex> lk(g_lineLock);
    g_line = buf;
}
}  // namespace

// async-signal-safe: atomics only
void sample(uintptr_t pc) {
    const uintptr_t lo = g_ndLo.load(std::memory_order_relaxed), hi = g_ndHi.load(std::memory_order_relaxed);
    if (pc >= lo && pc < hi) g_native.fetch_add(1, std::memory_order_relaxed);
    else if ((pc >= g_liftLo && pc < g_liftHi) || (pc >= g_hotLo && pc < g_hotHi)) g_lifted.fetch_add(1, std::memory_order_relaxed);
}

void registerNative(uintptr_t lo, uintptr_t hi, uint32_t count) {
    g_ndLo = lo, g_ndHi = hi, g_ndCount = count;
}

#ifndef _WIN32
void profilerOwnsSigprof() { g_profilerOwnsSigprof = true; }
#else
void setGameThread(void* h) { g_gameThread = static_cast<HANDLE>(h); }
#endif

bool on() {
    std::call_once(g_init, init);
    if (g_on.load()) ensureSampler();
    return g_on.load();
}

void toggle() {
    std::call_once(g_init, init);
    g_on = !g_on.load();
    if (g_on) ensureSampler();
    host::log("code share: %s", g_on ? "shown" : "hidden");
}

std::string line() {
    roll();
    std::lock_guard<std::mutex> lk(g_lineLock);
    return g_line;
}

}  // namespace host::codeshare

// called by the native-decomp objects' constructor (fable_lower)
extern "C" void nd_register_range(void* begin, void* end, uint32_t count) {
    host::codeshare::registerNative(reinterpret_cast<uintptr_t>(begin), reinterpret_cast<uintptr_t>(end), count);
}
