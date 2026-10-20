#include <cstdio>
#include <cstdlib>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __cdecl
#define __int64 long long
#endif
struct IDirect3DTexture9 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual long __stdcall GetLevelDesc(unsigned int level, struct D3DSURFACE_DESC* desc);   // slot 17
};
struct D3DSURFACE_DESC {
    long          Format;               // +0x00 D3DFORMAT
    long          Type;                 // +0x04
    unsigned long Usage;                // +0x08
    long          Pool;                 // +0x0c
    long          MultiSampleType;      // +0x10
    unsigned long MultiSampleQuality;   // +0x14
    unsigned int  Width;                // +0x18
    unsigned int  Height;               // +0x1c
};
struct BITMAPINFOHEADER {
    unsigned long  biSize;              // +0x00
    long           biWidth;             // +0x04
    long           biHeight;            // +0x08
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned long  biCompression;
    unsigned long  biSizeImage;
    long           biXPelsPerMeter;
    long           biYPelsPerMeter;
    unsigned long  biClrUsed;
    unsigned long  biClrImportant;
};
struct VIDEOINFOHEADER {
    long             rcSource[4];       // +0x00
    long             rcTarget[4];       // +0x10
    unsigned long    dwBitRate;         // +0x20
    unsigned long    dwBitErrorRate;    // +0x24
    __int64          AvgTimePerFrame;   // +0x28
    BITMAPINFOHEADER bmiHeader;         // +0x30
};
struct CMediaType {
    unsigned char  majortype[16];       // +0x00
    unsigned char  subtype[16];         // +0x10
    int            bFixedSizeSamples;   // +0x20
    int            bTemporalCompression;// +0x24
    unsigned long  lSampleSize;         // +0x28
    unsigned char  formattype[16];      // +0x2c
    void*          pUnk;                // +0x3c
    unsigned long  cbFormat;            // +0x40
    unsigned char* pbFormat;            // +0x44
};
struct C2DExtentsI {
    long X, Y;
    C2DExtentsI(long x, long y) : X(x), Y(y) {}
};
struct CPixelFormat {
    long FormatIndex;
    void Initialise(long r, long g, long b, long a, bool flag);   // 0x009E3790
};
enum ESurfacePool { SURFACE_POOL_0, SURFACE_POOL_MANAGED };
struct CTexture {
    IDirect3DTexture9* PD3DTexture;     // +0x00
    unsigned long      ByteLength : 28; // +0x04
    unsigned long      AllocationSource : 4;
    CTexture() : PD3DTexture(0), ByteLength(0), AllocationSource(0) {}
    bool Initialise(const C2DExtentsI* size, long mipLevels, const CPixelFormat* format, unsigned long usage,
                    ESurfacePool pool, bool a, bool b);           // 0x009FA280
};
struct CVideoSys {
    unsigned char _pad00[0x84];
    long          Height;               // +0x84 (retail layout)
    long          Width;                // +0x88
};
struct CTextureRenderer {
    unsigned char _base[0x160];         // CBaseVideoRenderer
    CVideoSys*    PVideoSys;            // +0x160
    CTexture*     Texture;              // +0x164
    long          TextureFormat;        // +0x168
    int           Width;                // +0x16c
    int           Height;               // +0x170
    int           Pitch;                // +0x174
    bool          CreateOwnTexture;     // +0x178
    long SetMediaType(const CMediaType* pmt);
};


long CTextureRenderer::SetMediaType(const CMediaType* pmt)
{
    VIDEOINFOHEADER* info = (VIDEOINFOHEADER*)pmt->pbFormat;
    Width = info->bmiHeader.biWidth;
    Height = abs(info->bmiHeader.biHeight);
    PVideoSys->Height = Height;
    PVideoSys->Width = Width;
    Pitch = (Width * 3 + 3) & ~3;
    if (Texture == 0) {
        Texture = new CTexture;
        CPixelFormat format;
        format.Initialise(8, 8, 8, 8, false);
        C2DExtentsI size(Width, Height);
        Texture->Initialise(&size, 1, &format, 0, SURFACE_POOL_MANAGED, false, false);
    } else {
        D3DSURFACE_DESC desc;
        Texture->PD3DTexture->GetLevelDesc(0, &desc);
        Width = desc.Width;
        Height = desc.Height;
    }
    D3DSURFACE_DESC desc;
    long hr = Texture->PD3DTexture->GetLevelDesc(0, &desc);
    if (hr < 0)
        return hr;
    TextureFormat = desc.Format;
    if (TextureFormat != 21 && TextureFormat != 25)
        return 0x8004022A;
    return 0;
}

