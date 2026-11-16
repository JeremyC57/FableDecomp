// Self-contained test: real shared class declarations; external operations are test doubles.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#ifdef _MSC_VER
#define FABLE_DUAL_PATH_TEST
#else
#define __fastcall
#define __cdecl
#define __stdcall
#define __int64 long long
#define __declspec(x)
#define FABLE_PORTABLE
#endif
// Access-only test instrumentation; no field or class is replaced.
#define private public
#define protected public
#define FABLE_RETAIL_STLP_HOST_TAGS_PASS23
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL_COMPLETE_ENUM_PASS23
#include "engine/EIndexBufferFormat.h"
#include "engine/EIndexBufferPool.h"
#include "engine/EPrimitiveType.h"
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __int64 long long
#define __declspec(x)
#define FABLE_PORTABLE
#endif
// 00B8DCD0 CEngineOcclusionManager::SelectFlareTestVertexBuffer -- cache and select a circle fan vertex buffer.
// Leaf: ?SelectFlareTestVertexBuffer@CEngineOcclusionManager@@AAEXK@Z
// Flags: none
// Retail-asm-helper: GFFloatToLongNear, existing shared header unchanged.
// Retail-asm: 0x00B8DE0E..0x00B8DE12 and 0x00B8DE62..0x00B8DE66: dword FISTP.
// Both per-site ladders:30 plain nearest-even C++ spellings and10 intrinsic spellings,
// retained in PROBES.md/catalogs/history; no C++ or intrinsic emits these native stores.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_PASS22_FLAREBUFFER
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_STLP_PASS22_LOWER_BOUND_COMPARE
#define FABLE_RETAIL_STLP_PASS22_LOWER_BOUND_BODY
#define FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD
#define FABLE_RETAIL_STLP_PASS22_COPY_BACKWARD_FASTCALL
#define FABLE_RETAIL_STLP_PASS22_FLARE_ABI
#define FABLE_RETAIL_CSHADERRENDERMANAGER_SELECTVERTEXBUFFER
#define FABLE_RETAIL_CGENERICVERTEX_INLINE_SETTERS
#define FABLE_RETAIL_CGENERICVERTEX_PASS22_SETU1V1
#include <math.h>
#include "engine/EVertexType.h"
#include "engine/EVertexBufferPool.h"
#include "engine/CEngineOcclusionManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CTVertexNormTex1.h"
#include "stlport/fable_algorithm.h"
namespace NEngineInternal {extern CShaderRenderManager* GPShaderRenderManager;}
extern float GFastCosTable[1025];
namespace {
struct MapperPred {
    template<class T> bool operator()(const std::pair<unsigned long,T>& value,unsigned long key)const {return value.first<key;}
};
}
#include "engine/GFFloatToLongNear.h"
#include "engine/GFFastSin.h"
void CEngineOcclusionManager::SelectFlareTestVertexBuffer(unsigned long circle_segments)
{
    CCountedPointer<CVertexBufferWin32> vertexbuffer;
    std::vector<std::pair<unsigned long,CCountedPointer<CVertexBufferWin32> > >::iterator found=
        std::lower_bound(FlareVertexBuffers.begin(),FlareVertexBuffers.end(),circle_segments,MapperPred());
    if(found==FlareVertexBuffers.end()||found->first!=circle_segments) {
        vertexbuffer.Reset(new CVertexBufferWin32);
        vertexbuffer->Initialise(VERTEX_NULL,circle_segments+2,sizeof(CTVertexNormTex1),0,VB_POOL_MANAGED);
        CTVertexNormTex1* verts=reinterpret_cast<CTVertexNormTex1*>(vertexbuffer->Lock(0,0,0));
        CTVertexNormTex1& vert0=verts[0];
        vert0.SetPosition(C3DVector(0,0,0));
        float anglefactor=1.0f/(float)circle_segments;
        for(unsigned long c_vert=0;c_vert<circle_segments+1;++c_vert) {
            CTVertexNormTex1& vert=verts[c_vert+1];
            vert.SetPosition(C3DVector(GFFastCos(c_vert*anglefactor),-GFFastSin(c_vert*anglefactor),0));
            vert.SetNormal(C3DVector(0,0,0));
            vert.SetU1V1(0,0);
        }
        vertexbuffer->Unlock();
        FlareVertexBuffers.insert(found,std::make_pair(circle_segments,vertexbuffer));
    } else {
        vertexbuffer=found->second;
    }
    NEngineInternal::GPShaderRenderManager->SelectVertexBuffer(*vertexbuffer);
}

#undef private
#undef protected

