// The guest's hottest audio loops as native host code.
//
// Two leaf routines of Fable's own sound code topped the RG34XX profile (device log 527c3e3b):
//
// 1. The software HRTF's FIR filters. Fable's statically linked software DirectSound mixer (run
//    on its own time-critical mixer thread) gives every "full HRTF" 3D voice one 25-tap int16
//    FIR per ear (object vtable 0x12BEDCC, Process 0xC70480), applied to 128-sample blocks by
//      0xC78A10  y[n] += (sum h*x) >> 14                        steady state: 2 calls per voice
//      0xC79440  y[n] += pmulhw(sat16((sum h*x) >> 15), g) * 4  filter change: the old filter
//                faded out and the new one in over the block, 4 calls per voice
//    with MMX pmaddwd (4 outputs per iteration, 32 iterations). The lifter expands every MMX
//    lane into scalar C through the context's mm registers and the guest stack, about 900
//    AArch64 instructions per 4 output samples; here it is about 65 (NEON), bit-identical.
//    Together the two were the hottest guest functions (2.9% of all process samples).
// 2. libvorbis 1.0.1's mdct_butterfly_generic (0xC9C2F0), the inner FFT pass of the Ogg music
//    decoders (one thread per music stream). x87 code, which the lifter models as IEEE double
//    with an explicit x87 register stack in the context; here the same double operations in
//    the same order (rounded to float after each one when the x87 control word asks for 24-bit
//    precision, as the lifted code does), so the result is bit-identical too.
//
// The lifter wraps each (--wrap 0xC78A10=host_hrtf_fir --wrap 0xC79440=host_hrtf_fir_ramp
// --wrap 0xC9C2F0=host_mdct_butterfly). The first FABLE_DSP_VERIFY calls of each (default 64)
// also run the lifted code on the same input and compare the memory it writes and the
// registers it returns; a difference logs the first one and goes back to the lifted code for
// good. FABLE_DSP_HOST=0 keeps the lifted code (A/B testing).
#include "host.hpp"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <vector>

#if defined(__aarch64__) || defined(_M_ARM64) || defined(__ARM_NEON)
#include <arm_neon.h>
#define HRTF_FIR_NEON 1
#endif

extern "C" void F_00C78A10_orig(Ctx* c);
extern "C" void F_00C79440_orig(Ctx* c);
extern "C" void F_00C9C2F0_orig(Ctx* c);

