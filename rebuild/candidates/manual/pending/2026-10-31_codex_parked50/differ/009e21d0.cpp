// 009e21d0 GFACoordToVCoord
// Codex parked-50 best attempt: DIFFER(106v106); residue: 106/106 bytes; Y-coordinate path MOV of call receiver and FSTP of scalar argument are interchanged.
// Leaf: ?GFACoordToVCoord@@YIXABVC2DVector@@AAV1@@Z
// Flags:
#include "rebuild_abi.h"
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
// Layout: SDK GUID16 and 32bit criticalsection24; fields retain SDK spellings.
struct _GUID{unsigned long Data1;unsigned short Data2,Data3;unsigned char Data4[8];};
struct _RTL_CRITICAL_SECTION_DEBUG;
struct _RTL_CRITICAL_SECTION{_RTL_CRITICAL_SECTION_DEBUG*DebugInfo;long LockCount,RecursionCount;void*OwningThread;void*LockSemaphore;unsigned long SpinCount;};
#include "engine/CBaseClass.h"
// Layout: original CInitBaseClass is CBaseClass vptr0, Valid4, ABIpadding5..7. Explicit padding preserves the same baseboundary on GNU32.
class CInitBaseClass:public CBaseClass{public:bool Valid;unsigned char _padding_[3];};
class CDebugManager;class CInputManager;class CDisplayManager;class CSystemManager;class CCPPointerInfo;
// Layout: original CCountedPointer is Data0, Info4, size8.
#include "engine/CCountedPointer.h"
// Layout: original C2DExtentsI classV has long X0/Y4, size8; genuine user constructors establish byvalue classreturn ABI.
class C2DExtentsI{public:long X,Y;C2DExtentsI();C2DExtentsI(long,long);C2DExtentsI(const C2DExtentsI&);};
// Layout: full genuine DisplayManager prefix through RenderTargetDimensions194; SDK caps at5C..18C, BackBufferDimensions18C. No guessed byte array replaces known objects.
#include "engine/CBaseClassNonCopyable.h"
struct IDirect3D9;struct IDirect3DDevice9;struct IDirect3DTexture9;struct IDirect3DSurface9;
class CRenderManager;
namespace boost{template<class T>class scoped_ptr{private:T*ptr;public:explicit scoped_ptr(T*p=0);~scoped_ptr();};}
class CSurface{public:virtual~CSurface();IDirect3DSurface9*PD3DSurface;unsigned long AllocationSource;void*PAllocatedMemory;};
class CTexture{public:IDirect3DTexture9*PD3DTexture;unsigned long ByteLength:28;unsigned long AllocationSource:4;};
enum _D3DDEVTYPE{};
struct _D3DVSHADERCAPS2_0{unsigned long Caps;int DynamicFlowControlDepth;int NumTemps;int StaticFlowControlDepth;};
struct _D3DPSHADERCAPS2_0{unsigned long Caps;int DynamicFlowControlDepth;int NumTemps;int StaticFlowControlDepth;int NumInstructionSlots;};
struct _D3DCAPS9{_D3DDEVTYPE DeviceType;unsigned int AdapterOrdinal;unsigned long Caps;unsigned long Caps2;unsigned long Caps3;unsigned long PresentationIntervals;unsigned long CursorCaps;unsigned long DevCaps;unsigned long PrimitiveMiscCaps;unsigned long RasterCaps;unsigned long ZCmpCaps;unsigned long SrcBlendCaps;unsigned long DestBlendCaps;unsigned long AlphaCmpCaps;unsigned long ShadeCaps;unsigned long TextureCaps;unsigned long TextureFilterCaps;unsigned long CubeTextureFilterCaps;unsigned long VolumeTextureFilterCaps;unsigned long TextureAddressCaps;unsigned long VolumeTextureAddressCaps;unsigned long LineCaps;unsigned long MaxTextureWidth;unsigned long MaxTextureHeight;unsigned long MaxVolumeExtent;unsigned long MaxTextureRepeat;unsigned long MaxTextureAspectRatio;unsigned long MaxAnisotropy;float MaxVertexW;float GuardBandLeft;float GuardBandTop;float GuardBandRight;float GuardBandBottom;float ExtentsAdjust;unsigned long StencilCaps;unsigned long FVFCaps;unsigned long TextureOpCaps;unsigned long MaxTextureBlendStages;unsigned long MaxSimultaneousTextures;unsigned long VertexProcessingCaps;unsigned long MaxActiveLights;unsigned long MaxUserClipPlanes;unsigned long MaxVertexBlendMatrices;unsigned long MaxVertexBlendMatrixIndex;float MaxPointSize;unsigned long MaxPrimitiveCount;unsigned long MaxVertexIndex;unsigned long MaxStreams;unsigned long MaxStreamStride;unsigned long VertexShaderVersion;unsigned long MaxVertexShaderConst;unsigned long PixelShaderVersion;float PixelShader1xMaxValue;unsigned long DevCaps2;float MaxNpatchTessellationLevel;unsigned long Reserved5;unsigned int MasterAdapterOrdinal;unsigned int AdapterOrdinalInGroup;unsigned int NumberOfAdaptersInGroup;unsigned long DeclTypes;unsigned long NumSimultaneousRTs;unsigned long StretchRectFilterCaps;_D3DVSHADERCAPS2_0 VS20Caps;_D3DPSHADERCAPS2_0 PS20Caps;unsigned long VertexTextureFilterCaps;unsigned long MaxVShaderInstructionsExecuted;unsigned long MaxPShaderInstructionsExecuted;unsigned long MaxVertexShader30InstructionSlots;unsigned long MaxPixelShader30InstructionSlots;};
class CDisplayManager:public CBaseClassNonCopyable{public:CSystemManager*SystemManager;boost::scoped_ptr<CRenderManager>PRenderManager;CSurface RenderTargetSurface,RenderTargetDepthBuffer,BackBufferSurface,DepthBufferSurface;CTexture BackBufferCopyTexture;IDirect3D9*D3D;IDirect3DDevice9*D3DDevice;_D3DCAPS9 D3DDeviceCaps;C2DExtentsI BackBufferDimensions,RenderTargetDimensions;__declspec(noinline) const C2DExtentsI GetRenderTargetDimensions();};
FABLE_STATIC_ASSERT(sizeof(_D3DCAPS9)==304);FABLE_STATIC_ASSERT(sizeof(CSurface)==16);FABLE_STATIC_ASSERT(sizeof(CTexture)==8);

