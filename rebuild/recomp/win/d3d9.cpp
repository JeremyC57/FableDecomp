// Direct3D 9 and D3DX: the real 64-bit runtime behind guest proxies.
// Hand-written methods: device creation/reset (D3DPRESENT_PARAMETERS has an HWND),
// and every Lock (host pointers are copied through low guest staging buffers).
#include "com.hpp"

#include <d3d9.h>
#include <d3dx9.h>

#include <cmath>
#include <map>
#include <mutex>

using namespace host;
using host::com::unwrap;
using host::com::wrap;

namespace {

// ---------------------------------------------------------------------------
// D3DPRESENT_PARAMETERS (x86: 14 dwords, hDeviceWindow at +28)
// ---------------------------------------------------------------------------
D3DPRESENT_PARAMETERS ppIn(uint32_t g) {
    D3DPRESENT_PARAMETERS p{};
    p.BackBufferWidth = rd32(g + 0);
    p.BackBufferHeight = rd32(g + 4);
    p.BackBufferFormat = static_cast<D3DFORMAT>(rd32(g + 8));
    p.BackBufferCount = rd32(g + 12);
    p.MultiSampleType = static_cast<D3DMULTISAMPLE_TYPE>(rd32(g + 16));
    p.MultiSampleQuality = rd32(g + 20);
    p.SwapEffect = static_cast<D3DSWAPEFFECT>(rd32(g + 24));
    p.hDeviceWindow = static_cast<HWND>(hh(rd32(g + 28)));
    p.Windowed = rd32(g + 32);
    p.EnableAutoDepthStencil = rd32(g + 36);
    p.AutoDepthStencilFormat = static_cast<D3DFORMAT>(rd32(g + 40));
    p.Flags = rd32(g + 44);
    p.FullScreen_RefreshRateInHz = rd32(g + 48);
    p.PresentationInterval = rd32(g + 52);
    return p;
}
void ppOut(uint32_t g, const D3DPRESENT_PARAMETERS& p) {
    wr32(g + 0, p.BackBufferWidth);
    wr32(g + 4, p.BackBufferHeight);
    wr32(g + 8, p.BackBufferFormat);
    wr32(g + 12, p.BackBufferCount);
    wr32(g + 16, p.MultiSampleType);
    wr32(g + 20, p.MultiSampleQuality);
    wr32(g + 24, p.SwapEffect);
    wr32(g + 28, gh(p.hDeviceWindow));
    wr32(g + 32, p.Windowed);
    wr32(g + 36, p.EnableAutoDepthStencil);
    wr32(g + 40, p.AutoDepthStencilFormat);
    wr32(g + 44, p.Flags);
    wr32(g + 48, p.FullScreen_RefreshRateInHz);
    wr32(g + 52, p.PresentationInterval);
}
void logPP(const char* what, const D3DPRESENT_PARAMETERS& p, HRESULT hr) {
    log("%s: %ux%u fmt %d count %u ms %d swap %d windowed %d depth %d/%d flags 0x%X rate %u interval 0x%X -> 0x%08lX", what,
        p.BackBufferWidth, p.BackBufferHeight, p.BackBufferFormat, p.BackBufferCount, p.MultiSampleType, p.SwapEffect, p.Windowed,
        p.EnableAutoDepthStencil, p.AutoDepthStencilFormat, p.Flags, p.FullScreen_RefreshRateInHz, p.PresentationInterval,
        static_cast<unsigned long>(hr));
}

// Direct3D switches the calling thread's x87 unit to 24-bit precision unless
// D3DCREATE_FPU_PRESERVE is given; the retail game ran that way, so mirror it.
void applyFpuMode(Ctx* c, DWORD flags) {
    if (!(flags & D3DCREATE_FPU_PRESERVE)) c->fpu.cw = static_cast<uint16_t>(c->fpu.cw & ~0x0300u);
}

// ---------------------------------------------------------------------------
// staging buffers for Lock
// ---------------------------------------------------------------------------
struct Staging {
    uint32_t guest = 0, capacity = 0;
    void* host = nullptr;
    uint32_t size = 0;
    DWORD flags = 0;
};
std::mutex g_stLock;
std::map<std::pair<IUnknown*, uint32_t>, Staging> g_staging;

void freeStaging(IUnknown* res) {
    std::lock_guard<std::mutex> l(g_stLock);
    for (auto it = g_staging.lower_bound({res, 0}); it != g_staging.end() && it->first.first == res;) {
        gfree(it->second.guest);
        it = g_staging.erase(it);
    }
}

uint32_t stageIn(IUnknown* res, uint32_t sub, void* hostPtr, uint32_t size, DWORD flags) {
    com::onRelease(res, freeStaging);
    std::lock_guard<std::mutex> l(g_stLock);
    Staging& s = g_staging[{res, sub}];
    if (s.capacity < size) {
        if (s.guest) gfree(s.guest);
        s.capacity = (size + 0xFFF) & ~0xFFFu;
        s.guest = gmalloc(s.capacity);
    }
    s.host = hostPtr;
    s.size = size;
    s.flags = flags;
    if (!(flags & D3DLOCK_DISCARD) && size) std::memcpy(gp(s.guest), hostPtr, size);
    return s.guest;
}
void stageOut(IUnknown* res, uint32_t sub) {
    std::lock_guard<std::mutex> l(g_stLock);
    auto it = g_staging.find({res, sub});
    if (it == g_staging.end() || !it->second.host) return;
    Staging& s = it->second;
    if (!(s.flags & D3DLOCK_READONLY) && s.size) std::memcpy(s.host, gp(s.guest), s.size);
    s.host = nullptr;
}

struct FmtInfo { uint32_t bw, bh, bytes; };
FmtInfo fmtInfo(D3DFORMAT f) {
    switch (f) {
    case D3DFMT_DXT1: return {4, 4, 8};
    case D3DFMT_DXT2: case D3DFMT_DXT3: case D3DFMT_DXT4: case D3DFMT_DXT5: return {4, 4, 16};
    case D3DFMT_R8G8B8: return {1, 1, 3};
    case D3DFMT_R5G6B5: case D3DFMT_X1R5G5B5: case D3DFMT_A1R5G5B5: case D3DFMT_A4R4G4B4: case D3DFMT_X4R4G4B4: case D3DFMT_A8R3G3B2:
    case D3DFMT_L16: case D3DFMT_A8L8: case D3DFMT_A8P8: case D3DFMT_D16: case D3DFMT_D16_LOCKABLE: case D3DFMT_D15S1: case D3DFMT_R16F:
    case D3DFMT_V8U8: case D3DFMT_L6V5U5: case D3DFMT_INDEX16:
        return {1, 1, 2};
    case D3DFMT_A8: case D3DFMT_L8: case D3DFMT_P8: case D3DFMT_A4L4: case D3DFMT_R3G3B2: return {1, 1, 1};
    case D3DFMT_A16B16G16R16: case D3DFMT_A16B16G16R16F: case D3DFMT_G32R32F: case D3DFMT_Q16W16V16U16: return {1, 1, 8};
    case D3DFMT_A32B32G32R32F: return {1, 1, 16};
    default: return {1, 1, 4};
    }
}
uint32_t rectBytes(D3DFORMAT f, uint32_t w, uint32_t h, uint32_t pitch) {
    const FmtInfo fi = fmtInfo(f);
    const uint32_t rows = (h + fi.bh - 1) / fi.bh;
    const uint32_t rowBytes = (w + fi.bw - 1) / fi.bw * fi.bytes;
    return rows ? pitch * (rows - 1) + rowBytes : 0;
}
void rectSize(const RECT* r, const D3DSURFACE_DESC& d, uint32_t& w, uint32_t& h) {
    if (r) { w = r->right - r->left; h = r->bottom - r->top; }
    else { w = d.Width; h = d.Height; }
}
const RECT* guestRect(uint32_t g) { return g ? gp<RECT>(g) : nullptr; }
const D3DBOX* guestBox(uint32_t g) { return g ? gp<D3DBOX>(g) : nullptr; }

void finishLockRect(Ctx* c, IUnknown* res, uint32_t sub, HRESULT hr, const D3DLOCKED_RECT& lr, const D3DSURFACE_DESC& d, const RECT* r,
                    DWORD flags, uint32_t guestLr, int nargs) {
    if (SUCCEEDED(hr)) {
        uint32_t w, h;
        rectSize(r, d, w, h);
        const uint32_t size = rectBytes(d.Format, w, h, static_cast<uint32_t>(lr.Pitch));
        const uint32_t g = stageIn(res, sub, lr.pBits, size, flags);
        wr32(guestLr, static_cast<uint32_t>(lr.Pitch));
        wr32(guestLr + 4, g);
    }
    retStd(c, static_cast<uint32_t>(hr), nargs);
}

void finishLockBox(Ctx* c, IUnknown* res, uint32_t sub, HRESULT hr, const D3DLOCKED_BOX& lb, const D3DVOLUME_DESC& d, const D3DBOX* b,
                   DWORD flags, uint32_t guestLb, int nargs) {
    if (SUCCEEDED(hr)) {
        const uint32_t w = b ? b->Right - b->Left : d.Width, h = b ? b->Bottom - b->Top : d.Height, dep = b ? b->Back - b->Front : d.Depth;
        const uint32_t slice = rectBytes(d.Format, w, h, static_cast<uint32_t>(lb.RowPitch));
        const uint32_t size = dep ? static_cast<uint32_t>(lb.SlicePitch) * (dep - 1) + slice : 0;
        const uint32_t g = stageIn(res, sub, lb.pBits, size, flags);
        wr32(guestLb, static_cast<uint32_t>(lb.RowPitch));
        wr32(guestLb + 4, static_cast<uint32_t>(lb.SlicePitch));
        wr32(guestLb + 8, g);
    }
    retStd(c, static_cast<uint32_t>(hr), nargs);
}

void getContainer(Ctx* c, IUnknown* self, HRESULT (*fn)(IUnknown*, REFIID, void**)) {
    const GUID& iid = *gp<GUID>(arg(c, 1));
    void* p = nullptr;
    const HRESULT hr = fn(self, iid, &p);
    com::Class* cls = com::classForIid(iid);
    if (arg(c, 2)) wr32(arg(c, 2), SUCCEEDED(hr) && cls ? com::wrapRaw(static_cast<IUnknown*>(p), *cls) : 0);
    retStd(c, static_cast<uint32_t>(hr), 3);
}

// ---------------------------------------------------------------------------
// D3DX: math done natively, texture helpers from whichever x64 d3dx9 is installed
// ---------------------------------------------------------------------------
HMODULE g_d3dx;
HMODULE d3dx() {
    static bool tried = false;
    if (!tried) {
        tried = true;
        for (int v = 43; v >= 24 && !g_d3dx; --v) {
            char n[32];
            std::snprintf(n, sizeof n, "d3dx9_%d.dll", v);
            g_d3dx = LoadLibraryA(n);
            if (g_d3dx) log("using %s for D3DX texture helpers", n);
        }
        if (!g_d3dx) log("no 64-bit d3dx9_xx.dll found: install the DirectX End-User Runtime (June 2010)");
    }
    return g_d3dx;
}

template <class Sig> struct DynCall;
template <class R, class... A> struct DynCall<R(A...)> {
    static void call(Ctx* c, const char* name) {
        HMODULE m = d3dx();
        void* fn = m ? reinterpret_cast<void*>(GetProcAddress(m, name)) : nullptr;
        if (!fn) die("%s is not available (no 64-bit D3DX runtime)", name);
        using P = R(WINAPI*)(A...);
        const uint32_t used = detail::invoke<R, A...>(c, 0, [fn](auto&&... a) -> R { return reinterpret_cast<P>(fn)(std::forward<decltype(a)>(a)...); });
        c->esp += 4 + used;
    }
};
#define D3DX_DYN(name) IMPORT("d3dx9_25.dll", name) { DynCall<decltype(::name)>::call(c, #name); }

}  // namespace

