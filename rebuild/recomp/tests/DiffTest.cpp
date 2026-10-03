// Differential test: recompiled C vs the retail code in Unicorn, from
// byte-identical memory and register states.
//
//   FABLE_EXE=<Fable.exe> recomp_difftest <funcs.tsv-like list: addr [retbytes]> [rounds]
//
// For every function: random heap (dense with valid heap pointers), random
// arguments, identical stack. Both sides run; EAX, EDX, ST0, stack, heap and
// the whole writable .data image must match byte-for-byte. Functions whose
// retail run leaves the pure-CPU boundary (imports, unmapped memory) are
// reported as SKIP, not failures.

#include "fable/oracle/RetailOracle.hpp"

extern "C" {
#include "recomp.h"
int recomp_init_memory(void);
GuestFn recomp_lookup(uint32_t target);
extern void (*recomp_on_fatal)(Ctx* c, uint32_t eip, const char* what);
}

#include <bit>
#include <csetjmp>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <sstream>
#include <vector>

using fable::oracle::CallingConvention;
using fable::oracle::OracleError;
using fable::oracle::RetailOracle;

namespace {

constexpr uint32_t kImageBase = 0x00400000, kImageEnd = 0x0146C000;  // whole SizeOfImage incl. .idata (IAT)
constexpr uint32_t kDataLo = 0x01374000, kDataHi = 0x0143F000;
constexpr uint32_t kHeap = 0x10000000, kHeapSize = 0x40000;
constexpr uint32_t kStackTop = 0x0F000000 + 0x00100000 - 0x1000;  // oracle's initial ESP region
constexpr uint32_t kStackLo = kStackTop - 0x10000, kStackHi = 0x0F100000;  // whole mapped stack
constexpr uint32_t kTeb = 0x7FFDE000, kSentinel = 0x7E00FFF0;

std::jmp_buf g_jump;
std::string g_fatal;

void onFatal(Ctx*, uint32_t eip, const char* what) {
    char b[160];
    std::snprintf(b, sizeof b, "%s at 0x%08X", what, eip);
    g_fatal = b;
    std::longjmp(g_jump, 1);
}

std::vector<uint8_t> retailBytes(RetailOracle& o, uint32_t a, uint32_t n) {
    std::vector<uint8_t> v(n);
    o.read(a, v.data(), n);
    return v;
}

bool sameFloatBytes(const uint8_t* a, const uint8_t* b, size_t n, size_t& at) {
    for (size_t i = 0; i < n; ++i) {
        if (a[i] == b[i]) continue;
        // Tolerate differing NaN payloads (x87 vs host) at float/double granularity.
        const size_t f = i & ~size_t{3};
        if (f + 4 <= n) {
            float x, y;
            std::memcpy(&x, a + f, 4);
            std::memcpy(&y, b + f, 4);
            if (std::isnan(x) && std::isnan(y)) continue;
        }
        const size_t d = i & ~size_t{7};
        if (d + 8 <= n) {
            double x, y;
            std::memcpy(&x, a + d, 8);
            std::memcpy(&y, b + d, 8);
            if (std::isnan(x) && std::isnan(y)) continue;
        }
        at = i;
        return false;
    }
    return true;
}

struct Outcome {
    int pass = 0;
    std::string fail, skip;
};

} // namespace

