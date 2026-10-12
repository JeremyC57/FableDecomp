#include "census.hpp"

#include "host.hpp"

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace host::census {
namespace {

std::string g_dir;
std::map<std::string, unsigned> g_calls;
std::map<std::string, std::set<std::string>> g_values;  // "SetRenderState 27" -> {"1", "2"}
std::map<std::string, unsigned> g_valueCounts;
unsigned g_frames = 0;
int g_shaderN = 0;

void note(const char* key, const std::string& value) {
    auto& s = g_values[key];
    if (s.size() < 64) s.insert(value);
    ++g_valueCounts[std::string(key) + " " + value];
}
void note(const std::string& key, const std::string& value) { note(key.c_str(), value); }
std::string hex(uint32_t v) { char b[16]; std::snprintf(b, sizeof b, "0x%X", v); return b; }
std::string dec(long v) { return std::to_string(v); }

void report() {
    const std::string path = g_dir + "/census.txt";
    FILE* f = std::fopen(path.c_str(), "w");
    if (!f) return;
    std::fprintf(f, "frames %u\n\n== calls ==\n", g_frames);
    for (auto& [k, n] : g_calls) std::fprintf(f, "%-40s %u\n", k.c_str(), n);
    std::fprintf(f, "\n== values ==\n");
    for (auto& [k, s] : g_values) {
        std::fprintf(f, "%s:", k.c_str());
        for (auto& v : s) std::fprintf(f, " %s(%u)", v.c_str(), g_valueCounts[k + " " + v]);
        std::fprintf(f, "%s\n", s.size() >= 64 ? " ..." : "");
    }
    std::fclose(f);
}

struct CensusDevice : IDirect3DDevice9 {
    IDirect3DDevice9* inner;
    explicit CensusDevice(IDirect3DDevice9* d) : inner(d) {}
#define COUNT ++g_calls[__func__]
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** out) override { return inner->QueryInterface(riid, out); }
    ULONG STDMETHODCALLTYPE AddRef() override { return inner->AddRef(); }
    ULONG STDMETHODCALLTYPE Release() override {
        const ULONG n = inner->Release();
        if (n == 0) { report(); delete this; }
        return n;
    }
#include "census_decl.inc"
    // ---- hand-written hooks ----
    HRESULT STDMETHODCALLTYPE SetRenderState(D3DRENDERSTATETYPE s, DWORD v) override {
        COUNT; note(("SetRenderState " + dec(s)).c_str(), hex(v)); return inner->SetRenderState(s, v);
    }
    HRESULT STDMETHODCALLTYPE SetTextureStageState(DWORD st, D3DTEXTURESTAGESTATETYPE t, DWORD v) override {
        COUNT; note(("SetTextureStageState " + dec(st) + " type " + dec(t)).c_str(), hex(v)); return inner->SetTextureStageState(st, t, v);
    }
    HRESULT STDMETHODCALLTYPE SetSamplerState(DWORD s, D3DSAMPLERSTATETYPE t, DWORD v) override {
        COUNT; note(("SetSamplerState type " + dec(t)).c_str(), hex(v)); note("SetSamplerState sampler", dec(s)); return inner->SetSamplerState(s, t, v);
    }
    HRESULT STDMETHODCALLTYPE SetFVF(DWORD fvf) override { COUNT; note("SetFVF", hex(fvf)); return inner->SetFVF(fvf); }
    HRESULT STDMETHODCALLTYPE SetTransform(D3DTRANSFORMSTATETYPE s, const D3DMATRIX* m) override { COUNT; note("SetTransform", dec(s)); return inner->SetTransform(s, m); }
    HRESULT STDMETHODCALLTYPE SetLight(DWORD i, const D3DLIGHT9* l) override { COUNT; note("SetLight type", l ? dec(l->Type) : "null"); return inner->SetLight(i, l); }
    HRESULT STDMETHODCALLTYPE LightEnable(DWORD i, BOOL e) override { COUNT; note("LightEnable", dec(i) + (e ? " on" : " off")); return inner->LightEnable(i, e); }
    HRESULT STDMETHODCALLTYPE SetMaterial(const D3DMATERIAL9* m) override { COUNT; return inner->SetMaterial(m); }
    HRESULT STDMETHODCALLTYPE SetVertexShader(IDirect3DVertexShader9* s) override { COUNT; note("SetVertexShader", s ? "shader" : "null"); return inner->SetVertexShader(s); }
    HRESULT STDMETHODCALLTYPE SetPixelShader(IDirect3DPixelShader9* s) override { COUNT; note("SetPixelShader", s ? "shader" : "null"); return inner->SetPixelShader(s); }
    void dumpShader(const char* kind, const DWORD* code) {
        size_t n = 0;
        while (code[n] != 0x0000FFFF) ++n;
        ++n;
        note(std::string(kind) + " version", hex(code[0]));
        note(std::string(kind) + " tokens", dec((long)n));
        char name[64];
        std::snprintf(name, sizeof name, "/%s_%03d_%08X.bin", kind, g_shaderN++, code[0]);
        if (FILE* f = std::fopen((g_dir + name).c_str(), "wb")) { std::fwrite(code, 4, n, f); std::fclose(f); }
    }
    HRESULT STDMETHODCALLTYPE CreateVertexShader(const DWORD* code, IDirect3DVertexShader9** out) override { COUNT; dumpShader("vs", code); return inner->CreateVertexShader(code, out); }
    HRESULT STDMETHODCALLTYPE CreatePixelShader(const DWORD* code, IDirect3DPixelShader9** out) override { COUNT; dumpShader("ps", code); return inner->CreatePixelShader(code, out); }
    HRESULT STDMETHODCALLTYPE CreateTexture(UINT w, UINT h, UINT levels, DWORD usage, D3DFORMAT fmt, D3DPOOL pool, IDirect3DTexture9** out, HANDLE* sh) override {
        COUNT; note("CreateTexture fmt", dec(fmt)); note("CreateTexture usage/pool", hex(usage) + "/" + dec(pool)); note("CreateTexture levels", dec(levels));
        note("CreateTexture size", dec(w) + "x" + dec(h)); return inner->CreateTexture(w, h, levels, usage, fmt, pool, out, sh);
    }
    HRESULT STDMETHODCALLTYPE CreateCubeTexture(UINT e, UINT levels, DWORD usage, D3DFORMAT fmt, D3DPOOL pool, IDirect3DCubeTexture9** out, HANDLE* sh) override {
        COUNT; note("CreateCubeTexture fmt/usage", dec(fmt) + "/" + hex(usage)); return inner->CreateCubeTexture(e, levels, usage, fmt, pool, out, sh);
    }
    HRESULT STDMETHODCALLTYPE CreateVolumeTexture(UINT w, UINT h, UINT d, UINT levels, DWORD usage, D3DFORMAT fmt, D3DPOOL pool, IDirect3DVolumeTexture9** out, HANDLE* sh) override {
        COUNT; note("CreateVolumeTexture fmt/usage", dec(fmt) + "/" + hex(usage)); return inner->CreateVolumeTexture(w, h, d, levels, usage, fmt, pool, out, sh);
    }
    HRESULT STDMETHODCALLTYPE CreateVertexBuffer(UINT len, DWORD usage, DWORD fvf, D3DPOOL pool, IDirect3DVertexBuffer9** out, HANDLE* sh) override {
        COUNT; note("CreateVertexBuffer usage/fvf/pool", hex(usage) + "/" + hex(fvf) + "/" + dec(pool)); return inner->CreateVertexBuffer(len, usage, fvf, pool, out, sh);
    }
    HRESULT STDMETHODCALLTYPE CreateIndexBuffer(UINT len, DWORD usage, D3DFORMAT fmt, D3DPOOL pool, IDirect3DIndexBuffer9** out, HANDLE* sh) override {
        COUNT; note("CreateIndexBuffer usage/fmt/pool", hex(usage) + "/" + dec(fmt) + "/" + dec(pool)); return inner->CreateIndexBuffer(len, usage, fmt, pool, out, sh);
    }
    HRESULT STDMETHODCALLTYPE CreateVertexDeclaration(const D3DVERTEXELEMENT9* el, IDirect3DVertexDeclaration9** out) override {
        COUNT;
        std::string d;
        for (const D3DVERTEXELEMENT9* e = el; e->Stream != 0xFF; ++e)
            d += dec(e->Stream) + ":" + dec(e->Offset) + ":t" + dec(e->Type) + ":u" + dec(e->Usage) + "." + dec(e->UsageIndex) + " ";
        note("CreateVertexDeclaration", d);
        return inner->CreateVertexDeclaration(el, out);
    }
    HRESULT STDMETHODCALLTYPE CreateRenderTarget(UINT w, UINT h, D3DFORMAT fmt, D3DMULTISAMPLE_TYPE ms, DWORD q, BOOL lock, IDirect3DSurface9** out, HANDLE* sh) override {
        COUNT; note("CreateRenderTarget", dec(w) + "x" + dec(h) + " fmt " + dec(fmt) + (lock ? " lockable" : "")); return inner->CreateRenderTarget(w, h, fmt, ms, q, lock, out, sh);
    }
    HRESULT STDMETHODCALLTYPE CreateDepthStencilSurface(UINT w, UINT h, D3DFORMAT fmt, D3DMULTISAMPLE_TYPE ms, DWORD q, BOOL disc, IDirect3DSurface9** out, HANDLE* sh) override {
        COUNT; note("CreateDepthStencilSurface", dec(w) + "x" + dec(h) + " fmt " + dec(fmt)); return inner->CreateDepthStencilSurface(w, h, fmt, ms, q, disc, out, sh);
    }
    HRESULT STDMETHODCALLTYPE CreateOffscreenPlainSurface(UINT w, UINT h, D3DFORMAT fmt, D3DPOOL pool, IDirect3DSurface9** out, HANDLE* sh) override {
        COUNT; note("CreateOffscreenPlainSurface", dec(w) + "x" + dec(h) + " fmt " + dec(fmt) + " pool " + dec(pool)); return inner->CreateOffscreenPlainSurface(w, h, fmt, pool, out, sh);
    }
    HRESULT STDMETHODCALLTYPE CreateQuery(D3DQUERYTYPE t, IDirect3DQuery9** out) override { COUNT; note("CreateQuery", dec(t)); return inner->CreateQuery(t, out); }
    HRESULT STDMETHODCALLTYPE SetRenderTarget(DWORD i, IDirect3DSurface9* s) override { COUNT; note("SetRenderTarget index", dec(i) + (s ? "" : " null")); return inner->SetRenderTarget(i, s); }
    HRESULT STDMETHODCALLTYPE SetDepthStencilSurface(IDirect3DSurface9* s) override { COUNT; note("SetDepthStencilSurface", s ? "surface" : "null"); return inner->SetDepthStencilSurface(s); }
    HRESULT STDMETHODCALLTYPE DrawPrimitive(D3DPRIMITIVETYPE t, UINT a, UINT n) override { COUNT; note("DrawPrimitive type", dec(t)); return inner->DrawPrimitive(t, a, n); }
    HRESULT STDMETHODCALLTYPE DrawIndexedPrimitive(D3DPRIMITIVETYPE t, INT b, UINT mi, UINT nv, UINT si, UINT pc) override { COUNT; note("DrawIndexedPrimitive type", dec(t)); return inner->DrawIndexedPrimitive(t, b, mi, nv, si, pc); }
    HRESULT STDMETHODCALLTYPE DrawPrimitiveUP(D3DPRIMITIVETYPE t, UINT n, const void* d, UINT s) override { COUNT; note("DrawPrimitiveUP type/stride", dec(t) + "/" + dec(s)); return inner->DrawPrimitiveUP(t, n, d, s); }
    HRESULT STDMETHODCALLTYPE DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE t, UINT mi, UINT nv, UINT pc, const void* idx, D3DFORMAT f, const void* d, UINT s) override {
        COUNT; note("DrawIndexedPrimitiveUP type/stride/fmt", dec(t) + "/" + dec(s) + "/" + dec(f)); return inner->DrawIndexedPrimitiveUP(t, mi, nv, pc, idx, f, d, s);
    }
    HRESULT STDMETHODCALLTYPE SetVertexShaderConstantF(UINT r, const float* d, UINT n) override { COUNT; note("SetVertexShaderConstantF max", dec(r + n)); return inner->SetVertexShaderConstantF(r, d, n); }
    HRESULT STDMETHODCALLTYPE SetVertexShaderConstantI(UINT r, const int* d, UINT n) override { COUNT; return inner->SetVertexShaderConstantI(r, d, n); }
    HRESULT STDMETHODCALLTYPE SetVertexShaderConstantB(UINT r, const BOOL* d, UINT n) override { COUNT; return inner->SetVertexShaderConstantB(r, d, n); }
    HRESULT STDMETHODCALLTYPE SetPixelShaderConstantF(UINT r, const float* d, UINT n) override { COUNT; note("SetPixelShaderConstantF max", dec(r + n)); return inner->SetPixelShaderConstantF(r, d, n); }
    HRESULT STDMETHODCALLTYPE SetPixelShaderConstantI(UINT r, const int* d, UINT n) override { COUNT; return inner->SetPixelShaderConstantI(r, d, n); }
    HRESULT STDMETHODCALLTYPE SetPixelShaderConstantB(UINT r, const BOOL* d, UINT n) override { COUNT; return inner->SetPixelShaderConstantB(r, d, n); }
    HRESULT STDMETHODCALLTYPE SetStreamSource(UINT s, IDirect3DVertexBuffer9* b, UINT off, UINT stride) override { COUNT; note("SetStreamSource stream/stride", dec(s) + "/" + dec(stride)); return inner->SetStreamSource(s, b, off, stride); }
    HRESULT STDMETHODCALLTYPE SetStreamSourceFreq(UINT s, UINT f) override { COUNT; note("SetStreamSourceFreq", hex(f)); return inner->SetStreamSourceFreq(s, f); }
    HRESULT STDMETHODCALLTYPE SetIndices(IDirect3DIndexBuffer9* b) override { COUNT; return inner->SetIndices(b); }
    HRESULT STDMETHODCALLTYPE SetTexture(DWORD st, IDirect3DBaseTexture9* t) override {
        COUNT; note("SetTexture stage", dec(st) + (t ? " type " + dec(t->GetType()) : " null")); return inner->SetTexture(st, t);
    }
    HRESULT STDMETHODCALLTYPE SetClipPlane(DWORD i, const float* p) override { COUNT; return inner->SetClipPlane(i, p); }
    HRESULT STDMETHODCALLTYPE StretchRect(IDirect3DSurface9* a, const RECT* ar, IDirect3DSurface9* b, const RECT* br, D3DTEXTUREFILTERTYPE f) override { COUNT; return inner->StretchRect(a, ar, b, br, f); }
    HRESULT STDMETHODCALLTYPE GetRenderTargetData(IDirect3DSurface9* a, IDirect3DSurface9* b) override { COUNT; return inner->GetRenderTargetData(a, b); }
    HRESULT STDMETHODCALLTYPE ColorFill(IDirect3DSurface9* s, const RECT* r, D3DCOLOR c) override { COUNT; return inner->ColorFill(s, r, c); }
    HRESULT STDMETHODCALLTYPE UpdateTexture(IDirect3DBaseTexture9* a, IDirect3DBaseTexture9* b) override { COUNT; return inner->UpdateTexture(a, b); }
    HRESULT STDMETHODCALLTYPE UpdateSurface(IDirect3DSurface9* a, const RECT* r, IDirect3DSurface9* b, const POINT* p) override { COUNT; return inner->UpdateSurface(a, r, b, p); }
    HRESULT STDMETHODCALLTYPE SetScissorRect(const RECT* r) override { COUNT; return inner->SetScissorRect(r); }
    HRESULT STDMETHODCALLTYPE BeginStateBlock() override { COUNT; return inner->BeginStateBlock(); }
    HRESULT STDMETHODCALLTYPE EndStateBlock(IDirect3DStateBlock9** out) override { COUNT; return inner->EndStateBlock(out); }
    HRESULT STDMETHODCALLTYPE CreateStateBlock(D3DSTATEBLOCKTYPE t, IDirect3DStateBlock9** out) override { COUNT; note("CreateStateBlock", dec(t)); return inner->CreateStateBlock(t, out); }
    HRESULT STDMETHODCALLTYPE SetPaletteEntries(UINT n, const PALETTEENTRY* e) override { COUNT; return inner->SetPaletteEntries(n, e); }
    HRESULT STDMETHODCALLTYPE Clear(DWORD n, const D3DRECT* r, DWORD flags, D3DCOLOR c, float z, DWORD s) override { COUNT; note("Clear flags/rects", hex(flags) + "/" + dec(n)); return inner->Clear(n, r, flags, c, z, s); }
    HRESULT STDMETHODCALLTYPE SetViewport(const D3DVIEWPORT9* v) override { COUNT; return inner->SetViewport(v); }
    HRESULT STDMETHODCALLTYPE Present(const RECT* a, const RECT* b, HWND h, const RGNDATA* d) override {
        COUNT;
        if (++g_frames % 2000 == 0) report();
        return inner->Present(a, b, h, d);
    }
#undef COUNT
};
#include "census_fwd.inc"

}  // namespace

IDirect3DDevice9* wrap(IDirect3DDevice9* inner) {
    const char* dir = std::getenv("FABLE_D3D_CENSUS");
    if (!dir || !*dir || !inner) return inner;
    g_dir = dir;
    log("D3D9 census: recording to %s", dir);
    return new CensusDevice(inner);
}

}  // namespace host::census
