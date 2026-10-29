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
// 00a67530 CVolume::ReadOnlyLock
// Leaf: ?ReadOnlyLock@CVolume@@QBE?AVCVolumeLockedBox@@K@Z
// Flags:
#include <string.h>
#include "rebuild_abi.h"
struct _GUID;struct IDirect3DDevice9;
enum _D3DFORMAT{D3DFMT_UNKNOWN=0};enum _D3DRESOURCETYPE{D3DRTYPE_VOLUME=2};enum _D3DPOOL{D3DPOOL_DEFAULT=0};
struct _D3DVOLUME_DESC{_D3DFORMAT Format;_D3DRESOURCETYPE Type;unsigned long Usage;_D3DPOOL Pool;unsigned int Width,Height,Depth;};
struct _D3DBOX{unsigned int Left,Top,Right,Bottom,Front,Back;};struct _D3DLOCKED_BOX{int RowPitch,SlicePitch;void*pBits;};
struct IUnknown{virtual long __stdcall QueryInterface(const _GUID&,void**)=0;virtual unsigned long __stdcall AddRef()=0;virtual unsigned long __stdcall Release()=0;};
struct IDirect3DVolume9:public IUnknown{virtual long __stdcall GetDevice(IDirect3DDevice9**)=0;virtual long __stdcall SetPrivateData(const _GUID&,const void*,unsigned long,unsigned long)=0;virtual long __stdcall GetPrivateData(const _GUID&,void*,unsigned long*)=0;virtual long __stdcall FreePrivateData(const _GUID&)=0;virtual long __stdcall GetContainer(const _GUID&,void**)=0;virtual long __stdcall GetDesc(_D3DVOLUME_DESC*)=0;virtual long __stdcall LockBox(_D3DLOCKED_BOX*,const _D3DBOX*,unsigned long)=0;virtual long __stdcall UnlockBox()=0;};
class CBaseClass{public:virtual~CBaseClass();};
// Layout: the original nonpolymorphic parent of C3DExtentsI is unproved. Keep its first eight bytes opaque; SDK Width/Height words are implementation-only physical projections, not invented members or base classes.
class C3DExtentsI{public:unsigned char prefix[8];long Z;C3DExtentsI(){}C3DExtentsI(long x,long y,long z){memcpy(prefix,&x,4);memcpy(prefix+4,&y,4);Z=z;}C3DExtentsI(const C3DExtentsI&o){memcpy(prefix,o.prefix,8);Z=o.Z;}};
class CVolumeLockedBox{public:C3DExtentsI Dimensions;long RowPitch,SlicePitch;unsigned char*PMem;CVolumeLockedBox(){}CVolumeLockedBox(const CVolumeLockedBox&o):Dimensions(o.Dimensions),RowPitch(o.RowPitch),SlicePitch(o.SlicePitch),PMem(o.PMem){}};
class CVolume:public CBaseClass{public:IDirect3DVolume9*PD3DVolume;virtual~CVolume();C3DExtentsI GetDimensions()const{_D3DVOLUME_DESC desc;PD3DVolume->GetDesc(&desc);return C3DExtentsI(desc.Width,desc.Height,desc.Depth);}CVolumeLockedBox Lock(unsigned long);CVolumeLockedBox ReadOnlyLock(unsigned long)const;};
FABLE_STATIC_ASSERT(sizeof(C3DExtentsI)==12);FABLE_STATIC_ASSERT(sizeof(CVolumeLockedBox)==24);FABLE_STATIC_ASSERT(sizeof(CVolume)==8);
CVolumeLockedBox CVolume::ReadOnlyLock(unsigned long flags)const{
 C3DExtentsI dimensions=GetDimensions();_D3DBOX box;box.Front=0;box.Top=0;box.Left=0;box.Bottom=reinterpret_cast<const long*>(dimensions.prefix)[1];box.Right=reinterpret_cast<const long*>(dimensions.prefix)[0];box.Back=dimensions.Z;_D3DLOCKED_BOX locked;
 if(PD3DVolume->LockBox(&locked,&box,flags|0x10)>=0){CVolumeLockedBox result;result.Dimensions=dimensions;result.RowPitch=locked.RowPitch;result.SlicePitch=locked.SlicePitch;result.PMem=(unsigned char*)locked.pBits;return result;}return CVolumeLockedBox();
}

