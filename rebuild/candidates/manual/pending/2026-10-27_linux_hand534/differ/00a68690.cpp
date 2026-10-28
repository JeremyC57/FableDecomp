// CVolumeTexture::InitialiseSlicesFromTextureList(CTexture* textures, long no_textures) -- build a volume texture from a list of 2D slices: size it from the first texture (depth = list length, uncompressed format), then copy each slice's top level into the locked box row by row through one scratch texture
// Grade: DIFFER(626v626, probe) -- frame `sub esp,60h` vs retail 58h: retail overlaps the C3DExtentsI with the scratch CTexture (both at the same slot), ours keeps them apart, so every frame offset after the GetDimensions temporaries is 8 higher; instruction stream otherwise identical
// Leaf: ?InitialiseSlicesFromTextureList@CVolumeTexture@@QAE_NPAVCTexture@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A68690, 626 bytes, thiscall `ret 8`.
// Identity: Ego_r.pdb / FableWin.pdb `?InitialiseSlicesFromTextureList@CVolumeTexture@@QAE_NPAVCTexture@@J@Z` (the manifest
//   row carries the same signature); the landed GetAsTextureList 0x00A68910 is its inverse and sits right after it.
// Callees: CVolumeTexture::Uninitialise 0x00A68060 / Initialise 0x00A68370 / Lock 0x00A685E0 / GetVolumeLevel 0x00A67EA0,
//   CVolume::Unswizzle 0x00A67450 / Swizzle 0x00A67440 / Uninitialise(bool) 0x00A67460 (inline ~CVolume, vtable 0x0129C840,
//   then ~CBaseClass 0x0099A2E0), CTexture::GetDimensions 0x009F9DB0 / GetPixelFormat 0x009F9D80 / GetNoLevels 0x009F9DF0 /
//   Initialise 0x009FA280 / CopyFromTexture 0x009FA4E0 / Lock 0x009FA450 / Unswizzle 0x009FA180 / Swizzle 0x009FA140 /
//   Unlock 0x009F9DE0 / Uninitialise 0x009F9F70 (inline ~CTexture), CPixelFormat::IsCompressed 0x009E3940 /
//   GetByteColourDepth 0x009E3800; IDirect3DVolumeTexture9::UnlockBox (slot 20, the inline CVolumeTexture::Unlock).
// Global: DefaultUncompressedPFormat 0x013D28E4 (both PDBs: `dynamic initializer for 'DefaultUncompressedPFormat'`).
// Shape: the scratch texture is FableWin's inline `CTexture(const C2DExtentsI&, long, const CPixelFormat&, unsigned long,
//   ESurfacePool)` (clears AllocationSource and PD3DTexture, then Initialise); its C2DExtentsI is built with the volume
//   extents (both before the volume Initialise); the format is `IsCompressed() ? DefaultUncompressedPFormat : format`;
//   the row copy is the memcpy intrinsic (as in GetAsTextureList).
#include <string.h>
#pragma intrinsic(memcpy)

struct IDirect3DVolumeTexture9 {
    virtual void __stdcall Slot00() = 0; virtual void __stdcall Slot01() = 0; virtual void __stdcall Slot02() = 0;
    virtual void __stdcall Slot03() = 0; virtual void __stdcall Slot04() = 0; virtual void __stdcall Slot05() = 0;
    virtual void __stdcall Slot06() = 0; virtual void __stdcall Slot07() = 0; virtual void __stdcall Slot08() = 0;
    virtual void __stdcall Slot09() = 0; virtual void __stdcall Slot10() = 0; virtual void __stdcall Slot11() = 0;
    virtual void __stdcall Slot12() = 0; virtual void __stdcall Slot13() = 0; virtual void __stdcall Slot14() = 0;
    virtual void __stdcall Slot15() = 0; virtual void __stdcall Slot16() = 0; virtual void __stdcall Slot17() = 0;
    virtual void __stdcall Slot18() = 0; virtual void __stdcall Slot19() = 0;
    virtual long __stdcall UnlockBox(unsigned int level) = 0;                                   // slot 20 (+0x50)
};
struct IDirect3DVolume9;
struct IDirect3DTexture9;

class C2DExtentsI {
public:
    long X;                                 // +0x00
    long Y;                                 // +0x04
    C2DExtentsI(long x, long y) : X(x), Y(y) { }
    C2DExtentsI(const C2DExtentsI& e) : X(e.X), Y(e.Y) { }
};
class C3DExtentsI : public C2DExtentsI {
public:
    long Z;                                 // +0x08
    C3DExtentsI(long x, long y, long z) : C2DExtentsI(x, y), Z(z) { }
};

class CPixelFormat {
public:
    long FormatIndex;                       // +0x00
    CPixelFormat();
    bool IsCompressed() const;                              // 0x009E3940
    unsigned long GetByteColourDepth() const;               // 0x009E3800
};
extern CPixelFormat DefaultUncompressedPFormat;             // 0x013D28E4

enum EVolumePool { };
enum ESurfacePool { };

