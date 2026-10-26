#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#endif
namespace N_0099af00 { struct CWideString {
    void* PData;
    ~CWideString();                                         // 0x0099B510
    operator const unsigned short*() const;                 // 0x0099AF00
}; } using N_0099af00::CWideString;
CWideString __fastcall ConvertPathToFullPath(const CWideString& path);   // 0x00999110
struct IDirect3DDevice9;
struct CRenderSystem {
    unsigned char      _pad00[0x58];
    IDirect3DDevice9*  m_pDevice;           // +0x58
};
class CSystemManager { public:
    unsigned char  _pad00[0x60];
    CRenderSystem* m_pRender;               // +0x60
};
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0
struct IDirect3DTexture9 {
    virtual long __stdcall QueryInterface(const void* iid, void** object);   // slot 0
    virtual unsigned long __stdcall AddRef();                                // slot 1
    virtual unsigned long __stdcall Release();                               // slot 2
};
struct D3DXIMAGE_INFO {
    unsigned long Width;                    // +0x00
    unsigned long Height;                   // +0x04
    unsigned long Depth;                    // +0x08
    unsigned long MipLevels;                // +0x0c
    long          Format;                   // +0x10 D3DFORMAT
    long          ResourceType;             // +0x14
    long          ImageFileFormat;          // +0x18
};
long __stdcall D3DXCreateTextureFromFileExW(IDirect3DDevice9* device, const unsigned short* srcFile,
    unsigned long width, unsigned long height, unsigned long mipLevels, unsigned long usage, long format, long pool,
    unsigned long filter, unsigned long mipFilter, unsigned long colorKey, D3DXIMAGE_INFO* srcInfo, void* palette,
    IDirect3DTexture9** texture);           // 0x00BFEFD4
struct SPixelFormatInfo { long d3dFormat; long rest[8]; };   // 0x24 bytes
extern SPixelFormatInfo g_pixelFormatInfo[];
class C2DExtentsI { public:
    long X, Y;
    C2DExtentsI() {}
    C2DExtentsI(long x, long y) : X(x), Y(y) {}
};
class CPixelFormat { public:
    long FormatIndex;
    void Initialise(long d3dFormat);                         // 0x009E3830
};
struct CTextureFileInfo {
    C2DExtentsI  Size;                      // +0x00
    long         MipLevels;                 // +0x08
    CPixelFormat Format;                    // +0x0c
};
class CTexture {
public:
    IDirect3DTexture9* PD3DTexture;                // +0x00
    unsigned long      ByteLength : 28;            // +0x04
    unsigned long      AllocationSource : 4;       // +0x04
    void Uninitialise()                            // 0x009F9F70 (inlined here)
    {
        if (PD3DTexture) {
            PD3DTexture->Release();
            PD3DTexture = 0;
            AllocationSource = 0;
        }
    }
    void CalcByteLength();                         // 0x009F9EE0
    bool LoadFromBitmapFile(const CWideString& filename, const C2DExtentsI* size, const CPixelFormat* format,
                            unsigned long mipLevels, unsigned long usage, long pool, unsigned long filter,
                            unsigned long mipFilter, CTextureFileInfo* info, long);
};

