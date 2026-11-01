// CSurface::SetPixel(long x, long y, const CRGBColour& colour, const CSurfaceLockedRect& rect) -- write one pixel into a locked surface: take the surface's pixel format (GetPixelFormat, inlined), address the pixel at PMem + y * Pitch + x * bytes-per-pixel and let the format encode the colour there
// Leaf: ?SetPixel@CSurface@@QAEXJJABVCRGBColour@@ABVCSurfaceLockedRect@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F31C0, length 82 bytes.
// Naming wave 26 (DECISIONS_W25 D7): Ego_r.pdb / FableWin.pdb ?SetPixel@CSurface@@QAEXJJABVCRGBColour@@ABVCSurfaceLockedRect@@@Z -- the colour
//   by const reference (retail `mov eax,[esp+30h] ; push eax` forwards the argument word unchanged: the same bytes for the reference),
//   non-const; the encoder 0x009E3F60 is ?ConvertRGBToPFDData@CPixelFormat@@QBEXABVCRGBColour@@PAE@Z and 0x009E3830
//   ?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z (Ego_r.pdb; the inline format constructor calls it -- naming wave 25). Was
//   CSurface_Methods::SetPixel(long, long, CRGBColour, ...) const over a CPixelFormatFromD3D stand-in with WriteColour(CRGBColour) by value.
// Callees: IDirect3DSurface9::GetDesc (COM slot 12), CPixelFormat::Initialise(_D3DFORMAT) @ 0x009E3830,
// CPixelFormat::GetByteColourDepth @ 0x009E3800, CPixelFormat::ConvertRGBToPFDData(colour, destination) @ 0x009E3F60.
// Shape: 0x24-byte frame = the CPixelFormat local (+0, the inlined GetPixelFormat's hidden result) and its
// D3DSURFACE_DESC (+4); `ret 10h` (x, y, the colour and the locked rect by reference).
// The address is `PMem + y*Pitch + x*depth` with the depth a named local (retail: depth in eax, `imul eax,[x]`,
// then `add edx,eax ; add edx,[PMem]`). Tried: the same sum with the depth call inline (86 v 82: the depth copied
// to ecx, rect in eax) and a named `long offset` local added to PMem (84 v 82).
// Model: engine/CSurface.h (+4 PD3DSurface) as the layout base of CSurface (the header alias); GetPixelFormat (0x009F2F60) is
//   defined in the same TU and auto-inlined, as in retail; PDB CSurfaceLockedRect.
enum _D3DFORMAT { D3DFMT_UNKNOWN = 0, D3DFMT_A8R8G8B8 = 21, D3DFMT_D24S8 = 75, D3DFMT_DXT1 = 0x31545844, D3DFMT_DXT3 = 0x33545844, D3DFMT_DXT5 = 0x35545844, D3DFMT_FORCE_DWORD = 0x7fffffff };
#define CSurface CSurfaceLayout
#include "engine/CSurface.h"
#undef CSurface

struct D3DSURFACE_DESC_Local {                     // D3DSURFACE_DESC, 0x20 bytes
    _D3DFORMAT    Format;                          // +0x00 D3DFORMAT
    unsigned long Type;                            // +0x04
    unsigned long Usage;                           // +0x08
    unsigned long Pool;                            // +0x0c
    unsigned long MultiSampleType;                 // +0x10
    unsigned long MultiSampleQuality;              // +0x14
    unsigned long Width;                           // +0x18
    unsigned long Height;                          // +0x1c
};

// The COM surface (completes the header's forward declaration): IUnknown + IDirect3DResource9 slots.
struct IDirect3DSurface9 {
    virtual long __stdcall QueryInterface(const void* iid, void** object);   // slot 0
    virtual unsigned long __stdcall AddRef();                                // slot 1 (+0x04)
    virtual unsigned long __stdcall Release();                               // slot 2 (+0x08)
    virtual long __stdcall GetDevice(void** device);                         // slot 3
    virtual long __stdcall SetPrivateData(const void* guid, const void* data, unsigned long size, unsigned long flags);
    virtual long __stdcall GetPrivateData(const void* guid, void* data, unsigned long* size);
    virtual long __stdcall FreePrivateData(const void* guid);
    virtual unsigned long __stdcall SetPriority(unsigned long priority);
    virtual unsigned long __stdcall GetPriority();
    virtual void __stdcall PreLoad();
    virtual long __stdcall GetType();
    virtual long __stdcall GetContainer(const void* iid, void** container);  // slot 11
    virtual long __stdcall GetDesc(D3DSURFACE_DESC_Local* desc);             // slot 12 (+0x30)
    virtual long __stdcall LockRect(void* lockedRect, const void* rect, unsigned long flags);   // slot 13 (+0x34)
    virtual long __stdcall UnlockRect();                                     // slot 14 (+0x38)
};

class CRGBColour { public:                                // 4 bytes
    unsigned char B, G, R, A;
};

class C2DExtentsI { public:
    long Width;
    long Height;
};

class CSurfaceLockedRect { public:                        // PDB CSurfaceLockedRect, 16 bytes
    C2DExtentsI    Dimensions;                     // +0x00
    long           Pitch;                          // +0x08
    unsigned char* PMem;                           // +0x0c
};

// CPixelFormat with its D3DFORMAT constructor and depth queries (the engine header holds only the layout).
class CPixelFormat { public:
    long FormatIndex;                              // +0x00 (PDB CPixelFormat::FormatIndex)
    CPixelFormat(_D3DFORMAT d3dFormat) { Initialise(d3dFormat); }
    void Initialise(_D3DFORMAT d3dFormat);         // 0x009E3830
    unsigned long GetByteColourDepth() const;      // 0x009E3800 (bytes per pixel)
    void ConvertRGBToPFDData(const CRGBColour& colour, unsigned char* data) const;   // 0x009E3F60 (encode one pixel)
};

class CSurface : public CSurfaceLayout { public:
    CPixelFormat GetPixelFormat() const;
    void SetPixel(long x, long y, const CRGBColour& colour, const CSurfaceLockedRect& rect);
};

CPixelFormat CSurface::GetPixelFormat() const
{
    D3DSURFACE_DESC_Local desc;
    PD3DSurface->GetDesc(&desc);
    return CPixelFormat(desc.Format);
}

void CSurface::SetPixel(long x, long y, const CRGBColour& colour, const CSurfaceLockedRect& rect)
{
    CPixelFormat format = GetPixelFormat();
    long depth = format.GetByteColourDepth();
    format.ConvertRGBToPFDData(colour, rect.PMem + y * rect.Pitch + x * depth);
}
