// 00b8d420 CEngineOcclusionManager::RenderOcclusionBox
// Leaf: ?RenderOcclusionBox@CEngineOcclusionManager@@QAEKABVCMatrix3x4@@@Z
// Flags:
#include "engine/CBaseClassNonCopyable.h"
#define CRenderStateManager CRenderStateManagerFields
#include "engine/CRenderStateManager.h"
#undef CRenderStateManager
#define CEngineOcclusionManager CEngineOcclusionManagerFields
#include "engine/CEngineOcclusionManager.h"
#undef CEngineOcclusionManager
#define CVertexShaderConstantLayout CVertexShaderConstantLayoutFields
#include "engine/CVertexShaderConstantLayout.h"
#undef CVertexShaderConstantLayout
#define CVertexBufferWin32 CVertexBufferWin32Fields
#include "engine/CVertexBufferWin32.h"
#undef CVertexBufferWin32
#define CIndexBuffer CIndexBufferFields
#include "engine/CIndexBuffer.h"
#undef CIndexBuffer
// Original Direct3D9 interface declarations from the SDK; unused parameter records remain opaque.
typedef long HRESULT;typedef unsigned long ULONG;typedef unsigned long DWORD;typedef unsigned int UINT;typedef int INT;typedef int BOOL;typedef unsigned long D3DCOLOR;
struct _GUID;typedef const _GUID&REFIID;struct HWND__;typedef HWND__*HWND;typedef void*HANDLE;
struct tagRECT;typedef tagRECT RECT;struct _RGNDATA;typedef _RGNDATA RGNDATA;struct tagPOINT;typedef tagPOINT POINT;struct tagPALETTEENTRY;typedef tagPALETTEENTRY PALETTEENTRY;
enum _D3DBACKBUFFER_TYPE{};typedef _D3DBACKBUFFER_TYPE D3DBACKBUFFER_TYPE;
struct _D3DCAPS9;typedef _D3DCAPS9 D3DCAPS9;
struct _D3DCLIPSTATUS9;typedef _D3DCLIPSTATUS9 D3DCLIPSTATUS9;
struct _D3DDEVICE_CREATION_PARAMETERS;typedef _D3DDEVICE_CREATION_PARAMETERS D3DDEVICE_CREATION_PARAMETERS;
struct _D3DDISPLAYMODE;typedef _D3DDISPLAYMODE D3DDISPLAYMODE;
enum _D3DFORMAT{};typedef _D3DFORMAT D3DFORMAT;
struct _D3DGAMMARAMP;typedef _D3DGAMMARAMP D3DGAMMARAMP;
struct _D3DLIGHT9;typedef _D3DLIGHT9 D3DLIGHT9;
struct _D3DMATERIAL9;typedef _D3DMATERIAL9 D3DMATERIAL9;
struct _D3DMATRIX;typedef _D3DMATRIX D3DMATRIX;
enum _D3DMULTISAMPLE_TYPE{};typedef _D3DMULTISAMPLE_TYPE D3DMULTISAMPLE_TYPE;
enum _D3DPOOL{};typedef _D3DPOOL D3DPOOL;
struct _D3DPRESENT_PARAMETERS_;typedef _D3DPRESENT_PARAMETERS_ D3DPRESENT_PARAMETERS;
enum _D3DPRIMITIVETYPE{};typedef _D3DPRIMITIVETYPE D3DPRIMITIVETYPE;
enum _D3DQUERYTYPE{};typedef _D3DQUERYTYPE D3DQUERYTYPE;
struct _D3DRASTER_STATUS;typedef _D3DRASTER_STATUS D3DRASTER_STATUS;
struct _D3DRECT;typedef _D3DRECT D3DRECT;
struct _D3DRECTPATCH_INFO;typedef _D3DRECTPATCH_INFO D3DRECTPATCH_INFO;
enum _D3DRENDERSTATETYPE{};typedef _D3DRENDERSTATETYPE D3DRENDERSTATETYPE;
enum _D3DSAMPLERSTATETYPE{};typedef _D3DSAMPLERSTATETYPE D3DSAMPLERSTATETYPE;
enum _D3DSTATEBLOCKTYPE{};typedef _D3DSTATEBLOCKTYPE D3DSTATEBLOCKTYPE;
enum _D3DTEXTUREFILTERTYPE{};typedef _D3DTEXTUREFILTERTYPE D3DTEXTUREFILTERTYPE;
enum _D3DTEXTURESTAGESTATETYPE{};typedef _D3DTEXTURESTAGESTATETYPE D3DTEXTURESTAGESTATETYPE;
enum _D3DTRANSFORMSTATETYPE{};typedef _D3DTRANSFORMSTATETYPE D3DTRANSFORMSTATETYPE;
struct _D3DTRIPATCH_INFO;typedef _D3DTRIPATCH_INFO D3DTRIPATCH_INFO;
struct _D3DVERTEXELEMENT9;typedef _D3DVERTEXELEMENT9 D3DVERTEXELEMENT9;
struct _D3DVIEWPORT9;typedef _D3DVIEWPORT9 D3DVIEWPORT9;
struct IDirect3D9;
struct IDirect3DBaseTexture9;
struct IDirect3DCubeTexture9;
struct IDirect3DIndexBuffer9;
struct IDirect3DPixelShader9;
struct IDirect3DQuery9;
struct IDirect3DStateBlock9;
struct IDirect3DSurface9;
struct IDirect3DSwapChain9;
struct IDirect3DTexture9;
struct IDirect3DVertexBuffer9;
struct IDirect3DVertexDeclaration9;
struct IDirect3DVertexShader9;
struct IDirect3DVolumeTexture9;
struct IUnknown{virtual HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObj)=0;virtual ULONG __stdcall AddRef()=0;virtual ULONG __stdcall Release()=0;};
struct IDirect3DDevice9:public IUnknown{
virtual HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObj)=0;
virtual ULONG __stdcall AddRef()=0;
virtual ULONG __stdcall Release()=0;
virtual HRESULT __stdcall TestCooperativeLevel()=0;
virtual UINT __stdcall GetAvailableTextureMem()=0;
virtual HRESULT __stdcall EvictManagedResources()=0;
virtual HRESULT __stdcall GetDirect3D(IDirect3D9** ppD3D9)=0;
virtual HRESULT __stdcall GetDeviceCaps(D3DCAPS9* pCaps)=0;
virtual HRESULT __stdcall GetDisplayMode(UINT iSwapChain,D3DDISPLAYMODE* pMode)=0;
virtual HRESULT __stdcall GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS *pParameters)=0;
virtual HRESULT __stdcall SetCursorProperties(UINT XHotSpot,UINT YHotSpot,IDirect3DSurface9* pCursorBitmap)=0;
virtual void __stdcall SetCursorPosition(int X,int Y,DWORD Flags)=0;
virtual BOOL __stdcall ShowCursor(BOOL bShow)=0;
virtual HRESULT __stdcall CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DSwapChain9** pSwapChain)=0;
virtual HRESULT __stdcall GetSwapChain(UINT iSwapChain,IDirect3DSwapChain9** pSwapChain)=0;
virtual UINT __stdcall GetNumberOfSwapChains()=0;
virtual HRESULT __stdcall Reset(D3DPRESENT_PARAMETERS* pPresentationParameters)=0;
virtual HRESULT __stdcall Present(const RECT* pSourceRect,const RECT* pDestRect,HWND hDestWindowOverride,const RGNDATA* pDirtyRegion)=0;
virtual HRESULT __stdcall GetBackBuffer(UINT iSwapChain,UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer)=0;
virtual HRESULT __stdcall GetRasterStatus(UINT iSwapChain,D3DRASTER_STATUS* pRasterStatus)=0;
virtual HRESULT __stdcall SetDialogBoxMode(BOOL bEnableDialogs)=0;
virtual void __stdcall SetGammaRamp(UINT iSwapChain,DWORD Flags,const D3DGAMMARAMP* pRamp)=0;
virtual void __stdcall GetGammaRamp(UINT iSwapChain,D3DGAMMARAMP* pRamp)=0;
virtual HRESULT __stdcall CreateTexture(UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle)=0;
virtual HRESULT __stdcall CreateVolumeTexture(UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture9** ppVolumeTexture,HANDLE* pSharedHandle)=0;
virtual HRESULT __stdcall CreateCubeTexture(UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture9** ppCubeTexture,HANDLE* pSharedHandle)=0;
virtual HRESULT __stdcall CreateVertexBuffer(UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer,HANDLE* pSharedHandle)=0;
virtual HRESULT __stdcall CreateIndexBuffer(UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer,HANDLE* pSharedHandle)=0;
virtual HRESULT __stdcall CreateRenderTarget(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)=0;
virtual HRESULT __stdcall CreateDepthStencilSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)=0;
virtual HRESULT __stdcall UpdateSurface(IDirect3DSurface9* pSourceSurface,const RECT* pSourceRect,IDirect3DSurface9* pDestinationSurface,const POINT* pDestPoint)=0;
virtual HRESULT __stdcall UpdateTexture(IDirect3DBaseTexture9* pSourceTexture,IDirect3DBaseTexture9* pDestinationTexture)=0;
virtual HRESULT __stdcall GetRenderTargetData(IDirect3DSurface9* pRenderTarget,IDirect3DSurface9* pDestSurface)=0;
virtual HRESULT __stdcall GetFrontBufferData(UINT iSwapChain,IDirect3DSurface9* pDestSurface)=0;
virtual HRESULT __stdcall StretchRect(IDirect3DSurface9* pSourceSurface,const RECT* pSourceRect,IDirect3DSurface9* pDestSurface,const RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter)=0;
virtual HRESULT __stdcall ColorFill(IDirect3DSurface9* pSurface,const RECT* pRect,D3DCOLOR color)=0;
virtual HRESULT __stdcall CreateOffscreenPlainSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)=0;
virtual HRESULT __stdcall SetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9* pRenderTarget)=0;
virtual HRESULT __stdcall GetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9** ppRenderTarget)=0;
virtual HRESULT __stdcall SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil)=0;
virtual HRESULT __stdcall GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface)=0;
virtual HRESULT __stdcall BeginScene()=0;
virtual HRESULT __stdcall EndScene()=0;
virtual HRESULT __stdcall Clear(DWORD Count,const D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,float Z,DWORD Stencil)=0;
virtual HRESULT __stdcall SetTransform(D3DTRANSFORMSTATETYPE State,const D3DMATRIX* pMatrix)=0;
virtual HRESULT __stdcall GetTransform(D3DTRANSFORMSTATETYPE State,D3DMATRIX* pMatrix)=0;
virtual HRESULT __stdcall MultiplyTransform(D3DTRANSFORMSTATETYPE,const D3DMATRIX*)=0;
virtual HRESULT __stdcall SetViewport(const D3DVIEWPORT9* pViewport)=0;
virtual HRESULT __stdcall GetViewport(D3DVIEWPORT9* pViewport)=0;
virtual HRESULT __stdcall SetMaterial(const D3DMATERIAL9* pMaterial)=0;
virtual HRESULT __stdcall GetMaterial(D3DMATERIAL9* pMaterial)=0;
virtual HRESULT __stdcall SetLight(DWORD Index,const D3DLIGHT9*)=0;
virtual HRESULT __stdcall GetLight(DWORD Index,D3DLIGHT9*)=0;
virtual HRESULT __stdcall LightEnable(DWORD Index,BOOL Enable)=0;
virtual HRESULT __stdcall GetLightEnable(DWORD Index,BOOL* pEnable)=0;
virtual HRESULT __stdcall SetClipPlane(DWORD Index,const float* pPlane)=0;
virtual HRESULT __stdcall GetClipPlane(DWORD Index,float* pPlane)=0;
virtual HRESULT __stdcall SetRenderState(D3DRENDERSTATETYPE State,DWORD Value)=0;
virtual HRESULT __stdcall GetRenderState(D3DRENDERSTATETYPE State,DWORD* pValue)=0;
virtual HRESULT __stdcall CreateStateBlock(D3DSTATEBLOCKTYPE Type,IDirect3DStateBlock9** ppSB)=0;
virtual HRESULT __stdcall BeginStateBlock()=0;
virtual HRESULT __stdcall EndStateBlock(IDirect3DStateBlock9** ppSB)=0;
virtual HRESULT __stdcall SetClipStatus(const D3DCLIPSTATUS9* pClipStatus)=0;
virtual HRESULT __stdcall GetClipStatus(D3DCLIPSTATUS9* pClipStatus)=0;
virtual HRESULT __stdcall GetTexture(DWORD Stage,IDirect3DBaseTexture9** ppTexture)=0;
virtual HRESULT __stdcall SetTexture(DWORD Stage,IDirect3DBaseTexture9* pTexture)=0;
virtual HRESULT __stdcall GetTextureStageState(DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD* pValue)=0;
virtual HRESULT __stdcall SetTextureStageState(DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD Value)=0;
virtual HRESULT __stdcall GetSamplerState(DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD* pValue)=0;
virtual HRESULT __stdcall SetSamplerState(DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD Value)=0;
virtual HRESULT __stdcall ValidateDevice(DWORD* pNumPasses)=0;
virtual HRESULT __stdcall SetPaletteEntries(UINT PaletteNumber,const PALETTEENTRY* pEntries)=0;
virtual HRESULT __stdcall GetPaletteEntries(UINT PaletteNumber,PALETTEENTRY* pEntries)=0;
virtual HRESULT __stdcall SetCurrentTexturePalette(UINT PaletteNumber)=0;
virtual HRESULT __stdcall GetCurrentTexturePalette(UINT *PaletteNumber)=0;
virtual HRESULT __stdcall SetScissorRect(const RECT* pRect)=0;
virtual HRESULT __stdcall GetScissorRect(RECT* pRect)=0;
virtual HRESULT __stdcall SetSoftwareVertexProcessing(BOOL bSoftware)=0;
virtual BOOL __stdcall GetSoftwareVertexProcessing()=0;
virtual HRESULT __stdcall SetNPatchMode(float nSegments)=0;
virtual float __stdcall GetNPatchMode()=0;
virtual HRESULT __stdcall DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount)=0;
virtual HRESULT __stdcall DrawIndexedPrimitive(D3DPRIMITIVETYPE,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)=0;
virtual HRESULT __stdcall DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,const void* pVertexStreamZeroData,UINT VertexStreamZeroStride)=0;
virtual HRESULT __stdcall DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertices,UINT PrimitiveCount,const void* pIndexData,D3DFORMAT IndexDataFormat,const void* pVertexStreamZeroData,UINT VertexStreamZeroStride)=0;
virtual HRESULT __stdcall ProcessVertices(UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,IDirect3DVertexBuffer9* pDestBuffer,IDirect3DVertexDeclaration9* pVertexDecl,DWORD Flags)=0;
virtual HRESULT __stdcall CreateVertexDeclaration(const D3DVERTEXELEMENT9* pVertexElements,IDirect3DVertexDeclaration9** ppDecl)=0;
virtual HRESULT __stdcall SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl)=0;
virtual HRESULT __stdcall GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl)=0;
virtual HRESULT __stdcall SetFVF(DWORD FVF)=0;
virtual HRESULT __stdcall GetFVF(DWORD* pFVF)=0;
virtual HRESULT __stdcall CreateVertexShader(const DWORD* pFunction,IDirect3DVertexShader9** ppShader)=0;
virtual HRESULT __stdcall SetVertexShader(IDirect3DVertexShader9* pShader)=0;
virtual HRESULT __stdcall GetVertexShader(IDirect3DVertexShader9** ppShader)=0;
virtual HRESULT __stdcall SetVertexShaderConstantF(UINT StartRegister,const float* pConstantData,UINT Vector4fCount)=0;
virtual HRESULT __stdcall GetVertexShaderConstantF(UINT StartRegister,float* pConstantData,UINT Vector4fCount)=0;
virtual HRESULT __stdcall SetVertexShaderConstantI(UINT StartRegister,const int* pConstantData,UINT Vector4iCount)=0;
virtual HRESULT __stdcall GetVertexShaderConstantI(UINT StartRegister,int* pConstantData,UINT Vector4iCount)=0;
virtual HRESULT __stdcall SetVertexShaderConstantB(UINT StartRegister,const BOOL* pConstantData,UINT  BoolCount)=0;
virtual HRESULT __stdcall GetVertexShaderConstantB(UINT StartRegister,BOOL* pConstantData,UINT BoolCount)=0;
virtual HRESULT __stdcall SetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT OffsetInBytes,UINT Stride)=0;
virtual HRESULT __stdcall GetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer9** ppStreamData,UINT* pOffsetInBytes,UINT* pStride)=0;
virtual HRESULT __stdcall SetStreamSourceFreq(UINT StreamNumber,UINT Setting)=0;
virtual HRESULT __stdcall GetStreamSourceFreq(UINT StreamNumber,UINT* pSetting)=0;
virtual HRESULT __stdcall SetIndices(IDirect3DIndexBuffer9* pIndexData)=0;
virtual HRESULT __stdcall GetIndices(IDirect3DIndexBuffer9** ppIndexData)=0;
virtual HRESULT __stdcall CreatePixelShader(const DWORD* pFunction,IDirect3DPixelShader9** ppShader)=0;
virtual HRESULT __stdcall SetPixelShader(IDirect3DPixelShader9* pShader)=0;
virtual HRESULT __stdcall GetPixelShader(IDirect3DPixelShader9** ppShader)=0;
virtual HRESULT __stdcall SetPixelShaderConstantF(UINT StartRegister,const float* pConstantData,UINT Vector4fCount)=0;
virtual HRESULT __stdcall GetPixelShaderConstantF(UINT StartRegister,float* pConstantData,UINT Vector4fCount)=0;
virtual HRESULT __stdcall SetPixelShaderConstantI(UINT StartRegister,const int* pConstantData,UINT Vector4iCount)=0;
virtual HRESULT __stdcall GetPixelShaderConstantI(UINT StartRegister,int* pConstantData,UINT Vector4iCount)=0;
virtual HRESULT __stdcall SetPixelShaderConstantB(UINT StartRegister,const BOOL* pConstantData,UINT  BoolCount)=0;
virtual HRESULT __stdcall GetPixelShaderConstantB(UINT StartRegister,BOOL* pConstantData,UINT BoolCount)=0;
virtual HRESULT __stdcall DrawRectPatch(UINT Handle,const float* pNumSegs,const D3DRECTPATCH_INFO* pRectPatchInfo)=0;
virtual HRESULT __stdcall DrawTriPatch(UINT Handle,const float* pNumSegs,const D3DTRIPATCH_INFO* pTriPatchInfo)=0;
virtual HRESULT __stdcall DeletePatch(UINT Handle)=0;
virtual HRESULT __stdcall CreateQuery(D3DQUERYTYPE Type,IDirect3DQuery9** ppQuery)=0;
};
enum POLY_CULL_MODE{};enum EPrimitiveType{PT_NULL=-1,PT_TRIANGLE_LIST=4,PT_TRIANGLE_STRIP=5,PT_TRIANGLE_FAN=6,PT_LINE_LIST=2,PT_LINE_STRIP=3,PT_POINT_LIST=1};enum EVertexType{};
class CRenderStateManager:public CRenderStateManagerFields{
 using CRenderStateManagerFields::StateStack;using CRenderStateManagerFields::StateUpdateList;using CRenderStateManagerFields::StateStackSize;using CRenderStateManagerFields::StateUpdateListSize;using CRenderStateManagerFields::CurrentBookmarkMask;using CRenderStateManagerFields::DeviceState;using CRenderStateManagerFields::BookmarkDummyState;
 struct CStateInfo{public:unsigned long CurrentState,DesiredState,BookmarkMask;unsigned char unreadParam[4];bool DirtyListFlag;unsigned char unreadType[3];};
 struct CStateStackElement{public:CStateInfo*State;unsigned long Value;};
 struct CDeviceState{public:unsigned char unread0[40];CStateInfo ColourWriteEnable,ZBufferWriteEnable,ZBufferEnable;unsigned char unread100[20];CStateInfo PolyCullMode;};
 static unsigned long D3DPolyCullModeTable[3];
 __forceinline void SetRenderState(unsigned long value,CStateInfo&state){
 if(state.DesiredState!=value){
 if((CurrentBookmarkMask&state.BookmarkMask)==0){state.BookmarkMask|=CurrentBookmarkMask;reinterpret_cast<CStateStackElement*>(StateStack)[StateStackSize].State=&state;reinterpret_cast<CStateStackElement*>(StateStack)[StateStackSize].Value=state.DesiredState;++StateStackSize;}
 state.DesiredState=value;if(!state.DirtyListFlag){state.DirtyListFlag=true;StateUpdateList[StateUpdateListSize]=reinterpret_cast< ::CStateInfo*>(&state);++StateUpdateListSize;}
 }}
 public:void RestoreCaptureBlock();void RealiseRenderState();
 __forceinline void StartCaptureBlock(){reinterpret_cast<CStateStackElement*>(StateStack)[StateStackSize++].State=reinterpret_cast<CStateInfo*>(BookmarkDummyState);CurrentBookmarkMask<<=1;}
 __forceinline void SetPolyCullMode(POLY_CULL_MODE value){SetRenderState(D3DPolyCullModeTable[value],reinterpret_cast<CDeviceState*>(DeviceState)->PolyCullMode);}
 __forceinline void SetColourWriteEnable(bool value){SetRenderState(value?15:0,reinterpret_cast<CDeviceState*>(DeviceState)->ColourWriteEnable);}
 __forceinline void SetZBufferEnable(bool value){SetRenderState(value?1:0,reinterpret_cast<CDeviceState*>(DeviceState)->ZBufferEnable);}
 __forceinline void SetZBufferWriteEnable(bool value){SetRenderState(value?1:0,reinterpret_cast<CDeviceState*>(DeviceState)->ZBufferWriteEnable);}
};
class CRenderManagerCore:public CRenderStateManager{};class CRenderManager2D:public CRenderManagerCore{};class CRenderManager:public CRenderManager2D{};
namespace NEngineInternal{extern CRenderManager*GPRenderManager;}
class CRenderStateCaptureBlock{bool Open;static CRenderStateManager*StateManager;public:
 __forceinline CRenderStateCaptureBlock():Open(false){Capture();}
 __forceinline ~CRenderStateCaptureBlock(){Restore();}
 __forceinline void Capture(){if(!Open){StateManager->StartCaptureBlock();Open=true;}}
 __forceinline void Restore(){if(Open){StateManager->RestoreCaptureBlock();Open=false;}}
};

