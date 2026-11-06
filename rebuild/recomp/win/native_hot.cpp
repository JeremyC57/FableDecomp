// Native ports of hot lifted functions, from the byte-matched reconstruction (the FullDecomp branch,
// rebuild/src/compiled/): each one is the matched C++ rewritten against guest memory and wrapped over
// the lifted body (--wrap 0xADDR=host_native_...). Lifted code pays for every guest register in
// Ctx, the lazy flags and the guest stack; the native body keeps its values in host registers.
//
// Exactness: the game runs the x87 FPU at 24-bit precision once its D3D device exists (d3d9.cpp:
// CreateDevice without D3DCREATE_FPU_PRESERVE clears the precision bits), where every x87 operation
// rounds exactly like an IEEE single-precision one (the extended exponent aside: denormal and overflow
// results can differ). A port that does `float` arithmetic in the retail operation order (the order
// of the retail instructions, which VC7.1 does not always take from the source: it reorders sums)
// therefore gives the lifted code's bits. Each site runs only at 24-bit precision; at any other
// control word the lifted body runs.
//
// Verification, as audio_dsp.cpp's: the first FABLE_NATIVE_VERIFY calls (default 64) of each site run
// the lifted body, then the native body on the same input (the guest bytes the call writes and the
// registers restored between), and compare the bytes, the registers the callers can see and the x87
// result. A mismatch logs once and turns the site off for the session (the lifted result stands);
// a full match logs the per-call times of both. FABLE_NATIVE=0 turns every site off,
// FABLE_NATIVE_OFF=<hex address>[,...] single ones.
#include "host.hpp"

#include <atomic>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

using namespace host;

namespace {

enum : uint32_t {  // what a site's callers can see after the call
    kEax = 1u << 0,
    kAl = 1u << 1,  // a bool result: the low byte only (the rest of eax is left over)
    kEcx = 1u << 2,
    kEdx = 1u << 3,
    kSaved = 1u << 4,  // esp, ebx, ebp, esi, edi
    kSt0 = 1u << 5,    // a float or double result in st(0)
};

struct Site {
    const char* name;
    uint32_t addr;
    void (*orig)(Ctx*);
    uint32_t sees;
    std::atomic<int> calls{0};
    std::atomic<bool> failed{false};
    std::atomic<int64_t> origNs{0}, hostNs{0};
    int8_t enabled = -1;  // -1: not read yet
};

bool envOn(const char* name, bool dflt) {
    const char* e = std::getenv(name);
    return e ? std::atoi(e) != 0 : dflt;
}

bool siteEnabled(Site& s) {
    if (s.enabled < 0) {
        bool on = envOn("FABLE_NATIVE", true);
        if (const char* off = std::getenv("FABLE_NATIVE_OFF")) {
            for (const char* p = off; *p;) {
                char* end = nullptr;
                const unsigned long a = std::strtoul(p, &end, 16);
                if (end == p) break;
                if (a == s.addr) on = false;
                p = *end ? end + 1 : end;
            }
        }
        s.enabled = on ? 1 : 0;
        if (!on) host::log("native: %s (0x%08X) off: the lifted code runs", s.name, s.addr);
    }
    return s.enabled == 1;
}

int verifyCalls() {
    static const int n = [] { const char* e = std::getenv("FABLE_NATIVE_VERIFY"); return e ? std::max(0, std::atoi(e)) : 64; }();
    return n;
}

int64_t nsSince(std::chrono::steady_clock::time_point t) {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - t).count();
}

// 24-bit precision control (bits 8-9 of the control word 00): where float arithmetic is exact
bool singlePrecision(const Ctx* c) { return ((c->fpu.cw >> 8) & 3u) == 0; }

bool sameBits(double a, double b) { return std::memcmp(&a, &b, sizeof a) == 0; }

struct Range {  // the guest bytes a call writes (two spans at most)
    uint32_t lo[2] = {0, 0}, hi[2] = {0, 0};
    void add(uint32_t a, uint32_t n) {
        const int i = hi[0] ? 1 : 0;
        lo[i] = a, hi[i] = a + n;
    }
};

