// Direct3D 9 on OpenGL ES 3.0 (see gl_renderer.hpp). Scope: what Fable TLC uses, from the
// census (census.cpp): vs_1_1/ps_1_x shaders (dx9shader.cpp), the pre-transformed vertex
// path for the 2D layer, the fixed-function pixel pipeline for a few passes, textures in
// A8R8G8B8/X8R8G8B8/R5G6B5/A4R4G4B4/A8/L8/A8L8/V8U8/DXT1-5 (DXT decoded on the CPU: Mali has
// no S3TC), render-target textures, D16 depth textures and D24S8 depth-stencil surfaces,
// dynamic vertex/index buffers, occlusion queries.
//
// Conventions: everything renders into an FBO whose rows are in Direct3D order (the vertex
// epilogue negates clip y), so render-target textures sample as on Direct3D; Present blits
// the FBO to the window with a vertical flip. Clip z is remapped from [0,1] to [-1,1].
#include <SDL.h>  // before the Windows headers: SDL keys its intrinsics on MinGW macros
#define GL_GLES_PROTOTYPES 0  // entry points come from SDL_GL_GetProcAddress (loadGl): no link-time libGLESv2
#include <GLES3/gl3.h>
#include <unistd.h>

#include "gl_renderer.hpp"

#include "dx9shader.hpp"
#include "../w32/w32.hpp"
#include "egl_fbdev.hpp"
#include "gl_queue_gen.hpp"
#include "../overlay/overlay.hpp"
#include "gfx_options.hpp"
#include "../overlay/overlay_font.inc"

#include <algorithm>
#include <atomic>
#include <cmath>
#include <cerrno>
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

using namespace host;