class CMatrix3x4;class CVertexShaderData;class CPixelShaderData;class CShaderBase{protected:virtual~CShaderBase();};class CVertexShader:public CShaderBase{private:CVertexShaderData*Data;};class CPixelShader:public CShaderBase{private:CPixelShaderData*Data;};
namespace std{template<class T>class allocator{};template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:unsigned int size()const{return this->_M_finish-this->_M_start;}const T*begin()const{return this->_M_start;}};}
template<class T>class CArray:public std::vector<T>{public:const T*PeekAsCArray()const{return this->size()>0?this->begin():GetZeroSizeArray();}private:const T*GetZeroSizeArray()const{return reinterpret_cast<const T*>(this);}};
class CVertexShaderConstant{public:float X,Y,Z,W;};
class CShaderConstantRange:public CBaseClass{public:long Offset;unsigned long Count;};
// Layout: original private range4 and vector140 use supplied native layout; shader manager is their original consumer.
class CVertexShaderConstantLayout:public CBaseClass{private:CShaderConstantRange Reserved;unsigned char unreadRanges[0x130];CArray<CVertexShaderConstant>ReservedPresets;friend class CShaderRenderManager;};
// Layout: original vertex-buffer protected data from shared52-byte header.
class CVertexBufferBase:public CBaseClassNonCopyable{protected:IDirect3DVertexBuffer9*PVertexBuffer;unsigned long Length;EVertexType Type;unsigned char Stride;unsigned char unreadCompression[0x34-0x11];public:long GetStride()const{return Stride;}IDirect3DVertexBuffer9*GetD3DVertexBuffer(){return PVertexBuffer;}};
class CVertexBufferWin32:public CVertexBufferBase{};
class CIndexBuffer:public CIndexBufferFields{public:IDirect3DIndexBuffer9*GetD3DIndexBuffer(){return PD3DIndexBuffer;}EPrimitiveType GetPrimitiveType()const{return reinterpret_cast<const EPrimitiveType&>(PrimitiveType);}};
class CShaderRenderManager:public CBaseClassNonCopyable{private:const CVertexShaderConstantLayout*CurrentVSConstantLayout;unsigned char unread08[0x1c8];CRenderManager*RenderManager;IDirect3DDevice9*D3DDevice;unsigned char unread1d8[0x10];bool WorldMatrixIsIdentity;unsigned char unread1e9[0x1ef];unsigned long UpdateFlags;
public:void ApplyVertexShader(const CVertexShader&,bool);void ApplyPixelShader(const CPixelShader&);void SetWorldTransform(const CMatrix3x4&);
 __forceinline void SelectVertexBuffer(CVertexBufferWin32&vb){D3DDevice->SetStreamSource(0,vb.GetD3DVertexBuffer(),0,vb.GetStride());}
 __forceinline void DrawBuffersIndexed(CVertexBufferWin32&vb,unsigned long first,unsigned long count,CIndexBuffer&ib,unsigned long start,unsigned long primitives,EPrimitiveType type){SelectVertexBuffer(vb);D3DDevice->SetIndices(ib.GetD3DIndexBuffer());if(type==PT_NULL)type=ib.GetPrimitiveType();RenderManager->RealiseRenderState();D3DDevice->DrawIndexedPrimitive(static_cast<D3DPRIMITIVETYPE>(type),0,first,count,start,primitives);}
    __forceinline void PrepareForRender(unsigned long flags)
    {
        if (UpdateFlags & 4)
            SetVSConstants(CurrentVSConstantLayout->Reserved.Offset, CurrentVSConstantLayout->ReservedPresets.PeekAsCArray(),
                           CurrentVSConstantLayout->ReservedPresets.size());
        if (UpdateFlags & 1)
            UpdatePixelShader();
        if (UpdateFlags & 2)
            UpdatePixelShaderConstants();
        if ((flags & 0x400) && (UpdateFlags & 0x20))
            UpdateBoneTransform();
        if (flags & 0x40) {
            if (UpdateFlags & 0x1000)
                UpdateWorldSpaceTextureTransform();
        } else if (flags & 0x80) {
            if (UpdateFlags & 0x800)
                UpdateObjectSpaceTextureTransform();
        }
        if (flags & 0x200) {
            if (UpdateFlags & 0x2000)
                UpdateWorldSpaceFogTransform();
        } else if (flags & 0x100) {
            if (UpdateFlags & 0x4000)
                UpdateObjectSpaceFogTransform();
        }
        if (UpdateFlags & 4)
            UpdateAmbient();
        if (UpdateFlags & 0x20000)
            UpdateFogColour();
        if (UpdateFlags & 0x400)
            UpdateLightGlobals();
        if (flags & 8) {
            if (UpdateFlags & 0x100)
                UpdateObjectSpaceLighting(flags);
        } else if (flags & 0x10) {
            if (UpdateFlags & 0x200)
                UpdateWorldSpaceLighting();
        }
        if ((flags & 0x20) && (UpdateFlags & 0x8000))
            UpdateLocalisedSphereMapping();
        if (!(flags & 1) && !WorldMatrixIsIdentity)
            ResetWorldTransform();
        if (UpdateFlags & 0x80)
            UpdateCombinedProjectionTransform();
        if ((flags & 4) && (UpdateFlags & 8))
            UpdateCameraPos();
    }

private:
    void SetVSConstants(long reg, const void* data, unsigned long count);   // 0x0098B930
    void UpdatePixelShader();                           // 0x00988A20
    void UpdatePixelShaderConstants();                  // 0x00989BF0
    void UpdateBoneTransform();                         // 0x0098A8E0
    void UpdateWorldSpaceTextureTransform();            // 0x009894B0
    void UpdateObjectSpaceTextureTransform();           // 0x00989230
    void UpdateWorldSpaceFogTransform();                // 0x00989640
    void UpdateObjectSpaceFogTransform();               // 0x00989540
    void UpdateAmbient();                               // 0x00989760
    void UpdateFogColour();                             // 0x009897C0
    void UpdateLightGlobals();                          // 0x0098A760
    void UpdateObjectSpaceLighting(unsigned long flags);   // 0x00989CB0
    void UpdateWorldSpaceLighting();                    // 0x0098A540
    void UpdateLocalisedSphereMapping();                // 0x00988E80
    void ResetWorldTransform();                         // 0x00988290
    void UpdateCombinedProjectionTransform();           // 0x00988A50
    void UpdateCameraPos();                             // 0x00989700
};