static int LiveBuffers,DestroyedBuffers,CreatedBuffers,InitialiseCalls,LockCalls,UnlockCalls;
static bool SwitchManagerOnInitialise;
struct IDirect3DVertexBuffer9 { CVertexBufferWin32* Owner; };
class MockDevice : public IDirect3DDevice9 {
public:
 int Calls; CVertexBufferWin32* Last;
 unsigned int LastStream,LastOffset,LastStride; IDirect3DVertexBuffer9* LastBuffer;
 MockDevice():Calls(0),Last(0),LastStream(99),LastOffset(99),LastStride(99),LastBuffer(0){}
long __stdcall QueryInterface(const _GUID& riid, void** ppvObj) { assert(false); return 0; }
unsigned long __stdcall AddRef() { assert(false); return 0; }
unsigned long __stdcall Release() { assert(false); return 0; }
long __stdcall TestCooperativeLevel() { assert(false); return 0; }
unsigned int __stdcall GetAvailableTextureMem() { assert(false); return 0; }
long __stdcall EvictManagedResources() { assert(false); return 0; }
long __stdcall GetDirect3D(IDirect3D9** ppD3D9) { assert(false); return 0; }
long __stdcall GetDeviceCaps(_D3DCAPS9* pCaps) { assert(false); return 0; }
long __stdcall GetDisplayMode(unsigned int iSwapChain, _D3DDISPLAYMODE* pMode) { assert(false); return 0; }
long __stdcall GetCreationParameters(_D3DDEVICE_CREATION_PARAMETERS* pParameters) { assert(false); return 0; }
long __stdcall SetCursorProperties(unsigned int XHotSpot, unsigned int YHotSpot, IDirect3DSurface9* pCursorBitmap) { assert(false); return 0; }
void __stdcall SetCursorPosition(int X, int Y, unsigned long Flags) { assert(false); }
int __stdcall ShowCursor(int bShow) { assert(false); return 0; }
long __stdcall CreateAdditionalSwapChain(_D3DPRESENT_PARAMETERS_* pPresentationParameters, IDirect3DSwapChain9** pSwapChain) { assert(false); return 0; }
long __stdcall GetSwapChain(unsigned int iSwapChain, IDirect3DSwapChain9** pSwapChain) { assert(false); return 0; }
unsigned int __stdcall GetNumberOfSwapChains() { assert(false); return 0; }
long __stdcall Reset(_D3DPRESENT_PARAMETERS_* pPresentationParameters) { assert(false); return 0; }
long __stdcall Present(const tagRECT* pSourceRect, const tagRECT* pDestRect, HWND__* hDestWindowOverride, const _RGNDATA* pDirtyRegion) { assert(false); return 0; }
long __stdcall GetBackBuffer(unsigned int iSwapChain, unsigned int iBackBuffer, unsigned long Type, IDirect3DSurface9** ppBackBuffer) { assert(false); return 0; }
long __stdcall GetRasterStatus(unsigned int iSwapChain, _D3DRASTER_STATUS* pRasterStatus) { assert(false); return 0; }
long __stdcall SetDialogBoxMode(int bEnableDialogs) { assert(false); return 0; }
void __stdcall SetGammaRamp(unsigned int iSwapChain, unsigned long Flags, const _D3DGAMMARAMP* pRamp) { assert(false); }
void __stdcall GetGammaRamp(unsigned int iSwapChain, _D3DGAMMARAMP* pRamp) { assert(false); }
long __stdcall CreateTexture(unsigned int Width, unsigned int Height, unsigned int Levels, unsigned long Usage, unsigned long Format, unsigned long Pool, IDirect3DTexture9** ppTexture, void** pSharedHandle) { assert(false); return 0; }
long __stdcall CreateVolumeTexture(unsigned int Width, unsigned int Height, unsigned int Depth, unsigned int Levels, unsigned long Usage, unsigned long Format, unsigned long Pool, IDirect3DVolumeTexture9** ppVolumeTexture, void** pSharedHandle) { assert(false); return 0; }
long __stdcall CreateCubeTexture(unsigned int EdgeLength, unsigned int Levels, unsigned long Usage, unsigned long Format, unsigned long Pool, IDirect3DCubeTexture9** ppCubeTexture, void** pSharedHandle) { assert(false); return 0; }
long __stdcall CreateVertexBuffer(unsigned int Length, unsigned long Usage, unsigned long FVF, unsigned long Pool, IDirect3DVertexBuffer9** ppVertexBuffer, void** pSharedHandle) { assert(false); return 0; }
long __stdcall CreateIndexBuffer(unsigned int Length, unsigned long Usage, unsigned long Format, unsigned long Pool, IDirect3DIndexBuffer9** ppIndexBuffer, void** pSharedHandle) { assert(false); return 0; }
long __stdcall CreateRenderTarget(unsigned int Width, unsigned int Height, unsigned long Format, unsigned long MultiSample, unsigned long MultisampleQuality, int Lockable, IDirect3DSurface9** ppSurface, void** pSharedHandle) { assert(false); return 0; }
long __stdcall CreateDepthStencilSurface(unsigned int Width, unsigned int Height, unsigned long Format, unsigned long MultiSample, unsigned long MultisampleQuality, int Discard, IDirect3DSurface9** ppSurface, void** pSharedHandle) { assert(false); return 0; }
long __stdcall UpdateSurface(IDirect3DSurface9* pSourceSurface, const tagRECT* pSourceRect, IDirect3DSurface9* pDestinationSurface, const tagPOINT* pDestPoint) { assert(false); return 0; }
long __stdcall UpdateTexture(IDirect3DBaseTexture9* pSourceTexture, IDirect3DBaseTexture9* pDestinationTexture) { assert(false); return 0; }
long __stdcall GetRenderTargetData(IDirect3DSurface9* pRenderTarget, IDirect3DSurface9* pDestSurface) { assert(false); return 0; }
long __stdcall GetFrontBufferData(unsigned int iSwapChain, IDirect3DSurface9* pDestSurface) { assert(false); return 0; }
long __stdcall StretchRect(IDirect3DSurface9* pSourceSurface, const tagRECT* pSourceRect, IDirect3DSurface9* pDestSurface, const tagRECT* pDestRect, unsigned long Filter) { assert(false); return 0; }
long __stdcall ColorFill(IDirect3DSurface9* pSurface, const tagRECT* pRect, unsigned long color) { assert(false); return 0; }
long __stdcall CreateOffscreenPlainSurface(unsigned int Width, unsigned int Height, unsigned long Format, unsigned long Pool, IDirect3DSurface9** ppSurface, void** pSharedHandle) { assert(false); return 0; }
long __stdcall SetRenderTarget(unsigned long RenderTargetIndex, IDirect3DSurface9* pRenderTarget) { assert(false); return 0; }
long __stdcall GetRenderTarget(unsigned long RenderTargetIndex, IDirect3DSurface9** ppRenderTarget) { assert(false); return 0; }
long __stdcall SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil) { assert(false); return 0; }
long __stdcall GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface) { assert(false); return 0; }
long __stdcall BeginScene() { assert(false); return 0; }
long __stdcall EndScene() { assert(false); return 0; }
long __stdcall Clear(unsigned long Count, const _D3DRECT* pRects, unsigned long Flags, unsigned long Color, float Z, unsigned long Stencil) { assert(false); return 0; }
long __stdcall SetTransform(unsigned long State, const _D3DMATRIX* pMatrix) { assert(false); return 0; }
long __stdcall GetTransform(unsigned long State, _D3DMATRIX* pMatrix) { assert(false); return 0; }
long __stdcall MultiplyTransform(unsigned long, const _D3DMATRIX*) { assert(false); return 0; }
long __stdcall SetViewport(const _D3DVIEWPORT9* pViewport) { assert(false); return 0; }
long __stdcall GetViewport(_D3DVIEWPORT9* pViewport) { assert(false); return 0; }
long __stdcall SetMaterial(const _D3DMATERIAL9* pMaterial) { assert(false); return 0; }
long __stdcall GetMaterial(_D3DMATERIAL9* pMaterial) { assert(false); return 0; }
long __stdcall SetLight(unsigned long Index, const _D3DLIGHT9*) { assert(false); return 0; }
long __stdcall GetLight(unsigned long Index, _D3DLIGHT9*) { assert(false); return 0; }
long __stdcall LightEnable(unsigned long Index, int Enable) { assert(false); return 0; }
long __stdcall GetLightEnable(unsigned long Index, int* pEnable) { assert(false); return 0; }
long __stdcall SetClipPlane(unsigned long Index, const float* pPlane) { assert(false); return 0; }
long __stdcall GetClipPlane(unsigned long Index, float* pPlane) { assert(false); return 0; }
long __stdcall SetRenderState(unsigned long State, unsigned long Value) { assert(false); return 0; }
long __stdcall GetRenderState(unsigned long State, unsigned long* pValue) { assert(false); return 0; }
long __stdcall CreateStateBlock(unsigned long Type, IDirect3DStateBlock9** ppSB) { assert(false); return 0; }
long __stdcall BeginStateBlock() { assert(false); return 0; }
long __stdcall EndStateBlock(IDirect3DStateBlock9** ppSB) { assert(false); return 0; }
long __stdcall SetClipStatus(const _D3DCLIPSTATUS9* pClipStatus) { assert(false); return 0; }
long __stdcall GetClipStatus(_D3DCLIPSTATUS9* pClipStatus) { assert(false); return 0; }
long __stdcall GetTexture(unsigned long Stage, IDirect3DBaseTexture9** ppTexture) { assert(false); return 0; }
long __stdcall SetTexture(unsigned long Stage, IDirect3DBaseTexture9* pTexture) { assert(false); return 0; }
long __stdcall GetTextureStageState(unsigned long Stage, unsigned long Type, unsigned long* pValue) { assert(false); return 0; }
long __stdcall SetTextureStageState(unsigned long Stage, unsigned long Type, unsigned long Value) { assert(false); return 0; }
long __stdcall GetSamplerState(unsigned long Sampler, unsigned long Type, unsigned long* pValue) { assert(false); return 0; }
long __stdcall SetSamplerState(unsigned long Sampler, unsigned long Type, unsigned long Value) { assert(false); return 0; }
long __stdcall ValidateDevice(unsigned long* pNumPasses) { assert(false); return 0; }
long __stdcall SetPaletteEntries(unsigned int PaletteNumber, const tagPALETTEENTRY* pEntries) { assert(false); return 0; }
long __stdcall GetPaletteEntries(unsigned int PaletteNumber, tagPALETTEENTRY* pEntries) { assert(false); return 0; }
long __stdcall SetCurrentTexturePalette(unsigned int PaletteNumber) { assert(false); return 0; }
long __stdcall GetCurrentTexturePalette(unsigned int* PaletteNumber) { assert(false); return 0; }
long __stdcall SetScissorRect(const tagRECT* pRect) { assert(false); return 0; }
long __stdcall GetScissorRect(tagRECT* pRect) { assert(false); return 0; }
long __stdcall SetSoftwareVertexProcessing(int bSoftware) { assert(false); return 0; }
int __stdcall GetSoftwareVertexProcessing() { assert(false); return 0; }
long __stdcall SetNPatchMode(float nSegments) { assert(false); return 0; }
float __stdcall GetNPatchMode() { assert(false); return 0; }
long __stdcall DrawPrimitive(unsigned long PrimitiveType, unsigned int StartVertex, unsigned int PrimitiveCount) { assert(false); return 0; }
long __stdcall DrawIndexedPrimitive(unsigned long PrimitiveType, int BaseVertexIndex, unsigned int MinVertexIndex, unsigned int NumVertices, unsigned int startIndex, unsigned int primCount) { assert(false); return 0; }
long __stdcall DrawPrimitiveUP(unsigned long PrimitiveType, unsigned int PrimitiveCount, const void* pVertexStreamZeroData, unsigned int VertexStreamZeroStride) { assert(false); return 0; }
long __stdcall DrawIndexedPrimitiveUP(unsigned long PrimitiveType, unsigned int MinVertexIndex, unsigned int NumVertices, unsigned int PrimitiveCount, const void* pIndexData, unsigned long IndexDataFormat, const void* pVertexStreamZeroData, unsigned int VertexStreamZeroStride) { assert(false); return 0; }
long __stdcall ProcessVertices(unsigned int SrcStartIndex, unsigned int DestIndex, unsigned int VertexCount, IDirect3DVertexBuffer9* pDestBuffer, IDirect3DVertexDeclaration9* pVertexDecl, unsigned long Flags) { assert(false); return 0; }
long __stdcall CreateVertexDeclaration(const _D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl) { assert(false); return 0; }
long __stdcall SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl) { assert(false); return 0; }
long __stdcall GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl) { assert(false); return 0; }
long __stdcall SetFVF(unsigned long FVF) { assert(false); return 0; }
long __stdcall GetFVF(unsigned long* pFVF) { assert(false); return 0; }
long __stdcall CreateVertexShader(const unsigned long* pFunction, IDirect3DVertexShader9** ppShader) { assert(false); return 0; }
long __stdcall SetVertexShader(IDirect3DVertexShader9* pShader) { assert(false); return 0; }
long __stdcall GetVertexShader(IDirect3DVertexShader9** ppShader) { assert(false); return 0; }
long __stdcall SetVertexShaderConstantF(unsigned int StartRegister, const float* pConstantData, unsigned int Vector4fCount) { assert(false); return 0; }
long __stdcall GetVertexShaderConstantF(unsigned int StartRegister, float* pConstantData, unsigned int Vector4fCount) { assert(false); return 0; }
long __stdcall SetVertexShaderConstantI(unsigned int StartRegister, const int* pConstantData, unsigned int Vector4iCount) { assert(false); return 0; }
long __stdcall GetVertexShaderConstantI(unsigned int StartRegister, int* pConstantData, unsigned int Vector4iCount) { assert(false); return 0; }
long __stdcall SetVertexShaderConstantB(unsigned int StartRegister, const int* pConstantData, unsigned int BoolCount) { assert(false); return 0; }
long __stdcall GetVertexShaderConstantB(unsigned int StartRegister, int* pConstantData, unsigned int BoolCount) { assert(false); return 0; }
long __stdcall SetStreamSource(unsigned int StreamNumber, IDirect3DVertexBuffer9* pStreamData, unsigned int OffsetInBytes, unsigned int Stride) { ++Calls; Last=pStreamData?pStreamData->Owner:0;
 LastStream=StreamNumber;LastOffset=OffsetInBytes;LastStride=Stride;LastBuffer=pStreamData; return 0; }
