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
#ifdef FABLE_RETAIL_CDISPLAYMANAGER_GETRENDERTARGET
#include "engine/CRenderTarget.h"
#endif
class CSystemManager;class CRenderManager;class CVsyncCallbackTimer;class CDeviceResetCallback;
#ifdef FABLE_RETAIL_CDISPLAYMANAGER
class CRGBColour;
#endif
struct IDirect3D9;struct IDirect3DDevice9;
// Layout: original fields; native STLport list/vector shrink later offsets. Complete native size0x2cc.
class CDisplayManager:public CBaseClassNonCopyable {
#ifdef FABLE_RETAIL_SHADOW_BUFFERS_PASS27
public:
 bool IsValidTextureFormat(const CPixelFormat&);
 bool IsValidRenderTargetFormat(const CPixelFormat&,const CPixelFormat&);
#endif
public:
 class CPresentationIntervalDesc {public:bool PresentImmediate,PresentImmediateIfSyncMissed;long VSyncCount;};
 struct CCreateDeviceParameters {
  HWND WinHandle;C2DExtentsI BackBufferDimensions;long BackBufferBitDepth,ZBufferBitDepth,BackBufferCount,RefreshRate;
  bool ExclusiveMode;CPresentationIntervalDesc PresentationInterval;ESurfaceMultisampleType MultisampleType;
  CCreateDeviceParameters() {}
 };
#ifdef FABLE_RETAIL_CDISPLAYMANAGER_CREATEDEVICE
 // Opt-in (hand3c2, row 0x009BF7E0 CDisplayManager::CreateDevice): Ego_d.pdb LF_ENUM 0xB3D6 CDisplayManager::ECreateDeviceSuccess (the
 // result type of CreateDevice / ResetDevice / SetDisplayMode / SetExclusiveMode).
 enum ECreateDeviceSuccess {CREATE_DEVICE_FAILED=0,CREATE_DEVICE_DEVICE_LOST=1,CREATE_DEVICE_SUCCESS=2};
 // Ego_r.pdb / FableWin.pdb ?IsAntiAliasingModeValid@CDisplayManager@@QAE_NW4ESurfaceMultisampleType@@ABVCPixelFormat@@@Z (retail
 // 0x009BE3C0, not catalogued yet; Ego_d parameters multisampling_mode, pixel_format). Declaration only: it is a function of the
 // display manager's own .cpp (Ego_d file id 600, lines 956..959), which the CreateDevice row defines as a same-TU copy.
 bool IsAntiAliasingModeValid(ESurfaceMultisampleType multisampling_mode,const CPixelFormat& pixel_format);   // 0x009BE3C0
#endif
#ifdef FABLE_RETAIL_CDISPLAYMANAGER_RESETDEVICE
 // NF145 opt-in (q11808, rows 0x009BFE70 / 0x009BFF10 / 0x009BFF40 / 0x009C0020, the ResetDevice callers; needs _CREATEDEVICE for
 // ECreateDeviceSuccess). Spellings from Ego_r.pdb / FableWin.pdb (Ego_r 0x00B24F00 / 0x00B24FA0 / 0x00B25050 / 0x00B25130). Declarations only.
 void ChangePresentationInterval(const CPresentationIntervalDesc& interval);                        // 0x009BFE70
 void ChangePresentationInterval(bool present_immediate,long vsync_count,bool present_immediate_if_sync_missed);   // 0x009BFF10
 ECreateDeviceSuccess SetDisplayMode(const C2DExtentsI& size,unsigned long bit_depth,ESurfaceMultisampleType multisample_type,unsigned long refresh_rate);   // 0x009BFF40
 ECreateDeviceSuccess SetExclusiveMode(bool exclusive);                                            // 0x009C0020
#endif
 CDisplayManager(CSystemManager&);
#ifdef FABLE_RETAIL_GLYPH_LOOKUP_PASS29
 long GetFrameCounter() const { return FrameCounter; }
#endif
 virtual ~CDisplayManager();
 bool GetBestAlphaTextureFormat(unsigned long,CPixelFormat&);
#ifdef FABLE_RETAIL_CDISPLAYMANAGER_ISVIEWPORTOFFSCREEN
 // Retail opt-in (agentNF41, row 0x009DB700 CRenderManager2D::AddText): FableWin.pdb ?IsViewportOffscreen@CDisplayManager@@QBE_NXZ
 // (0x02D46420; absent from Ego_r = inline in release: AddText reads +0x1D8 in place, 0x009DB70E). No layout change.
 bool IsViewportOffscreen() const { return ViewportOffscreen; }
#endif
#ifdef FABLE_RETAIL_CDISPLAYMANAGER_GETVIEWPORTFLOAT
 // Retail opt-in (agentNF41, row 0x00A0AAA0 CRenderManagerCore::GetCurrentAWindow): FableWin.pdb ?GetViewportFloat@CDisplayManager@@QBEABVC2DBoxF@@XZ
 // (absent from Ego_r = inline in release: `add eax,204h`, the address of ViewportBoxFloat +0x204). No layout change.
 const C2DBoxF& GetViewportFloat() const { return ViewportBoxFloat; }
#endif
#ifdef FABLE_RETAIL_CDISPLAYMANAGER_SETVIEWPORT_BOXF
 // NF56 opt-in (row 0x00A0AA80 CRenderManagerCore::SetAWindow(const C2DBoxF&), which tail-jumps here on DisplayManager +0x3A3C):
 // Ego_r.pdb 0x00B24470 / FableWin.pdb 0x0301F510 ?SetViewport@CDisplayManager@@QAEXABVC2DBoxF@@@Z = retail 0x009BF490. Declaration only.
 void SetViewport(const C2DBoxF& viewport);                             // 0x009BF490
#endif
#ifdef FABLE_RETAIL_CDISPLAYMANAGER_VERTEXSHADERVERSION
 // Opt-in (wfE7, row 0x00B33B50): Ego_r.pdb / FableWin.pdb ?GetVertexShaderVersion@CDisplayManager@@QBEKXZ (retail 0x009BEDE0, landed). Declaration only.
 unsigned long GetVertexShaderVersion() const;                         // 0x009BEDE0
#endif
#ifdef FABLE_RETAIL_CDISPLAYMANAGER_PIXELSHADERVERSION
 // Opt-in (row 0x00BB5040 CEngineClothRenderer ctor): Ego_r.pdb / FableWin.pdb ?GetPixelShaderVersion@CDisplayManager@@QBEKXZ
 // (landed 0x009BEE00; the ctor compares it unsigned against 1400 = ps_1_4).
 unsigned long GetPixelShaderVersion() const;                          // 0x009BEE00
#endif
#ifdef FABLE_RETAIL_CDISPLAYMANAGER_ALLTEXTUREFORMATS
 // Retail opt-in (chunk 2018, row 0x00B2CB40): the rest of the Ego_r.pdb / FableWin.pdb GetBest*TextureFormat run, all
 // QAE_NKAAVCPixelFormat@@ (Opaque with a trailing bool), retail in Ego_r donor order 0x009BE590 Signed, 0x009BE610 Opaque,
 // (0x009BE6C0 Alpha above), 0x009BE750 BooleanAlpha, 0x009BE830 CompressedOpaque, 0x009BE870 CompressedAlpha,
 // 0x009BE8B0 CompressedBooleanAlpha (Ego_r 0x00B23400 / 0x00B234A0 / 0x00B23560 / 0x00B23610 / 0x00B23700 / 0x00B23750 /
 // 0x00B237A0). Declarations only.
 bool GetBestSignedTextureFormat(unsigned long bitDepth, CPixelFormat& format);                    // 0x009BE590
 bool GetBestOpaqueTextureFormat(unsigned long bitDepth, CPixelFormat& format, bool allow_alpha);  // 0x009BE610
 bool GetBestBooleanAlphaTextureFormat(unsigned long bitDepth, CPixelFormat& format);              // 0x009BE750
#if !defined(FABLE_RETAIL_CDISPLAYMANAGER) || !defined(FABLE_RETAIL_CDISPLAYMANAGER_FRONTEND)
 bool GetBestCompressedOpaqueTextureFormat(unsigned long bitDepth, CPixelFormat& format);          // 0x009BE830
#endif
#if !defined(FABLE_RETAIL_CDISPLAYMANAGER) || !defined(FABLE_RETAIL_CDISPLAYMANAGER_TEXTUREFORMATS)
 bool GetBestCompressedAlphaTextureFormat(unsigned long bitDepth, CPixelFormat& format);           // 0x009BE870
#endif
 bool GetBestCompressedBooleanAlphaTextureFormat(unsigned long bitDepth, CPixelFormat& format);    // 0x009BE8B0
#endif
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
#ifdef FABLE_RETAIL_CDISPLAYMANAGER_SETGAMMA
 // Opt-in (wfA2, row 0x00435530 CDisplayEngine::DoRender): Ego_r.pdb ?SetGamma@CDisplayManager@@QAEXM_N@Z, out of line in retail at 0x009BEAA0
 // (DoRender 0x00435F3A: `push 0 ; push gamma ; mov ecx,GPDisplayManager`). Parameter names descriptive. Declaration only.
 void SetGamma(float gamma, bool flag);                                 // 0x009BEAA0
#endif
 bool SwapScreens();                                                    // 0x009BEEB0 ?SwapScreens@CDisplayManager@@QAE_NXZ
#ifdef FABLE_RETAIL_CDISPLAYMANAGER_BLITRENDERTARGETSURFACETOTEXTURE
 // NF98 opt-in (round 8, q11274, row 0x009BF350 itself): Ego_r.pdb 0x00B24330 / FableWin.pdb 0x0301CED0
 // ?BlitRenderTargetSurfaceToTexture@CDisplayManager@@QAEXAAVCTexture@@ABVCSurface@@@Z (public; `ret 8`). Declaration only.
 void BlitRenderTargetSurfaceToTexture(CTexture& texture, const CSurface& source);   // 0x009BF350
#endif
#ifdef FABLE_RETAIL_CDISPLAYMANAGER_RENDERTARGET
 // Retail opt-in (wf20 row 0x00F39730 CSurfaceCompositor::StartComposition): Ego_r.pdb / FableWin.pdb
 // ?SetRenderTarget@CDisplayManager@@QAE_NAAVCRenderTarget@@@Z (landed 0x009BF160) and FableWin-only ?IsRendering@CDisplayManager@@QBE_NXZ
 // (inline in release: 0x00F397A9 reads Rendering +0x214 in place).
 bool SetRenderTarget(class CRenderTarget& target);                    // 0x009BF160
 bool IsRendering() const { return Rendering; }
#endif
 // Chunk 774 (bake 0x0042DF9E CNewFrontendGameComponent::Draw): Ego_r.pdb ?GetVVisibleTelevisionScreenArea@CDisplayManager@@QBE?AVC2DBoxF@@XZ
 // (Draw calls it with a hidden C2DBoxF result and drops the value). Declaration only.
 C2DBoxF GetVVisibleTelevisionScreenArea() const;                       // 0x009BECE0
#ifdef FABLE_RETAIL_CDISPLAYMANAGER_VIDEODEFAULTS
 // Opt-in (wf21, row 0x004093E0 CUserProfileManager::SetDefaultValuesVideo): FableWin.pdb ?GetBitDepth@CDisplayManager@@QBEJXZ (debug 25 bytes:
 // BackBufferFormat.GetColourDepth()) and ?GetRefreshRate@CDisplayManager@@QBEJXZ (debug 20 bytes: the RefreshRate member), inline in retail
 // (the row calls CPixelFormat::GetColourDepth 0x009E3820 on [dm+1A8h] and reads [dm+1C8h] in place). Needs FABLE_RETAIL_CPIXELFORMAT_COLOURDEPTH.
 long GetBitDepth() const { return BackBufferFormat.GetColourDepth(); }
 long GetRefreshRate() const { return RefreshRate; }
#endif
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
 #ifdef FABLE_RETAIL_CDISPLAYMANAGER_SETRENDERTARGET
 // Retail opt-in (chunk 2008, row 0x00B665E0 CEngineSkyRenderer::GetEnvironmentMap): Ego_r.pdb 0x00B23F00
 // ?SetRenderTarget@CDisplayManager@@QAE_NAAVCRenderTarget@@@Z (retail 0x009BF160: `push &target ; call`, result dropped) and
 // ?SetRenderTargetAsBackScreen@CDisplayManager@@QAE_NXZ (0x009BF220, as the BACKSCREEN opt-in). Declarations only.
 bool SetRenderTarget(class CRenderTarget& target);                      // 0x009BF160
 #ifndef FABLE_RETAIL_CDISPLAYMANAGER_BACKSCREEN
 bool SetRenderTargetAsBackScreen();                                    // 0x009BF220
 #endif
 #endif
 #ifdef FABLE_RETAIL_CDISPLAYMANAGER_GETRENDERTARGET
 // Retail opt-in (row 0x00B89C30 ApplyOutline): FableWin.pdb ?GetRenderTarget@CDisplayManager@@QAE?AVCRenderTarget@@XZ (debug only, inline
 // in release: 0x00B89CA5 calls ??0CRenderTarget@@QAE@AAVCSurface@@0@Z 0x00A10DC0 on RenderTargetSurface +0xC / RenderTargetDepthBuffer +0x1C).
 CRenderTarget GetRenderTarget(){return CRenderTarget(RenderTargetSurface,RenderTargetDepthBuffer);}
 #endif
 #ifdef FABLE_RETAIL_CDISPLAYMANAGER_PEEKBACKBUFFERRESOLUTION
 // Nested opt-in (chunk 2030, row 0x00BEA110 CEngineWaterSettings::UpdateConstants): FableWin.pdb
 // ?PeekBackBufferResolution@CDisplayManager@@QBEABVC2DExtentsI@@XZ (debug only, inline in release: 0x00BEA52C..0x00BEA54B copies
 // BackBufferDimensions +0x18C / +0x190 into a local, as the landed 0x00B5A160 UpdateUVs does).
 const C2DExtentsI& PeekBackBufferResolution() const { return BackBufferDimensions; }
 #endif
 const C2DExtentsI GetRenderTargetDimensions();                         // 0x009BEDC0 ?GetRenderTargetDimensions@CDisplayManager@@QAE?BVC2DExtentsI@@XZ
#ifdef FABLE_RETAIL_CDISPLAYMANAGER_ENUMERATE_MODES_PASS26
 void EnumerateDisplayModes(CArray<CDisplayModeDesc>& modes, C2DExtentsI min_size, C2DExtentsI max_size);
#endif
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
#ifdef FABLE_RETAIL_CDISPLAYMANAGER_CREATEDEVICE
 // (same opt-in) the private members the row calls or defines, spellings from Ego_r.pdb / FableWin.pdb (parameter names Ego_d.pdb):
 // ?CreateDevice@CDisplayManager@@AAE?AW4ECreateDeviceSuccess@1@ABUCCreateDeviceParameters@1@@Z (retail 0x009BF7E0),
 // ?GetClosestMode@CDisplayManager@@ABE?AU_D3DDISPLAYMODE@@ABVC2DExtentsI@@J_N@Z (landed 0x009BF600),
 // ?GetClosestDepthBufferFormat@CDisplayManager@@ABE?AVCPixelFormat@@ABU_D3DDISPLAYMODE@@J@Z (landed 0x009BEBB0),
 // ?CalculateD3DPresentationInterval@CDisplayManager@@AAEIABVCPresentationIntervalDesc@1@@Z (landed 0x009BE2D0),
 // ?ReadDeviceRefreshRate@CDisplayManager@@AAEJXZ (retail 0x009BE340; Ego_d file id 600 lines 847..858, locals hr, mode) and
 // ?CalcPresentationIntervalTime@CDisplayManager@@AAEMJJ@Z (retail 0x009BE380; Ego_d lines 862..865, parameters refresh_rate,
 // vsync_count). Declarations only: the last two are functions of the display manager's own .cpp, expanded by CreateDevice.
 ECreateDeviceSuccess CreateDevice(const CCreateDeviceParameters& parameters);                    // 0x009BF7E0
 D3DDISPLAYMODE GetClosestMode(const C2DExtentsI& back_buffer_dim,long depth,bool exclusive) const; // 0x009BF600
 CPixelFormat GetClosestDepthBufferFormat(const D3DDISPLAYMODE& mode,long z_depth) const;         // 0x009BEBB0
 unsigned int CalculateD3DPresentationInterval(const CPresentationIntervalDesc& desc);            // 0x009BE2D0
 long ReadDeviceRefreshRate();                                                                    // 0x009BE340
 float CalcPresentationIntervalTime(long refresh_rate,long vsync_count);                          // 0x009BE380
#endif
#ifdef FABLE_RETAIL_CDISPLAYMANAGER_RESETDEVICE
 // (same NF145 opt-in) Ego_r.pdb 0x00B24B50 / FableWin.pdb 0x0301C220
 // ?ResetDevice@CDisplayManager@@AAE?AW4ECreateDeviceSuccess@1@ABUCCreateDeviceParameters@1@@Z (retail 0x009BFB30). Declaration only.
 ECreateDeviceSuccess ResetDevice(const CCreateDeviceParameters& parameters);                      // 0x009BFB30
#endif
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
FABLE_STATIC_ASSERT_AT(166, sizeof(CDisplayManager)==0x2cc);
#ifdef FABLE_RETAIL_REGION_DRAW_PASS29
class CDisplayManager;
extern CDisplayManager* GPDisplayManager;
#endif
#endif
