// CRegionMinimap::InitialiseTexture() -- (re)create the region minimap texture at the minimap size in the best compressed alpha format and copy the minimap surface into its top level
// Leaf: InitialiseTexture
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0066CB00, length 179 bytes (`ret`).
// Callees: CTexture::Uninitialise @ 0x009F9F70, GFGetSystemManager @ 0x009A4EC0, CDisplayManager::
// GetBestCompressedAlphaTextureFormat(ulong, CPixelFormat*) @ 0x009BE870 (the result is ignored), CTexture::Initialise
// (const C2DExtentsI*, long, const CPixelFormat*, ulong, ESurfacePool, bool, bool) @ 0x009FA280, CTexture::GetSurface(long)
// @ 0x009F9E00 (CSurface by hidden pointer; manifest label GetNoLevels, landed as CTexture_GetSurfaceLevel), CSurface::
// CopyFromSurface(const C2DBoxI*, const CSurface*, const C2DBoxI*, ulong) @ 0x009F37E0, CSurface::Uninitialise @ 0x009F2E20
// (the surface temporary's inline destructor after its vtable store, 0x0122F84C = CSurface).
// Layout: engine/CRegionMinimap.h (RegionMinimapSize +0x00, RegionMinimapSurface +0x24, RegionMinimapTexture +0x34); the
// opaque CSurface/CTexture members are reached through inline accessors.
// Shape: the twin of the landed CMinimapData::InitialiseMinimapTexture 0x00642880: a square C2DExtentsI local from the one
// size member, the system manager taken into a local before the pushes, a CPixelFormat local whose inline constructor stores -1 and is then filled by the display manager, the
// copy box (0, 0, X, Y) built after Initialise (its zero stores sink below the pushes) and the level-0 surface temporary
// used as `this` of the copy with flags 1.
#include "engine/CRegionMinimap.h"

struct IDirect3DSurface9;
struct C2DExtentsI {
    long X, Y;
};
struct C2DBoxI {
    long X0, Y0, X1, Y1;
    C2DBoxI(long x0, long y0, long x1, long y1) : X0(x0), Y0(y0), X1(x1), Y1(y1) { }
};
struct CPixelFormat {
    long FormatIndex;
    CPixelFormat() : FormatIndex(-1) { }
};
struct CDisplayManager {
    bool GetBestCompressedAlphaTextureFormat(unsigned long flags, CPixelFormat* format);   // 0x009BE870
};
struct CSystemManager {
    unsigned char    _pad00[0x60];
    CDisplayManager* PDisplayManager;           // +0x60 CCountedPointer<CDisplayManager>
};
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0

struct CSurface {                               // vtable 0x0122F84C
    IDirect3DSurface9* PD3DSurface;             // +0x04
    unsigned long      AllocationSource;        // +0x08
    void*              PAllocatedMemory;        // +0x0c
    virtual void v00();
    ~CSurface() { Uninitialise(); }
    void Uninitialise();                        // 0x009F2E20
    void CopyFromSurface(const C2DBoxI* destBox, const CSurface* source, const C2DBoxI* sourceBox, unsigned long flags);   // 0x009F37E0
};
struct CTexture {
    IDirect3DTexture9* PD3DTexture;             // +0x00
    unsigned long      Bits;                    // +0x04 ByteLength:28, AllocationSource:4
    bool IsInitialised() const { return PD3DTexture != 0; }
    void Uninitialise();                        // 0x009F9F70
    bool Initialise(const C2DExtentsI* size, long levels, const CPixelFormat* format, unsigned long usage, long pool, bool a, bool b);   // 0x009FA280
    CSurface GetSurface(long level);            // 0x009F9E00
};

struct CRegionMinimap_Methods : CRegionMinimap {
    CSurface& Surface() { return *(CSurface*)RegionMinimapSurface; }
    CTexture& Texture() { return *(CTexture*)RegionMinimapTexture; }
    void InitialiseTexture();
};

void CRegionMinimap_Methods::InitialiseTexture()
{
    CTexture& texture = Texture();
    if (texture.IsInitialised())
        texture.Uninitialise();
    C2DExtentsI size;
    size.X = RegionMinimapSize;
    size.Y = RegionMinimapSize;
    CPixelFormat format;
    CSystemManager* system = GFGetSystemManager();
    system->PDisplayManager->GetBestCompressedAlphaTextureFormat(0x10, &format);
    texture.Initialise(&size, 1, &format, 0, 1, false, false);
    C2DBoxI box(0, 0, size.X, size.Y);
    texture.GetSurface(0).CopyFromSurface(&box, &Surface(), &box, 1);
}
