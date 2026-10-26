// CManagedTextureData::CreateTexture(unsigned long width, unsigned long height, long mipLevels, const CPixelFormat& format) -- round the requested size up to the next dimensions the device supports for the format, record them as the texture's width/height, and (re)initialise the managed CTexture at that size (all mip levels, managed pool); the success flag is stored in Allocated and returned
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A697A0, length 100 bytes.
// Callees: GetNextValidDimensions(const C2DExtentsI&, const CPixelFormat&) @ 0x009FA620 (`__fastcall`, C2DExtentsI by hidden result in ecx),
//   CTexture::Initialise(const C2DExtentsI&, long, const CPixelFormat&, ulong, ESurfacePool, bool, bool) @ 0x009FA280.
// Identity: PDB ?CreateTexture@CManagedTextureData@@QAE_NKKJABVCPixelFormat@@@Z; the mip-level argument is unused (retail passes -1).
// Layout (engine/CManagedTextureData.h, PDB): Width +0x1c, Height +0x1e (unsigned short), Texture (CCountedPointer<CTexture>) +0x20,
//   Allocated +0x48 -- the header is a flat POD without methods, so the same members are declared locally to carry the method.
// Shape: the request and the result are two C2DExtentsI frame locals (user constructor, so the result comes back through a hidden
//   pointer); the result fields are stored to Width/Height and copied back into the request local, whose address goes to Initialise.
class C2DExtentsI { public:
    long Width;
    long Height;
    C2DExtentsI() { }
    C2DExtentsI(long w, long h) : Width(w), Height(h) { }
};
class CPixelFormat;
enum ESurfacePool { ESP_DEFAULT = 0, ESP_MANAGED = 1 };
C2DExtentsI __fastcall GetNextValidDimensions(const C2DExtentsI& requested, const CPixelFormat& format);   // 0x009FA620
class CTexture { public:
    bool Initialise(const C2DExtentsI& size, long mipLevels, const CPixelFormat& format, unsigned long usage, ESurfacePool pool,
                    bool renderTarget, bool dynamic);      // 0x009FA280
};
class CCPPointerInfo;
class CManagedTextureData { public:
    void*          __vftable;                      // +0x00
    unsigned char  _pad04[0x18];
    unsigned short Width;                          // +0x1c
    unsigned short Height;                         // +0x1e
    CTexture*      Texture_Data;                   // +0x20 CCountedPointer<CTexture>
    CCPPointerInfo* Texture_Info;                  // +0x24
    unsigned char  _pad28[0x20];
    bool           Allocated;                      // +0x48
    bool           Locked;                         // +0x49
    bool           BusyFlag;                       // +0x4a
    bool CreateTexture(unsigned long width, unsigned long height, long mipLevels, const CPixelFormat& format);
};
bool CManagedTextureData::CreateTexture(unsigned long width, unsigned long height, long mipLevels, const CPixelFormat& format)
{
    C2DExtentsI size(width, height);
    C2DExtentsI actual = GetNextValidDimensions(size, format);
    Width = (unsigned short)actual.Width;
    Height = (unsigned short)actual.Height;
    size = actual;
    Allocated = Texture_Data->Initialise(size, -1, format, 0, ESP_MANAGED, false, false);
    return Allocated;
}