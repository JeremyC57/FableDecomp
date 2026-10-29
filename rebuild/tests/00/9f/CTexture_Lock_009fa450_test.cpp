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
// 009fa450 CTexture::Lock
// Leaf: ?Lock@CTexture@@QAE?AVCSurfaceLockedRect@@KK@Z
// Flags:
#include <stddef.h>
#include "rebuild_abi.h"
struct _GUID;struct HDC__;struct IDirect3DDevice9;
enum _D3DFORMAT{D3DFMT_UNKNOWN=0};enum _D3DRESOURCETYPE{D3DRTYPE_SURFACE=1};enum _D3DPOOL{D3DPOOL_DEFAULT=0};enum _D3DMULTISAMPLE_TYPE{D3DMULTISAMPLE_NONE=0};enum _D3DTEXTUREFILTERTYPE{D3DTEXF_NONE=0};
struct _D3DSURFACE_DESC{_D3DFORMAT Format;_D3DRESOURCETYPE Type;unsigned long Usage;_D3DPOOL Pool;_D3DMULTISAMPLE_TYPE MultiSampleType;unsigned long MultiSampleQuality;unsigned int Width,Height;};
struct tagRECT{long left,top,right,bottom;};struct _D3DLOCKED_RECT{int Pitch;void*pBits;};
struct IUnknown{virtual long __stdcall QueryInterface(const _GUID&,void**)=0;virtual unsigned long __stdcall AddRef()=0;virtual unsigned long __stdcall Release()=0;};
struct IDirect3DResource9:public IUnknown{virtual long __stdcall GetDevice(IDirect3DDevice9**)=0;virtual long __stdcall SetPrivateData(const _GUID&,const void*,unsigned long,unsigned long)=0;virtual long __stdcall GetPrivateData(const _GUID&,void*,unsigned long*)=0;virtual long __stdcall FreePrivateData(const _GUID&)=0;virtual unsigned long __stdcall SetPriority(unsigned long)=0;virtual unsigned long __stdcall GetPriority()=0;virtual void __stdcall PreLoad()=0;virtual _D3DRESOURCETYPE __stdcall GetType()=0;};
struct IDirect3DBaseTexture9:public IDirect3DResource9{virtual unsigned long __stdcall SetLOD(unsigned long)=0;virtual unsigned long __stdcall GetLOD()=0;virtual unsigned long __stdcall GetLevelCount()=0;virtual long __stdcall SetAutoGenFilterType(_D3DTEXTUREFILTERTYPE)=0;virtual _D3DTEXTUREFILTERTYPE __stdcall GetAutoGenFilterType()=0;virtual void __stdcall GenerateMipSubLevels()=0;};
struct IDirect3DSurface9;
struct IDirect3DTexture9:public IDirect3DBaseTexture9{virtual long __stdcall GetLevelDesc(unsigned int,_D3DSURFACE_DESC*)=0;virtual long __stdcall GetSurfaceLevel(unsigned int,IDirect3DSurface9**)=0;virtual long __stdcall LockRect(unsigned int,_D3DLOCKED_RECT*,const tagRECT*,unsigned long)=0;virtual long __stdcall UnlockRect(unsigned int)=0;virtual long __stdcall AddDirtyRect(const tagRECT*)=0;};
#define CTexture CTextureFields
#include "engine/CTexture.h"
#undef CTexture
#define CRenderStateManager CRenderStateManagerFields
#include "engine/CRenderStateManager.h"
#undef CRenderStateManager
class CBaseClass{public:virtual~CBaseClass();};class CBaseClassNonCopyable:public CBaseClass{protected:virtual~CBaseClassNonCopyable();};class CInitBaseClass:public CBaseClass{public:bool Valid;};
// Layout: all unread state of the proven original render-state base remains opaque; receiver-only derived declarations expose the original primary base chain.
class CRenderStateManager:public CBaseClassNonCopyable{unsigned char unread[0x3a38];};
class CRenderManagerCore:public CRenderStateManager{public:void WaitWhileResourceIsBusy(IDirect3DResource9*,float);};
class CRenderManager2D:public CRenderManagerCore{};class CRenderManager:public CRenderManager2D{};
namespace boost{template<class T>class scoped_ptr{T*ptr;public:T*operator->()const{return ptr;}};}
class CSystemManager;
class CDisplayManager:public CBaseClassNonCopyable{public:CSystemManager*SystemManager;boost::scoped_ptr<CRenderManager>PRenderManager;};
class CCPPointerInfo;template<class T>class CCountedPointer{public:T*Data;CCPPointerInfo*Info;};
class CSystemManager:public CInitBaseClass{unsigned char unread[0x58];public:CCountedPointer<CDisplayManager>PDisplayManager;};
CSystemManager*__fastcall GFGetSystemManager();
class C2DExtentsI{public:long X,Y;C2DExtentsI(){}C2DExtentsI(long x,long y){Initialise(x,y);}C2DExtentsI(const C2DExtentsI&o):X(o.X),Y(o.Y){}void Initialise(long x,long y){X=x;Y=y;}};
class CSurfaceLockedRect{public:C2DExtentsI Dimensions;long Pitch;unsigned char*PMem;CSurfaceLockedRect(){}CSurfaceLockedRect(const CSurfaceLockedRect&o):Dimensions(o.Dimensions),Pitch(o.Pitch),PMem(o.PMem){}};
// Layout: the imported header provides the original eight-byte physical record. Only PD3DTexture is read; the packed allocation/length word remains unread.
class CTexture{public:IDirect3DTexture9*PD3DTexture;unsigned char unread[4];CSurfaceLockedRect Lock(unsigned long,unsigned long);C2DExtentsI GetDimensions(unsigned long level)const{_D3DSURFACE_DESC desc;PD3DTexture->GetLevelDesc(level,&desc);return C2DExtentsI(desc.Width,desc.Height);}};
CSurfaceLockedRect CTexture::Lock(unsigned long level,unsigned long flags){
 C2DExtentsI dimensions=GetDimensions(level);tagRECT rect;rect.top=0;rect.left=0;rect.bottom=dimensions.Y;rect.right=dimensions.X;
 CSystemManager*manager=GFGetSystemManager();manager->PDisplayManager.Data->PRenderManager->WaitWhileResourceIsBusy(PD3DTexture,1.0f);
 _D3DLOCKED_RECT locked;if(PD3DTexture->LockRect(level,&locked,&rect,flags)>=0){CSurfaceLockedRect result;result.Dimensions=dimensions;result.Pitch=locked.Pitch;result.PMem=(unsigned char*)locked.pBits;return result;}return CSurfaceLockedRect();
}

