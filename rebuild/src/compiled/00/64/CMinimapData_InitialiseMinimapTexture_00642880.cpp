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
// Port (NF98, round 8, q11274): the local CTexture / CSurface / C2DExtentsI / C2DBoxI / CPixelFormat stand-ins replaced by the shared
//   headers (Ego_r.pdb / FableWin.pdb spellings): ?GetSurfaceLevel@CTexture@@QBE?AVCSurface@@J@Z (0x009F9E00, FABLE_RETAIL_CTEXTURE_SIZED),
//   ?Initialise@CTexture@@QAE_NABVC2DExtentsI@@JABVCPixelFormat@@KW4ESurfacePool@@_N3@Z, ?GetPixelFormat@CSurface@@QBE?AVCPixelFormat@@XZ
//   (FABLE_RETAIL_CSURFACE_PIXELFORMAT), ?CopyFromSurface@CSurface@@QAEXPBVC2DBoxI@@ABV1@0K@Z; the texture test is FableWin.pdb's inline
//   ?IsValid@CTexture@@QBE_NXZ. CMinimapData has no shared header (Ego_r.pdb layout below).
// Leaf: ?InitialiseMinimapTexture@CMinimapData@@QAEXXZ
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE_SIZED
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_COPYFROMMEMORY
#define FABLE_RETAIL_CSURFACE_COPYFROMSURFACE
#define FABLE_RETAIL_CSURFACE_PIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_C2DBOXI_CTORS
#include "engine/Direct3D9Types.h"
#include "engine/C2DExtentsI.h"
#include "engine/C2DBoxI.h"
#include "engine/CPixelFormat.h"
#include "engine/CTexture.h"
#include "engine/CSurface.h"
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
    if (MinimapTexture.IsValid())
        MinimapTexture.Uninitialise();
    C2DExtentsI size = MinimapExtents;
    CPixelFormat format = MinimapSurface.GetPixelFormat();
    MinimapTexture.Initialise(size, 1, format, 0, SURFACE_POOL_MANAGED, false, false);
    C2DBoxI box(0, 0, size.X, size.Y);
    MinimapTexture.GetSurfaceLevel(0).CopyFromSurface(&box, MinimapSurface, &box, 0);
}
