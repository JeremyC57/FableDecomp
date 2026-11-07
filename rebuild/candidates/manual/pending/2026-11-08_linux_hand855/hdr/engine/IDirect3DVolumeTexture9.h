#ifndef FABLE_ENGINE_IDIRECT3DVOLUMETEXTURE9_H
#define FABLE_ENGINE_IDIRECT3DVOLUMETEXTURE9_H
// IDirect3DVolumeTexture9 (chunk 855, row 0x00A68DE0): the d3d9.h COM interface (IUnknown 0-2, IDirect3DResource9 3-10,
// IDirect3DBaseTexture9 11-16, then GetLevelDesc 17, GetVolumeLevel 18, LockBox 19, UnlockBox 20, AddDirtyBox 21), slots as
// retail calls them (GetLevelCount `call [ecx+34h]`, GetLevelDesc `[ecx+44h]`, UnlockBox `[ecx+50h]`). D3DVOLUME_DESC is the
// d3d9types.h struct (7 dwords).
#include "engine/Direct3D9Types.h"
struct IDirect3DVolume9;
struct _D3DVOLUME_DESC {
    _D3DFORMAT    Format;                                                // +0x00
    unsigned long Type;                                                  // +0x04 (D3DRESOURCETYPE)
    unsigned long Usage;                                                 // +0x08
    unsigned long Pool;                                                  // +0x0c (D3DPOOL)
    unsigned int  Width;                                                 // +0x10
    unsigned int  Height;                                                // +0x14
    unsigned int  Depth;                                                 // +0x18
};
struct IDirect3DVolumeTexture9 {
    virtual long __stdcall QueryInterface(const void* riid, void** object) = 0;              // 0
    virtual unsigned long __stdcall AddRef() = 0;                                              // 1
    virtual unsigned long __stdcall Release() = 0;                                             // 2
    virtual long __stdcall GetDevice(void** device) = 0;                                       // 3
    virtual long __stdcall SetPrivateData(const void*, const void*, unsigned long, unsigned long) = 0; // 4
    virtual long __stdcall GetPrivateData(const void*, void*, unsigned long*) = 0;             // 5
    virtual long __stdcall FreePrivateData(const void*) = 0;                                   // 6
    virtual unsigned long __stdcall SetPriority(unsigned long priority) = 0;                   // 7
    virtual unsigned long __stdcall GetPriority() = 0;                                         // 8
    virtual void __stdcall PreLoad() = 0;                                                      // 9
    virtual long __stdcall GetType() = 0;                                                      // 10
    virtual unsigned long __stdcall SetLOD(unsigned long lod) = 0;                             // 11
    virtual unsigned long __stdcall GetLOD() = 0;                                              // 12
    virtual unsigned long __stdcall GetLevelCount() = 0;                                       // 13
    virtual long __stdcall SetAutoGenFilterType(long filterType) = 0;                          // 14
    virtual long __stdcall GetAutoGenFilterType() = 0;                                         // 15
    virtual void __stdcall GenerateMipSubLevels() = 0;                                         // 16
    virtual long __stdcall GetLevelDesc(unsigned int level, _D3DVOLUME_DESC* desc) = 0;        // 17
    virtual long __stdcall GetVolumeLevel(unsigned int level, IDirect3DVolume9** volume) = 0;  // 18
    virtual long __stdcall LockBox(unsigned int level, void* lockedVolume, const void* box, unsigned long flags) = 0; // 19
    virtual long __stdcall UnlockBox(unsigned int level) = 0;                                  // 20
    virtual long __stdcall AddDirtyBox(const void* dirtyBox) = 0;                              // 21
};
#endif
