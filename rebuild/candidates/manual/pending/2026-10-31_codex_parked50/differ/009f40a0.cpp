// 009f40a0 CSurface::Clear
// Codex parked-50 best attempt: DIFFER(120v120); residue: 120/120 bytes; Width/Height multiply order and memset destination load timing differ.
// Leaf: ?Clear@CSurface@@QAEXXZ
// Flags:
#include <stddef.h>
#include "rebuild_abi.h"
struct _GUID;
struct HDC__;
struct IDirect3DDevice9;
enum _D3DFORMAT { D3DFMT_UNKNOWN=0 };
enum _D3DRESOURCETYPE { D3DRTYPE_SURFACE=1 };
enum _D3DPOOL { D3DPOOL_DEFAULT=0 };
enum _D3DMULTISAMPLE_TYPE { D3DMULTISAMPLE_NONE=0 };
struct _D3DSURFACE_DESC { _D3DFORMAT Format;_D3DRESOURCETYPE Type;unsigned long Usage;_D3DPOOL Pool;_D3DMULTISAMPLE_TYPE MultiSampleType;unsigned long MultiSampleQuality;unsigned int Width,Height; };
struct tagRECT {long left,top,right,bottom;};
struct _D3DLOCKED_RECT {int Pitch;void*pBits;};
struct IUnknown{virtual long __stdcall QueryInterface(const _GUID&,void**)=0;virtual unsigned long __stdcall AddRef()=0;virtual unsigned long __stdcall Release()=0;};
struct IDirect3DResource9:public IUnknown{virtual long __stdcall GetDevice(IDirect3DDevice9**)=0;virtual long __stdcall SetPrivateData(const _GUID&,const void*,unsigned long,unsigned long)=0;virtual long __stdcall GetPrivateData(const _GUID&,void*,unsigned long*)=0;virtual long __stdcall FreePrivateData(const _GUID&)=0;virtual unsigned long __stdcall SetPriority(unsigned long)=0;virtual unsigned long __stdcall GetPriority()=0;virtual void __stdcall PreLoad()=0;virtual _D3DRESOURCETYPE __stdcall GetType()=0;};
struct IDirect3DSurface9:public IDirect3DResource9{
 virtual long __stdcall GetContainer(const _GUID&,void**)=0;virtual long __stdcall GetDesc(_D3DSURFACE_DESC*)=0;virtual long __stdcall LockRect(_D3DLOCKED_RECT*,const tagRECT*,unsigned long)=0;virtual long __stdcall UnlockRect()=0;virtual long __stdcall GetDC(HDC__**)=0;virtual long __stdcall ReleaseDC(HDC__*)=0;
};
class C2DExtentsI{public:long X,Y;C2DExtentsI(){}C2DExtentsI(long x,long y){Initialise(x,y);}void Initialise(long x,long y){X=x;Y=y;}C2DExtentsI(const C2DExtentsI&o):X(o.X),Y(o.Y){}};
class CSurfaceLockedRect{public:C2DExtentsI Dimensions;long Pitch;unsigned char*PMem;CSurfaceLockedRect(){}CSurfaceLockedRect(const CSurfaceLockedRect&o):Dimensions(o.Dimensions),Pitch(o.Pitch),PMem(o.PMem){}};
#define CPixelFormat CPixelFormatFields
#include "engine/CPixelFormat.h"
#undef CPixelFormat
class CPixelFormat{public:long FormatIndex;CPixelFormat(_D3DFORMAT f){Initialise(f);}void Initialise(_D3DFORMAT);unsigned long GetColourDepth()const;};
#define CSurface CSurfaceFields
#include "engine/CSurface.h"
#undef CSurface
// Fidelity: input-reading analysis context only for the pixel-format getter.
#pragma auto_inline(off)
unsigned long CPixelFormat::GetColourDepth()const{return FormatIndex==0?0UL:32UL;}
#pragma auto_inline(on)
class CSurface{public:virtual~CSurface();IDirect3DSurface9*PD3DSurface;unsigned long AllocationSource;void*PAllocatedMemory;
 CSurfaceLockedRect Lock(unsigned long);
 C2DExtentsI GetDimensions()const{_D3DSURFACE_DESC desc;PD3DSurface->GetDesc(&desc);return C2DExtentsI(desc.Width,desc.Height);}
 CPixelFormat GetPixelFormat()const{_D3DSURFACE_DESC desc;PD3DSurface->GetDesc(&desc);return CPixelFormat(desc.Format);}
 unsigned long GetByteLength()const{_D3DSURFACE_DESC desc;PD3DSurface->GetDesc(&desc);return GetPixelFormat().GetColourDepth()*desc.Height*desc.Width/8;}
 void Unlock()const{PD3DSurface->UnlockRect();}
 void Clear();
};
extern "C" void* __cdecl memset(void*,int,unsigned int);
#pragma intrinsic(memset)
FABLE_STATIC_ASSERT(sizeof(CSurface)==sizeof(CSurfaceFields));
FABLE_STATIC_ASSERT(sizeof(CPixelFormat)==sizeof(CPixelFormatFields));
void CSurface::Clear(){CSurfaceLockedRect locked=Lock(0);unsigned long length=GetByteLength();memset(locked.PMem,0,length);Unlock();}