class CVolumeLockedBox {                    // 24 bytes (Ego_r)
public:
    C3DExtentsI    Dimensions;              // +0x00
    long           RowPitch;                // +0x0c
    long           SlicePitch;              // +0x10
    unsigned char* PMem;                    // +0x14
};
class CSurfaceLockedRect {                  // 16 bytes (Ego_r)
public:
    C2DExtentsI    Dimensions;              // +0x00
    long           Pitch;                   // +0x08
    unsigned char* PMem;                    // +0x0c
};

class CBaseClass {
public:
    virtual ~CBaseClass();                                  // 0x0099A2E0
};
class CVolume : public CBaseClass {         // vtable 0x0129C840
public:
    IDirect3DVolume9* PD3DVolume;           // +0x04
    CVolume(const CVolume& other);
    virtual ~CVolume() { Uninitialise(true); }
    void Uninitialise(bool release);                        // 0x00A67460
    void Swizzle(CVolumeLockedBox& box);                    // 0x00A67440
    void Unswizzle(CVolumeLockedBox& box);                  // 0x00A67450
};

class CTexture {                            // 8 bytes (Ego_r)
public:
    IDirect3DTexture9* PD3DTexture;         // +0x00
    unsigned long      ByteLength : 28;     // +0x04
    unsigned long      AllocationSource : 4;
    CTexture(const C2DExtentsI& extents, long no_levels, const CPixelFormat& format, unsigned long usage, ESurfacePool pool)
    {
        PD3DTexture = 0;
        AllocationSource = 0;
        Initialise(extents, no_levels, format, usage, pool, false, false);
    }
    ~CTexture() { Uninitialise(); }
    bool Initialise(const C2DExtentsI& extents, long no_levels, const CPixelFormat& format, unsigned long usage,
                    ESurfacePool pool, bool flag_a, bool flag_b);                               // 0x009FA280
    void Uninitialise();                                                                         // 0x009F9F70
    C2DExtentsI GetDimensions(unsigned long level) const;                                        // 0x009F9DB0
    CPixelFormat GetPixelFormat() const;                                                         // 0x009F9D80
    long GetNoLevels() const;                                                                    // 0x009F9DF0
    void CopyFromTexture(const CTexture& source, unsigned long levels);                          // 0x009FA4E0
    CSurfaceLockedRect Lock(unsigned long level, unsigned long flags);                           // 0x009FA450
    void Unlock(unsigned long level);                                                            // 0x009F9DE0
    void Swizzle(long level, CSurfaceLockedRect& rect);                                          // 0x009FA140
    void Unswizzle(long level, CSurfaceLockedRect& rect);                                        // 0x009FA180
};

class CVolumeTexture {
public:
    virtual ~CVolumeTexture();
    IDirect3DVolumeTexture9* PD3DVolumeTexture;    // +0x04
    long                     ByteLength;           // +0x08
    void*                    PData;                // +0x0c
    void Uninitialise();                                                                         // 0x00A68060
    bool Initialise(const C3DExtentsI& extents, long no_levels, const CPixelFormat& format, unsigned long usage,
                    EVolumePool pool, bool flag);                                                // 0x00A68370
    CVolumeLockedBox Lock(unsigned long level, unsigned long flags);                             // 0x00A685E0
    void Unlock(unsigned long level) { PD3DVolumeTexture->UnlockBox(level); }
    CVolume GetVolumeLevel(long level) const;                                                    // 0x00A67EA0
    __forceinline void Swizzle(long level, CVolumeLockedBox& box) { GetVolumeLevel(level).Swizzle(box); }
    __forceinline void Unswizzle(long level, CVolumeLockedBox& box) { GetVolumeLevel(level).Unswizzle(box); }
    bool InitialiseSlicesFromTextureList(CTexture* textures, long no_textures);
};

bool CVolumeTexture::InitialiseSlicesFromTextureList(CTexture* textures, long no_textures)
{
    Uninitialise();
    if (textures == 0 || no_textures == 0)
        return false;
    long height = textures->GetDimensions(0).Y;
    long width = textures->GetDimensions(0).X;
    C3DExtentsI extents(width, height, no_textures);
    C2DExtentsI slice_extents(width, height);
    CPixelFormat texture_format = textures->GetPixelFormat();
    CPixelFormat format = texture_format.IsCompressed() ? DefaultUncompressedPFormat : texture_format;
    if (Initialise(extents, 1, format, 0, (EVolumePool)1, false)) {
        long row_bytes = format.GetByteColourDepth() * width;
        CTexture slice(slice_extents, textures->GetNoLevels(), format, 0, (ESurfacePool)1);
        CVolumeLockedBox box = Lock(0, 0);
        Unswizzle(0, box);
        unsigned char* dst = box.PMem;
        for (long z = 0; z < no_textures; ++z) {
            slice.CopyFromTexture(textures[z], (unsigned long)-1);
            CSurfaceLockedRect rect = slice.Lock(0, 0);
            slice.Unswizzle(0, rect);
            unsigned char* src = rect.PMem;
            unsigned char* slice_start = dst;
            for (long y = 0; y < height; ++y) {
                memcpy(dst, src, row_bytes);
                dst += box.RowPitch;
                src += rect.Pitch;
            }
            dst = slice_start + box.SlicePitch;
            slice.Swizzle(0, rect);
            slice.Unlock(0);
        }
        Swizzle(0, box);
        Unlock(0);
        return true;
    }
    return false;
}

