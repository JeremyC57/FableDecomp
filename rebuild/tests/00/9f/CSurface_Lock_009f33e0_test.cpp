#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __stdcall
#define __declspec(x)
#define __forceinline inline
#define __int64 __attribute__((__mode__(__DI__))) long
#endif
#define PASS9_TEST
// 009f33e0 CSurface::Lock
// Leaf: ?Lock@CSurface@@QAE?AVCSurfaceLockedRect@@K@Z
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
#define CSurface CSurfaceFields
#include "engine/CSurface.h"
#undef CSurface
// Layout: the imported raw header is a layout check. The original class is polymorphic and has no RTTI base classes.
class CSurface{public:virtual~CSurface();IDirect3DSurface9*PD3DSurface;unsigned long AllocationSource;void*PAllocatedMemory;
 CSurfaceLockedRect Lock(unsigned long);
 C2DExtentsI GetDimensions()const{_D3DSURFACE_DESC desc;PD3DSurface->GetDesc(&desc);return C2DExtentsI(desc.Width,desc.Height);}
};
FABLE_STATIC_ASSERT(sizeof(CSurface)==sizeof(CSurfaceFields));

CSurfaceLockedRect CSurface::Lock(unsigned long flags){
 C2DExtentsI dimensions=GetDimensions();
 tagRECT rect;rect.top=0;rect.left=0;rect.bottom=dimensions.Y;rect.right=dimensions.X;
 _D3DLOCKED_RECT locked;
 if(PD3DSurface->LockRect(&locked,&rect,flags)>=0){
  CSurfaceLockedRect result;result.Dimensions=dimensions;result.Pitch=locked.Pitch;result.PMem=(unsigned char*)locked.pBits;return result;
 }
 CSurfaceLockedRect result;result.Pitch=0;result.PMem=0;result.Dimensions=C2DExtentsI(0,0);return result;
}

FABLE_STATIC_ASSERT(sizeof(C2DExtentsI)==8);
FABLE_STATIC_ASSERT(sizeof(CSurfaceLockedRect)==16);

static int testError,testDescCalls,testLockCalls;static _D3DSURFACE_DESC testDesc;static long testDescStatus,testLockStatus;static unsigned long testFlags;static long testPitch;static unsigned char*testMemory;
struct TestSurface:IDirect3DSurface9{
 virtual long __stdcall QueryInterface(const _GUID&,void**){testError=1;return -1;}virtual unsigned long __stdcall AddRef(){testError=1;return 0;}virtual unsigned long __stdcall Release(){testError=1;return 0;}
 virtual long __stdcall GetDevice(IDirect3DDevice9**){testError=1;return -1;}virtual long __stdcall SetPrivateData(const _GUID&,const void*,unsigned long,unsigned long){testError=1;return -1;}virtual long __stdcall GetPrivateData(const _GUID&,void*,unsigned long*){testError=1;return -1;}virtual long __stdcall FreePrivateData(const _GUID&){testError=1;return -1;}
 virtual unsigned long __stdcall SetPriority(unsigned long){testError=1;return 0;}virtual unsigned long __stdcall GetPriority(){testError=1;return 0;}virtual void __stdcall PreLoad(){testError=1;}virtual _D3DRESOURCETYPE __stdcall GetType(){testError=1;return D3DRTYPE_SURFACE;}
 virtual long __stdcall GetContainer(const _GUID&,void**){testError=1;return -1;}virtual long __stdcall GetDesc(_D3DSURFACE_DESC*desc){if(testLockCalls)testError=2;*desc=testDesc;++testDescCalls;return testDescStatus;}
 virtual long __stdcall LockRect(_D3DLOCKED_RECT*locked,const tagRECT*rect,unsigned long flags){if(testDescCalls!=1||!rect||rect->left||rect->top||rect->right!=(long)testDesc.Width||rect->bottom!=(long)testDesc.Height||flags!=testFlags)testError=3;locked->Pitch=testPitch;locked->pBits=testMemory;++testLockCalls;return testLockStatus;}
 virtual long __stdcall UnlockRect(){testError=1;return -1;}virtual long __stdcall GetDC(HDC__**){testError=1;return -1;}virtual long __stdcall ReleaseDC(HDC__*){testError=1;return -1;}
};
CSurface::~CSurface(){}
int main(){CSurface surface;TestSurface device;unsigned char memory[16];surface.PD3DSurface=&device;surface.AllocationSource=0x63ab1e57UL;surface.PAllocatedMemory=memory+7;const unsigned long sizes[]={0,1,7,0x7fffffffUL,0x80000000UL,0xffffffffUL};const unsigned long flags[]={0,1,16,0x12345678UL,0xffffffffUL};const long pitches[]={0,1,64,-1,-128};const long statuses[]={-1,0,1,(long)0x80004005UL};int cases=0;
 for(int width=0;width<6;++width)for(int height=0;height<6;++height)for(int f=0;f<5;++f)for(int p=0;p<5;++p)for(int status=0;status<4;++status)for(int descStatus=0;descStatus<3;++descStatus){
  memset(&testDesc,0x71,sizeof testDesc);testDesc.Width=sizes[width];testDesc.Height=sizes[height];testDescStatus=descStatus-1;testLockStatus=statuses[status];testFlags=flags[f];testPitch=pitches[p];testMemory=memory+(f+p)%16;testError=testDescCalls=testLockCalls=0;
  CSurfaceLockedRect actual=surface.Lock(testFlags);if(testError||testDescCalls!=1||testLockCalls!=1||surface.PD3DSurface!=&device||surface.AllocationSource!=0x63ab1e57UL||surface.PAllocatedMemory!=memory+7)return 1;
  if(testLockStatus>=0){if(actual.Dimensions.X!=(long)testDesc.Width||actual.Dimensions.Y!=(long)testDesc.Height||actual.Pitch!=testPitch||actual.PMem!=testMemory)return 2;}else if(actual.Dimensions.X||actual.Dimensions.Y||actual.Pitch||actual.PMem)return 3;++cases;
 }
 if(cases!=10800)return 4;puts("009f33e0_TEST PASS");return 0;}