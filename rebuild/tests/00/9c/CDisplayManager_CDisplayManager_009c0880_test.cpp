// Standalone C++03 fixture; the exact production constructor is pasted unchanged.
// Private/protected visibility is relaxed only in the fixture to inspect real members.
// The external CSystemManager reference is opaque: the constructor stores its address and never reads it.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <new>
#if !defined(_MSC_VER)
#define __declspec(x)
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __stdcall
#define long int
#define FABLE_STATIC_ASSERT(x) typedef char fixture_assert_##__LINE__[1]
#else
#define FABLE_STATIC_ASSERT(x) typedef char fixture_assert_##__LINE__[(x)?1:-1]
#endif
static void* live_blocks[32];static unsigned int live_count,allocated,freed;
static void* fixture_malloc(size_t n){void* p=malloc(n);assert(p && live_count<32);memset(p,0xcc,n);live_blocks[live_count++]=p;++allocated;return p;}
static void fixture_free(void* p){if(!p)return;unsigned int i=0;while(i<live_count && live_blocks[i]!=p)++i;assert(i<live_count);live_blocks[i]=live_blocks[--live_count];++freed;free(p);}
#define malloc fixture_malloc
#define free fixture_free
#define private public
#define protected public
// CDisplayManager::CDisplayManager(CSystemManager&) -- initialise device resources and supported surface formats.
// Leaf: ??0CDisplayManager@@QAE@AAVCSystemManager@@@Z
#include <stdlib.h>
#include <new>
namespace std {
struct __false_type {};
template<class T> class allocator {
public:
 allocator(){} allocator(const allocator&){}
 template<class U> allocator(const allocator<U>&){}
 T* allocate(unsigned int n,const void* =0){return n?(T*)malloc(n*sizeof(T)):0;}
 void deallocate(T* p,unsigned int){if(p)free(p);}
};
template<class Value,class T,class Alloc> class _STLP_alloc_proxy:public Alloc {
public: Value _M_data;
 _STLP_alloc_proxy(const Alloc& a,Value p):Alloc(a),_M_data(p){}
};
struct _List_node_base {_List_node_base* _M_next;_List_node_base* _M_prev;};
template<class T> struct _List_node:public _List_node_base {T _M_data;};
template<class T,class Alloc> class _List_base {
protected:typedef _List_node<T> _Node;
public:
 _List_base(const Alloc& a):_M_node(allocator<_Node>(a),(_Node*)0){
  _Node* n=_M_node.allocate(1);n->_M_next=n;n->_M_prev=n;_M_node._M_data=n;
 }
 _STLP_alloc_proxy<_Node*,_Node,allocator<_Node> > _M_node;
};
template<class T,class Alloc=allocator<T> > class list:public _List_base<T,Alloc> {
public:explicit list(const Alloc& a=Alloc()):_List_base<T,Alloc>(a){}
};
template<class T,class U>inline void _Construct(T* p,const U& value){new(p)T(value);}
template<class T>inline void _Destroy(T* p){p->~T();}
template<class It>inline void _Destroy(It first,It last){for(;first!=last;++first)_Destroy(&*first);}
template<class In,class Out>inline Out __uninitialized_copy(In first,In last,Out dest,const __false_type&){
 Out p=dest;for(;first!=last;++first,++p)_Construct(&*p,*first);return p;
}
template<class Out,class Size,class T>inline Out __uninitialized_fill_n(Out first,Size count,const T& value,const __false_type&){
 Out p=first;for(;count>0;--count,++p)_Construct(&*p,value);return p;
}
template<class T>inline const T& (max)(const T& a,const T& b){return a<b?b:a;}
template<class T,class Alloc>class _Vector_base {
public:_Vector_base(const Alloc& a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}
protected:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,Alloc> _M_end_of_storage;
};
template<class T,class Alloc=allocator<T> >class vector:public _Vector_base<T,Alloc> {
public:
 explicit vector(const Alloc& a=Alloc()):_Vector_base<T,Alloc>(a){}
 unsigned int size()const{return(unsigned int)(this->_M_finish-this->_M_start);}
 T* begin(){return this->_M_start;}T* end(){return this->_M_finish;}
 void push_back(const T& x){
  if(this->_M_finish!=this->_M_end_of_storage._M_data){_Construct(this->_M_finish,x);++this->_M_finish;}
  else _M_insert_overflow(this->_M_finish,x,__false_type(),1,true);
 }
protected:
 __declspec(noinline) void _M_insert_overflow(T* position,const T& x,const __false_type&,unsigned int fill_len,bool at_end=false);
 void _M_clear(){_Destroy(this->_M_start,this->_M_finish);this->_M_end_of_storage.deallocate(this->_M_start,(unsigned int)(this->_M_end_of_storage._M_data-this->_M_start));}
 void _M_set(T* s,T* f,T* e){this->_M_start=s;this->_M_finish=f;this->_M_end_of_storage._M_data=e;}
};
template<class T,class Alloc>
__declspec(noinline)void vector<T,Alloc>::_M_insert_overflow(T* position,const T& x,const __false_type&,unsigned int fill_len,bool at_end){
 const unsigned int old_size=size();const unsigned int length=old_size+(max)(old_size,fill_len);
 T* new_start=this->_M_end_of_storage.allocate(length);T* new_finish=new_start;
 new_finish=__uninitialized_copy(this->_M_start,position,new_start,__false_type());
 if(fill_len==1){_Construct(new_finish,x);++new_finish;}
 else new_finish=__uninitialized_fill_n(new_finish,fill_len,x,__false_type());
 if(!at_end)new_finish=__uninitialized_copy(position,this->_M_finish,new_finish,__false_type());
 _M_clear();_M_set(new_start,new_finish,new_start+length);
}
}
// Retail layout (hand-written; no members). RTTI CBaseClassNonCopyable : CBaseClass; vtable 0x0129A7C4, slot 0 scalar
// deleting dtor 0x0099A470. Ego_r.pdb / FableWin.pdb: ??0CBaseClassNonCopyable@@IAE@XZ (protected),
// ??1CBaseClassNonCopyable@@MAE@XZ (protected virtual). The copy operations are private and undefined (non-copyable).
// Retail layout (hand-written; Ego_r.pdb has no members): the root polymorphic base. RTTI .?AVCBaseClass@@ (no bases);
// vtable 0x01231710, slot 0 scalar deleting dtor 0x004371F0, which calls ~CBaseClass 0x0099A2E0 (one vtable store).
// The default ctor is out of line (0x0099A2D0: `mov eax,ecx; mov [eax],0x01231710; ret`): Ego_r.pdb places
// ??0CBaseClass@@QAE@XZ / ??1CBaseClass@@UAE@XZ side by side (0x00B01F20 / 0x00B01F30, one .cpp) and FableWin.pdb does the
// same (0x03044700 / 0x03044720), and 1,225 landed rows call 0x0099A2D0. The copy ctor is the implicit one: FableWin.pdb
// emits ??0CBaseClass@@QAE@ABV0@@Z beside the compiler-generated ??_E (0x018CD7E0), and retail expands it inline (a bare
// vtable store in a derived copy ctor, landed chunk 577 0x00DCEDF0 / chunk 578 0x00F05F30: write `: CBaseClass(other)`).

