// UI scale: a larger HUD, menus and text at high resolutions.
//
// Fable lays out its 2D interface in screen pixels (HUD positions are authored for 640x480
// and anchored to the screen corners; sprites and glyphs are drawn at their texture size),
// so at 1920x1080 everything is small. Every part of the game asks the display manager for
// the render-target size through one getter (0x9BEDC0; C2DExtentsI at +0x194), which the
// lifter replaces with host_rtdims (--hook 0x9BEDC0=host_rtdims). With a UI scale s it
// reports the size divided by s, so the game lays out and renders a smaller screen; the
// Direct3D layer (d3d9.cpp) scales its back-buffer viewports and pixel rectangles back up,
// so the 3D scene still renders at the full resolution and the 2D layer is s times larger.
// New profiles still default to the real resolution.
#include "host.hpp"

#include <algorithm>
#include <cmath>

using namespace host;

namespace {

double g_uiScale = 1.0;

struct Range { uint32_t lo, hi; };
// The default resolution of a new profile (SetDefaultValues*) and the IME window: the real size.
// (ApplyVideoValues and ResetDimensions only refresh the menus' cached screen size at
// 0x13B876C, which must match what the rest of the 2D code sees.)
constexpr Range kRealSize[] = {
    {0x405650, 0x4057A0}, {0x4093E0, 0x409730}, {0x409B70, 0x409FD0}, {0x40C790, 0x40CCA0},
};

bool wantsRealSize(uint32_t ret) {
    for (const Range& r : kRealSize)
        if (ret >= r.lo && ret < r.hi) return true;
    return false;
}

}  // namespace

namespace host {

void uiScaleInit() {
    DWORD pct = 100, size = sizeof pct;
    if (RegGetValueW(HKEY_CURRENT_USER, L"Software\\FableRecomp", L"UIScale", RRF_RT_REG_DWORD, nullptr, &pct, &size) != ERROR_SUCCESS)
        pct = 100;
    int n = 0;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &n);
    for (int i = 1; i < n; ++i)
        if (_wcsnicmp(argv[i], L"--ui-scale=", 11) == 0) {
            pct = static_cast<DWORD>(_wtoi(argv[i] + 11));
            RegSetKeyValueW(HKEY_CURRENT_USER, L"Software\\FableRecomp", L"UIScale", REG_DWORD, &pct, sizeof pct);
        }
    LocalFree(argv);
    pct = std::clamp<DWORD>(pct, 100, 300);
    g_uiScale = pct / 100.0;
    log("UI scale %lu%% (--ui-scale=N to change)", static_cast<unsigned long>(pct));
}

double uiScale() { return g_uiScale; }
uint32_t uiSize(uint32_t real) { return static_cast<uint32_t>(std::max<long>(1, std::lround(real / g_uiScale))); }

}  // namespace host

// 0x9BEDC0: C2DExtentsI* __thiscall GetRenderTargetDimensions(C2DExtentsI* out)
extern "C" void host_rtdims(Ctx* c) {
    const uint32_t self = c->ecx, ret = rd32(c->esp), out = rd32(c->esp + 4);
    uint32_t w = rd32(self + 0x194), h = rd32(self + 0x198);
    if (g_uiScale != 1.0 && !wantsRealSize(ret)) w = uiSize(w), h = uiSize(h);
    wr32(out, w);
    wr32(out + 4, h);
    c->eax = out;
    c->esp += 8;
}