namespace {

// ---- kernels ------------------------------------------------------------------------------
// HRTF FIR. Arguments (cdecl): x (int16*, the block; iteration i reads x[4i-32 .. 4i-1]),
// coef (int16[4][32]: row r holds the 25 taps for output 4i+r, shifted by r and by the voice's
// interaural delay mod 4 = align), align (0..3), count (iterations), out (int32*, 4 per
// iteration) and, for the ramp, the starting gain g0 and its per-sample step d (lane r starts
// at g0 + r*d, all lanes step by 4d with a saturating add after each iteration). Row r uses
// coefficient quadwords 0..6 when r < align and 1..7 otherwise (the MMX code skips the other
// one): it is zeroed in the copy here, so the sums are those of the MMX code mod 2^32.
struct FirArgs {
    const int16_t* x;
    const int16_t* coef;
    uint32_t align, count;
    int32_t* out;
    bool ramp;
    int16_t gain[4], step;
};

inline int16_t sat16(int32_t v) { return static_cast<int16_t>(std::clamp<int32_t>(v, -32768, 32767)); }
inline bool usesQword(uint32_t row, uint32_t k, uint32_t align) { return row < align ? k <= 6 : k >= 1; }

#ifdef HRTF_FIR_NEON
// One output row: the coefficient quadword pairs times the input pairs, widened to int32.
template <int KMIN>
inline int32x4_t rowDot(const int16x8_t* c, int16x8_t x0, int16x8_t x1, int16x8_t x2, int16x8_t x3) {
    int32x4_t s = vmull_s16(vget_low_s16(c[0]), vget_low_s16(x0));
    s = vmlal_high_s16(s, c[0], x0);
    s = vmlal_s16(s, vget_low_s16(c[1]), vget_low_s16(x1));
    s = vmlal_high_s16(s, c[1], x1);
    s = vmlal_s16(s, vget_low_s16(c[2]), vget_low_s16(x2));
    s = vmlal_high_s16(s, c[2], x2);
    s = vmlal_s16(s, vget_low_s16(c[3]), vget_low_s16(x3));
    if (KMIN == 0) s = vmlal_high_s16(s, c[3], x3);
    return s;
}

// KMIN 1 (align 0): no row reads quadword 0, so neither is x[-32..-29] read (as the MMX code).
template <int KMIN, bool RAMP>
void firNeon(const FirArgs& a) {
    int16_t cz[4][32];
    for (uint32_t r = 0; r < 4; ++r)
        for (uint32_t k = 0; k < 8; ++k)
            for (uint32_t j = 0; j < 4; ++j) cz[r][4 * k + j] = usesQword(r, k, a.align) ? a.coef[32 * r + 4 * k + j] : 0;
    int16x8_t c[4][4];  // KMIN 0: quadwords (0,1) (2,3) (4,5) (6,7); KMIN 1: (1,2) (3,4) (5,6) (7,-)
    for (int r = 0; r < 4; ++r)
        for (int p = 0; p < 4; ++p)
            c[r][p] = KMIN == 0 || p < 3 ? vld1q_s16(&cz[r][4 * KMIN + 8 * p]) : vcombine_s16(vld1_s16(&cz[r][28]), vdup_n_s16(0));
    int16x4_t g = vld1_s16(a.gain);
    const int16x4_t step = vdup_n_s16(a.step);
    const int16_t* b = a.x - 32 + 4 * KMIN;
    int32_t* out = a.out;
    for (uint32_t i = 0, n = a.count; i < n; ++i, b += 4, out += 4) {
        const int16x8_t x0 = vld1q_s16(b), x1 = vld1q_s16(b + 8), x2 = vld1q_s16(b + 16);
        const int16x8_t x3 = KMIN == 0 ? vld1q_s16(b + 24) : vcombine_s16(vld1_s16(b + 24), vdup_n_s16(0));
        const int32x4_t s0 = rowDot<KMIN>(c[0], x0, x1, x2, x3), s1 = rowDot<KMIN>(c[1], x0, x1, x2, x3);
        const int32x4_t s2 = rowDot<KMIN>(c[2], x0, x1, x2, x3), s3 = rowDot<KMIN>(c[3], x0, x1, x2, x3);
        const int32x4_t sum = vpaddq_s32(vpaddq_s32(s0, s1), vpaddq_s32(s2, s3));  // rows 0..3
        int32x4_t y;
        if (!RAMP) {
            y = vshrq_n_s32(sum, 14);
        } else {
            const int16x4_t w = vqmovn_s32(vshrq_n_s32(sum, 15));      // packssdw
            const int16x4_t m = vshrn_n_s32(vmull_s16(w, g), 16);      // pmulhw
            y = vshll_n_s16(m, 2);                                     // (m << 16) >> 14
            g = vqadd_s16(g, step);                                    // paddsw
        }
        vst1q_s32(out, vaddq_s32(vld1q_s32(out), y));
    }
}
#endif

void firScalar(const FirArgs& a) {
    int16_t g[4] = {a.gain[0], a.gain[1], a.gain[2], a.gain[3]};
    for (uint32_t i = 0; i < a.count; ++i) {
        const int16_t* b = a.x - 32 + 4 * i;
        for (uint32_t r = 0; r < 4; ++r) {
            uint32_t s = 0;  // mod 2^32, as paddd
            for (uint32_t k = 0; k < 8; ++k)
                if (usesQword(r, k, a.align))
                    for (uint32_t j = 0; j < 4; ++j)
                        s += static_cast<uint32_t>(int32_t{a.coef[32 * r + 4 * k + j]} * int32_t{b[4 * k + j]});
            int32_t y;
            if (!a.ramp) {
                y = static_cast<int32_t>(s) >> 14;
            } else {
                const int16_t m = static_cast<int16_t>((int32_t{sat16(static_cast<int32_t>(s) >> 15)} * g[r]) >> 16);
                y = int32_t{m} * 4;
            }
            a.out[4 * i + r] = static_cast<int32_t>(static_cast<uint32_t>(a.out[4 * i + r]) + static_cast<uint32_t>(y));
        }
        for (int r = 0; r < 4; ++r) g[r] = sat16(int32_t{g[r]} + a.step);
    }
}

void fir(const FirArgs& a) {
    if (a.align > 3 || a.count == 0) return;
#ifdef HRTF_FIR_NEON
    if (a.align == 0) a.ramp ? firNeon<1, true>(a) : firNeon<1, false>(a);
    else a.ramp ? firNeon<0, true>(a) : firNeon<0, false>(a);
#else
    firScalar(a);
#endif
}

// mdct_butterfly_generic(float* T, float* x, int points, int trigint), MSVC register convention:
// ebx = x, ecx = points, edx = T, the stack argument trigint, ret 4. Byte addresses as the
// x86 code keeps them: eax walks x2 = x + points/2 - 8 down, ecx walks &x1[7] (x1 = x + points
// - 8), edx walks T by trigint floats per group. Every x87 operation is a double operation
// rounded by prec (the lifter's fprec), every fstp a float store, in the x86 code's order.
struct MdctRegs {
    uint32_t eax, ecx, edx;
};
template <bool SINGLE>
MdctRegs butterflyGeneric(uint32_t x, int32_t points, uint32_t t, uint32_t trigint) {
    const auto P = [](double v) { return SINGLE ? static_cast<double>(static_cast<float>(v)) : v; };
    const auto F = [](uint32_t a) { float f; std::memcpy(&f, reinterpret_cast<const void*>(static_cast<uintptr_t>(a)), 4); return static_cast<double>(f); };
    const auto S = [](uint32_t a, double v) { const float f = static_cast<float>(v); std::memcpy(reinterpret_cast<void*>(static_cast<uintptr_t>(a)), &f, 4); };
    const uint32_t step = trigint << 2;
    const int32_t half = points >> 1;
    uint32_t eax = x + static_cast<uint32_t>(half) * 4u - 0x20u;
    uint32_t ecx = eax + static_cast<uint32_t>(points - half) * 4u + 0x1Cu;
    uint32_t edx = t;
    // x2[a], x2[a+1] = the rotation of (r0, r1) by T[0], T[1]; T advances.
    const auto rotate = [&](double r0, double r1, uint32_t lo, uint32_t hi) {
        const double a = P(r1 * F(edx + 4)), b = P(r0 * F(edx));
        S(lo, P(a + b));
        const double c = P(r1 * F(edx)), d = P(r0 * F(edx + 4));
        edx += step;
        S(hi, P(c - d));
    };
    do {
        double r0 = P(F(ecx - 4) - F(eax + 0x18)), r1 = P(F(ecx) - F(eax + 0x1C));  // x1[6], x1[7]
        S(ecx - 4, P(F(ecx - 4) + F(eax + 0x18)));
        S(ecx, P(F(eax + 0x1C) + F(ecx)));
        rotate(r0, r1, eax + 0x18, eax + 0x1C);
        r0 = P(F(ecx - 0xC) - F(eax + 0x10)), r1 = P(F(ecx - 8) - F(eax + 0x14));  // x1[4], x1[5]
        S(ecx - 0xC, P(F(eax + 0x10) + F(ecx - 0xC)));
        S(ecx - 8, P(F(eax + 0x14) + F(ecx - 8)));
        rotate(r0, r1, eax + 0x10, eax + 0x14);
        r0 = P(F(ecx - 0x14) - F(eax + 8)), r1 = P(F(ecx - 0x10) - F(eax + 0xC));  // x1[2], x1[3]
        S(ecx - 0x14, P(F(eax + 8) + F(ecx - 0x14)));
        S(ecx - 0x10, P(F(eax + 0xC) + F(ecx - 0x10)));
        rotate(r0, r1, eax + 8, eax + 0xC);
        r0 = P(F(ecx - 0x1C) - F(eax)), r1 = P(F(ecx - 0x18) - F(eax + 4));  // x1[0], x1[1]
        S(ecx - 0x1C, P(F(eax) + F(ecx - 0x1C)));
        S(ecx - 0x18, P(F(ecx - 0x18) + F(eax + 4)));
        eax -= 0x20, ecx -= 0x20;
        rotate(r0, r1, eax + 0x20, eax + 0x24);
    } while (eax >= x);
    return {eax, ecx, edx};
}

// ---- host glue -----------------------------------------------------------------------------
using namespace host;

constexpr uint32_t kSecurityCookie = 0x139C8A8;  // /GS cookie: ecx when the FIRs return

struct Site {
    const char* name;
    void (*orig)(Ctx*);
    std::atomic<int> calls{0};
    std::atomic<bool> failed{false};
    std::atomic<int64_t> origNs{0}, hostNs{0};  // over the verified calls
};
Site g_fir{"HRTF FIR 0xC78A10", F_00C78A10_orig};
Site g_firRamp{"HRTF FIR ramp 0xC79440", F_00C79440_orig};
Site g_butterfly{"Vorbis mdct_butterfly_generic 0xC9C2F0", F_00C9C2F0_orig};

bool hostEnabled() {
    static const bool on = [] { const char* e = std::getenv("FABLE_DSP_HOST"); return !e || std::atoi(e) != 0; }();
    return on;
}
int verifyCalls() {
    static const int n = [] { const char* e = std::getenv("FABLE_DSP_VERIFY"); return e ? std::max(0, std::atoi(e)) : 64; }();
    return n;
}
int64_t nsSince(std::chrono::steady_clock::time_point t) {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - t).count();
}

