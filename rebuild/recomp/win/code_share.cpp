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
#include <vector>

#ifdef _WIN32
#include <d3d9.h>
#include <timeapi.h>
#endif
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

#ifdef _WIN32
// The line over the frame, before Present: the overlay's 8x16 font in a managed texture, text with
// a one-pixel shadow, the game's state saved and restored by a state block. F9 toggles.
namespace {
#include "../posix/overlay/overlay_font.inc"
struct Vtx { float x, y, z, rhw; DWORD color; float u, v; };
IDirect3DDevice9* g_dev = nullptr;
IDirect3DTexture9* g_font = nullptr;
IDirect3DStateBlock9* g_state = nullptr;
constexpr int kCols = 16, kRows = 6, kTexW = kCols * 8, kTexH = kRows * 16;

bool makeFont(IDirect3DDevice9* d) {
    if (FAILED(d->CreateTexture(kTexW, kTexH, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &g_font, nullptr))) return false;
    D3DLOCKED_RECT r;
    if (FAILED(g_font->LockRect(0, &r, nullptr, 0))) return false;
    for (int y = 0; y < kTexH; ++y)
        for (int x = 0; x < kTexW; ++x) {
            const int g = (y / 16) * kCols + x / 8;
            const bool bit = g < 95 && (kOverlayFont[g][y % 16] & (0x80 >> (x % 8)));
            static_cast<DWORD*>(r.pBits)[y * (r.Pitch / 4) + x] = bit ? 0xFFFFFFFFu : 0x00FFFFFFu;
        }
    g_font->UnlockRect(0);
    return true;
}

void text(std::vector<Vtx>& v, const std::string& s, float x, float y, int scale, DWORD color) {
    for (char ch : s) {
        const int g = (ch < 32 || ch > 126) ? 0 : ch - 32;
        const float u0 = (g % kCols) * 8.0f / kTexW, v0 = (g / kCols) * 16.0f / kTexH, u1 = u0 + 8.0f / kTexW, v1 = v0 + 16.0f / kTexH;
        const float x0 = x - 0.5f, y0 = y - 0.5f, x1 = x0 + 8 * scale, y1 = y0 + 16 * scale;
        v.push_back({x0, y0, 0, 1, color, u0, v0}), v.push_back({x1, y0, 0, 1, color, u1, v0}), v.push_back({x0, y1, 0, 1, color, u0, v1});
        v.push_back({x1, y0, 0, 1, color, u1, v0}), v.push_back({x1, y1, 0, 1, color, u1, v1}), v.push_back({x0, y1, 0, 1, color, u0, v1});
        x += 8 * scale;
    }
}
}  // namespace

void drawD3D9(IDirect3DDevice9* d) {
    static bool prev = false;
    const bool key = (GetAsyncKeyState(VK_F9) & 0x8000) != 0;
    if (key && !prev) toggle();
    prev = key;
    if (!on()) return;
    if (d != g_dev) {  // a new device: the old one's objects went with it
        g_dev = d, g_font = nullptr, g_state = nullptr;
        if (!makeFont(d) || FAILED(d->CreateStateBlock(D3DSBT_ALL, &g_state))) {
            g_font = nullptr, g_state = nullptr;
            return;
        }
    }
    if (!g_font || !g_state) return;
    IDirect3DSurface9 *bb = nullptr, *rt = nullptr;
    if (FAILED(d->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &bb))) return;
    D3DSURFACE_DESC desc;
    bb->GetDesc(&desc);
    g_state->Capture();
    d->GetRenderTarget(0, &rt);
    d->SetRenderTarget(0, bb);
    D3DVIEWPORT9 vp{0, 0, desc.Width, desc.Height, 0.0f, 1.0f};
    d->SetViewport(&vp);
    d->SetVertexShader(nullptr), d->SetPixelShader(nullptr);
    d->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
    d->SetTexture(0, g_font), d->SetTexture(1, nullptr);
    d->SetRenderState(D3DRS_ZENABLE, FALSE), d->SetRenderState(D3DRS_ZWRITEENABLE, FALSE), d->SetRenderState(D3DRS_STENCILENABLE, FALSE);
    d->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE), d->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA), d->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    d->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE), d->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE), d->SetRenderState(D3DRS_LIGHTING, FALSE);
    d->SetRenderState(D3DRS_FOGENABLE, FALSE), d->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE), d->SetRenderState(D3DRS_COLORWRITEENABLE, 0xF);
    d->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);
    d->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE), d->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE), d->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    d->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE), d->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE), d->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    d->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0), d->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
    d->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE), d->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    d->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT), d->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT), d->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
    d->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP), d->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
    const int scale = desc.Height > 600 ? 2 : 1;
    std::vector<Vtx> v;
    const std::string s = line() + "   (F9)";
    text(v, s, 9, 9, scale, 0xC0000000);
    text(v, s, 8, 8, scale, 0xFF80FF80);
    if (SUCCEEDED(d->BeginScene())) {
        d->DrawPrimitiveUP(D3DPT_TRIANGLELIST, static_cast<UINT>(v.size() / 3), v.data(), sizeof(Vtx));
        d->EndScene();
    }
    d->SetRenderTarget(0, rt);
    g_state->Apply();
    if (rt) rt->Release();
    bb->Release();
}
#endif

}  // namespace host::codeshare

// called by the native-decomp objects' constructor (fable_lower)
extern "C" void nd_register_range(void* begin, void* end, uint32_t count) {
    host::codeshare::registerNative(reinterpret_cast<uintptr_t>(begin), reinterpret_cast<uintptr_t>(end), count);
}
