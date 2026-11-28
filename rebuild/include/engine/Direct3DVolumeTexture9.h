#ifndef FABLE_ENGINE_VOLUME_D3D9_H
#define FABLE_ENGINE_VOLUME_D3D9_H
#ifdef FABLE_RETAIL_VOLUME_SERIALIZATION_PASS30
#include "rebuild_abi.h"
#include "engine/Direct3D9Types.h"
// Direct3D 9 interface order, corroborated by the donor call signatures and native slot dispatches.
#include "engine/Direct3DTexture9.h"
struct IDirect3DVolume9;
typedef _D3DRESOURCETYPE D3DRESOURCETYPE;
typedef _D3DTEXTUREFILTERTYPE D3DTEXTUREFILTERTYPE;
typedef _D3DPOOL D3DPOOL;
typedef struct _D3DBOX {
    UINT                Left;
    UINT                Top;
    UINT                Right;
    UINT                Bottom;
    UINT                Front;
    UINT                Back;
} D3DBOX;
typedef struct _D3DLOCKED_BOX {
    INT                 RowPitch;
    INT                 SlicePitch;
    void*               pBits;
} D3DLOCKED_BOX;
typedef struct _D3DVOLUME_DESC {
    D3DFORMAT           Format;
    D3DRESOURCETYPE     Type;
    DWORD               Usage;
    D3DPOOL             Pool;

    UINT                Width;
    UINT                Height;
    UINT                Depth;
} D3DVOLUME_DESC;
struct IDirect3DVolumeTexture9 : IDirect3DBaseTexture9 {
    virtual long FABLE_STDCALL GetLevelDesc(UINT Level, D3DVOLUME_DESC *pDesc) = 0;
    virtual long FABLE_STDCALL GetVolumeLevel(UINT Level, IDirect3DVolume9** ppVolumeLevel) = 0;
    virtual long FABLE_STDCALL LockBox(UINT level, D3DLOCKED_BOX *locked_box, const D3DBOX *box, DWORD flags) = 0;
    virtual long FABLE_STDCALL UnlockBox(UINT Level) = 0;
    virtual long FABLE_STDCALL AddDirtyBox(const D3DBOX *dirty_box) = 0;
};
#endif

#endif