class CBaseClass {
public:
    CBaseClass();                                                        // 0x0099A2D0
    virtual ~CBaseClass();                                               // slot 0  0x0099A2E0
};
FABLE_STATIC_ASSERT(sizeof(CBaseClass) == 0x4);


class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();
    virtual ~CBaseClassNonCopyable();                                    // slot 0
private:
    CBaseClassNonCopyable(const CBaseClassNonCopyable&);
    CBaseClassNonCopyable& operator=(const CBaseClassNonCopyable&);
};
FABLE_STATIC_ASSERT(sizeof(CBaseClassNonCopyable) == 0x4);

namespace boost {
template<class T> class scoped_ptr {
public: explicit scoped_ptr(T* p=0):ptr(p) {} ~scoped_ptr();
private: T* ptr;
};
}
struct IDirect3DSurface9;
// Layout: original size16; real virtual destructor, fields4/8/12. Last pointer is not initialized by default ctor.
class CSurface {
public:
 CSurface() :PD3DSurface(0),AllocationSource(0) {}
 virtual ~CSurface();
 void Uninitialise();
private:
 IDirect3DSurface9* PD3DSurface;
 unsigned long AllocationSource;
 void* PAllocatedMemory;
};
struct IDirect3DTexture9;
// Layout: original private texture pointer and two packed bitfields within the existing dword.
class CTexture {
public:
 CTexture() :PD3DTexture(0),ByteLength(0),AllocationSource(0) {}
 ~CTexture();
 void Uninitialise();
private:
 IDirect3DTexture9* PD3DTexture;
 unsigned long ByteLength:28;
 unsigned long AllocationSource:4;
};
// Selected original Direct3D9 SDK declarations, independently checked against Ego_r TPI.
// Copyright (C) Microsoft Corporation. All Rights Reserved.
typedef unsigned long DWORD;
typedef unsigned int UINT;
typedef int INT;
typedef int BOOL;
struct HWND__;typedef HWND__* HWND;
typedef enum _D3DDEVTYPE
{
    D3DDEVTYPE_HAL         = 1,
    D3DDEVTYPE_REF         = 2,
    D3DDEVTYPE_SW          = 3,

    D3DDEVTYPE_NULLREF     = 4,

    D3DDEVTYPE_FORCE_DWORD  = 0x7fffffff
} D3DDEVTYPE;
typedef enum _D3DMULTISAMPLE_TYPE
{
    D3DMULTISAMPLE_NONE            =  0,
    D3DMULTISAMPLE_NONMASKABLE     =  1,
    D3DMULTISAMPLE_2_SAMPLES       =  2,
    D3DMULTISAMPLE_3_SAMPLES       =  3,
    D3DMULTISAMPLE_4_SAMPLES       =  4,
    D3DMULTISAMPLE_5_SAMPLES       =  5,
    D3DMULTISAMPLE_6_SAMPLES       =  6,
    D3DMULTISAMPLE_7_SAMPLES       =  7,
    D3DMULTISAMPLE_8_SAMPLES       =  8,
    D3DMULTISAMPLE_9_SAMPLES       =  9,
    D3DMULTISAMPLE_10_SAMPLES      = 10,
    D3DMULTISAMPLE_11_SAMPLES      = 11,
    D3DMULTISAMPLE_12_SAMPLES      = 12,
    D3DMULTISAMPLE_13_SAMPLES      = 13,
    D3DMULTISAMPLE_14_SAMPLES      = 14,
    D3DMULTISAMPLE_15_SAMPLES      = 15,
    D3DMULTISAMPLE_16_SAMPLES      = 16,

    D3DMULTISAMPLE_FORCE_DWORD     = 0x7fffffff
} D3DMULTISAMPLE_TYPE;
typedef enum _D3DSWAPEFFECT
{
    D3DSWAPEFFECT_DISCARD           = 1,
    D3DSWAPEFFECT_FLIP              = 2,
    D3DSWAPEFFECT_COPY              = 3,

/* D3D9Ex only -- */
    D3DSWAPEFFECT_OVERLAY           = 4,
    D3DSWAPEFFECT_FLIPEX            = 5,
/* -- D3D9Ex only */

    D3DSWAPEFFECT_FORCE_DWORD       = 0x7fffffff
} D3DSWAPEFFECT;
enum _D3DFORMAT {
 D3DFMT_UNKNOWN=0,
 D3DFMT_R8G8B8=20,
 D3DFMT_A8R8G8B8=21,
 D3DFMT_X8R8G8B8=22,
 D3DFMT_R5G6B5=23,
 D3DFMT_X1R5G5B5=24,
 D3DFMT_A1R5G5B5=25,
 D3DFMT_A4R4G4B4=26,
 D3DFMT_R3G3B2=27,
 D3DFMT_A8=28,
 D3DFMT_A8R3G3B2=29,
 D3DFMT_X4R4G4B4=30,
 D3DFMT_A2B10G10R10=31,
 D3DFMT_A8B8G8R8=32,
 D3DFMT_X8B8G8R8=33,
 D3DFMT_G16R16=34,
 D3DFMT_A2R10G10B10=35,
 D3DFMT_A16B16G16R16=36,
 D3DFMT_A8P8=40,
 D3DFMT_P8=41,
 D3DFMT_L8=50,
 D3DFMT_A8L8=51,
 D3DFMT_A4L4=52,
 D3DFMT_V8U8=60,
 D3DFMT_L6V5U5=61,
 D3DFMT_X8L8V8U8=62,
 D3DFMT_Q8W8V8U8=63,
 D3DFMT_V16U16=64,
 D3DFMT_A2W10V10U10=67,
 D3DFMT_UYVY=1498831189,
 D3DFMT_R8G8_B8G8=1195525970,
 D3DFMT_YUY2=844715353,
 D3DFMT_G8R8_G8B8=1111970375,
 D3DFMT_DXT1=827611204,
 D3DFMT_DXT2=844388420,
 D3DFMT_DXT3=861165636,
 D3DFMT_DXT4=877942852,
 D3DFMT_DXT5=894720068,
 D3DFMT_D16_LOCKABLE=70,
 D3DFMT_D32=71,
 D3DFMT_D15S1=73,
 D3DFMT_D24S8=75,
 D3DFMT_D24X8=77,
 D3DFMT_D24X4S4=79,
 D3DFMT_D16=80,
 D3DFMT_D32F_LOCKABLE=82,
 D3DFMT_D24FS8=83,
 D3DFMT_L16=81,
 D3DFMT_VERTEXDATA=100,
 D3DFMT_INDEX16=101,
 D3DFMT_INDEX32=102,
 D3DFMT_Q16W16V16U16=110,
 D3DFMT_MULTI2_ARGB8=827606349,
 D3DFMT_R16F=111,
 D3DFMT_G16R16F=112,
 D3DFMT_A16B16G16R16F=113,
 D3DFMT_R32F=114,
 D3DFMT_G32R32F=115,
 D3DFMT_A32B32G32R32F=116,
 D3DFMT_CxV8U8=117,
 D3DFMT_FORCE_DWORD=2147483647
};
typedef _D3DFORMAT D3DFORMAT;
typedef struct _D3DVSHADERCAPS2_0
{
        DWORD Caps;
        INT DynamicFlowControlDepth;
        INT NumTemps;
        INT StaticFlowControlDepth;
} D3DVSHADERCAPS2_0;
typedef struct _D3DPSHADERCAPS2_0
{
    DWORD Caps;
    INT DynamicFlowControlDepth;
    INT NumTemps;
    INT StaticFlowControlDepth;
    INT NumInstructionSlots;
} D3DPSHADERCAPS2_0;
typedef struct _D3DCAPS9
{
    /* Device Info */
    D3DDEVTYPE  DeviceType;
    UINT        AdapterOrdinal;

    /* Caps from DX7 Draw */
    DWORD   Caps;
    DWORD   Caps2;
    DWORD   Caps3;
    DWORD   PresentationIntervals;

    /* Cursor Caps */
    DWORD   CursorCaps;

    /* 3D Device Caps */
    DWORD   DevCaps;

    DWORD   PrimitiveMiscCaps;
    DWORD   RasterCaps;
    DWORD   ZCmpCaps;
    DWORD   SrcBlendCaps;
    DWORD   DestBlendCaps;
    DWORD   AlphaCmpCaps;
    DWORD   ShadeCaps;
    DWORD   TextureCaps;
    DWORD   TextureFilterCaps;          // D3DPTFILTERCAPS for IDirect3DTexture9's
    DWORD   CubeTextureFilterCaps;      // D3DPTFILTERCAPS for IDirect3DCubeTexture9's
    DWORD   VolumeTextureFilterCaps;    // D3DPTFILTERCAPS for IDirect3DVolumeTexture9's
    DWORD   TextureAddressCaps;         // D3DPTADDRESSCAPS for IDirect3DTexture9's
    DWORD   VolumeTextureAddressCaps;   // D3DPTADDRESSCAPS for IDirect3DVolumeTexture9's

    DWORD   LineCaps;                   // D3DLINECAPS

    DWORD   MaxTextureWidth, MaxTextureHeight;
    DWORD   MaxVolumeExtent;

    DWORD   MaxTextureRepeat;
    DWORD   MaxTextureAspectRatio;
    DWORD   MaxAnisotropy;
    float   MaxVertexW;

    float   GuardBandLeft;
    float   GuardBandTop;
    float   GuardBandRight;
    float   GuardBandBottom;

    float   ExtentsAdjust;
    DWORD   StencilCaps;

    DWORD   FVFCaps;
    DWORD   TextureOpCaps;
    DWORD   MaxTextureBlendStages;
    DWORD   MaxSimultaneousTextures;

    DWORD   VertexProcessingCaps;
    DWORD   MaxActiveLights;
    DWORD   MaxUserClipPlanes;
    DWORD   MaxVertexBlendMatrices;
    DWORD   MaxVertexBlendMatrixIndex;

    float   MaxPointSize;

    DWORD   MaxPrimitiveCount;          // max number of primitives per DrawPrimitive call
    DWORD   MaxVertexIndex;
    DWORD   MaxStreams;
    DWORD   MaxStreamStride;            // max stride for SetStreamSource

    DWORD   VertexShaderVersion;
    DWORD   MaxVertexShaderConst;       // number of vertex shader constant registers

    DWORD   PixelShaderVersion;
    float   PixelShader1xMaxValue;      // max value storable in registers of ps.1.x shaders

    // Here are the DX9 specific ones
    DWORD   DevCaps2;

    float   MaxNpatchTessellationLevel;
    DWORD   Reserved5;

    UINT    MasterAdapterOrdinal;       // ordinal of master adaptor for adapter group
    UINT    AdapterOrdinalInGroup;      // ordinal inside the adapter group
    UINT    NumberOfAdaptersInGroup;    // number of adapters in this adapter group (only if master)
    DWORD   DeclTypes;                  // Data types, supported in vertex declarations
    DWORD   NumSimultaneousRTs;         // Will be at least 1
    DWORD   StretchRectFilterCaps;      // Filter caps supported by StretchRect
    D3DVSHADERCAPS2_0 VS20Caps;
    D3DPSHADERCAPS2_0 PS20Caps;
    DWORD   VertexTextureFilterCaps;    // D3DPTFILTERCAPS for IDirect3DTexture9's for texture, used in vertex shaders
    DWORD   MaxVShaderInstructionsExecuted; // maximum number of vertex shader instructions that can be executed
    DWORD   MaxPShaderInstructionsExecuted; // maximum number of pixel shader instructions that can be executed
    DWORD   MaxVertexShader30InstructionSlots; 
    DWORD   MaxPixelShader30InstructionSlots;
} D3DCAPS9;
typedef struct _D3DDISPLAYMODE
{
    UINT            Width;
    UINT            Height;
    UINT            RefreshRate;
    D3DFORMAT       Format;
} D3DDISPLAYMODE;
typedef struct _D3DVIEWPORT9 {
    DWORD       X;
    DWORD       Y;            /* Viewport Top left */
    DWORD       Width;
    DWORD       Height;       /* Viewport Dimensions */
    float       MinZ;         /* Min/max of clip Volume */
    float       MaxZ;
} D3DVIEWPORT9;
typedef struct _D3DPRESENT_PARAMETERS_
{
    UINT                BackBufferWidth;
    UINT                BackBufferHeight;
    D3DFORMAT           BackBufferFormat;
    UINT                BackBufferCount;

    D3DMULTISAMPLE_TYPE MultiSampleType;
    DWORD               MultiSampleQuality;

    D3DSWAPEFFECT       SwapEffect;
    HWND                hDeviceWindow;
    BOOL                Windowed;
    BOOL                EnableAutoDepthStencil;
    D3DFORMAT           AutoDepthStencilFormat;
    DWORD               Flags;

    /* FullScreen_RefreshRateInHz must be zero for Windowed mode */
    UINT                FullScreen_RefreshRateInHz;
    UINT                PresentationInterval;
} D3DPRESENT_PARAMETERS;