// ---------------------------------------------------------------------------
// overrides referenced from com_vtables.inc
// ---------------------------------------------------------------------------
void ovr_IDirect3D9_CreateDevice(Ctx* c) {
    auto* self = unwrap<IDirect3D9>(arg(c, 0));
    D3DPRESENT_PARAMETERS pp = ppIn(arg(c, 5));
    const DWORD flags = arg(c, 4);
    IDirect3DDevice9* dev = nullptr;
    const HRESULT hr = self->CreateDevice(arg(c, 1), static_cast<D3DDEVTYPE>(arg(c, 2)), static_cast<HWND>(hh(arg(c, 3))), flags, &pp, &dev);
    logPP("CreateDevice", pp, hr);
    ppOut(arg(c, 5), pp);
    if (arg(c, 6)) wr32(arg(c, 6), SUCCEEDED(hr) ? wrap(dev) : 0);
    if (SUCCEEDED(hr)) applyFpuMode(c, flags);
    retStd(c, static_cast<uint32_t>(hr), 7);
}
// Present(this, srcRect, dstRect, hwnd, dirtyRegion); also logs the frame rate every 5 s.
void ovr_IDirect3DDevice9_Present(Ctx* c) {
    auto* self = unwrap<IDirect3DDevice9>(arg(c, 0));
    const HRESULT hr = self->Present(arg(c, 1) ? gp<RECT>(arg(c, 1)) : nullptr, arg(c, 2) ? gp<RECT>(arg(c, 2)) : nullptr,
                                     static_cast<HWND>(hh(arg(c, 3))), arg(c, 4) ? gp<RGNDATA>(arg(c, 4)) : nullptr);
    static uint32_t frames = 0;
    static DWORD since = GetTickCount();
    ++frames;
    if (const DWORD now = GetTickCount(); now - since >= 5000) {
        HLOG(1, "Present: %.1f fps", frames * 1000.0 / (now - since));
        frames = 0, since = now;
    }
    retStd(c, static_cast<uint32_t>(hr), 5);
}

