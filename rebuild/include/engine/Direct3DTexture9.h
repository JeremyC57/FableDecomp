#ifndef FABLE_ENGINE_DIRECT3DTEXTURE9_H
#define FABLE_ENGINE_DIRECT3DTEXTURE9_H
#ifdef FABLE_RETAIL_PASS22_D3D_TEXTURE_ORIGINAL_BASES
// Official DirectX9 SDK d3d9.h interfaces, both original PDB base offsets0.
// Native/Ego texture size4; later D3D_DEBUG_INFO-only52-byte members excluded.
#include "engine/Direct3DSurface9.h"
enum _D3DTEXTUREFILTERTYPE { D3DTEXF_NONE=0,D3DTEXF_POINT=1,D3DTEXF_LINEAR=2,D3DTEXF_ANISOTROPIC=3,D3DTEXF_PYRAMIDALQUAD=6,D3DTEXF_GAUSSIANQUAD=7,D3DTEXF_FORCE_DWORD=0x7fffffff };
struct IDirect3DBaseTexture9 : public IDirect3DResource9 {
    virtual long FABLE_STDCALL QueryInterface(const _GUID& riid, void** ppvObj) = 0;
    virtual unsigned long FABLE_STDCALL AddRef() = 0;
    virtual unsigned long FABLE_STDCALL Release() = 0;
    virtual long FABLE_STDCALL GetDevice(IDirect3DDevice9** ppDevice) = 0;
    virtual long FABLE_STDCALL SetPrivateData(const _GUID& refguid,const void* pData,unsigned long SizeOfData,unsigned long Flags) = 0;
    virtual long FABLE_STDCALL GetPrivateData(const _GUID& refguid,void* pData,unsigned long* pSizeOfData) = 0;
    virtual long FABLE_STDCALL FreePrivateData(const _GUID& refguid) = 0;
    virtual unsigned long FABLE_STDCALL SetPriority(unsigned long PriorityNew) = 0;
    virtual unsigned long FABLE_STDCALL GetPriority() = 0;
    virtual void FABLE_STDCALL PreLoad() = 0;
    virtual _D3DRESOURCETYPE FABLE_STDCALL GetType() = 0;
    virtual unsigned long FABLE_STDCALL SetLOD(unsigned long LODNew) = 0;
    virtual unsigned long FABLE_STDCALL GetLOD() = 0;
    virtual unsigned long FABLE_STDCALL GetLevelCount() = 0;
    virtual long FABLE_STDCALL SetAutoGenFilterType(_D3DTEXTUREFILTERTYPE FilterType) = 0;
    virtual _D3DTEXTUREFILTERTYPE FABLE_STDCALL GetAutoGenFilterType() = 0;
    virtual void FABLE_STDCALL GenerateMipSubLevels() = 0;
};
struct IDirect3DTexture9 : public IDirect3DBaseTexture9 {
    virtual long FABLE_STDCALL QueryInterface(const _GUID& riid, void** ppvObj) = 0;
    virtual unsigned long FABLE_STDCALL AddRef() = 0;
    virtual unsigned long FABLE_STDCALL Release() = 0;
    virtual long FABLE_STDCALL GetDevice(IDirect3DDevice9** ppDevice) = 0;
    virtual long FABLE_STDCALL SetPrivateData(const _GUID& refguid,const void* pData,unsigned long SizeOfData,unsigned long Flags) = 0;
    virtual long FABLE_STDCALL GetPrivateData(const _GUID& refguid,void* pData,unsigned long* pSizeOfData) = 0;
    virtual long FABLE_STDCALL FreePrivateData(const _GUID& refguid) = 0;
    virtual unsigned long FABLE_STDCALL SetPriority(unsigned long PriorityNew) = 0;
    virtual unsigned long FABLE_STDCALL GetPriority() = 0;
    virtual void FABLE_STDCALL PreLoad() = 0;
    virtual _D3DRESOURCETYPE FABLE_STDCALL GetType() = 0;
    virtual unsigned long FABLE_STDCALL SetLOD(unsigned long LODNew) = 0;
    virtual unsigned long FABLE_STDCALL GetLOD() = 0;
    virtual unsigned long FABLE_STDCALL GetLevelCount() = 0;
    virtual long FABLE_STDCALL SetAutoGenFilterType(_D3DTEXTUREFILTERTYPE FilterType) = 0;
    virtual _D3DTEXTUREFILTERTYPE FABLE_STDCALL GetAutoGenFilterType() = 0;
    virtual void FABLE_STDCALL GenerateMipSubLevels() = 0;
    virtual long FABLE_STDCALL GetLevelDesc(unsigned int Level,_D3DSURFACE_DESC* pDesc) = 0;
    virtual long FABLE_STDCALL GetSurfaceLevel(unsigned int Level,IDirect3DSurface9** ppSurfaceLevel) = 0;
    virtual long FABLE_STDCALL LockRect(unsigned int Level,_D3DLOCKED_RECT* pLockedRect,const tagRECT* pRect,unsigned long Flags) = 0;
    virtual long FABLE_STDCALL UnlockRect(unsigned int Level) = 0;
    virtual long FABLE_STDCALL AddDirtyRect(const tagRECT* pDirtyRect) = 0;
};
#endif
#endif