long __stdcall GetStreamSource(unsigned int StreamNumber, IDirect3DVertexBuffer9** ppStreamData, unsigned int* pOffsetInBytes, unsigned int* pStride) { assert(false); return 0; }
long __stdcall SetStreamSourceFreq(unsigned int StreamNumber, unsigned int Setting) { assert(false); return 0; }
long __stdcall GetStreamSourceFreq(unsigned int StreamNumber, unsigned int* pSetting) { assert(false); return 0; }
long __stdcall SetIndices(IDirect3DIndexBuffer9* pIndexData) { assert(false); return 0; }
long __stdcall GetIndices(IDirect3DIndexBuffer9** ppIndexData) { assert(false); return 0; }
long __stdcall CreatePixelShader(const unsigned long* pFunction, IDirect3DPixelShader9** ppShader) { assert(false); return 0; }
long __stdcall SetPixelShader(IDirect3DPixelShader9* pShader) { assert(false); return 0; }
long __stdcall GetPixelShader(IDirect3DPixelShader9** ppShader) { assert(false); return 0; }
long __stdcall SetPixelShaderConstantF(unsigned int StartRegister, const float* pConstantData, unsigned int Vector4fCount) { assert(false); return 0; }
long __stdcall GetPixelShaderConstantF(unsigned int StartRegister, float* pConstantData, unsigned int Vector4fCount) { assert(false); return 0; }
long __stdcall SetPixelShaderConstantI(unsigned int StartRegister, const int* pConstantData, unsigned int Vector4iCount) { assert(false); return 0; }
long __stdcall GetPixelShaderConstantI(unsigned int StartRegister, int* pConstantData, unsigned int Vector4iCount) { assert(false); return 0; }
long __stdcall SetPixelShaderConstantB(unsigned int StartRegister, const int* pConstantData, unsigned int BoolCount) { assert(false); return 0; }
long __stdcall GetPixelShaderConstantB(unsigned int StartRegister, int* pConstantData, unsigned int BoolCount) { assert(false); return 0; }
long __stdcall DrawRectPatch(unsigned int Handle, const float* pNumSegs, const _D3DRECTPATCH_INFO* pRectPatchInfo) { assert(false); return 0; }
long __stdcall DrawTriPatch(unsigned int Handle, const float* pNumSegs, const _D3DTRIPATCH_INFO* pTriPatchInfo) { assert(false); return 0; }
long __stdcall DeletePatch(unsigned int Handle) { assert(false); return 0; }
long __stdcall CreateQuery(unsigned long Type, IDirect3DQuery9** ppQuery) { assert(false); return 0; }
};
struct FixtureShaderManager : CShaderRenderManager {
 MockDevice Device;
 FixtureShaderManager(){D3DDevice=&Device;}
};
namespace NEngineInternal { CShaderRenderManager* GPShaderRenderManager; }
static FixtureShaderManager ManagerA,ManagerB;
struct BufferState { CVertexBufferWin32* Buffer; unsigned char* Bytes; IDirect3DVertexBuffer9 Handle; };
static BufferState BufferStates[64];
static BufferState& State(CVertexBufferWin32* b) {
 for(int i=0;i<64;++i)if(BufferStates[i].Buffer==b)return BufferStates[i];
 assert(false);return BufferStates[0];
}
static void Attach(CVertexBufferWin32* b) {
 for(int i=0;i<64;++i)if(!BufferStates[i].Buffer){
  BufferStates[i].Buffer=b;BufferStates[i].Bytes=0;BufferStates[i].Handle.Owner=b;
  b->PVertexBuffer=&BufferStates[i].Handle;b->Stride=32;++CreatedBuffers;++LiveBuffers;return;
 }
 assert(false);
}
static CVertexBufferWin32* NewBuffer(){CVertexBufferWin32* b=new CVertexBufferWin32;Attach(b);return b;}
static void MarkPreexisting(CVertexBufferWin32* b){assert(b->PVertexBuffer);}
static void StartVertexLifetimes(unsigned char* bytes,long count,long stride){for(long i=0;i<count;++i)new(bytes+i*stride)CTVertexNormTex1;}
bool CVertexBufferWin32::Initialise(EVertexType type,long count,long stride,unsigned long usage,EVertexBufferPool pool){
 assert(type==VERTEX_NULL&&count>=2&&count<=2048&&stride==32&&usage==0&&pool==VB_POOL_MANAGED);
 Attach(this);++InitialiseCalls;Length=(unsigned long)count;Type=type;Stride=(unsigned char)stride;
 BufferState& st=State(this);st.Bytes=(unsigned char*)malloc((size_t)count*(size_t)stride);assert(st.Bytes);
 memset(st.Bytes,0xA5,(size_t)count*(size_t)stride);StartVertexLifetimes(st.Bytes,count,stride);
 if(SwitchManagerOnInitialise)NEngineInternal::GPShaderRenderManager=&ManagerB;
 return true;
}
void CVertexBufferWin32::Uninitialise(){
 for(int i=0;i<64;++i)if(BufferStates[i].Buffer==this){free(BufferStates[i].Bytes);BufferStates[i].Buffer=0;--LiveBuffers;++DestroyedBuffers;return;}
}
unsigned char* CVertexBufferBase::Lock(unsigned long a,unsigned long b,unsigned long c){return DoLock(a,b,c);}
void CVertexBufferBase::Unlock(){DoUnlock();}
unsigned char* CVertexBufferWin32::DoLock(unsigned long a,unsigned long b,unsigned long c){assert(a==0&&b==0&&c==0);++LockCalls;assert(State(this).Bytes);return State(this).Bytes;}
void CVertexBufferWin32::DoUnlock(){++UnlockCalls;}
bool CVertexBufferWin32::DoIsBusy()const{return false;}
unsigned long CVertexBufferWin32::DoSizeof()const{return 0;}
float GFastCosTable[1025];
typedef std::pair<unsigned long,CCountedPointer<CVertexBufferWin32> > Pair;
typedef std::vector<Pair> BufferVector;
struct FixtureVector:BufferVector {
 void Overflow(Pair* p,const Pair& x,unsigned int n,bool end){_M_insert_overflow(p,x,std::__false_type(),n,end);}
};
static unsigned int Bits(float v){unsigned int b;memcpy(&b,&v,4);return b;}
static long RefNearest(float v){double d=floor((double)v);double f=(double)v-d;long result=(long)d;if(f>0.5||(f==0.5&&(result&1)))++result;return result;}
static float RefTrig(float angle,bool sine){float pos=angle*1024.0f;long n=RefNearest(pos-0.5f);float f=pos-(float)n;unsigned int i=(unsigned int)(n-(sine?256:0))&1023u;return (1.0f-f)*GFastCosTable[i]+f*GFastCosTable[i+1];}
static void BackVector(BufferVector& v,unsigned int count,unsigned int capacity){
 assert(capacity>=count&&capacity>0);assert(!v._M_start);
 v._M_start=(Pair*)malloc(sizeof(Pair)*capacity);assert(v._M_start);v._M_finish=v._M_start;v._M_end_of_storage._M_data=v._M_start+capacity;
 for(unsigned int i=0;i<count;++i){CCountedPointer<CVertexBufferWin32> p(NewBuffer());MarkPreexisting(p.Get());new(v._M_finish)Pair((i+1)*4,p);++v._M_finish;}
}
static void VerifyFan(CVertexBufferWin32* buffer,unsigned long segments){
 assert(buffer&&buffer->Length==segments+2&&buffer->Stride==32&&buffer->Type==VERTEX_NULL);
 const CTVertexNormTex1* vertices=(const CTVertexNormTex1*)State(buffer).Bytes;
 assert(Bits(vertices[0].X)==0&&Bits(vertices[0].Y)==0&&Bits(vertices[0].Z)==0);
 for(unsigned int i=12;i<32;++i)assert(State(buffer).Bytes[i]==0xA5); // center's normal/UV are not written natively
 float factor=1.0f/(float)segments;
 for(unsigned long i=0;i<=segments;++i){
  const CTVertexNormTex1& v=vertices[i+1];float angle=(float)i*factor;
  assert(fabs((double)v.X-RefTrig(angle,false))<0.000001);
  assert(fabs((double)v.Y+RefTrig(angle,true))<0.000001);
  assert(Bits(v.Z)==0&&Bits(v.NormalX)==0&&Bits(v.NormalY)==0&&Bits(v.NormalZ)==0&&Bits(v.U1)==0&&Bits(v.V1)==0);
 }
}
static void OneScenario(unsigned int oldcount,unsigned int spare,unsigned long segments,bool switch_manager){
 const int before=LiveBuffers,created=CreatedBuffers,initialised=InitialiseCalls,locked=LockCalls,unlocked=UnlockCalls;
 {
  CEngineOcclusionManager node;BackVector(node.FlareVertexBuffers,oldcount,oldcount+spare+(oldcount==0?1:0));
  bool hit=false;CVertexBufferWin32* expected=0;
  for(unsigned int i=0;i<oldcount;++i)if(node.FlareVertexBuffers[i].first==segments){hit=true;expected=node.FlareVertexBuffers[i].second.Get();}
  ManagerA.Device.Calls=0;ManagerB.Device.Calls=0;ManagerA.Device.Last=0;ManagerB.Device.Last=0;
  NEngineInternal::GPShaderRenderManager=&ManagerA;SwitchManagerOnInitialise=switch_manager;
  node.SelectFlareTestVertexBuffer(segments);
  assert(CreatedBuffers-created==(int)oldcount+(hit?0:1));
  assert(InitialiseCalls-initialised==(hit?0:1));assert(LockCalls-locked==(hit?0:1));assert(UnlockCalls-unlocked==(hit?0:1));
  CVertexBufferWin32* selected=switch_manager&&!hit?ManagerB.Device.Last:ManagerA.Device.Last;
  assert(ManagerA.Device.Calls==(switch_manager&&!hit?0:1)&&ManagerB.Device.Calls==(switch_manager&&!hit?1:0));
  if(hit)assert(selected==expected);else VerifyFan(selected,segments);
  MockDevice& device=switch_manager&&!hit?ManagerB.Device:ManagerA.Device;
  assert(device.LastStream==0&&device.LastOffset==0&&device.LastStride==selected->Stride&&device.LastBuffer==selected->PVertexBuffer);
  assert(node.FlareVertexBuffers.size()==oldcount+(hit?0:1));
  for(unsigned int i=0;i<node.FlareVertexBuffers.size();++i){
   if(i)assert(node.FlareVertexBuffers[i-1].first<node.FlareVertexBuffers[i].first);
   assert(node.FlareVertexBuffers[i].second.GetReferenceCount()==1);
   if(node.FlareVertexBuffers[i].first==segments)assert(node.FlareVertexBuffers[i].second.Get()==selected);
  }
  // Cache repeat is a hit, preserving allocation, reference counts and all bytes.
  int c=CreatedBuffers,l=LockCalls;node.SelectFlareTestVertexBuffer(segments);assert(CreatedBuffers==c&&LockCalls==l);
  for(unsigned int i=0;i<node.FlareVertexBuffers.size();++i)assert(node.FlareVertexBuffers[i].second.GetReferenceCount()==1);
 }
 assert(LiveBuffers==before);
}
typedef FixtureVector ExposedVector;
static void CheckHelpers(){
 // Lower-bound boundary/duplicate keys with real backing, including empty range.
 {
  FixtureVector v;BackVector(v,4,6);v[1].first=4;v[2].first=8;v[3].first=12;
  for(unsigned long key=0;key<=16;++key){Pair* actual=std::lower_bound(v.begin(),v.end(),key,MapperPred());Pair* ref=v.begin();while(ref!=v.end()&&ref->first<key)++ref;assert(actual==ref);}
  assert(std::lower_bound(v.begin(),v.begin(),0UL,MapperPred())==v.begin());
 }
 // Direct full Reset/assignment/deleter lifetimes, shared-owner and self assignments.
 {
  CCountedPointer<CVertexBufferWin32> a(NewBuffer()),b;
  a=a;assert(a.GetReferenceCount()==1);b=a;assert(a.GetReferenceCount()==2&&a.Get()==b.Get());b=b;assert(a.GetReferenceCount()==2);
  b.Reset(NewBuffer());assert(a.GetReferenceCount()==1&&b.GetReferenceCount()==1);a=b;assert(b.GetReferenceCount()==2);a.Reset(0);assert(b.GetReferenceCount()==1);
  CCountedPointer<CVertexBufferWin32>::DeleteData(NewBuffer());
 }
 // Full counted backward-copy body with overlap and pair assignment/destruction.
 {
  FixtureVector v;BackVector(v,5,6);Pair* finish=std::__copy_backward(v.begin(),v.begin()+4,v.end(),std::random_access_iterator_tag(),(ptrdiff_t*)0);assert(finish==v.begin()+1);
  unsigned long keys[]={4,4,8,12,16};for(int i=0;i<5;++i)assert(v[i].first==keys[i]);
 }
 // Non-POD uninitialized-copy/fill and destroy_aux use actual shared bodies.
 {
  FixtureVector v;BackVector(v,3,4);Pair* data=(Pair*)malloc(sizeof(Pair)*6);assert(data);
  Pair* p=std::__uninitialized_copy(v.begin(),v.end(),data,std::__false_type());assert(p==data+3);
  for(int i=0;i<3;++i)assert(v[i].second.GetReferenceCount()==2&&data[i].first==v[i].first);
  std::__destroy_aux(data,data+3,std::__false_type());
  p=std::__uninitialized_fill_n(data,6u,v[1],std::__false_type());assert(p==data+6);assert(v[1].second.GetReferenceCount()==7);
  std::__destroy_aux(data,data+6,std::__false_type());assert(v[1].second.GetReferenceCount()==1);free(data);
 }
 // Every overflow arm: empty/nonempty, prefix/suffix, fill0/1/many, atEnd.
 for(unsigned int count=0;count<=4;++count)for(unsigned int fill=0;fill<=3;++fill)for(unsigned int mode=0;mode<3;++mode){
  if(count==0&&fill==0)continue; // avoid source library's nullptr subtraction on an empty zero-capacity result
  ExposedVector v;BackVector(v,count,count+1);CCountedPointer<CVertexBufferWin32> x(NewBuffer());Pair value(99,x);
  unsigned int pos=mode==0?0:mode==1?count/2:count;bool end=mode==2;
  v.Overflow(v.begin()+pos,value,fill,end);assert(v.size()==count+fill);
  for(unsigned int i=0;i<pos;++i)assert(v[i].first==(i+1)*4);
  for(unsigned int i=0;i<fill;++i)assert(v[pos+i].first==99&&v[pos+i].second.Get()==x.Get());
  for(unsigned int i=pos;i<count;++i)assert(v[i+fill].first==(i+1)*4);
  assert(x.GetReferenceCount()==fill+2);
 }
 assert(LiveBuffers==0);
}
static void CheckNear(float value){
 long actual=GFFloatToLongNear(value);long portable=fable_portable::GFFloatToLongNear(value);assert(actual==portable);
 assert(actual>=-2147483647L-1L&&actual<=2147483647L);
}
int main(){
#ifdef _MSC_VER
 _controlfp(_PC_53|_RC_NEAR,_MCW_PC|_MCW_RC);
#endif
 for(unsigned int i=0;i<1025;++i)GFastCosTable[i]=(float)(i%29)*0.03125f-0.4375f; // controlled external table, no retail bytes copied
 CheckHelpers();unsigned int cases=0;
 for(unsigned int count=0;count<=5;++count)for(unsigned int spare=0;spare<=2;++spare)for(unsigned long n=1;n<=26;++n)for(unsigned int sw=0;sw<2;++sw){OneScenario(count,spare,n,sw!=0);++cases;}
 assert(cases==936);
 unsigned long extra[]={31,32,33,63,64,65,127,128,129,255,256,257,1024};
 for(unsigned int i=0;i<sizeof(extra)/sizeof(extra[0]);++i)for(unsigned int sw=0;sw<2;++sw){OneScenario(0,0,extra[i],sw!=0);++cases;}
 assert(cases==962);assert(LiveBuffers==0);
 float nearcases[]={0.0f,-0.0f,0.25f,-0.25f,0.5f,-0.5f,1.5f,-1.5f,2.5f,-2.5f,1023.5f,2147483520.0f,-2147483648.0f,2147483648.0f};
 for(unsigned int i=0;i<sizeof(nearcases)/sizeof(nearcases[0]);++i)CheckNear(nearcases[i]);
 unsigned int state=0x8b8dcd0u;for(int j=0;j<20000;++j){state=state*1664525u+1013904223u;float value;memcpy(&value,&state,4);CheckNear(value);}
 puts("00b8dcd0_TEST PASS");return 0;
}
// Test-only external operations. Unexpected engine entry points fail immediately.
CBaseClass::CBaseClass(){}
CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){}
CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CEngineComponent::CEngineComponent():GlobalPointerAddress(0),RenderEnabled(false),KeepAlive(0){}
CEngineComponent::~CEngineComponent(){}
void CEngineComponent::FrameStart(const CEngineComponentParams&){abort();}
void CEngineComponent::FrameEnd(const CEngineComponentParams&){abort();}
bool CEngineComponent::LoadNewScene(const CEngineComponentLoadNewSceneParams&){abort();return false;}
void CEngineComponent::Render(const CEngineComponentRenderParams&){abort();}
void CEngineComponent::PreLayerRender(const CEngineComponentRenderParams&){abort();}
void CEngineComponent::PostLayerRender(const CEngineComponentRenderParams&){abort();}
void CEngineComponent::WorldFrameUpdate(bool){abort();}
void CEngineComponent::PausedWorldFrameUpdate(){abort();}
CCharString CEngineComponent::GetConsoleEnableFunctionName(){abort();}
void CEngineComponent::OnReleaseDefaultPoolResources(){abort();}
bool CEngineComponent::OnRestoreDefaultPoolResources(){abort();return false;}
CEngineOcclusionManager::CEngineOcclusionManager(){}
unsigned long CEngineOcclusionManager::PeekSceneFilterFlags()const{abort();return 0;}
void CEngineOcclusionManager::FrameStart(const CEngineComponentParams&){abort();}
void CEngineOcclusionManager::FrameEnd(const CEngineComponentParams&){abort();}
CCharString CShaderBase::GetDebugName()const{abort();}
CVertexShader::CVertexShader():Data(0){}
CCharString CVertexShader::GetDebugName()const{abort();}
unsigned long CVertexShader::CountReferences(){abort();return 0;}
CPixelShader::CPixelShader():Data(0){}
CCharString CPixelShader::GetDebugName()const{abort();}
unsigned long CPixelShader::CountReferences(){abort();return 0;}
CIndexBuffer::CIndexBuffer(){}
void CIndexBuffer::Uninitialise(){}
void CIndexBuffer::RelocateData(long,void*,void*,long){abort();}
bool CMovableResource::CanRelocateData(long)const{abort();return false;}
unsigned long CMovableResource::Verify()const{abort();return 0;}
CShaderRenderManager::CShaderRenderManager():LightPool(0,0),D3DDevice(0){}
CShaderRenderManager::~CShaderRenderManager(){}
CSimpleMemoryPool::CSimpleMemoryPool(unsigned long capacity,unsigned long growthGranularity):FirstMemoryBlock(0),Capacity(capacity),GrowthGranularity(growthGranularity){memset(&Root,0,sizeof(Root));}
void CSimpleMemoryPool::ClearMemoryBlocks(){assert(!FirstMemoryBlock);}
CVertexShaderConstantLayout::CVertexShaderConstantLayout(){}
CVertexShaderConstantLayout::~CVertexShaderConstantLayout(){}
CCharString CVertexShaderConstantLayout::PeekName()const{abort();}
CShaderConstantRange::CShaderConstantRange():Offset(0),Count(0){}
CShaderConstantRange::~CShaderConstantRange(){}
CCharString::~CCharString(){abort();}