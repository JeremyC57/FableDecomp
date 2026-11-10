#ifndef FABLE_ENGINE_DIRECT3DSURFACE9_H
#define FABLE_ENGINE_DIRECT3DSURFACE9_H
#include "rebuild_abi.h"
#include "engine/Direct3D9Types.h"
struct _GUID;struct HDC__;struct IDirect3DDevice9;
enum _D3DRESOURCETYPE{D3DRTYPE_SURFACE=1};enum _D3DPOOL{D3DPOOL_DEFAULT=0};
struct _D3DSURFACE_DESC{_D3DFORMAT Format;_D3DRESOURCETYPE Type;unsigned long Usage;_D3DPOOL Pool;_D3DMULTISAMPLE_TYPE MultiSampleType;unsigned long MultiSampleQuality;unsigned int Width,Height;};
struct tagRECT{long left,top,right,bottom;};struct _D3DLOCKED_RECT{int Pitch;void* pBits;};
struct IUnknown{virtual long FABLE_STDCALL QueryInterface(const _GUID&,void**)=0;virtual unsigned long FABLE_STDCALL AddRef()=0;virtual unsigned long FABLE_STDCALL Release()=0;};
struct IDirect3DResource9:public IUnknown{virtual long FABLE_STDCALL GetDevice(IDirect3DDevice9**)=0;virtual long FABLE_STDCALL SetPrivateData(const _GUID&,const void*,unsigned long,unsigned long)=0;virtual long FABLE_STDCALL GetPrivateData(const _GUID&,void*,unsigned long*)=0;virtual long FABLE_STDCALL FreePrivateData(const _GUID&)=0;virtual unsigned long FABLE_STDCALL SetPriority(unsigned long)=0;virtual unsigned long FABLE_STDCALL GetPriority()=0;virtual void FABLE_STDCALL PreLoad()=0;virtual _D3DRESOURCETYPE FABLE_STDCALL GetType()=0;};
struct IDirect3DSurface9:public IDirect3DResource9{virtual long FABLE_STDCALL GetContainer(const _GUID&,void**)=0;virtual long FABLE_STDCALL GetDesc(_D3DSURFACE_DESC*)=0;virtual long FABLE_STDCALL LockRect(_D3DLOCKED_RECT*,const tagRECT*,unsigned long)=0;virtual long FABLE_STDCALL UnlockRect()=0;virtual long FABLE_STDCALL GetDC(HDC__**)=0;virtual long FABLE_STDCALL ReleaseDC(HDC__*)=0;};
#endif