FABLE_STATIC_ASSERT(sizeof(CTexture)==sizeof(CTextureFields));
FABLE_STATIC_ASSERT(sizeof(CRenderStateManager)==sizeof(CRenderStateManagerFields));

static int testError,testPhase,testDescCalls,testSystemCalls,testWaitCalls,testLockCalls;static CSystemManager*testSystem;static CRenderManagerCore*testRenderer;static IDirect3DTexture9*testDevice;static _D3DSURFACE_DESC testDesc;static unsigned long testLevel,testFlags;static long testDescStatus,testLockStatus,testPitch;static unsigned char*testMemory;
CBaseClass::~CBaseClass(){}CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CSystemManager*__fastcall GFGetSystemManager(){if(testPhase!=1)testError=1;testPhase=2;++testSystemCalls;return testSystem;}
void CRenderManagerCore::WaitWhileResourceIsBusy(IDirect3DResource9*resource,float duration){if(testPhase!=2||this!=testRenderer||resource!=static_cast<IDirect3DResource9*>(testDevice)||duration!=1.0f)testError=2;testPhase=3;++testWaitCalls;}
struct TestTexture:IDirect3DTexture9{
 virtual long __stdcall QueryInterface(const _GUID&,void**){testError=3;return -1;}virtual unsigned long __stdcall AddRef(){testError=3;return 0;}virtual unsigned long __stdcall Release(){testError=3;return 0;}
 virtual long __stdcall GetDevice(IDirect3DDevice9**){testError=3;return -1;}virtual long __stdcall SetPrivateData(const _GUID&,const void*,unsigned long,unsigned long){testError=3;return -1;}virtual long __stdcall GetPrivateData(const _GUID&,void*,unsigned long*){testError=3;return -1;}virtual long __stdcall FreePrivateData(const _GUID&){testError=3;return -1;}virtual unsigned long __stdcall SetPriority(unsigned long){testError=3;return 0;}virtual unsigned long __stdcall GetPriority(){testError=3;return 0;}virtual void __stdcall PreLoad(){testError=3;}virtual _D3DRESOURCETYPE __stdcall GetType(){testError=3;return D3DRTYPE_SURFACE;}
 virtual unsigned long __stdcall SetLOD(unsigned long){testError=3;return 0;}virtual unsigned long __stdcall GetLOD(){testError=3;return 0;}virtual unsigned long __stdcall GetLevelCount(){testError=3;return 0;}virtual long __stdcall SetAutoGenFilterType(_D3DTEXTUREFILTERTYPE){testError=3;return -1;}virtual _D3DTEXTUREFILTERTYPE __stdcall GetAutoGenFilterType(){testError=3;return D3DTEXF_NONE;}virtual void __stdcall GenerateMipSubLevels(){testError=3;}
 virtual long __stdcall GetLevelDesc(unsigned int level,_D3DSURFACE_DESC*desc){if(testPhase||level!=testLevel)testError=4;testPhase=1;*desc=testDesc;++testDescCalls;return testDescStatus;}
 virtual long __stdcall GetSurfaceLevel(unsigned int,IDirect3DSurface9**){testError=3;return -1;}
 virtual long __stdcall LockRect(unsigned int level,_D3DLOCKED_RECT*locked,const tagRECT*rect,unsigned long flags){if(testPhase!=3||level!=testLevel||flags!=testFlags||!rect||rect->left||rect->top||rect->right!=(long)testDesc.Width||rect->bottom!=(long)testDesc.Height)testError=5;testPhase=4;locked->Pitch=testPitch;locked->pBits=testMemory;++testLockCalls;return testLockStatus;}
 virtual long __stdcall UnlockRect(unsigned int){testError=3;return -1;}virtual long __stdcall AddDirtyRect(const tagRECT*){testError=3;return -1;}
};
int main(){CTexture texture;TestTexture device;CSystemManager system;CDisplayManager display;CRenderManager renderer;unsigned char memory[16];texture.PD3DTexture=&device;memset(texture.unread,0x7b,sizeof texture.unread);system.PDisplayManager.Data=&display;system.PDisplayManager.Info=0;CRenderManager*rendererPointer=&renderer;memcpy(&display.PRenderManager,&rendererPointer,sizeof rendererPointer);testSystem=&system;testRenderer=&renderer;testDevice=&device;
 const unsigned long sizes[]={0,1,7,0x7fffffffUL,0x80000000UL,0xffffffffUL},flags[]={0,1,16,0x12345678UL,0xffffffffUL},levels[]={0,1,3,0x80000000UL,0xffffffffUL};const long pitches[]={0,1,64,-1,-128},statuses[]={-1,0,1,(long)0x80004005UL};int cases=0;
 for(int width=0;width<6;++width)for(int height=0;height<6;++height)for(int f=0;f<5;++f)for(int pitch=0;pitch<5;++pitch)for(int status=0;status<4;++status)for(int descStatus=0;descStatus<3;++descStatus)for(int level=0;level<5;++level){
  memset(&testDesc,0x71,sizeof testDesc);testDesc.Width=sizes[width];testDesc.Height=sizes[height];testLevel=levels[level];testFlags=flags[f];testPitch=pitches[pitch];testMemory=memory+(f+pitch)%16;testDescStatus=descStatus-1;testLockStatus=statuses[status];testError=testPhase=testDescCalls=testSystemCalls=testWaitCalls=testLockCalls=0;
  // Failure leaves the original result fields unspecified. Exercise forwarding and ordering while discarding that result.
  if(testLockStatus<0)texture.Lock(testLevel,testFlags);else{CSurfaceLockedRect actual=texture.Lock(testLevel,testFlags);if(actual.Dimensions.X!=(long)testDesc.Width||actual.Dimensions.Y!=(long)testDesc.Height||actual.Pitch!=testPitch||actual.PMem!=testMemory)return 1;}
  if(testError||testPhase!=4||testDescCalls!=1||testSystemCalls!=1||testWaitCalls!=1||testLockCalls!=1||texture.PD3DTexture!=&device)return 2;for(int i=0;i<4;++i)if(texture.unread[i]!=0x7b)return 3;++cases;
 }
 if(cases!=54000)return 4;puts("009fa450_TEST PASS");return 0;}