namespace host::com { extern std::atomic<uint32_t> g_queryInterfaceCalls; }  // com.cpp: guest QueryInterface calls (perf log)
namespace host::gles {
static bool g_rtDumpThisDraw;  // FABLE_GLES_DUMP_RT=all: this draw goes to the dump (prepareDraw/afterDraw)

// ---- GL entry points, resolved from the context's driver (the device's own libGLESv2) ----
namespace {
void* getProc(const char* n) { return w32::fbdevGl() ? fb::proc(n) : SDL_GL_GetProcAddress(n); }
bool loadGl() { return gq::loadReal(getProc); }
}  // namespace
namespace {

struct Device;
Device* g_device = nullptr;

#define GLCHECK(what) \
    do { \
        if (host::g_logLevel >= 2) { GLenum e = glGetError(); if (e) log("gles: GL error 0x%X at %s", e, what); } \
    } while (0)

// ---- timing: nanoseconds spent inside the renderer, per frame (the overlay statistics) ----
inline uint64_t nowNs() { timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return static_cast<uint64_t>(t.tv_sec) * 1000000000ull + t.tv_nsec; }
uint64_t g_nsGl = 0;
struct GlTime { uint64_t t0 = nowNs(); ~GlTime() { g_nsGl += nowNs() - t0; } };
uint64_t g_nsSwap = 0, g_nsUpload = 0;  // parts of g_nsGl: the swap (vsync wait) and texture uploads
// vertex/index buffer uploads this frame (the frame log line): an in-place write to a buffer the
// GPU may still read (glBufferSubData) makes a tiler's driver copy the buffer or wait
uint32_t g_bufUploads = 0, g_bufInPlace = 0;
uint64_t g_bufBytes = 0;
std::atomic<uint64_t> g_nsSwapRender{0};  // the swap as timed on the render thread
uint64_t g_nsCapSleep = 0;  // game thread: time slept by the frame rate cap since the last stats line
unsigned g_capLate = 0;     // frames that ended after the cap's deadline (no sleep) since the last stats line

// A 64-bit hash of a byte range, 8 bytes a step (each step is a bijection of the state, so a
// single changed word always changes the result). The byte-wise FNV it replaces cost ~5 cycles
// a byte on the A53 (a dependent multiply per byte): the vertex-array key of every draw and the
// HUD's re-locked text textures go through it.
inline uint64_t hashBytes(const void* data, size_t n) {
    const unsigned char* p = static_cast<const unsigned char*>(data);
    uint64_t h = 0x9E3779B97F4A7C15ull ^ n;
    size_t i = 0;
    for (; i + 8 <= n; i += 8) {
        uint64_t w;
        std::memcpy(&w, p + i, 8);
        h = (h ^ w) * 0xFF51AFD7ED558CCDull;
        h ^= h >> 32;
    }
    for (; i < n; ++i) h = (h ^ p[i]) * 1099511628211ull;
    return h ^ (h >> 29);
}

// ---- COM base ------------------------------------------------------------------------------
template <class I, const IID* ID> struct Unknown : I {
    LONG refs = 1;
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** out) override {
        if (!out) return E_POINTER;
        if (IsEqualGUID(riid, *ID) || IsEqualGUID(riid, IID_IUnknown)) { *out = this; AddRef(); return S_OK; }
        *out = nullptr;
        return E_NOINTERFACE;
    }
    ULONG STDMETHODCALLTYPE AddRef() override { return ++refs; }
    ULONG STDMETHODCALLTYPE Release() override {
        const LONG n = --refs;
        if (n == 0) delete this;
        return n;
    }
    virtual ~Unknown() = default;
};
template <class T> struct Ref {
    T* p = nullptr;
    Ref() = default;
    Ref(T* q) : p(q) { if (p) p->AddRef(); }
    Ref(const Ref& o) : p(o.p) { if (p) p->AddRef(); }
    Ref& operator=(T* q) { if (q) q->AddRef(); if (p) p->Release(); p = q; return *this; }
    Ref& operator=(const Ref& o) { return *this = o.p; }
    ~Ref() { if (p) p->Release(); }
    T* operator->() const { return p; }
    operator T*() const { return p; }
};

// ---- formats ------------------------------------------------------------------------------
struct FormatInfo {
    GLenum internal = 0, format = 0, type = 0;
    int bytesPerPixel = 0;  // 0: block compressed (DXT)
    int blockBytes = 0;     // DXT: bytes per 4x4 block
    bool depth = false;
    GLint swizzle[4] = {GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA};
    bool valid = false;
};
FormatInfo formatInfo(D3DFORMAT f) {
    FormatInfo i;
    i.valid = true;
    switch (static_cast<uint32_t>(f)) {
        case D3DFMT_A8R8G8B8: i = {GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, 4, 0, false, {GL_BLUE, GL_GREEN, GL_RED, GL_ALPHA}, true}; break;
        case D3DFMT_X8R8G8B8: i = {GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, 4, 0, false, {GL_BLUE, GL_GREEN, GL_RED, GL_ONE}, true}; break;
        case D3DFMT_A8B8G8R8: i = {GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, 4, 0, false, {GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA}, true}; break;
        case D3DFMT_X8B8G8R8: i = {GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, 4, 0, false, {GL_RED, GL_GREEN, GL_BLUE, GL_ONE}, true}; break;
        case D3DFMT_R5G6B5: i = {GL_RGB565, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, 2, 0, false, {GL_RED, GL_GREEN, GL_BLUE, GL_ONE}, true}; break;
        case D3DFMT_A4R4G4B4: i = {GL_RGBA4, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, 2, 0, false, {GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA}, true}; break;  // converted on upload
        case D3DFMT_A1R5G5B5: i = {GL_RGB5_A1, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, 2, 0, false, {GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA}, true}; break;  // converted
        case D3DFMT_X1R5G5B5: i = {GL_RGB5_A1, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, 2, 0, false, {GL_RED, GL_GREEN, GL_BLUE, GL_ONE}, true}; break;
        case D3DFMT_A8: i = {GL_R8, GL_RED, GL_UNSIGNED_BYTE, 1, 0, false, {GL_ZERO, GL_ZERO, GL_ZERO, GL_RED}, true}; break;
        case D3DFMT_L8: i = {GL_R8, GL_RED, GL_UNSIGNED_BYTE, 1, 0, false, {GL_RED, GL_RED, GL_RED, GL_ONE}, true}; break;
        case D3DFMT_A8L8: i = {GL_RG8, GL_RG, GL_UNSIGNED_BYTE, 2, 0, false, {GL_RED, GL_RED, GL_RED, GL_GREEN}, true}; break;
        case D3DFMT_V8U8: i = {GL_RG8_SNORM, GL_RG, GL_BYTE, 2, 0, false, {GL_RED, GL_GREEN, GL_ONE, GL_ONE}, true}; break;
        case D3DFMT_DXT1: i = {GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, 0, 8, false, {GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA}, true}; break;
        case D3DFMT_DXT2: case D3DFMT_DXT3: i = {GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, 0, 16, false, {GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA}, true}; break;
        case D3DFMT_DXT4: case D3DFMT_DXT5: i = {GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, 0, 16, false, {GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA}, true}; break;
        case D3DFMT_D16: case D3DFMT_D16_LOCKABLE: i = {GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, 2, 0, true, {GL_RED, GL_RED, GL_RED, GL_ONE}, true}; break;
        case D3DFMT_D24S8: case D3DFMT_D24X8: i = {GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 4, 0, true, {GL_RED, GL_RED, GL_RED, GL_ONE}, true}; break;
        case D3DFMT_D32: case D3DFMT_D24FS8: i = {GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 4, 0, true, {GL_RED, GL_RED, GL_RED, GL_ONE}, true}; break;
        default: i.valid = false; break;
    }
    return i;
}
bool isDxt(D3DFORMAT f) { return f == D3DFMT_DXT1 || f == D3DFMT_DXT2 || f == D3DFMT_DXT3 || f == D3DFMT_DXT4 || f == D3DFMT_DXT5; }
// The pitch Direct3D reports for a locked level, and the bytes of the level.
UINT levelPitch(D3DFORMAT f, UINT w) {
    const FormatInfo i = formatInfo(f);
    if (i.blockBytes) return ((w + 3) / 4) * i.blockBytes;
    return w * std::max(1, i.bytesPerPixel);
}
UINT levelBytes(D3DFORMAT f, UINT w, UINT h) {
    const FormatInfo i = formatInfo(f);
    if (i.blockBytes) return ((w + 3) / 4) * ((h + 3) / 4) * i.blockBytes;
    return w * h * std::max(1, i.bytesPerPixel);
}

// DXT decoding to RGBA8.
void decodeColor565(uint16_t c, uint8_t* out) {
    out[0] = static_cast<uint8_t>(((c >> 11) & 31) * 255 / 31);
    out[1] = static_cast<uint8_t>(((c >> 5) & 63) * 255 / 63);
    out[2] = static_cast<uint8_t>((c & 31) * 255 / 31);
    out[3] = 255;
}
void decodeDxtBlock(const uint8_t* block, bool dxt1, int alphaKind, uint8_t rgba[16][4]) {
    // alphaKind: 0 none (DXT1), 3 explicit (DXT3), 5 interpolated (DXT5)
    const uint8_t* cb = block + (dxt1 ? 0 : 8);
    uint16_t c0, c1;
    std::memcpy(&c0, cb, 2);
    std::memcpy(&c1, cb + 2, 2);
    uint8_t col[4][4];
    decodeColor565(c0, col[0]);
    decodeColor565(c1, col[1]);
    if (c0 > c1 || !dxt1) {
        for (int k = 0; k < 3; ++k) col[2][k] = static_cast<uint8_t>((2 * col[0][k] + col[1][k]) / 3), col[3][k] = static_cast<uint8_t>((col[0][k] + 2 * col[1][k]) / 3);
        col[2][3] = col[3][3] = 255;
    } else {
        for (int k = 0; k < 3; ++k) col[2][k] = static_cast<uint8_t>((col[0][k] + col[1][k]) / 2), col[3][k] = 0;
        col[2][3] = 255, col[3][3] = 0;
    }
    uint32_t idx;
    std::memcpy(&idx, cb + 4, 4);
    for (int p = 0; p < 16; ++p) std::memcpy(rgba[p], col[(idx >> (2 * p)) & 3], 4);
    if (alphaKind == 3) {
        uint64_t a;
        std::memcpy(&a, block, 8);
        for (int p = 0; p < 16; ++p) rgba[p][3] = static_cast<uint8_t>(((a >> (4 * p)) & 15) * 17);
    } else if (alphaKind == 5) {
        const uint8_t a0 = block[0], a1 = block[1];
        uint8_t a[8] = {a0, a1};
        if (a0 > a1) for (int k = 1; k < 7; ++k) a[1 + k] = static_cast<uint8_t>(((7 - k) * a0 + k * a1) / 7);
        else { for (int k = 1; k < 5; ++k) a[1 + k] = static_cast<uint8_t>(((5 - k) * a0 + k * a1) / 5); a[6] = 0; a[7] = 255; }
        uint64_t bits = 0;
        std::memcpy(&bits, block + 2, 6);
        for (int p = 0; p < 16; ++p) rgba[p][3] = a[(bits >> (3 * p)) & 7];
    }
}
std::vector<uint8_t> decodeDxt(D3DFORMAT f, const uint8_t* src, UINT w, UINT h) {
    std::vector<uint8_t> out(static_cast<size_t>(w) * h * 4);
    const bool dxt1 = f == D3DFMT_DXT1;
    const int alphaKind = dxt1 ? 0 : (f == D3DFMT_DXT2 || f == D3DFMT_DXT3) ? 3 : 5;
    const int bw = (w + 3) / 4, bh = (h + 3) / 4, bs = dxt1 ? 8 : 16;
    uint8_t rgba[16][4];
    for (int by = 0; by < bh; ++by)
        for (int bx = 0; bx < bw; ++bx) {
            decodeDxtBlock(src + (by * bw + bx) * bs, dxt1, alphaKind, rgba);
            for (int py = 0; py < 4; ++py) {
                const UINT y = by * 4 + py;
                if (y >= h) break;
                for (int px = 0; px < 4; ++px) {
                    const UINT x = bx * 4 + px;
                    if (x >= w) break;
                    std::memcpy(&out[(static_cast<size_t>(y) * w + x) * 4], rgba[py * 4 + px], 4);
                }
            }
        }
    return out;
}
// Repacks the 16-bit formats whose bit order differs between the APIs.
void convert16(D3DFORMAT f, const uint8_t* src, uint8_t* dst, size_t n) {
    const uint16_t* s = reinterpret_cast<const uint16_t*>(src);
    uint16_t* d = reinterpret_cast<uint16_t*>(dst);
    if (f == D3DFMT_A4R4G4B4) {
        for (size_t i = 0; i < n; ++i) { const uint16_t v = s[i]; d[i] = static_cast<uint16_t>(((v >> 8) & 0xF) << 12 | ((v >> 4) & 0xF) << 8 | (v & 0xF) << 4 | (v >> 12)); }  // ARGB -> RGBA
    } else if (f == D3DFMT_A1R5G5B5 || f == D3DFMT_X1R5G5B5) {
        for (size_t i = 0; i < n; ++i) { const uint16_t v = s[i]; d[i] = static_cast<uint16_t>(((v >> 10) & 31) << 11 | ((v >> 5) & 31) << 6 | (v & 31) << 1 | (v >> 15)); }  // ARGB1555 -> RGBA5551
    } else {
        std::memcpy(dst, src, n * 2);
    }
}

// 16-bit storage for decoded DXT (FABLE_GLES_TEX16 / texture detail Low): half the memory and
// the texture-fetch bandwidth of RGBA8 on a bandwidth-bound tiler. The storage follows what the
// texture's levels need, in this order; a level that needs more re-creates it.
enum DxtStore { ST_565, ST_5551, ST_4444, ST_8888 };
struct DxtStoreFormat { GLenum internal, format, type; };
constexpr DxtStoreFormat kDxtStore[4] = {
    {GL_RGB565, GL_RGB, GL_UNSIGNED_SHORT_5_6_5},        // opaque (every DXT1 colour endpoint is exactly 565)
    {GL_RGB5_A1, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1},    // alpha only 0 or 255 (DXT1 punch-through, alpha-tested DXT3/5)
    {GL_RGBA4, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4},      // smooth alpha, mode 2 only (DXT3's alpha is 4-bit already)
    {GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE},               // smooth alpha
};
// 0 off; 1: RGB565 / RGB5_A1, smooth alpha stays RGBA8; 2: smooth alpha as RGBA4 as well.
// FABLE_GLES_TEX16 sets it; unset, it is 1 when the host texture detail is Low.
int dxt16Mode() {
    static const int env = [] { const char* e = getenv("FABLE_GLES_TEX16"); return e ? std::clamp(atoi(e), 0, 2) : -1; }();
    if (env >= 0) return env;
    return host::gfx::get().texture == host::gfx::LOW ? 1 : 0;
}
// The storage a decoded level needs.
int dxtNeed(const std::vector<uint8_t>& rgba, int mode) {
    bool opaque = true;
    for (size_t i = 3; i < rgba.size(); i += 4) {
        const uint8_t a = rgba[i];
        if (a == 255) continue;
        if (a != 0) return mode >= 2 ? ST_4444 : ST_8888;
        opaque = false;
    }
    return opaque ? ST_565 : ST_5551;
}
// RGBA8 -> a packed 16-bit store (first component in the high bits, as GL's packed types read),
// rounded to nearest (a 565 endpoint decoded by decodeColor565 packs back to itself).
void packDxt16(int rank, const uint8_t* rgba, size_t n, uint16_t* out) {
    auto q = [](unsigned v, unsigned max) { return (v * max + 127) / 255; };
    for (size_t i = 0; i < n; ++i, rgba += 4) {
        const unsigned r = rgba[0], g = rgba[1], b = rgba[2], a = rgba[3];
        if (rank == ST_565) out[i] = static_cast<uint16_t>(q(r, 31) << 11 | q(g, 63) << 5 | q(b, 31));
        else if (rank == ST_5551) out[i] = static_cast<uint16_t>(q(r, 31) << 11 | q(g, 31) << 6 | q(b, 31) << 1 | (a >= 128 ? 1 : 0));
        else out[i] = static_cast<uint16_t>(q(r, 15) << 12 | q(g, 15) << 8 | q(b, 15) << 4 | q(a, 15));
    }
}

// ---- resources ----------------------------------------------------------------------------
struct Surface;
struct Texture;

struct Level {  // one mip level (or the only level) of a texture / a standalone surface
    UINT width = 0, height = 0;
    std::vector<uint8_t> cpu;  // system-memory copy (allocated at the first Lock)
    bool locked = false;
    RECT lockRect{};
    bool wholeLock = true;
    uint64_t hash = 0;  // of the last upload (small levels only): the HUD re-locks unchanged text each frame
};

struct Surface : Unknown<IDirect3DSurface9, &IID_IDirect3DSurface9> {
    Device* dev;
    D3DFORMAT format;
    D3DPOOL pool = D3DPOOL_DEFAULT;
    DWORD usage = 0;
    Level level;
    Texture* texture = nullptr;  // owner (not referenced: the texture outlives its surfaces through the device's bookkeeping)
    UINT mip = 0;
    GLuint rbo = 0;   // standalone render target / depth-stencil: a renderbuffer
    GLuint tex = 0;   // standalone render target as a texture (for StretchRect sources) - unused
    bool isBackBuffer = false, isMainDepth = false;
    Surface(Device* d, D3DFORMAT f, UINT w, UINT h) : dev(d), format(f) { level.width = w, level.height = h; }
    ~Surface() override;
    // IDirect3DResource9
    HRESULT STDMETHODCALLTYPE GetDevice(IDirect3DDevice9** out) override;
    HRESULT STDMETHODCALLTYPE SetPrivateData(REFGUID, const void*, DWORD, DWORD) override { return S_OK; }
    HRESULT STDMETHODCALLTYPE GetPrivateData(REFGUID, void*, DWORD*) override { return D3DERR_NOTFOUND; }
    HRESULT STDMETHODCALLTYPE FreePrivateData(REFGUID) override { return S_OK; }
    DWORD STDMETHODCALLTYPE SetPriority(DWORD) override { return 0; }
    DWORD STDMETHODCALLTYPE GetPriority() override { return 0; }
    void STDMETHODCALLTYPE PreLoad() override {}
    D3DRESOURCETYPE STDMETHODCALLTYPE GetType() override { return D3DRTYPE_SURFACE; }
    // IDirect3DSurface9
    HRESULT STDMETHODCALLTYPE GetContainer(REFIID, void** out) override;
    HRESULT STDMETHODCALLTYPE GetDesc(D3DSURFACE_DESC* d) override {
        if (!d) return E_POINTER;
        *d = {};
        d->Format = format, d->Type = D3DRTYPE_SURFACE, d->Usage = usage, d->Pool = pool, d->MultiSampleType = D3DMULTISAMPLE_NONE;
        d->Width = level.width, d->Height = level.height;
        return S_OK;
    }
    HRESULT STDMETHODCALLTYPE LockRect(D3DLOCKED_RECT* out, const RECT* rect, DWORD flags) override;
    HRESULT STDMETHODCALLTYPE UnlockRect() override;
    HRESULT STDMETHODCALLTYPE GetDC(HDC*) override { return D3DERR_INVALIDCALL; }
    HRESULT STDMETHODCALLTYPE ReleaseDC(HDC) override { return D3DERR_INVALIDCALL; }
};

struct Texture : Unknown<IDirect3DTexture9, &IID_IDirect3DTexture9> {
    Device* dev;
    D3DFORMAT format;
    D3DPOOL pool;
    DWORD usage;
    UINT width, height, levels;
    FormatInfo info;
    GLuint gl = 0;
    std::vector<Surface*> surfaces;  // one per level, created on demand (referenced by us)
    DWORD lod = 0;
    D3DTEXTUREFILTERTYPE autogen = D3DTEXF_LINEAR;
    int tex16 = 0;        // DXT decoded to 16-bit texels (dxt16Mode at creation); 0 = RGBA8
    int storeRank = -1;   // tex16: the DxtStore its storage has (-1: none yet, allocated at the first upload)
    Texture(Device* d, UINT w, UINT h, UINT l, DWORD u, D3DFORMAT f, D3DPOOL p);
    ~Texture() override;
    Surface* surface(UINT level);
    void upload(UINT level, const RECT* rect, bool force = false);
    void initObject(GLenum internal);  // binds gl, gives it storage (0: none) and its parameters
    void store16(int rank);            // tex16: storage of that rank (a new object if it had one)
    // IDirect3DResource9
    HRESULT STDMETHODCALLTYPE GetDevice(IDirect3DDevice9** out) override;
    HRESULT STDMETHODCALLTYPE SetPrivateData(REFGUID, const void*, DWORD, DWORD) override { return S_OK; }
    HRESULT STDMETHODCALLTYPE GetPrivateData(REFGUID, void*, DWORD*) override { return D3DERR_NOTFOUND; }
    HRESULT STDMETHODCALLTYPE FreePrivateData(REFGUID) override { return S_OK; }
    DWORD STDMETHODCALLTYPE SetPriority(DWORD) override { return 0; }
    DWORD STDMETHODCALLTYPE GetPriority() override { return 0; }
    void STDMETHODCALLTYPE PreLoad() override {}
    D3DRESOURCETYPE STDMETHODCALLTYPE GetType() override { return D3DRTYPE_TEXTURE; }
    // IDirect3DBaseTexture9
    DWORD STDMETHODCALLTYPE SetLOD(DWORD l) override { const DWORD o = lod; lod = l; return o; }
    DWORD STDMETHODCALLTYPE GetLOD() override { return lod; }
    DWORD STDMETHODCALLTYPE GetLevelCount() override { return levels; }
    HRESULT STDMETHODCALLTYPE SetAutoGenFilterType(D3DTEXTUREFILTERTYPE t) override { autogen = t; return S_OK; }
    D3DTEXTUREFILTERTYPE STDMETHODCALLTYPE GetAutoGenFilterType() override { return autogen; }
    void STDMETHODCALLTYPE GenerateMipSubLevels() override;
    // IDirect3DTexture9
    HRESULT STDMETHODCALLTYPE GetLevelDesc(UINT level, D3DSURFACE_DESC* d) override { return level < levels ? surface(level)->GetDesc(d) : D3DERR_INVALIDCALL; }
    HRESULT STDMETHODCALLTYPE GetSurfaceLevel(UINT level, IDirect3DSurface9** out) override {
        if (level >= levels || !out) return D3DERR_INVALIDCALL;
        Surface* s = surface(level);
        s->AddRef();
        *out = s;
        return S_OK;
    }
    HRESULT STDMETHODCALLTYPE LockRect(UINT level, D3DLOCKED_RECT* out, const RECT* rect, DWORD flags) override { return level < levels ? surface(level)->LockRect(out, rect, flags) : D3DERR_INVALIDCALL; }
    HRESULT STDMETHODCALLTYPE UnlockRect(UINT level) override { return level < levels ? surface(level)->UnlockRect() : D3DERR_INVALIDCALL; }
    HRESULT STDMETHODCALLTYPE AddDirtyRect(const RECT*) override { return S_OK; }
};

void dropVaosForBuffer(GLuint gl);       // Device: forget cached vertex array objects that reference it
void dropVaosForPointer(const void* p);  // ... a vertex declaration / shader address about to be reused

struct Buffer {  // vertex / index buffer storage
    GLuint gl = 0;
    UINT size = 0;
    std::vector<uint8_t> cpu;
    bool locked = false;
    UINT lockOff = 0, lockSize = 0;
    DWORD lockFlags = 0;
    bool dirtyAll = true;
    bool dynamic = false;
    GLenum target;
    void create(GLenum t, UINT n, DWORD usage) {
        target = t, size = n, dynamic = (usage & D3DUSAGE_DYNAMIC) != 0;
        cpu.assign(n, 0);
        glGenBuffers(1, &gl);
        // Uploads go through the copy-write binding point: binding an index buffer to
        // GL_ELEMENT_ARRAY_BUFFER here would silently replace the element buffer of whichever
        // cached vertex array object is bound, behind the device's tracking.
        glBindBuffer(GL_COPY_WRITE_BUFFER, gl);
        glBufferData(GL_COPY_WRITE_BUFFER, n, nullptr, (usage & D3DUSAGE_DYNAMIC) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    }
    void* lock(UINT off, UINT n, DWORD flags) {
        if (n == 0 || off + n > size) n = size - off;
        locked = true, lockOff = off, lockSize = n, lockFlags = flags;
        return cpu.data() + off;
    }
    void unlock() {
        if (!locked) return;
        locked = false;
        if (lockFlags & D3DLOCK_READONLY) return;
        GlTime t;
        glBindBuffer(GL_COPY_WRITE_BUFFER, gl);  // never the element target (see create)
        // A dynamic buffer still in use by queued draws must not be written in place (the
        // driver would wait or copy): orphan it and upload the whole mirror, unless the game
        // promises NOOVERWRITE. Static buffers are filled before use.
        ++g_bufUploads;
        if ((lockFlags & D3DLOCK_DISCARD) || (dynamic && !(lockFlags & D3DLOCK_NOOVERWRITE))) {
            glBufferData(GL_COPY_WRITE_BUFFER, size, cpu.data(), GL_DYNAMIC_DRAW);
            g_bufBytes += size;
        } else {
            glBufferSubData(GL_COPY_WRITE_BUFFER, lockOff, lockSize, cpu.data() + lockOff);
            ++g_bufInPlace, g_bufBytes += lockSize;
        }
    }
    ~Buffer() { if (gl) { dropVaosForBuffer(gl); glDeleteBuffers(1, &gl); } }
};

struct VertexBuffer : Unknown<IDirect3DVertexBuffer9, &IID_IDirect3DVertexBuffer9> {
    Device* dev;
    Buffer buf;
    DWORD usage, fvf;
    D3DPOOL pool;
    VertexBuffer(Device* d, UINT n, DWORD u, DWORD f, D3DPOOL p) : dev(d), usage(u), fvf(f), pool(p) { buf.create(GL_ARRAY_BUFFER, n, u); }
    HRESULT STDMETHODCALLTYPE GetDevice(IDirect3DDevice9** out) override;
    HRESULT STDMETHODCALLTYPE SetPrivateData(REFGUID, const void*, DWORD, DWORD) override { return S_OK; }
    HRESULT STDMETHODCALLTYPE GetPrivateData(REFGUID, void*, DWORD*) override { return D3DERR_NOTFOUND; }
    HRESULT STDMETHODCALLTYPE FreePrivateData(REFGUID) override { return S_OK; }
    DWORD STDMETHODCALLTYPE SetPriority(DWORD) override { return 0; }
    DWORD STDMETHODCALLTYPE GetPriority() override { return 0; }
    void STDMETHODCALLTYPE PreLoad() override {}
    D3DRESOURCETYPE STDMETHODCALLTYPE GetType() override { return D3DRTYPE_VERTEXBUFFER; }
    HRESULT STDMETHODCALLTYPE Lock(UINT off, UINT n, void** out, DWORD flags) override { if (!out) return E_POINTER; *out = buf.lock(off, n, flags); return S_OK; }
    HRESULT STDMETHODCALLTYPE Unlock() override { buf.unlock(); return S_OK; }
    HRESULT STDMETHODCALLTYPE GetDesc(D3DVERTEXBUFFER_DESC* d) override { if (!d) return E_POINTER; *d = {D3DFMT_VERTEXDATA, D3DRTYPE_VERTEXBUFFER, usage, pool, buf.size, fvf}; return S_OK; }
};
struct IndexBuffer : Unknown<IDirect3DIndexBuffer9, &IID_IDirect3DIndexBuffer9> {
    Device* dev;
    Buffer buf;
    DWORD usage;
    D3DFORMAT format;
    D3DPOOL pool;
    IndexBuffer(Device* d, UINT n, DWORD u, D3DFORMAT f, D3DPOOL p) : dev(d), usage(u), format(f), pool(p) { buf.create(GL_ELEMENT_ARRAY_BUFFER, n, u); }
    HRESULT STDMETHODCALLTYPE GetDevice(IDirect3DDevice9** out) override;
    HRESULT STDMETHODCALLTYPE SetPrivateData(REFGUID, const void*, DWORD, DWORD) override { return S_OK; }
    HRESULT STDMETHODCALLTYPE GetPrivateData(REFGUID, void*, DWORD*) override { return D3DERR_NOTFOUND; }
    HRESULT STDMETHODCALLTYPE FreePrivateData(REFGUID) override { return S_OK; }
    DWORD STDMETHODCALLTYPE SetPriority(DWORD) override { return 0; }
    DWORD STDMETHODCALLTYPE GetPriority() override { return 0; }
    void STDMETHODCALLTYPE PreLoad() override {}
    D3DRESOURCETYPE STDMETHODCALLTYPE GetType() override { return D3DRTYPE_INDEXBUFFER; }
    HRESULT STDMETHODCALLTYPE Lock(UINT off, UINT n, void** out, DWORD flags) override { if (!out) return E_POINTER; *out = buf.lock(off, n, flags); return S_OK; }
    HRESULT STDMETHODCALLTYPE Unlock() override { buf.unlock(); return S_OK; }
    HRESULT STDMETHODCALLTYPE GetDesc(D3DINDEXBUFFER_DESC* d) override { if (!d) return E_POINTER; *d = {format, D3DRTYPE_INDEXBUFFER, usage, pool, buf.size}; return S_OK; }
};

struct VertexDecl : Unknown<IDirect3DVertexDeclaration9, &IID_IDirect3DVertexDeclaration9> {
    Device* dev;
    std::vector<D3DVERTEXELEMENT9> elements;  // without the terminator
    bool preTransformed = false;
    ~VertexDecl() override { dropVaosForPointer(this); }
    VertexDecl(Device* d, const D3DVERTEXELEMENT9* el) : dev(d) {
        for (const D3DVERTEXELEMENT9* e = el; e->Stream != 0xFF; ++e) {
            elements.push_back(*e);
            if (e->Usage == D3DDECLUSAGE_POSITIONT) preTransformed = true;
        }
    }
    HRESULT STDMETHODCALLTYPE GetDevice(IDirect3DDevice9** out) override;
    HRESULT STDMETHODCALLTYPE GetDeclaration(D3DVERTEXELEMENT9* out, UINT* n) override {
        if (!n) return E_POINTER;
        if (out) {
            for (size_t i = 0; i < elements.size(); ++i) out[i] = elements[i];
            out[elements.size()] = D3DDECL_END();
        }
        *n = static_cast<UINT>(elements.size() + 1);
        return S_OK;
    }
};

struct Shader {
    Dx9Shader info;
    GLuint gl = 0;
    bool ok = false;
};
struct VertexShader : Unknown<IDirect3DVertexShader9, &IID_IDirect3DVertexShader9> {
    Device* dev;
    Shader sh;
    std::vector<uint32_t> code;
    uint32_t id;
    HRESULT STDMETHODCALLTYPE GetDevice(IDirect3DDevice9** out) override;
    HRESULT STDMETHODCALLTYPE GetFunction(void* out, UINT* n) override {
        if (!n) return E_POINTER;
        if (out) std::memcpy(out, code.data(), code.size() * 4);
        *n = static_cast<UINT>(code.size() * 4);
        return S_OK;
    }
    ~VertexShader() override { dropVaosForPointer(this); if (sh.gl) glDeleteShader(sh.gl); }
};
struct PixelShader : Unknown<IDirect3DPixelShader9, &IID_IDirect3DPixelShader9> {
    Device* dev;
    Shader sh;
    GLuint glAlphaTest = 0;  // the variant with the alpha test (withAlphaTest), compiled when a draw first needs it
    std::vector<uint32_t> code;
    uint32_t id;
    HRESULT STDMETHODCALLTYPE GetDevice(IDirect3DDevice9** out) override;
    HRESULT STDMETHODCALLTYPE GetFunction(void* out, UINT* n) override {
        if (!n) return E_POINTER;
        if (out) std::memcpy(out, code.data(), code.size() * 4);
        *n = static_cast<UINT>(code.size() * 4);
        return S_OK;
    }
    ~PixelShader() override { if (sh.gl) glDeleteShader(sh.gl); if (glAlphaTest) glDeleteShader(glAlphaTest); }
};

struct Query : Unknown<IDirect3DQuery9, &IID_IDirect3DQuery9> {
    Device* dev;
    D3DQUERYTYPE type;
    GLuint gl = 0;
    bool issued = false, active = false;
    struct State { std::atomic<DWORD> lastResult{256}; std::atomic<bool> checking{false}; };  // shared with the render thread's check
    std::shared_ptr<State> st = std::make_shared<State>();  // the last answer; "visible" before the first one
    Query(Device* d, D3DQUERYTYPE t) : dev(d), type(t) { if (t == D3DQUERYTYPE_OCCLUSION) glGenQueries(1, &gl); }
    ~Query() override { if (gl) glDeleteQueries(1, &gl); }
    HRESULT STDMETHODCALLTYPE GetDevice(IDirect3DDevice9** out) override;
    D3DQUERYTYPE STDMETHODCALLTYPE GetType() override { return type; }
    DWORD STDMETHODCALLTYPE GetDataSize() override { return 4; }
    HRESULT STDMETHODCALLTYPE Issue(DWORD flags) override {
        if (!gl) return S_OK;
        if (flags & D3DISSUE_BEGIN) { glBeginQuery(GL_ANY_SAMPLES_PASSED, gl); active = true; issued = false; }
        if (flags & D3DISSUE_END) { if (active) glEndQuery(GL_ANY_SAMPLES_PASSED); active = false; issued = true; }
        return S_OK;
    }
    HRESULT STDMETHODCALLTYPE GetData(void* out, DWORD size, DWORD flags) override {
        if (!gl) { if (out && size >= 4) *static_cast<DWORD*>(out) = 0; return S_OK; }
        if (!issued) return S_FALSE;
        // Waiting for the GPU here (D3DGETDATA_FLUSH, polled every frame for the sun's lens
        // flare) would serialise the CPU and the GPU, and asking the driver from this thread
        // would be a round trip through the render thread's queue: the check runs there, without
        // blocking, and the previous answer is returned meanwhile, which a flare cannot tell apart.
        if (!st->checking.exchange(true)) {
            const GLuint q = gl;
            std::shared_ptr<State> state = st;
            gq::call([q, state] {
                GLuint avail = 0;
                glGetQueryObjectuiv(q, GL_QUERY_RESULT_AVAILABLE, &avail);
                if (avail) {
                    GLuint r = 0;
                    glGetQueryObjectuiv(q, GL_QUERY_RESULT, &r);
                    state->lastResult.store(r ? 256 : 0);  // Direct3D reports a pixel count; the boolean query stands in
                }
                state->checking.store(false);
            });
        }
        if (out && size >= 4) *static_cast<DWORD*>(out) = st->lastResult.load();
        return S_OK;
    }
};

struct SwapChain : Unknown<IDirect3DSwapChain9, &IID_IDirect3DSwapChain9> {
    Device* dev;
    explicit SwapChain(Device* d) : dev(d) {}
    HRESULT STDMETHODCALLTYPE Present(const RECT*, const RECT*, HWND, const RGNDATA*, DWORD) override;
    HRESULT STDMETHODCALLTYPE GetFrontBufferData(IDirect3DSurface9*) override { return D3DERR_INVALIDCALL; }
    HRESULT STDMETHODCALLTYPE GetBackBuffer(UINT, D3DBACKBUFFER_TYPE, IDirect3DSurface9** out) override;
    HRESULT STDMETHODCALLTYPE GetRasterStatus(D3DRASTER_STATUS* s) override { if (s) *s = {FALSE, 0}; return S_OK; }
    HRESULT STDMETHODCALLTYPE GetDisplayMode(D3DDISPLAYMODE* m) override;
    HRESULT STDMETHODCALLTYPE GetDevice(IDirect3DDevice9** out) override;
    HRESULT STDMETHODCALLTYPE GetPresentParameters(D3DPRESENT_PARAMETERS* pp) override;
};

// ---- the device ---------------------------------------------------------------------------
struct StreamSource { Ref<VertexBuffer> vb; UINT offset = 0, stride = 0; };
struct SamplerState {
    DWORD v[14] = {0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0};
    GLuint gl = 0;
    bool dirty = true;
    GLint sent[4] = {-1, -1, -1, -1};  // wrap s / t, mag, min last set on the sampler object (-1: never)
    void param(int i, GLenum pname, GLint value) { if (sent[i] != value) { glSamplerParameteri(gl, pname, value); sent[i] = value; } }
};
struct ProgramKey {
    uint32_t vs = 0;   // 0: fixed function (pre-transformed)
    uint32_t ps = 0;   // 0: fixed function
    FfpPixelKey ffp;
    bool alphaTest = false;  // the fragment shader carries the alpha test (and so may discard)
    bool operator==(const ProgramKey&) const = default;
};
struct ProgramKeyHash { size_t operator()(const ProgramKey& k) const { return (static_cast<size_t>(k.vs) * 131) ^ k.ps ^ (k.ffp.stage[0].colorOp << 8) ^ (k.ffp.stage[0].colorArg2 << 12) ^ (k.ffp.stage[0].alphaOp << 16) ^ (k.ffp.specular << 20) ^ (k.ffp.stage[1].colorOp << 22) ^ (static_cast<size_t>(k.alphaTest) << 30); } };
struct Program {
    GLuint gl = 0;
    GLint uC = -1, uColorBgra = -1, uPc = -1, uBump = -1, uProjMask = -1, uFogColor = -1, uFogEnable = -1, uAlphaFunc = -1, uAlphaRef = -1, uViewport = -1, uTFactor = -1, uHalfPixel = -1;
    GLint uS[6] = {-1, -1, -1, -1, -1, -1};
    GLint uCElem[128] = {};  // locations of c[k] / pc[k] (ranges of constants upload from them)
    GLint uPcElem[8] = {};
    // What this program object last received. GL keeps uniform values per program, so a draw
    // sends only what differs from that: the game toggles blend / z-write / colour mask around
    // most draws, which used to re-send every state-mirroring uniform (~9 calls a draw), and a
    // program switch used to re-send all 128 constant registers.
    uint64_t vsSeen[128] = {}, psSeen[8] = {};  // register versions (Device::vsVer / psVer); 0 = the link-time zero
    uint64_t constGen = 0;                       // Device::constGen when the registers were last synced
    uint64_t miscGen = 0;                        // Device::miscGen when the mirrored states were last compared
    struct Misc {
        float bump[6][4];
        float fogColor[4], tFactor[4], halfPixel[2], viewport[4], alphaRef;
        int projMask, fogEnable, alphaFunc;
    } misc{};
    bool miscKnown = false;  // misc holds what was sent (false: nothing sent yet)
    int colorBgra = -1;      // uColorBgra last sent
};

struct Device : Unknown<IDirect3DDevice9, &IID_IDirect3DDevice9> {
    IDirect3D9* d3d;
    D3DPRESENT_PARAMETERS pp{};
    SDL_Window* window = nullptr;
    SDL_GLContext context = nullptr;
    int fbWidth = 0, fbHeight = 0;  // the framebuffer EGL surface (w32::fbdevGl())
    GLuint overlayProgram = 0, overlayTexture = 0, overlayVbo = 0;
    GLint overlayUSize = -1;
    int overlayVsync = -1;
    void drawOverlay();
    void afterDraw();
    bool skipDraw();
    bool dumpFrameWanted();
    void saveScreenshot();
    void setSwapInterval(int i) { gq::call([i] { if (w32::fbdevGl()) fb::setSwapInterval(i); else SDL_GL_SetSwapInterval(i); }); }
    bool es32 = false;
    // the back buffer
    GLuint mainFbo = 0, mainColor = 0, mainDepth = 0;
    // anti-aliasing: the scene is drawn into multisampled renderbuffers and resolved into
    // mainColor when something reads the back buffer (StretchRect, readback, Present)
    GLuint msaaFbo = 0, msaaColor = 0, msaaDepth = 0;
    int msaaSamples = 0;
    bool msaaDirty = false;
    void resolveMain();
    GLuint readFboFor(Surface* src);
    // brightness: the present pass applies a gamma exponent instead of the plain blit
    GLuint presentProgram = 0;
    GLint presentUGamma = -1, presentUSize = -1;
    bool presentPass(int ww, int wh, float gamma);
    Ref<Surface> backBuffer, mainDepthSurface;
    Ref<SwapChain> swapChain;
    // render targets
    Ref<Surface> renderTarget, depthStencil;
    std::map<std::pair<Surface*, Surface*>, GLuint> fbos;
    GLuint boundFbo = ~0u;
    // state
    DWORD rs[256] = {};
    DWORD tss[8][33] = {};
    SamplerState samplers[8];
    Ref<Texture> textures[8];
    bool texDirty = true;
    float vsConst[128][4] = {};
    float psConst[8][4] = {};
    // Constant registers carry a version (the constGen of their last change in value); each
    // program records the versions it was sent (Program::vsSeen). An unchanged value re-set by
    // the game keeps its version, so it is not sent again.
    uint64_t vsVer[128] = {}, psVer[8] = {};
    uint64_t constGen = 1;  // bumped whenever a register's value changes
    int vsHi = 0;           // 1 + the highest vertex register ever changed (the rest are still the link-time zero)
    uint64_t miscGen = 1;   // bumped when a state the mirroring uniforms read (fog, alpha test, texture factor, bump, projection flags, viewport) changes
    void miscChanged() { ++miscGen; }
    static void syncRegs(const GLint* loc, const float (*val)[4], const uint64_t* ver, uint64_t* seen, int hi);
    GLuint lastProgramGl = 0;
    GLuint boundTexGl[8] = {};        // texture bound to each unit
    int activeUnit = 0;               // the unit glActiveTexture last selected
    // Binds `t` on the active unit for a creation, an upload or a mip generation. That unit then
    // no longer holds what the draws bound there; without recording it, the next draw that keeps
    // the same texture on that unit sampled the uploaded one instead (applySamplers saw no change).
    void bindForUpdate(GLuint t) {
        if (boundTexGl[activeUnit] == t) return;  // already bound there
        glBindTexture(GL_TEXTURE_2D, t);
        boundTexGl[activeUnit] = t;
        texDirty = true;
    }
    bool samplersBound = false, anySamplerDirty = true;
    struct LayoutKey {
        const VertexDecl* decl = nullptr; const VertexShader* vs = nullptr; GLuint program = 0;
        bool up = false; UINT upStride = 0; INT baseVertex = 0;
        GLuint vb[8] = {}; UINT off[8] = {}, stride[8] = {};
    } lastLayout;
    bool layoutValid = false;
    struct GlShadow {  // the GL state last issued; -1 / ~0 = unknown (every field is re-issued after reset())
        GLint vx, vy, vw, vh, sx, sy, sw, sh;
        float zn, zf, slope, bias;
        int scissor, depth, depthMask, blend, stencil, polyOff, cull;  // tri-state
        GLenum depthFunc, sb, db, bop, sfunc, sfail, szfail, spass;
        DWORD blendColor, colorMask;
        GLint sref;
        GLuint smask, swmask;
        bool valid = false;
        void reset() {
            vx = vy = vw = vh = sx = sy = sw = sh = -1;
            zn = zf = slope = bias = -1e30f;
            scissor = depth = depthMask = blend = stencil = polyOff = cull = -1;
            depthFunc = sb = db = bop = sfunc = sfail = szfail = spass = ~0u;
            blendColor = colorMask = ~0u;
            sref = -1;
            smask = swmask = ~0u;
            valid = true;
        }
    } glShadow;
    void glStateUnknown() { rsDirty = true; glShadow.valid = false; }  // after a pass that set GL state directly
    // Vertex array objects, one per layout key (declaration, shader, buffers, offsets): a draw
    // then binds one object instead of re-issuing every attribute pointer. The element buffer
    // binding is part of the object, so it is tracked per object.
    struct LayoutKeyHash { size_t operator()(const LayoutKey& k) const { return static_cast<size_t>(hashBytes(&k, sizeof k)); } };
    struct LayoutKeyEq { bool operator()(const LayoutKey& a, const LayoutKey& b) const { return std::memcmp(&a, &b, sizeof a) == 0; } };
    struct VaoEntry { GLuint vao = 0, ibo = 0; int colorMask = 0; };
    std::unordered_map<LayoutKey, VaoEntry, LayoutKeyHash, LayoutKeyEq> vaos;
    VaoEntry* curVao = nullptr;
    GLuint overlayVao = 0;  // the overlay and the present pass set attribute pointers on this one
    void dropVaos(GLuint buffer, const void* pointer);
    uint64_t lastPresentNs = 0;
    double frameMs = 0, glMs = 0, swapMs = 0, uploadMs = 0, bufKb = 0;
    static constexpr unsigned kPerfWindow = 240;  // frames between two log lines (120) and some slack
    float perfFrames[kPerfWindow] = {};
    unsigned perfN = 0;
    double perfDraws = 0, perfGlMs = 0, perfSwapMs = 0, perfWaitMs = 0, perfRenderMs = 0;
    unsigned bufUploads = 0, bufInPlace = 0;  // the last frame's buffer uploads (g_bufUploads)
    Surface* lastFboRt = nullptr; Surface* lastFboDs = nullptr; GLuint lastFboId = 0; bool lastFboValid = false;
    bool programDirty = true;  // vs/ps/stage states changed since program() last ran
    GLuint boundIbo = ~0u;
    Ref<VertexShader> vs;
    Ref<PixelShader> ps;
    Ref<VertexDecl> decl;
    DWORD fvf = 0;
    std::unordered_map<DWORD, Ref<VertexDecl>> fvfDecls;  // the declaration built from each FVF seen (its VAOs stay cached)
    StreamSource streams[8];
    Ref<IndexBuffer> indices;
    D3DVIEWPORT9 viewport{};
    RECT scissor{};
    D3DMATERIAL9 material{};
    D3DMATRIX transforms[8]{};
    bool rsDirty = true;
    // programs
    std::unordered_map<ProgramKey, Program, ProgramKeyHash> programs;
    Program* current = nullptr;
    uint32_t nextShaderId = 1;
    GLuint ffpVs = 0;
    GLuint scratchVbo = 0, scratchIbo = 0;
    uint32_t frames = 0;
    uint32_t statDraws = 0, statClears = 0;
    bool inScene = false;