// Layout: original CSystemManager accessed prefix: base8, flags8/9/A, GUIDC/1C, PBestGUID2C, flag30, critical34, fibre4C, counted pointers50/58/60. The unused tail beyond68 is not declared or sized here.
class CSystemManager:public CInitBaseClass{public:bool WindowsQuit,ApplicationActive,ApplicationHasInputFocus;_GUID ApplicationGUID,BestGUID;_GUID*PBestGUID;bool PreferPrimaryDevice;_RTL_CRITICAL_SECTION CriticalSection;void*MainFibre;CCountedPointer<CDebugManager>LibDebugManager;CCountedPointer<CInputManager>PInputManager;CCountedPointer<CDisplayManager>PDisplayManager;CDisplayManager*GetDisplayManager(){return PDisplayManager.Get();}};
CSystemManager*__fastcall GFGetSystemManager();
// Original data symbols recovered from Ego_r global stream and InitVirtualPixels native stores.
extern double VIRTUAL_SCREEN_WIDTH;
extern double VIRTUAL_SCREEN_HEIGHT;
// Layout: original C2DVector classV float X0/Y4, sizeof8. No engine header.
class C2DVector{public:float X,Y;};
inline float __fastcall GFAXToVX(float x){
 CSystemManager*system=GFGetSystemManager();
 return ((float)VIRTUAL_SCREEN_WIDTH*x)/system->GetDisplayManager()->GetRenderTargetDimensions().X;
}
inline float __fastcall GFAYToVY(float y){
 CSystemManager*system=GFGetSystemManager();
 y=(float)VIRTUAL_SCREEN_HEIGHT*y;
 CDisplayManager*display=system->GetDisplayManager();
 return y/display->GetRenderTargetDimensions().Y;
}
void __fastcall GFACoordToVCoord(const C2DVector&a,C2DVector&v){v.X=GFAXToVX(a.X);v.Y=GFAYToVY(a.Y);}
FABLE_STATIC_ASSERT(sizeof(_GUID)==16);FABLE_STATIC_ASSERT(sizeof(_RTL_CRITICAL_SECTION)==24);FABLE_STATIC_ASSERT(sizeof(C2DExtentsI)==8);
