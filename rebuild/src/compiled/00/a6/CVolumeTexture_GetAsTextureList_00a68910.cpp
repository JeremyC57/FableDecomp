// CVolumeTexture::GetAsTextureList(CTexture* textures) -- split the volume texture's top level into one 2D texture per depth slice: create each texture with the volume's width, height and pixel format, and copy the slice into it row by row
// Leaf: ?GetAsTextureList@CVolumeTexture@@QAEXPAVCTexture@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A68910, 452 bytes, thiscall `ret 4`.
// Identity: both PDBs name `?GetAsTextureList@CVolumeTexture@@QAEXPAVCTexture@@@Z` (void, one CTexture*), the one
//   CVolumeTexture method with that shape; `this` is Ego_r.pdb CVolumeTexture (PD3DVolumeTexture +4) and the textures
//   are 8-byte Ego_r.pdb CTexture objects (`add ebp,8`). No direct callers. Catalog label
//   "CVolumeTexture_ExtractSlicesToTextures" is descriptive.
// Callees: IDirect3DVolumeTexture9::GetLevelDesc (slot 17, twice: the inline dimension and pixel-format getters) and
//   UnlockBox (slot 20: the inline Unlock), CPixelFormat::Initialise(_D3DFORMAT) 0x009E3830,
//   CPixelFormat::GetByteColourDepth 0x009E3800, CVolumeTexture::Lock 0x00A685E0, CVolumeTexture::GetVolumeLevel 0x00A67EA0,
//   CVolume::Unswizzle 0x00A67450 / Swizzle 0x00A67440 (empty on PC; inside the inline CVolumeTexture::Unswizzle/Swizzle),
//   the inline ~CVolume (vtable 0x0129C840, CVolume::Uninitialise(bool) 0x00A67460, ~CBaseClass 0x0099A2E0),
//   CTexture::Initialise 0x009FA280, CTexture::Lock 0x009FA450, CTexture::Unswizzle 0x009FA180 / Swizzle 0x009FA140,
//   CTexture::Unlock 0x009F9DE0. Swizzle/Unswizzle are told apart by Ego_r.pdb donor order (CSurface::Swizzle 0x00B46B20
//   before Unswizzle 0x00B46B30; retail CSurface 0x009F2E40 before 0x009F2E50, which the two CTexture callees reach).
// Shape: the inline GetDimensions(0) and GetPixelFormat() (both in the PDBs) each read the level desc (GetPixelFormat's
//   desc shares its frame slot with the later locked rect); the row copy is the inline memcpy intrinsic (`rep movsd ; rep movsb`).
// Re-port (2026-11-19): the row's local C2DExtentsI / C3DExtentsI / CPixelFormat / CSurfaceLockedRect / CTexture / CVolumeTexture models are
//   replaced by the shared headers: engine/C2DExtentsI.h, engine/C3DExtentsI.h (Ego_d.pdb: a C2DExtentsI base plus long Z), engine/CPixelFormat.h
//   (FABLE_RETAIL_CPIXELFORMAT, _BYTECOLOURDEPTH_OUTOFLINE), engine/CSurfaceLockedRect.h, engine/CTexture.h (FABLE_RETAIL_CTEXTURE, _LOCK and the
//   new _SWIZZLE_RECT) and engine/CVolumeTexture.h (new FABLE_RETAIL_CVOLUMETEXTURE_GETASTEXTURELIST: declarations; the inline bodies are
//   below, over this TU's IDirect3DVolumeTexture9 model). The pool is the header's SURFACE_POOL_MANAGED (1); the format comes from the PDB's
//   inline CVolumeTexture::GetPixelFormat (GetLevelDesc(0), then the inline CPixelFormat(_D3DFORMAT) constructor: Initialise, no -1 store).
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT_BYTECOLOURDEPTH_OUTOFLINE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE_LOCK
#define FABLE_RETAIL_CTEXTURE_SWIZZLE_RECT
#define FABLE_RETAIL_CVOLUMETEXTURE_GETASTEXTURELIST
#include <string.h>
#pragma intrinsic(memcpy)
#include "engine/Direct3D9Types.h"
#include "engine/C2DExtentsI.h"
#include "engine/C3DExtentsI.h"
#include "engine/CPixelFormat.h"
#include "engine/CSurfaceLockedRect.h"
#include "engine/ESurfacePool.h"
#include "engine/CTexture.h"
#include "engine/CVolumeTexture.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

