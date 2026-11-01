#include <stdio.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __declspec(x)
#define __cdecl
#define __fastcall
#endif
static int erase_calls=0,insert_calls=0,delete_calls=0;
// 00ac8a60 QuadricOptimiserInternals::COptimisedPrimitive::UpdateCost
// Leaf: ?UpdateCost@COptimisedPrimitive@QuadricOptimiserInternals@@AAEXABV?$CCountedPointer@VCVertexPairContraction@QuadricOptimiserInternals@@@@@Z
// Flags:
#include <stddef.h>
#include <new>
extern "C" void*__cdecl malloc(size_t);
// Begin shared header engine/CCountedPointer.h
#ifndef FABLE_ENGINE_CCOUNTEDPOINTER_H
#define FABLE_ENGINE_CCOUNTEDPOINTER_H
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
// Begin shared header rebuild_abi.h
#ifndef TEST_EMBED_3AC84EBD4026
#define TEST_EMBED_3AC84EBD4026


// ABI primitives for reconstructed FableTLC modules. Keep this header valid
// under VC7.1; modern-only language/library features belong outside Tier 0.
#if defined(_MSC_VER)
#  define FABLE_CDECL __cdecl
#  define FABLE_STDCALL __stdcall
#  define FABLE_FASTCALL __fastcall
#  define FABLE_THISCALL __fastcall
#else
#  define FABLE_CDECL
#  define FABLE_STDCALL
#  define FABLE_FASTCALL
#  define FABLE_THISCALL
#endif

typedef signed char fable_i8;
typedef unsigned char fable_u8;
typedef signed short fable_i16;
typedef unsigned short fable_u16;
typedef signed int fable_i32;
typedef unsigned int fable_u32;

typedef char fable_require_i8[sizeof(fable_i8) == 1 ? 1 : -1];
typedef char fable_require_i16[sizeof(fable_i16) == 2 ? 1 : -1];
typedef char fable_require_i32[sizeof(fable_i32) == 4 ? 1 : -1];
#ifdef _MSC_VER
typedef char fable_require_ptr32[sizeof(void*) == 4 ? 1 : -1];
#endif

#define FABLE_JOIN_INNER(a, b) a##b
#define FABLE_JOIN(a, b) FABLE_JOIN_INNER(a, b)
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_sassert_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h

class CCPPointerInfo {
public:
    unsigned long RefCount;                                              // +0x0
    void (FABLE_FASTCALL* DeleteFunc)(void*);                             // +0x4
    void* Data;                                                          // +0x8
    CCPPointerInfo(unsigned long refCount, void (FABLE_FASTCALL* deleteFunc)(void*), void* data)
        : RefCount(refCount), DeleteFunc(deleteFunc), Data(data) { }     // FableWin.pdb ??0CCPPointerInfo@@QAE@KP6IXPAX@Z0@Z
};
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CCPPointerInfo) == 0xc);
#endif
#endif

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

#endif // FABLE_ENGINE_CCOUNTEDPOINTER_H

// End shared header engine/CCountedPointer.h
#define COptimisedPrimitive COptimisedPrimitiveFields
// Begin shared header engine/COptimisedPrimitive.h
#ifndef FABLE_ENGINE_COPTIMISEDPRIMITIVE_H
#define FABLE_ENGINE_COPTIMISEDPRIMITIVE_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof COptimisedPrimitive = 0xa0.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof COptimisedPrimitive = 0x84.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
// Begin shared header rebuild_abi.h
#ifndef TEST_EMBED_3AC84EBD4026
#define TEST_EMBED_3AC84EBD4026


// ABI primitives for reconstructed FableTLC modules. Keep this header valid
// under VC7.1; modern-only language/library features belong outside Tier 0.
#if defined(_MSC_VER)
#  define FABLE_CDECL __cdecl
#  define FABLE_STDCALL __stdcall
#  define FABLE_FASTCALL __fastcall
#  define FABLE_THISCALL __fastcall
#else
#  define FABLE_CDECL
#  define FABLE_STDCALL
#  define FABLE_FASTCALL
#  define FABLE_THISCALL
#endif

typedef signed char fable_i8;
typedef unsigned char fable_u8;
typedef signed short fable_i16;
typedef unsigned short fable_u16;
typedef signed int fable_i32;
typedef unsigned int fable_u32;

typedef char fable_require_i8[sizeof(fable_i8) == 1 ? 1 : -1];
typedef char fable_require_i16[sizeof(fable_i16) == 2 ? 1 : -1];
typedef char fable_require_i32[sizeof(fable_i32) == 4 ? 1 : -1];
#ifdef _MSC_VER
typedef char fable_require_ptr32[sizeof(void*) == 4 ? 1 : -1];
#endif

