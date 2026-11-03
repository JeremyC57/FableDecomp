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
class CRenderTarget;class CDisplayModeDesc;class CDisplayManagerInit;class CRGBColour;struct HWND__;
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
#ifndef FABLE_RETAIL_RENDER2D_TYPES
  CCreateDeviceParameters() {}
#endif
 };
 CDisplayManager(CSystemManager&);
#ifdef FABLE_RETAIL_RENDER2D_TYPES
 IDirect3DDevice9* GetD3DDevice() {return D3DDevice;}
#endif
 virtual ~CDisplayManager();
 bool GetBestAlphaTextureFormat(unsigned long,CPixelFormat&);
#if defined(FABLE_RETAIL_CDISPLAYMANAGER) && !defined(FABLE_RETAIL_RENDER2D_TYPES)
 // Retail opt-in (movie row 0x006286F0; needs FABLE_RETAIL_SCOPED_PTR for GetRenderManager). Spellings: Ego_r.pdb + FableWin.pdb.
 void ClearRenderTarget(const CRGBColour&,unsigned long);               // 0x009BE420 ?ClearRenderTarget@CDisplayManager@@QAEXABVCRGBColour@@K@Z
 bool BeginRender();                                                    // 0x009BEF20 ?BeginRender@CDisplayManager@@QAE_NXZ
 void EndRender();                                                      // 0x009BEF50 ?EndRender@CDisplayManager@@QAEXXZ
 bool SwapScreens();                                                    // 0x009BEEB0 ?SwapScreens@CDisplayManager@@QAE_NXZ
 const C2DExtentsI GetRenderTargetDimensions();                         // 0x009BEDC0 ?GetRenderTargetDimensions@CDisplayManager@@QAE?BVC2DExtentsI@@XZ
 // FableWin.pdb ?GetRenderManager@CDisplayManager@@QAEAAVCRenderManager@@XZ (debug only: inline in release; 0x006286F0 reads
 // PRenderManager +0x08 in place).
 CRenderManager& GetRenderManager(){return *PRenderManager;}