// The lifted code, then the host code on the same input (guest bytes [lo, hi) and the registers
// restored in between); the bytes and the returned registers must match.
template <class Host>
void verify(Ctx* c, Site& s, int n, uint32_t lo, uint32_t hi, Host&& host) {
    std::vector<uint8_t> before(gp<uint8_t>(lo), gp<uint8_t>(hi));
    const Ctx in = *c;
    auto t = std::chrono::steady_clock::now();
    s.orig(c);
    s.origNs += nsSince(t);
    const Ctx lifted = *c;
    std::vector<uint8_t> want(gp<uint8_t>(lo), gp<uint8_t>(hi));
    std::memcpy(gp(lo), before.data(), before.size());
    c->eax = in.eax, c->ecx = in.ecx, c->edx = in.edx, c->ebx = in.ebx, c->esp = in.esp, c->ebp = in.ebp, c->esi = in.esi, c->edi = in.edi;
    t = std::chrono::steady_clock::now();
    host(c);
    s.hostNs += nsSince(t);
    size_t bad = want.size();
    for (size_t i = 0; i < want.size(); ++i)
        if (want[i] != gp<uint8_t>(lo)[i]) { bad = i; break; }
    const bool regsOk = c->eax == lifted.eax && c->ecx == lifted.ecx && c->edx == lifted.edx && c->ebx == lifted.ebx && c->esp == lifted.esp &&
                        c->ebp == lifted.ebp && c->esi == lifted.esi && c->edi == lifted.edi;
    if (bad < want.size() || !regsOk) {
        if (bad < want.size()) {
            const uint32_t at = static_cast<uint32_t>(bad & ~size_t{3});
            uint32_t w = 0, h = 0;
            std::memcpy(&w, want.data() + at, std::min<size_t>(4, want.size() - at));
            std::memcpy(&h, gp<uint8_t>(lo) + at, std::min<size_t>(4, want.size() - at));
            host::log("audio: %s host code MISMATCH on call %d at 0x%08X+0x%X: lifted 0x%08X host 0x%08X; using the lifted code", s.name, n + 1, lo, at, w, h);
        } else {
            host::log("audio: %s host registers differ on call %d (eax %08X/%08X ecx %08X/%08X edx %08X/%08X esp %08X/%08X); using the lifted code",
                      s.name, n + 1, lifted.eax, c->eax, lifted.ecx, c->ecx, lifted.edx, c->edx, lifted.esp, c->esp);
        }
        std::memcpy(gp(lo), want.data(), want.size());  // the lifted result stands
        c->eax = lifted.eax, c->ecx = lifted.ecx, c->edx = lifted.edx, c->ebx = lifted.ebx, c->esp = lifted.esp, c->ebp = lifted.ebp,
        c->esi = lifted.esi, c->edi = lifted.edi;
        s.failed = true;
    } else if (n + 1 == verifyCalls()) {
        host::log("audio: %s host code matches the lifted code on %d calls; lifted %.1f us/call, host %.1f us/call", s.name, n + 1, s.origNs.load() / 1e3 / (n + 1),
                  s.hostNs.load() / 1e3 / (n + 1));
    }
}

