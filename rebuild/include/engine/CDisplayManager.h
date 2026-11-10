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
#ifdef FABLE_RETAIL_CDISPLAYMANAGER_ANTIALIASING
 // Opt-in (chunk 880, row 0x0058AF10 CTCInventory::OpenPCOptionsMenu): Ego_r.pdb
 // ?EnumerateAntiAliasingModes@CDisplayManager@@QAEXAAV?$CArray@W4ESurfaceMultisampleType@@@@@Z, landed at 0x009C05C0.
 void EnumerateAntiAliasingModes(CArray<ESurfaceMultisampleType>& types);   // 0x009C05C0
#endif
#ifdef FABLE_RETAIL_CDISPLAYMANAGER
 // Retail opt-in (movie row 0x006286F0; needs FABLE_RETAIL_SCOPED_PTR for GetRenderManager). Spellings: Ego_r.pdb + FableWin.pdb.
 void ClearRenderTarget(const CRGBColour&,unsigned long);               // 0x009BE420 ?ClearRenderTarget@CDisplayManager@@QAEXABVCRGBColour@@K@Z
 bool BeginRender();                                                    // 0x009BEF20 ?BeginRender@CDisplayManager@@QAE_NXZ
 void EndRender();                                                      // 0x009BEF50 ?EndRender@CDisplayManager@@QAEXXZ
 bool SwapScreens();                                                    // 0x009BEEB0 ?SwapScreens@CDisplayManager@@QAE_NXZ
 // Chunk 774 (bake 0x0042DF9E CNewFrontendGameComponent::Draw): Ego_r.pdb ?GetVVisibleTelevisionScreenArea@CDisplayManager@@QBE?AVC2DBoxF@@XZ
 // (Draw calls it with a hidden C2DBoxF result and drops the value). Declaration only.
 C2DBoxF GetVVisibleTelevisionScreenArea() const;                       // 0x009BECE0
#ifdef FABLE_RETAIL_CDISPLAYMANAGER_SETVIEWPORT
 // Nested opt-in (asm round 2b): Ego_r.pdb 0x00B23C90 / FableWin.pdb 0x0301F590 ?SetViewport@CDisplayManager@@QAEXABVC2DBoxI@@@Z.
 void SetViewport(const C2DBoxI& viewport);                             // 0x009BEF80
#endif
 #ifdef FABLE_RETAIL_CDISPLAYMANAGER_TEXTUREFORMATS
 // Retail opt-in (chunk 792, row 0x00AB8E10 CStaticFontBank::CStaticFontBank): Ego_r.pdb / FableWin.pdb
 // ?GetBestCompressedAlphaTextureFormat@CDisplayManager@@QAE_NKAAVCPixelFormat@@@Z (landed 0x009BE870 under this spelling).
 bool GetBestCompressedAlphaTextureFormat(unsigned long bitDepth, class CPixelFormat& format);   // 0x009BE870
 #endif
 #ifdef FABLE_RETAIL_CDISPLAYMANAGER_BACKSCREEN
 // Retail opt-in (chunk 792, row 0x00B5A3F0 CEngineScreenEffectColourFilter::ApplyColourFilter): Ego_r.pdb / FableWin.pdb
 // ?CopyBackBufferToTexture@CDisplayManager@@QAEAAVCTexture@@XZ (landed 0x009BF5E0; the caller keeps the reference and later
 // passes its address to AttachTextureToStage) and ?SetRenderTargetAsBackScreen@CDisplayManager@@QAE_NXZ (landed 0x009BF220;
 // result dropped). Declarations only.
 CTexture& CopyBackBufferToTexture();                                   // 0x009BF5E0
 bool SetRenderTargetAsBackScreen();                                    // 0x009BF220
 #endif
 const C2DExtentsI GetRenderTargetDimensions();                         // 0x009BEDC0 ?GetRenderTargetDimensions@CDisplayManager@@QAE?BVC2DExtentsI@@XZ
 void AddDeviceResetCallback(CDeviceResetCallback* callback);          // 0x009C0190 ?AddDeviceResetCallback@CDisplayManager@@QAEXPAVCDeviceResetCallback@@@Z (CEngine::Initialise)
 unsigned long GetVertexShaderVersion() const;                         // 0x009BEDE0 ?GetVertexShaderVersion@CDisplayManager@@QBEKXZ (Ego_r 0x00B23A60, FableWin)
 // FableWin.pdb ?GetRenderManager@CDisplayManager@@QAEAAVCRenderManager@@XZ (debug only: inline in release; 0x006286F0 reads
 // PRenderManager +0x08 in place).
 CRenderManager& GetRenderManager(){return *PRenderManager;}
#ifdef FABLE_RETAIL_CDISPLAYMANAGER_FRONTEND
 // Retail opt-in (chunk 858, row 0x00494900 CFrontendGameComponent::Output): FableWin.pdb ?GetBackBuffer@CDisplayManager@@QAEAAVCSurface@@XZ
 // (debug only, inline in release: Output adds +0x2C, BackBufferSurface, to the manager in place) and Ego_r.pdb 0x00B23700
 // ?GetBestCompressedOpaqueTextureFormat@CDisplayManager@@QAE_NKAAVCPixelFormat@@@Z (retail 0x009BE830, the first of the
 // landed 0x009BE830 / 0x009BE870 / 0x009BE8B0 run, as Ego_r 0x00B23700 / 0x00B23750 Alpha / 0x00B237A0 BooleanAlpha).
 CSurface& GetBackBuffer(){return BackBufferSurface;}
 bool GetBestCompressedOpaqueTextureFormat(unsigned long bitDepth, class CPixelFormat& format);   // 0x009BE830
#endif
 // ?GetD3DDevice@CDisplayManager@@QAEPAUIDirect3DDevice9@@XZ (FableWin 0x030C99A0, debug only: inline in release; Codex pass 13
 // row 0x009DA9F0 CRenderManager2D::Render2DDrawList reads D3DDevice in place for DrawPrimitiveUP).
 IDirect3DDevice9* GetD3DDevice(){return D3DDevice;}
#ifdef FABLE_RETAIL_CDISPLAYMANAGER_GETVIEWPORT
 // Retail opt-in (chunk 783, row 0x009FA780): FableWin.pdb ?GetViewport@CDisplayManager@@QBEABVC2DBoxI@@XZ (debug only, inline in
 // release): 0x009FA7FE..0x009FA828 copies ViewportBox (+0x1F4, Ego_r.pdb offset 500) field by field in place.
 const C2DBoxI& GetViewport() const{return ViewportBox;}
#endif
#endif
private:
#ifdef FABLE_RETAIL_CDISPLAYMANAGER
#ifdef FABLE_RETAIL_CDISPLAYMANAGER_BUFFERTEXTURES
public:
 // Retail opt-in (Codex pass 17): original debug0301ce50/70 and original caller graph.
 CSurface& GetRenderTargetSurface(){return RenderTargetSurface;}
 bool IsNeedingPower2Textures(){return (D3DDeviceCaps.TextureCaps>>1)&1;}
private:
#endif
 bool SetupDepthAndBackBufferTextures();                                // 0x009BEE40 ?SetupDepthAndBackBufferTextures@CDisplayManager@@AAE_NXZ (chunk 751)
#endif
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