#define FABLE_JOIN_INNER(a, b) a##b
#define FABLE_JOIN(a, b) FABLE_JOIN_INNER(a, b)
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_sassert_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h

#pragma pack(push, 1)
struct COptimisedPrimitive {
    long          MaterialIndex;                             // +0x00
    unsigned char UniqueVertices[0xc];                       // +0x04 CArray<QuadricOptimiserInternals::CUniqueVertex>
    unsigned char Vertices[0xc];                             // +0x10 CArray<QuadricOptimiserInternals::COptimisedVertex>
    unsigned char Faces[0xc];                                // +0x1c CArray<QuadricOptimiserInternals::COptimisedFace>
    unsigned char Edges[0xc];                                // +0x28 CArray<QuadricOptimiserInternals::COptimisedEdge>
    unsigned char VertexPairContractions[3*sizeof(void*)];               // +0x34 multimap<float,CCountedPointer<QuadricOptimiserInternals::CVertexPairContraction>,std::less<float>,std::allocator<std::pair<float_const_,CCountedPointer<QuadricOptimiserInternals::CVertexPairContraction>_>_>_>
    long          SegmentCount;                              // +0x40
    unsigned char SegmentSizes[0xc];                         // +0x44 CArray<float>
    unsigned char SubChunksToKeep[0xc];                      // +0x50 CArray<CCountedPointer<CChunkedFileChunk>_>
    float         Parameters_Scaling;                        // +0x5c COptimisationParameters::Scaling
    float         Parameters_EdgeWeight;                     // +0x60 COptimisationParameters::EdgeWeight
    float         Parameters_EdgeCost;                       // +0x64 COptimisationParameters::EdgeCost
    float         Parameters_GroupWeight;                    // +0x68 COptimisationParameters::GroupWeight
    float         Parameters_TextureWeight;                  // +0x6c COptimisationParameters::TextureWeight
    float         Parameters_TexturePenalty;                 // +0x70 COptimisationParameters::TexturePenalty
    float         Parameters_UnsplitTextureWeight;           // +0x74 COptimisationParameters::UnsplitTextureWeight
    float         Parameters_UnsplitNormalWeight;            // +0x78 COptimisationParameters::UnsplitNormalWeight
    float         Parameters_HighPenalty;                    // +0x7c COptimisationParameters::HighPenalty
    unsigned long Parameters_MaximumBonesPerVertex;          // +0x80 COptimisationParameters::MaximumBonesPerVertex
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(COptimisedPrimitive) == 0x84);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(COptimisedPrimitive, MaterialIndex) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(COptimisedPrimitive, UniqueVertices) == 0x4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(COptimisedPrimitive, Vertices) == 0x10);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(COptimisedPrimitive, Faces) == 0x1c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(COptimisedPrimitive, Edges) == 0x28);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(COptimisedPrimitive, VertexPairContractions) == 0x34);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(COptimisedPrimitive, SegmentCount) == 0x40);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(COptimisedPrimitive, SegmentSizes) == 0x44);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(COptimisedPrimitive, SubChunksToKeep) == 0x50);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(COptimisedPrimitive, Parameters_Scaling) == 0x5c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(COptimisedPrimitive, Parameters_EdgeWeight) == 0x60);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(COptimisedPrimitive, Parameters_EdgeCost) == 0x64);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(COptimisedPrimitive, Parameters_GroupWeight) == 0x68);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(COptimisedPrimitive, Parameters_TextureWeight) == 0x6c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(COptimisedPrimitive, Parameters_TexturePenalty) == 0x70);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(COptimisedPrimitive, Parameters_UnsplitTextureWeight) == 0x74);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(COptimisedPrimitive, Parameters_UnsplitNormalWeight) == 0x78);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(COptimisedPrimitive, Parameters_HighPenalty) == 0x7c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(COptimisedPrimitive, Parameters_MaximumBonesPerVertex) == 0x80);
#endif
#endif

#endif // FABLE_ENGINE_COPTIMISEDPRIMITIVE_H