    Device(IDirect3D9* d, const D3DPRESENT_PARAMETERS& p) : d3d(d), pp(p) { d3d->AddRef(); }
    ~Device() override;
    bool init(HWND hwnd);
    bool createBackBuffer();
    void destroyBackBuffer();
    GLuint fboFor(Surface* rt, Surface* ds);
    void bindTarget();
    void applyRenderStates();
    void applySamplers();
    Program* program();
    bool bindVertexLayout(const VertexDecl* d, bool up, const void* upData, UINT upStride, UINT baseVertex, INT baseVertexOffset);
    int specifyAttributes(const VertexDecl* d, bool up, UINT upStride, INT baseVertex);
    void prepareDraw(bool up, const void* upData, UINT upStride, UINT upVertices, INT baseVertex);
    VertexDecl* fvfDeclaration();
    GLuint compileShader(GLenum type, const std::string& src, const char* what);
    GLuint linkProgram(GLuint v, GLuint f);
    void setConstants(Program* p);
    void setColorBgra(int mask);

    // IDirect3DDevice9 (the methods are grouped below; declarations generated by gen_fwd.py would
    // match these signatures)
    HRESULT STDMETHODCALLTYPE TestCooperativeLevel() override { return S_OK; }
    UINT STDMETHODCALLTYPE GetAvailableTextureMem() override { return 512u << 20; }
    HRESULT STDMETHODCALLTYPE EvictManagedResources() override { return S_OK; }
    HRESULT STDMETHODCALLTYPE GetDirect3D(IDirect3D9** out) override { if (!out) return E_POINTER; d3d->AddRef(); *out = d3d; return S_OK; }
    HRESULT STDMETHODCALLTYPE GetDeviceCaps(D3DCAPS9* caps) override;
    HRESULT STDMETHODCALLTYPE GetDisplayMode(UINT, D3DDISPLAYMODE* m) override { if (!m) return E_POINTER; *m = {pp.BackBufferWidth, pp.BackBufferHeight, 60, D3DFMT_X8R8G8B8}; return S_OK; }
    HRESULT STDMETHODCALLTYPE GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS* p) override { if (!p) return E_POINTER; *p = {0, D3DDEVTYPE_HAL, pp.hDeviceWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING}; return S_OK; }
    HRESULT STDMETHODCALLTYPE SetCursorProperties(UINT, UINT, IDirect3DSurface9*) override { return S_OK; }
    void STDMETHODCALLTYPE SetCursorPosition(int, int, DWORD) override {}
    WINBOOL STDMETHODCALLTYPE ShowCursor(WINBOOL) override { return FALSE; }
    HRESULT STDMETHODCALLTYPE CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS*, IDirect3DSwapChain9**) override { return D3DERR_NOTAVAILABLE; }
    HRESULT STDMETHODCALLTYPE GetSwapChain(UINT i, IDirect3DSwapChain9** out) override { if (i || !out) return D3DERR_INVALIDCALL; swapChain->AddRef(); *out = swapChain; return S_OK; }
    UINT STDMETHODCALLTYPE GetNumberOfSwapChains() override { return 1; }
    HRESULT STDMETHODCALLTYPE Reset(D3DPRESENT_PARAMETERS* p) override;
    HRESULT STDMETHODCALLTYPE Present(const RECT*, const RECT*, HWND, const RGNDATA*) override;
    HRESULT STDMETHODCALLTYPE GetBackBuffer(UINT, UINT, D3DBACKBUFFER_TYPE, IDirect3DSurface9** out) override { if (!out) return E_POINTER; backBuffer->AddRef(); *out = backBuffer; return S_OK; }
    HRESULT STDMETHODCALLTYPE GetRasterStatus(UINT, D3DRASTER_STATUS* s) override { if (s) *s = {FALSE, 0}; return S_OK; }
    HRESULT STDMETHODCALLTYPE SetDialogBoxMode(WINBOOL) override { return S_OK; }
    void STDMETHODCALLTYPE SetGammaRamp(UINT, DWORD, const D3DGAMMARAMP*) override {}
    void STDMETHODCALLTYPE GetGammaRamp(UINT, D3DGAMMARAMP* r) override { if (r) for (int i = 0; i < 256; ++i) r->red[i] = r->green[i] = r->blue[i] = static_cast<WORD>(i * 257); }
    HRESULT STDMETHODCALLTYPE CreateTexture(UINT w, UINT h, UINT levels, DWORD usage, D3DFORMAT f, D3DPOOL pool, IDirect3DTexture9** out, HANDLE*) override;
    HRESULT STDMETHODCALLTYPE CreateVolumeTexture(UINT, UINT, UINT, UINT, DWORD, D3DFORMAT, D3DPOOL, IDirect3DVolumeTexture9**, HANDLE*) override { return D3DERR_NOTAVAILABLE; }
    HRESULT STDMETHODCALLTYPE CreateCubeTexture(UINT, UINT, DWORD, D3DFORMAT, D3DPOOL, IDirect3DCubeTexture9**, HANDLE*) override { return D3DERR_NOTAVAILABLE; }
    HRESULT STDMETHODCALLTYPE CreateVertexBuffer(UINT n, DWORD usage, DWORD f, D3DPOOL pool, IDirect3DVertexBuffer9** out, HANDLE*) override { if (!out) return E_POINTER; *out = new VertexBuffer(this, n, usage, f, pool); return S_OK; }
    HRESULT STDMETHODCALLTYPE CreateIndexBuffer(UINT n, DWORD usage, D3DFORMAT f, D3DPOOL pool, IDirect3DIndexBuffer9** out, HANDLE*) override { if (!out) return E_POINTER; *out = new IndexBuffer(this, n, usage, f, pool); return S_OK; }
    HRESULT STDMETHODCALLTYPE CreateRenderTarget(UINT w, UINT h, D3DFORMAT f, D3DMULTISAMPLE_TYPE, DWORD, WINBOOL, IDirect3DSurface9** out, HANDLE*) override;
    HRESULT STDMETHODCALLTYPE CreateDepthStencilSurface(UINT w, UINT h, D3DFORMAT f, D3DMULTISAMPLE_TYPE, DWORD, WINBOOL, IDirect3DSurface9** out, HANDLE*) override;
    HRESULT STDMETHODCALLTYPE UpdateSurface(IDirect3DSurface9*, const RECT*, IDirect3DSurface9*, const POINT*) override;
    HRESULT STDMETHODCALLTYPE UpdateTexture(IDirect3DBaseTexture9*, IDirect3DBaseTexture9*) override;
    HRESULT STDMETHODCALLTYPE GetRenderTargetData(IDirect3DSurface9*, IDirect3DSurface9*) override;
    HRESULT STDMETHODCALLTYPE GetFrontBufferData(UINT, IDirect3DSurface9*) override { return D3DERR_INVALIDCALL; }
    HRESULT STDMETHODCALLTYPE StretchRect(IDirect3DSurface9*, const RECT*, IDirect3DSurface9*, const RECT*, D3DTEXTUREFILTERTYPE) override;
    HRESULT STDMETHODCALLTYPE ColorFill(IDirect3DSurface9*, const RECT*, D3DCOLOR) override;
    HRESULT STDMETHODCALLTYPE CreateOffscreenPlainSurface(UINT w, UINT h, D3DFORMAT f, D3DPOOL pool, IDirect3DSurface9** out, HANDLE*) override;
    HRESULT STDMETHODCALLTYPE SetRenderTarget(DWORD i, IDirect3DSurface9* s) override;
    HRESULT STDMETHODCALLTYPE GetRenderTarget(DWORD i, IDirect3DSurface9** out) override { if (i || !out) return D3DERR_INVALIDCALL; if (renderTarget) renderTarget->AddRef(); *out = renderTarget; return renderTarget ? S_OK : D3DERR_NOTFOUND; }
    HRESULT STDMETHODCALLTYPE SetDepthStencilSurface(IDirect3DSurface9* s) override;
    HRESULT STDMETHODCALLTYPE GetDepthStencilSurface(IDirect3DSurface9** out) override { if (!out) return E_POINTER; if (depthStencil) depthStencil->AddRef(); *out = depthStencil; return depthStencil ? S_OK : D3DERR_NOTFOUND; }
    HRESULT STDMETHODCALLTYPE BeginScene() override { inScene = true; return S_OK; }
    HRESULT STDMETHODCALLTYPE EndScene() override { inScene = false; return S_OK; }
    HRESULT STDMETHODCALLTYPE Clear(DWORD n, const D3DRECT* rects, DWORD flags, D3DCOLOR color, float z, DWORD stencil) override;
    HRESULT STDMETHODCALLTYPE SetTransform(D3DTRANSFORMSTATETYPE s, const D3DMATRIX* m) override { if (m && s < 8) transforms[s] = *m; return S_OK; }
    HRESULT STDMETHODCALLTYPE GetTransform(D3DTRANSFORMSTATETYPE s, D3DMATRIX* m) override { if (m && s < 8) *m = transforms[s]; return S_OK; }
    HRESULT STDMETHODCALLTYPE MultiplyTransform(D3DTRANSFORMSTATETYPE, const D3DMATRIX*) override { return S_OK; }
    HRESULT STDMETHODCALLTYPE SetViewport(const D3DVIEWPORT9* v) override { if (v) viewport = *v; rsDirty = true; miscChanged(); return S_OK; }
    HRESULT STDMETHODCALLTYPE GetViewport(D3DVIEWPORT9* v) override { if (v) *v = viewport; return S_OK; }
    HRESULT STDMETHODCALLTYPE SetMaterial(const D3DMATERIAL9* m) override { if (m) material = *m; return S_OK; }
    HRESULT STDMETHODCALLTYPE GetMaterial(D3DMATERIAL9* m) override { if (m) *m = material; return S_OK; }
    HRESULT STDMETHODCALLTYPE SetLight(DWORD, const D3DLIGHT9*) override { return S_OK; }
    HRESULT STDMETHODCALLTYPE GetLight(DWORD, D3DLIGHT9*) override { return D3DERR_INVALIDCALL; }
    HRESULT STDMETHODCALLTYPE LightEnable(DWORD, WINBOOL) override { return S_OK; }
    HRESULT STDMETHODCALLTYPE GetLightEnable(DWORD, WINBOOL* e) override { if (e) *e = FALSE; return S_OK; }
    HRESULT STDMETHODCALLTYPE SetClipPlane(DWORD, const float*) override { return S_OK; }
    HRESULT STDMETHODCALLTYPE GetClipPlane(DWORD, float*) override { return D3DERR_INVALIDCALL; }
    HRESULT STDMETHODCALLTYPE SetRenderState(D3DRENDERSTATETYPE s, DWORD v) override {
        if (s < 256 && rs[s] != v) {
            rs[s] = v;
            rsDirty = true;
            if (s == D3DRS_SPECULARENABLE || s == D3DRS_ALPHATESTENABLE || s == D3DRS_ALPHAFUNC) programDirty = true;  // the program key reads them
            // the states setConstants mirrors into uniforms (the rest are GL state: applyRenderStates)
            if (s == D3DRS_FOGCOLOR || s == D3DRS_FOGENABLE || s == D3DRS_ALPHATESTENABLE || s == D3DRS_ALPHAFUNC || s == D3DRS_ALPHAREF || s == D3DRS_TEXTUREFACTOR) miscChanged();
        }
        return S_OK;
    }
    HRESULT STDMETHODCALLTYPE GetRenderState(D3DRENDERSTATETYPE s, DWORD* v) override { if (!v) return E_POINTER; *v = s < 256 ? rs[s] : 0; return S_OK; }
    HRESULT STDMETHODCALLTYPE CreateStateBlock(D3DSTATEBLOCKTYPE, IDirect3DStateBlock9**) override { return D3DERR_NOTAVAILABLE; }
    HRESULT STDMETHODCALLTYPE BeginStateBlock() override { return D3DERR_NOTAVAILABLE; }
    HRESULT STDMETHODCALLTYPE EndStateBlock(IDirect3DStateBlock9**) override { return D3DERR_NOTAVAILABLE; }
    HRESULT STDMETHODCALLTYPE SetClipStatus(const D3DCLIPSTATUS9*) override { return S_OK; }
    HRESULT STDMETHODCALLTYPE GetClipStatus(D3DCLIPSTATUS9* s) override { if (s) *s = {0, 0}; return S_OK; }
    HRESULT STDMETHODCALLTYPE GetTexture(DWORD st, IDirect3DBaseTexture9** out) override { if (!out || st >= 8) return D3DERR_INVALIDCALL; if (textures[st]) textures[st]->AddRef(); *out = textures[st]; return S_OK; }
    HRESULT STDMETHODCALLTYPE SetTexture(DWORD st, IDirect3DBaseTexture9* t) override;
    HRESULT STDMETHODCALLTYPE GetTextureStageState(DWORD st, D3DTEXTURESTAGESTATETYPE t, DWORD* v) override { if (!v || st >= 8 || t > 32) return D3DERR_INVALIDCALL; *v = tss[st][t]; return S_OK; }
    HRESULT STDMETHODCALLTYPE SetTextureStageState(DWORD st, D3DTEXTURESTAGESTATETYPE t, DWORD v) override { if (st < 8 && t <= 32 && tss[st][t] != v) { tss[st][t] = v; miscChanged(); programDirty = true; } return S_OK; }
    HRESULT STDMETHODCALLTYPE GetSamplerState(DWORD s, D3DSAMPLERSTATETYPE t, DWORD* v) override { if (!v || s >= 8 || t > 13) return D3DERR_INVALIDCALL; *v = samplers[s].v[t]; return S_OK; }
    HRESULT STDMETHODCALLTYPE SetSamplerState(DWORD s, D3DSAMPLERSTATETYPE t, DWORD v) override { if (s < 8 && t <= 13 && samplers[s].v[t] != v) { samplers[s].v[t] = v; samplers[s].dirty = true; anySamplerDirty = true; } return S_OK; }
    HRESULT STDMETHODCALLTYPE ValidateDevice(DWORD* n) override { if (n) *n = 1; return S_OK; }
    HRESULT STDMETHODCALLTYPE SetPaletteEntries(UINT, const PALETTEENTRY*) override { return S_OK; }
    HRESULT STDMETHODCALLTYPE GetPaletteEntries(UINT, PALETTEENTRY*) override { return D3DERR_INVALIDCALL; }
    HRESULT STDMETHODCALLTYPE SetCurrentTexturePalette(UINT) override { return S_OK; }
    HRESULT STDMETHODCALLTYPE GetCurrentTexturePalette(UINT* n) override { if (n) *n = 0; return S_OK; }
    HRESULT STDMETHODCALLTYPE SetScissorRect(const RECT* r) override { if (r) scissor = *r; rsDirty = true; return S_OK; }
    HRESULT STDMETHODCALLTYPE GetScissorRect(RECT* r) override { if (r) *r = scissor; return S_OK; }
    HRESULT STDMETHODCALLTYPE SetSoftwareVertexProcessing(WINBOOL) override { return S_OK; }
    WINBOOL STDMETHODCALLTYPE GetSoftwareVertexProcessing() override { return FALSE; }
    HRESULT STDMETHODCALLTYPE SetNPatchMode(float) override { return S_OK; }
    float STDMETHODCALLTYPE GetNPatchMode() override { return 0; }
    HRESULT STDMETHODCALLTYPE DrawPrimitive(D3DPRIMITIVETYPE t, UINT start, UINT n) override;
    HRESULT STDMETHODCALLTYPE DrawIndexedPrimitive(D3DPRIMITIVETYPE t, INT base, UINT minIdx, UINT numV, UINT start, UINT n) override;
    HRESULT STDMETHODCALLTYPE DrawPrimitiveUP(D3DPRIMITIVETYPE t, UINT n, const void* data, UINT stride) override;
    HRESULT STDMETHODCALLTYPE DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE t, UINT minIdx, UINT numV, UINT n, const void* idx, D3DFORMAT fmt, const void* data, UINT stride) override;
    HRESULT STDMETHODCALLTYPE ProcessVertices(UINT, UINT, UINT, IDirect3DVertexBuffer9*, IDirect3DVertexDeclaration9*, DWORD) override { return D3DERR_NOTAVAILABLE; }
    HRESULT STDMETHODCALLTYPE CreateVertexDeclaration(const D3DVERTEXELEMENT9* el, IDirect3DVertexDeclaration9** out) override { if (!out || !el) return E_POINTER; *out = new VertexDecl(this, el); return S_OK; }
    HRESULT STDMETHODCALLTYPE SetVertexDeclaration(IDirect3DVertexDeclaration9* d) override { decl = static_cast<VertexDecl*>(d); if (d) fvf = 0; programDirty = true; return S_OK; }
    HRESULT STDMETHODCALLTYPE GetVertexDeclaration(IDirect3DVertexDeclaration9** out) override { if (!out) return E_POINTER; if (decl) decl->AddRef(); *out = decl; return S_OK; }
    HRESULT STDMETHODCALLTYPE SetFVF(DWORD f) override { if (f && (f != fvf || decl)) { fvf = f; decl = nullptr; programDirty = true; } return S_OK; }
    HRESULT STDMETHODCALLTYPE GetFVF(DWORD* f) override { if (f) *f = fvf; return S_OK; }
    HRESULT STDMETHODCALLTYPE CreateVertexShader(const DWORD* code, IDirect3DVertexShader9** out) override;
    HRESULT STDMETHODCALLTYPE SetVertexShader(IDirect3DVertexShader9* s) override { if (vs.p != s) programDirty = true; vs = static_cast<VertexShader*>(s); return S_OK; }
    HRESULT STDMETHODCALLTYPE GetVertexShader(IDirect3DVertexShader9** out) override { if (!out) return E_POINTER; if (vs) vs->AddRef(); *out = vs; return S_OK; }
    // Stores the registers whose value changed and stamps them with a new version; returns the
    // end of the last changed register (0: nothing changed).
    UINT setRegs(float (*dst)[4], uint64_t* ver, UINT r, const float* d, UINT n) {
        UINT end = 0;
        bool bumped = false;
        for (UINT k = 0; k < n; ++k) {
            if (std::memcmp(dst[r + k], d + 4 * k, 16) == 0) continue;  // the game re-sets identical values (view matrices, light terms) every draw
            if (!bumped) ++constGen, bumped = true;
            std::memcpy(dst[r + k], d + 4 * k, 16);
            ver[r + k] = constGen;
            end = r + k + 1;
        }
        return end;
    }
    HRESULT STDMETHODCALLTYPE SetVertexShaderConstantF(UINT r, const float* d, UINT n) override {
        if (r > 128 || n > 128 - r) return D3DERR_INVALIDCALL;
        if (!d) return n ? D3DERR_INVALIDCALL : S_OK;
        const UINT end = setRegs(vsConst, vsVer, r, d, n);
        vsHi = std::max<int>(vsHi, static_cast<int>(end));
        return S_OK;
    }
    HRESULT STDMETHODCALLTYPE GetVertexShaderConstantF(UINT r, float* d, UINT n) override { if (r + n > 128) return D3DERR_INVALIDCALL; std::memcpy(d, vsConst[r], n * 16); return S_OK; }
    HRESULT STDMETHODCALLTYPE SetVertexShaderConstantI(UINT, const int*, UINT) override { return S_OK; }
    HRESULT STDMETHODCALLTYPE GetVertexShaderConstantI(UINT, int*, UINT) override { return D3DERR_INVALIDCALL; }
    HRESULT STDMETHODCALLTYPE SetVertexShaderConstantB(UINT, const WINBOOL*, UINT) override { return S_OK; }
    HRESULT STDMETHODCALLTYPE GetVertexShaderConstantB(UINT, WINBOOL*, UINT) override { return D3DERR_INVALIDCALL; }
    HRESULT STDMETHODCALLTYPE SetStreamSource(UINT i, IDirect3DVertexBuffer9* vb, UINT off, UINT stride) override { if (i >= 8) return D3DERR_INVALIDCALL; streams[i].vb = static_cast<VertexBuffer*>(vb); streams[i].offset = off; streams[i].stride = stride; return S_OK; }
    HRESULT STDMETHODCALLTYPE GetStreamSource(UINT i, IDirect3DVertexBuffer9** vb, UINT* off, UINT* stride) override { if (i >= 8) return D3DERR_INVALIDCALL; if (vb) { if (streams[i].vb) streams[i].vb->AddRef(); *vb = streams[i].vb; } if (off) *off = streams[i].offset; if (stride) *stride = streams[i].stride; return S_OK; }
    HRESULT STDMETHODCALLTYPE SetStreamSourceFreq(UINT, UINT) override { return S_OK; }
    HRESULT STDMETHODCALLTYPE GetStreamSourceFreq(UINT, UINT* f) override { if (f) *f = 1; return S_OK; }
    HRESULT STDMETHODCALLTYPE SetIndices(IDirect3DIndexBuffer9* ib) override { indices = static_cast<IndexBuffer*>(ib); return S_OK; }
    HRESULT STDMETHODCALLTYPE GetIndices(IDirect3DIndexBuffer9** out) override { if (!out) return E_POINTER; if (indices) indices->AddRef(); *out = indices; return S_OK; }
    HRESULT STDMETHODCALLTYPE CreatePixelShader(const DWORD* code, IDirect3DPixelShader9** out) override;
    HRESULT STDMETHODCALLTYPE SetPixelShader(IDirect3DPixelShader9* s) override { if (ps.p != s) programDirty = true; ps = static_cast<PixelShader*>(s); return S_OK; }
    HRESULT STDMETHODCALLTYPE GetPixelShader(IDirect3DPixelShader9** out) override { if (!out) return E_POINTER; if (ps) ps->AddRef(); *out = ps; return S_OK; }
    HRESULT STDMETHODCALLTYPE SetPixelShaderConstantF(UINT r, const float* d, UINT n) override {
        if (r > 8 || n > 8 - r) return D3DERR_INVALIDCALL;
        if (!d) return n ? D3DERR_INVALIDCALL : S_OK;
        setRegs(psConst, psVer, r, d, n);
        return S_OK;
    }
    HRESULT STDMETHODCALLTYPE GetPixelShaderConstantF(UINT r, float* d, UINT n) override { if (r + n > 8) return D3DERR_INVALIDCALL; std::memcpy(d, psConst[r], n * 16); return S_OK; }
    HRESULT STDMETHODCALLTYPE SetPixelShaderConstantI(UINT, const int*, UINT) override { return S_OK; }
    HRESULT STDMETHODCALLTYPE GetPixelShaderConstantI(UINT, int*, UINT) override { return D3DERR_INVALIDCALL; }
    HRESULT STDMETHODCALLTYPE SetPixelShaderConstantB(UINT, const WINBOOL*, UINT) override { return S_OK; }
    HRESULT STDMETHODCALLTYPE GetPixelShaderConstantB(UINT, WINBOOL*, UINT) override { return D3DERR_INVALIDCALL; }
    HRESULT STDMETHODCALLTYPE DrawRectPatch(UINT, const float*, const D3DRECTPATCH_INFO*) override { return D3DERR_NOTAVAILABLE; }
    HRESULT STDMETHODCALLTYPE DrawTriPatch(UINT, const float*, const D3DTRIPATCH_INFO*) override { return D3DERR_NOTAVAILABLE; }
    HRESULT STDMETHODCALLTYPE DeletePatch(UINT) override { return D3DERR_NOTAVAILABLE; }
    HRESULT STDMETHODCALLTYPE CreateQuery(D3DQUERYTYPE t, IDirect3DQuery9** out) override {
        if (t != D3DQUERYTYPE_OCCLUSION && t != D3DQUERYTYPE_EVENT) return D3DERR_NOTAVAILABLE;
        if (out) *out = new Query(this, t);
        return S_OK;
    }
};

