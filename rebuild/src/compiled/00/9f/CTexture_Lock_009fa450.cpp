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
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CBaseClassNonCopyable:public CBaseClass{protected:virtual~CBaseClassNonCopyable();};class CInitBaseClass:public CBaseClass{public:bool Valid;};
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