bool CTexture::LoadFromBitmapFile(const CWideString& filename, const C2DExtentsI* size, const CPixelFormat* format,
                                  unsigned long mipLevels, unsigned long usage, long pool, unsigned long filter,
                                  unsigned long mipFilter, CTextureFileInfo* info, long)
{
    CWideString path = ConvertPathToFullPath(filename);
    Uninitialise();
    unsigned long width = (unsigned long)-1;
    unsigned long height = (unsigned long)-1;
    long d3dFormat = 0;
    if (size) {
        width = size->X;
        height = size->Y;
    }
    if (format)
        d3dFormat = g_pixelFormatInfo[format->FormatIndex].d3dFormat;
    IDirect3DDevice9* device = GFGetSystemManager()->m_pRender->m_pDevice;
    D3DXIMAGE_INFO imageInfo;
    long result = D3DXCreateTextureFromFileExW(device, path, width, height,
                                               mipLevels, usage, d3dFormat, pool, filter, mipFilter, 0, &imageInfo, 0,
                                               &PD3DTexture);
    if (result < 0)
        return false;
    if (result != 0)
        return false;
    if (info) {
        info->Size = C2DExtentsI(imageInfo.Width, imageInfo.Height);
        info->MipLevels = imageInfo.MipLevels;
        info->Format.Initialise(imageInfo.Format);
    }
    CalcByteLength();
    return true;
}
// ---- test doubles ----
static int g_paths = 0;
CWideString::~CWideString() { --g_paths; }
static unsigned short g_full[] = { 'C', ':', '/', 'x', 0 };
CWideString::operator const unsigned short*() const { return (const unsigned short*)PData; }
CWideString __fastcall ConvertPathToFullPath(const CWideString&) { static CWideString w; w.PData = g_full; ++g_paths; return w; }
static IDirect3DDevice9* g_device = (IDirect3DDevice9*)0x1234;
static CRenderSystem g_render; static CSystemManager g_system;
CSystemManager* __fastcall GFGetSystemManager() { return &g_system; }
static int g_released = 0;
struct CTestTex : IDirect3DTexture9 {
    virtual long __stdcall QueryInterface(const void*, void**) { return 0; }
    virtual unsigned long __stdcall AddRef() { return 1; }
    virtual unsigned long __stdcall Release() { ++g_released; return 0; }
};
long __stdcall IDirect3DTexture9::QueryInterface(const void*, void**) { return 0; }
unsigned long __stdcall IDirect3DTexture9::AddRef() { return 0; }
unsigned long __stdcall IDirect3DTexture9::Release() { return 0; }
static CTestTex g_old, g_new;
static long g_hr = 0; static unsigned long g_w, g_h; static long g_fmt; static bool g_argsOk;
long __stdcall D3DXCreateTextureFromFileExW(IDirect3DDevice9* device, const unsigned short* file, unsigned long width,
    unsigned long height, unsigned long mips, unsigned long usage, long format, long pool, unsigned long filter,
    unsigned long mipFilter, unsigned long colorKey, D3DXIMAGE_INFO* info, void* palette, IDirect3DTexture9** out)
{
    g_w = width; g_h = height; g_fmt = format;
    g_argsOk = device == g_device && file == g_full && mips == 3 && usage == 4 && pool == 1 && filter == 5 && mipFilter == 6
        && colorKey == 0 && palette == 0;
    info->Width = 64; info->Height = 32; info->MipLevels = 7; info->Format = 21;
    if (g_hr == 0) *out = &g_new;
    return g_hr;
}
SPixelFormatInfo g_pixelFormatInfo[4] = { { 0 }, { 0 }, { 25 }, { 0 } };
static long g_initFormat = 0; static int g_calcs = 0;
void CPixelFormat::Initialise(long f) { g_initFormat = f; FormatIndex = 99; }
void CTexture::CalcByteLength() { ++g_calcs; ByteLength = 123; }
int main()
{
    g_render.m_pDevice = g_device; g_system.m_pRender = &g_render;
    CTexture t; t.PD3DTexture = &g_old; t.ByteLength = 5; t.AllocationSource = 3;
    CWideString name; name.PData = 0;
    CTextureFileInfo info;
    bool ok = t.LoadFromBitmapFile(name, 0, 0, 3, 4, 1, 5, 6, &info, 0) && g_released == 1 && t.PD3DTexture == &g_new
        && t.AllocationSource == 0 && g_w == (unsigned long)-1 && g_h == (unsigned long)-1 && g_fmt == 0 && g_argsOk
        && info.Size.X == 64 && info.Size.Y == 32 && info.MipLevels == 7 && g_initFormat == 21 && g_calcs == 1
        && t.ByteLength == 123 && g_paths == 0;
    C2DExtentsI size(16, 8); CPixelFormat pf; pf.FormatIndex = 2;
    ok = ok && t.LoadFromBitmapFile(name, &size, &pf, 3, 4, 1, 5, 6, 0, 0) && g_w == 16 && g_h == 8 && g_fmt == 25 && g_calcs == 2;
    g_hr = 1;
    ok = ok && !t.LoadFromBitmapFile(name, &size, &pf, 3, 4, 1, 5, 6, 0, 0) && g_calcs == 2 && g_paths == 0;
    g_hr = -5;
    ok = ok && !t.LoadFromBitmapFile(name, &size, &pf, 3, 4, 1, 5, 6, 0, 0) && g_calcs == 2 && g_paths == 0;
    g_paths = 1000;   /* the static path copy is destroyed at exit */
    if (!ok) { std::printf("009fa340_TEST FAIL\n"); return 1; }
    std::printf("009fa340_TEST PASS\n");
    return 0;
}