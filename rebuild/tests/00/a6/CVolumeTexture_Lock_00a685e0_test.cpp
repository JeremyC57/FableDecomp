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
// 00a685e0 CVolumeTexture::Lock
// Leaf: ?Lock@CVolumeTexture@@QAE?AVCVolumeLockedBox@@KK@Z
// Flags:
#include <string.h>
#include "rebuild_abi.h"
struct _GUID;struct IDirect3DDevice9;
enum _D3DFORMAT{D3DFMT_UNKNOWN=0};enum _D3DRESOURCETYPE{D3DRTYPE_VOLUME=2};enum _D3DPOOL{D3DPOOL_DEFAULT=0};enum _D3DTEXTUREFILTERTYPE{D3DTEXF_NONE=0};
struct _D3DVOLUME_DESC{_D3DFORMAT Format;_D3DRESOURCETYPE Type;unsigned long Usage;_D3DPOOL Pool;unsigned int Width,Height,Depth;};struct _D3DBOX{unsigned int Left,Top,Right,Bottom,Front,Back;};struct _D3DLOCKED_BOX{int RowPitch,SlicePitch;void*pBits;};
struct IUnknown{virtual long __stdcall QueryInterface(const _GUID&,void**)=0;virtual unsigned long __stdcall AddRef()=0;virtual unsigned long __stdcall Release()=0;};
struct IDirect3DResource9:public IUnknown{virtual long __stdcall GetDevice(IDirect3DDevice9**)=0;virtual long __stdcall SetPrivateData(const _GUID&,const void*,unsigned long,unsigned long)=0;virtual long __stdcall GetPrivateData(const _GUID&,void*,unsigned long*)=0;virtual long __stdcall FreePrivateData(const _GUID&)=0;virtual unsigned long __stdcall SetPriority(unsigned long)=0;virtual unsigned long __stdcall GetPriority()=0;virtual void __stdcall PreLoad()=0;virtual _D3DRESOURCETYPE __stdcall GetType()=0;};
struct IDirect3DBaseTexture9:public IDirect3DResource9{virtual unsigned long __stdcall SetLOD(unsigned long)=0;virtual unsigned long __stdcall GetLOD()=0;virtual unsigned long __stdcall GetLevelCount()=0;virtual long __stdcall SetAutoGenFilterType(_D3DTEXTUREFILTERTYPE)=0;virtual _D3DTEXTUREFILTERTYPE __stdcall GetAutoGenFilterType()=0;virtual void __stdcall GenerateMipSubLevels()=0;};
struct IDirect3DVolume9;struct IDirect3DVolumeTexture9:public IDirect3DBaseTexture9{virtual long __stdcall GetLevelDesc(unsigned int,_D3DVOLUME_DESC*)=0;virtual long __stdcall GetVolumeLevel(unsigned int,IDirect3DVolume9**)=0;virtual long __stdcall LockBox(unsigned int,_D3DLOCKED_BOX*,const _D3DBOX*,unsigned long)=0;virtual long __stdcall UnlockBox(unsigned int)=0;virtual long __stdcall AddDirtyBox(const _D3DBOX*)=0;};
#define CRenderStateManager CRenderStateManagerFields
#include "engine/CRenderStateManager.h"
#undef CRenderStateManager
class CBaseClass{public:virtual~CBaseClass();};class CBaseClassNonCopyable:public CBaseClass{protected:virtual~CBaseClassNonCopyable();};class CInitBaseClass:public CBaseClass{public:bool Valid;};
// Layout: receiver-only render declarations restore true primary bases; unread render-state storage is opaque and checked against its existing header.
class CRenderStateManager:public CBaseClassNonCopyable{unsigned char unread[0x3a38];};class CRenderManagerCore:public CRenderStateManager{public:void WaitWhileResourceIsBusy(IDirect3DResource9*,float);};class CRenderManager2D:public CRenderManagerCore{};class CRenderManager:public CRenderManager2D{};
namespace boost{template<class T>class scoped_ptr{T*ptr;public:T*operator->()const{return ptr;}};}
class CSystemManager;class CDisplayManager:public CBaseClassNonCopyable{public:CSystemManager*SystemManager;boost::scoped_ptr<CRenderManager>PRenderManager;};class CCPPointerInfo;template<class T>class CCountedPointer{public:T*Data;CCPPointerInfo*Info;};
class CSystemManager:public CInitBaseClass{unsigned char unread[0x58];public:CCountedPointer<CDisplayManager>PDisplayManager;};CSystemManager*__fastcall GFGetSystemManager();
// Layout: the original nonpolymorphic parent of C3DExtentsI is unproved. Keep its first eight bytes opaque; SDK Width/Height words are implementation-only physical projections, not invented members or base classes.
class C3DExtentsI{public:unsigned char prefix[8];long Z;C3DExtentsI(){}C3DExtentsI(long x,long y,long z){memcpy(prefix,&x,4);memcpy(prefix+4,&y,4);Z=z;}C3DExtentsI(const C3DExtentsI&o){memcpy(prefix,o.prefix,8);Z=o.Z;}};
class CVolumeLockedBox{public:C3DExtentsI Dimensions;long RowPitch,SlicePitch;unsigned char*PMem;CVolumeLockedBox(){}CVolumeLockedBox(const CVolumeLockedBox&o):Dimensions(o.Dimensions),RowPitch(o.RowPitch),SlicePitch(o.SlicePitch),PMem(o.PMem){}};
class CVolumeTexture:public CBaseClass{public:IDirect3DVolumeTexture9*PD3DVolumeTexture;long ByteLength;void*PData;virtual~CVolumeTexture();C3DExtentsI GetDimensions(unsigned long level)const{_D3DVOLUME_DESC desc;PD3DVolumeTexture->GetLevelDesc(level,&desc);return C3DExtentsI(desc.Width,desc.Height,desc.Depth);}CVolumeLockedBox Lock(unsigned long,unsigned long);};
CVolumeLockedBox CVolumeTexture::Lock(unsigned long level,unsigned long flags){C3DExtentsI dimensions=GetDimensions(level);_D3DBOX box;box.Front=0;box.Top=0;box.Left=0;box.Bottom=reinterpret_cast<const long*>(dimensions.prefix)[1];box.Right=reinterpret_cast<const long*>(dimensions.prefix)[0];box.Back=dimensions.Z;CSystemManager*system=GFGetSystemManager();system->PDisplayManager.Data->PRenderManager->WaitWhileResourceIsBusy(PD3DVolumeTexture,1.0f);_D3DLOCKED_BOX locked;if(PD3DVolumeTexture->LockBox(level,&locked,&box,flags)>=0){CVolumeLockedBox result;result.Dimensions=dimensions;result.RowPitch=locked.RowPitch;result.SlicePitch=locked.SlicePitch;result.PMem=(unsigned char*)locked.pBits;return result;}return CVolumeLockedBox();}
FABLE_STATIC_ASSERT(sizeof(CVolumeTexture)==16);FABLE_STATIC_ASSERT(sizeof(CRenderStateManager)==sizeof(CRenderStateManagerFields));FABLE_STATIC_ASSERT(sizeof(CVolumeLockedBox)==24);

