#ifndef FABLE_ENGINE_CVOLUMETEXTURE_H
#define FABLE_ENGINE_CVOLUMETEXTURE_H
// NEW (chunk 2008, row 0x00B54890 CEngineWeatherRenderer::RenderRain). Ego_r.pdb CVolumeTexture (class, 16 bytes): a vptr (??_ECVolumeTexture@@UAEPAXI@Z,
// virtual destructor), PD3DVolumeTexture +0x04, ByteLength +0x08, PData +0x0C. RenderRain reads PD3DVolumeTexture through the inline
// AttachVolumeTextureToStage (0x00B54926 `mov eax,[eax+4]`). Members only; the methods (Ego_r.pdb ?PreLoad@ / ?Unlock@ / ...) are not declared.
#include "rebuild_abi.h"
struct IDirect3DVolumeTexture9;
#ifdef FABLE_RETAIL_CVOLUMETEXTURE_INITIALISE
// Re-port opt-in (audit h3c2b, 2026-11-20, row 0x00A699D0 CManagedTextureData::CreateVolumeTexture): the real primary base (Fable.exe RTTI
// .?AVCVolumeTexture@@ : CBaseClass@0; the row's inline constructor calls CBaseClass() 0x0099A2D0 before the vtable 0x0129DC18 store),
// FableWin.pdb's inline default constructor ??0CVolumeTexture@@QAE@XZ (retail stores PD3DVolumeTexture = 0 and PData = 0, not
// ByteLength), Ego_r.pdb / FableWin.pdb ?Initialise@CVolumeTexture@@QAE_NABVC3DExtentsI@@JABVCPixelFormat@@KW4EVolumePool@@_N@Z
// (0x00A68370) and Ego_d.pdb's enum EVolumePool (field list 0x22D8). Same 16-byte layout.
#include "engine/CBaseClass.h"
enum EVolumePool { VOLUME_POOL_MANAGED = 1, VOLUME_POOL_SYSTEM_MEM = 2, VOLUME_POOL_DEFAULT_LOCATION = 0 };
class CVolumeTexture : public CBaseClass {
public:
    CVolumeTexture() : PD3DVolumeTexture(0), PData(0) { }
    bool Initialise(const class C3DExtentsI& extents, long no_levels, const class CPixelFormat& format, unsigned long usage, EVolumePool pool, bool flag);   // 0x00A68370
#else
class CVolumeTexture {
public:
#endif
    virtual ~CVolumeTexture();
    IDirect3DVolumeTexture9* PD3DVolumeTexture;                          // +0x04
    long                     ByteLength;                                 // +0x08
    void*                    PData;                                      // +0x0c
#ifdef FABLE_RETAIL_CVOLUMETEXTURE_GETASTEXTURELIST
    // Re-port opt-in (2026-11-19, row 0x00A68910): Ego_r.pdb / FableWin.pdb ?GetAsTextureList@CVolumeTexture@@QAEXPAVCTexture@@@Z (0x00A68910),
    // ?Lock@CVolumeTexture@@QAE?AVCVolumeLockedBox@@KK@Z (0x00A685E0), ?GetVolumeLevel@CVolumeTexture@@QBE?AVCVolume@@J@Z (0x00A67EA0), and the
    // members retail expands in that row: ?Unlock@CVolumeTexture@@QAEXK@Z, ?GetDimensions@CVolumeTexture@@QBE?AVC3DExtentsI@@K@Z,
    // ?GetPixelFormat@CVolumeTexture@@QBE?AVCPixelFormat@@XZ (Ego_r + FableWin) and ?Swizzle@ / ?Unswizzle@CVolumeTexture@@QAEXJAAVCVolumeLockedBox@@@Z
    // (FableWin.pdb / Ego_d.pdb, debug only). Declarations only: the row's TU defines the inline bodies over its own D3D interface model
    // (this header only forward-declares IDirect3DVolumeTexture9). No layout change.
    class CVolumeLockedBox Lock(unsigned long level, unsigned long flags);   // 0x00A685E0
    void Unlock(unsigned long level);
    class CVolume GetVolumeLevel(long level) const;                      // 0x00A67EA0
    void Swizzle(long level, class CVolumeLockedBox& box);
    void Unswizzle(long level, class CVolumeLockedBox& box);
    class C3DExtentsI GetDimensions(unsigned long level) const;
    class CPixelFormat GetPixelFormat() const;
    void GetAsTextureList(class CTexture* textures);                     // 0x00A68910
#endif
#ifdef FABLE_RETAIL_VOLUME_SERIALIZATION_PASS30
    long GetNoLevels() const;
    long GetLevelByteLength(long level) const;
    void SaveToDataStream(class CDataOutputStream& stream);
#endif
};
FABLE_STATIC_ASSERT_AT(15, sizeof(CVolumeTexture) == 0x10);
#ifdef FABLE_RETAIL_VOLUME_SERIALIZATION_PASS30
#include "engine/Direct3DVolumeTexture9.h"
#include "engine/C3DExtentsI.h"
#include "engine/CPixelFormat.h"
inline long CVolumeTexture::GetNoLevels() const
{
    return PD3DVolumeTexture->GetLevelCount();
}
inline C3DExtentsI CVolumeTexture::GetDimensions(unsigned long level) const
{
    _D3DVOLUME_DESC desc;
    PD3DVolumeTexture->GetLevelDesc(level, &desc);
    return C3DExtentsI(desc.Width, desc.Height, desc.Depth);
}
inline CPixelFormat CVolumeTexture::GetPixelFormat() const
{
    _D3DVOLUME_DESC desc;
    PD3DVolumeTexture->GetLevelDesc(0, &desc);
    return CPixelFormat(desc.Format);
}
inline void CVolumeTexture::Unlock(unsigned long level)
{
    PD3DVolumeTexture->UnlockBox(level);
}
#endif
#ifdef FABLE_RETAIL_VOLUME_SAVE_LOCK_BODY_PASS30
#include "engine/CVolumeLockedBox.h"
#include "engine/CSystemManager.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"
inline CVolumeLockedBox CVolumeTexture::Lock(unsigned long level, unsigned long flags)
{
    C3DExtentsI dimensions = GetDimensions(level);
    _D3DBOX box;
    box.Front = 0;
    box.Top = 0;
    box.Left = 0;
    box.Bottom = dimensions.Y;
    box.Right = dimensions.X;
    box.Back = dimensions.Z;
    CSystemManager* system = GFGetSystemManager();
    system->GetDisplayManager()->GetRenderManager().WaitWhileResourceIsBusy(PD3DVolumeTexture, 1.0f);
    _D3DLOCKED_BOX locked;
    if (PD3DVolumeTexture->LockBox(level, &locked, &box, flags) >= 0) {
        CVolumeLockedBox result;
        result.Dimensions = dimensions;
        result.RowPitch = locked.RowPitch;
        result.SlicePitch = locked.SlicePitch;
        result.PMem = (unsigned char*)locked.pBits;
        return result;
    }
    return CVolumeLockedBox();
}
#endif

#endif
