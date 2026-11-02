// Standalone C++03 fixture; exact production function pasted with selected headers.
// Opaque external receiver storage is used only by callee contracts; rendering is not executed.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <new>
#if !defined(_MSC_VER)
#define __declspec(x)
#define __fastcall
#define __cdecl
#define __stdcall
#define long int
#define FABLE_STATIC_ASSERT(x) typedef char fixture_assert_##__LINE__[1]
#else
#define FABLE_STATIC_ASSERT(x) typedef char fixture_assert_##__LINE__[(x)?1:-1]
#endif
#define FABLE_FASTCALL __fastcall
#define private public
#define protected public
// InitializeStreamingFontTextureCache(CStreamingFontDataBank*) -- initialise glyph texture cache and load bank indices.
// Leaf: ?InitializeStreamingFontTextureCache@?A0xfe46a155@@YIXPAVCStreamingFontDataBank@@@Z
// Flags: /Gr
#include <stdlib.h>
#include <new>
#include <string.h>
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
struct __true_type {};
template<class T>struct __type_traits {typedef __false_type has_trivial_destructor;};
template<>struct __type_traits<unsigned char>{typedef __true_type has_trivial_destructor;};
template<class It>inline void __destroy_aux(It first,It last,const __false_type&){for(;first!=last;++first)_Destroy(&*first);}
template<class It>inline void __destroy_aux(It,It,const __true_type&){}
template<class It,class T>inline void __destroy(It first,It last,T*){typedef typename __type_traits<T>::has_trivial_destructor Trivial;__destroy_aux(first,last,Trivial());}
struct input_iterator_tag {};struct forward_iterator_tag:public input_iterator_tag {};struct bidirectional_iterator_tag:public forward_iterator_tag {};struct random_access_iterator_tag:public bidirectional_iterator_tag {};
template<class It>struct iterator_traits {typedef typename It::value_type value_type;};
template<class T>struct iterator_traits<T*> {typedef random_access_iterator_tag iterator_category;typedef T value_type;typedef int difference_type;typedef T* pointer;typedef T& reference;};
template<class It>inline void _Destroy(It first,It last){__destroy(first,last,(typename iterator_traits<It>::value_type*)0);}
inline void* __copy_trivial(const void* first,const void* last,void* result){return last==first?result:(char*)memmove(result,first,(const char*)last-(const char*)first)+((const char*)last-(const char*)first);}
template<class T>inline T* __copy_ptrs(const T* first,const T* last,T* result,const __true_type&){return (T*)__copy_trivial(first,last,result);}
template<class T>inline T* __uninitialized_copy(const T* first,const T* last,T* result,const __true_type&){return(T*)__copy_trivial(first,last,result);}
template<class T>inline T* __copy_backward_ptrs(const T* first,const T* last,T* result,const __true_type&){unsigned int n=(unsigned int)(last-first);return(T*)memmove(result-n,first,n*sizeof(T));}
template<class T>inline void fill(T* first,T* last,const T& x){for(;first!=last;++first)*first=x;}
template<class T>inline T* uninitialized_fill_n(T* first,unsigned int n,const T& x){T* p=first;for(;n>0;--n,++p)_Construct(p,x);return p;}

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
 T& front(){return *begin();}
 void _M_fill_insert(T*,unsigned int,const T&);
 T* erase(T* first,T* last){
  T* i=__copy_ptrs(last,this->_M_finish,first,__true_type());
  _Destroy(i,this->_M_finish);this->_M_finish=i;return first;
 }
 void insert(T* position,unsigned int count,const T& value){_M_fill_insert(position,count,value);}
 // Historical STLport4.5.2 reference overload: native font fill receives the caller byte temporary.
 void resize(unsigned int __new_size,const T& __x){if(__new_size<size())erase(begin()+__new_size,end());else insert(end(),__new_size-size(),__x);}
 void resize(unsigned int __new_size){resize(__new_size,T());}
 void push_back(const T& x){
  if(this->_M_finish!=this->_M_end_of_storage._M_data){_Construct(this->_M_finish,x);++this->_M_finish;}
  else _M_insert_overflow(this->_M_finish,x,__false_type(),1,true);
 }