int main(int argc, char** argv) {
    const char* exe = std::getenv("FABLE_EXE");
    if (!exe || argc < 2) {
        std::cerr << "usage: FABLE_EXE=... recomp_difftest <list> [rounds]\n";
        return 2;
    }
    const int rounds = argc > 2 ? std::atoi(argv[2]) : 20;
    std::vector<std::pair<uint32_t, uint32_t>> funcs;  // addr, ret bytes (0 = cdecl)
    {
        std::ifstream in(argv[1]);
        std::string l;
        while (std::getline(in, l)) {
            std::istringstream ss(l);
            std::string a;
            uint32_t r = 0;
            if (!(ss >> a)) continue;
            ss >> r;
            funcs.emplace_back(static_cast<uint32_t>(std::stoul(a, nullptr, 16)), r);
        }
    }

    RetailOracle::Config cfg;
    cfg.stackSize = kStackHi - kStackLo;  // map exactly what is reset each run
    cfg.heapSize = kHeapSize;
    RetailOracle o(exe, cfg);
    o.setInstructionBudget(2'000'000);
    if (!recomp_init_memory()) {
        std::cerr << "cannot reserve guest memory\n";
        return 1;
    }
    recomp_on_fatal = onFatal;
    // Mirror the oracle's freshly loaded image and TEB into recompiled memory.
    o.read(kImageBase, g_mem + kImageBase, kImageEnd - kImageBase);
    o.read(kTeb, g_mem + kTeb, 0x2000);
    const std::vector<uint8_t> pristine(g_mem + kImageBase, g_mem + kImageEnd);
    const std::vector<uint8_t> teb(g_mem + kTeb, g_mem + kTeb + 0x2000);

    std::mt19937 rng(0xFAB1E);
    int nPass = 0, nFail = 0, nSkip = 0, nMissing = 0;
    std::map<std::string, int> skipReasons;
    for (auto [addr, retBytes] : funcs) {
        GuestFn fn = recomp_lookup(addr);
        if (!fn) {
            ++nMissing;
            continue;
        }
        const uint32_t nargs = retBytes ? retBytes / 4 : 4;
        const CallingConvention cc = retBytes ? CallingConvention::Stdcall : CallingConvention::Cdecl;
        Outcome out;
        for (int r = 0; r < rounds && out.fail.empty() && out.skip.empty(); ++r) {
            // Heap: dense with valid pointers into itself, plus plausible floats and ints.
            std::vector<uint8_t> heap(kHeapSize);
            for (uint32_t i = 0; i < kHeapSize; i += 4) {
                uint32_t v;
                switch (rng() % 4) {
                case 0: case 1: v = kHeap + (rng() % (kHeapSize / 2)) * 1 & ~3u; break;
                case 2: { float f = std::uniform_real_distribution<float>(-100.0f, 100.0f)(rng); v = std::bit_cast<uint32_t>(f); break; }
                default: v = rng() % 64; break;
                }
                std::memcpy(&heap[i], &v, 4);
            }
            std::vector<uint32_t> args(nargs);
            for (auto& a : args) a = (rng() % 4 != 0) ? kHeap + (rng() % (kHeapSize / 2) & ~3u) : rng() % 16;
            const uint32_t ecx = kHeap + (rng() % (kHeapSize / 2) & ~3u), edx = kHeap + (rng() % (kHeapSize / 2) & ~3u);

            // ---- retail ----
            o.reset();
            o.write(kTeb, teb.data(), teb.size());  // SEH chain head etc. must start identical
            o.write(kHeap, heap.data(), heap.size());
            std::vector<uint8_t> zeroStack(kStackHi - kStackLo, 0);
            o.write(kStackLo, zeroStack.data(), zeroStack.size());
            fable::oracle::CallResult rr;
            try {
                rr = o.call(addr, cc, args, ecx, edx);
            } catch (const OracleError& e) {
                out.skip = e.what();
                break;
            }
            if (!rr.importsCalled.empty()) {
                out.skip = "calls import " + rr.importsCalled.front();
                break;
            }

            // ---- recompiled ----
            std::memcpy(g_mem + kImageBase, pristine.data(), pristine.size());
            std::memcpy(g_mem + kHeap, heap.data(), heap.size());
            std::memcpy(g_mem + kTeb, teb.data(), teb.size());
            std::memset(g_mem + kStackLo, 0, kStackHi - kStackLo);
            Ctx c{};
            uint32_t esp = kStackTop;
            for (auto it = args.rbegin(); it != args.rend(); ++it) { esp -= 4; wr32(esp, *it); }
            esp -= 4;
            wr32(esp, kSentinel);
            c.esp = esp;
            c.ecx = ecx;
            c.edx = edx;
            c.fs_base = kTeb;
            c.fpu.cw = 0x027F;
            c.mxcsr = 0x1F80;
            if (setjmp(g_jump) == 0) {
                fn(&c);
            } else {
                out.fail = "recompiled code trapped: " + g_fatal;
                break;
            }

            // ---- compare ----
            const uint32_t expectedEsp = esp + 4 + (retBytes ? retBytes : 0);
            if (c.esp != expectedEsp) { out.fail = "stack pointer differs"; break; }
            if (c.eax != rr.eax) { char b[96]; std::snprintf(b, sizeof b, "eax recomp 0x%08X retail 0x%08X", c.eax, rr.eax); out.fail = b; break; }
            if (c.edx != rr.edx) { char b[96]; std::snprintf(b, sizeof b, "edx recomp 0x%08X retail 0x%08X", c.edx, rr.edx); out.fail = b; break; }
            const bool st0 = (c.fpu.top & 7u) == 7u;
            if (st0 != rr.st0Valid) { out.fail = "x87 stack depth differs"; break; }
            if (st0) {
                const double v = c.fpu.st[7];
                if (!(std::bit_cast<uint64_t>(v) == std::bit_cast<uint64_t>(rr.st0) || (std::isnan(v) && std::isnan(rr.st0)))) {
                    char b[128]; std::snprintf(b, sizeof b, "st0 recomp %.17g retail %.17g", v, rr.st0); out.fail = b; break;
                }
            }
            struct Region { const char* name; uint32_t lo, hi; } regions[] = {
                {"heap", kHeap, kHeap + kHeapSize}, {"stack", kStackLo, kStackHi}, {".data", kDataLo, kDataHi}};
            for (const auto& reg : regions) {
                const auto retail = retailBytes(o, reg.lo, reg.hi - reg.lo);
                size_t at = 0;
                if (!sameFloatBytes(g_mem + reg.lo, retail.data(), retail.size(), at)) {
                    char b[128];
                    std::snprintf(b, sizeof b, "%s differs at 0x%08X (recomp %02X retail %02X)", reg.name,
                                  reg.lo + static_cast<uint32_t>(at), g_mem[reg.lo + at], retail[at]);
                    out.fail = b;
                    break;
                }
            }
            if (out.fail.empty()) ++out.pass;
        }
        if (!out.fail.empty()) {
            ++nFail;
            std::printf("FAIL %08X %s\n", addr, out.fail.c_str());
        } else if (out.pass == 0) {
            ++nSkip;
            const auto key = out.skip.substr(0, out.skip.find(" at 0x") == std::string::npos ? out.skip.size() : out.skip.find(" at 0x"));
            skipReasons[key.substr(0, 60)]++;
        } else {
            ++nPass;
        }
    }
    std::printf("functions: %zu  pass: %d  fail: %d  skip: %d  not generated: %d\n", funcs.size(), nPass, nFail, nSkip, nMissing);
    std::vector<std::pair<int, std::string>> top;
    for (auto& [k, v] : skipReasons) top.emplace_back(v, k);
    std::sort(top.rbegin(), top.rend());
    for (size_t i = 0; i < top.size() && i < 12; ++i) std::printf("  skip %5d  %s\n", top[i].first, top[i].second.c_str());
    return nFail == 0 ? 0 : 1;
}