class CPixelFormat {
protected: long FormatIndex;
public:
 CPixelFormat() :FormatIndex(-1) {}
 CPixelFormat(_D3DFORMAT format) { Initialise(format); }
 void Initialise(_D3DFORMAT);
};
// Original public fields and user default constructor from Ego_r TPI.
class C2DExtentsI { public: long X,Y; C2DExtentsI() {} };
// Original public fields and user default constructor from Ego_r TPI.
class C2DBoxI { public: long TLX,TLY,BRX,BRY; C2DBoxI() {} };
// Original public fields and user default constructor from Ego_r TPI.
class C2DBoxF { public: float TLX,TLY,BRX,BRY; C2DBoxF() {} };
// Original PDB CWideString, class key V, one protected pointer at +0.
class CWideStringData;
class CWideString {
public:
    CWideString();
    ~CWideString();
protected:
    CWideStringData* PStringData;
};
enum ESurfaceMultisampleType {
 SURFACE_MULTISAMPLE_NONE=0,
 SURFACE_MULTISAMPLE_2=2,
 SURFACE_MULTISAMPLE_4=4,
 SURFACE_MULTISAMPLE_9=9,
 SURFACE_MULTISAMPLE_3=3,
 SURFACE_MULTISAMPLE_5=5,
 SURFACE_MULTISAMPLE_6=6,
 SURFACE_MULTISAMPLE_7=7,
 SURFACE_MULTISAMPLE_8=8,
 SURFACE_MULTISAMPLE_10=10,
 SURFACE_MULTISAMPLE_11=11,
 SURFACE_MULTISAMPLE_12=12,
 SURFACE_MULTISAMPLE_13=13,
 SURFACE_MULTISAMPLE_14=14,
 SURFACE_MULTISAMPLE_15=15,
 SURFACE_MULTISAMPLE_16=16
};
// Original CArray adds no fields to its public STLport vector base.
template<class T> class CArray : public std::vector<T> {};