void ovr_IDirect3DDevice9_Reset(Ctx* c) {
    auto* self = unwrap<IDirect3DDevice9>(arg(c, 0));
    D3DPRESENT_PARAMETERS pp = ppIn(arg(c, 1));
    const HRESULT hr = self->Reset(&pp);
    logPP("Reset", pp, hr);
    ppOut(arg(c, 1), pp);
    retStd(c, static_cast<uint32_t>(hr), 2);
}
void ovr_IDirect3DDevice9_CreateAdditionalSwapChain(Ctx* c) {
    auto* self = unwrap<IDirect3DDevice9>(arg(c, 0));
    D3DPRESENT_PARAMETERS pp = ppIn(arg(c, 1));
    IDirect3DSwapChain9* sc = nullptr;
    const HRESULT hr = self->CreateAdditionalSwapChain(&pp, &sc);
    ppOut(arg(c, 1), pp);
    if (arg(c, 2)) wr32(arg(c, 2), SUCCEEDED(hr) ? wrap(sc) : 0);
    retStd(c, static_cast<uint32_t>(hr), 3);
}
void ovr_IDirect3DDevice9_GetCreationParameters(Ctx* c) {
    auto* self = unwrap<IDirect3DDevice9>(arg(c, 0));
    D3DDEVICE_CREATION_PARAMETERS p{};
    const HRESULT hr = self->GetCreationParameters(&p);
    const uint32_t g = arg(c, 1);
    wr32(g, p.AdapterOrdinal);
    wr32(g + 4, p.DeviceType);
    wr32(g + 8, gh(p.hFocusWindow));
    wr32(g + 12, p.BehaviorFlags);
    retStd(c, static_cast<uint32_t>(hr), 2);
}
void ovr_IDirect3DSwapChain9_GetPresentParameters(Ctx* c) {
    auto* self = unwrap<IDirect3DSwapChain9>(arg(c, 0));
    D3DPRESENT_PARAMETERS pp{};
    const HRESULT hr = self->GetPresentParameters(&pp);
    ppOut(arg(c, 1), pp);
    retStd(c, static_cast<uint32_t>(hr), 2);
}