// ---- GetDevice for the resources ----
#define GETDEVICE(T) HRESULT STDMETHODCALLTYPE T::GetDevice(IDirect3DDevice9** out) { if (!out) return E_POINTER; dev->AddRef(); *out = dev; return S_OK; }
GETDEVICE(Surface) GETDEVICE(Texture) GETDEVICE(VertexBuffer) GETDEVICE(IndexBuffer) GETDEVICE(VertexDecl) GETDEVICE(VertexShader) GETDEVICE(PixelShader) GETDEVICE(Query) GETDEVICE(SwapChain)

// ---- Surface / Texture ----
Surface::~Surface() { if (rbo) glDeleteRenderbuffers(1, &rbo); if (tex) glDeleteTextures(1, &tex); }
HRESULT STDMETHODCALLTYPE Surface::GetContainer(REFIID riid, void** out) {
    if (!out) return E_POINTER;
    if (texture) return texture->QueryInterface(riid, out);
    return dev->QueryInterface(riid, out);
}
HRESULT STDMETHODCALLTYPE Surface::LockRect(D3DLOCKED_RECT* out, const RECT* rect, DWORD flags) {
    if (!out) return E_POINTER;
    if (level.locked) return D3DERR_INVALIDCALL;
    if (level.cpu.empty()) level.cpu.assign(levelBytes(format, level.width, level.height), 0);
    // A render target's lock reads the GL contents back first: D3D9 refuses the lock, which
    // makes D3DX copy through GetRenderTargetData, but here it succeeded on a zeroed buffer,
    // so the hero's composed face copied into its DXT1 texture as black (the adult hero's
    // head). Only the 32-bit colour formats are read back; the rest keep the CPU copy.
    const bool renderTarget = (usage & D3DUSAGE_RENDERTARGET) || isBackBuffer;
    if (renderTarget && !isMainDepth && (format == D3DFMT_A8R8G8B8 || format == D3DFMT_X8R8G8B8) && !(flags & D3DLOCK_DISCARD)) {
        const GLuint f = dev->readFboFor(this);
        glBindFramebuffer(GL_FRAMEBUFFER, f);
        dev->boundFbo = f;
        std::vector<uint8_t> px(static_cast<size_t>(level.width) * level.height * 4);
        glReadPixels(0, 0, static_cast<GLsizei>(level.width), static_cast<GLsizei>(level.height), GL_RGBA, GL_UNSIGNED_BYTE, px.data());
        for (size_t i = 0; i + 3 < px.size() && i + 3 < level.cpu.size(); i += 4) { level.cpu[i] = px[i + 2]; level.cpu[i + 1] = px[i + 1]; level.cpu[i + 2] = px[i]; level.cpu[i + 3] = px[i + 3]; }
        static bool once;
        if (!once) once = true, log("gles: a render target (%ux%u) was locked: read back from GL", level.width, level.height);
    }
    const UINT pitch = levelPitch(format, level.width);
    level.locked = true;
    level.wholeLock = !rect;
    if (rect) level.lockRect = *rect;
    out->Pitch = static_cast<INT>(pitch);
    const FormatInfo i = formatInfo(format);
    const UINT bx = rect && !i.blockBytes ? rect->left * i.bytesPerPixel : rect ? (rect->left / 4) * i.blockBytes : 0;
    const UINT by = rect ? (i.blockBytes ? rect->top / 4 : rect->top) : 0;
    out->pBits = level.cpu.data() + by * pitch + bx;
    (void)flags;
    return S_OK;
}
HRESULT STDMETHODCALLTYPE Surface::UnlockRect() {
    if (!level.locked) return D3DERR_INVALIDCALL;
    level.locked = false;
    if (texture) texture->upload(mip, level.wholeLock ? nullptr : &level.lockRect);
    return S_OK;
}

Texture::Texture(Device* d, UINT w, UINT h, UINT l, DWORD u, D3DFORMAT f, D3DPOOL p) : dev(d), format(f), pool(p), usage(u), width(w), height(h) {
    if (l == 0) { l = 1; UINT s = std::max(w, h); while (s > 1) s >>= 1, ++l; }
    levels = l;
    info = formatInfo(f);
    surfaces.assign(levels, nullptr);
    if (pool != D3DPOOL_SYSTEMMEM && pool != D3DPOOL_SCRATCH) {
        glGenTextures(1, &gl);
        tex16 = isDxt(f) ? dxt16Mode() : 0;
        initObject(tex16 ? 0 : info.internal);  // tex16: the storage waits for the first level's alpha
        GLCHECK("CreateTexture");
    }
}
void Texture::initObject(GLenum internal) {
    dev->bindForUpdate(gl);
    if (internal) glTexStorage2D(GL_TEXTURE_2D, static_cast<GLsizei>(levels), internal, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, &info.swizzle[0]);
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, &info.swizzle[1]);
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, &info.swizzle[2]);
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, &info.swizzle[3]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(levels - 1));
    if (info.depth) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
}
void Texture::store16(int rank) {
    if (storeRank >= 0) {  // immutable storage keeps its format: a new object, the levels uploaded again
        for (GLuint& b : dev->boundTexGl) if (b == gl) b = ~0u;
        glDeleteTextures(1, &gl);
        glGenTextures(1, &gl);
        initObject(0);
    } else {
        dev->bindForUpdate(gl);
    }
    glTexStorage2D(GL_TEXTURE_2D, static_cast<GLsizei>(levels), kDxtStore[rank].internal, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
    storeRank = rank;
}
Texture::~Texture() {
    for (GLuint& b : dev->boundTexGl) if (b == gl && gl) b = ~0u;
    for (Surface* s : surfaces)
        if (s) { s->texture = nullptr; s->Release(); }
    if (gl) glDeleteTextures(1, &gl);
}
Surface* Texture::surface(UINT level) {
    if (!surfaces[level]) {
        Surface* s = new Surface(dev, format, std::max(1u, width >> level), std::max(1u, height >> level));
        s->texture = this, s->mip = level, s->pool = pool, s->usage = usage;
        surfaces[level] = s;
    }
    return surfaces[level];
}
void Texture::upload(UINT level, const RECT* rect, bool force) {
    if (!gl) return;
    Surface* s = surfaces[level];
    if (!s || s->level.cpu.empty()) return;
    const uint64_t t0 = nowNs();
    if (s->level.cpu.size() <= 65536) {  // skip an identical re-upload of a small level
        const uint64_t h = hashBytes(s->level.cpu.data(), s->level.cpu.size());
        if (h == s->level.hash && !force) return;
        s->level.hash = h;
    }
    struct UploadTime { uint64_t t0; ~UploadTime() { const uint64_t d = nowNs() - t0; g_nsUpload += d; g_nsGl += d; } } timing{t0};
    const UINT w = s->level.width, h = s->level.height;
    if (tex16) {
        std::vector<uint8_t> rgba = decodeDxt(format, s->level.cpu.data(), w, h);
        const int need = dxtNeed(rgba, tex16);
        if (need > storeRank) {
            const bool had = storeRank >= 0;
            store16(need);
            if (had)  // the levels already uploaded, into the new object
                for (UINT l = 0; l < levels; ++l)
                    if (l != level && surfaces[l] && !surfaces[l]->level.cpu.empty()) upload(l, nullptr, true);
        }
        dev->bindForUpdate(gl);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        const DxtStoreFormat& sf = kDxtStore[storeRank];
        if (storeRank == ST_8888) {
            glTexSubImage2D(GL_TEXTURE_2D, static_cast<GLint>(level), 0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h), sf.format, sf.type, rgba.data());
        } else {
            std::vector<uint16_t> packed(static_cast<size_t>(w) * h);
            packDxt16(storeRank, rgba.data(), packed.size(), packed.data());
            glTexSubImage2D(GL_TEXTURE_2D, static_cast<GLint>(level), 0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h), sf.format, sf.type, packed.data());
        }
        (void)rect;
        GLCHECK("upload");
        return;
    }
    dev->bindForUpdate(gl);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    if (isDxt(format)) {
        std::vector<uint8_t> rgba = decodeDxt(format, s->level.cpu.data(), w, h);
        glTexSubImage2D(GL_TEXTURE_2D, static_cast<GLint>(level), 0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h), GL_RGBA, GL_UNSIGNED_BYTE, rgba.data());
    } else if (info.bytesPerPixel == 2 && (format == D3DFMT_A4R4G4B4 || format == D3DFMT_A1R5G5B5 || format == D3DFMT_X1R5G5B5)) {
        std::vector<uint8_t> conv(s->level.cpu.size());
        convert16(format, s->level.cpu.data(), conv.data(), static_cast<size_t>(w) * h);
        glTexSubImage2D(GL_TEXTURE_2D, static_cast<GLint>(level), 0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h), info.format, info.type, conv.data());
    } else if (!info.depth) {
        glTexSubImage2D(GL_TEXTURE_2D, static_cast<GLint>(level), 0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h), info.format, info.type, s->level.cpu.data());
    }
    (void)rect;
    GLCHECK("upload");
}
void STDMETHODCALLTYPE Texture::GenerateMipSubLevels() { if (gl) { dev->bindForUpdate(gl); glGenerateMipmap(GL_TEXTURE_2D); } }

// ---- SwapChain ----
HRESULT STDMETHODCALLTYPE SwapChain::Present(const RECT* a, const RECT* b, HWND h, const RGNDATA* d, DWORD) { return dev->Present(a, b, h, d); }
HRESULT STDMETHODCALLTYPE SwapChain::GetBackBuffer(UINT i, D3DBACKBUFFER_TYPE t, IDirect3DSurface9** out) { return dev->GetBackBuffer(0, i, t, out); }
HRESULT STDMETHODCALLTYPE SwapChain::GetDisplayMode(D3DDISPLAYMODE* m) { return dev->GetDisplayMode(0, m); }
HRESULT STDMETHODCALLTYPE SwapChain::GetPresentParameters(D3DPRESENT_PARAMETERS* p) { if (!p) return E_POINTER; *p = dev->pp; return S_OK; }

// ---- Device: setup ----
Device::~Device() {
    if (gq::enabled()) {
        gq::stop();
        if (w32::fbdevGl()) fb::makeCurrent(); else SDL_GL_MakeCurrent(window, context);
    }
    for (auto& [k, f] : fbos) glDeleteFramebuffers(1, &f);
    for (auto& [k, p] : programs) glDeleteProgram(p.gl);
    destroyBackBuffer();
    if (overlayVao) glDeleteVertexArrays(1, &overlayVao);
    for (auto& [k, e] : vaos) glDeleteVertexArrays(1, &e.vao);
    vaos.clear();
    if (scratchVbo) glDeleteBuffers(1, &scratchVbo);
    if (scratchIbo) glDeleteBuffers(1, &scratchIbo);
    if (ffpVs) glDeleteShader(ffpVs);
    for (auto& s : samplers) if (s.gl) glDeleteSamplers(1, &s.gl);
    if (context) SDL_GL_DeleteContext(context);
    if (w32::fbdevGl()) fb::close();
    d3d->Release();
    if (g_device == this) g_device = nullptr;
}

bool Device::init(HWND hwnd) {
    // win/d3d9.cpp passes the native window (the SDL_Window*), as it does to DXVK.
    window = reinterpret_cast<SDL_Window*>(hwnd ? hwnd : pp.hDeviceWindow);
    if (!window) { log("gles: no SDL window for the device"); return false; }
    if (w32::fbdevGl()) {  // no KMS/DRM: EGL straight on the framebuffer (egl_fbdev.cpp)
        if (!fb::open(fbWidth, fbHeight)) return false;
    } else {
        context = SDL_GL_CreateContext(window);
        if (!context) { log("gles: SDL_GL_CreateContext: %s", SDL_GetError()); return false; }
        SDL_GL_MakeCurrent(window, context);
    }
    if (!loadGl()) { log("gles: the driver lacks an OpenGL ES 3.0 entry point"); return false; }
    {   // the render thread owns the context from here on (every gl* below is a queued wrapper)
        const bool fbdev = w32::fbdevGl();
        if (fbdev) fb::releaseCurrent(); else SDL_GL_MakeCurrent(window, nullptr);
        const bool threaded = gq::start(
            [this, fbdev] { return fbdev ? fb::makeCurrent() : SDL_GL_MakeCurrent(window, context) == 0; },
            [this, fbdev] { if (fbdev) fb::releaseCurrent(); else SDL_GL_MakeCurrent(window, nullptr); });
        if (!threaded) { if (fbdev) fb::makeCurrent(); else SDL_GL_MakeCurrent(window, context); }
    }
    setSwapInterval(pp.PresentationInterval == D3DPRESENT_INTERVAL_IMMEDIATE ? 0 : 1);
    const char* ver = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    log("gles: %s | %s", ver ? ver : "?", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    GLint maj = 0, min = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &maj);
    glGetIntegerv(GL_MINOR_VERSION, &min);
    es32 = maj > 3 || (maj == 3 && min >= 2);
    glGenVertexArrays(1, &overlayVao);
    glBindVertexArray(overlayVao);
    glGenBuffers(1, &scratchVbo);
    glGenBuffers(1, &scratchIbo);
    for (int i = 0; i < 8; ++i) glGenSamplers(1, &samplers[i].gl);
    ffpVs = compileShader(GL_VERTEX_SHADER, ffpVertexShaderRhw(4), "fixed-function vertex shader");
    swapChain = new SwapChain(this);
    swapChain->Release();  // Ref holds it
    // Direct3D defaults
    rs[D3DRS_ZENABLE] = pp.EnableAutoDepthStencil ? D3DZB_TRUE : D3DZB_FALSE;
    rs[D3DRS_FILLMODE] = D3DFILL_SOLID, rs[D3DRS_SHADEMODE] = D3DSHADE_GOURAUD, rs[D3DRS_ZWRITEENABLE] = TRUE;
    rs[D3DRS_SRCBLEND] = D3DBLEND_ONE, rs[D3DRS_DESTBLEND] = D3DBLEND_ZERO, rs[D3DRS_CULLMODE] = D3DCULL_CCW, rs[D3DRS_ZFUNC] = D3DCMP_LESSEQUAL;
    rs[D3DRS_ALPHAFUNC] = D3DCMP_ALWAYS, rs[D3DRS_STENCILFUNC] = D3DCMP_ALWAYS, rs[D3DRS_STENCILMASK] = 0xFFFFFFFF, rs[D3DRS_STENCILWRITEMASK] = 0xFFFFFFFF;
    rs[D3DRS_STENCILFAIL] = rs[D3DRS_STENCILZFAIL] = rs[D3DRS_STENCILPASS] = D3DSTENCILOP_KEEP;
    rs[D3DRS_COLORWRITEENABLE] = 0xF, rs[D3DRS_BLENDOP] = D3DBLENDOP_ADD, rs[D3DRS_LIGHTING] = TRUE, rs[D3DRS_FOGCOLOR] = 0;
    rs[D3DRS_TEXTUREFACTOR] = 0xFFFFFFFF;
    for (int s = 0; s < 8; ++s) {
        tss[s][D3DTSS_COLOROP] = s == 0 ? D3DTOP_MODULATE : D3DTOP_DISABLE;
        tss[s][D3DTSS_COLORARG1] = D3DTA_TEXTURE, tss[s][D3DTSS_COLORARG2] = D3DTA_CURRENT;
        tss[s][D3DTSS_ALPHAOP] = s == 0 ? D3DTOP_SELECTARG1 : D3DTOP_DISABLE;
        tss[s][D3DTSS_ALPHAARG1] = D3DTA_TEXTURE, tss[s][D3DTSS_ALPHAARG2] = D3DTA_CURRENT;
        tss[s][D3DTSS_TEXCOORDINDEX] = static_cast<DWORD>(s);
        tss[s][D3DTSS_BUMPENVLSCALE] = 0, tss[s][D3DTSS_BUMPENVLOFFSET] = 0;
    }
    return createBackBuffer();
}

bool Device::createBackBuffer() {
    const UINT w = std::max(1u, pp.BackBufferWidth), h = std::max(1u, pp.BackBufferHeight);
    glGenTextures(1, &mainColor);
    bindForUpdate(mainColor);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, static_cast<GLsizei>(w), static_cast<GLsizei>(h));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenRenderbuffers(1, &mainDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, mainDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, static_cast<GLsizei>(w), static_cast<GLsizei>(h));
    glGenFramebuffers(1, &mainFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, mainFbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mainColor, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mainDepth);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) { log("gles: back buffer framebuffer incomplete"); return false; }
    msaaSamples = 0, msaaDirty = false;
    if (pp.MultiSampleType >= 2 && real::glRenderbufferStorageMultisample) {
        GLint maxSamples = 0;
        glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
        const int want = std::min<int>(static_cast<int>(pp.MultiSampleType), maxSamples);
        if (want >= 2) {
            glGenRenderbuffers(1, &msaaColor);
            glBindRenderbuffer(GL_RENDERBUFFER, msaaColor);
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, want, GL_RGBA8, static_cast<GLsizei>(w), static_cast<GLsizei>(h));
            glGenRenderbuffers(1, &msaaDepth);
            glBindRenderbuffer(GL_RENDERBUFFER, msaaDepth);
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, want, GL_DEPTH24_STENCIL8, static_cast<GLsizei>(w), static_cast<GLsizei>(h));
            glGenFramebuffers(1, &msaaFbo);
            glBindFramebuffer(GL_FRAMEBUFFER, msaaFbo);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, msaaColor);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, msaaDepth);
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
                msaaSamples = want;
                log("gles: %dx multisampled back buffer (device max %d)", want, maxSamples);
            } else {
                log("gles: multisampled back buffer incomplete, anti-aliasing off");
                glDeleteFramebuffers(1, &msaaFbo), glDeleteRenderbuffers(1, &msaaColor), glDeleteRenderbuffers(1, &msaaDepth);
                msaaFbo = msaaColor = msaaDepth = 0;
            }
            GLCHECK("msaa back buffer");
        }
    } else if (pp.MultiSampleType >= 2) {
        log("gles: no glRenderbufferStorageMultisample, anti-aliasing off");
    }
    backBuffer = new Surface(this, D3DFMT_X8R8G8B8, w, h);
    backBuffer->Release();
    backBuffer->isBackBuffer = true, backBuffer->usage = D3DUSAGE_RENDERTARGET;
    mainDepthSurface = new Surface(this, pp.AutoDepthStencilFormat ? pp.AutoDepthStencilFormat : D3DFMT_D24S8, w, h);
    mainDepthSurface->Release();
    mainDepthSurface->isMainDepth = true, mainDepthSurface->usage = D3DUSAGE_DEPTHSTENCIL;
    renderTarget = backBuffer;
    depthStencil = pp.EnableAutoDepthStencil ? mainDepthSurface : Ref<Surface>();
    viewport = {0, 0, w, h, 0.0f, 1.0f};
    scissor = {0, 0, static_cast<LONG>(w), static_cast<LONG>(h)};
    miscChanged();
    boundFbo = ~0u;
    glStateUnknown();
    return true;
}
void Device::destroyBackBuffer() {
    for (auto& [k, f] : fbos) glDeleteFramebuffers(1, &f);
    fbos.clear();
    if (mainFbo) glDeleteFramebuffers(1, &mainFbo), mainFbo = 0;
    if (mainColor) glDeleteTextures(1, &mainColor), mainColor = 0;
    if (mainDepth) glDeleteRenderbuffers(1, &mainDepth), mainDepth = 0;
    if (msaaFbo) glDeleteFramebuffers(1, &msaaFbo), msaaFbo = 0;
    if (msaaColor) glDeleteRenderbuffers(1, &msaaColor), msaaColor = 0;
    if (msaaDepth) glDeleteRenderbuffers(1, &msaaDepth), msaaDepth = 0;
    msaaSamples = 0, msaaDirty = false;
}

void Device::resolveMain() {
    if (!msaaSamples || !msaaDirty) return;
    glBindFramebuffer(GL_READ_FRAMEBUFFER, msaaFbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mainFbo);
    glDisable(GL_SCISSOR_TEST);
    glBlitFramebuffer(0, 0, static_cast<GLint>(pp.BackBufferWidth), static_cast<GLint>(pp.BackBufferHeight), 0, 0, static_cast<GLint>(pp.BackBufferWidth),
                      static_cast<GLint>(pp.BackBufferHeight), GL_COLOR_BUFFER_BIT, GL_NEAREST);
    msaaDirty = false;
    boundFbo = ~0u;
    glStateUnknown();
}

GLuint Device::readFboFor(Surface* src) {  // a framebuffer to read `src` from (the back buffer: resolved)
    if (src && src->isBackBuffer && msaaSamples) { resolveMain(); return mainFbo; }
    return fboFor(src, nullptr);
}

HRESULT STDMETHODCALLTYPE Device::Reset(D3DPRESENT_PARAMETERS* p) {
    if (!p) return E_POINTER;
    renderTarget = nullptr, depthStencil = nullptr;
    destroyBackBuffer();
    pp = *p;
    setSwapInterval(pp.PresentationInterval == D3DPRESENT_INTERVAL_IMMEDIATE ? 0 : 1);
    return createBackBuffer() ? S_OK : D3DERR_INVALIDCALL;
}

GLuint Device::fboFor(Surface* rt, Surface* ds) {
    if (rt && rt->isBackBuffer && (!ds || ds->isMainDepth)) return msaaSamples ? msaaFbo : mainFbo;
    if (lastFboValid && rt == lastFboRt && ds == lastFboDs) return lastFboId;
    const auto key = std::make_pair(rt, ds);
    auto it = fbos.find(key);
    if (it != fbos.end()) { lastFboRt = rt, lastFboDs = ds, lastFboId = it->second, lastFboValid = true; return it->second; }
    GLuint f = 0;
    glGenFramebuffers(1, &f);
    glBindFramebuffer(GL_FRAMEBUFFER, f);
    if (rt) {
        if (rt->isBackBuffer) glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mainColor, 0);
        else if (rt->texture) glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rt->texture->gl, static_cast<GLint>(rt->mip));
        else if (rt->rbo) glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rt->rbo);
    }
    if (ds) {
        if (ds->isMainDepth) glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mainDepth);
        else if (ds->texture) glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ds->texture->gl, 0);
        else if (ds->rbo) glFramebufferRenderbuffer(GL_FRAMEBUFFER, ds->format == D3DFMT_D16 || ds->format == D3DFMT_D32 ? GL_DEPTH_ATTACHMENT : GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, ds->rbo);
    }
    const GLenum st = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (st != GL_FRAMEBUFFER_COMPLETE) log("gles: framebuffer (rt %p, ds %p) incomplete: 0x%X", static_cast<void*>(rt), static_cast<void*>(ds), st);
    fbos[key] = f;
    return f;
}
void Device::bindTarget() {
    const GLuint f = fboFor(renderTarget, depthStencil);
    if (msaaSamples && f == msaaFbo) msaaDirty = true;
    if (f != boundFbo) { glBindFramebuffer(GL_FRAMEBUFFER, f); boundFbo = f; }
}

