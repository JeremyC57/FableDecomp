#include <cstdio>
static long g_args[5]; static long g_hr = 0;
struct IDirect3D9 {
    virtual long __stdcall v0() { return 0; } virtual unsigned long __stdcall v1() { return 0; } virtual unsigned long __stdcall v2() { return 0; } virtual long __stdcall v3(void*) { return 0; }
    virtual unsigned long __stdcall v4() { return 0; } virtual long __stdcall v5(unsigned long, unsigned long, void*) { return 0; } virtual unsigned long __stdcall v6(unsigned long, long) { return 0; }
    virtual long __stdcall v7(unsigned long, long, unsigned long, void*) { return 0; } virtual long __stdcall v8(unsigned long, void*) { return 0; }
    virtual long __stdcall v9(unsigned long, long, long, long, int) { return 0; } virtual long __stdcall v10(unsigned long, long, long, unsigned long, long, long) { return 0; }
    virtual long __stdcall v11(unsigned long, long, long, int, long, unsigned long*) { return 0; }
    virtual long __stdcall CheckDepthStencilMatch(unsigned long adapter, long devType, long adapterFormat, long rt, long ds) { g_args[0] = adapter; g_args[1] = devType; g_args[2] = adapterFormat; g_args[3] = rt; g_args[4] = ds; return g_hr; }
};
class CPixelFormat { public: long FormatIndex; };
struct SPixelFormatInfo { long m_d3dFormat; unsigned char _rest[0x20]; };
SPixelFormatInfo g_PixelFormatTable[3] = { { 21 }, { 22 }, { 75 } };
class CDisplayManager { public:
    unsigned char _pad[0x54]; IDirect3D9* D3D; void* D3DDevice; long D3DDeviceCaps_DeviceType; unsigned long D3DDeviceCaps_AdapterOrdinal; unsigned char _caps[0x1b8 - 0x64];
    unsigned long CurrentMode_Width, CurrentMode_Height, CurrentMode_RefreshRate; long CurrentMode_Format;
    static long GetD3DFormat(const CPixelFormat& f) { return g_PixelFormatTable[f.FormatIndex].m_d3dFormat; }
    bool IsValidRenderTargetFormat(const CPixelFormat& rt, const CPixelFormat& ds) { return D3D->CheckDepthStencilMatch(D3DDeviceCaps_AdapterOrdinal, D3DDeviceCaps_DeviceType, CurrentMode_Format, GetD3DFormat(rt), GetD3DFormat(ds)) >= 0; }
};
int main()
{
    IDirect3D9 d3d; CDisplayManager m; m.D3D = &d3d; m.D3DDeviceCaps_DeviceType = 1; m.D3DDeviceCaps_AdapterOrdinal = 0; m.CurrentMode_Format = 22;
    CPixelFormat rt = { 1 }, ds = { 2 };
    g_hr = 0; bool ok = m.IsValidRenderTargetFormat(rt, ds); g_hr = -2005530518; bool bad = m.IsValidRenderTargetFormat(rt, ds);
    if (!ok || bad || g_args[0] != 0 || g_args[1] != 1 || g_args[2] != 22 || g_args[3] != 22 || g_args[4] != 75 || sizeof(SPixelFormatInfo) != 36
        || (char*)&m.CurrentMode_Format - (char*)&m != 0x1c4) { std::printf("009be930_TEST FAIL\n"); return 1; }
    std::printf("009be930_TEST PASS\n"); return 0;
}