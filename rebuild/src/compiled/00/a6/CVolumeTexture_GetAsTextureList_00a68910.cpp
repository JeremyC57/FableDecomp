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
//   the inline ~CVolume (vtable 0x0129C840, CVolume::ReleaseVolume 0x00A67460, ~CBaseClass 0x0099A2E0),
//   CTexture::Initialise 0x009FA280, CTexture::Lock 0x009FA450, CTexture::Unswizzle 0x009FA180 / Swizzle 0x009FA140,
//   CTexture::Unlock 0x009F9DE0. Swizzle/Unswizzle are told apart by Ego_r.pdb donor order (CSurface::Swizzle 0x00B46B20
//   before Unswizzle 0x00B46B30; retail CSurface 0x009F2E40 before 0x009F2E50, which the two CTexture callees reach).
// Shape: the inline GetDimensions (in the PDBs) and the scoped pixel-format block each read the level desc (the format
//   block's desc shares its frame slot with the later locked rect); the row copy is the inline memcpy intrinsic (`rep movsd ; rep movsb`).
#include <string.h>
#pragma intrinsic(memcpy)

enum _D3DFORMAT { D3DFMT_UNKNOWN = 0, D3DFMT_FORCE_DWORD = 0x7fffffff };
enum ESurfacePool { ESurfacePool_0 = 0, ESurfacePool_1 = 1 };

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
struct IDirect3DTexture9;

class C2DExtentsI { public:
    long X;
    long Y;
    C2DExtentsI(long x, long y) : X(x), Y(y) { }
};

class C3DExtentsI { public:
    long X;
    long Y;
    long Z;
    C3DExtentsI(long x, long y, long z) : X(x), Y(y), Z(z) { }
};

class CPixelFormat { public:
    long FormatIndex;
    void Initialise(_D3DFORMAT format);                     // 0x009E3830
    unsigned long GetByteColourDepth() const;               // 0x009E3800
};

class CVolumeLockedBox { public:                            // 24 bytes (Ego_r.pdb)
    C3DExtentsI    Dimensions;                              // +0x00
    long           RowPitch;                                // +0x0c
    long           SlicePitch;                              // +0x10
    unsigned char* PMem;                                    // +0x14
};

class CSurfaceLockedRect { public:                          // 16 bytes (Ego_r.pdb)
    C2DExtentsI    Dimensions;                              // +0x00
    long           Pitch;                                   // +0x08
    unsigned char* PMem;                                    // +0x0c
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CVolume : public CBaseClass { public:                 // vtable 0x0129C840
    IDirect3DVolume9* PD3DVolume;                           // +0x04
    CVolume(const CVolume& other);
    virtual ~CVolume() { ReleaseVolume(true); }
    void ReleaseVolume(bool release);                       // 0x00A67460
    void Swizzle(CVolumeLockedBox& box);                    // 0x00A67440
    void Unswizzle(CVolumeLockedBox& box);                  // 0x00A67450
};

class CTexture { public:                                    // 8 bytes (Ego_r.pdb)
    IDirect3DTexture9* PD3DTexture;                         // +0x00
    unsigned long      ByteLength : 28;                     // +0x04
    unsigned long      AllocationSource : 4;
    bool Initialise(const C2DExtentsI& extents, long levels, const CPixelFormat& format, unsigned long usage,
                    ESurfacePool pool, bool flagA, bool flagB);                                  // 0x009FA280
    CSurfaceLockedRect Lock(unsigned long level, unsigned long flags);                           // 0x009FA450
    void Unlock(unsigned long level);                                                            // 0x009F9DE0
    void Swizzle(long level, CSurfaceLockedRect& rect);                                          // 0x009FA140
    void Unswizzle(long level, CSurfaceLockedRect& rect);                                        // 0x009FA180
};

class CVolumeTexture { public:                              // 16 bytes (Ego_r.pdb)
    void*                    _vtable;                       // +0x00
    IDirect3DVolumeTexture9* PD3DVolumeTexture;             // +0x04
    long                     ByteLength;                    // +0x08
    void*                    PData;                         // +0x0c
    CVolumeLockedBox Lock(unsigned long level, unsigned long flags);                             // 0x00A685E0
    void Unlock(unsigned long level) { PD3DVolumeTexture->UnlockBox(level); }
    CVolume GetVolumeLevel(long level) const;                                                    // 0x00A67EA0
    void Swizzle(long level, CVolumeLockedBox& box) { GetVolumeLevel(level).Swizzle(box); }
    void Unswizzle(long level, CVolumeLockedBox& box) { GetVolumeLevel(level).Unswizzle(box); }
    C3DExtentsI GetDimensions() const
    {
        D3DVOLUME_DESC desc;
        PD3DVolumeTexture->GetLevelDesc(0, &desc);
        return C3DExtentsI(desc.Width, desc.Height, desc.Depth);
    }
    void GetAsTextureList(CTexture* textures);
};

void CVolumeTexture::GetAsTextureList(CTexture* textures)
{
    C3DExtentsI dimensions = GetDimensions();
    C2DExtentsI extents(dimensions.X, dimensions.Y);
    CPixelFormat format;
    {
        D3DVOLUME_DESC formatDesc;
        PD3DVolumeTexture->GetLevelDesc(0, &formatDesc);
        format.Initialise(formatDesc.Format);
    }
    long rowBytes = format.GetByteColourDepth() * dimensions.X;
    CVolumeLockedBox box = Lock(0, 0);
    Unswizzle(0, box);
    unsigned char* src = box.PMem;
    for (long z = 0; z < dimensions.Z; ++z) {
        textures[z].Initialise(extents, 1, format, 0, ESurfacePool_1, false, false);
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