HRESULT STDMETHODCALLTYPE Device::SetRenderTarget(DWORD i, IDirect3DSurface9* s) {
    if (i) return D3DERR_INVALIDCALL;
    if (!s) return D3DERR_INVALIDCALL;
    Surface* surf = static_cast<Surface*>(s);
    renderTarget = surf;
    viewport = {0, 0, surf->level.width, surf->level.height, 0.0f, 1.0f};
    scissor = {0, 0, static_cast<LONG>(surf->level.width), static_cast<LONG>(surf->level.height)};
    rsDirty = true;
    miscChanged();  // the viewport (uViewport, uHalfPixel)
    return S_OK;
}
HRESULT STDMETHODCALLTYPE Device::SetDepthStencilSurface(IDirect3DSurface9* s) {
    depthStencil = static_cast<Surface*>(s);
    rsDirty = true;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE Device::CreateTexture(UINT w, UINT h, UINT levels, DWORD usage, D3DFORMAT f, D3DPOOL pool, IDirect3DTexture9** out, HANDLE*) {
    if (!out) return E_POINTER;
    if (!formatInfo(f).valid) { log("gles: CreateTexture: unsupported format %u", f); return D3DERR_INVALIDCALL; }
    *out = new Texture(this, w, h, levels, usage, f, pool);
    return S_OK;
}
HRESULT STDMETHODCALLTYPE Device::CreateRenderTarget(UINT w, UINT h, D3DFORMAT f, D3DMULTISAMPLE_TYPE, DWORD, WINBOOL, IDirect3DSurface9** out, HANDLE*) {
    if (!out) return E_POINTER;
    Surface* s = new Surface(this, f, w, h);
    s->usage = D3DUSAGE_RENDERTARGET;
    glGenRenderbuffers(1, &s->rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, s->rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, static_cast<GLsizei>(w), static_cast<GLsizei>(h));
    *out = s;
    return S_OK;
}
HRESULT STDMETHODCALLTYPE Device::CreateDepthStencilSurface(UINT w, UINT h, D3DFORMAT f, D3DMULTISAMPLE_TYPE, DWORD, WINBOOL, IDirect3DSurface9** out, HANDLE*) {
    if (!out) return E_POINTER;
    Surface* s = new Surface(this, f, w, h);
    s->usage = D3DUSAGE_DEPTHSTENCIL;
    glGenRenderbuffers(1, &s->rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, s->rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, f == D3DFMT_D16 ? GL_DEPTH_COMPONENT16 : GL_DEPTH24_STENCIL8, static_cast<GLsizei>(w), static_cast<GLsizei>(h));
    *out = s;
    return S_OK;
}
HRESULT STDMETHODCALLTYPE Device::CreateOffscreenPlainSurface(UINT w, UINT h, D3DFORMAT f, D3DPOOL pool, IDirect3DSurface9** out, HANDLE*) {
    if (!out) return E_POINTER;
    Surface* s = new Surface(this, f, w, h);
    s->pool = pool;
    *out = s;
    return S_OK;
}
HRESULT STDMETHODCALLTYPE Device::UpdateSurface(IDirect3DSurface9* src, const RECT*, IDirect3DSurface9* dst, const POINT*) {
    Surface* a = static_cast<Surface*>(src);
    Surface* b = static_cast<Surface*>(dst);
    if (!a || !b || a->format != b->format || a->level.cpu.empty()) return D3DERR_INVALIDCALL;
    b->level.cpu = a->level.cpu;
    if (b->texture) b->texture->upload(b->mip, nullptr);
    return S_OK;
}
HRESULT STDMETHODCALLTYPE Device::UpdateTexture(IDirect3DBaseTexture9* src, IDirect3DBaseTexture9* dst) {
    Texture* a = static_cast<Texture*>(src);
    Texture* b = static_cast<Texture*>(dst);
    if (!a || !b) return D3DERR_INVALIDCALL;
    for (UINT l = 0; l < std::min(a->levels, b->levels); ++l) {
        Surface* sa = a->surfaces[l];
        if (!sa || sa->level.cpu.empty()) continue;
        b->surface(l)->level.cpu = sa->level.cpu;
        b->upload(l, nullptr);
    }
    return S_OK;
}
HRESULT STDMETHODCALLTYPE Device::GetRenderTargetData(IDirect3DSurface9* src, IDirect3DSurface9* dst) {
    Surface* a = static_cast<Surface*>(src);
    Surface* b = static_cast<Surface*>(dst);
    if (!a || !b) return D3DERR_INVALIDCALL;
    const GLuint f = readFboFor(a);
    glBindFramebuffer(GL_FRAMEBUFFER, f);
    boundFbo = f;
    std::vector<uint8_t> px(static_cast<size_t>(a->level.width) * a->level.height * 4);
    glReadPixels(0, 0, static_cast<GLsizei>(a->level.width), static_cast<GLsizei>(a->level.height), GL_RGBA, GL_UNSIGNED_BYTE, px.data());
    b->level.cpu.resize(px.size());
    for (size_t i = 0; i < px.size(); i += 4) { b->level.cpu[i] = px[i + 2]; b->level.cpu[i + 1] = px[i + 1]; b->level.cpu[i + 2] = px[i]; b->level.cpu[i + 3] = px[i + 3]; }  // to BGRA
    return S_OK;
}
HRESULT STDMETHODCALLTYPE Device::StretchRect(IDirect3DSurface9* src, const RECT* sr, IDirect3DSurface9* dst, const RECT* dr, D3DTEXTUREFILTERTYPE filter) {
    Surface* a = static_cast<Surface*>(src);
    Surface* b = static_cast<Surface*>(dst);
    if (!a || !b) return D3DERR_INVALIDCALL;
    const GLuint fa = readFboFor(a);
    GLuint fb = fboFor(b, nullptr);
    if (msaaSamples && fb == msaaFbo) {  // a blit cannot write a multisampled buffer: into the resolved one (the next draw overwrites it)
        static bool once;
        if (!once) once = true, log("gles: StretchRect into the multisampled back buffer: written to the resolved buffer");
        resolveMain();
        fb = mainFbo;
    }
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fa);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb);
    boundFbo = ~0u;
    const RECT s = sr ? *sr : RECT{0, 0, static_cast<LONG>(a->level.width), static_cast<LONG>(a->level.height)};
    const RECT d = dr ? *dr : RECT{0, 0, static_cast<LONG>(b->level.width), static_cast<LONG>(b->level.height)};
    glDisable(GL_SCISSOR_TEST);
    glBlitFramebuffer(s.left, s.top, s.right, s.bottom, d.left, d.top, d.right, d.bottom, GL_COLOR_BUFFER_BIT, filter == D3DTEXF_POINT ? GL_NEAREST : GL_LINEAR);
    glStateUnknown();
    return S_OK;
}
HRESULT STDMETHODCALLTYPE Device::ColorFill(IDirect3DSurface9* s, const RECT* r, D3DCOLOR c) {
    Surface* surf = static_cast<Surface*>(s);
    if (!surf) return D3DERR_INVALIDCALL;
    const GLuint f = fboFor(surf, nullptr);
    if (msaaSamples && f == msaaFbo) msaaDirty = true;
    glBindFramebuffer(GL_FRAMEBUFFER, f);
    boundFbo = f;
    glEnable(GL_SCISSOR_TEST);
    if (r) glScissor(r->left, r->top, r->right - r->left, r->bottom - r->top);
    else glScissor(0, 0, static_cast<GLsizei>(surf->level.width), static_cast<GLsizei>(surf->level.height));
    glColorMask(1, 1, 1, 1);
    glClearColor(((c >> 16) & 255) / 255.0f, ((c >> 8) & 255) / 255.0f, (c & 255) / 255.0f, ((c >> 24) & 255) / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glStateUnknown();
    return S_OK;
}

HRESULT STDMETHODCALLTYPE Device::Clear(DWORD n, const D3DRECT* rects, DWORD flags, D3DCOLOR color, float z, DWORD stencil) {
    GlTime timer;
    bindTarget();
    GLbitfield bits = 0;
    if (flags & D3DCLEAR_TARGET) { bits |= GL_COLOR_BUFFER_BIT; glClearColor(((color >> 16) & 255) / 255.0f, ((color >> 8) & 255) / 255.0f, (color & 255) / 255.0f, ((color >> 24) & 255) / 255.0f); glColorMask(1, 1, 1, 1); }
    if (flags & D3DCLEAR_ZBUFFER) { bits |= GL_DEPTH_BUFFER_BIT; glClearDepthf(z); glDepthMask(GL_TRUE); }
    if (flags & D3DCLEAR_STENCIL) { bits |= GL_STENCIL_BUFFER_BIT; glClearStencil(static_cast<GLint>(stencil)); glStencilMask(0xFF); }
    // A clear of the whole target goes without the scissor test, so a tile-based GPU sees a
    // clear of the entire attachments (no load of the previous contents into the tiles; the
    // game clears colour, depth and stencil at every frame start) instead of a scissored one it
    // must compare against the framebuffer size or execute as a draw.
    // (Only when the depth-stencil, if any, is the target's size: with attachments of different
    // sizes the scissored clear stays.)
    const UINT tw = renderTarget ? renderTarget->level.width : 0, th = renderTarget ? renderTarget->level.height : 0;
    const bool sameSize = !depthStencil || (depthStencil->level.width == tw && depthStencil->level.height == th);
    const bool whole = sameSize && !(n && rects) && viewport.X == 0 && viewport.Y == 0 && viewport.Width >= tw && viewport.Height >= th;
    if (whole) glDisable(GL_SCISSOR_TEST);
    else glEnable(GL_SCISSOR_TEST);
    if (whole) {
        glClear(bits);
    } else if (n && rects) {
        for (DWORD i = 0; i < n; ++i) {
            glScissor(rects[i].x1, rects[i].y1, rects[i].x2 - rects[i].x1, rects[i].y2 - rects[i].y1);
            glClear(bits);
        }
    } else {
        glScissor(static_cast<GLint>(viewport.X), static_cast<GLint>(viewport.Y), static_cast<GLsizei>(viewport.Width), static_cast<GLsizei>(viewport.Height));
        glClear(bits);
    }
    ++statClears;
    glStateUnknown();
    GLCHECK("Clear");
    return S_OK;
}

HRESULT STDMETHODCALLTYPE Device::SetTexture(DWORD st, IDirect3DBaseTexture9* t) {
    if (st < 2) programDirty = true;  // the fixed-function key reads whether stages 0-1 have a texture
    if (st >= 8) return D3DERR_INVALIDCALL;
    if (t && t->GetType() != D3DRTYPE_TEXTURE) return D3DERR_INVALIDCALL;
    textures[st] = static_cast<Texture*>(t);
    texDirty = true;
    return S_OK;
}

// ---- shaders / programs ----
// Run `fn` on the render thread and wait for it: one queue round trip for a sequence of calls
// that each need the driver's answer (glCreate*, glGet*), instead of one round trip per call.
template <class F> void runOnRenderThread(F&& fn) {
    if (gq::direct()) { fn(); return; }
    gq::Sync done;
    gq::call([&fn, &done] { fn(); gq::signal(done); });
    gq::wait(done);
}

GLuint Device::compileShader(GLenum type, const std::string& src, const char* what) {
    if (!gq::direct()) { GLuint r = 0; runOnRenderThread([&] { r = compileShader(type, src, what); }); return r; }
    GLuint s = glCreateShader(type);
    const char* p = src.c_str();
    glShaderSource(s, 1, &p, nullptr);
    glCompileShader(s);
    GLint ok = 0;
    glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char buf[2048] = {};
        glGetShaderInfoLog(s, sizeof buf, nullptr, buf);
        log("gles: %s failed to compile:\n%s", what, buf);
        if (host::g_logLevel >= 2) log("%s", src.c_str());
        glDeleteShader(s);
        return 0;
    }
    return s;
}
GLuint Device::linkProgram(GLuint v, GLuint f) {
    if (!v || !f) return 0;
    if (!gq::direct()) { GLuint r = 0; runOnRenderThread([&] { r = linkProgram(v, f); }); return r; }
    GLuint p = glCreateProgram();
    glAttachShader(p, v);
    glAttachShader(p, f);
    glLinkProgram(p);
    GLint ok = 0;
    glGetProgramiv(p, GL_LINK_STATUS, &ok);
    if (!ok) {
        char buf[2048] = {};
        glGetProgramInfoLog(p, sizeof buf, nullptr, buf);
        log("gles: program link failed:\n%s", buf);
        glDeleteProgram(p);
        return 0;
    }
    return p;
}
HRESULT STDMETHODCALLTYPE Device::CreateVertexShader(const DWORD* code, IDirect3DVertexShader9** out) {
    if (!out || !code) return E_POINTER;
    VertexShader* s = new VertexShader();
    s->dev = this, s->id = nextShaderId++;
    size_t n = 0;
    while (code[n] != 0x0000FFFF) ++n;
    s->code.assign(code, code + n + 1);
    std::string err;
    if (translateDx9Shader(code, s->sh.info, err)) {
        s->sh.gl = compileShader(GL_VERTEX_SHADER, s->sh.info.glsl, "vertex shader");
        s->sh.ok = s->sh.gl != 0;
    } else {
        log("gles: vertex shader not translated: %s", err.c_str());
    }
    *out = s;
    return S_OK;
}
HRESULT STDMETHODCALLTYPE Device::CreatePixelShader(const DWORD* code, IDirect3DPixelShader9** out) {
    if (!out || !code) return E_POINTER;
    PixelShader* s = new PixelShader();
    s->dev = this, s->id = nextShaderId++;
    size_t n = 0;
    while (code[n] != 0x0000FFFF) ++n;
    s->code.assign(code, code + n + 1);
    std::string err;
    if (translateDx9Shader(code, s->sh.info, err)) {
        s->sh.gl = compileShader(GL_FRAGMENT_SHADER, s->sh.info.glsl, "pixel shader");
        s->sh.ok = s->sh.gl != 0;
    } else {
        log("gles: pixel shader not translated: %s", err.c_str());
    }
    *out = s;
    return S_OK;
}

VertexDecl* Device::fvfDeclaration() {
    if (auto it = fvfDecls.find(fvf); it != fvfDecls.end()) return it->second;
    std::vector<D3DVERTEXELEMENT9> el;
    WORD off = 0;
    const DWORD pos = fvf & D3DFVF_POSITION_MASK;
    if (pos == D3DFVF_XYZRHW) el.push_back({0, off, D3DDECLTYPE_FLOAT4, 0, D3DDECLUSAGE_POSITIONT, 0}), off += 16;
    else if (pos == D3DFVF_XYZ) el.push_back({0, off, D3DDECLTYPE_FLOAT3, 0, D3DDECLUSAGE_POSITION, 0}), off += 12;
    else if (pos == D3DFVF_XYZW) el.push_back({0, off, D3DDECLTYPE_FLOAT4, 0, D3DDECLUSAGE_POSITION, 0}), off += 16;
    else if (pos >= D3DFVF_XYZB1) { const int nb = static_cast<int>((pos - D3DFVF_XYZB1) / 2) + 1; el.push_back({0, off, D3DDECLTYPE_FLOAT3, 0, D3DDECLUSAGE_POSITION, 0}); off += 12; el.push_back({0, off, static_cast<BYTE>(D3DDECLTYPE_FLOAT1 + nb - 1), 0, D3DDECLUSAGE_BLENDWEIGHT, 0}); off += static_cast<WORD>(4 * nb); }
    if (fvf & D3DFVF_NORMAL) el.push_back({0, off, D3DDECLTYPE_FLOAT3, 0, D3DDECLUSAGE_NORMAL, 0}), off += 12;
    if (fvf & D3DFVF_PSIZE) el.push_back({0, off, D3DDECLTYPE_FLOAT1, 0, D3DDECLUSAGE_PSIZE, 0}), off += 4;
    if (fvf & D3DFVF_DIFFUSE) el.push_back({0, off, D3DDECLTYPE_D3DCOLOR, 0, D3DDECLUSAGE_COLOR, 0}), off += 4;
    if (fvf & D3DFVF_SPECULAR) el.push_back({0, off, D3DDECLTYPE_D3DCOLOR, 0, D3DDECLUSAGE_COLOR, 1}), off += 4;
    const int ntex = static_cast<int>((fvf & D3DFVF_TEXCOUNT_MASK) >> D3DFVF_TEXCOUNT_SHIFT);
    for (int i = 0; i < ntex; ++i) {
        const int size = static_cast<int>((fvf >> (16 + i * 2)) & 3);  // D3DFVF_TEXCOORDSIZE: 0 = 2, 3 = 1, 1 = 3, 2 = 4
        const BYTE type = size == 0 ? D3DDECLTYPE_FLOAT2 : size == 3 ? D3DDECLTYPE_FLOAT1 : size == 1 ? D3DDECLTYPE_FLOAT3 : D3DDECLTYPE_FLOAT4;
        el.push_back({0, off, type, 0, D3DDECLUSAGE_TEXCOORD, static_cast<BYTE>(i)});
        off += size == 0 ? 8 : size == 3 ? 4 : size == 1 ? 12 : 16;
    }
    el.push_back(D3DDECL_END());
    VertexDecl* d = new VertexDecl(this, el.data());
    fvfDecls[fvf] = d;  // the Ref holds it
    d->Release();
    return d;
}

Program* Device::program() {
    if (!programDirty && current) return current;
    programDirty = false;
    const VertexDecl* d = decl ? decl.p : fvf ? fvfDeclaration() : nullptr;
    const bool rhw = d && d->preTransformed;
    ProgramKey key;
    key.vs = rhw || !vs || !vs->sh.ok ? 0 : vs->id;
    key.ps = ps && ps->sh.ok ? ps->id : 0;
    if (!key.ps) {
        for (int i = 0; i < 4; ++i) {
            FfpStage& st = key.ffp.stage[i];
            st.colorOp = tss[i][D3DTSS_COLOROP], st.colorArg1 = tss[i][D3DTSS_COLORARG1], st.colorArg2 = tss[i][D3DTSS_COLORARG2];
            st.alphaOp = tss[i][D3DTSS_ALPHAOP], st.alphaArg1 = tss[i][D3DTSS_ALPHAARG1], st.alphaArg2 = tss[i][D3DTSS_ALPHAARG2];
            st.texCoordIndex = tss[i][D3DTSS_TEXCOORDINDEX] & 7;
            st.projected = (tss[i][D3DTSS_TEXTURETRANSFORMFLAGS] & D3DTTFF_PROJECTED) != 0;
            if (!textures[i]) st.colorArg1 = st.colorArg1 == D3DTA_TEXTURE ? D3DTA_CURRENT : st.colorArg1;  // no texture: white? Direct3D disables the stage
        }
        key.ffp.specular = rs[D3DRS_SPECULARENABLE] != 0;
    }
    // The alpha test is compiled into its own variant: a shader that can discard costs a Mali GPU
    // its early depth update and forward pixel kill on every draw, alpha test on or off.
    key.alphaTest = rs[D3DRS_ALPHATESTENABLE] && rs[D3DRS_ALPHAFUNC] != D3DCMP_ALWAYS;
    auto it = programs.find(key);
    if (it != programs.end()) return it->second.gl ? &it->second : nullptr;
    Program& p = programs[key];  // node-based map: the slot's address stays valid while the render thread fills it
    const GLuint v = key.vs ? vs->sh.gl : ffpVs;
    PixelShader* const pso = key.ps ? ps.p : nullptr;
    const bool alphaTest = key.alphaTest;
    const std::string ffpSrc = key.ps ? std::string() : alphaTest ? withAlphaTest(ffpPixelShader(key.ffp)) : ffpPixelShader(key.ffp);
    // The fixed-function shader's compile, the link and the ~150 uniform-location queries each
    // need an answer from the driver: through the queue every one of them was a round trip to
    // the render thread (a hitch the first time a shader pair draws). They run there in one go.
    auto build = [this, &p, v, pso, alphaTest, &ffpSrc] {
        GLuint f = 0;
        if (pso && alphaTest) {
            if (!pso->glAlphaTest) pso->glAlphaTest = compileShader(GL_FRAGMENT_SHADER, withAlphaTest(pso->sh.info.glsl), "pixel shader (alpha test)");
            f = pso->glAlphaTest;
        } else if (pso) {
            f = pso->sh.gl;
        } else {
            f = compileShader(GL_FRAGMENT_SHADER, ffpSrc, "fixed-function pixel shader");
        }
        p.gl = linkProgram(v, f);
        if (!pso && f) glDeleteShader(f);
        if (!p.gl) return;
        p.uC = glGetUniformLocation(p.gl, "c");
        p.uColorBgra = glGetUniformLocation(p.gl, "uColorBgra");
        p.uPc = glGetUniformLocation(p.gl, "pc");
        p.uBump = glGetUniformLocation(p.gl, "uBump");
        p.uProjMask = glGetUniformLocation(p.gl, "uProjMask");
        p.uFogColor = glGetUniformLocation(p.gl, "uFogColor");
        p.uFogEnable = glGetUniformLocation(p.gl, "uFogEnable");
        p.uAlphaFunc = glGetUniformLocation(p.gl, "uAlphaFunc");
        p.uAlphaRef = glGetUniformLocation(p.gl, "uAlphaRef");
        p.uViewport = glGetUniformLocation(p.gl, "uViewport");
        p.uTFactor = glGetUniformLocation(p.gl, "uTFactor");
        p.uHalfPixel = glGetUniformLocation(p.gl, "uHalfPixel");
        glUseProgram(p.gl);
        for (int i = 0; i < 6; ++i) {
            const std::string n = "s" + std::to_string(i);
            p.uS[i] = glGetUniformLocation(p.gl, n.c_str());
            if (p.uS[i] >= 0) glUniform1i(p.uS[i], i);
        }
        for (int k = 0; k < 128; ++k) p.uCElem[k] = p.uC >= 0 ? glGetUniformLocation(p.gl, ("c[" + std::to_string(k) + "]").c_str()) : -1;
        for (int k = 0; k < 8; ++k) p.uPcElem[k] = p.uPc >= 0 ? glGetUniformLocation(p.gl, ("pc[" + std::to_string(k) + "]").c_str()) : -1;
    };
    runOnRenderThread(build);
    if (p.gl) {
        lastProgramGl = p.gl;  // build() left it bound
        layoutValid = false;
    }
    return p.gl ? &p : nullptr;
}