// The lifted code when disabled or after a mismatch, both (compared) for the first calls, then the
// host code alone. range(c, lo, hi) gives the guest bytes the call writes.
template <class Range, class Host>
void dispatch(Ctx* c, Site& s, Range&& range, Host&& host) {
    if (!hostEnabled() || s.failed.load(std::memory_order_relaxed)) {
        s.orig(c);
        return;
    }
    const int limit = verifyCalls();
    if (s.calls.load(std::memory_order_relaxed) < limit) {
        const int n = s.calls.fetch_add(1);
        uint32_t lo = 0, hi = 0;
        if (n < limit && range(c, lo, hi)) {
            verify(c, s, n, lo, hi, host);
            return;
        }
    }
    host(c);
}

// ---- HRTF FIR ----
FirArgs firArgs(Ctx* c, bool ramp) {
    FirArgs a{};
    a.x = gp<const int16_t>(arg(c, 0));
    a.coef = gp<const int16_t>(arg(c, 1));
    a.align = arg(c, 2);
    a.count = arg(c, 3);
    a.out = gp<int32_t>(arg(c, 4));
    a.ramp = ramp;
    if (ramp) {
        const uint32_t g0 = arg(c, 5), d = arg(c, 6);
        for (uint32_t r = 0; r < 4; ++r) a.gain[r] = static_cast<int16_t>(g0 + r * d);
        a.step = static_cast<int16_t>(d << 2);
    }
    return a;
}
// The filter, then the registers the MMX code returns with (input and output cursors, the /GS
// cookie in ecx) and ret.
void firHost(Ctx* c, bool ramp) {
    const FirArgs a = firArgs(c, ramp);
    fir(a);
    uint32_t eax = arg(c, 0) - 0x40, edx = arg(c, 4);
    if (a.align <= 3 && a.count) eax += (a.align == 0 ? 0x30 : 0) + 8 * a.count, edx += 16 * a.count;
    c->eax = eax;
    c->ecx = rd32(kSecurityCookie);
    c->edx = edx;
    c->esp += 4;
}
bool firRange(Ctx* c, uint32_t& lo, uint32_t& hi) {
    const uint32_t align = arg(c, 2), count = arg(c, 3);
    lo = hi = arg(c, 4);
    if (align <= 3 && count) hi += 16 * count;
    return count <= 0x10000;
}

