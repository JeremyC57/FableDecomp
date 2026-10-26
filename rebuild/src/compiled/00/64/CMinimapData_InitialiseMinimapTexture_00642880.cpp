// CMinimapData::InitialiseMinimapTexture() -- (re)create the minimap texture at the minimap's size and pixel format and copy the built minimap surface into its top level
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00642880, length 166 bytes (`ret`).
// Callees: CTexture::Uninitialise @ 0x009F9F70, CSurface::GetPixelFormat @ 0x009F2F60 (CPixelFormat by hidden pointer), CTexture::Initialise
// (const C2DExtentsI*, long, const CPixelFormat*, ulong, ESurfacePool, bool, bool) @ 0x009FA280, CTexture::GetSurface(long level) @ 0x009F9E00
// (a CSurface returned by hidden pointer; labelled GetNoLevels), CSurface::CopyFromSurface(const C2DBoxI*, const CSurface*, const C2DBoxI*,
// ulong) @ 0x009F37E0, CSurface::Uninitialise @ 0x009F2E20 (the temporary's inline destructor after its vtable store, 0x0122F84C).
// Layout: Ego_r.pdb CMinimapData with retail's 12-byte vector: MinimapExtents +0x00, MinimapSurface +0x14, MinimapTexture +0x24
// (CTexture: PD3DTexture, then the bit fields).
// Shape: the extents are copied into a local, the pixel format is a by-value result, the copy box (0, 0, X, Y) is built from the local
// extents after the Initialise call, and the level-0 surface is a temporary used as `this` of the copy.
struct IDirect3DTexture9;
struct IDirect3DSurface9;
class C2DExtentsI { public:
    long X, Y;
};
class C2DBoxI { public:
    long X0, Y0, X1, Y1;
    C2DBoxI(long x0, long y0, long x1, long y1) : X0(x0), Y0(y0), X1(x1), Y1(y1) { }
};
class CPixelFormat { public:
    long FormatIndex;
    CPixelFormat() { }
};
class CSurface { public:                               // vtable 0x0122F84C
    IDirect3DSurface9* PD3DSurface;             // +0x04
    unsigned long      AllocationSource;        // +0x08
    void*              PAllocatedMemory;        // +0x0c
    virtual void v00();
    ~CSurface() { Uninitialise(); }
    void Uninitialise();                        // 0x009F2E20
    CPixelFormat GetPixelFormat() const;        // 0x009F2F60
    void CopyFromSurface(const C2DBoxI* destBox, const CSurface* source, const C2DBoxI* sourceBox, unsigned long flags);   // 0x009F37E0
};
class CTexture { public:
    IDirect3DTexture9* PD3DTexture;             // +0x00
    unsigned long      Bits;                    // +0x04 ByteLength:28, AllocationSource:4
    bool IsInitialised() const { return PD3DTexture != 0; }
    void Uninitialise();                        // 0x009F9F70
    bool Initialise(const C2DExtentsI* size, long levels, const CPixelFormat* format, unsigned long usage, long pool, bool a, bool b);   // 0x009FA280
    CSurface GetSurface(long level);            // 0x009F9E00
};
template<class T> struct CRetailVector { T* _M_start; T* _M_finish; T* _M_end_of_storage; };
class CMinimapData { public:
    C2DExtentsI         MinimapExtents;         // +0x00
    CRetailVector<long> MinimapThemeList;       // +0x08
    CSurface            MinimapSurface;         // +0x14
    CTexture            MinimapTexture;         // +0x24
    unsigned long       MaxMinimapThemeColours; // +0x2c
    CRetailVector<long> ColourMinimapTheme;     // +0x30
    void InitialiseMinimapTexture();
};

void CMinimapData::InitialiseMinimapTexture()
{
    if (MinimapTexture.IsInitialised())
        MinimapTexture.Uninitialise();
    C2DExtentsI size = MinimapExtents;
    CPixelFormat format = MinimapSurface.GetPixelFormat();
    MinimapTexture.Initialise(&size, 1, &format, 0, 1, false, false);
    C2DBoxI box(0, 0, size.X, size.Y);
    MinimapTexture.GetSurface(0).CopyFromSurface(&box, &MinimapSurface, &box, 0);
}