// End shared header engine/COptimisedPrimitive.h
#undef COptimisedPrimitive
namespace std{
template<class T>class allocator{public:T*allocate(size_t n){return static_cast<T*>(malloc(n*sizeof(T)));}};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{};
template<class A,class B>struct pair{typedef A first_type;typedef B second_type;A first;B second;pair(const A&a,const B&b):first(a),second(b){}};
template<class A,class R>struct unary_function{typedef A argument_type;typedef R result_type;};
template<class A,class B,class R>struct binary_function{typedef A first_argument_type;typedef B second_argument_type;typedef R result_type;};
template<class T>struct less:public binary_function<T,T,bool>{bool operator()(const T&a,const T&b)const{return a<b;}};
template<class P>struct _Select1st:public unary_function<P,typename P::first_type>{const typename P::first_type&operator()(const P&p)const{return p.first;}};
template<class T>struct _Nonconst_traits{typedef T*pointer;typedef T&reference;};
struct _Rb_tree_node_base{bool _M_color;_Rb_tree_node_base*_M_parent,*_M_left,*_M_right;};
template<class Dummy>class _Rb_global{public:static void __cdecl _Rebalance(_Rb_tree_node_base*,_Rb_tree_node_base*&);};
template<class T>struct _Rb_tree_node:public _Rb_tree_node_base{T _M_value_field;};
struct _Rb_tree_base_iterator{_Rb_tree_node_base*_M_node;};
template<class T,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator{_Rb_tree_iterator(){} _Rb_tree_iterator(_Rb_tree_node<T>*node){_M_node=node;} _Rb_tree_iterator(const _Rb_tree_iterator&x){_M_node=x._M_node;}};
template<class T,class A>struct _Rb_tree_base{protected:_STLP_alloc_proxy<_Rb_tree_node<T>*,_Rb_tree_node<T>,allocator<_Rb_tree_node<T> > >_M_header;};
template<class K,class V,class Select,class Compare,class A=allocator<V> >class _Rb_tree:public _Rb_tree_base<V,A>{protected:unsigned int _M_node_count;Compare _M_key_compare;public:typedef _Rb_tree_iterator<V,_Nonconst_traits<V> >iterator;void erase(iterator);__declspec(noinline) iterator insert_equal(const V&v){_Rb_tree_node<V>*y=this->_M_header._M_data;_Rb_tree_node<V>*x=static_cast<_Rb_tree_node<V>*>(y->_M_parent);while(x){y=x;x=static_cast<_Rb_tree_node<V>*>(_M_key_compare(Select()(v),Select()(x->_M_value_field))?x->_M_left:x->_M_right);}return _M_insert(x,y,v,0);}
private:
 __declspec(noinline) iterator _M_insert(_Rb_tree_node_base*x,_Rb_tree_node_base*y,const V&v,_Rb_tree_node_base*w){
  _Rb_tree_node<V>*z;
  if(y==this->_M_header._M_data||(w==0&&(x!=0||_M_key_compare(Select()(v),Select()(static_cast<_Rb_tree_node<V>*>(y)->_M_value_field))))){
   z=_M_create_node(v);y->_M_left=z;
   if(y==this->_M_header._M_data){this->_M_header._M_data->_M_parent=z;this->_M_header._M_data->_M_right=z;}else if(y==this->_M_header._M_data->_M_left)this->_M_header._M_data->_M_left=z;
  }else{z=_M_create_node(v);y->_M_right=z;if(y==this->_M_header._M_data->_M_right)this->_M_header._M_data->_M_right=z;}
  z->_M_parent=y;z->_M_left=z->_M_right=0;_Rb_global<bool>::_Rebalance(z,this->_M_header._M_data->_M_parent);++_M_node_count;return iterator(z);
 }
protected:
 _Rb_tree_node<V>*_M_create_node(const V&v){_Rb_tree_node<V>*n=this->_M_header.allocate(1);new(&n->_M_value_field)V(v);return n;}
};
template<class K,class V,class C=less<K>,class A=allocator<pair<const K,V> > >class multimap{typedef pair<const K,V>value_type;typedef _Rb_tree<K,value_type,_Select1st<value_type>,C,A>_Rep_type;_Rep_type _M_t;public:typedef typename _Rep_type::iterator iterator;void erase(iterator p){_M_t.erase(p);}iterator insert(const value_type&v){return _M_t.insert_equal(v);}};
}
template<class T>class CArray:public std::vector<T>{};
class C3DVector{public:float X,Y,Z;};
namespace QuadricOptimiserInternals{
class CSubContraction;class CVertexPairContraction;
typedef std::multimap<float,CCountedPointer<CVertexPairContraction> > ContractionMap;
// Layout: original fields; retail CArray is12 bytes, reducing donor offsets by4.
class CVertexPairContraction{public:bool Exists,Unsplit;CArray<CSubContraction>SubContractions;float Weight2,Cost;C3DVector ResultingPosition;ContractionMap::iterator ListIterator;};
// Layout: shared flattened fields; original class namespace and private method restored.
class COptimisedPrimitive:public COptimisedPrimitiveFields{public:void UpdateCost(const CCountedPointer<CVertexPairContraction>&);};
void COptimisedPrimitive::UpdateCost(const CCountedPointer<CVertexPairContraction>&contraction){
 if(contraction->Exists){
  reinterpret_cast<ContractionMap&>(VertexPairContractions).erase(contraction->ListIterator);
  contraction->ListIterator=reinterpret_cast<ContractionMap&>(VertexPairContractions).insert(std::pair<const float,CCountedPointer<CVertexPairContraction> >(contraction->Cost,contraction));
 }
}
}