// Sends the registers in [0, hi) whose version differs from what the program has seen, as runs
// (a gap of a few in-sync registers is sent along: one call is cheaper than two).
void Device::syncRegs(const GLint* loc, const float (*val)[4], const uint64_t* ver, uint64_t* seen, int hi) {
    constexpr int kGap = 4;
    for (int k = 0; k < hi;) {
        if (seen[k] == ver[k]) { ++k; continue; }
        int e = k + 1;
        for (int j = e; j < hi && j < e + kGap; ++j)
            if (seen[j] != ver[j]) e = j + 1;
        // Elements past the array's active size have no location; GL ignores the values of a
        // run that reaches past it.
        if (loc[k] >= 0) glUniform4fv(loc[k], e - k, &val[k][0]);
        for (int j = k; j < e; ++j) seen[j] = ver[j];
        k = e;
    }
}

void Device::setConstants(Program* p) {
    // Uniforms live per program object: each program is sent what changed since its own last
    // draw (Program::vsSeen / misc), whichever programs ran in between.
    if (p->constGen != constGen) {
        if (p->uC >= 0) syncRegs(p->uCElem, vsConst, vsVer, p->vsSeen, vsHi);
        if (p->uPc >= 0) syncRegs(p->uPcElem, psConst, psVer, p->psSeen, 8);
        p->constGen = constGen;
    }
    if (p->miscGen == miscGen) return;
    p->miscGen = miscGen;
    Program::Misc m{};
    for (int s = 0; s < 6; ++s)
        for (int k = 0; k < 4; ++k) std::memcpy(&m.bump[s][k], &tss[s][D3DTSS_BUMPENVMAT00 + k], 4);
    for (int s = 0; s < 8; ++s) if (tss[s][D3DTSS_TEXTURETRANSFORMFLAGS] & D3DTTFF_PROJECTED) m.projMask |= 1 << s;
    auto color = [](DWORD c, float* o) { o[0] = ((c >> 16) & 255) / 255.0f, o[1] = ((c >> 8) & 255) / 255.0f, o[2] = (c & 255) / 255.0f, o[3] = ((c >> 24) & 255) / 255.0f; };
    color(rs[D3DRS_FOGCOLOR], m.fogColor);
    color(rs[D3DRS_TEXTUREFACTOR], m.tFactor);
    m.fogEnable = rs[D3DRS_FOGENABLE] ? 1 : 0;
    m.alphaFunc = rs[D3DRS_ALPHATESTENABLE] ? static_cast<int>(rs[D3DRS_ALPHAFUNC]) : 8;
    m.alphaRef = (rs[D3DRS_ALPHAREF] & 255) / 255.0f;
    m.halfPixel[0] = 1.0f / static_cast<float>(std::max(1u, viewport.Width)), m.halfPixel[1] = 1.0f / static_cast<float>(std::max(1u, viewport.Height));
    m.viewport[0] = static_cast<float>(viewport.X), m.viewport[1] = static_cast<float>(viewport.Y);
    m.viewport[2] = static_cast<float>(std::max(1u, viewport.Width)), m.viewport[3] = static_cast<float>(std::max(1u, viewport.Height));
    Program::Misc& o = p->misc;
    const bool all = !p->miscKnown;
#define CHANGED(field) (all || std::memcmp(&m.field, &o.field, sizeof m.field) != 0)
    if (p->uBump >= 0 && CHANGED(bump)) glUniform4fv(p->uBump, 6, &m.bump[0][0]);
    if (p->uProjMask >= 0 && CHANGED(projMask)) glUniform1i(p->uProjMask, m.projMask);
    if (p->uFogColor >= 0 && CHANGED(fogColor)) glUniform4f(p->uFogColor, m.fogColor[0], m.fogColor[1], m.fogColor[2], m.fogColor[3]);
    if (p->uFogEnable >= 0 && CHANGED(fogEnable)) glUniform1i(p->uFogEnable, m.fogEnable);
    if (p->uAlphaFunc >= 0 && CHANGED(alphaFunc)) glUniform1i(p->uAlphaFunc, m.alphaFunc);
    if (p->uAlphaRef >= 0 && CHANGED(alphaRef)) glUniform1f(p->uAlphaRef, m.alphaRef);
    if (p->uHalfPixel >= 0 && CHANGED(halfPixel)) glUniform2f(p->uHalfPixel, m.halfPixel[0], m.halfPixel[1]);
    if (p->uViewport >= 0 && CHANGED(viewport)) glUniform4f(p->uViewport, m.viewport[0], m.viewport[1], m.viewport[2], m.viewport[3]);
    if (p->uTFactor >= 0 && CHANGED(tFactor)) glUniform4f(p->uTFactor, m.tFactor[0], m.tFactor[1], m.tFactor[2], m.tFactor[3]);
#undef CHANGED
    o = m;
    p->miscKnown = true;
}
void Device::setColorBgra(int mask) {  // uColorBgra of the current program (bindVertexLayout)
    if (current && current->uColorBgra >= 0 && current->colorBgra != mask) { glUniform1i(current->uColorBgra, mask); current->colorBgra = mask; }
}

GLenum blendFactor(DWORD b) {
    switch (b) {
        case D3DBLEND_ZERO: return GL_ZERO;
        case D3DBLEND_ONE: return GL_ONE;
        case D3DBLEND_SRCCOLOR: return GL_SRC_COLOR;
        case D3DBLEND_INVSRCCOLOR: return GL_ONE_MINUS_SRC_COLOR;
        case D3DBLEND_SRCALPHA: return GL_SRC_ALPHA;
        case D3DBLEND_INVSRCALPHA: return GL_ONE_MINUS_SRC_ALPHA;
        case D3DBLEND_DESTALPHA: return GL_DST_ALPHA;
        case D3DBLEND_INVDESTALPHA: return GL_ONE_MINUS_DST_ALPHA;
        case D3DBLEND_DESTCOLOR: return GL_DST_COLOR;
        case D3DBLEND_INVDESTCOLOR: return GL_ONE_MINUS_DST_COLOR;
        case D3DBLEND_SRCALPHASAT: return GL_SRC_ALPHA_SATURATE;
        case D3DBLEND_BLENDFACTOR: return GL_CONSTANT_COLOR;
        case D3DBLEND_INVBLENDFACTOR: return GL_ONE_MINUS_CONSTANT_COLOR;
        default: return GL_ONE;
    }
}
GLenum cmpFunc(DWORD f) {
    switch (f) {
        case D3DCMP_NEVER: return GL_NEVER;
        case D3DCMP_LESS: return GL_LESS;
        case D3DCMP_EQUAL: return GL_EQUAL;
        case D3DCMP_LESSEQUAL: return GL_LEQUAL;
        case D3DCMP_GREATER: return GL_GREATER;
        case D3DCMP_NOTEQUAL: return GL_NOTEQUAL;
        case D3DCMP_GREATEREQUAL: return GL_GEQUAL;
        default: return GL_ALWAYS;
    }
}
GLenum stencilOp(DWORD o) {
    switch (o) {
        case D3DSTENCILOP_ZERO: return GL_ZERO;
        case D3DSTENCILOP_REPLACE: return GL_REPLACE;
        case D3DSTENCILOP_INCRSAT: return GL_INCR;
        case D3DSTENCILOP_DECRSAT: return GL_DECR;
        case D3DSTENCILOP_INVERT: return GL_INVERT;
        case D3DSTENCILOP_INCR: return GL_INCR_WRAP;
        case D3DSTENCILOP_DECR: return GL_DECR_WRAP;
        default: return GL_KEEP;
    }
}
GLenum blendOp(DWORD o) {
    switch (o) {
        case D3DBLENDOP_SUBTRACT: return GL_FUNC_SUBTRACT;
        case D3DBLENDOP_REVSUBTRACT: return GL_FUNC_REVERSE_SUBTRACT;
        case D3DBLENDOP_MIN: return GL_MIN;
        case D3DBLENDOP_MAX: return GL_MAX;
        default: return GL_FUNC_ADD;
    }
}

// The GL state last issued, so a render-state change between draws re-issues only what
// changed (the Mali driver's cost is per call; Fable toggles blend / cull / z-write between
// most draws, which used to re-apply everything). Passes that touch GL state directly (Clear,
// ColorFill, StretchRect, the overlay, Present) invalidate it through glStateUnknown().
void Device::applyRenderStates() {
    if (!rsDirty) return;
    rsDirty = false;
    GlShadow& g = glShadow;
    static const bool noShadow = [] { const char* e = getenv("FABLE_GLES_NOSHADOW"); return e && atoi(e) != 0; }();  // diagnostic: re-issue every state
    if (!g.valid || noShadow) g.reset();  // a pass set GL state directly: nothing is known, every field is re-issued
    // viewport & scissor (rows are in Direct3D order in our framebuffers: no flip)
    const GLint vx = static_cast<GLint>(viewport.X), vy = static_cast<GLint>(viewport.Y);
    const GLsizei vw = static_cast<GLsizei>(std::max(1u, viewport.Width)), vh = static_cast<GLsizei>(std::max(1u, viewport.Height));
    if (vx != g.vx || vy != g.vy || vw != g.vw || vh != g.vh) { glViewport(vx, vy, vw, vh); g.vx = vx, g.vy = vy, g.vw = vw, g.vh = vh; }
    if (viewport.MinZ != g.zn || viewport.MaxZ != g.zf) { glDepthRangef(viewport.MinZ, viewport.MaxZ); g.zn = viewport.MinZ, g.zf = viewport.MaxZ; }
    const int sc = rs[D3DRS_SCISSORTESTENABLE] != 0;
    if (sc != g.scissor) { if (sc) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST); g.scissor = sc; }
    if (sc) {
        const GLint sx = scissor.left, sy = scissor.top, sw = std::max<LONG>(0, scissor.right - scissor.left), shh = std::max<LONG>(0, scissor.bottom - scissor.top);
        if (sx != g.sx || sy != g.sy || sw != g.sw || shh != g.sh) { glScissor(sx, sy, sw, shh); g.sx = sx, g.sy = sy, g.sw = sw, g.sh = shh; }
    }
    // depth
    const int depth = rs[D3DRS_ZENABLE] && depthStencil;
    if (depth != g.depth) { if (depth) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST); g.depth = depth; }
    if (depth) { const GLenum f = cmpFunc(rs[D3DRS_ZFUNC]); if (f != g.depthFunc) { glDepthFunc(f); g.depthFunc = f; } }
    const int zw = rs[D3DRS_ZWRITEENABLE] != 0;
    if (zw != g.depthMask) { glDepthMask(zw ? GL_TRUE : GL_FALSE); g.depthMask = zw; }
    // blend
    const int blend = rs[D3DRS_ALPHABLENDENABLE] != 0;
    if (blend != g.blend) { if (blend) glEnable(GL_BLEND); else glDisable(GL_BLEND); g.blend = blend; }
    if (blend) {
        const GLenum sb = blendFactor(rs[D3DRS_SRCBLEND]), db = blendFactor(rs[D3DRS_DESTBLEND]), op = blendOp(rs[D3DRS_BLENDOP]);
        if (sb != g.sb || db != g.db) { glBlendFunc(sb, db); g.sb = sb, g.db = db; }
        if (op != g.bop) { glBlendEquation(op); g.bop = op; }
        const DWORD c = rs[D3DRS_BLENDFACTOR];
        if (c != g.blendColor) { glBlendColor(((c >> 16) & 255) / 255.0f, ((c >> 8) & 255) / 255.0f, (c & 255) / 255.0f, ((c >> 24) & 255) / 255.0f); g.blendColor = c; }
    }
    const DWORD cw = rs[D3DRS_COLORWRITEENABLE] & 15;
    if (cw != g.colorMask) { glColorMask((cw & 1) != 0, (cw & 2) != 0, (cw & 4) != 0, (cw & 8) != 0); g.colorMask = cw; }
    // cull: clip y is negated in the vertex epilogue, which mirrors the winding, so a Direct3D
    // clockwise triangle comes out counter-clockwise in GL (D3DCULL_CCW culls what GL calls CW).
    const int cull = rs[D3DRS_CULLMODE] == D3DCULL_CW ? 1 : rs[D3DRS_CULLMODE] == D3DCULL_CCW ? 2 : 0;
    if (cull != g.cull) {
        if (cull) { if (g.cull <= 0) { glEnable(GL_CULL_FACE); glCullFace(GL_BACK); } glFrontFace(cull == 1 ? GL_CW : GL_CCW); }
        else glDisable(GL_CULL_FACE);
        g.cull = cull;
    }
    // stencil
    const int st = rs[D3DRS_STENCILENABLE] && depthStencil;
    if (st != g.stencil) { if (st) glEnable(GL_STENCIL_TEST); else glDisable(GL_STENCIL_TEST); g.stencil = st; }
    if (st) {
        const GLenum f = cmpFunc(rs[D3DRS_STENCILFUNC]);
        const GLint ref = static_cast<GLint>(rs[D3DRS_STENCILREF]);
        const GLuint mask = rs[D3DRS_STENCILMASK];
        if (f != g.sfunc || ref != g.sref || mask != g.smask) { glStencilFunc(f, ref, mask); g.sfunc = f, g.sref = ref, g.smask = mask; }
        const GLenum a = stencilOp(rs[D3DRS_STENCILFAIL]), b = stencilOp(rs[D3DRS_STENCILZFAIL]), cc = stencilOp(rs[D3DRS_STENCILPASS]);
        if (a != g.sfail || b != g.szfail || cc != g.spass) { glStencilOp(a, b, cc); g.sfail = a, g.szfail = b, g.spass = cc; }
        const GLuint wm = rs[D3DRS_STENCILWRITEMASK];
        if (wm != g.swmask) { glStencilMask(wm); g.swmask = wm; }
    }
    const int po = rs[D3DRS_SLOPESCALEDEPTHBIAS] || rs[D3DRS_DEPTHBIAS];
    if (po != g.polyOff) { if (po) glEnable(GL_POLYGON_OFFSET_FILL); else glDisable(GL_POLYGON_OFFSET_FILL); g.polyOff = po; }
    if (po) {
        float slope, bias;
        std::memcpy(&slope, &rs[D3DRS_SLOPESCALEDEPTHBIAS], 4);
        std::memcpy(&bias, &rs[D3DRS_DEPTHBIAS], 4);
        if (slope != g.slope || bias != g.bias) { glPolygonOffset(slope, bias * (1 << 24)); g.slope = slope, g.bias = bias; }
    }
}

GLenum wrapMode(DWORD a) {
    switch (a) {
        case D3DTADDRESS_MIRROR: return GL_MIRRORED_REPEAT;
        case D3DTADDRESS_CLAMP: case D3DTADDRESS_BORDER: return GL_CLAMP_TO_EDGE;
        default: return GL_REPEAT;
    }
}
void Device::applySamplers() {
    if (!texDirty && !anySamplerDirty && samplersBound) return;
    for (int i = 0; i < 8; ++i) {
        const GLuint want = textures[i] ? textures[i]->gl : 0;
        if (want != boundTexGl[i]) {
            if (activeUnit != i) { glActiveTexture(GL_TEXTURE0 + i); activeUnit = i; }
            glBindTexture(GL_TEXTURE_2D, want);
            boundTexGl[i] = want;
        }
        SamplerState& s = samplers[i];
        if (!samplersBound) glBindSampler(static_cast<GLuint>(i), s.gl);  // the sampler objects stay on their units
        if (!s.dirty) continue;
        s.dirty = false;
        // only the parameters that differ from what the sampler object holds (a change of one
        // Direct3D sampler state used to re-send all five)
        s.param(0, GL_TEXTURE_WRAP_S, static_cast<GLint>(wrapMode(s.v[D3DSAMP_ADDRESSU])));
        s.param(1, GL_TEXTURE_WRAP_T, static_cast<GLint>(wrapMode(s.v[D3DSAMP_ADDRESSV])));
        const bool mag = s.v[D3DSAMP_MAGFILTER] >= D3DTEXF_LINEAR, min = s.v[D3DSAMP_MINFILTER] >= D3DTEXF_LINEAR;
        const DWORD mip = s.v[D3DSAMP_MIPFILTER];
        s.param(2, GL_TEXTURE_MAG_FILTER, mag ? GL_LINEAR : GL_NEAREST);
        GLenum minf;
        if (mip == D3DTEXF_NONE) minf = min ? GL_LINEAR : GL_NEAREST;
        else if (mip == D3DTEXF_POINT) minf = min ? GL_LINEAR_MIPMAP_NEAREST : GL_NEAREST_MIPMAP_NEAREST;
        else minf = min ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR;
        s.param(3, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(minf));
        float bias;
        std::memcpy(&bias, &s.v[D3DSAMP_MIPMAPLODBIAS], 4);
        // the game asks for a -4 bias on some stages to force the top level: the device
        // honours it within the levels that exist
        (void)bias;
        // GL_TEXTURE_COMPARE_MODE is GL_NONE on a new sampler object and nothing changes it
    }
    samplersBound = true;
    texDirty = false;
    anySamplerDirty = false;
}

GLenum declType(BYTE t, GLint& size, GLboolean& norm, bool& isColor) {
    isColor = false;
    switch (t) {
        case D3DDECLTYPE_FLOAT1: size = 1; norm = GL_FALSE; return GL_FLOAT;
        case D3DDECLTYPE_FLOAT2: size = 2; norm = GL_FALSE; return GL_FLOAT;
        case D3DDECLTYPE_FLOAT3: size = 3; norm = GL_FALSE; return GL_FLOAT;
        case D3DDECLTYPE_FLOAT4: size = 4; norm = GL_FALSE; return GL_FLOAT;
        case D3DDECLTYPE_D3DCOLOR: size = 4; norm = GL_TRUE; isColor = true; return GL_UNSIGNED_BYTE;
        case D3DDECLTYPE_UBYTE4: size = 4; norm = GL_FALSE; return GL_UNSIGNED_BYTE;
        case D3DDECLTYPE_UBYTE4N: size = 4; norm = GL_TRUE; return GL_UNSIGNED_BYTE;
        case D3DDECLTYPE_SHORT2: size = 2; norm = GL_FALSE; return GL_SHORT;
        case D3DDECLTYPE_SHORT4: size = 4; norm = GL_FALSE; return GL_SHORT;
        case D3DDECLTYPE_SHORT2N: size = 2; norm = GL_TRUE; return GL_SHORT;
        case D3DDECLTYPE_SHORT4N: size = 4; norm = GL_TRUE; return GL_SHORT;
        case D3DDECLTYPE_USHORT2N: size = 2; norm = GL_TRUE; return GL_UNSIGNED_SHORT;
        case D3DDECLTYPE_USHORT4N: size = 4; norm = GL_TRUE; return GL_UNSIGNED_SHORT;
        default: size = 4; norm = GL_FALSE; return GL_FLOAT;
    }
}

// Issues the attribute pointers of `d` on the bound vertex array object; returns the mask of
// colour attributes (the shader swizzles those from BGRA).
int Device::specifyAttributes(const VertexDecl* d, bool up, UINT upStride, INT baseVertex) {
    int colorMask = 0;
    for (const D3DVERTEXELEMENT9& e : d->elements) {
        int loc = -1;
        if (d->preTransformed) {
            if (e.Usage == D3DDECLUSAGE_POSITIONT || e.Usage == D3DDECLUSAGE_POSITION) loc = 0;
            else if (e.Usage == D3DDECLUSAGE_COLOR) loc = 1 + std::min<int>(e.UsageIndex, 1);
            else if (e.Usage == D3DDECLUSAGE_TEXCOORD && e.UsageIndex < 4) loc = 3 + e.UsageIndex;
        } else if (vs) {
            for (const Dx9Input& in : vs->sh.info.inputs)
                if (in.usage == e.Usage && in.index == e.UsageIndex) loc = in.reg;
        }
        if (loc < 0) continue;
        GLint size;
        GLboolean norm;
        bool isColor;
        const GLenum type = declType(e.Type, size, norm, isColor);
        if (isColor) colorMask |= 1 << loc;
        if (up) {
            glBindBuffer(GL_ARRAY_BUFFER, scratchVbo);
            glVertexAttribPointer(static_cast<GLuint>(loc), size, type, norm, static_cast<GLsizei>(upStride), reinterpret_cast<const void*>(static_cast<uintptr_t>(e.Offset)));
        } else {
            const StreamSource& s = streams[e.Stream < 8 ? e.Stream : 0];
            if (!s.vb) continue;
            glBindBuffer(GL_ARRAY_BUFFER, s.vb->buf.gl);
            glVertexAttribPointer(static_cast<GLuint>(loc), size, type, norm, static_cast<GLsizei>(s.stride), reinterpret_cast<const void*>(static_cast<uintptr_t>(s.offset + e.Offset + static_cast<INT>(s.stride) * baseVertex)));
        }
        glEnableVertexAttribArray(static_cast<GLuint>(loc));
    }
    return colorMask;
}

// Binds the vertex attributes for the current declaration. `baseVertex` shifts every stream.
bool Device::bindVertexLayout(const VertexDecl* d, bool up, const void* upData, UINT upStride, UINT, INT baseVertex) {
    if (!d) return false;
    LayoutKey key;
    std::memset(&key, 0, sizeof key);  // the padding too: the key is hashed and compared as bytes
    key.decl = d, key.vs = d->preTransformed ? nullptr : vs.p, key.program = lastProgramGl, key.up = up, key.upStride = up ? upStride : 0, key.baseVertex = up ? 0 : baseVertex;
    if (!up)
        for (int i = 0; i < 8; ++i) key.vb[i] = streams[i].vb ? streams[i].vb->buf.gl : 0, key.off[i] = streams[i].offset, key.stride[i] = streams[i].stride;
    if (layoutValid && std::memcmp(&key, &lastLayout, sizeof key) == 0) return true;  // same pointers as the last draw
    lastLayout = key;
    layoutValid = true;
    static const bool noVao = [] { const char* e = getenv("FABLE_GLES_NOVAO"); return e && atoi(e) != 0; }();
    if (noVao) {  // diagnostic: one object, attributes re-specified every draw (the pre-cache behaviour)
        static VaoEntry plain;
        if (!plain.vao) glGenVertexArrays(1, &plain.vao);
        glBindVertexArray(plain.vao);
        for (int i = 0; i < 16; ++i) glDisableVertexAttribArray(static_cast<GLuint>(i));
        boundIbo = ~0u;
        curVao = nullptr;
        plain.colorMask = specifyAttributes(d, up, upStride, baseVertex);
        setColorBgra(plain.colorMask);
        return true;
    }
    if (auto it = vaos.find(key); it != vaos.end()) {
        glBindVertexArray(it->second.vao);
        curVao = &it->second;
        boundIbo = it->second.ibo;
        setColorBgra(it->second.colorMask);
        return true;
    }
    if (vaos.size() >= 4096) {
        for (auto& [k, e] : vaos) glDeleteVertexArrays(1, &e.vao);
        vaos.clear();
    }
    VaoEntry entry;
    glGenVertexArrays(1, &entry.vao);
    glBindVertexArray(entry.vao);
    boundIbo = 0;  // a fresh object has no element buffer
    entry.colorMask = specifyAttributes(d, up, upStride, baseVertex);
    setColorBgra(entry.colorMask);
    curVao = &vaos.emplace(key, entry).first->second;  // node pointers stay valid across rehashing
    (void)upData;
    return true;
}

void Device::dropVaos(GLuint buffer, const void* pointer) {
    for (auto it = vaos.begin(); it != vaos.end();) {
        const LayoutKey& k = it->first;
        bool hit = pointer && (k.decl == pointer || k.vs == pointer);
        if (buffer) for (int i = 0; i < 8 && !hit; ++i) hit = k.vb[i] == buffer;
        if (buffer && it->second.ibo == buffer) hit = true;
        if (hit) {
            if (curVao == &it->second) curVao = nullptr, layoutValid = false, boundIbo = ~0u;
            glDeleteVertexArrays(1, &it->second.vao);
            it = vaos.erase(it);
        } else ++it;
    }
}
void dropVaosForBuffer(GLuint gl) { if (g_device) g_device->dropVaos(gl, nullptr); }
void dropVaosForPointer(const void* p) { if (g_device) g_device->dropVaos(0, p); }

