// Behaviour test for CDisplayManager::GetBestOpaqueTextureFormat (0x009BE610) over a small format table.
#include <cstdio>
struct IDirect3D9 {
    long supportedMask; unsigned long lastUsage; int calls;
    virtual long CheckDeviceFormat(unsigned long, long, long, unsigned long usage, long type, long format)
    { ++calls; lastUsage = usage; return (type == 3 && (supportedMask >> format) & 1) ? 0 : -1; }
};
enum { D3DRTYPE_TEXTURE = 3 };
struct CD3DFormatInfo { long D3DFormat, PFType, BitDepth, AlphaBits, RBits, GBits, BBits, StencilBits; bool Linear; };
static const CD3DFormatInfo g_D3DFormatInfo[] = {
    { 1, 2, 32, 8, 8, 8, 8, 0, false },   // A8R8G8B8
    { 2, 2, 32, 0, 8, 8, 8, 0, false },   // X8R8G8B8
    { 3, 2, 16, 1, 5, 5, 5, 0, false },   // A1R5G5B5
    { 4, 2, 16, 4, 4, 4, 4, 0, false },   // A4R4G4B4
    { 5, 6, 16, 0, 8, 8, 0, 0, false },   // signed
    { 6, 2, 16, 0, 5, 6, 5, 0, false },   // R5G6B5
    { 0, 0, -1, 0, 0, 0, 0, 0, false } };
struct CPixelFormat { long FormatIndex; void Initialise(long f) { FormatIndex = f; } };
struct CDisplayManager {
    IDirect3D9* D3D; long DeviceType; unsigned long AdapterOrdinal; long CurrentModeFormat;
    bool GetBestOpaqueTextureFormat(long bitDepth, CPixelFormat& out, bool renderTarget);
};
bool CDisplayManager::GetBestOpaqueTextureFormat(long bitDepth, CPixelFormat& out, bool renderTarget)
{
    const CD3DFormatInfo* best = 0;
    for (long i = 0; g_D3DFormatInfo[i].BitDepth != -1; ++i) {
        const CD3DFormatInfo& info = g_D3DFormatInfo[i];
        if (info.PFType == 2 && info.BitDepth == bitDepth && info.RBits > 0 && info.GBits > 0 && info.BBits > 0
            && (best == 0 || info.AlphaBits < best->AlphaBits)
            && D3D->CheckDeviceFormat(AdapterOrdinal, DeviceType, CurrentModeFormat, renderTarget ? 1 : 0, D3DRTYPE_TEXTURE, info.D3DFormat) >= 0)
            best = &info;
    }
    if (best != 0) {
        out.Initialise(best->D3DFormat);
        return true;
    }
    return false;
}

int main()
{
    IDirect3D9 d3d; d3d.supportedMask = 0x7e; d3d.calls = 0;
    CDisplayManager m; m.D3D = &d3d; m.DeviceType = 1; m.AdapterOrdinal = 0; m.CurrentModeFormat = 22;
    CPixelFormat pf; pf.FormatIndex = 0; int bad = 0;
    if (!m.GetBestOpaqueTextureFormat(32, pf, false) || pf.FormatIndex != 2 || d3d.lastUsage != 0) bad = 1;   // X8R8G8B8: no alpha
    if (!m.GetBestOpaqueTextureFormat(16, pf, true) || pf.FormatIndex != 6 || d3d.lastUsage != 1) bad = 2;    // R5G6B5 (signed skipped)
    d3d.supportedMask = 0x18;   // only A1R5G5B5 and A4R4G4B4 supported
    if (!m.GetBestOpaqueTextureFormat(16, pf, false) || pf.FormatIndex != 3) bad = 3;   // fewest alpha bits
    pf.FormatIndex = 99;
    if (m.GetBestOpaqueTextureFormat(24, pf, false) || pf.FormatIndex != 99) bad = 4;
    if (bad) { std::printf("009be610_TEST FAIL\n"); return 1; }
    std::printf("009be610_TEST PASS\n");
    return 0;
}