static int testError,testPhase,testDescCalls,testLocks,testSystems,testWaits;static _D3DVOLUME_DESC testDesc;static unsigned long testLevel,testFlags;static long testDescStatus,testLockStatus,testRowPitch,testSlicePitch;static unsigned char*testMemory;static CSystemManager*testSystem;static CRenderManagerCore*testRender;static IDirect3DVolumeTexture9*testDevice;
CBaseClass::~CBaseClass(){}CBaseClassNonCopyable::~CBaseClassNonCopyable(){}CVolumeTexture::~CVolumeTexture(){}
CSystemManager*__fastcall GFGetSystemManager(){if(testPhase!=1)testError=1;testPhase=2;++testSystems;return testSystem;}
void CRenderManagerCore::WaitWhileResourceIsBusy(IDirect3DResource9*resource,float duration){if(this!=testRender||resource!=static_cast<IDirect3DResource9*>(testDevice)||duration!=1.0f||testPhase!=2)testError=2;testPhase=3;++testWaits;}
struct TestVolumeTexture:IDirect3DVolumeTexture9{
 virtual long __stdcall QueryInterface(const _GUID&,void**){testError=3;return -1;}virtual unsigned long __stdcall AddRef(){testError=3;return 0;}virtual unsigned long __stdcall Release(){testError=3;return 0;}
 virtual long __stdcall GetDevice(IDirect3DDevice9**){testError=3;return -1;}virtual long __stdcall SetPrivateData(const _GUID&,const void*,unsigned long,unsigned long){testError=3;return -1;}virtual long __stdcall GetPrivateData(const _GUID&,void*,unsigned long*){testError=3;return -1;}virtual long __stdcall FreePrivateData(const _GUID&){testError=3;return -1;}virtual unsigned long __stdcall SetPriority(unsigned long){testError=3;return 0;}virtual unsigned long __stdcall GetPriority(){testError=3;return 0;}virtual void __stdcall PreLoad(){testError=3;}virtual _D3DRESOURCETYPE __stdcall GetType(){testError=3;return D3DRTYPE_VOLUME;}
 virtual unsigned long __stdcall SetLOD(unsigned long){testError=3;return 0;}virtual unsigned long __stdcall GetLOD(){testError=3;return 0;}virtual unsigned long __stdcall GetLevelCount(){testError=3;return 0;}virtual long __stdcall SetAutoGenFilterType(_D3DTEXTUREFILTERTYPE){testError=3;return -1;}virtual _D3DTEXTUREFILTERTYPE __stdcall GetAutoGenFilterType(){testError=3;return D3DTEXF_NONE;}virtual void __stdcall GenerateMipSubLevels(){testError=3;}