static bool key_trace_once(uint32_t id) { static std::vector<uint32_t> seen; if (std::find(seen.begin(), seen.end(), id) != seen.end()) return false; seen.push_back(id); return true; }
// Host graphics settings: draws into the shadow buffer (a render target of the def's
// ShadowBufferSize) are skipped with shadows off, so the cleared buffer shadows nothing;
// draws into the small glow chain (<= 256 square, not the shadow size) with glow off.
bool Device::skipDraw() {
    // glow off: the engine still runs its bloom chain into small square targets; skip those draws
    if (!renderTarget || renderTarget->isBackBuffer) return false;
    if (host::gfx::get().glow) return false;
    const UINT w = renderTarget->level.width, h = renderTarget->level.height;
    return w == h && w <= 256 && w != host::gfx::shadowBufferSize();
}

void Device::prepareDraw(bool up, const void* upData, UINT upStride, UINT upVertices, INT baseVertex) {
    bindTarget();
    applyRenderStates();
    applySamplers();
    Program* p = program();
    current = p;
    if (!p) return;
    if (p->gl != lastProgramGl) { glUseProgram(p->gl); lastProgramGl = p->gl; }
    setConstants(p);
    if (up) {
        glBindBuffer(GL_ARRAY_BUFFER, scratchVbo);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(upStride) * upVertices, upData, GL_STREAM_DRAW);
    }
    bindVertexLayout(decl ? decl.p : fvf ? fvfDeclaration() : nullptr, up, upData, upStride, upVertices, baseVertex);
    static int traceLeft = [] { const char* t = getenv("FABLE_GLES_TRACE"); return t ? atoi(t) : 0; }();
    if (dumpFrameWanted()) traceLeft = 1;  // every draw of that frame is traced
    // FABLE_GLES_DUMP_RT=all: every textured draw into an off-screen render target, in any
    // frame, is traced and dumped (up to 300), to catch one-off composites such as the hero's
    // morph textures at load.
    static const bool rtAll = [] { const char* t = getenv("FABLE_GLES_DUMP_RT"); return t && std::strcmp(t, "all") == 0; }();
    static int rtAllLeft = 300;
    g_rtDumpThisDraw = false;
    if (rtAll && rtAllLeft > 0 && renderTarget && !renderTarget->isBackBuffer && textures[0] && textures[0]->width >= 64) { --rtAllLeft; traceLeft = 1; g_rtDumpThisDraw = true; }
    if (traceLeft > 0) {
        --traceLeft;
        const VertexDecl* d = decl ? decl.p : fvf ? fvfDeclaration() : nullptr;
        std::string el;
        if (d) for (const auto& e : d->elements) el += " s" + std::to_string(e.Stream) + ":" + std::to_string(e.Offset) + ":t" + std::to_string(e.Type) + ":u" + std::to_string(e.Usage) + "." + std::to_string(e.UsageIndex);
        log("gles trace: draw up=%d prog=%u vs=%u ps=%u rhw=%d fvf=0x%X decl[%s] vp=%u,%u %ux%u rt=%p ds=%p blend=%u(%u,%u) z=%u/%u zw=%u cull=%u cw=%X atest=%u fog=%u tex0=%p(%u %ux%u gl%u) tss0=%u/%u,%u a%u/%u,%u",
            up, p->gl, vs ? vs->id : 0, ps ? ps->id : 0, d && d->preTransformed, fvf, el.c_str(), viewport.X, viewport.Y, viewport.Width, viewport.Height,
            static_cast<void*>(renderTarget.p), static_cast<void*>(depthStencil.p), rs[D3DRS_ALPHABLENDENABLE], rs[D3DRS_SRCBLEND], rs[D3DRS_DESTBLEND],
            rs[D3DRS_ZENABLE], rs[D3DRS_ZFUNC], rs[D3DRS_ZWRITEENABLE], rs[D3DRS_CULLMODE], rs[D3DRS_COLORWRITEENABLE], rs[D3DRS_ALPHATESTENABLE], rs[D3DRS_FOGENABLE],
            static_cast<void*>(textures[0].p), textures[0] ? textures[0]->format : 0, textures[0] ? textures[0]->width : 0, textures[0] ? textures[0]->height : 0, textures[0] ? textures[0]->gl : 0,
            tss[0][D3DTSS_COLOROP], tss[0][D3DTSS_COLORARG1], tss[0][D3DTSS_COLORARG2], tss[0][D3DTSS_ALPHAOP], tss[0][D3DTSS_ALPHAARG1], tss[0][D3DTSS_ALPHAARG2]);
        log("gles trace:   tex1=%p(%u gl%u) tex2=%p(%u gl%u) tex3=%p(%u gl%u) pc0=%.3f,%.3f,%.3f,%.3f c0=%.3f,%.3f,%.3f,%.3f",
            static_cast<void*>(textures[1].p), textures[1] ? textures[1]->format : 0, textures[1] ? textures[1]->gl : 0,
            static_cast<void*>(textures[2].p), textures[2] ? textures[2]->format : 0, textures[2] ? textures[2]->gl : 0,
            static_cast<void*>(textures[3].p), textures[3] ? textures[3]->format : 0, textures[3] ? textures[3]->gl : 0,
            psConst[0][0], psConst[0][1], psConst[0][2], psConst[0][3], vsConst[0][0], vsConst[0][1], vsConst[0][2], vsConst[0][3]);
        if (vs && key_trace_once(vs->id)) log("gles trace: vs %u glsl:\n%s", vs->id, vs->sh.info.glsl.c_str());
        if (ps && key_trace_once(0x10000u + ps->id)) log("gles trace: ps %u glsl:\n%s", ps->id, ps->sh.info.glsl.c_str());
        if (up && upData && d) {
            for (UINT v = 0; v < std::min<UINT>(upVertices, 4); ++v) {
                const float* f = reinterpret_cast<const float*>(static_cast<const char*>(upData) + static_cast<size_t>(v) * upStride);
                log("gles trace:   v%u pos %.1f %.1f %.3f %.3f", v, f[0], f[1], f[2], f[3]);
            }
        }
    }
}

GLenum primMode(D3DPRIMITIVETYPE t, UINT prims, GLsizei& count) {
    switch (t) {
        case D3DPT_POINTLIST: count = prims; return GL_POINTS;
        case D3DPT_LINELIST: count = prims * 2; return GL_LINES;
        case D3DPT_LINESTRIP: count = prims + 1; return GL_LINE_STRIP;
        case D3DPT_TRIANGLELIST: count = prims * 3; return GL_TRIANGLES;
        case D3DPT_TRIANGLESTRIP: count = prims + 2; return GL_TRIANGLE_STRIP;
        case D3DPT_TRIANGLEFAN: count = prims + 2; return GL_TRIANGLE_FAN;
        default: count = 0; return GL_TRIANGLES;
    }
}

HRESULT STDMETHODCALLTYPE Device::DrawPrimitive(D3DPRIMITIVETYPE t, UINT start, UINT n) {
    GlTime timer;
    if (skipDraw()) return S_OK;
    prepareDraw(false, nullptr, 0, 0, 0);
    if (!current) return S_OK;
    GLsizei count;
    const GLenum mode = primMode(t, n, count);
    glDrawArrays(mode, static_cast<GLint>(start), count);
    ++statDraws;
    afterDraw();
    GLCHECK("DrawPrimitive");
    return S_OK;
}
HRESULT STDMETHODCALLTYPE Device::DrawIndexedPrimitive(D3DPRIMITIVETYPE t, INT base, UINT, UINT, UINT start, UINT n) {
    GlTime timer;
    if (skipDraw()) return S_OK;
    if (!indices) return D3DERR_INVALIDCALL;
    prepareDraw(false, nullptr, 0, 0, base);
    if (!current) return S_OK;
    GLsizei count;
    const GLenum mode = primMode(t, n, count);
    if (boundIbo != indices->buf.gl) { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices->buf.gl); boundIbo = indices->buf.gl; if (curVao) curVao->ibo = boundIbo; }
    const bool i32 = indices->format == D3DFMT_INDEX32;
    glDrawElements(mode, count, i32 ? GL_UNSIGNED_INT : GL_UNSIGNED_SHORT, reinterpret_cast<const void*>(static_cast<uintptr_t>(start) * (i32 ? 4 : 2)));
    ++statDraws;
    afterDraw();
    GLCHECK("DrawIndexedPrimitive");
    return S_OK;
}
HRESULT STDMETHODCALLTYPE Device::DrawPrimitiveUP(D3DPRIMITIVETYPE t, UINT n, const void* data, UINT stride) {
    GlTime timer;
    if (skipDraw()) return S_OK;
    GLsizei count;
    const GLenum mode = primMode(t, n, count);
    prepareDraw(true, data, stride, static_cast<UINT>(count), 0);
    if (!current) return S_OK;
    glDrawArrays(mode, 0, count);
    ++statDraws;
    afterDraw();
    GLCHECK("DrawPrimitiveUP");
    return S_OK;
}
HRESULT STDMETHODCALLTYPE Device::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE t, UINT minIdx, UINT numV, UINT n, const void* idx, D3DFORMAT fmt, const void* data, UINT stride) {
    GlTime timer;
    if (skipDraw()) return S_OK;
    GLsizei count;
    const GLenum mode = primMode(t, n, count);
    prepareDraw(true, data, stride, minIdx + numV, 0);
    if (!current) return S_OK;
    const bool i32 = fmt == D3DFMT_INDEX32;
    if (boundIbo != scratchIbo) { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, scratchIbo); boundIbo = scratchIbo; if (curVao) curVao->ibo = scratchIbo; }
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(count) * (i32 ? 4 : 2), idx, GL_STREAM_DRAW);
    glDrawElements(mode, count, i32 ? GL_UNSIGNED_INT : GL_UNSIGNED_SHORT, nullptr);
    ++statDraws;
    afterDraw();
    GLCHECK("DrawIndexedPrimitiveUP");
    return S_OK;
}

// ---- the host overlay (overlay/): text over the back buffer before it is presented ----
void Device::drawOverlay() {
    using namespace host::overlay;
    if (!enabled()) return;
    notePresent();
    if (takeScreenshot()) saveScreenshot();
    if (showStats()) {
        char buf[160];
        std::snprintf(buf, sizeof buf, "%.0f ms frame, %.0f ms in GL (swap %.0f, uploads %.0f), %u draws, %u clears, %ux%u", frameMs, glMs, swapMs, uploadMs, statDraws, statClears, pp.BackBufferWidth, pp.BackBufferHeight);
        setStats(buf);
    }
    if (host::overlay::vsync() != overlayVsync) { overlayVsync = host::overlay::vsync(); setSwapInterval(overlayVsync); }
    const auto ls = lines(static_cast<int>(pp.BackBufferWidth), static_cast<int>(pp.BackBufferHeight));
    if (ls.empty()) return;
    if (!overlayProgram) {
        const char* vs =
            "#version 300 es\nprecision highp float;\nlayout(location = 0) in vec4 aPosUv;\nlayout(location = 1) in vec4 aColor;\n"
            "uniform vec2 uSize;\nout vec2 vUv;\nout vec4 vColor;\nvoid main() {\n"
            "  vec2 p = aPosUv.xy / uSize * 2.0 - 1.0;\n  gl_Position = vec4(p.x, p.y, 0.0, 1.0);\n  vUv = aPosUv.zw; vColor = aColor;\n}\n";
        const char* fs =
            "#version 300 es\nprecision mediump float;\nuniform sampler2D uTex;\nin vec2 vUv;\nin vec4 vColor;\nout vec4 o;\n"
            "void main() { o = vec4(vColor.rgb, vColor.a * texture(uTex, vUv).r); }\n";
        const GLuint v = compileShader(GL_VERTEX_SHADER, vs, "overlay vertex shader"), f = compileShader(GL_FRAGMENT_SHADER, fs, "overlay fragment shader");
        overlayProgram = linkProgram(v, f);
        if (v) glDeleteShader(v);
        if (f) glDeleteShader(f);
        if (!overlayProgram) return;
        overlayUSize = glGetUniformLocation(overlayProgram, "uSize");
        glUseProgram(overlayProgram);
        glUniform1i(glGetUniformLocation(overlayProgram, "uTex"), 0);
        // the atlas: 96 cells of 8x16 (95 glyphs + a solid block), one byte per pixel
        std::vector<unsigned char> atlas(96 * 8 * 16, 0);
        for (int g = 0; g < 96; ++g)
            for (int y = 0; y < 16; ++y)
                for (int x = 0; x < 8; ++x) atlas[static_cast<size_t>(y) * 96 * 8 + g * 8 + x] = g == 95 ? 255 : (kOverlayFont[g][y] & (0x80 >> x)) ? 255 : 0;
        glGenTextures(1, &overlayTexture);
        bindForUpdate(overlayTexture);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_R8, 96 * 8, 16);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 96 * 8, 16, GL_RED, GL_UNSIGNED_BYTE, atlas.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glGenBuffers(1, &overlayVbo);
    }
    // vertices: x, y (back-buffer pixels, y down), u, v, then colour
    std::vector<float> verts;
    const int sc = textScale(static_cast<int>(pp.BackBufferHeight));
    auto quad = [&](float x0, float y0, float x1, float y1, int glyph, uint32_t argb) {
        const float u0 = glyph / 96.0f, u1 = (glyph + 1) / 96.0f;
        const float r = ((argb >> 16) & 255) / 255.0f, g = ((argb >> 8) & 255) / 255.0f, b = (argb & 255) / 255.0f, a = ((argb >> 24) & 255) / 255.0f;
        // rows are in Direct3D order in the back buffer: y down maps to NDC bottom-up without a flip
        const float p[6][4] = {{x0, y0, u0, 0}, {x1, y0, u1, 0}, {x1, y1, u1, 1}, {x0, y0, u0, 0}, {x1, y1, u1, 1}, {x0, y1, u0, 1}};
        for (auto& v : p) { verts.insert(verts.end(), {v[0], v[1], v[2], v[3], r, g, b, a}); }
    };
    if (host::overlay::open()) {  // a dark panel behind the menu
        int minX = 1 << 30, minY = 1 << 30, maxX = 0, maxY = 0;
        for (const Line& l : ls) {
            minX = std::min(minX, l.x), minY = std::min(minY, l.y);
            maxX = std::max(maxX, l.x + static_cast<int>(l.text.size()) * 8 * sc), maxY = std::max(maxY, l.y + 16 * sc);
        }
        quad(static_cast<float>(minX - 12), static_cast<float>(minY - 12), static_cast<float>(maxX + 12), static_cast<float>(maxY + 12), 95, 0xD0101018);
    }
    for (const Line& l : ls) {
        float x = static_cast<float>(l.x);
        for (char c : l.text) {
            const int g = (c >= 32 && c < 127) ? c - 32 : 0;
            quad(x + 1, static_cast<float>(l.y) + 1, x + 8 * sc + 1, static_cast<float>(l.y + 16 * sc) + 1, g, 0xFF000000);  // shadow
            quad(x, static_cast<float>(l.y), x + 8 * sc, static_cast<float>(l.y + 16 * sc), g, l.color);
            x += 8 * sc;
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, mainFbo);
    glViewport(0, 0, static_cast<GLsizei>(pp.BackBufferWidth), static_cast<GLsizei>(pp.BackBufferHeight));
    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_CULL_FACE);
    glColorMask(1, 1, 1, 1);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glUseProgram(overlayProgram);
    glUniform2f(overlayUSize, static_cast<float>(pp.BackBufferWidth), static_cast<float>(pp.BackBufferHeight));
    glActiveTexture(GL_TEXTURE0);
    activeUnit = 0;
    glBindTexture(GL_TEXTURE_2D, overlayTexture);
    glBindSampler(0, 0);
    glBindVertexArray(overlayVao);
    glBindBuffer(GL_ARRAY_BUFFER, overlayVbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(verts.size() * sizeof(float)), verts.data(), GL_STREAM_DRAW);
    for (int i = 0; i < 16; ++i) glDisableVertexAttribArray(static_cast<GLuint>(i));
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 32, nullptr);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 32, reinterpret_cast<const void*>(16));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(verts.size() / 8));
    // the game's state is re-applied on the next draw
    glStateUnknown();
    texDirty = true;
    boundTexGl[0] = ~0u;
    samplersBound = false;  // unit 0 got the default sampler
    current = nullptr;
    layoutValid = false;
    curVao = nullptr;
    programDirty = true;
    boundIbo = ~0u;
    lastProgramGl = 0;
    glUseProgram(0);
    GLCHECK("overlay");
}

// FABLE_GLES_DRAWDUMP=<frame>: after each draw of that frame, the bound render target goes to
// FABLE_GLES_DUMP/draw_<n>.ppm (with the trace line logged by prepareDraw).
// FABLE_GLES_DRAWDUMP: a frame number, or a path: that frame (or the frame after the file
// appears; the file is removed) has every draw dumped.
bool Device::dumpFrameWanted() {
    static const char* spec = getenv("FABLE_GLES_DRAWDUMP");
    static uint32_t target = 0;
    if (!spec) return false;
    if (spec[0] == '/') {
        if (!target && access(spec, F_OK) == 0) { target = frames + 1; unlink(spec); log("gles trace: dumping frame %u", target); }
    } else if (std::strncmp(spec, "purple", 6) == 0) {
        // purple[:minframe]: dump the frame after one whose back buffer shows a patch of purple
        // (the NPC glow), from frame minframe on (the front end has purple too)
        static bool done = false;
        static const uint32_t minFrame = spec[6] == ':' ? static_cast<uint32_t>(atoi(spec + 7)) : 100u;
        if (!target && !done && frames > minFrame && mainFbo) {
            const UINT w = pp.BackBufferWidth, h = pp.BackBufferHeight;
            std::vector<unsigned char> buf(static_cast<size_t>(w) * h * 4);
            glBindFramebuffer(GL_READ_FRAMEBUFFER, mainFbo);
            glReadPixels(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h), GL_RGBA, GL_UNSIGNED_BYTE, buf.data());
            int c = 0;
            for (UINT y = 0; y < h; y += 3)
                for (UINT x = 0; x < w; x += 3) {
                    const unsigned char* q = &buf[(static_cast<size_t>(y) * w + x) * 4];
                    if (q[2] > q[0] + 15 && q[2] > q[1] + 40 && q[0] > 90 && q[1] < 150) ++c;
                }
            if (c > 60) { target = frames + 2; done = true; log("gles trace: purple (%d) in frame %u: dumping frame %u", c, frames + 1, target); }
        }
    } else if (!target) {
        target = static_cast<uint32_t>(atoi(spec));
    }
    return target && frames + 1 == target;
}

void Device::afterDraw() {
    if (!dumpFrameWanted() && !g_rtDumpThisDraw) return;
    const char* dir = getenv("FABLE_GLES_DUMP");
    if (!dir) return;
    static const bool rtOnly = getenv("FABLE_GLES_DUMP_RT") != nullptr;  // only draws into off-screen render targets
    if (rtOnly && (!renderTarget || renderTarget->isBackBuffer)) return;
    static const std::pair<unsigned, unsigned> range = [] {  // FABLE_GLES_DUMP_DRAWS=a-b: only draws a..b of the dumped frame
        const char* r = getenv("FABLE_GLES_DUMP_DRAWS"); unsigned a = 0, b = 0xFFFFFFFFu;
        if (r) sscanf(r, "%u-%u", &a, &b);
        return std::make_pair(a, b);
    }();
    if (!g_rtDumpThisDraw && (statDraws < range.first || statDraws > range.second)) return;
    UINT w = pp.BackBufferWidth, h = pp.BackBufferHeight;
    if (renderTarget && !renderTarget->isBackBuffer) w = renderTarget->level.width, h = renderTarget->level.height;
    std::vector<unsigned char> buf(static_cast<size_t>(w) * h * 4);
    glReadPixels(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h), GL_RGBA, GL_UNSIGNED_BYTE, buf.data());
    char name[512];
    std::snprintf(name, sizeof name, "%s/draw_%04u.ppm", dir, statDraws);
    if (FILE* f = fopen(name, "wb")) {
        fprintf(f, "P6\n%u %u\n255\n", w, h);
        for (UINT y = 0; y < h; ++y)
            for (UINT x = 0; x < w; ++x) fwrite(&buf[(static_cast<size_t>(y) * w + x) * 4], 1, 3, f);
        fclose(f);
    }
    log("gles trace:   -> draw %u on %s %ux%u", statDraws, renderTarget && !renderTarget->isBackBuffer ? "render target" : "back buffer", w, h);
}

// Brightness: the resolved frame to the window through a gamma exponent (a plain blit when
// neutral). A full-screen triangle from gl_VertexID, no vertex buffer.
bool Device::presentPass(int ww, int wh, float gamma) {
    if (!presentProgram) {
        const char* vs =
            "#version 300 es\nvoid main() {\n  vec2 p = vec2(gl_VertexID == 1 ? 3.0 : -1.0, gl_VertexID == 2 ? 3.0 : -1.0);\n  gl_Position = vec4(p, 0.0, 1.0);\n}\n";
        const char* fs =
            "#version 300 es\nprecision mediump float;\nuniform sampler2D uTex;\nuniform float uGamma;\nuniform vec2 uSize;\nout vec4 o;\n"
            "void main() {\n  vec2 uv = gl_FragCoord.xy / uSize;\n  vec3 c = texture(uTex, vec2(uv.x, 1.0 - uv.y)).rgb;\n"
            "  o = vec4(pow(max(c, vec3(0.0)), vec3(uGamma)), 1.0);\n}\n";
        const GLuint v = compileShader(GL_VERTEX_SHADER, vs, "present vertex shader"), f = compileShader(GL_FRAGMENT_SHADER, fs, "present fragment shader");
        presentProgram = linkProgram(v, f);
        if (v) glDeleteShader(v);
        if (f) glDeleteShader(f);
        if (!presentProgram) { log("gles: no present program: brightness ignored"); presentProgram = ~0u; }
        else {
            presentUGamma = glGetUniformLocation(presentProgram, "uGamma");
            presentUSize = glGetUniformLocation(presentProgram, "uSize");
            glUseProgram(presentProgram);
            glUniform1i(glGetUniformLocation(presentProgram, "uTex"), 0);
        }
    }
    if (presentProgram == ~0u) return false;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, ww, wh);
    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glColorMask(1, 1, 1, 1);
    glUseProgram(presentProgram);
    glUniform1f(presentUGamma, gamma);
    glUniform2f(presentUSize, static_cast<float>(ww), static_cast<float>(wh));
    glActiveTexture(GL_TEXTURE0);
    activeUnit = 0;
    glBindTexture(GL_TEXTURE_2D, mainColor);
    glBindSampler(0, 0);
    glBindVertexArray(overlayVao);
    for (int i = 0; i < 16; ++i) glDisableVertexAttribArray(static_cast<GLuint>(i));
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // the game's state is re-applied on the next draw
    glStateUnknown();
    texDirty = true;
    boundTexGl[0] = ~0u;
    samplersBound = false;  // unit 0 got the default sampler
    current = nullptr;
    layoutValid = false;
    curVao = nullptr;
    programDirty = true;
    boundIbo = ~0u;
    boundFbo = ~0u;
    lastProgramGl = 0;
    glUseProgram(0);
    GLCHECK("present pass");
    return true;
}