// What a site's callers can see, for the compare and for keeping the lifted result.
struct Seen {
    Ctx regs;
    std::vector<uint8_t> bytes[2];
};

Seen capture(const Ctx* c, const Range& r) {
    Seen s{*c, {}};
    for (int i = 0; i < 2; ++i)
        if (r.hi[i] > r.lo[i]) s.bytes[i].assign(gp<uint8_t>(r.lo[i]), gp<uint8_t>(r.hi[i]));
    return s;
}

void restore(Ctx* c, const Seen& s, const Range& r) {
    for (int i = 0; i < 2; ++i)
        if (!s.bytes[i].empty()) std::memcpy(gp(r.lo[i]), s.bytes[i].data(), s.bytes[i].size());
    *c = s.regs;
}

// "" when equal, else what differs
std::string differs(const Site& site, const Seen& want, const Ctx* c, const Range& r) {
    char buf[200];
    for (int i = 0; i < 2; ++i)
        for (size_t k = 0; k < want.bytes[i].size(); ++k)
            if (want.bytes[i][k] != gp<uint8_t>(r.lo[i])[k]) {
                const uint32_t at = r.lo[i] + static_cast<uint32_t>(k & ~size_t{3});
                uint32_t w = 0, h = 0;
                const size_t n = std::min<size_t>(4, want.bytes[i].size() - (k & ~size_t{3}));
                std::memcpy(&w, want.bytes[i].data() + (k & ~size_t{3}), n);
                std::memcpy(&h, gp<uint8_t>(at), n);
                std::snprintf(buf, sizeof buf, "memory 0x%08X: lifted 0x%08X native 0x%08X", at, w, h);
                return buf;
            }
    const Ctx& l = want.regs;
    const uint32_t m = site.sees;
    if ((m & kEax) && l.eax != c->eax) return std::snprintf(buf, sizeof buf, "eax: lifted %08X native %08X", l.eax, c->eax), buf;
    if ((m & kAl) && (l.eax & 0xFF) != (c->eax & 0xFF)) return std::snprintf(buf, sizeof buf, "al: lifted %02X native %02X", l.eax & 0xFF, c->eax & 0xFF), buf;
    if ((m & kEcx) && l.ecx != c->ecx) return std::snprintf(buf, sizeof buf, "ecx: lifted %08X native %08X", l.ecx, c->ecx), buf;
    if ((m & kEdx) && l.edx != c->edx) return std::snprintf(buf, sizeof buf, "edx: lifted %08X native %08X", l.edx, c->edx), buf;
    if ((m & kSaved) && (l.esp != c->esp || l.ebx != c->ebx || l.ebp != c->ebp || l.esi != c->esi || l.edi != c->edi))
        return std::snprintf(buf, sizeof buf, "saved registers: esp %08X/%08X ebx %08X/%08X ebp %08X/%08X esi %08X/%08X edi %08X/%08X", l.esp, c->esp, l.ebx,
                             c->ebx, l.ebp, c->ebp, l.esi, c->esi, l.edi, c->edi),
               buf;
    if (l.fpu.top != c->fpu.top) return std::snprintf(buf, sizeof buf, "x87 stack top: lifted %u native %u", l.fpu.top, c->fpu.top), buf;
    if ((m & kSt0) && !sameBits(l.fpu.st[l.fpu.top & 7u], c->fpu.st[c->fpu.top & 7u]))
        return std::snprintf(buf, sizeof buf, "st(0): lifted %.9g native %.9g", l.fpu.st[l.fpu.top & 7u], c->fpu.st[c->fpu.top & 7u]), buf;
    return {};
}

