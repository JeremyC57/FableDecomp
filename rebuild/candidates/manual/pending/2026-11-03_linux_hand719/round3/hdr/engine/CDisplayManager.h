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
#include "fable_string.h"
#include "engine/ESurfaceMultisampleType.h"
#include "engine/CArray.h"
#include "engine/CBusyResourceManager.h"
class CSystemManager;class CRenderManager;class CVsyncCallbackTimer;class CDeviceResetCallback;
#ifdef FABLE_RETAIL_CDISPLAYMANAGER
class CRGBColour;
class CDisplayModeDesc;
#endif
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
 virtual ~CDisplayManager();
 bool GetBestAlphaTextureFormat(unsigned long,CPixelFormat&);
#ifdef FABLE_RETAIL_CDISPLAYMANAGER
 // Retail opt-in (movie row 0x006286F0; needs FABLE_RETAIL_SCOPED_PTR for GetRenderManager). Spellings: Ego_r.pdb + FableWin.pdb.
 void ClearRenderTarget(const CRGBColour&,unsigned long);               // 0x009BE420 ?ClearRenderTarget@CDisplayManager@@QAEXABVCRGBColour@@K@Z
 bool BeginRender();                                                    // 0x009BEF20 ?BeginRender@CDisplayManager@@QAE_NXZ
 void EndRender();                                                      // 0x009BEF50 ?EndRender@CDisplayManager@@QAEXXZ
 bool SwapScreens();                                                    // 0x009BEEB0 ?SwapScreens@CDisplayManager@@QAE_NXZ
 const C2DExtentsI GetRenderTargetDimensions();                         // 0x009BEDC0 ?GetRenderTargetDimensions@CDisplayManager@@QAE?BVC2DExtentsI@@XZ
 // FableWin.pdb ?GetRenderManager@CDisplayManager@@QAEAAVCRenderManager@@XZ (debug only: inline in release; 0x006286F0 reads
 // PRenderManager +0x08 in place).
 CRenderManager& GetRenderManager(){return *PRenderManager;}
 // Retail opt-in additions (2026-11-03, chunk 719 row 0x0040CEC0 the CUserProfileManager ctor): Ego_r.pdb
 // ?EnumerateDisplayModes@CDisplayManager@@QAEXAAV?$CArray@VCDisplayModeDesc@@@@VC2DExtentsI@@1@Z (retail 0x009C02D0) and
 // ?AddDeviceResetCallback@CDisplayManager@@QAEXPAVCDeviceResetCallback@@@Z (retail 0x009C0190).
 void EnumerateDisplayModes(CArray<CDisplayModeDesc>& modes,C2DExtentsI minDimensions,C2DExtentsI maxDimensions);
 void AddDeviceResetCallback(CDeviceResetCallback* callback);
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
