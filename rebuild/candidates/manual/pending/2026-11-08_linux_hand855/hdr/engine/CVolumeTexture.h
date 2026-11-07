#ifndef FABLE_ENGINE_CVOLUMETEXTURE_H
#define FABLE_ENGINE_CVOLUMETEXTURE_H
// CVolumeTexture (chunk 855, row 0x00A68DE0 LoadFromDataStream). Ego_r.pdb: 16 bytes, polymorphic (vtable 0x0129DC18, the
// donor's unnamed dword +0 is the vptr; CBaseClass base as the landed ctor rows 0x00A67DD0 / 0x00A695C0 model it):
// PD3DVolumeTexture +4, ByteLength +8, PData +0xC. Methods are the Ego_r / FableWin spellings with their retail addresses;
// the accessors GetNoLevels / GetDimensions / GetPixelFormat / Unlock are inline in retail (their D3D calls are expanded at
// every site: GetLevelCount slot 13, GetLevelDesc slot 17, UnlockBox slot 20).
#include "rebuild_abi.h"
#include "engine/CBaseClass.h"
#include "engine/IDirect3DVolumeTexture9.h"
#include "engine/C3DExtentsI.h"
#include "engine/CPixelFormat.h"
#include "engine/CVolumeLockedBox.h"
#include "engine/EVolumePool.h"
class CDataInputStream;
class CDataOutputStream;
class CVolumeTexture : public CBaseClass {
public:
    virtual ~CVolumeTexture();
    bool Initialise(const C3DExtentsI& dimensions, long noLevels, const CPixelFormat& pixelFormat, unsigned long useageFlags,
                    EVolumePool pool, bool preallocated);                                      // 0x00A68370
    long GetNoLevels() const { return PD3DVolumeTexture->GetLevelCount(); }
    C3DExtentsI GetDimensions(unsigned long level) const
    {
        _D3DVOLUME_DESC desc;
        PD3DVolumeTexture->GetLevelDesc(level, &desc);
        return C3DExtentsI(desc.Width, desc.Height, desc.Depth);
    }
    CPixelFormat GetPixelFormat() const
    {
        _D3DVOLUME_DESC desc;
        PD3DVolumeTexture->GetLevelDesc(0, &desc);
        return CPixelFormat(desc.Format);
    }
    long GetLevelByteLength(long level) const;                                                 // 0x00A681F0
    CVolumeLockedBox Lock(unsigned long level, unsigned long flags);                           // 0x00A685E0
    void Unlock(unsigned long level) { PD3DVolumeTexture->UnlockBox(level); }
    void SaveToDataStream(CDataOutputStream& stream);                                          // 0x00A68AE0
    void LoadFromDataStream(CDataInputStream& stream);                                         // 0x00A68DE0
    IDirect3DVolumeTexture9* PD3DVolumeTexture;                                                // +0x04
    long                     ByteLength;                                                       // +0x08
    void*                    PData;                                                            // +0x0c
};
FABLE_STATIC_ASSERT(sizeof(CVolumeTexture) == 0x10);
#endif