// The lifted body when the site is off, failed or not at 24-bit precision; both bodies (compared)
// for the first calls; then the native body alone. range(c, r) names the bytes the call writes
// (false: this call cannot be compared, so the lifted body runs it).
template <class RangeFn, class Native>
void run(Ctx* c, Site& s, RangeFn&& range, Native&& native) {
    if (!singlePrecision(c) || s.failed.load(std::memory_order_relaxed) || !siteEnabled(s)) {
        s.orig(c);
        return;
    }
    const int limit = verifyCalls();
    if (s.calls.load(std::memory_order_relaxed) >= limit) {
        native(c);
        return;
    }
    Range r;
    if (!range(c, r)) {
        s.orig(c);
        return;
    }
    const int n = s.calls.fetch_add(1);
    const Seen in = capture(c, r);
    auto t = std::chrono::steady_clock::now();
    s.orig(c);
    s.origNs += nsSince(t);
    const Seen want = capture(c, r);
    restore(c, in, r);
    t = std::chrono::steady_clock::now();
    native(c);
    s.hostNs += nsSince(t);
    const std::string bad = differs(s, want, c, r);
    if (!bad.empty()) {
        restore(c, want, r);  // the lifted result stands
        if (!s.failed.exchange(true))
            host::log("native: %s (0x%08X) MISMATCH on call %d, %s; the lifted code runs from now on", s.name, s.addr, n + 1, bad.c_str());
    } else if (n + 1 == limit) {
        host::log("native: %s (0x%08X) matches the lifted code on %d calls; lifted %.3f us/call, native %.3f us/call", s.name, s.addr, limit,
                  s.origNs.load() / 1e3 / limit, s.hostNs.load() / 1e3 / limit);
    }
}

}  // namespace

// ============================================================================================
// The sites. Each port follows the retail instructions (objdump of Fable.exe), not only the
// matched source: VC7.1 reorders sums, and the order decides the rounding.
// ============================================================================================
extern "C" void F_00A88B60_orig(Ctx* c);
extern "C" void F_00ADDD80_orig(Ctx* c);
extern "C" void F_00B9DC60_orig(Ctx* c);
extern "C" void F_009E1BC0_orig(Ctx* c);

