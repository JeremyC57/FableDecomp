// CGraphicDataBank::LoadVolumeGraphic(CGraphicVolume& volume, const CDataBuffer& data, ...) -- lock level 0 of the volume texture and copy the bank data into it slice by slice, row by row (honouring the row and slice pitches), then unlock and release the level surfaces
// Grade: DIFFER(455v462) -- first draft: the counted texture pointer copy, the per-call GetDimensions temporaries and the end-of-load surface pair (second GetVolumeLevel plus two inline ~CVolumeSurface) are not yet in retail order
// Leaf: LoadVolumeGraphic
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FD9B0, 460 bytes, `ret 0x18`, 8-aligned frame.
// Callees: the volume texture's GetVolumeLevel 0x00A67EA0 (by value), CVolumeSurface GetByteLength 0x00A674E0 /
//   Lock 0x00A677D0 / GetPixelFormat 0x00A67480 / GetDimensions 0x00A674B0 (by value) / DoUnlock 0x00A67410 /
//   ReleaseVolume 0x00A67460, CPixelFormat::GetByteColourDepth 0x009E3800, CDataBuffer::GetData 0x009D5530, the empty
//   unlock hook 0x00A67440, CBaseObject dtor 0x0099A2E0, operator delete 0x00BFE9BC.
// Layout: local projection; the volume's info (+4) holds the texture counted pointer at +0x28.
#include <string.h>
struct CCPPointerInfo { long Count; void (__fastcall* Delete)(void*); void* Object; };
struct CPixelFormat { unsigned char Data[0xc]; long GetByteColourDepth() const; };   // 0x009E3800
struct CVolumeDimensions { long Width; long Height; long Depth; };
struct CLockedBox { long RowPitch; long SlicePitch; void* Bits; };
struct CBaseObject { virtual ~CBaseObject(); };       // 0x0099A2E0
struct CVolumeSurface : CBaseObject {                   // vtable 0x0129C840
    void* Volume;
    ~CVolumeSurface() { ReleaseVolume(true); }
    long GetByteLength() const;                          // 0x00A674E0
    void Lock(CLockedBox* box, long flags);              // 0x00A677D0
    CPixelFormat GetPixelFormat() const;                 // 0x00A67480
    CVolumeDimensions GetDimensions() const;             // 0x00A674B0
    void DoUnlock();                                     // 0x00A67410
    void ReleaseVolume(bool release);                    // 0x00A67460
    void OnUnlocked() { }
};
struct CVolumeTexture {
    CVolumeSurface GetVolumeLevel(long level);           // 0x00A67EA0
    CVolumeSurface GetVolumeLevel(long level, const CLockedBox* box);   // 0x00A67EA0
};
struct CTextureInfo { unsigned char Data[0x28]; CVolumeTexture* Texture; CCPPointerInfo* TextureInfo; };
struct CGraphicVolume { void* VTable; CTextureInfo* PInfo; };
struct CDataBuffer { const unsigned char* GetData() const; };   // 0x009D5530
struct CGraphicDataBank {
    bool LoadVolumeGraphic(CGraphicVolume& volume, const CDataBuffer& data, long a, long b, long c, long d);
};

bool CGraphicDataBank::LoadVolumeGraphic(CGraphicVolume& volume, const CDataBuffer& data, long, long, long, long)
{
    CVolumeTexture* texture = volume.PInfo->Texture;
    CCPPointerInfo* info = volume.PInfo->TextureInfo;
    if (info)
        ++info->Count;
    CVolumeSurface surface = texture->GetVolumeLevel(0);
    surface.GetByteLength();
    CLockedBox box;
    surface.Lock(&box, 0);
    unsigned char* slice = (unsigned char*)box.Bits;
    const unsigned char* src = data.GetData();
    long depth = surface.GetPixelFormat().GetByteColourDepth();
    for (long z = 0; z < surface.GetDimensions().Depth; ++z) {
        for (long y = 0; y < surface.GetDimensions().Height; ++y) {
            long rowBytes = surface.GetDimensions().Width * depth;
            memcpy(slice + box.RowPitch * y, src, rowBytes);
            src += surface.GetDimensions().Width * depth;
        }
        slice += box.SlicePitch;
    }
    texture->GetVolumeLevel(0, &box).OnUnlocked();
    surface.DoUnlock();
    if (info) {
        if (--info->Count == 0) {
            info->Delete(info->Object);
            delete info;
        }
    }
    return true;
}
