// Runtime for the native-decomp objects (rebuild/recomp/native/fable_lower.cpp): FullDecomp's
// byte-matched C++ lowered to host code that shares guest memory with the lifted code. The lowered
// functions keep their stack frames on the guest stack, call lifted (or host-trap) code through
// nd_call with the x86 arguments on the guest stack, and are entered from lifted code through
// nd_entry_<ADDR> (wrapped by the lifter's --wrap ADDR=nd_wrap_<ADDR>, which asks nd_enabled).
//
// FABLE_NATIVE_DECOMP=0 turns every lowered function off (the lifted bodies run);
// FABLE_ND_OFF=<hex>[,<hex>-<hex>...] single addresses or ranges, for bisecting a crash.
// FABLE_ND_VERIFY=N: for the functions described in verifySpec (what a call writes), the first N
// calls run the lifted body, then the native body on the same input, and compare the bytes and the
// result registers; a difference is logged ("native decomp verify:") and the lifted result kept.
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
int g_verify = 0;
std::once_flag g_once;

void readEnv() {
    if (const char* e = std::getenv("FABLE_NATIVE_DECOMP")) g_all = std::atoi(e) != 0;
    if (const char* e = std::getenv("FABLE_ND_TIME")) g_time = std::atoi(e) != 0;
    if (const char* e = std::getenv("FABLE_ND_VERIFY")) g_verify = std::max(0, std::atoi(e));
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

// ---- FABLE_ND_VERIFY: what a described function's call writes (from FullDecomp's signatures)
struct VRange { uint32_t addr, size; int kind; };  // kind: 0 bytes, 1 float, 2 int16
struct VSpec { std::vector<VRange> ranges; bool eax = false; };
uint32_t sarg(const Ctx* c, int i) { return rd32(c->esp + 4 + 4 * i); }  // the i-th stack argument
bool verifySpec(uint32_t addr, const Ctx* c, VSpec& v) {
    switch (addr) {
        case 0xC5B030: {  // CSoftwareVoice::Resample(out, in, frames, channel), thiscall
            const uint32_t self = c->ecx, out = sarg(c, 0), in = sarg(c, 1), frames = sarg(c, 2), ch = sarg(c, 3);
            const uint32_t dst = rd32(out + 4) + rd32(out + 0x18) * rd32(out + 0xC) + rd32(out + 8) * ch * 2;
            v.ranges = {{dst, frames * 2, 2}, {self + 0x1EC, 4, 0}, {in, 0x20, 0}};
            return frames && frames < 0x100000;
        }
        case 0xC9C620: {  // mdct_backward(init: ecx, in: edx, out), fastcall
            const uint32_t n = rd32(c->ecx);
            v.ranges = {{sarg(c, 0), n * 4, 1}};
            return n && n <= 0x10000;
        }
        case 0xC96760: {  // vorbis_book_decodevv_add(book: ecx, a: edx, offset, ch, b, n), fastcall
            const uint32_t book = c->ecx, a = c->edx, offset = sarg(c, 0), ch = sarg(c, 1), b = sarg(c, 2), n = sarg(c, 3), dim = rd32(book);
            if (!ch || ch > 8 || dim > 64) return false;
            for (uint32_t k = 0; k < ch; ++k) v.ranges.push_back({rd32(a + 4 * k) + offset / ch * 4, ((offset + n) / ch - offset / ch + dim) * 4, 1});
            v.ranges.push_back({b, 20, 0});
            v.eax = true;
            return true;
        }
        case 0xC96320: {  // decode_packed_entry_number(book, b), callee pops
            v.ranges = {{sarg(c, 1), 20, 0}};
            v.eax = true;
            return true;
        }
        default: return false;
    }
}
bool hasVerifySpec(uint32_t addr) { return addr == 0xC5B030 || addr == 0xC9C620 || addr == 0xC96760 || addr == 0xC96320; }

struct VCount { uint32_t addr; std::atomic<int> calls{0}, bad{0}; };
VCount g_vcount[8];
VCount* vcount(uint32_t addr) {
    for (VCount& v : g_vcount) {
        uint32_t a = v.addr;
        if (a == addr) return &v;
        if (!a) { v.addr = addr; return &v; }  // filled from the game thread first; a race only shares a slot
    }
    return nullptr;
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
    if (g_verify && hasVerifySpec(addr)) {
        VCount* v = vcount(addr);
        if (v && v->calls.load(std::memory_order_relaxed) < g_verify) return 1 | 4;
    }
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

// Both bodies on the same input: the lifted one, the bytes it wrote and its registers kept; the
// state restored; the native one; the two compared.
void nd_verify(uint32_t addr, Ctx* c, void (*native)(Ctx*), void (*orig)(Ctx*)) {
    VSpec spec;
    VCount* vc = vcount(addr);
    if (!vc || !verifySpec(addr, c, spec)) {
        native(c);
        return;
    }
    const int n = vc->calls.fetch_add(1);
    std::vector<std::vector<uint8_t>> before, lifted;
    for (const VRange& r : spec.ranges) before.emplace_back(gp<uint8_t>(r.addr), gp<uint8_t>(r.addr) + r.size);
    const Ctx in = *c;
    // the stack arguments too: VC7.1 code reuses dead argument slots as locals
    uint8_t args[0x40];
    std::memcpy(args, gp<uint8_t>(c->esp), sizeof args);
    orig(c);
    const Ctx afterLifted = *c;
    uint8_t argsLifted[0x40];
    std::memcpy(argsLifted, gp<uint8_t>(in.esp), sizeof argsLifted);
    for (const VRange& r : spec.ranges) lifted.emplace_back(gp<uint8_t>(r.addr), gp<uint8_t>(r.addr) + r.size);
    for (size_t k = 0; k < spec.ranges.size(); ++k) std::memcpy(gp(spec.ranges[k].addr), before[k].data(), before[k].size());
    *c = in;
    std::memcpy(gp(in.esp), args, sizeof args);
    native(c);
    bool bad = false;
    char why[300] = "";
    for (size_t k = 0; k < spec.ranges.size() && !bad; ++k) {
        const VRange& r = spec.ranges[k];
        const uint8_t* now = gp<uint8_t>(r.addr);
        if (std::memcmp(now, lifted[k].data(), r.size) == 0) continue;
        bad = true;
        size_t diffs = 0, first = ~size_t{0};
        double maxd = 0;
        const size_t unit = r.kind == 1 ? 4 : r.kind == 2 ? 2 : 1;
        for (size_t i = 0; i + unit <= r.size; i += unit)
            if (std::memcmp(now + i, lifted[k].data() + i, unit)) {
                ++diffs;
                if (first == ~size_t{0}) first = i;
                double l = 0, h = 0;
                if (r.kind == 1) { float a, b; std::memcpy(&a, lifted[k].data() + i, 4), std::memcpy(&b, now + i, 4); l = a, h = b; }
                else if (r.kind == 2) { int16_t a, b; std::memcpy(&a, lifted[k].data() + i, 2), std::memcpy(&b, now + i, 2); l = a, h = b; }
                maxd = std::max(maxd, std::abs(l - h));
            }
        double l0 = 0, h0 = 0;
        if (r.kind == 1) { float a, b; std::memcpy(&a, lifted[k].data() + first, 4), std::memcpy(&b, now + first, 4); l0 = a, h0 = b; }
        else if (r.kind == 2) { int16_t a, b; std::memcpy(&a, lifted[k].data() + first, 2), std::memcpy(&b, now + first, 2); l0 = a, h0 = b; }
        else l0 = lifted[k][first], h0 = now[first];
        std::snprintf(why, sizeof why, "range %zu (0x%08X, %u bytes): %zu of %zu values differ, first at +0x%zX lifted %.9g native %.9g, max |diff| %.6g", k,
                      r.addr, r.size, diffs, r.size / unit, first, l0, h0, maxd);
    }
    if (!bad && (c->esp != afterLifted.esp || (spec.eax && c->eax != afterLifted.eax))) {
        bad = true;
        std::snprintf(why, sizeof why, "registers: esp %08X/%08X eax %08X/%08X (lifted/native)", afterLifted.esp, c->esp, afterLifted.eax, c->eax);
    }
    if (bad && addr == 0xC9C620 && std::getenv("FABLE_ND_DUMP") && vc->bad.load() == 0) {
        // the inputs and both outputs, for replaying offline: lookup (20 bytes), trig (n + n/4 floats), bitrev (n/4 ints),
        // in (the input as the call saw it: n floats from in), lifted out, native out (n floats each)
        const uint32_t L = in.ecx, nn = rd32(L), inp = in.edx, outp = rd32(in.esp + 4);
        if (FILE* f = std::fopen(std::getenv("FABLE_ND_DUMP"), "wb")) {
            const uint32_t hdr[4] = {nn, inp, outp, rd32(L + 4)};
            std::fwrite(hdr, 4, 4, f);
            std::fwrite(gp<uint8_t>(L), 1, 20, f);
            std::fwrite(gp<uint8_t>(rd32(L + 8)), 4, nn + nn / 4, f);
            std::fwrite(gp<uint8_t>(rd32(L + 12)), 4, nn / 4, f);
            // the input before either run: `before` holds the out range; when in lies inside it, take it from there
            std::vector<uint8_t> inBytes(nn * 4);
            if (inp >= outp && inp + nn * 4 <= outp + nn * 4 + 0) std::memcpy(inBytes.data(), before[0].data() + (inp - outp), std::min<size_t>(nn * 4, before[0].size() - (inp - outp)));
            else std::memcpy(inBytes.data(), gp<uint8_t>(inp), nn * 4);
            std::fwrite(inBytes.data(), 1, inBytes.size(), f);
            std::fwrite(lifted[0].data(), 1, lifted[0].size(), f);
            std::fwrite(gp<uint8_t>(spec.ranges[0].addr), 1, spec.ranges[0].size, f);
            std::fclose(f);
            host::log("native decomp verify: dumped mdct_backward inputs (n %u, in 0x%08X, out 0x%08X) to %s", nn, inp, outp, std::getenv("FABLE_ND_DUMP"));
        }
    }
    if (bad) {
        if (vc->bad.fetch_add(1) < 8) host::log("native decomp verify: 0x%08X call %d MISMATCH: %s", addr, n + 1, why);
        for (size_t k = 0; k < spec.ranges.size(); ++k) std::memcpy(gp(spec.ranges[k].addr), lifted[k].data(), lifted[k].size());
        std::memcpy(gp(in.esp), argsLifted, sizeof argsLifted);
        *c = afterLifted;  // the lifted result stands
    } else if (n + 1 == g_verify) {
        host::log("native decomp verify: 0x%08X matches the lifted code on %d calls (%d mismatched)", addr, g_verify, vc->bad.load());
    }
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