namespace {

float f32(uint32_t a) { return rdf32(a); }
void setf32(uint32_t a, float v) { wrf32(a, v); }
uint32_t bitsOf(float v) { uint32_t u; std::memcpy(&u, &v, 4); return u; }

// ---- CQuaternion::operator*(const CQuaternion&) const, 0x00A88B60 ----------------------------
// rebuild/src/compiled/00/a8/CQuaternion_operator_00a88b60.cpp. thiscall, ret 8: [esp+4] the
// hidden result, [esp+8] q; returns the result in eax (and ecx; edx holds W's bits).
Site g_quatMul{"CQuaternion::operator*", 0xA88B60, F_00A88B60_orig, kEax | kEcx | kEdx | kSaved};

void quatMul(Ctx* c) {
    const uint32_t t = c->ecx, out = arg(c, 0), q = arg(c, 1);
    const float X = f32(t), Y = f32(t + 4), Z = f32(t + 8), W = f32(t + 12);
    const float qX = f32(q), qY = f32(q + 4), qZ = f32(q + 8), qW = f32(q + 12);
    const float rW = W * qW - qX * X - Y * qY - Z * qZ;
    const float rX = W * qX + X * qW + Y * qZ - Z * qY;
    const float rY = qY * W + Y * qW + Z * qX - qZ * X;
    const float rZ = qZ * W + qY * X + Z * qW - Y * qX;
    setf32(out, rX), setf32(out + 4, rY), setf32(out + 8, rZ), setf32(out + 12, rW);
    c->eax = c->ecx = out;
    c->edx = bitsOf(rW);
    c->esp += 12;
}

// ---- C3DBoundingSphere::AddToBounds(const C3DVector&, float), 0x00ADDD80 ---------------------
// rebuild/src/compiled/00/ad/C3DBoundingSphere_AddToBounds_00addd80.cpp. thiscall, ret 8. The
// division by the distance is a multiply by its reciprocal; the distance is stored over the
// point argument's slot (dead after the call, written here too).
Site g_sphereAdd{"C3DBoundingSphere::AddToBounds", 0xADDD80, F_00ADDD80_orig, kSaved};

void sphereAdd(Ctx* c) {
    const uint32_t s = c->ecx, p = arg(c, 0);
    const float radius = rdf32(c->esp + 8);
    const float R = f32(s + 12);
    const auto adopt = [&] {
        wr32(s, rd32(p)), wr32(s + 4, rd32(p + 4)), wr32(s + 8, rd32(p + 8));
        wr32(s + 12, rd32(c->esp + 8));
    };
    if (R == 0.0f) {
        adopt();
    } else if (radius != 0.0f) {
        const float o0 = f32(s) - f32(p), o1 = f32(s + 4) - f32(p + 4), o2 = f32(s + 8) - f32(p + 8);
        const float dist = std::sqrt(o2 * o2 + o0 * o0 + o1 * o1);
        setf32(c->esp + 4, dist);
        if (R < radius) {
            if (radius - R > dist) {
                adopt();
                c->esp += 12;
                return;
            }
        } else if (R - radius > dist) {
            c->esp += 12;
            return;
        }
        const float A = R - radius, inv = 1.0f / dist;
        const float s0 = o0 * A * inv, s1 = o1 * A * inv, s2 = o2 * A * inv;
        const float t0 = f32(p) + f32(s), t1 = f32(p + 4) + f32(s + 4), t2 = f32(s + 8) + f32(p + 8);
        const float half = 0.5f;
        setf32(s, (t0 + s0) * half);
        setf32(s + 4, (t1 + s1) * half);
        setf32(s + 8, (t2 + s2) * half);
        setf32(s + 12, (dist + radius + R) * half);
    }
    c->esp += 12;
}

// ---- CEngineInternalPrimitiveStaticMesh::IsInsideFrustum(const CEngineCamera&, bool), 0x00B9DC60
// rebuild/src/compiled/00/b9/CEngineInternalPrimitiveStaticMesh_IsInsideFrustum_00b9dc60.cpp, with
// its one callee, the fade distance 0x00BEB660, inline. thiscall, ret 8, the result in al. The
// sphere radius is stored over the bool argument's slot (written here too).
Site g_inFrustum{"CEngineInternalPrimitiveStaticMesh::IsInsideFrustum", 0xB9DC60, F_00B9DC60_orig, kAl | kSaved};

float fadeDistance(uint32_t prim) {  // 0x00BEB660
    const float f = f32(prim + 0x18);
    if (!(rd8(prim + 0x34) & 4)) return f;
    const uint32_t g = rd32(0x1436E78);
    const float t = f * f32(g + 0x7C), lo = f32(g + 0x80), hi = f32(g + 0x84);
    if (t < lo) return lo;
    return t > hi ? hi : t;
}

void inFrustum(Ctx* c) {
    const uint32_t m = c->ecx, cam = arg(c, 0), info = rd32(m + 0x50);
    const float cx = f32(info + 0x28), cy = f32(info + 0x2C), cz = f32(info + 0x30);
    const float X = cz * f32(m + 0x88) + cy * f32(m + 0x7C) + cx * f32(m + 0x70) + f32(m + 0x94);
    const float Y = cz * f32(m + 0x8C) + cy * f32(m + 0x80) + cx * f32(m + 0x74) + f32(m + 0x98);
    const float Z = cz * f32(m + 0x90) + cy * f32(m + 0x84) + cx * f32(m + 0x78) + f32(m + 0x9C);
    const float dX = X - f32(cam + 0x54), dY = Y - f32(cam + 0x58), dZ = Z - f32(cam + 0x5C);
    const float fade = fadeDistance(m);
    bool in = false;
    if (!(dZ * dZ + dY * dY + dX * dX > fade * fade)) {
        if (rd8(c->esp + 8)) {
            in = true;
        } else {
            const float r = f32(info + 0x34) * f32(m + 0x58);
            setf32(c->esp + 8, r);
            const auto dot = [&](uint32_t n) { return dZ * f32(n + 8) + dY * f32(n + 4) + dX * f32(n); };
            const float depth = dot(cam + 0x1B4);
            in = !(r + depth < f32(cam + 0x6C)) && !(depth - r > f32(cam + 0x70)) && !(dot(cam + 0x1E0) > r) && !(dot(cam + 0x1F0) > r) &&
                 !(dot(cam + 0x1C0) > r) && !(dot(cam + 0x1D0) > r);
        }
    }
    c->eax = (c->eax & ~0xFFu) | (in ? 1u : 0u);
    c->esp += 12;
}

// ---- GFGetTime(), 0x009E1BC0 --------------------------------------------------------------------
// rebuild/src/compiled/00/9e/global_GFGetTime_009e1bc0.cpp. __fastcall, no arguments, the seconds
// since InitialiseTimerBase in st(0): the host's QueryPerformanceCounter called directly instead of
// through the import trap. Until its one-time probe has run, and on the timeGetTime fallback, the
// lifted body runs. The result is a clock, so the check is that the native value is not earlier
// than the lifted one and less than 10 ms later.
Site g_getTime{"GFGetTime", 0x9E1BC0, F_009E1BC0_orig, kSt0 | kSaved};

bool getTimeNative(Ctx* c) {
    if (!rd8(0x13CAA10) || !rd8(0x13CAA00)) return false;
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    const int64_t start = static_cast<int64_t>(rd32(0x13CB2F0) | (uint64_t{rd32(0x13CB2F4)} << 32));
    const int64_t freq = static_cast<int64_t>(rd32(0x13CAA08) | (uint64_t{rd32(0x13CAA0C)} << 32));
    // fild, fild, fdivp at 24-bit precision: the quotient rounded once to single (a double quotient
    // rounded again is the same: 53 >= 2 * 24 + 2)
    fpush(c, static_cast<double>(static_cast<float>(static_cast<double>(now.QuadPart - start) / static_cast<double>(freq))));
    c->esp += 4;
    return true;
}

}  // namespace