 virtual long __stdcall GetLevelDesc(unsigned int level,_D3DVOLUME_DESC*desc){if(testPhase||level!=testLevel)testError=4;testPhase=1;*desc=testDesc;++testDescCalls;return testDescStatus;}
 virtual long __stdcall GetVolumeLevel(unsigned int,IDirect3DVolume9**){testError=3;return -1;}
 virtual long __stdcall LockBox(unsigned int level,_D3DLOCKED_BOX*locked,const _D3DBOX*box,unsigned long flags){if(testPhase!=3||level!=testLevel||flags!=testFlags||!box||box->Left||box->Top||box->Front||box->Right!=testDesc.Width||box->Bottom!=testDesc.Height||box->Back!=testDesc.Depth)testError=5;testPhase=4;locked->RowPitch=testRowPitch;locked->SlicePitch=testSlicePitch;locked->pBits=testMemory;++testLocks;return testLockStatus;}
 virtual long __stdcall UnlockBox(unsigned int){testError=3;return -1;}virtual long __stdcall AddDirtyBox(const _D3DBOX*){testError=3;return -1;}
};

int main(){const unsigned long values[]={0,1,17,0x7fffffffUL,0x80000000UL,0xffffffffUL},flags[]={0,1,16,0x12345678UL,0xffffffffUL},levels[]={0,1,3,0x80000000UL,0xffffffffUL};const long pitches[]={0,-1,257},statuses[]={-1,0,1,(long)0x80004005UL};CVolumeTexture texture;TestVolumeTexture device;CSystemManager system;CDisplayManager display;CRenderManager renderer;unsigned char memory[16];texture.PD3DVolumeTexture=&device;texture.ByteLength=0x12345678L;texture.PData=memory+7;system.PDisplayManager.Data=&display;system.PDisplayManager.Info=0;CRenderManager*render=&renderer;memcpy(&display.PRenderManager,&render,4);testSystem=&system;testRender=&renderer;testDevice=&device;int cases=0;
 for(int width=0;width<6;++width)for(int height=0;height<6;++height)for(int depth=0;depth<6;++depth)for(int f=0;f<5;++f)for(int row=0;row<3;++row)for(int slice=0;slice<3;++slice)for(int status=0;status<4;++status)for(int descStatus=0;descStatus<3;++descStatus)for(int level=0;level<5;++level){memset(&testDesc,0x71,sizeof testDesc);testDesc.Width=values[width];testDesc.Height=values[height];testDesc.Depth=values[depth];testFlags=flags[f];testLevel=levels[level];testRowPitch=pitches[row];testSlicePitch=pitches[slice];testLockStatus=statuses[status];testDescStatus=descStatus-1;testMemory=memory+(f+row+slice)%16;testError=testPhase=testDescCalls=testLocks=testSystems=testWaits=0;
 // Native failure result fields are unspecified; test the calls and ordering while discarding that result.
 if(testLockStatus<0)texture.Lock(testLevel,testFlags);else{CVolumeLockedBox result=texture.Lock(testLevel,testFlags);long prefix[2];memcpy(prefix,result.Dimensions.prefix,8);if(prefix[0]!=(long)testDesc.Width||prefix[1]!=(long)testDesc.Height||result.Dimensions.Z!=(long)testDesc.Depth||result.RowPitch!=testRowPitch||result.SlicePitch!=testSlicePitch||result.PMem!=testMemory)return 1;}
 if(testError||testPhase!=4||testDescCalls!=1||testLocks!=1||testSystems!=1||testWaits!=1||texture.PD3DVolumeTexture!=&device||texture.ByteLength!=0x12345678L||texture.PData!=memory+7)return 2;++cases;
 }
 if(cases!=583200)return 3;puts("00a685e0_TEST PASS");return 0;}