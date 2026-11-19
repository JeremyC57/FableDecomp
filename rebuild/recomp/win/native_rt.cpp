// Runtime for the native-decomp objects (rebuild/recomp/native/fable_lower.cpp): FullDecomp's
// byte-matched C++ lowered to host code that shares guest memory with the lifted code. The lowered
// functions keep their stack frames on the guest stack, call lifted (or host-trap) code through
// nd_call with the x86 arguments on the guest stack, and are entered from lifted code through
// nd_entry_<ADDR> (wrapped by the lifter's --wrap ADDR=nd_wrap_<ADDR>, which asks nd_enabled).
//
// FABLE_NATIVE_DECOMP=0 turns every lowered function off (the lifted bodies run);
// FABLE_ND_OFF=<hex>[,<hex>-<hex>...] single addresses or ranges, for bisecting a crash.
// FABLE_ND_TIME=1 times every call of a wrapped function, native or lifted (inclusive of what it
// calls), and logs the top functions by total time every 30 s ("native decomp time:" lines): two
// runs over the same route, one with FABLE_NATIVE_DECOMP=0, compare the two bodies function by function.
#include "host.hpp"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include <vector>

using namespace host;

namespace {
thread_local Ctx* t_nd = nullptr;

struct Off { uint32_t lo, hi; };
std::vector<Off> g_off;
bool g_all = true, g_time = false;
std::once_flag g_once;

void readEnv() {
    if (const char* e = std::getenv("FABLE_NATIVE_DECOMP")) g_all = std::atoi(e) != 0;
    if (const char* e = std::getenv("FABLE_ND_TIME")) g_time = std::atoi(e) != 0;
    if (const char* e = std::getenv("FABLE_ND_OFF")) {
        for (const char* p = e; *p;) {
            char* end = nullptr;
            const unsigned long lo = std::strtoul(p, &end, 16);
            if (end == p) break;
            unsigned long hi = lo;
            if (*end == '-') {
                p = end + 1;
                hi = std::strtoul(p, &end, 16);
            }
            g_off.push_back({static_cast<uint32_t>(lo), static_cast<uint32_t>(hi)});
            p = *end ? end + 1 : end;
        }
    }
    host::log("native decomp: %s%s%s", g_all ? "on" : "off (FABLE_NATIVE_DECOMP=0): the lifted code runs",
              g_off.empty() ? "" : ", with addresses off: ", g_off.empty() ? "" : std::getenv("FABLE_ND_OFF"));
}

// FABLE_ND_TIME: per-address call counts and nanoseconds, an open-addressed table (addresses only
// ever go in), reported every 30 s
struct TimeSlot {
    std::atomic<uint32_t> addr{0};
    std::atomic<uint64_t> calls{0}, ns{0};
    std::atomic<uint32_t> native{0};
};
constexpr uint32_t kTimeSlots = 4096;
TimeSlot g_times[kTimeSlots];
std::atomic<uint64_t> g_nextReport{0};

void timeReport() {
    struct Row { uint32_t addr; uint64_t calls, ns; bool native; };
    std::vector<Row> rows;
    uint64_t total = 0;
    for (TimeSlot& t : g_times)
        if (const uint32_t a = t.addr.load(std::memory_order_relaxed)) {
            const uint64_t c = t.calls.exchange(0), n = t.ns.exchange(0);
            if (c) rows.push_back({a, c, n, t.native.load() != 0}), total += n;
        }
    std::sort(rows.begin(), rows.end(), [](const Row& a, const Row& b) { return a.ns > b.ns; });
    host::log("native decomp time: %zu functions called in the last window, %.1f ms in all (inclusive of callees)", rows.size(), total / 1e6);
    for (size_t i = 0; i < rows.size() && i < 30; ++i)
        host::log("native decomp time: 0x%08X %s %8llu calls %9.3f ms %8.1f ns/call", rows[i].addr, rows[i].native ? "native" : "lifted",
                  static_cast<unsigned long long>(rows[i].calls), rows[i].ns / 1e6, static_cast<double>(rows[i].ns) / rows[i].calls);
}

// the lifted (or host-trap) code at a guest address, with a small per-thread cache
void callGuest(Ctx* c, uint32_t target) {
    struct Slot { uint32_t addr; GuestFn f; };
    static thread_local Slot cache[512];
    Slot& s = cache[(target >> 4) & 511];
    if (s.addr != target || !s.f) {
        s.f = recomp_lookup(target);
        s.addr = target;
    }
    if (s.f) s.f(c);
    else recomp_dispatch(c, target);
}
}  // namespace