struct IDirect3DDevice9;struct IDirect3DQuery9;
// Layout: original members with native four-byte STLport lists, size20.
class CBusyResourceManager {
 struct CPendingQuery {IDirect3DQuery9* Query;long Frame;};
public: CBusyResourceManager();~CBusyResourceManager();
private:
 std::list<CPendingQuery> FreeQueoryObjects;
 std::list<CPendingQuery> PendingQueories;
 long PendingFrame,FinishedFrame;
 IDirect3DDevice9* D3DDevice;
};
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
 virtual ~CDisplayManager();
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
CDisplayManager::CDisplayManager(CSystemManager& systemManager)
 :SystemManager(systemManager),D3D(0),D3DDevice(0),FrameCounter(0),Gamma(1.0f)
{
 BackBufferFormats.push_back(CPixelFormat(D3DFMT_A1R5G5B5));
 BackBufferFormats.push_back(CPixelFormat(D3DFMT_A2B10G10R10));
 BackBufferFormats.push_back(CPixelFormat(D3DFMT_A8R8G8B8));
 BackBufferFormats.push_back(CPixelFormat(D3DFMT_R5G6B5));
 BackBufferFormats.push_back(CPixelFormat(D3DFMT_X1R5G5B5));
 BackBufferFormats.push_back(CPixelFormat(D3DFMT_X8R8G8B8));
 MultisampleTypes.push_back(SURFACE_MULTISAMPLE_NONE);
 MultisampleTypes.push_back(SURFACE_MULTISAMPLE_2);
 MultisampleTypes.push_back(SURFACE_MULTISAMPLE_3);
 MultisampleTypes.push_back(SURFACE_MULTISAMPLE_4);
 MultisampleTypes.push_back(SURFACE_MULTISAMPLE_5);
 MultisampleTypes.push_back(SURFACE_MULTISAMPLE_6);
 MultisampleTypes.push_back(SURFACE_MULTISAMPLE_7);
 MultisampleTypes.push_back(SURFACE_MULTISAMPLE_8);
 MultisampleTypes.push_back(SURFACE_MULTISAMPLE_9);
 MultisampleTypes.push_back(SURFACE_MULTISAMPLE_10);
 MultisampleTypes.push_back(SURFACE_MULTISAMPLE_11);
 MultisampleTypes.push_back(SURFACE_MULTISAMPLE_12);
 MultisampleTypes.push_back(SURFACE_MULTISAMPLE_13);
 MultisampleTypes.push_back(SURFACE_MULTISAMPLE_14);
 MultisampleTypes.push_back(SURFACE_MULTISAMPLE_15);
 MultisampleTypes.push_back(SURFACE_MULTISAMPLE_16);
}

