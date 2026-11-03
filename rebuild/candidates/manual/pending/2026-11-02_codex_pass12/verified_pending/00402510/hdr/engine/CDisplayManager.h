#ifdef FABLE_RETAIL_GFMAIN
#ifndef FABLE_ENGINE_CDISPLAYMANAGER_H
#define FABLE_ENGINE_CDISPLAYMANAGER_H
#include "engine/CBaseClassNonCopyable.h"
#ifdef FABLE_RETAIL_GFMAIN
#include "engine/Direct3D9Types.h"
#endif
#include "boost/scoped_ptr.h"
#include "engine/CSurface.h"
#include "engine/CTexture.h"
#include "engine/CPixelFormat.h"
#include "engine/C2DExtentsI.h"
#include "engine/C2DBoxI.h"
#include "engine/C2DBoxF.h"
#include "engine/CWideString.h"
#include "engine/ESurfaceMultisampleType.h"
#include "engine/CArray.h"
#include "engine/CBusyResourceManager.h"
class CSystemManager;class CRenderManager;class CVsyncCallbackTimer;class CDeviceResetCallback;
struct IDirect3D9;struct IDirect3DDevice9;
// Layout: original fields; native STLport list/vector shrink later offsets. Complete native size0x2cc.
class CDisplayManager:public CBaseClassNonCopyable {
public:
 class CPresentationIntervalDesc {public:bool PresentImmediate,PresentImmediateIfSyncMissed;long VSyncCount;};
 struct CCreateDeviceParameters {
  HWND WinHandle;C2DExtentsI BackBufferDimensions;long BackBufferBitDepth,ZBufferBitDepth,BackBufferCount,RefreshRate;
  bool ExclusiveMode;CPresentationIntervalDesc PresentationInterval;ESurfaceMultisampleType MultisampleType;
  CCreateDeviceParameters() {}
 };
 CDisplayManager(CSystemManager&);
#ifdef FABLE_RETAIL_STATBOX
 const C2DExtentsI GetRenderTargetDimensions();
#endif
 virtual ~CDisplayManager();
#ifdef FABLE_RETAIL_GFMAIN
 bool IsTextureBitDepthSupported(unsigned long);
 long GetRefreshRate()const{return RefreshRate;}
#endif
#if defined(FABLE_RETAIL_CENTERED_TEXT) || defined(FABLE_RETAIL_GFMAIN)
 CRenderManager& GetRenderManager() { return *PRenderManager; }
#endif
#if defined(FABLE_RETAIL_FONT_PIXEL_ACCESS)
 bool GetBestAlphaTextureFormat(unsigned long,CPixelFormat&);
#endif
private:
 CSystemManager& SystemManager;
 boost::scoped_ptr<CRenderManager> PRenderManager;
 CSurface RenderTargetSurface,RenderTargetDepthBuffer,BackBufferSurface,DepthBufferSurface;
 CTexture BackBufferCopyTexture;
 IDirect3D9* D3D;IDirect3DDevice9* D3DDevice;D3DCAPS9 D3DDeviceCaps;
 C2DExtentsI BackBufferDimensions,RenderTargetDimensions;
 long NoBackBuffers;bool ExclusiveMode;ESurfaceMultisampleType MultisampleType;
 CPixelFormat BackBufferFormat,DepthBufferFormat;
 bool UsingRenderManager,UsingShaderRenderManager;HWND WindowHandle;D3DDISPLAYMODE CurrentMode;
 long RefreshRate;CPresentationIntervalDesc PresentationInterval;float PresentationIntervalTime;
 bool ViewportOffscreen;D3DVIEWPORT9 D3DViewport;C2DBoxI ViewportBox;C2DBoxF ViewportBoxFloat;
 bool Rendering,CanBeginRender;long CurrentScreenIndex;D3DPRESENT_PARAMETERS PresentParams;
 bool VirtualCoordsResolutionIndependant,DeviceLostFlag;CWideString ShaderResourcePath;
 long FrameCounter;unsigned long RenderManagerInitFlags;std::list<CDeviceResetCallback*> DeviceResetCallbacks;
 CCreateDeviceParameters LastDeviceCreationParameters;boost::scoped_ptr<CVsyncCallbackTimer> VsyncCallbackTimer;
 CArray<CPixelFormat> BackBufferFormats;CArray<ESurfaceMultisampleType> MultisampleTypes;
 CBusyResourceManager BusyResourceManager;float Gamma;bool m_bWantHiResScreenShot,m_bTakingScreenShot;
};
FABLE_STATIC_ASSERT(sizeof(CDisplayManager)==0x2cc);
#endif