void Device::saveScreenshot() {
    resolveMain();
    const UINT w = pp.BackBufferWidth, h = pp.BackBufferHeight;
    std::vector<unsigned char> px(static_cast<size_t>(w) * h * 4);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, mainFbo);
    glReadPixels(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h), GL_RGBA, GL_UNSIGNED_BYTE, px.data());
    const char* dir = getenv("FABLE_RECOMP_DATA");
    static int n = 0;
    char name[512];
    std::snprintf(name, sizeof name, "%s/screenshot_%03d.bmp", dir ? dir : ".", ++n);
    FILE* f = fopen(name, "wb");
    if (!f) { log("overlay: cannot write %s", name); return; }
    // BMP, 24-bit, bottom-up rows: our rows are in Direct3D order (row 0 = top), so write them last-first
    const uint32_t rowBytes = (w * 3 + 3) & ~3u, size = 54 + rowBytes * h;
    unsigned char hdr[54] = {'B', 'M'};
    auto put32 = [&](int at, uint32_t v) { hdr[at] = v & 255, hdr[at + 1] = (v >> 8) & 255, hdr[at + 2] = (v >> 16) & 255, hdr[at + 3] = v >> 24; };
    put32(2, size), put32(10, 54), put32(14, 40), put32(18, w), put32(22, h), hdr[26] = 1, hdr[28] = 24, put32(34, rowBytes * h);
    fwrite(hdr, 1, 54, f);
    std::vector<unsigned char> row(rowBytes, 0);
    for (UINT y = h; y-- > 0;) {
        for (UINT x = 0; x < w; ++x) { const unsigned char* s = &px[(static_cast<size_t>(y) * w + x) * 4]; row[x * 3] = s[2], row[x * 3 + 1] = s[1], row[x * 3 + 2] = s[0]; }
        fwrite(row.data(), 1, rowBytes, f);
    }
    fclose(f);
    log("overlay: wrote %s", name);
}

HRESULT STDMETHODCALLTYPE Device::Present(const RECT*, const RECT*, HWND, const RGNDATA*) {
    const uint64_t t0 = nowNs();
    int ww = fbWidth, wh = fbHeight;
    if (!w32::fbdevGl()) SDL_GL_GetDrawableSize(window, &ww, &wh);
    resolveMain();      // anti-aliasing: the frame into mainColor
    dumpFrameWanted();  // FABLE_GLES_DRAWDUMP=purple scans the finished frame
    drawOverlay();
    // the depth/stencil of the frame is not needed after this: tell the tiler not to write it
    // back (the next frame clears it)
    if (real::glInvalidateFramebuffer) {
        static const GLenum ds[] = {GL_DEPTH_ATTACHMENT, GL_STENCIL_ATTACHMENT};
        glBindFramebuffer(GL_FRAMEBUFFER, msaaSamples ? msaaFbo : mainFbo);
        glInvalidateFramebuffer(GL_FRAMEBUFFER, 2, ds);
    }
    const int brightness = host::gfx::get().brightness;
    if (brightness == 50 || !presentPass(ww, wh, host::gfx::gammaExponent(brightness))) {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, mainFbo);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glDisable(GL_SCISSOR_TEST);
        glBlitFramebuffer(0, 0, static_cast<GLint>(pp.BackBufferWidth), static_cast<GLint>(pp.BackBufferHeight), 0, wh, ww, 0, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    }
    gq::call([this] {  // on the render thread: the swap (vsync wait, if any) is its time, not the game's
        const uint64_t ts = nowNs();
        if (w32::fbdevGl()) fb::swap(); else SDL_GL_SwapWindow(window);
        g_nsSwapRender.store(nowNs() - ts);
    });
    ++frames;
    {
        const uint64_t t1 = nowNs();
        g_nsGl += t1 - t0;
        frameMs = lastPresentNs ? (t1 - lastPresentNs) / 1e6 : 0;
        glMs = g_nsGl / 1e6;
        swapMs = (gq::enabled() ? g_nsSwapRender.load() : g_nsSwap) / 1e6, uploadMs = g_nsUpload / 1e6;
        g_nsGl = g_nsSwap = g_nsUpload = 0;
        bufUploads = g_bufUploads, bufInPlace = g_bufInPlace, bufKb = static_cast<double>(g_bufBytes) / 1024.0;
        g_bufUploads = g_bufInPlace = 0, g_bufBytes = 0;
        lastPresentNs = t1;
        if (frameMs > 0) perfFrames[perfN++ % kPerfWindow] = static_cast<float>(frameMs);
        perfDraws += statDraws, perfGlMs += glMs, perfSwapMs += swapMs, perfWaitMs += gq::waitMs(), perfRenderMs += gq::renderMs();
    }
    if (host::g_logLevel >= 1 && (frames % 120 == 1 || getenv("FABLE_GLES_DUMP"))) {
        // the readback runs on the render thread (a round trip here would stall the overlap)
        const unsigned fr = frames, draws = statDraws, clears = statClears, progs = static_cast<unsigned>(programs.size());
        const double fMs = frameMs, gMs = glMs, sMs = swapMs, uMs = uploadMs, rMs = gq::renderMs(), wMs = gq::waitMs(), bKb = bufKb;
        const unsigned bUp = bufUploads, bIn = bufInPlace;
        // the frame rate cap since the last line: mean sleep per frame, frames that missed its deadline
        static unsigned capFrom = 0;
        static uint64_t lineNs = 0;
        const double cMs = fr > capFrom ? g_nsCapSleep / 1e6 / (fr - capFrom) : 0;
        const double mMs = fr > capFrom && lineNs ? (lastPresentNs - lineNs) / 1e6 / (fr - capFrom) : 0;  // mean frame since the last line
        const unsigned capLate = g_capLate, capFps = static_cast<unsigned>(host::gfx::get().fpsCap);
        g_nsCapSleep = 0, g_capLate = 0, capFrom = fr, lineNs = lastPresentNs;
        // The distribution since the last line (device logs: what to tune next). Percentiles of
        // the frame, hitches, the mean draws, game-thread GL time and wait for the render
        // thread, the render thread's own time, and guest COM QueryInterface calls per frame.
        {
            const unsigned n = std::min(perfN, kPerfWindow);
            if (n >= 8) {
                float v[kPerfWindow];
                std::copy(perfFrames, perfFrames + n, v);
                std::sort(v, v + n);
                const auto pct = [&](double q) { return v[std::min<unsigned>(n - 1, static_cast<unsigned>(q * (n - 1) + 0.5))]; };
                unsigned over33 = 0, over50 = 0;
                for (unsigned i = 0; i < n; ++i) over33 += v[i] > 33.4f, over50 += v[i] > 50.0f;
                static uint32_t qiFrom = 0;
                const uint32_t qi = host::com::g_queryInterfaceCalls.load(std::memory_order_relaxed);
                log("gles: perf %u frames: p50 %.1f p90 %.1f p99 %.1f max %.1f ms, %u over 33 ms, %u over 50 ms; per frame: %.0f draws, gl %.2f ms (swap %.2f), waited %.2f, render thread %.2f, QueryInterface %.1f",
                    n, pct(0.5), pct(0.9), pct(0.99), v[n - 1], over33, over50, perfDraws / n, perfGlMs / n, perfSwapMs / n, perfWaitMs / n, perfRenderMs / n,
                    static_cast<double>(qi - qiFrom) / n);
                qiFrom = qi;
            }
            perfN = 0, perfDraws = 0, perfGlMs = perfSwapMs = perfWaitMs = perfRenderMs = 0;
        }
        const UINT bw = pp.BackBufferWidth, bh = pp.BackBufferHeight;
        const GLuint fbo = mainFbo;
        // The centre pixel (a black-screen check for device logs) is a glReadPixels: the render
        // thread waits for the GPU to finish the frame, a stall every 120 frames. Only on request.
        static const bool probe = [] { const char* e = getenv("FABLE_GLES_PROBE"); return (e && atoi(e) != 0) || getenv("FABLE_GLES_DUMP"); }();
        gq::call([=] {
            unsigned char px[4] = {0, 0, 0, 0};
            if (probe) {
                glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
                glReadPixels(static_cast<GLint>(bw / 2), static_cast<GLint>(bh / 2), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, px);
            }
            const GLenum err = glGetError();  // on the render thread: a direct call, errors of the whole frame
            char centre[16] = "-";  // FABLE_GLES_PROBE=1
            if (probe) std::snprintf(centre, sizeof centre, "%02X%02X%02X%02X", px[0], px[1], px[2], px[3]);
            log("gles: frame %u: %u draws, %u clears, %u programs, centre %s, window %dx%d, frame %.1f ms (mean %.1f) of which gl %.1f (swap %.1f, uploads %.1f), render thread %.1f, waited %.1f, buffer uploads %u (%u in place, %.0f KB), cap %u slept %.1f/frame, %u late%s%X", fr, draws, clears, progs, centre, ww, wh, fMs, mMs, gMs, sMs, uMs, rMs, wMs, bUp, bIn, bKb, capFps, cMs, capLate, err ? ", GL error 0x" : "", err);
            if (const char* dir = getenv("FABLE_GLES_DUMP")) {
                std::vector<unsigned char> buf(static_cast<size_t>(bw) * bh * 4);
                glReadPixels(0, 0, static_cast<GLint>(bw), static_cast<GLint>(bh), GL_RGBA, GL_UNSIGNED_BYTE, buf.data());
                char name[512];
                snprintf(name, sizeof name, "%s/frame_%05u.ppm", dir, fr);
                if (FILE* f = fopen(name, "wb")) {
                    fprintf(f, "P6\n%u %u\n255\n", bw, bh);
                    for (UINT y = 0; y < bh; ++y)
                        for (UINT x = 0; x < bw; ++x) fwrite(&buf[(static_cast<size_t>(y) * bw + x) * 4], 1, 3, f);
                    fclose(f);
                }
            }
        });
    }
    gq::frameEnd();  // at most one frame unreplayed: this frame's replay overlaps the next frame's game work
    if (const int cap = host::gfx::get().fpsCap) {  // hold the game thread to the cap: steadier pacing, idle cores sleep
        // `next` is the earliest time this frame may end. An early frame sleeps until then; a
        // late one never sleeps: it keeps the schedule (the next frame can catch up) unless it
        // is a whole period behind, then the schedule restarts from now. (The schedule used to
        // restart at now + period on every late frame, so each frame slower than the cap slept
        // a full extra period: 41 ms Guild frames became 74 ms at a 30 cap.)
        static uint64_t next = 0;
        const uint64_t now = nowNs(), period = 1000000000ull / static_cast<uint64_t>(cap);
        next = next ? next + period : now + period;
        if (now < next) {
            const timespec ts = {static_cast<time_t>(next / 1000000000ull), static_cast<long>(next % 1000000000ull)};
            while (clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &ts, nullptr) == EINTR) {}  // FABLE_PROFILE's SIGPROF
            g_nsCapSleep += nowNs() - now;
        } else {
            ++g_capLate;
            if (now - next > period) next = now;  // too far behind to catch up: drop the debt
        }
    }
    statDraws = statClears = 0;
    boundFbo = ~0u;
    glStateUnknown();
    GLCHECK("Present");
    return S_OK;
}

HRESULT STDMETHODCALLTYPE Device::GetDeviceCaps(D3DCAPS9* c) {
    if (!c) return E_POINTER;
    std::memset(c, 0, sizeof *c);
    c->DeviceType = D3DDEVTYPE_HAL;
    c->Caps2 = D3DCAPS2_FULLSCREENGAMMA | D3DCAPS2_DYNAMICTEXTURES;
    c->Caps3 = D3DCAPS3_ALPHA_FULLSCREEN_FLIP_OR_DISCARD;
    c->PresentationIntervals = D3DPRESENT_INTERVAL_IMMEDIATE | D3DPRESENT_INTERVAL_ONE;
    c->DevCaps = D3DDEVCAPS_HWTRANSFORMANDLIGHT | D3DDEVCAPS_HWRASTERIZATION | D3DDEVCAPS_DRAWPRIMTLVERTEX | D3DDEVCAPS_TEXTUREVIDEOMEMORY | D3DDEVCAPS_TLVERTEXVIDEOMEMORY |
                 D3DDEVCAPS_EXECUTESYSTEMMEMORY | D3DDEVCAPS_EXECUTEVIDEOMEMORY | D3DDEVCAPS_CANRENDERAFTERFLIP | D3DDEVCAPS_DRAWPRIMITIVES2 | D3DDEVCAPS_DRAWPRIMITIVES2EX | D3DDEVCAPS_PUREDEVICE;
    c->PrimitiveMiscCaps = D3DPMISCCAPS_CULLNONE | D3DPMISCCAPS_CULLCW | D3DPMISCCAPS_CULLCCW | D3DPMISCCAPS_COLORWRITEENABLE | D3DPMISCCAPS_MASKZ | D3DPMISCCAPS_BLENDOP | D3DPMISCCAPS_TSSARGTEMP;
    c->RasterCaps = D3DPRASTERCAPS_ZTEST | D3DPRASTERCAPS_FOGVERTEX | D3DPRASTERCAPS_FOGTABLE | D3DPRASTERCAPS_MIPMAPLODBIAS | D3DPRASTERCAPS_SCISSORTEST | D3DPRASTERCAPS_DEPTHBIAS | D3DPRASTERCAPS_SLOPESCALEDEPTHBIAS | D3DPRASTERCAPS_ANISOTROPY;
    c->ZCmpCaps = c->AlphaCmpCaps = 0xFF;
    c->SrcBlendCaps = c->DestBlendCaps = 0x1FFF;
    c->ShadeCaps = D3DPSHADECAPS_COLORGOURAUDRGB | D3DPSHADECAPS_SPECULARGOURAUDRGB | D3DPSHADECAPS_ALPHAGOURAUDBLEND | D3DPSHADECAPS_FOGGOURAUD;
    c->TextureCaps = D3DPTEXTURECAPS_ALPHA | D3DPTEXTURECAPS_MIPMAP | D3DPTEXTURECAPS_PROJECTED | D3DPTEXTURECAPS_PERSPECTIVE | D3DPTEXTURECAPS_CUBEMAP;
    c->TextureFilterCaps = c->CubeTextureFilterCaps = D3DPTFILTERCAPS_MINFPOINT | D3DPTFILTERCAPS_MINFLINEAR | D3DPTFILTERCAPS_MINFANISOTROPIC | D3DPTFILTERCAPS_MIPFPOINT | D3DPTFILTERCAPS_MIPFLINEAR | D3DPTFILTERCAPS_MAGFPOINT | D3DPTFILTERCAPS_MAGFLINEAR;
    c->TextureAddressCaps = D3DPTADDRESSCAPS_WRAP | D3DPTADDRESSCAPS_MIRROR | D3DPTADDRESSCAPS_CLAMP | D3DPTADDRESSCAPS_BORDER | D3DPTADDRESSCAPS_INDEPENDENTUV;
    c->LineCaps = D3DLINECAPS_TEXTURE | D3DLINECAPS_ZTEST | D3DLINECAPS_BLEND | D3DLINECAPS_ALPHACMP | D3DLINECAPS_FOG;
    c->MaxTextureWidth = c->MaxTextureHeight = 4096;
    c->MaxVolumeExtent = 256;
    c->MaxTextureRepeat = 8192;
    c->MaxTextureAspectRatio = 4096;
    c->MaxAnisotropy = 4;
    c->MaxVertexW = 1e10f;
    c->GuardBandLeft = c->GuardBandTop = -32768.0f;
    c->GuardBandRight = c->GuardBandBottom = 32768.0f;
    c->StencilCaps = 0x1FF;
    c->FVFCaps = 8 | D3DFVFCAPS_PSIZE;
    c->TextureOpCaps = 0x3FFFFFF;
    c->MaxTextureBlendStages = 8;
    c->MaxSimultaneousTextures = 8;
    c->VertexProcessingCaps = D3DVTXPCAPS_TEXGEN | D3DVTXPCAPS_MATERIALSOURCE7 | D3DVTXPCAPS_DIRECTIONALLIGHTS | D3DVTXPCAPS_POSITIONALLIGHTS | D3DVTXPCAPS_LOCALVIEWER;
    c->MaxActiveLights = 8;
    c->MaxUserClipPlanes = 0;
    c->MaxVertexBlendMatrices = 4;
    c->MaxVertexBlendMatrixIndex = 0;
    c->MaxPointSize = 64.0f;
    c->MaxPrimitiveCount = 0xFFFFF;
    c->MaxVertexIndex = 0xFFFFF;
    c->MaxStreams = 8;
    c->MaxStreamStride = 256;
    c->VertexShaderVersion = D3DVS_VERSION(1, 1);
    c->MaxVertexShaderConst = 128;
    c->PixelShaderVersion = D3DPS_VERSION(1, 4);
    c->PixelShader1xMaxValue = 8.0f;
    c->DevCaps2 = D3DDEVCAPS2_STREAMOFFSET;
    c->MaxNpatchTessellationLevel = 0;
    c->MasterAdapterOrdinal = 0;
    c->NumberOfAdaptersInGroup = 1;
    c->DeclTypes = D3DDTCAPS_UBYTE4 | D3DDTCAPS_UBYTE4N | D3DDTCAPS_SHORT2N | D3DDTCAPS_SHORT4N | D3DDTCAPS_USHORT2N | D3DDTCAPS_USHORT4N;
    c->NumSimultaneousRTs = 1;
    c->StretchRectFilterCaps = D3DPTFILTERCAPS_MINFPOINT | D3DPTFILTERCAPS_MINFLINEAR | D3DPTFILTERCAPS_MAGFPOINT | D3DPTFILTERCAPS_MAGFLINEAR;
    c->VS20Caps = {};
    c->PS20Caps = {};
    c->VertexTextureFilterCaps = 0;
    c->MaxVShaderInstructionsExecuted = 65535;
    c->MaxPShaderInstructionsExecuted = 65535;
    c->MaxVertexShader30InstructionSlots = 0;
    c->MaxPixelShader30InstructionSlots = 0;
    return S_OK;
}

// ---- IDirect3D9 ---------------------------------------------------------------------------
struct D3D9 : Unknown<IDirect3D9, &IID_IDirect3D9> {
    HRESULT STDMETHODCALLTYPE RegisterSoftwareDevice(void*) override { return S_OK; }
    UINT STDMETHODCALLTYPE GetAdapterCount() override { return 1; }
    HRESULT STDMETHODCALLTYPE GetAdapterIdentifier(UINT, DWORD, D3DADAPTER_IDENTIFIER9* id) override {
        if (!id) return E_POINTER;
        std::memset(id, 0, sizeof *id);
        std::strcpy(id->Driver, "gles");
        std::strcpy(id->Description, "OpenGL ES (Direct3D 9 on GLES)");
        std::strcpy(id->DeviceName, "\\\\.\\DISPLAY1");
        id->VendorId = 0x10DE;  // the game has vendor-specific paths; NVIDIA's are the ones it tested most
        id->DeviceId = 0x0100;
        id->DriverVersion.QuadPart = 0x0006000E00000000ll;
        return S_OK;
    }
    UINT STDMETHODCALLTYPE GetAdapterModeCount(UINT, D3DFORMAT) override { return 0; }      // handled by win/d3d9.cpp on POSIX
    HRESULT STDMETHODCALLTYPE EnumAdapterModes(UINT, D3DFORMAT, UINT, D3DDISPLAYMODE*) override { return D3DERR_INVALIDCALL; }
    HRESULT STDMETHODCALLTYPE GetAdapterDisplayMode(UINT, D3DDISPLAYMODE* m) override { if (m) *m = {1024, 768, 60, D3DFMT_X8R8G8B8}; return S_OK; }
    HRESULT STDMETHODCALLTYPE CheckDeviceType(UINT, D3DDEVTYPE, D3DFORMAT, D3DFORMAT, WINBOOL) override { return S_OK; }
    HRESULT STDMETHODCALLTYPE CheckDeviceFormat(UINT, D3DDEVTYPE, D3DFORMAT, DWORD usage, D3DRESOURCETYPE rtype, D3DFORMAT f) override {
        const FormatInfo i = formatInfo(f);
        if (!i.valid) return D3DERR_NOTAVAILABLE;
        if (rtype == D3DRTYPE_VOLUMETEXTURE || rtype == D3DRTYPE_CUBETEXTURE) return D3DERR_NOTAVAILABLE;
        if ((usage & D3DUSAGE_RENDERTARGET) && (i.depth || i.blockBytes || i.bytesPerPixel < 2)) return D3DERR_NOTAVAILABLE;
        if ((usage & D3DUSAGE_DEPTHSTENCIL) && !i.depth) return D3DERR_NOTAVAILABLE;
        if (usage & D3DUSAGE_QUERY_SRGBREAD) return D3DERR_NOTAVAILABLE;
        return S_OK;
    }
    HRESULT STDMETHODCALLTYPE CheckDeviceMultiSampleType(UINT, D3DDEVTYPE, D3DFORMAT, WINBOOL, D3DMULTISAMPLE_TYPE t, DWORD* q) override { if (q) *q = 1; return t == D3DMULTISAMPLE_NONE || t == D3DMULTISAMPLE_2_SAMPLES || t == D3DMULTISAMPLE_4_SAMPLES ? S_OK : D3DERR_NOTAVAILABLE; }
    HRESULT STDMETHODCALLTYPE CheckDepthStencilMatch(UINT, D3DDEVTYPE, D3DFORMAT, D3DFORMAT, D3DFORMAT ds) override { return formatInfo(ds).depth ? S_OK : D3DERR_NOTAVAILABLE; }
    HRESULT STDMETHODCALLTYPE CheckDeviceFormatConversion(UINT, D3DDEVTYPE, D3DFORMAT, D3DFORMAT) override { return S_OK; }
    HRESULT STDMETHODCALLTYPE GetDeviceCaps(UINT, D3DDEVTYPE, D3DCAPS9* c) override { Device tmp(this, D3DPRESENT_PARAMETERS{}); return tmp.GetDeviceCaps(c); }
    HMONITOR STDMETHODCALLTYPE GetAdapterMonitor(UINT) override { return nullptr; }
    HRESULT STDMETHODCALLTYPE CreateDevice(UINT, D3DDEVTYPE, HWND hwnd, DWORD, D3DPRESENT_PARAMETERS* p, IDirect3DDevice9** out) override {
        if (!p || !out) return E_POINTER;
        if (!p->BackBufferWidth || !p->BackBufferHeight) {
            SDL_Window* w = reinterpret_cast<SDL_Window*>(hwnd ? hwnd : p->hDeviceWindow);
            int ww = 1024, wh = 768;
            if (w) SDL_GetWindowSize(w, &ww, &wh);
            p->BackBufferWidth = static_cast<UINT>(ww), p->BackBufferHeight = static_cast<UINT>(wh);
        }
        if (p->BackBufferFormat == D3DFMT_UNKNOWN) p->BackBufferFormat = D3DFMT_X8R8G8B8;
        Device* d = new Device(this, *p);
        if (!d->init(hwnd)) { d->Release(); *out = nullptr; return D3DERR_NOTAVAILABLE; }
        g_device = d;
        *out = d;
        return S_OK;
    }
};

}  // namespace

IDirect3D9* createD3D9() {
    log("Direct3D 9: OpenGL ES renderer");
    return new D3D9();
}

}  // namespace host::gles