static int testError,testDescCalls,testLocks;static _D3DVOLUME_DESC testDesc;static long testDescStatus,testLockStatus,testRowPitch,testSlicePitch;static unsigned long testFlags;static unsigned char*testMemory;
struct TestVolume:IDirect3DVolume9{
 virtual long __stdcall QueryInterface(const _GUID&,void**){testError=1;return -1;}virtual unsigned long __stdcall AddRef(){testError=1;return 0;}virtual unsigned long __stdcall Release(){testError=1;return 0;}virtual long __stdcall GetDevice(IDirect3DDevice9**){testError=1;return -1;}virtual long __stdcall SetPrivateData(const _GUID&,const void*,unsigned long,unsigned long){testError=1;return -1;}virtual long __stdcall GetPrivateData(const _GUID&,void*,unsigned long*){testError=1;return -1;}virtual long __stdcall FreePrivateData(const _GUID&){testError=1;return -1;}virtual long __stdcall GetContainer(const _GUID&,void**){testError=1;return -1;}
 virtual long __stdcall GetDesc(_D3DVOLUME_DESC*desc){if(testLocks)testError=2;*desc=testDesc;++testDescCalls;return testDescStatus;}
 virtual long __stdcall LockBox(_D3DLOCKED_BOX*locked,const _D3DBOX*box,unsigned long flags){if(testDescCalls!=1||flags!=(testFlags|0x10)||!box||box->Left||box->Top||box->Front||box->Right!=testDesc.Width||box->Bottom!=testDesc.Height||box->Back!=testDesc.Depth)testError=3;locked->RowPitch=testRowPitch;locked->SlicePitch=testSlicePitch;locked->pBits=testMemory;++testLocks;return testLockStatus;}
 virtual long __stdcall UnlockBox(){testError=1;return -1;}
};
CBaseClass::~CBaseClass(){}CVolume::~CVolume(){}
int main(){const unsigned long values[]={0,1,17,0x7fffffffUL,0x80000000UL,0xffffffffUL},flags[]={0,1,16,0x12345678UL,0xffffffffUL};const long pitches[]={0,-1,257},statuses[]={-1,0,1,(long)0x80004005UL};CVolume volume;TestVolume device;unsigned char memory[16];volume.PD3DVolume=&device;int cases=0;
 for(int width=0;width<6;++width)for(int height=0;height<6;++height)for(int depth=0;depth<6;++depth)for(int f=0;f<5;++f)for(int row=0;row<3;++row)for(int slice=0;slice<3;++slice)for(int status=0;status<4;++status)for(int descStatus=0;descStatus<3;++descStatus){memset(&testDesc,0x71,sizeof testDesc);testDesc.Width=values[width];testDesc.Height=values[height];testDesc.Depth=values[depth];testFlags=flags[f];testRowPitch=pitches[row];testSlicePitch=pitches[slice];testLockStatus=statuses[status];testDescStatus=descStatus-1;testMemory=memory+(f+row+slice)%16;testError=testDescCalls=testLocks=0;
  // The native failure result is unspecified; exercise its protocol while discarding that result.
  if(testLockStatus<0)volume.ReadOnlyLock(testFlags);else{CVolumeLockedBox result=volume.ReadOnlyLock(testFlags);long prefix[2];memcpy(prefix,result.Dimensions.prefix,8);if(prefix[0]!=(long)testDesc.Width||prefix[1]!=(long)testDesc.Height||result.Dimensions.Z!=(long)testDesc.Depth||result.RowPitch!=testRowPitch||result.SlicePitch!=testSlicePitch||result.PMem!=testMemory)return 1;}
  if(testError||testDescCalls!=1||testLocks!=1||volume.PD3DVolume!=&device)return 2;++cases;
 }
 if(cases!=116640)return 3;puts("00a67530_TEST PASS");return 0;}