extern "C" {

// bit 0: run the native body; bit 1: time the call
int nd_enabled(uint32_t addr, Ctx*) {
    std::call_once(g_once, readEnv);
    const int t = g_time ? 2 : 0;
    if (!g_all) return t;
    for (const Off& o : g_off)
        if (addr >= o.lo && addr <= o.hi) return t;
    return 1 | t;
}

uint64_t nd_clock() {
    return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
}

void nd_time_add(uint32_t addr, uint64_t t0, uint32_t flags);

void nd_time_add(uint32_t addr, uint64_t t0, uint32_t flags) {
    const uint64_t now = nd_clock();
    uint32_t h = (addr * 2654435761u) >> 20;
    for (uint32_t probe = 0; probe < kTimeSlots; ++probe, h = (h + 1) & (kTimeSlots - 1)) {
        TimeSlot& t = g_times[h];
        uint32_t a = t.addr.load(std::memory_order_relaxed);
        if (a != addr) {
            if (a != 0) continue;
            if (!t.addr.compare_exchange_strong(a, addr) && a != addr) continue;
        }
        t.calls.fetch_add(1, std::memory_order_relaxed);
        t.ns.fetch_add(now - t0, std::memory_order_relaxed);
        t.native.store(flags & 1, std::memory_order_relaxed);
        break;
    }
    uint64_t next = g_nextReport.load(std::memory_order_relaxed);
    if (!next) g_nextReport.compare_exchange_strong(next, now + 30'000'000'000ull);
    else if (now >= next && g_nextReport.compare_exchange_strong(next, now + 30'000'000'000ull)) timeReport();
}

// entry from lifted code: the guest esp (the return address is at [esp])
uint32_t nd_entry_begin(Ctx* c) {
    t_nd = c;
    return c->esp;
}

uint32_t nd_get_reg(Ctx* c, uint32_t r) { return r == 1 ? c->ecx : c->edx; }

// kind: 0 none, 1 eax, 2 edx:eax, 3 st(0); pop: the stack bytes the x86 callee removes
void nd_entry_end(Ctx* c, uint32_t kind, uint32_t eax, uint32_t edx, double st, uint32_t pop) {
    switch (kind) {
        case 1: c->eax = eax; break;
        case 2: c->eax = eax, c->edx = edx; break;
        case 3: fpush(c, st); break;
        default: break;
    }
    c->esp += 4 + pop;
}

// a lowered function's stack frame on the guest stack: returns the esp to restore
uint32_t nd_frame_enter(uint32_t size) {
    Ctx* c = t_nd;
    const uint32_t old = c->esp;
    c->esp = old - size;
    return old;
}

void nd_frame_leave(uint32_t old) { t_nd->esp = old; }

struct NdResult {
    uint32_t eax, edx;
    double st0;
};

// a call out of the lowered code: the argument words (first argument first) on the guest stack,
// ecx / edx when flags bits 0 / 1, the result kind in flags bits 8-9 (as nd_entry_end's)
void nd_call(uint32_t target, const uint32_t* words, uint32_t n, uint32_t flags, uint32_t ecx, uint32_t edx, NdResult* r) {
    Ctx* c = t_nd;
    const uint32_t esp0 = c->esp;
    uint32_t esp = esp0 - 4 * n;
    for (uint32_t i = 0; i < n; ++i) wr32(esp + 4 * i, words[i]);
    esp -= 4;
    wr32(esp, kReturnSentinel);
    c->esp = esp;
    if (flags & 1) c->ecx = ecx;
    if (flags & 2) c->edx = edx;
    callGuest(c, target);
    t_nd = c;  // the callee may have entered other lowered code on this thread
    r->eax = c->eax, r->edx = c->edx;
    r->st0 = ((flags >> 8) & 3) == 3 ? fpop(c) : 0.0;
    c->esp = esp0;
}

}  // extern "C"