struct D3DVOLUME_DESC {
    _D3DFORMAT    Format;                                   // +0x00
    long          Type;                                     // +0x04
    unsigned long Usage;                                    // +0x08
    long          Pool;                                     // +0x0c
    unsigned long Width;                                    // +0x10
    unsigned long Height;                                   // +0x14
    unsigned long Depth;                                    // +0x18
};

struct IDirect3DVolumeTexture9 {
    virtual void __stdcall Slot00() = 0; virtual void __stdcall Slot01() = 0; virtual void __stdcall Slot02() = 0;
    virtual void __stdcall Slot03() = 0; virtual void __stdcall Slot04() = 0; virtual void __stdcall Slot05() = 0;
    virtual void __stdcall Slot06() = 0; virtual void __stdcall Slot07() = 0; virtual void __stdcall Slot08() = 0;
    virtual void __stdcall Slot09() = 0; virtual void __stdcall Slot10() = 0; virtual void __stdcall Slot11() = 0;
    virtual void __stdcall Slot12() = 0; virtual void __stdcall Slot13() = 0; virtual void __stdcall Slot14() = 0;
    virtual void __stdcall Slot15() = 0; virtual void __stdcall Slot16() = 0;
    virtual long __stdcall GetLevelDesc(unsigned long level, D3DVOLUME_DESC* desc) = 0;          // slot 17
    virtual void __stdcall Slot18() = 0; virtual void __stdcall Slot19() = 0;
    virtual long __stdcall UnlockBox(unsigned long level) = 0;                                   // slot 20
};
struct IDirect3DVolume9;

class CVolumeLockedBox { public:                            // 24 bytes (Ego_r.pdb)
    C3DExtentsI    Dimensions;                              // +0x00
    long           RowPitch;                                // +0x0c
    long           SlicePitch;                              // +0x10
    unsigned char* PMem;                                    // +0x14
};

class CVolume : public CBaseClass { public:                 // vtable 0x0129C840
    IDirect3DVolume9* PD3DVolume;                           // +0x04
    CVolume(const CVolume& other);
    virtual ~CVolume() { Uninitialise(true); }
    void Uninitialise(bool release);                        // 0x00A67460 ?Uninitialise@CVolume@@QAEX_N@Z (Ego_r.pdb; was spelled ReleaseVolume(bool) here)
    void Swizzle(CVolumeLockedBox& box);                    // 0x00A67440
    void Unswizzle(CVolumeLockedBox& box);                  // 0x00A67450
};

// The CVolumeTexture members retail expands in this row (declared by the header opt-in).
inline void CVolumeTexture::Unlock(unsigned long level) { PD3DVolumeTexture->UnlockBox(level); }
inline void CVolumeTexture::Swizzle(long level, CVolumeLockedBox& box) { GetVolumeLevel(level).Swizzle(box); }
inline void CVolumeTexture::Unswizzle(long level, CVolumeLockedBox& box) { GetVolumeLevel(level).Unswizzle(box); }
inline C3DExtentsI CVolumeTexture::GetDimensions(unsigned long level) const
{
    D3DVOLUME_DESC desc;
    PD3DVolumeTexture->GetLevelDesc(level, &desc);
    return C3DExtentsI(desc.Width, desc.Height, desc.Depth);
}
inline CPixelFormat CVolumeTexture::GetPixelFormat() const
{
    D3DVOLUME_DESC desc;
    PD3DVolumeTexture->GetLevelDesc(0, &desc);
    return CPixelFormat(desc.Format);
}

void CVolumeTexture::GetAsTextureList(CTexture* textures)
{
    C3DExtentsI dimensions = GetDimensions(0);
    C2DExtentsI extents(dimensions.X, dimensions.Y);
    CPixelFormat format = GetPixelFormat();
    long rowBytes = format.GetByteColourDepth() * dimensions.X;
    CVolumeLockedBox box = Lock(0, 0);
    Unswizzle(0, box);
    unsigned char* src = box.PMem;
    for (long z = 0; z < dimensions.Z; ++z) {
        textures[z].Initialise(extents, 1, format, 0, SURFACE_POOL_MANAGED, false, false);
        CSurfaceLockedRect rect = textures[z].Lock(0, 0);
        textures[z].Unswizzle(0, rect);
        unsigned char* dst = rect.PMem;
        unsigned char* sliceStart = src;
        for (long y = 0; y < dimensions.Y; ++y) {
            memcpy(dst, src, rowBytes);
            dst += rect.Pitch;
            src += box.RowPitch;
        }
        src = sliceStart + box.SlicePitch;
        textures[z].Swizzle(0, rect);
        textures[z].Unlock(0);
    }
    Swizzle(0, box);
    Unlock(0);
}