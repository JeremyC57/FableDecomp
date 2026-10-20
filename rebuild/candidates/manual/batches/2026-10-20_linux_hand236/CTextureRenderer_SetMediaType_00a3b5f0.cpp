// CTextureRenderer::SetMediaType(const CMediaType* pmt) -- DirectShow renderer: take the video size from the VIDEOINFO header, publish it to the video system, create the 8-bit-per-channel render texture on first use (else read the size back from the existing texture), and accept only A8R8G8B8 / A1R5G5B5 textures
// Leaf: ?SetMediaType@CTextureRenderer
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3B5F0, 305 bytes (+15 int3), `ret 4`. CTextureRenderer vtable 0x0129D08C slot 36
//   (CBaseRenderer::SetMediaType override).
// Callees: operator new @ 0x00BFEA1A, CPixelFormat::Initialise(long, long, long, long, bool) @ 0x009E3790,
//   CTexture::Initialise(const C2DExtentsI*, long, const CPixelFormat*, unsigned long, ESurfacePool, bool, bool) @ 0x009FA280,
//   IDirect3DTexture9::GetLevelDesc (COM slot 17, `call [ecx+44h]`).
// Layout: PDB CTextureRenderer members (PVideoSys +0x160, Texture +0x164, TextureFormat +0x168, Width +0x16c, Height +0x170,
//   Pitch +0x174) after the 0x160-byte CBaseVideoRenderer base. CVideoSys is a local model: retail stores the video
//   height/width at +0x84/+0x88 (the PDB's CVideoSys puts Height/Width at +0x8c/+0x90, so the header is not used).
//   AM_MEDIA_TYPE pbFormat at +0x44; VIDEOINFOHEADER bmiHeader at +0x30 (biWidth +0x34, biHeight +0x38);
//   D3DSURFACE_DESC Format +0x00, Width +0x18, Height +0x1c.
// Shape: height = abs(biHeight) through the abs intrinsic (cdq/xor/sub; a hand-written ?: emits test/jge/neg); pitch = (width * 3 + 3) & ~3; `new CTexture` with an inline zeroing ctor;
//   the pixel format is a named local (built in the dead pmt slot); two separate surface descriptors.
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

extern "C" int __cdecl abs(int value);
#pragma intrinsic(abs)

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