void IDirect3DTexture9::v00() {} void IDirect3DTexture9::v01() {} void IDirect3DTexture9::v02() {}
void IDirect3DTexture9::v03() {} void IDirect3DTexture9::v04() {} void IDirect3DTexture9::v05() {}
void IDirect3DTexture9::v06() {} void IDirect3DTexture9::v07() {} void IDirect3DTexture9::v08() {}
void IDirect3DTexture9::v09() {} void IDirect3DTexture9::v10() {} void IDirect3DTexture9::v11() {}
void IDirect3DTexture9::v12() {} void IDirect3DTexture9::v13() {} void IDirect3DTexture9::v14() {}
void IDirect3DTexture9::v15() {} void IDirect3DTexture9::v16() {}
static int g_descCalls;
static long g_descFormat;
static long g_descHr;
long __stdcall IDirect3DTexture9::GetLevelDesc(unsigned int level, D3DSURFACE_DESC* desc)
{
    ++g_descCalls;
    desc->Format = g_descFormat;
    desc->Width = 128;
    desc->Height = 64;
    return level == 0 ? g_descHr : -1;
}
static IDirect3DTexture9 g_d3dTex;
static int g_pfCalls, g_pfArgs;
void CPixelFormat::Initialise(long r, long g, long b, long a, bool flag)
{
    ++g_pfCalls;
    g_pfArgs = (r == 8 && g == 8 && b == 8 && a == 8 && !flag);
    FormatIndex = 3;
}
static int g_initCalls, g_initOk;
bool CTexture::Initialise(const C2DExtentsI* size, long mipLevels, const CPixelFormat* format, unsigned long usage,
                          ESurfacePool pool, bool a, bool b)
{
    ++g_initCalls;
    g_initOk = (size->X == 100 && size->Y == 50 && mipLevels == 1 && format->FormatIndex == 3 && usage == 0
                && pool == SURFACE_POOL_MANAGED && !a && !b && PD3DTexture == 0 && ByteLength == 0);
    PD3DTexture = &g_d3dTex;
    return true;
}

int main()
{
    int fails = 0;
    CVideoSys vs;
    CTextureRenderer r;
    r.PVideoSys = &vs;
    r.Texture = 0;
    VIDEOINFOHEADER vih;
    vih.bmiHeader.biWidth = 100;
    vih.bmiHeader.biHeight = -50;
    CMediaType mt;
    mt.pbFormat = (unsigned char*)&vih;
    // first call: creates the texture, keeps the media size
    g_descFormat = 21; g_descHr = 0;
    long hr = r.SetMediaType(&mt);
    if (hr != 0) ++fails;
    if (r.Width != 100 || r.Height != 50 || vs.Width != 100 || vs.Height != 50) ++fails;
    if (r.Pitch != 300) ++fails;
    if (!r.Texture || g_initCalls != 1 || !g_initOk || g_pfCalls != 1 || !g_pfArgs) ++fails;
    if (g_descCalls != 1 || r.TextureFormat != 21) ++fails;
    // second call: size read back from the existing texture; pitch from the media size
    vih.bmiHeader.biWidth = 33;
    vih.bmiHeader.biHeight = 20;
    g_descFormat = 25;
    hr = r.SetMediaType(&mt);
    if (hr != 0 || g_initCalls != 1 || g_descCalls != 3) ++fails;
    if (r.Width != 128 || r.Height != 64 || vs.Width != 33 || vs.Height != 20 || r.Pitch != 100) ++fails;
    if (r.TextureFormat != 25) ++fails;
    // rejected format
    g_descFormat = 22;
    if (r.SetMediaType(&mt) != (long)0x8004022A) ++fails;
    // failing GetLevelDesc result propagates; format left unchanged
    g_descFormat = 21; g_descHr = (long)(int)0x80004005;
    if (r.SetMediaType(&mt) != (long)(int)0x80004005 || r.TextureFormat != 22) ++fails;
    delete r.Texture;
    if (fails == 0) std::printf("00a3b5f0_TEST PASS\n");
    else std::printf("00a3b5f0_TEST FAIL %d\n", fails);
    return fails;
}