// ---- Vorbis mdct_butterfly_generic ----
void butterflyHost(Ctx* c) {
    const bool single = ((c->fpu.cw >> 8) & 3u) == 0;  // x87 precision control: 24-bit
    const uint32_t trigint = arg(c, 0);
    const MdctRegs r = single ? butterflyGeneric<true>(c->ebx, static_cast<int32_t>(c->ecx), c->edx, trigint)
                              : butterflyGeneric<false>(c->ebx, static_cast<int32_t>(c->ecx), c->edx, trigint);
    c->eax = r.eax;
    c->ecx = r.ecx;
    c->edx = r.edx;
    c->esp += 8;  // ret 4
}
bool butterflyRange(Ctx* c, uint32_t& lo, uint32_t& hi) {
    const uint32_t x = c->ebx;
    const int32_t points = static_cast<int32_t>(c->ecx), half = points >> 1;
    const uint32_t eax0 = x + static_cast<uint32_t>(half) * 4u - 0x20u, ecx0 = eax0 + static_cast<uint32_t>(points - half) * 4u + 0x1Cu;
    if (points < 16 || points > 0x10000 || eax0 < x) return false;  // not a Vorbis block: no comparison
    const uint32_t back = (eax0 - x) / 0x20 * 0x20;  // the last iteration's x2 is eax0 - back
    lo = std::min(eax0 - back, ecx0 - 0x1C - back);
    hi = std::max(eax0 + 0x20, ecx0 + 4);
    return true;
}

}  // namespace

// 0xC78A10 void __cdecl (const short* x, const short* coef, unsigned align, unsigned count, int* out)
extern "C" void host_hrtf_fir(Ctx* c) { dispatch(c, g_fir, firRange, [](Ctx* c) { firHost(c, false); }); }
// 0xC79440 void __cdecl (x, coef, align, count, out, int g0, int d): the crossfade (gain ramp) form
extern "C" void host_hrtf_fir_ramp(Ctx* c) { dispatch(c, g_firRamp, firRange, [](Ctx* c) { firHost(c, true); }); }
// 0xC9C2F0 libvorbis mdct_butterfly_generic (ebx = x, ecx = points, edx = T, stack trigint; ret 4)
extern "C" void host_mdct_butterfly(Ctx* c) { dispatch(c, g_butterfly, butterflyRange, butterflyHost); }