namespace NEngineInternal{extern CShaderRenderManager*GPShaderRenderManager;}
// Layout: shared native manager retains all original storage; the original entry is a nested struct, not a global surrogate.
class CEngineOcclusionManager:public CEngineOcclusionManagerFields{private:struct COccluderEntry{unsigned long PixelCount;unsigned long SlotID:30;unsigned long Testing:1;unsigned long Completed:1;COccluderEntry():PixelCount(0),SlotID(0),Testing(0),Completed(0){}};
public:unsigned long AllocateOccluderHandle(){if(EntriesSize<8192){unsigned long handle=EntriesSize;reinterpret_cast<COccluderEntry*>(Entries)[handle]=COccluderEntry();++EntriesSize;return handle;}return static_cast<unsigned long>(-1);}unsigned long RenderOcclusionBox(const CMatrix3x4&);
};
namespace NEngineInternal{extern ::CEngineOcclusionManager*GPOcclusionManager;}
unsigned long CEngineOcclusionManager::RenderOcclusionBox(const CMatrix3x4&transform){
 CRenderStateCaptureBlock capture;
 NEngineInternal::GPShaderRenderManager->ApplyVertexShader(reinterpret_cast<const CVertexShader&>(OcclusionBoxVShader),false);
 NEngineInternal::GPShaderRenderManager->ApplyPixelShader(reinterpret_cast<const CPixelShader&>(OcclusionBoxPixelShader));
 NEngineInternal::GPShaderRenderManager->SetWorldTransform(transform);
 NEngineInternal::GPRenderManager->SetColourWriteEnable(false);
 NEngineInternal::GPRenderManager->SetZBufferWriteEnable(false);
 NEngineInternal::GPShaderRenderManager->PrepareForRender(1);
 CShaderRenderManager*shaders=NEngineInternal::GPShaderRenderManager;
 CVertexBufferWin32&vb=reinterpret_cast<CVertexBufferWin32&>(UnitBoxVB);CIndexBuffer&ib=reinterpret_cast<CIndexBuffer&>(UnitBoxIB);
 shaders->DrawBuffersIndexed(vb,0,8,ib,0,12,PT_NULL);
 return NEngineInternal::GPOcclusionManager->AllocateOccluderHandle();
}