void ovr_IDirect3DSurface9_LockRect(Ctx* c) {
    auto* self = unwrap<IDirect3DSurface9>(arg(c, 0));
    const RECT* r = guestRect(arg(c, 2));
    const DWORD flags = arg(c, 3);
    D3DLOCKED_RECT lr{};
    const HRESULT hr = self->LockRect(&lr, r, flags);
    D3DSURFACE_DESC d{};
    self->GetDesc(&d);
    finishLockRect(c, self, 0, hr, lr, d, r, flags, arg(c, 1), 4);
}
void ovr_IDirect3DSurface9_UnlockRect(Ctx* c) {
    auto* self = unwrap<IDirect3DSurface9>(arg(c, 0));
    stageOut(self, 0);
    retStd(c, static_cast<uint32_t>(self->UnlockRect()), 1);
}
void ovr_IDirect3DSurface9_GetContainer(Ctx* c) {
    getContainer(c, unwrap<IUnknown>(arg(c, 0)),
                 [](IUnknown* s, REFIID i, void** p) { return static_cast<IDirect3DSurface9*>(s)->GetContainer(i, p); });
}
void ovr_IDirect3DTexture9_LockRect(Ctx* c) {
    auto* self = unwrap<IDirect3DTexture9>(arg(c, 0));
    const UINT level = arg(c, 1);
    const RECT* r = guestRect(arg(c, 3));
    const DWORD flags = arg(c, 4);
    D3DLOCKED_RECT lr{};
    const HRESULT hr = self->LockRect(level, &lr, r, flags);
    D3DSURFACE_DESC d{};
    self->GetLevelDesc(level, &d);
    finishLockRect(c, self, level, hr, lr, d, r, flags, arg(c, 2), 5);
}
void ovr_IDirect3DTexture9_UnlockRect(Ctx* c) {
    auto* self = unwrap<IDirect3DTexture9>(arg(c, 0));
    stageOut(self, arg(c, 1));
    retStd(c, static_cast<uint32_t>(self->UnlockRect(arg(c, 1))), 2);
}
void ovr_IDirect3DCubeTexture9_LockRect(Ctx* c) {
    auto* self = unwrap<IDirect3DCubeTexture9>(arg(c, 0));
    const auto face = static_cast<D3DCUBEMAP_FACES>(arg(c, 1));
    const UINT level = arg(c, 2);
    const RECT* r = guestRect(arg(c, 4));
    const DWORD flags = arg(c, 5);
    D3DLOCKED_RECT lr{};
    const HRESULT hr = self->LockRect(face, level, &lr, r, flags);
    D3DSURFACE_DESC d{};
    self->GetLevelDesc(level, &d);
    finishLockRect(c, self, level | (static_cast<uint32_t>(face) << 16), hr, lr, d, r, flags, arg(c, 3), 6);
}
void ovr_IDirect3DCubeTexture9_UnlockRect(Ctx* c) {
    auto* self = unwrap<IDirect3DCubeTexture9>(arg(c, 0));
    stageOut(self, arg(c, 2) | (arg(c, 1) << 16));
    retStd(c, static_cast<uint32_t>(self->UnlockRect(static_cast<D3DCUBEMAP_FACES>(arg(c, 1)), arg(c, 2))), 3);
}
void ovr_IDirect3DVolumeTexture9_LockBox(Ctx* c) {
    auto* self = unwrap<IDirect3DVolumeTexture9>(arg(c, 0));
    const UINT level = arg(c, 1);
    const D3DBOX* b = guestBox(arg(c, 3));
    const DWORD flags = arg(c, 4);
    D3DLOCKED_BOX lb{};
    const HRESULT hr = self->LockBox(level, &lb, b, flags);
    D3DVOLUME_DESC d{};
    self->GetLevelDesc(level, &d);
    finishLockBox(c, self, level, hr, lb, d, b, flags, arg(c, 2), 5);
}
void ovr_IDirect3DVolumeTexture9_UnlockBox(Ctx* c) {
    auto* self = unwrap<IDirect3DVolumeTexture9>(arg(c, 0));
    stageOut(self, arg(c, 1));
    retStd(c, static_cast<uint32_t>(self->UnlockBox(arg(c, 1))), 2);
}
void ovr_IDirect3DVolume9_LockBox(Ctx* c) {
    auto* self = unwrap<IDirect3DVolume9>(arg(c, 0));
    const D3DBOX* b = guestBox(arg(c, 2));
    const DWORD flags = arg(c, 3);
    D3DLOCKED_BOX lb{};
    const HRESULT hr = self->LockBox(&lb, b, flags);
    D3DVOLUME_DESC d{};
    self->GetDesc(&d);
    finishLockBox(c, self, 0, hr, lb, d, b, flags, arg(c, 1), 4);
}
void ovr_IDirect3DVolume9_UnlockBox(Ctx* c) {
    auto* self = unwrap<IDirect3DVolume9>(arg(c, 0));
    stageOut(self, 0);
    retStd(c, static_cast<uint32_t>(self->UnlockBox()), 1);
}
void ovr_IDirect3DVolume9_GetContainer(Ctx* c) {
    getContainer(c, unwrap<IUnknown>(arg(c, 0)),
                 [](IUnknown* s, REFIID i, void** p) { return static_cast<IDirect3DVolume9*>(s)->GetContainer(i, p); });
}
template <class B, class D> void bufferLock(Ctx* c) {
    auto* self = unwrap<B>(arg(c, 0));
    const UINT off = arg(c, 1);
    UINT size = arg(c, 2);
    const DWORD flags = arg(c, 4);
    void* p = nullptr;
    const HRESULT hr = self->Lock(off, size, &p, flags);
    if (SUCCEEDED(hr)) {
        if (!size) {
            D desc{};
            self->GetDesc(&desc);
            size = desc.Size - off;
        }
        wr32(arg(c, 3), stageIn(self, 0, p, size, flags));
    }
    retStd(c, static_cast<uint32_t>(hr), 5);
}
template <class B> void bufferUnlock(Ctx* c) {
    auto* self = unwrap<B>(arg(c, 0));
    stageOut(self, 0);
    retStd(c, static_cast<uint32_t>(self->Unlock()), 1);
}
void ovr_IDirect3DVertexBuffer9_Lock(Ctx* c) { bufferLock<IDirect3DVertexBuffer9, D3DVERTEXBUFFER_DESC>(c); }
void ovr_IDirect3DVertexBuffer9_Unlock(Ctx* c) { bufferUnlock<IDirect3DVertexBuffer9>(c); }
void ovr_IDirect3DIndexBuffer9_Lock(Ctx* c) { bufferLock<IDirect3DIndexBuffer9, D3DINDEXBUFFER_DESC>(c); }
void ovr_IDirect3DIndexBuffer9_Unlock(Ctx* c) { bufferUnlock<IDirect3DIndexBuffer9>(c); }