#endif
#ifdef FABLE_RETAIL_RENDER2D_TYPES
public: enum ECreateDeviceSuccess { CREATE_DEVICE_FAILED=0,CREATE_DEVICE_DEVICE_LOST=1,CREATE_DEVICE_SUCCESS=2 };
public: bool BeginRender();
public: void EndRender();
public: bool IsRendering() const;
public: CSurface & GetRenderTargetSurface();
public: CSurface & GetBackBuffer();
public: CSurface & GetDepthBuffer();
public: CRenderTarget GetRenderTarget();
public: CTexture & CopyBackBufferToTexture();
public: void BlitRenderTargetSurfaceToTexture(CTexture &,const CSurface &);
public: const CPixelFormat & GetDepthBufferFormat();
public: void EnumerateDisplayModes(CArray<CDisplayModeDesc> &,C2DExtentsI,C2DExtentsI);
public: void EnumerateAntiAliasingModes(CArray<enum ESurfaceMultisampleType> &);
public: bool IsAntiAliasingModeValid(const ESurfaceMultisampleType,const CPixelFormat &);
public: ESurfaceMultisampleType PeekMultiSampleType();
public: CDisplayManager::ECreateDeviceSuccess SetDisplayMode(const C2DExtentsI &,unsigned long,ESurfaceMultisampleType,unsigned long);
public: void FlushVideoMemeory();
public: long GetAvailableTextureMemory();
public: void SetViewport(const C2DBoxF &);
public: void SetViewport(const C2DBoxI &);
public: const C2DBoxI & GetViewport() const;
public: const C2DBoxF & GetViewportFloat() const;
public: bool SetRenderTarget(CRenderTarget &);
public: bool SetRenderTargetAsBackScreen();
public: const C2DExtentsI GetRenderTargetDimensions();
public: void ClearRenderTarget(const CRGBColour &,unsigned long);
public: bool SwapScreens();
public: bool ProcessLostDevice();
public: long GetFrameCounter() const;
public: bool IsExclusive() const;
public: bool IsDeviceLost() const;
public: CRenderManager & GetRenderManager();
public: long GetRefreshRate() const;
public: long GetBitDepth() const;
public: float GetPresentationIntervalTime() const;
public: void SetGamma(float,bool);
public: float GetGamma() const;
public: bool SetClientWindow(HWND__ *);
public: unsigned long GetBackBufferCount() const;
public: void ChangePresentationInterval(bool,long,bool);
public: void ChangePresentationInterval(const CDisplayManager::CPresentationIntervalDesc &);
public: CDisplayManager::CPresentationIntervalDesc GetPresentationInterval() const;
public: void ChangePresentationIntervalLockToGivenFrameRateOrLower(float,bool);
public: CVsyncCallbackTimer & GetVsyncCallbackTimer();
public: bool GetBestSignedTextureFormat(unsigned long,CPixelFormat &);
public: bool IsTextureBitDepthSupported(unsigned long);
public: bool GetBestOpaqueTextureFormat(unsigned long,CPixelFormat &,bool);
public: bool GetBestBooleanAlphaTextureFormat(unsigned long,CPixelFormat &);
public: bool IsSupportingCompressedTextures();
public: bool GetBestCompressedOpaqueTextureFormat(unsigned long,CPixelFormat &);
public: bool GetBestCompressedAlphaTextureFormat(unsigned long,CPixelFormat &);
public: bool GetBestCompressedBooleanAlphaTextureFormat(unsigned long,CPixelFormat &);
public: bool IsValidTextureFormat(const CPixelFormat &);
public: bool IsValidRenderTargetFormat(const CPixelFormat &,const CPixelFormat &);
public: bool IsNeedingPower2Textures();
public: bool IsViewportOffscreen() const;
public: unsigned long GetVertexShaderVersion() const;
public: unsigned long GetPixelShaderVersion() const;
public: C2DBoxF GetAVisibleTelevisionScreenArea() const;
public: C2DBoxF GetVVisibleTelevisionScreenArea() const;
public: void SetVirtualCoordsAsResolutionIndependant(bool);
public: bool IsVirtualCoordsResolutionIndependant() const;
public: void FlipToGDISurface();
public: void AddDeviceResetCallback(CDeviceResetCallback *);
public: void RemoveDeviceResetCallback(CDeviceResetCallback *);
public: float ScaleForAntialiasing(float);
public: const C2DExtentsI & PeekBackBufferResolution() const;
public: long AllocTile(CSurface &,bool);
public: void ReleaseTile(long,CSurface &);
public: void ResizeWindowToBackBufferSize();
public: void SetWantHiResScreenShot(bool);
public: void SetTakingScreenShot(bool);
public: bool WantsHiResScreenShot() const;
public: bool IsTakingScreenShot() const;
public: CBusyResourceManager & GetBusyResourceManager();
public: void Init(const CDisplayManagerInit &,bool);
public: CDisplayManager::ECreateDeviceSuccess SetExclusiveMode(bool);
private: CDisplayManager::ECreateDeviceSuccess CreateDevice(const CDisplayManager::CCreateDeviceParameters &);
private: CDisplayManager::ECreateDeviceSuccess ResetDevice(const CDisplayManager::CCreateDeviceParameters &);
private: void DestroyDevice();
private: void InitVirtualPixels();
private: CPixelFormat GetClosestDepthBufferFormat(const _D3DDISPLAYMODE &,long) const;
private: _D3DDISPLAYMODE GetClosestMode(const C2DExtentsI &,long,bool) const;
private: void PreDeviceReset();
private: bool PostDeviceReset();
private: void GetCurrentWindowedSettings(HWND__ *,C2DExtentsI &,long &);
private: bool SetupDepthAndBackBufferTextures();
private: float CalcPresentationIntervalTime(long,long);
private: unsigned int CalculateD3DPresentationInterval(const CDisplayManager::CPresentationIntervalDesc &);
private: long ReadDeviceRefreshRate();
private: void ProcessScreenshotCapture();
private: void ProcessBusyResourceManagement();
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
