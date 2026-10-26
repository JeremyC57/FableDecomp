// CTexture::Initialise(const C2DExtentsI& size, long mipLevels, const CPixelFormat& format, unsigned long usage, long pool, bool, bool) -- release any texture held, derive the mip count (halve both extents while both are at least 8, plus one) when none is given, create the D3D texture on the render device; records the allocation source on success
// Leaf: Initialise
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FA280, 181 bytes (+11 int3). `ret 1Ch` (seven arguments, the last two unused).
// Callees: IDirect3DTexture9::Release (slot 2, COM `__stdcall`), GFGetSystemManager 0x009A4EC0 (`__fastcall`; device = system
//          +0x60 -> +0x58), IDirect3DDevice9::CreateTexture (slot 23, +0x5c), CTexture::CalcByteLength 0x009F9EE0. The format's
//          D3DFORMAT comes from the 0x24-byte pixel-format table @ 0x0129BA40 (`lea edx,[ecx+ecx*8]` then `*4`).
// Shape: the held texture released through the COM slot with `this` pushed; the bitfield word is masked to clear
//        AllocationSource after the pointer is nulled; the mip loop keeps both extents unsigned (`jb`/`jae`) with the
//        width test at the loop bottom; the device fetched into a named local before the eight right-to-left pushes;
//        `hr < 0` is `test eax,eax ; jge` with `xor al,al` on the failure exit; AllocationSource = 1 is the masked `or`.
// Layout: engine/CTexture.h -- PD3DTexture +0x00, ByteLength (28 bits) + AllocationSource (4 bits) at +0x04.
struct IDirect3DTexture9 {
    virtual long __stdcall QueryInterface(const void* riid, void** ppv) = 0;
    virtual unsigned long __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;   // slot 2 (+0x08)
};
struct IDirect3DDevice9 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22();
    virtual long __stdcall CreateTexture(unsigned long width, unsigned long height, unsigned long levels, unsigned long usage,
                                         long format, long pool, IDirect3DTexture9** ppTexture, void** pSharedHandle) = 0;   // slot 23 (+0x5c)
};
struct CRenderSystem {
    unsigned char      _pad00[0x58];
    IDirect3DDevice9*  m_pDevice;           // +0x58
};
class CSystemManager { public:
    unsigned char  _pad00[0x60];
    CRenderSystem* m_pRender;               // +0x60
};
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0
struct SPixelFormatInfo { long d3dFormat; long rest[8]; };   // 0x24 bytes
extern SPixelFormatInfo g_pixelFormatInfo[];                 // 0x0129BA40
class CPixelFormat { public:
    long m_format;
    long GetD3DFormat() const { return g_pixelFormatInfo[m_format].d3dFormat; }
};
class C2DExtentsI { public: long X, Y; };
class CTexture { public:
    IDirect3DTexture9* PD3DTexture;         // +0x00
    unsigned long ByteLength : 28;          // +0x04
    unsigned long AllocationSource : 4;
    void CalcByteLength();                  // 0x009F9EE0
    bool Initialise(const C2DExtentsI& size, long mipLevels, const CPixelFormat& format, unsigned long usage, long pool, bool, bool);
};
bool CTexture::Initialise(const C2DExtentsI& size, long mipLevels, const CPixelFormat& format, unsigned long usage, long pool, bool, bool)
{
    if (PD3DTexture) {
        PD3DTexture->Release();
        PD3DTexture = 0;
        AllocationSource = 0;
    }
    long levels = mipLevels;
    if (levels <= 0) {
        unsigned long w = size.X;
        unsigned long h = size.Y;
        long n = 0;
        while (w >= 8 && h >= 8) {
            w >>= 1;
            h >>= 1;
            ++n;
        }
        levels = n + 1;
    }
    IDirect3DDevice9* device = GFGetSystemManager()->m_pRender->m_pDevice;
    if (device->CreateTexture(size.X, size.Y, levels, usage, format.GetD3DFormat(), pool, &PD3DTexture, 0) < 0)
        return false;
    AllocationSource = 1;
    CalcByteLength();
    return true;
}