extern "C" void host_native_quat_mul(Ctx* c) {
    run(c, g_quatMul, [](Ctx* c, Range& r) { r.add(arg(c, 0), 16); return true; }, quatMul);
}

extern "C" void host_native_sphere_add(Ctx* c) {
    run(c, g_sphereAdd, [](Ctx* c, Range& r) { r.add(c->ecx, 16); return true; }, sphereAdd);
}

extern "C" void host_native_inside_frustum(Ctx* c) {
    run(c, g_inFrustum, [](Ctx*, Range&) { return true; }, inFrustum);
}

extern "C" void host_native_gfgettime(Ctx* c) {
    Site& s = g_getTime;
    if (!singlePrecision(c) || s.failed.load(std::memory_order_relaxed) || !siteEnabled(s) || !rd8(0x13CAA10) || !rd8(0x13CAA00)) {
        s.orig(c);
        return;
    }
    if (s.calls.load(std::memory_order_relaxed) >= verifyCalls()) {
        getTimeNative(c);
        return;
    }
    const int n = s.calls.fetch_add(1);
    const Ctx in = *c;
    auto t = std::chrono::steady_clock::now();
    s.orig(c);
    s.origNs += nsSince(t);
    const Ctx lifted = *c;
    *c = in;
    t = std::chrono::steady_clock::now();
    getTimeNative(c);
    s.hostNs += nsSince(t);
    const double l = lifted.fpu.st[lifted.fpu.top & 7u], h = c->fpu.st[c->fpu.top & 7u];
    const bool ok = c->fpu.top == lifted.fpu.top && c->esp == lifted.esp && h >= l && h - l < 0.01;
    if (!ok) {
        *c = lifted;
        if (!s.failed.exchange(true))
            host::log("native: %s (0x%08X) MISMATCH on call %d: lifted %.9g native %.9g s; the lifted code runs from now on", s.name, s.addr, n + 1, l, h);
    } else if (n + 1 == verifyCalls()) {
        host::log("native: %s (0x%08X) agrees with the lifted code on %d calls; lifted %.3f us/call, native %.3f us/call", s.name, s.addr, n + 1,
                  s.origNs.load() / 1e3 / (n + 1), s.hostNs.load() / 1e3 / (n + 1));
    }
}