static int bases,strings,busy,surfaces,textures,pixel_calls;
static int fixture_seed;static const int formats[6]={25,31,21,23,24,22};
CBaseClass::CBaseClass(){} CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){++bases;}
CBaseClassNonCopyable::~CBaseClassNonCopyable(){--bases;}
CWideString::CWideString():PStringData(0){++strings;}
CWideString::~CWideString(){assert(PStringData==0);--strings;}
CSurface::~CSurface(){assert(PD3DSurface==0 && AllocationSource==0);++surfaces;}
CTexture::~CTexture(){assert(PD3DTexture==0 && ByteLength==0 && AllocationSource==0);++textures;}
CBusyResourceManager::CBusyResourceManager():PendingFrame(1),FinishedFrame(0){++busy;}
CBusyResourceManager::~CBusyResourceManager(){free(FreeQueoryObjects._M_node._M_data);free(PendingQueories._M_node._M_data);--busy;}
void CPixelFormat::Initialise(_D3DFORMAT format){assert(pixel_calls<6 && (int)format==formats[pixel_calls]);++pixel_calls;FormatIndex=(int)format*17+fixture_seed;}
namespace boost {template<class T>scoped_ptr<T>::~scoped_ptr(){assert(ptr==0);}}
CDisplayManager::~CDisplayManager(){
 free(BackBufferFormats._M_start);free(MultisampleTypes._M_start);free(DeviceResetCallbacks._M_node._M_data);
}
static void same_bytes(const void* first,const void* last,unsigned char value){
 const unsigned char* a=(const unsigned char*)first;const unsigned char* b=(const unsigned char*)last;
 for(;a!=b;++a)assert(*a==value);
}
static void list_empty(const std::_List_node_base* p){assert(p && p->_M_next==p && p->_M_prev==p);}
int main(){
 for(int iteration=0;iteration<64;++iteration){
  union Storage{double align;void* pointer;unsigned char data[sizeof(CDisplayManager)+128];}storage,context;
  unsigned char value=(unsigned char)(0x30+iteration);memset(storage.data,value,sizeof(storage.data));
  fixture_seed=iteration*3;pixel_calls=0;allocated=freed=0;surfaces=textures=0;
  CSystemManager* system=(CSystemManager*)context.data;
  CDisplayManager* p=new(storage.data+64)CDisplayManager(*system);
  assert(bases==1 && strings==1 && busy==1 && pixel_calls==6);
  assert(&p->SystemManager==system && p->PRenderManager.ptr==0 && p->VsyncCallbackTimer.ptr==0);
  assert(p->D3D==0 && p->D3DDevice==0 && p->FrameCounter==0 && p->Gamma==1.0f);
  assert(p->BackBufferFormat.FormatIndex==-1 && p->DepthBufferFormat.FormatIndex==-1);
  CSurface* surface[4]={&p->RenderTargetSurface,&p->RenderTargetDepthBuffer,&p->BackBufferSurface,&p->DepthBufferSurface};
  for(int i=0;i<4;++i){assert(surface[i]->PD3DSurface==0 && surface[i]->AllocationSource==0);same_bytes(&surface[i]->PAllocatedMemory,(unsigned char*)&surface[i]->PAllocatedMemory+sizeof(void*),value);}
  assert(p->BackBufferCopyTexture.PD3DTexture==0 && p->BackBufferCopyTexture.ByteLength==0 && p->BackBufferCopyTexture.AllocationSource==0);
  same_bytes(&p->D3DDeviceCaps,&p->BackBufferFormat,value);
  same_bytes(&p->UsingRenderManager,&p->ShaderResourcePath,value);
  same_bytes(&p->RenderManagerInitFlags,&p->DeviceResetCallbacks,value);
  same_bytes(&p->LastDeviceCreationParameters,(unsigned char*)&p->LastDeviceCreationParameters+sizeof(p->LastDeviceCreationParameters),value);
  same_bytes(&p->m_bWantHiResScreenShot,(unsigned char*)&p->m_bTakingScreenShot+1,value);
  same_bytes(&p->BusyResourceManager.D3DDevice,(unsigned char*)&p->BusyResourceManager.D3DDevice+sizeof(void*),value);
  list_empty(p->DeviceResetCallbacks._M_node._M_data);list_empty(p->BusyResourceManager.FreeQueoryObjects._M_node._M_data);list_empty(p->BusyResourceManager.PendingQueories._M_node._M_data);
  assert(p->BusyResourceManager.PendingFrame==1 && p->BusyResourceManager.FinishedFrame==0);
  assert(p->BackBufferFormats.size()==6 && p->MultisampleTypes.size()==16);
  for(int i=0;i<6;++i)assert(p->BackBufferFormats.begin()[i].FormatIndex==formats[i]*17+fixture_seed);
  for(int i=0;i<16;++i)assert((int)p->MultisampleTypes.begin()[i]==(i==0?0:i+1));
  assert(p->BackBufferFormats._M_end_of_storage._M_data-p->BackBufferFormats._M_start==8);
  assert(p->MultisampleTypes._M_end_of_storage._M_data-p->MultisampleTypes._M_start==16);
  assert(allocated==12 && freed==7 && live_count==5);
  same_bytes(storage.data,storage.data+64,value);same_bytes(storage.data+64+sizeof(CDisplayManager),storage.data+sizeof(storage.data),value);
  p->~CDisplayManager();assert(bases==0 && strings==0 && busy==0 && surfaces==4 && textures==1 && allocated==freed && live_count==0);
 }
 puts("009c0880_TEST PASS");return 0;
}