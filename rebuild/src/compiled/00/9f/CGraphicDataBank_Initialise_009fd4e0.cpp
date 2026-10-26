// CGraphicDataBank::Initialise(const CGraphicDataBankInit& init) -- set the bank's memory/frame-delay policy, derive the eleven per-texture-type pixel formats from the init block and create a 32x32 blank texture (full mip chain, cleared level 0) for every valid format
// Leaf: Initialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FD4E0, length 341 bytes (`ret 4`).
// Callees: CTexture::Initialise(const C2DExtentsI*, long, const CPixelFormat*, ulong, ESurfacePool, bool, bool) @ 0x009FA280,
//   CTexture::GetSurface(long) @ 0x009F9E00 (CSurface by hidden pointer; labelled GetNoLevels), CSurface::Clear @ 0x009F40A0,
//   CSurface::Uninitialise @ 0x009F2E20 (the temporary's inline destructor after its vtable store, 0x0122F84C), and an empty
//   bank notification taking a bool @ 0x009D5230 (manifest "$E2"; descriptive name).
// Layout: Ego_r.pdb names (CGraphicDataBankInit; CGraphicDataBank TextureSizeWarningMaxWidth .. BlankTextures) at the retail
//   offsets, which sit 0x7C below the PDB (12-byte retail vectors in the bases): TextureSizeWarningMaxWidth +0x260,
//   UnloadFrameDelay +0x268, FreezeGraphics +0x27C, PixelFormats +0x288, BlankTextures +0x2B4, Initialised +0x1F8; the
//   resource bank's frame delay word +0x19C. The landed CMinimapData::InitialiseMinimapTexture 0x00642880 texture model.
struct IDirect3DTexture9;
struct IDirect3DSurface9;
class C2DExtentsI { public: long X, Y; };
class CPixelFormat { public: long FormatIndex; };
class CSurface { public:                               // vtable 0x0122F84C
    IDirect3DSurface9* PD3DSurface;             // +0x04
    unsigned long      AllocationSource;        // +0x08
    void*              PAllocatedMemory;        // +0x0c
    virtual void v00();
    ~CSurface() { Uninitialise(); }
    void Uninitialise();                        // 0x009F2E20
    void Clear();                               // 0x009F40A0
};
class CTexture { public:
    IDirect3DTexture9* PD3DTexture;             // +0x00
    unsigned long      Bits;                    // +0x04
    bool Initialise(const C2DExtentsI* size, long levels, const CPixelFormat* format, unsigned long usage, long pool, bool a, bool b);   // 0x009FA280
    CSurface GetSurface(long level);            // 0x009F9E00
};
class CGraphicDataBankInit { public:
    CPixelFormat NonAlphaFormat;                // +0x00
    CPixelFormat AlphaFormat;                   // +0x04
    CPixelFormat InterpolatedAlphaFormat;       // +0x08
    CPixelFormat BooleanAlphaFormat;            // +0x0C
    CPixelFormat UncompressedNonAlphaFormat;    // +0x10
    CPixelFormat UncompressedAlphaFormat;       // +0x14
    CPixelFormat SignedFormat;                  // +0x18
    bool          GenerateMipmaps;              // +0x1C
    unsigned long MaxGraphicWidth;              // +0x20
    unsigned long MaxGraphicHeight;             // +0x24
};
class CGraphicDataBank { public:
    unsigned char _pad000[0x19c];
    unsigned long ResourceFrameDelay;           // +0x19C (resource bank base)
    unsigned char _pad1a0[0x58];
    bool          Initialised;                  // +0x1F8
    unsigned char _pad1f9[0x67];
    long          TextureSizeWarningMaxWidth;   // +0x260
    long          TextureSizeWarningMaxHeight;  // +0x264
    unsigned long UnloadFrameDelay;             // +0x268
    unsigned long ReduceSizeFrameDelay;         // +0x26C
    unsigned long TexturePointersValidForFrames;   // +0x270
    long          MinAvailablePreloadMemory;    // +0x274
    long          AvailableMemory;              // +0x278
    bool          FreezeGraphics;               // +0x27C
    unsigned char _pad27d[0xb];
    CPixelFormat  PixelFormats[11];             // +0x288
    CTexture      BlankTextures[11];            // +0x2B4
    void NotifyInitialised(bool initialised);   // 0x009D5230
    void Initialise(const CGraphicDataBankInit& init);
};

void CGraphicDataBank::Initialise(const CGraphicDataBankInit& init)
{
    UnloadFrameDelay = 30;
    ReduceSizeFrameDelay = 3;
    TexturePointersValidForFrames = 1;
    MinAvailablePreloadMemory = 0x200000;
    FreezeGraphics = false;
    ResourceFrameDelay = 3;
    PixelFormats[0] = init.NonAlphaFormat;
    PixelFormats[1] = init.AlphaFormat;
    PixelFormats[2] = init.BooleanAlphaFormat;
    PixelFormats[3] = init.InterpolatedAlphaFormat;
    PixelFormats[4] = init.NonAlphaFormat;
    PixelFormats[5] = init.AlphaFormat;
    PixelFormats[6] = init.UncompressedNonAlphaFormat;
    PixelFormats[7] = init.UncompressedAlphaFormat;
    PixelFormats[8] = init.UncompressedNonAlphaFormat;
    PixelFormats[9] = init.UncompressedAlphaFormat;
    PixelFormats[10] = init.SignedFormat;
    TextureSizeWarningMaxWidth = 512;
    TextureSizeWarningMaxHeight = 512;
    for (long i = 0; i < 11; ++i) {
        CPixelFormat format = PixelFormats[i];
        if (format.FormatIndex != -1) {
            C2DExtentsI size;
            size.X = 32;
            size.Y = 32;
            BlankTextures[i].Initialise(&size, -1, &format, 0, 1, false, false);
            BlankTextures[i].GetSurface(0).Clear();
        }
    }
    AvailableMemory = 0;
    NotifyInitialised(true);
    Initialised = true;
}