protected:
 void _M_insert_overflow(T*,const T&,const __true_type&,unsigned int,bool=false);
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
template<class T,class Alloc>void vector<T,Alloc>::_M_insert_overflow(T* position,const T& x,const __true_type&,unsigned int n,bool at_end){
 unsigned int old_size=size();unsigned int length=old_size+(max)(old_size,n);
 T* new_start=this->_M_end_of_storage.allocate(length);T* new_finish=(T*)__copy_trivial(this->_M_start,position,new_start);
 new_finish=uninitialized_fill_n(new_finish,n,x);
 if(!at_end)new_finish=(T*)__copy_trivial(position,this->_M_finish,new_finish);
 _M_clear();_M_set(new_start,new_finish,new_start+length);
}
template<class T,class Alloc>void vector<T,Alloc>::_M_fill_insert(T* position,unsigned int n,const T& x){
 if(n!=0){
  if((unsigned int)(this->_M_end_of_storage._M_data-this->_M_finish)>=n){
   T x_copy=x;const unsigned int elems_after=(unsigned int)(this->_M_finish-position);T* old_finish=this->_M_finish;
   if(elems_after>n){
    __uninitialized_copy(this->_M_finish-n,this->_M_finish,this->_M_finish,__true_type());
    this->_M_finish+=n;__copy_backward_ptrs(position,old_finish-n,old_finish,__true_type());fill(position,position+n,x_copy);
   }else{
    uninitialized_fill_n(this->_M_finish,n-elems_after,x_copy);this->_M_finish+=n-elems_after;
    __uninitialized_copy(position,old_finish,this->_M_finish,__true_type());this->_M_finish+=elems_after;fill(position,old_finish,x_copy);
   }
  }else _M_insert_overflow(position,x,__true_type(),n);
 }
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
public: explicit scoped_ptr(T* p=0):ptr(p) {}
 ~scoped_ptr();
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
// Original public fields and user default constructor from Ego_r TPI.
class C2DExtentsI { public: long X,Y; C2DExtentsI() {}
 C2DExtentsI(long x,long y):X(x),Y(y){}
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


enum EPixelFormatType {PF_TYPE_NULL=0,PF_TYPE_UNKNOWN=1,PF_TYPE_RGBA=2,PF_TYPE_DEPTH=3,PF_TYPE_COMPRESSED_ALPHA=4,PF_TYPE_COMPRESSED_BOOLEAN_ALPHA=5,PF_TYPE_BUMP=6,PF_TYPE_COMPRESSED_INTERPOLATED_ALPHA=7};
// Layout: original classV/public fields; native GD3DFormats stride36.
class CD3DFormatInfo {public:_D3DFORMAT D3DFormat;EPixelFormatType PFType;long BitDepth,AlphaBits,RBits,GBits,BBits,StencilBits;bool Linear;};
extern const CD3DFormatInfo GD3DFormats[];
class CPixelFormat {
protected: long FormatIndex;
public:
 CPixelFormat() :FormatIndex(-1) {}
 CPixelFormat(_D3DFORMAT format) { Initialise(format); }
 void Initialise(_D3DFORMAT);
 unsigned long GetNoABits() const {return GD3DFormats[FormatIndex].AlphaBits;}
 unsigned long GetNoRBits() const {return GD3DFormats[FormatIndex].RBits;}
 unsigned long GetNoGBits() const {return GD3DFormats[FormatIndex].GBits;}
 unsigned long GetNoBBits() const {return GD3DFormats[FormatIndex].BBits;}
 unsigned long GetRPosShift() const {return GetNoBBits()+GetNoGBits();}
 unsigned long GetAPosShift() const {const CD3DFormatInfo& format=GD3DFormats[FormatIndex];return format.RBits+format.GBits+format.BBits;}
 unsigned long GetAMask() const {return ((1UL<<GetNoABits())-1)<<GetAPosShift();}
};
// Original PDB enumeration values, confirmed by native texture call arguments.
enum ESurfacePool {SURFACE_POOL_MANAGED=1,SURFACE_POOL_SYSTEM_MEM=2,SURFACE_POOL_DEFAULT_LOCATION=0};
// Layout: original private texture pointer and two packed bitfields within the existing dword.
class CTexture {
public:
 CTexture() :PD3DTexture(0),ByteLength(0),AllocationSource(0) {}
 ~CTexture();
 void Uninitialise();
 bool Initialise(const C2DExtentsI&,long,const CPixelFormat&,unsigned long,ESurfacePool,bool,bool);
private:
 IDirect3DTexture9* PD3DTexture;
 unsigned long ByteLength:28;
 unsigned long AllocationSource:4;
};


// Original public fields and user default constructor from Ego_r TPI.
class C2DBoxI { public: long TLX,TLY,BRX,BRY; C2DBoxI() {}
};
// Original public fields and user default constructor from Ego_r TPI.
class C2DBoxF { public: float TLX,TLY,BRX,BRY; C2DBoxF() {} };
// Original PDB CWideString, class key V, one protected pointer at +0.
class CCharString;
class CWideStringData;
class CWideString {
public:
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
// Retail opt-in (Codex pass11): original public vector base and accessors.
template<class T> class CArray:public std::vector<T> {
public:
 T& operator[](unsigned int i){return this->_M_start[i];}
 T* GetAsCArray(){return this->size()>0?&this->front():GetZeroSizeArray();}
private:T* GetZeroSizeArray(){return (T*)this;}
};

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
 bool GetBestAlphaTextureFormat(unsigned long,CPixelFormat&);
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
// Declaration-only API projection of the original class: no object construction, sizeof, or field access. This is not a storage-layout definition.
class CMemoryAllocator {
protected:
 static CMemoryAllocator* PMemoryAllocator;
 static void __fastcall Create();
 static void __fastcall EnterSynchronisation();
 static void __fastcall LeaveSynchronisation();
 void* InternalAllocate(long,unsigned long,bool);
public:
 static void* __fastcall Allocate(unsigned long size,unsigned long flags) {
  if(!PMemoryAllocator)Create();
  EnterSynchronisation();
  void* p=PMemoryAllocator->InternalAllocate(size,flags,false);
  LeaveSynchronisation();return p;
 }
};

// Layout: original base class4 bytes plus private Valid+4, native size8.
class CInitBaseClass:public CBaseClass {private:bool Valid;public:CInitBaseClass();~CInitBaseClass();
};
// Retail model (hand-written 2026-10-30, class-model migration wave 9): the engine's reference-counted owning pointer and
// its shared count block. Not polymorphic (no vtable, no RTTI); every member is inline in the release build.
//
// CCPPointerInfo (Ego_r.pdb, 12 bytes): RefCount +0x0 (ulong), DeleteFunc +0x4, Data +0x8 (void*). DeleteFunc is
//   __fastcall: every retail release loads `mov ecx,[eax+8]; call [eax+4]` (0x006E7AB0, 0x00CE1000, 0x00CE1040, 0x008AB2B0)
//   and the deleters it points at are `SIXPAX@Z` (static __fastcall void(void*)) CCountedPointer<T>::DeleteData in
//   FableWin.pdb (343 instantiations); Ego_r.pdb's `_func___cdecl_void_void_ptr` is the PDB's generic spelling.
//   Ctor: FableWin.pdb ??0CCPPointerInfo@@QAE@KP6IXPAX@Z0@Z = (unsigned long, void (__fastcall*)(void*), void*); retail
//   Reset 0x008AB2B0 expands it after `new(0xc)`: [eax]=1, [eax+4]=deleter, [eax+8]=Data (member order).
//
// CCountedPointer<T> (Ego_r.pdb, 8 bytes for every T): Data +0x0 (T*), Info +0x4 (CCPPointerInfo*). Member set = FableWin.pdb
//   (the debug build emits them; Ego_r keeps out of line only ShareData / ReleaseData / Reset / DeleteData, the rest inline):
//   ??0 (), (T*), (const&), member template ??$?0 (const CCountedPointer<U>&); ??1; ??4 (const&), member template ??$?4;
//   ??C operator-> QBE, ??D operator* QBE; DeleteData SIXPAX@Z; Get / get QBE; GetReferenceCount QBEKXZ; IsNull QBE_NXZ;
//   ReleaseData QAEXXZ; Reset / reset QAEXPAV; ShareData QAEXPAV..PAVCCPPointerInfo@@@Z; Validate QBEXXZ (debug check);
//   member template ??$SetStaticCast (63). Not modelled: the free __fastcall templates ??$?8 / ??$?9 (operator== / !=
//   of two CCountedPointers, YI_NABV..0@Z) -- no landed includer compares two pointers.
// Bodies (retail bytes; no ICF in retail: 732 separate /O2 dtor COMDATs, one per T):
//   ~CCountedPointer = 0x006E7AB0 (MovieBase) / 0x00CE1000 (CEntityScriptBase) and the other 730: `if (Info) { dec [Info];
//     reload Info; cmp [Info],0; jne; ecx = Info->Data; call Info->DeleteFunc; operator delete(Info) }`, then Data = 0
//     and Info = 0 in that order. It is its own body, not a ReleaseData call (below).
//   ReleaseData: the same release, then Info = 0 and Data = 0 in THAT order. Retail never emits it out of line (no
//     release COMDAT with the Info-first tail exists); expanded at 0x00A45330 / 0x00A5F250 / 0x006633A0 (`mov [x+4],edi;
//     mov [x],edi`). Reset and ShareData are written over it (their dead zero stores vanish):
//   Reset(T*) = 0x008AB2B0 (and 197 more): <ReleaseData> leaves only `mov [esi+4],0`, then Data = data; if (data)
//     Info = new CCPPointerInfo(1, DeleteData, Data) (the block reads Data back; a failed new leaves Info 0).
//   ShareData = 0x00CE1040 (CCountedPointer<CScriptThing>, Ego_r.pdb name): `if (Info != info) { <release>; Data = data;
//     Info = info; if (info) ++info->RefCount; }`.
//   operator= = 0x00704580 (CScriptThing): loads o.Data / o.Info first, then the ShareData body, returns *this.
//   The (T*) ctor is the allocation without the release (out of line at 0x008C58F0 / 0x008C5980, `ret 4`).
//   Copy ctor: Data, Info copied, then `if (Info) ++Info->RefCount` (expanded in every CScriptThing copy 0x004ABE90).
//   DeleteData = `delete static_cast<T*>(data)`: 0x00CDEE00 (polymorphic T: `test ecx,ecx; je; mov eax,[ecx]; push 1;
//     call [eax]`), 0x00674E80 (CExpressionSet: the inline implicit dtor, then operator delete). Only its address is
//     stored (the count block); the landed DeleteData rows explicitly instantiate it.
// Model test (wave 9, 955 landed rows, logs/mc_cpvar_*.txt): this split (dtor own body, ReleaseData zeroing Info first)
//   gives 15 DIFFER; a dtor that calls a zeroing ReleaseData gives the same leaf bytes at more sites but names ReleaseData
//   where retail calls the Data-first dtor COMDAT, and `ReleaseData(); Data = 0; Info = 0;` gives 20 DIFFER.
// Inlining: every member is plain `inline` (in-class), never __forceinline / noinline. Retail calls the COMDAT copies at
//   some sites and expands them at others; VC7.1 RTM 3077 reproduces that per-site choice from these bodies on the landed
//   rows (wave 9 HEADERS.md lists the shapes it does not reproduce). unproven: the source order of Reset's two stores and
//   the bodies of GetReferenceCount / reset / Get / get / IsNull (FableWin names only, no retail out-of-line copy found).

class CCPPointerInfo {
public:
    unsigned long RefCount;                                              // +0x0
    void (FABLE_FASTCALL* DeleteFunc)(void*);                             // +0x4
    void* Data;                                                          // +0x8
    CCPPointerInfo(unsigned long refCount, void (FABLE_FASTCALL* deleteFunc)(void*), void* data)
        : RefCount(refCount), DeleteFunc(deleteFunc), Data(data) { }     // FableWin.pdb ??0CCPPointerInfo@@QAE@KP6IXPAX@Z0@Z
};
FABLE_STATIC_ASSERT(sizeof(CCPPointerInfo) == 0xc);

template <class T> class CCountedPointer {
public:
    T* Data;                                                             // +0x0
    CCPPointerInfo* Info;                                                // +0x4

    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(T* data) : Data(data)
    {
        if (data)
            Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
        else
            Info = 0;
    }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    template <class U> CCountedPointer(const CCountedPointer<U>& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        ShareData(other.Data, other.Info);
        return *this;
    }
    template <class U> CCountedPointer& operator=(const CCountedPointer<U>& other)
    {
        ShareData(other.Data, other.Info);
        return *this;
    }
    T* operator->() const { return Data; }
    T& operator*() const { return *Data; }
    T* Get() const { return Data; }
    T* get() const { return Data; }
    bool IsNull() const { return Data == 0; }
    unsigned long GetReferenceCount() const { return Info ? Info->RefCount : 0; }   // unproven body (FableWin name only)
    void Validate() const;                                               // FableWin.pdb only (debug build check)
    void ReleaseData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Info = 0;
        Data = 0;
    }
    void ShareData(T* data, CCPPointerInfo* info)
    {
        if (Info != info) {
            ReleaseData();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
    }
    void Reset(T* data)
    {
        ReleaseData();
        Data = data;
        if (data)
            Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
    }
    template <class U> void SetStaticCast(const CCountedPointer<U>& other)   // FableWin.pdb ??$SetStaticCast@..@?$CCountedPointer@..@@QAEXABV?$CCountedPointer@..@@@Z (63)
    {
        ShareData(static_cast<T*>(other.Data), other.Info);              // landed 0x00A32620 (C3DMeshFile::GetRoot) expands it
    }
    void reset(T* data) { Reset(data); }                                 // unproven body (FableWin name only)
    static void FABLE_FASTCALL DeleteData(void* data) { delete static_cast<T*>(data); }                    // per-T deleter, address stored in the count block
};

struct _GUID {unsigned long Data1;unsigned short Data2,Data3;unsigned char Data4[8];};
struct _RTL_CRITICAL_SECTION_DEBUG;
struct _RTL_CRITICAL_SECTION {_RTL_CRITICAL_SECTION_DEBUG* DebugInfo;long LockCount,RecursionCount;void* OwningThread;void* LockSemaphore;unsigned long SpinCount;};
class CDebugManager;class CInputManager;class CDisplayManager;
class CGraphicBankManager;class CMeshBankManager;class CFontManager;class CProfileManager;
struct HINSTANCE__;struct tagMSG;

enum ESystemCode {SYS_OK=1,SYS_ERROR_INIT_DISPLAY_MANAGER=2,SYS_ERROR_INIT_SOUND_MANAGER=3,SYS_ERROR_INIT_WINDOW=4,SYS_ERROR_INIT_DRIVE_MANAGER=5,SYS_ERROR_INIT_JOYSTICK1=6,SYS_ERROR_INIT_JOYSTICK2=7,SYS_ERROR_INIT_JOYSTICK3=8,SYS_ERROR_INIT_JOYSTICK4=9,SYS_ERROR_INIT_MOUSE=10,SYS_ERROR_INIT_KEYBOARD=11,SYS_ERROR_INIT_INPUT_MANAGER=12,SYS_ERROR_INIT_RENDER_MANAGER=13,SYS_ERROR_INIT_DEBUG_MANAGER=14,SYS_ERROR_INIT_SYSTEM_MANAGER=15,SYS_ERROR_INIT_DDRAW=16,SYS_ERROR_INIT_NETWORK_MANAGER=17,SYS_ERROR_INIT_DLL_MANAGER=18,SYS_ERROR_INIT_GENERIC=19};
// Layout: original complete CSystemManager, native size0x180; PDisplayManager+0x60. Composite tail storage is named by its original member.
class CSystemManager:public CInitBaseClass {
private:
 bool WindowsQuit,ApplicationActive,ApplicationHasInputFocus;
 _GUID ApplicationGUID,BestGUID;_GUID* PBestGUID;bool PreferPrimaryDevice;
 _RTL_CRITICAL_SECTION CriticalSection;void* MainFibre;
 CCountedPointer<CDebugManager> LibDebugManager;
 CCountedPointer<CInputManager> PInputManager;
 CCountedPointer<CDisplayManager> PDisplayManager;
 CDebugManager* DebugManager;
 CCountedPointer<CGraphicBankManager> PGraphicBankManager;
 CCountedPointer<CMeshBankManager> PMeshBankManager;
 CCountedPointer<CFontManager> PFontManager;
 unsigned char _pad_0084[8]; // Native ctor009a7440 additional unaccessed composite; original name not recovered.
 bool WindowInitialised,DDrawInitialised,ExclusiveMode;
 HINSTANCE__* WinInstanceHandle;HWND ApplicationWinHandle;int WinShow;char* WinCommandLine;
 CWideString WinAppName;
 unsigned char WinClass[48]; // tagWNDCLASSEXW WinClass at0xa4.
 bool ResolutionSet;unsigned long InitFlags;
 bool EverBeenActive,MouseButton0Pressed,MouseButton1Pressed,MouseButton2Pressed,MouseButton3Pressed,MouseButton4Pressed;
 long MouseZMove;
 bool WindowValid,RestoreExclusive,Restore,WaitWhileInactive,AttachedToExternalWindow,PreventExclusiveModeChanges,UsingAccurateTimer;
 void (__fastcall* AppReinitFunc)(void*);void* AppReinitContext;
 CCountedPointer<CProfileManager> ProfileManager;
 unsigned char SystemColours[100]; // CGuiColoursInfo SystemColours at0x100.
 CWideString TempPath;
 unsigned char ApplicationName[4]; // CCharString ApplicationName at0x168.
 bool ForceMusicPlay;float ForceMusicPlayOffset;
 bool (__fastcall* ErrorFunc)(ESystemCode);
 bool (__fastcall* IMEMessageFunc)(HWND,unsigned int,unsigned int,long,long&);
 bool (__fastcall* MessageFilter)(tagMSG*);
public:CDisplayManager* GetDisplayManager(){return PDisplayManager.Get();}
};
FABLE_STATIC_ASSERT(sizeof(CSystemManager)==0x180);
CSystemManager* __fastcall GFGetSystemManager();
// Retail opt-in (Codex pass11): declaration-only original API; no object construction, sizeof, or field access. This is not a storage-layout definition.

class CBankFileAsyncData;
class CStreamingFontDataBank {
public:
 CCountedPointer<CBankFileAsyncData> StreamGlyph(void*,unsigned long,unsigned long,bool);
 void ReadData(void*,unsigned long,unsigned long);
};
// Layout: original public fields, native element stride8, allocation count multiplied by8.
class CStreamingGlyphData {public:signed char OffsetX,OffsetY,Width,Height;unsigned long MemOffset:23,Advance:8,BigChar:1;};
// Layout: original public Scale+0 and Baseline+1; native element stride2.
class CStreamingGlyphAdjustment {public:unsigned char Scale;signed char Baseline;};
#if defined(_MSC_VER)
void* __cdecl operator new[](unsigned int);
#else
void* operator new[](size_t);
#endif
void __cdecl operator delete[](void*);
namespace {
extern unsigned char* textureCacheMem;
extern unsigned long textureCacheSize;
extern unsigned long glyphTextureCacheGlyphDataSize;
extern unsigned long glyphTextureCacheGlyphDataNumIndices;
extern unsigned long* glyphTextureCacheGlyphDataIndices;
extern void* glyphTextureCacheGlyphData;
extern unsigned long numTextureCacheGlyphDatasPerChunk;
extern CStreamingGlyphData* glyphTextureCacheGlyphDataCache;
extern unsigned long glyphTextureCacheGlyphDataCacheChunk;
extern CStreamingGlyphAdjustment* glyphTextureCacheGlyphAdjustment;
extern unsigned long textureCacheNumGlyphs;
extern unsigned long textureCacheNumGlyphAdjustments;
extern unsigned long textureCacheMemOffset;
extern CTexture GFontTextureCache;
extern CArray<unsigned char> GFontPixelLookupTable;
extern long GFontTextureBytesPerPixel;
void __fastcall InitializeStreamingFontTextureCache(CStreamingFontDataBank* data_bank)
{
 if(!textureCacheMem) {
  textureCacheSize=512*1024;
  textureCacheMem=(unsigned char*)CMemoryAllocator::Allocate(textureCacheSize,0);
  CPixelFormat pixel_format(D3DFMT_A8);
  GFontTextureBytesPerPixel=1;
  GFontPixelLookupTable.resize(256);
  for(long c_val=0;c_val<256;++c_val)GFontPixelLookupTable[c_val]=(unsigned char)c_val;
  bool success=GFontTextureCache.Initialise(C2DExtentsI(1024,512),1,pixel_format,0,SURFACE_POOL_MANAGED,false,true);
  if(!success) {
   CPixelFormat pixel_format(D3DFMT_A4R4G4B4);
   GFontTextureBytesPerPixel=2;
   GFontPixelLookupTable.resize(512);
   unsigned short* table_ptr=(unsigned short*)GFontPixelLookupTable.GetAsCArray();
   for(long c_val=0;c_val<256;++c_val) {
    unsigned long pixel_val=((c_val*16)/256)<<12;
    pixel_val|=0x0fff;
    table_ptr[c_val]=(unsigned short)pixel_val;
   }
   success=GFontTextureCache.Initialise(C2DExtentsI(1024,512),1,pixel_format,0,SURFACE_POOL_MANAGED,false,false);
  }
  if(!success) {
   CPixelFormat pixel_format;
   GFGetSystemManager()->GetDisplayManager()->GetBestAlphaTextureFormat(32,pixel_format);
   GFontTextureBytesPerPixel=4;
   GFontPixelLookupTable.resize(1024);
   unsigned long* table_ptr=(unsigned long*)GFontPixelLookupTable.GetAsCArray();
   for(long c_val=0;c_val<256;++c_val) {
    unsigned long pixel_val=0xffffffff;
    pixel_val&=~pixel_format.GetAMask();
    pixel_val|=c_val<<pixel_format.GetAPosShift();
    table_ptr[c_val]=pixel_val;
   }
   GFontTextureCache.Initialise(C2DExtentsI(1024,512),1,pixel_format,0,SURFACE_POOL_MANAGED,false,false);
  }
  data_bank->StreamGlyph(&textureCacheNumGlyphs,0,4,true);
  data_bank->StreamGlyph(&textureCacheNumGlyphAdjustments,4,4,true);
  data_bank->StreamGlyph(&textureCacheMemOffset,8,4,true);
  data_bank->StreamGlyph(&glyphTextureCacheGlyphDataSize,12,4,true);
  data_bank->StreamGlyph(&numTextureCacheGlyphDatasPerChunk,16,4,true);
  glyphTextureCacheGlyphDataNumIndices=(textureCacheNumGlyphs+numTextureCacheGlyphDatasPerChunk-1)/numTextureCacheGlyphDatasPerChunk;
  glyphTextureCacheGlyphDataCacheChunk=0xffffffff;
  const unsigned long glyphDataOffset=20;
  const unsigned long glyphIndicesOffset=glyphDataOffset+glyphTextureCacheGlyphDataSize;
  const unsigned long glyphIndicesSize=(glyphTextureCacheGlyphDataNumIndices+1)*sizeof(unsigned long);
  const unsigned long glyphAdjustmentOffset=glyphIndicesOffset+glyphIndicesSize;
  const unsigned long glyphAdjustmentSize=textureCacheNumGlyphAdjustments*sizeof(CStreamingGlyphAdjustment);
  glyphTextureCacheGlyphDataCache=new CStreamingGlyphData[numTextureCacheGlyphDatasPerChunk];
  glyphTextureCacheGlyphData=new unsigned char[glyphTextureCacheGlyphDataSize];
  glyphTextureCacheGlyphDataIndices=new unsigned long[glyphTextureCacheGlyphDataNumIndices+1];
  glyphTextureCacheGlyphAdjustment=new CStreamingGlyphAdjustment[textureCacheNumGlyphAdjustments];
  data_bank->ReadData(glyphTextureCacheGlyphData,glyphDataOffset,glyphTextureCacheGlyphDataSize);
  data_bank->ReadData(glyphTextureCacheGlyphDataIndices,glyphIndicesOffset,glyphIndicesSize);
  data_bank->ReadData(glyphTextureCacheGlyphAdjustment,glyphAdjustmentOffset,glyphAdjustmentSize);
 }
}
}

namespace {
unsigned char* textureCacheMem=0;unsigned long textureCacheSize=0;
unsigned long glyphTextureCacheGlyphDataSize=0,glyphTextureCacheGlyphDataNumIndices=0;
unsigned long* glyphTextureCacheGlyphDataIndices=0;void* glyphTextureCacheGlyphData=0;
unsigned long numTextureCacheGlyphDatasPerChunk=0;CStreamingGlyphData* glyphTextureCacheGlyphDataCache=0;
unsigned long glyphTextureCacheGlyphDataCacheChunk=0;CStreamingGlyphAdjustment* glyphTextureCacheGlyphAdjustment=0;
unsigned long textureCacheNumGlyphs=0,textureCacheNumGlyphAdjustments=0,textureCacheMemOffset=0;
CTexture GFontTextureCache;CArray<unsigned char> GFontPixelLookupTable;long GFontTextureBytesPerPixel=0;
}
const CD3DFormatInfo GD3DFormats[]={
 {D3DFMT_A8,PF_TYPE_RGBA,8,8,0,0,0,0,false},
 {D3DFMT_A4R4G4B4,PF_TYPE_RGBA,16,4,4,4,4,0,false},
 {D3DFMT_A8R8G8B8,PF_TYPE_RGBA,32,8,8,8,8,0,false}
};
static int texture_attempts,path,lookup_calls,create_calls,locks,alloc_calls,streams,reads,objects_deleted;
static unsigned long metadata[5];static CStreamingFontDataBank* expected_bank;
static CCPPointerInfo* shared[5];static int shared_count,ref_mode;
static unsigned char tokens[5];
static union SystemStorage{double align;void* pointer;unsigned char data[sizeof(CSystemManager)];}system_storage;
static union DisplayStorage{double align;void* pointer;unsigned char data[sizeof(CDisplayManager)];}display_storage;
static union MemoryStorage{double align;void* pointer;unsigned char data[16];}memory_storage;
CMemoryAllocator* CMemoryAllocator::PMemoryAllocator=0;
void __fastcall CMemoryAllocator::Create(){++create_calls;PMemoryAllocator=(CMemoryAllocator*)memory_storage.data;}
void __fastcall CMemoryAllocator::EnterSynchronisation(){assert(locks==0);++locks;}
void __fastcall CMemoryAllocator::LeaveSynchronisation(){assert(locks==1);--locks;}
void* CMemoryAllocator::InternalAllocate(long size,unsigned long flags,bool nozero){assert(locks==1 && size==524288 && flags==0 && nozero==false);++alloc_calls;return malloc(size);}
CSystemManager* __fastcall GFGetSystemManager(){return(CSystemManager*)system_storage.data;}
bool CDisplayManager::GetBestAlphaTextureFormat(unsigned long bits,CPixelFormat& format){assert(this==(CDisplayManager*)display_storage.data && bits==32);format.FormatIndex=2;return true;}
void CPixelFormat::Initialise(_D3DFORMAT f){++lookup_calls;if(f==D3DFMT_A8)FormatIndex=0;else {assert(f==D3DFMT_A4R4G4B4);FormatIndex=1;}}
CTexture::~CTexture(){}
bool CTexture::Initialise(const C2DExtentsI& e,long levels,const CPixelFormat& f,unsigned long flags,ESurfacePool pool,bool a,bool b){
 assert(this==&GFontTextureCache && e.X==1024 && e.Y==512 && levels==1 && flags==0 && pool==SURFACE_POOL_MANAGED && !a);
 int current=texture_attempts++;assert(f.FormatIndex==current && b==(current==0));
 assert(GFontTextureBytesPerPixel==(current==0?1:current==1?2:4));
 assert(GFontPixelLookupTable.size()==(unsigned int)(256*GFontTextureBytesPerPixel));
 for(unsigned int i=0;i<256;++i){
  if(current==0)assert(GFontPixelLookupTable[i]==i);
  if(current==1)assert(((unsigned short*)GFontPixelLookupTable.GetAsCArray())[i]==(unsigned short)(((i/16)<<12)|4095));
  if(current==2)assert(((unsigned long*)GFontPixelLookupTable.GetAsCArray())[i]==((i<<24)|0x00ffffff));
 }
 return current==path;
}
static void __fastcall destroy_token(void* p){assert(p>=tokens && p<tokens+5);++objects_deleted;}
CCountedPointer<CBankFileAsyncData> CStreamingFontDataBank::StreamGlyph(void* dest,unsigned long offset,unsigned long size,bool block){
 assert(this==expected_bank && streams<5 && offset==(unsigned long)streams*4 && size==4 && block);
 void* expected[5]={&textureCacheNumGlyphs,&textureCacheNumGlyphAdjustments,&textureCacheMemOffset,&glyphTextureCacheGlyphDataSize,&numTextureCacheGlyphDatasPerChunk};
 assert(dest==expected[streams]);memcpy(dest,&metadata[streams],4);
 CCountedPointer<CBankFileAsyncData> r;r.Data=(CBankFileAsyncData*)&tokens[streams];
 if(ref_mode!=0){r.Info=new CCPPointerInfo(ref_mode==1?1:2,destroy_token,&tokens[streams]);if(ref_mode==2)shared[shared_count++]=r.Info;}
 ++streams;return r;
}
void CStreamingFontDataBank::ReadData(void* dest,unsigned long offset,unsigned long length){
 assert(this==expected_bank && reads<3);
 unsigned long indices=(metadata[0]+metadata[4]-1)/metadata[4];
 unsigned long expected_offset=reads==0?20:reads==1?20+metadata[3]:20+metadata[3]+(indices+1)*4;
 unsigned long expected_length=reads==0?metadata[3]:reads==1?(indices+1)*4:metadata[1]*2;
 assert(offset==expected_offset && length==expected_length);
 for(unsigned long i=0;i<length;++i)((unsigned char*)dest)[i]=(unsigned char)((offset+i)*29+7);
 ++reads;
}
static void verify_data(const void* p,unsigned long off,unsigned long n){for(unsigned long i=0;i<n;++i)assert(((const unsigned char*)p)[i]==(unsigned char)((off+i)*29+7));}
int main(){
 memset(&system_storage,0,sizeof(system_storage));
 CSystemManager* system=(CSystemManager*)system_storage.data;system->PDisplayManager.Data=(CDisplayManager*)display_storage.data;
 union BankStorage{double align;void* pointer;unsigned char data[16];}bank_storage;
 CStreamingFontDataBank* bank=(CStreamingFontDataBank*)bank_storage.data;expected_bank=bank;unsigned int cases=0;
 for(path=0;path<4;++path)for(ref_mode=0;ref_mode<3;++ref_mode)for(unsigned int variant=0;variant<8;++variant){
  texture_attempts=lookup_calls=create_calls=locks=alloc_calls=streams=reads=objects_deleted=shared_count=0;
  metadata[0]=1+variant*7;metadata[1]=variant;metadata[2]=123+variant;metadata[3]=variant*17;metadata[4]=1+variant%4;
  CMemoryAllocator::PMemoryAllocator=variant&1?(CMemoryAllocator*)memory_storage.data:0;
  InitializeStreamingFontTextureCache(bank);
  assert(textureCacheMem && textureCacheSize==524288 && alloc_calls==1 && locks==0 && create_calls==((variant&1)?0:1));
  assert(texture_attempts==(path<2?path+1:3) && lookup_calls==(path==0?1:2));
  assert(streams==5 && reads==3 && objects_deleted==(ref_mode==1?5:0));
  unsigned long indices=(metadata[0]+metadata[4]-1)/metadata[4];
  assert(glyphTextureCacheGlyphDataNumIndices==indices && glyphTextureCacheGlyphDataCacheChunk==0xffffffff);
  assert(textureCacheNumGlyphs==metadata[0] && textureCacheNumGlyphAdjustments==metadata[1] && textureCacheMemOffset==metadata[2]);
  verify_data(glyphTextureCacheGlyphData,20,metadata[3]);verify_data(glyphTextureCacheGlyphDataIndices,20+metadata[3],(indices+1)*4);verify_data(glyphTextureCacheGlyphAdjustment,20+metadata[3]+(indices+1)*4,metadata[1]*2);
  InitializeStreamingFontTextureCache(bank);assert(streams==5 && reads==3 && alloc_calls==1 && texture_attempts==(path<2?path+1:3));
  for(int i=0;i<shared_count;++i){assert(shared[i]->RefCount==1);shared[i]->DeleteFunc(shared[i]->Data);delete shared[i];}
  free(textureCacheMem);textureCacheMem=0;delete[]glyphTextureCacheGlyphDataCache;delete[](unsigned char*)glyphTextureCacheGlyphData;delete[]glyphTextureCacheGlyphDataIndices;delete[]glyphTextureCacheGlyphAdjustment;
  free(GFontPixelLookupTable._M_start);GFontPixelLookupTable._M_start=GFontPixelLookupTable._M_finish=GFontPixelLookupTable._M_end_of_storage._M_data=0;
  ++cases;
 }
 assert(cases==96);puts("009a3640_TEST PASS");return 0;
}
