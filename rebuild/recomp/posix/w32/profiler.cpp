// A sampling profiler for the handheld builds (FABLE_PROFILE=1): SIGPROF at 1 kHz of process
// CPU time records the interrupted program counter; a thread resolves the samples every
// minute into the lifted guest function (through the recompiled tables) or the host object
// and symbol, and logs the hottest entries. Costs about 1% of the CPU.
#include "w32.hpp"

#include <dlfcn.h>
#include <signal.h>
#include <sys/time.h>
#include <ucontext.h>
#include <unistd.h>

#include <algorithm>
#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <string>
#include <thread>
#include <vector>

extern "C" {
typedef void (*GuestFn)(void*);
typedef struct RecompEntry { uint32_t addr; GuestFn fn; } RecompEntry;
uint32_t recomp_num_tables(void);
const RecompEntry* recomp_table_entries(uint32_t i, uint32_t* n);
}
namespace host { void log(const char* fmt, ...); }

namespace w32 {
namespace {

constexpr size_t kRing = 1 << 18;
uintptr_t g_ring[kRing];
std::atomic<size_t> g_head{0};
std::atomic<uint64_t> g_total{0};

void onProf(int, siginfo_t*, void* ctx) {
    auto* uc = static_cast<ucontext_t*>(ctx);
#if defined(__aarch64__)
    const uintptr_t pc = uc->uc_mcontext.pc;
#elif defined(__x86_64__)
    const uintptr_t pc = static_cast<uintptr_t>(uc->uc_mcontext.gregs[REG_RIP]);
#else
    const uintptr_t pc = 0;
    (void)uc;
#endif
    const size_t i = g_head.fetch_add(1, std::memory_order_relaxed);
    g_ring[i & (kRing - 1)] = pc;
    g_total.fetch_add(1, std::memory_order_relaxed);
}

struct GuestFnEntry { uintptr_t host; uint32_t guest; };
std::vector<GuestFnEntry> g_guestFns;  // sorted by host address
uintptr_t g_guestLo = 0, g_guestHi = 0;

void collectGuestFns() {
    for (uint32_t t = 0; t < recomp_num_tables(); ++t) {
        uint32_t n = 0;
        const RecompEntry* e = recomp_table_entries(t, &n);
        for (uint32_t i = 0; i < n; ++i) g_guestFns.push_back({reinterpret_cast<uintptr_t>(e[i].fn), e[i].addr});
    }
    std::sort(g_guestFns.begin(), g_guestFns.end(), [](const GuestFnEntry& a, const GuestFnEntry& b) { return a.host < b.host; });
    if (!g_guestFns.empty()) g_guestLo = g_guestFns.front().host, g_guestHi = g_guestFns.back().host + 0x20000;  // the last function's extent is unknown
}

void report() {
    const size_t head = g_head.load();
    const size_t n = std::min(head, kRing);
    if (n < 100) return;
    std::vector<uintptr_t> pcs(g_ring + 0, g_ring + n);
    std::map<std::string, unsigned> hist;
    unsigned guest = 0, host = 0;
    for (uintptr_t pc : pcs) {
        if (pc >= g_guestLo && pc < g_guestHi) {
            auto it = std::upper_bound(g_guestFns.begin(), g_guestFns.end(), pc, [](uintptr_t v, const GuestFnEntry& e) { return v < e.host; });
            if (it != g_guestFns.begin()) {
                --it;
                char buf[32];
                std::snprintf(buf, sizeof buf, "guest 0x%08X", it->guest);
                ++hist[buf];
                ++guest;
                continue;
            }
        }
        Dl_info info{};
        char buf[256];
        if (dladdr(reinterpret_cast<void*>(pc), &info) && info.dli_fname) {
            const char* base = std::strrchr(info.dli_fname, '/');
            base = base ? base + 1 : info.dli_fname;
            if (info.dli_sname) std::snprintf(buf, sizeof buf, "host %s!%s", base, info.dli_sname);
            else std::snprintf(buf, sizeof buf, "host %s+0x%lx", base, static_cast<unsigned long>(pc - reinterpret_cast<uintptr_t>(info.dli_fbase)));
        } else {
            std::snprintf(buf, sizeof buf, "host ?+0x%lx", static_cast<unsigned long>(pc));
        }
        ++hist[buf];
        ++host;
    }
    std::vector<std::pair<unsigned, std::string>> top;
    for (auto& [k, v] : hist) top.push_back({v, k});
    std::sort(top.begin(), top.end(), [](auto& a, auto& b) { return a.first > b.first; });
    host::log("profile: %zu samples (%u guest, %u host) over the last window, %llu in all", n, guest, host, static_cast<unsigned long long>(g_total.load()));
    for (size_t i = 0; i < top.size() && i < 60; ++i)
        host::log("profile: %5.1f%% %s (%u)", 100.0 * top[i].first / n, top[i].second.c_str(), top[i].first);
    g_head.store(0);
}

}  // namespace

void startProfiler() {
    const char* e = std::getenv("FABLE_PROFILE");
    if (!e || std::atoi(e) == 0) return;
    const int seconds = std::atoi(e) > 1 ? std::atoi(e) : 60;
    struct sigaction sa{};
    sa.sa_sigaction = onProf;
    sa.sa_flags = SA_SIGINFO | SA_RESTART;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGPROF, &sa, nullptr);
    itimerval tv{};
    tv.it_interval.tv_usec = 1000;
    tv.it_value.tv_usec = 1000;
    setitimer(ITIMER_PROF, &tv, nullptr);
    std::fprintf(stderr, "profile: sampling at 1 kHz, report every %d s\n", seconds);
    std::thread([seconds] {
        sleep(5);
        collectGuestFns();
        for (;;) {
            sleep(static_cast<unsigned>(seconds));
            report();
        }
    }).detach();
}

}  // namespace w32
