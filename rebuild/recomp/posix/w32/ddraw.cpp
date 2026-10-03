// DirectDraw: only what ConfigDetect.dll asks for (video memory, display mode, device id).
#include <SDL.h>

#include "comobj.hpp"
#include <ddraw.h>

namespace w32 {
namespace {

struct DirectDraw final : ComObject<IDirectDraw7> {
    bool supports(REFIID iid) const override { return iidIn(iid, {&IID_IDirectDraw7}); }
#define NI(...) override { return DDERR_UNSUPPORTED; }
    HRESULT STDMETHODCALLTYPE Compact() override { return DD_OK; }
    HRESULT STDMETHODCALLTYPE CreateClipper(DWORD, LPDIRECTDRAWCLIPPER*, IUnknown*) NI()
    HRESULT STDMETHODCALLTYPE CreatePalette(DWORD, LPPALETTEENTRY, LPDIRECTDRAWPALETTE*, IUnknown*) NI()
    HRESULT STDMETHODCALLTYPE CreateSurface(LPDDSURFACEDESC2, LPDIRECTDRAWSURFACE7*, IUnknown*) NI()
    HRESULT STDMETHODCALLTYPE DuplicateSurface(LPDIRECTDRAWSURFACE7, LPDIRECTDRAWSURFACE7*) NI()
    HRESULT STDMETHODCALLTYPE EnumDisplayModes(DWORD, LPDDSURFACEDESC2, LPVOID, LPDDENUMMODESCALLBACK2) NI()
    HRESULT STDMETHODCALLTYPE EnumSurfaces(DWORD, LPDDSURFACEDESC2, LPVOID, LPDDENUMSURFACESCALLBACK7) NI()
    HRESULT STDMETHODCALLTYPE FlipToGDISurface() override { return DD_OK; }
    HRESULT STDMETHODCALLTYPE GetCaps(LPDDCAPS hw, LPDDCAPS) override {
        if (hw) {
            const DWORD size = hw->dwSize;
            std::memset(hw, 0, size);
            hw->dwSize = size;
            hw->dwCaps = DDCAPS_3D | DDCAPS_BLT | DDCAPS_BLTSTRETCH;
            hw->dwVidMemTotal = hw->dwVidMemFree = 0x40000000;
        }
        return DD_OK;
    }
    HRESULT STDMETHODCALLTYPE GetDisplayMode(LPDDSURFACEDESC2 d) override {
        SDL_DisplayMode m{0, 1920, 1080, 60, nullptr};
        if (SDL_WasInit(SDL_INIT_VIDEO)) SDL_GetDesktopDisplayMode(0, &m);
        const DWORD size = d->dwSize;
        std::memset(d, 0, size);
        d->dwSize = size;
        d->dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT | DDSD_REFRESHRATE | DDSD_PITCH;
        d->dwWidth = static_cast<DWORD>(m.w);
        d->dwHeight = static_cast<DWORD>(m.h);
        d->lPitch = m.w * 4;
        d->dwRefreshRate = static_cast<DWORD>(m.refresh_rate ? m.refresh_rate : 60);
        d->ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
        d->ddpfPixelFormat.dwFlags = DDPF_RGB;
        d->ddpfPixelFormat.dwRGBBitCount = 32;
        d->ddpfPixelFormat.dwRBitMask = 0xFF0000;
        d->ddpfPixelFormat.dwGBitMask = 0x00FF00;
        d->ddpfPixelFormat.dwBBitMask = 0x0000FF;
        return DD_OK;
    }
    HRESULT STDMETHODCALLTYPE GetFourCCCodes(LPDWORD n, LPDWORD) override { if (n) *n = 0; return DD_OK; }
    HRESULT STDMETHODCALLTYPE GetGDISurface(LPDIRECTDRAWSURFACE7*) NI()
    HRESULT STDMETHODCALLTYPE GetMonitorFrequency(LPDWORD f) override { if (f) *f = 60; return DD_OK; }
    HRESULT STDMETHODCALLTYPE GetScanLine(LPDWORD l) override { if (l) *l = 0; return DD_OK; }
    HRESULT STDMETHODCALLTYPE GetVerticalBlankStatus(LPBOOL b) override { if (b) *b = FALSE; return DD_OK; }
    HRESULT STDMETHODCALLTYPE Initialize(GUID*) override { return DDERR_ALREADYINITIALIZED; }
    HRESULT STDMETHODCALLTYPE RestoreDisplayMode() override { return DD_OK; }
    HRESULT STDMETHODCALLTYPE SetCooperativeLevel(HWND, DWORD) override { return DD_OK; }
    HRESULT STDMETHODCALLTYPE SetDisplayMode(DWORD, DWORD, DWORD, DWORD, DWORD) override { return DD_OK; }
    HRESULT STDMETHODCALLTYPE WaitForVerticalBlank(DWORD, HANDLE) override { return DD_OK; }
    HRESULT STDMETHODCALLTYPE GetAvailableVidMem(LPDDSCAPS2, LPDWORD total, LPDWORD free) override {
        if (total) *total = 0x40000000;  // 1 GiB, what the Windows host reports too
        if (free) *free = 0x40000000;
        return DD_OK;
    }
    HRESULT STDMETHODCALLTYPE GetSurfaceFromDC(HDC, LPDIRECTDRAWSURFACE7*) NI()
    HRESULT STDMETHODCALLTYPE RestoreAllSurfaces() override { return DD_OK; }
    HRESULT STDMETHODCALLTYPE TestCooperativeLevel() override { return DD_OK; }
    HRESULT STDMETHODCALLTYPE GetDeviceIdentifier(LPDDDEVICEIDENTIFIER2 id, DWORD) override {
        std::memset(id, 0, sizeof *id);
        std::strcpy(id->szDriver, "dxvk");
        std::strcpy(id->szDescription, "Vulkan (DXVK)");
        id->dwVendorId = 0x10DE;  // a well-known vendor keeps vendor checks on their default paths
        id->dwDeviceId = 0x1B80;
        return DD_OK;
    }
    HRESULT STDMETHODCALLTYPE StartModeTest(LPSIZE, DWORD, DWORD) NI()
    HRESULT STDMETHODCALLTYPE EvaluateMode(DWORD, DWORD*) NI()
#undef NI
};

}  // namespace
}  // namespace w32

using namespace w32;

extern "C" {
HRESULT WINAPI DirectDrawCreateEx(GUID*, LPVOID* out, REFIID iid, IUnknown*) {
    auto* dd = new DirectDraw;
    const HRESULT hr = dd->QueryInterface(iid, out);
    dd->Release();
    return hr;
}
HRESULT WINAPI DirectDrawEnumerateExA(LPDDENUMCALLBACKEXA cb, LPVOID ctx, DWORD) {
    char desc[] = "Primary Display Driver", name[] = "display";
    cb(nullptr, desc, name, ctx, nullptr);
    return DD_OK;
}
}