namespace std{
template<class Dummy>void __cdecl _Rb_global<Dummy>::_Rebalance(_Rb_tree_node_base*,_Rb_tree_node_base*&){++insert_calls;}
template<class K,class V,class Select,class Compare,class A>void _Rb_tree<K,V,Select,Compare,A>::erase(iterator it){
 ++erase_calls;_Rb_tree_node<V>*n=static_cast<_Rb_tree_node<V>*>(it._M_node);_Rb_tree_node_base*p=n->_M_parent;_Rb_tree_node_base*child=n->_M_left?n->_M_left:n->_M_right;
 if(p==this->_M_header._M_data)this->_M_header._M_data->_M_parent=child;else if(p->_M_left==n)p->_M_left=child;else p->_M_right=child;if(child)child->_M_parent=p;
 if(this->_M_header._M_data->_M_left==n)this->_M_header._M_data->_M_left=child?child:p;if(this->_M_header._M_data->_M_right==n)this->_M_header._M_data->_M_right=child?child:p;
 n->_M_value_field.~V();free(n);--_M_node_count;
}
}
using namespace QuadricOptimiserInternals;
typedef std::pair<const float,CCountedPointer<CVertexPairContraction> > Value;
typedef std::_Rb_tree<float,Value,std::_Select1st<Value>,std::less<float> > Tree;
typedef std::_Rb_tree_node<Value> Node;
class TestTree:public Tree{public:void init(){this->_M_header._M_data=static_cast<Node*>(malloc(sizeof(Node)));this->_M_header._M_data->_M_parent=0;this->_M_header._M_data->_M_left=this->_M_header._M_data->_M_right=this->_M_header._M_data;this->_M_node_count=0;}void finish(){free(this->_M_header._M_data);}unsigned int count()const{return this->_M_node_count;}};
static void __fastcall ReleaseContraction(void*){++delete_calls;}
#define CHECK(x) do{if(!(x)){printf("failure line %d\n",__LINE__);return 1;}}while(0)
int main(){
 for(int oldside=0;oldside<2;++oldside)for(int newside=-1;newside<=1;++newside)for(int exists=0;exists<2;++exists)for(int extra=0;extra<2;++extra){
 COptimisedPrimitive owner;CVertexPairContraction object,peer;object.Exists=exists!=0;object.Cost=float(newside*8);peer.Exists=true;peer.Cost=0;
 TestTree&tree=*reinterpret_cast<TestTree*>(owner.VertexPairContractions);tree.init();int olddelete=delete_calls;
 {
 CCountedPointer<CVertexPairContraction>input,other;input.Data=&object;input.Info=new CCPPointerInfo(1,ReleaseContraction,&object);other.Data=&peer;other.Info=new CCPPointerInfo(1,ReleaseContraction,&peer);
 Tree::iterator peerpos=tree.insert_equal(Value(0,other));object.ListIterator=tree.insert_equal(Value(oldside?16.0f:-16.0f,input));std::_Rb_tree_node_base*oldpos=object.ListIterator._M_node;input.Info->RefCount+=extra;
 int olderase=erase_calls,oldinsert=insert_calls;owner.UpdateCost(input);
 CHECK(erase_calls==olderase+exists);CHECK(insert_calls==oldinsert+exists);CHECK(tree.count()==2);CHECK(input.Data==&object);CHECK(input.Info->RefCount==static_cast<unsigned long>(2+extra));CHECK(other.Info->RefCount==2);CHECK(delete_calls==olddelete);
 Node*node=static_cast<Node*>(object.ListIterator._M_node);CHECK(node->_M_value_field.second.Data==&object);CHECK(node->_M_value_field.first==(exists?object.Cost:(oldside?16.0f:-16.0f)));if(!exists)CHECK(object.ListIterator._M_node==oldpos);
 tree.erase(object.ListIterator);tree.erase(peerpos);CHECK(tree.count()==0);input.Info->RefCount-=extra;CHECK(input.Info->RefCount==1&&other.Info->RefCount==1);
 }
 CHECK(delete_calls==olddelete+2);tree.finish();
 }
 puts("00ac8a60_TEST PASS");return 0;
}