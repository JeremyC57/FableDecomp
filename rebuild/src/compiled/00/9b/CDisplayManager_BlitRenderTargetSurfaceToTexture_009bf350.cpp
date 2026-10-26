// CDisplayManager::BlitRenderTargetSurfaceToTexture(CTexture*,CSurface*) -- suspend scene, copy render target, retry compatible surfaces, resume
// Retail009BF350 310bytes. CDisplayManager donor Rendering+214,CanBeginRender+215,D3DDevice+58 agree.
// Actual CSurface,CTexture,CPixelFormat,CRenderManagerCore headers retained. Native D3D9 stdcall slots34 StretchRect,41 BeginScene,42 EndScene.
// Texture GetSurfaceLevel009F9E00 returns an owning CSurface; vtable0122F84C and release009F2E20 on scope exit.
// Leaf: BlitRenderTargetSurfaceToTexture
#include "engine/CSurface.h"
#include "engine/CTexture.h"
#include "engine/CPixelFormat.h"
#include "engine/CRenderManagerCore.h"
struct CPixelFormat_Methods : CPixelFormat {
 CPixelFormat_Methods(){} bool IsEquivalent(const CPixelFormat_Methods&)const; //009E37F0
};
inline bool operator==(const CPixelFormat_Methods& a,const CPixelFormat_Methods& b){return a.IsEquivalent(b);}
class C2DExtentsI { public: long X,Y;C2DExtentsI(){} long GetX()const{return X;}long GetY()const{return Y;} };
class C2DBoxI;
extern void* g_CSurfaceVTable[]; //0122F84C
struct CSurface_Methods : CSurface {
 CSurface_Methods(){} CSurface_Methods(const CSurface_Methods&); //009F2D60
 ~CSurface_Methods(){__vftable=g_CSurfaceVTable;Release();}
 void Release(); //009F2E20
 CPixelFormat_Methods GetPixelFormat()const; //009F2F60
 C2DExtentsI GetDimensions()const; //009F2F90
 void CopyFromSurface(const C2DBoxI*,const CSurface_Methods&,const C2DBoxI*,unsigned int); //009F37E0
};
struct CTexture_Methods : CTexture { CSurface_Methods GetSurfaceLevel(long)const; }; //009F9E00
struct CRenderManagerCore_Methods : CRenderManagerCore {
 void EndRender(); //00A0B520
 void BeginRender(); //00A0B480
};
struct BlitD3DDevice {
 void* VTable;
 typedef long (__stdcall BlitD3DDevice::*SceneMethod)();
 typedef long (__stdcall BlitD3DDevice::*StretchMethod)(IDirect3DSurface9*,const void*,IDirect3DSurface9*,const void*,long);
 long BeginScene(){return (this->*static_cast<SceneMethod*>(VTable)[41])();}
 long EndScene(){return (this->*static_cast<SceneMethod*>(VTable)[42])();}
 long StretchRect(IDirect3DSurface9* from,const void* a,IDirect3DSurface9* to,const void* b,long filter){return(this->*static_cast<StretchMethod*>(VTable)[34])(from,a,to,b,filter);}
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(BlitD3DDevice::SceneMethod)==4);
FABLE_STATIC_ASSERT(sizeof(BlitD3DDevice::StretchMethod)==4);
#endif
extern void* g_DisplayRenderingEnabled; //013CA810 native nullable render gate
class CDisplayManager { public:
 void* VTable;void* SystemManager;CRenderManagerCore_Methods* PRenderManager;
 unsigned char SurfacesAndD3D[0x58-0xc];BlitD3DDevice* D3DDevice;
 unsigned char DeviceState[0x214-0x5c];bool Rendering,CanBeginRender;
 void BlitRenderTargetSurfaceToTexture(CTexture_Methods*,CSurface_Methods*);
};
void CDisplayManager::BlitRenderTargetSurfaceToTexture(CTexture_Methods* texture,CSurface_Methods* source){
 bool rendering=Rendering;
 if(rendering&&g_DisplayRenderingEnabled){PRenderManager->EndRender();if(D3DDevice->EndScene()>=0){Rendering=false;CanBeginRender=false;}}
 CSurface_Methods destination=texture->GetSurfaceLevel(0);
 if(D3DDevice->StretchRect(source->PD3DSurface,0,destination.PD3DSurface,0,0)<0){
  if(source->GetPixelFormat()==destination.GetPixelFormat()&&
     source->GetDimensions().GetX()==destination.GetDimensions().GetX()&&
     source->GetDimensions().GetY()==destination.GetDimensions().GetY())
   destination.CopyFromSurface(0,*source,0,~0u);
 }
 if(rendering&&g_DisplayRenderingEnabled){if(D3DDevice->BeginScene()>=0){PRenderManager->BeginRender();Rendering=true;}}
}