#else
#ifndef FABLE_ENGINE_CDISPLAYMANAGER_H
#define FABLE_ENGINE_CDISPLAYMANAGER_H
#include "engine/CBaseClassNonCopyable.h"
#include "boost/scoped_ptr.h"
#include "engine/CSurface.h"
#include "engine/CTexture.h"
#include "engine/CPixelFormat.h"
#include "engine/C2DExtentsI.h"
#include "engine/C2DBoxI.h"
#include "engine/C2DBoxF.h"
#include "engine/CWideString.h"
#include "engine/ESurfaceMultisampleType.h"
#include "engine/CArray.h"
#include "engine/CBusyResourceManager.h"
class CSystemManager;class CRenderManager;class CVsyncCallbackTimer;class CDeviceResetCallback;
struct IDirect3D9;struct IDirect3DDevice9;
// Layout: original fields; native STLport list/vector shrink later offsets. Complete native size0x2cc.
class CDisplayManager:public CBaseClassNonCopyable {
public:
 class CPresentationIntervalDesc {public:bool PresentImmediate,PresentImmediateIfSyncMissed;long VSyncCount;};
 struct CCreateDeviceParameters {
  HWND WinHandle;C2DExtentsI BackBufferDimensions;long BackBufferBitDepth,ZBufferBitDepth,BackBufferCount,RefreshRate;
  bool ExclusiveMode;CPresentationIntervalDesc PresentationInterval;ESurfaceMultisampleType MultisampleType;
  CCreateDeviceParameters() {}
 };
 CDisplayManager(CSystemManager&);
#ifdef FABLE_RETAIL_STATBOX
 const C2DExtentsI GetRenderTargetDimensions();
#endif
 virtual ~CDisplayManager();
#ifdef FABLE_RETAIL_CENTERED_TEXT
 CRenderManager& GetRenderManager() { return *PRenderManager; }
#endif
#if defined(FABLE_RETAIL_FONT_PIXEL_ACCESS)
 bool GetBestAlphaTextureFormat(unsigned long,CPixelFormat&);
#endif
private:
 CSystemManager& SystemManager;
 boost::scoped_ptr<CRenderManager> PRenderManager;
 CSurface RenderTargetSurface,RenderTargetDepthBuffer,BackBufferSurface,DepthBufferSurface;
 CTexture BackBufferCopyTexture;
 IDirect3D9* D3D;IDirect3DDevice9* D3DDevice;D3DCAPS9 D3DDeviceCaps;
 C2DExtentsI BackBufferDimensions,RenderTargetDimensions;
 long NoBackBuffers;bool ExclusiveMode;ESurfaceMultisampleType MultisampleType;
 CPixelFormat BackBufferFormat,DepthBufferFormat;
 bool UsingRenderManager,UsingShaderRenderManager;HWND WindowHandle;D3DDISPLAYMODE CurrentMode;
 long RefreshRate;CPresentationIntervalDesc PresentationInterval;float PresentationIntervalTime;
 bool ViewportOffscreen;D3DVIEWPORT9 D3DViewport;C2DBoxI ViewportBox;C2DBoxF ViewportBoxFloat;
 bool Rendering,CanBeginRender;long CurrentScreenIndex;D3DPRESENT_PARAMETERS PresentParams;
 bool VirtualCoordsResolutionIndependant,DeviceLostFlag;CWideString ShaderResourcePath;
 long FrameCounter;unsigned long RenderManagerInitFlags;std::list<CDeviceResetCallback*> DeviceResetCallbacks;
 CCreateDeviceParameters LastDeviceCreationParameters;boost::scoped_ptr<CVsyncCallbackTimer> VsyncCallbackTimer;
 CArray<CPixelFormat> BackBufferFormats;CArray<ESurfaceMultisampleType> MultisampleTypes;
 CBusyResourceManager BusyResourceManager;float Gamma;bool m_bWantHiResScreenShot,m_bTakingScreenShot;
};
FABLE_STATIC_ASSERT(sizeof(CDisplayManager)==0x2cc);
#endif

#endif