namespace {

IMPORT("d3d9.dll", Direct3DCreate9) {
    IDirect3D9* d = Direct3DCreate9(arg(c, 0));
    log("Direct3DCreate9(%u) -> %p", arg(c, 0), static_cast<void*>(d));
    retStd(c, wrap(d), 1);
}

D3DX_DYN(D3DXSaveSurfaceToFileW)
D3DX_DYN(D3DXCreateVolumeTexture)
D3DX_DYN(D3DXLoadVolumeFromVolume)
D3DX_DYN(D3DXLoadVolumeFromMemory)
D3DX_DYN(D3DXCreateTextureFromFileExW)
D3DX_DYN(D3DXFilterTexture)
D3DX_DYN(D3DXLoadSurfaceFromFileW)
D3DX_DYN(D3DXLoadSurfaceFromSurface)
D3DX_DYN(D3DXLoadSurfaceFromMemory)

IMPORT("d3dx9_25.dll", D3DXVec3Hermite) {
    const float* v1 = argp<float>(c, 1);
    const float* t1 = argp<float>(c, 2);
    const float* v2 = argp<float>(c, 3);
    const float* t2 = argp<float>(c, 4);
    const float s = rdf32(c->esp + 4 + 20);
    const float s2 = s * s, s3 = s2 * s;
    const float h1 = 2.0f * s3 - 3.0f * s2 + 1.0f, h2 = s3 - 2.0f * s2 + s, h3 = -2.0f * s3 + 3.0f * s2, h4 = s3 - s2;
    float* o = argp<float>(c, 0);
    for (int i = 0; i < 3; ++i) o[i] = h1 * v1[i] + h2 * t1[i] + h3 * v2[i] + h4 * t2[i];
    retStd(c, arg(c, 0), 6);
}

IMPORT("d3dx9_25.dll", D3DXMatrixInverse) {
    const float* m = argp<float>(c, 2);
    float inv[16];
    inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
    inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
    inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
    inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
    inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
    inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
    inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
    inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
    inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
    inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
    inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
    inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
    inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
    inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
    inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
    inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];
    const float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
    if (arg(c, 1)) wrf32(arg(c, 1), det);
    if (det == 0.0f) { retStd(c, 0, 3); return; }
    const float r = 1.0f / det;
    float* o = argp<float>(c, 0);
    for (int i = 0; i < 16; ++i) o[i] = inv[i] * r;
    retStd(c, arg(c, 0), 3);
}

}  // namespace
