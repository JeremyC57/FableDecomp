#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __int64 long
#endif
#define private public
#define protected public
// 0057de57 CTCHeroStats::PrepareToKickHeroToMostRecentSafeRegion
// Leaf: ?PrepareToKickHeroToMostRecentSafeRegion@CTCHeroStats@@QAEXXZ
// Flags: /O1 /Oy-
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#endif
namespace std{
typedef ::size_t size_t;typedef ::ptrdiff_t ptrdiff_t;
struct input_iterator_tag{};struct forward_iterator_tag:public input_iterator_tag{};struct bidirectional_iterator_tag:public forward_iterator_tag{};
template<class A,class B>struct pair{typedef A first_type;typedef B second_type;A first;B second;pair():first(A()),second(B()){}pair(const A&a,const B&b):first(a),second(b){}template<class X,class Y>pair(const pair<X,Y>&p):first(p.first),second(p.second){}pair(const pair&p):first(p.first),second(p.second){}};
template<class A,class B,class C>struct binary_function{typedef A first_argument_type;typedef B second_argument_type;typedef C result_type;};
template<class A,class B>struct unary_function{typedef A argument_type;typedef B result_type;};
template<class T>struct less:public binary_function<T,T,bool>{bool operator()(const T&a,const T&b)const{return a<b;}};
template<class P>struct _Select1st:public unary_function<P,typename P::first_type>{const typename P::first_type&operator()(const P&p)const{return p.first;}};
template<class T>class allocator{public:typedef T value_type;template<class U>struct rebind{typedef allocator<U>other;};allocator(){}allocator(const allocator&){}template<class U>allocator(const allocator<U>&){}};
template<class T,class A>struct _Alloc_traits{typedef typename A::template rebind<T>::other allocator_type;};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;_STLP_alloc_proxy(const A&a,V p):A(a),_M_data(p){}};
typedef bool _Rb_tree_Color_type;
struct _Rb_tree_node_base{typedef _Rb_tree_node_base*_Base_ptr;_Rb_tree_Color_type _M_color;_Base_ptr _M_parent,_M_left,_M_right;};
template<class V>struct _Rb_tree_node:public _Rb_tree_node_base{V _M_value_field;_Rb_tree_node(const V&v):_M_value_field(v){}};
template<class Dummy>class _Rb_global{public:typedef _Rb_tree_node_base*_Base_ptr;static _Base_ptr __cdecl _M_increment(_Base_ptr);static _Base_ptr __cdecl _M_decrement(_Base_ptr);};
typedef _Rb_global<bool>_Rb_global_inst;
struct _Rb_tree_base_iterator{typedef _Rb_tree_node_base*_Base_ptr;typedef bidirectional_iterator_tag iterator_category;typedef ptrdiff_t difference_type;_Base_ptr _M_node;bool operator==(const _Rb_tree_base_iterator&x)const{return _M_node==x._M_node;}bool operator!=(const _Rb_tree_base_iterator&x)const{return _M_node!=x._M_node;}};
template<class T>struct _Nonconst_traits{typedef T value_type;typedef T&reference;typedef T*pointer;};
template<class V,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator{
 typedef V value_type;typedef typename Traits::reference reference;typedef typename Traits::pointer pointer;typedef _Rb_tree_iterator<V,Traits>_Self;typedef _Rb_tree_node<V>*_Link_type;
 _Rb_tree_iterator(){_M_node=0;}_Rb_tree_iterator(_Link_type p){_M_node=p;}_Rb_tree_iterator(const _Rb_tree_iterator<V,_Nonconst_traits<V> >&p){_M_node=p._M_node;}
 reference operator*()const{return _Link_type(_M_node)->_M_value_field;}
 _Self&operator++(){_M_node=_Rb_global_inst::_M_increment(_M_node);return*this;}_Self&operator--(){_M_node=_Rb_global_inst::_M_decrement(_M_node);return*this;}
};
template<class V,class A>struct _Rb_tree_base{
 typedef _Rb_tree_node<V>_Node;typedef typename _Alloc_traits<V,A>::allocator_type allocator_type;
 _Rb_tree_base(const allocator_type& =allocator_type());~_Rb_tree_base();
protected:typedef typename _Alloc_traits<_Node,A>::allocator_type _M_node_allocator_type;_STLP_alloc_proxy<_Node*,_Node,_M_node_allocator_type>_M_header;
};
template<class K,class V,class Key,class Compare,class A=allocator<V> >class _Rb_tree:public _Rb_tree_base<V,A>{
 typedef _Rb_tree_base<V,A>_Base;
protected:typedef _Rb_tree_node_base*_Base_ptr;typedef _Rb_tree_node<V>_Node;typedef _Rb_tree_Color_type _Color_type;
public:typedef K key_type;typedef V value_type;typedef V*pointer;typedef const V*const_pointer;typedef V&reference;typedef const V&const_reference;typedef _Rb_tree_node<V>*_Link_type;typedef size_t size_type;typedef ptrdiff_t difference_type;typedef bidirectional_iterator_tag _Iterator_category;typedef typename _Base::allocator_type allocator_type;
protected:size_type _M_node_count;Compare _M_key_compare;
 _Link_type&_M_leftmost()const{return(_Link_type&)this->_M_header._M_data->_M_left;}
 _Link_type&_M_rightmost()const{return(_Link_type&)this->_M_header._M_data->_M_right;}
 static _Link_type&__cdecl _S_right(_Link_type p){return(_Link_type&)p->_M_right;}
 static reference __cdecl _S_value(_Link_type p){return p->_M_value_field;}
 static const K&__cdecl _S_key(_Link_type p){return Key()(_S_value(p));}
 static _Link_type&__cdecl _S_right(_Base_ptr p){return(_Link_type&)p->_M_right;}
 static reference __cdecl _S_value(_Base_ptr p){return((_Link_type)p)->_M_value_field;}
 static const K&__cdecl _S_key(_Base_ptr p){return Key()(_S_value(_Link_type(p)));}
public:typedef _Rb_tree_iterator<V,_Nonconst_traits<V> >iterator;
private:iterator _M_insert(_Base_ptr,_Base_ptr,const value_type&,_Base_ptr=0);
public:iterator begin(){return iterator(_M_leftmost());}iterator end(){return iterator(this->_M_header._M_data);}
 size_type size()const{return _M_node_count;}pair<iterator,bool>insert_unique(const value_type&);iterator insert_unique(iterator,const value_type&);
};

template<class T>struct _Identity:public unary_function<T,T>{const T&operator()(const T&v)const{return v;}};
template<class K,class V,class C=less<K>,class A=allocator<pair<const K,V> > >class map{
public:typedef pair<const K,V>value_type;
private:typedef _Rb_tree<K,value_type,_Select1st<value_type>,C,A>_Rep_type;_Rep_type _M_t;
public:typedef typename _Rep_type::iterator iterator;iterator begin(){return _M_t.begin();}iterator end(){return _M_t.end();}V&operator[](const K&);
};
template<class K,class C=less<K>,class A=allocator<K> >class set{
private:typedef _Rb_tree<K,K,_Identity<K>,C,A>_Rep_type;_Rep_type _M_t;
};
}


#pragma optimize("y",off)
#pragma optimize("s",on)
class CCharStringData;class CCharString{protected:CCharStringData*PStringData;public:CCharString(const char*,long=-1);CCharString(const CCharString&);~CCharString();bool operator!=(const CCharString&)const;};
namespace std{
template<class T,class A>class _Vector_base{public:typedef typename _Alloc_traits<T,A>::allocator_type allocator_type;_Vector_base(const A& =A());~_Vector_base();protected:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,allocator_type>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:typedef T value_type;};
}
// Named observedRegion prefix: two truevector12 thenName18, unusedtail omitted.
class CRegion{public:CRegion():Name("Current"){}protected:std::vector<long>Maps,VisMaps;CCharString Name;public:const CCharString&GetName()const{return Name;}};
class C3DVector;class C2DBoxI;class CDrawMapInfo;
// Begin shared header engine/CBaseClassNonCopyable.h
#ifndef FABLE_ENGINE_CBASECLASSNONCOPYABLE_H
#define FABLE_ENGINE_CBASECLASSNONCOPYABLE_H
// Retail layout (hand-written; no members). RTTI CBaseClassNonCopyable : CBaseClass; vtable 0x0129A7C4, slot 0 scalar
// deleting dtor 0x0099A470. Ego_r.pdb / FableWin.pdb: ??0CBaseClassNonCopyable@@IAE@XZ (protected),
// ??1CBaseClassNonCopyable@@MAE@XZ (protected virtual). The copy operations are private and undefined (non-copyable).
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
// Begin shared header engine/CBaseClass.h
#ifndef FABLE_ENGINE_CBASECLASS_H
#define FABLE_ENGINE_CBASECLASS_H
// Retail layout (hand-written; Ego_r.pdb has no members): the root polymorphic base. RTTI .?AVCBaseClass@@ (no bases);
// vtable 0x01231710, slot 0 scalar deleting dtor 0x004371F0, which calls ~CBaseClass 0x0099A2E0 (one vtable store).
// The default ctor is out of line (0x0099A2D0: `mov eax,ecx; mov [eax],0x01231710; ret`): Ego_r.pdb places
// ??0CBaseClass@@QAE@XZ / ??1CBaseClass@@UAE@XZ side by side (0x00B01F20 / 0x00B01F30, one .cpp) and FableWin.pdb does the
// same (0x03044700 / 0x03044720), and 1,225 landed rows call 0x0099A2D0. The copy ctor is the implicit one: FableWin.pdb
// emits ??0CBaseClass@@QAE@ABV0@@Z beside the compiler-generated ??_E (0x018CD7E0), and retail expands it inline (a bare
// vtable store in a derived copy ctor, landed chunk 577 0x00DCEDF0 / chunk 578 0x00F05F30: write `: CBaseClass(other)`).
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

class CBaseClass {
public:
    CBaseClass();                                                        // 0x0099A2D0
    virtual ~CBaseClass();                                               // slot 0  0x0099A2E0
};
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CBaseClass) == 0x4);
#endif
#endif

#endif // FABLE_ENGINE_CBASECLASS_H

// End shared header engine/CBaseClass.h

class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();
    virtual ~CBaseClassNonCopyable();                                    // slot 0
private:
    CBaseClassNonCopyable(const CBaseClassNonCopyable&);
    CBaseClassNonCopyable& operator=(const CBaseClassNonCopyable&);
};
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CBaseClassNonCopyable) == 0x4);
#endif
#endif

#endif // FABLE_ENGINE_CBASECLASSNONCOPYABLE_H

// End shared header engine/CBaseClassNonCopyable.h
class CIDrawWorldMap{public:
 virtual long DrawGetNoMaps()const=0;
 virtual CDrawMapInfo DrawGetMapInfo(long)const=0;
 virtual C2DBoxI DrawGetDimensions()const=0;
 virtual long DrawGetGranularity()const=0;
 virtual bool DrawIsPosInMap(const C3DVector&)const=0;
 virtual float DrawGetSizeZAt(const C3DVector&,float)const=0;
 virtual C3DVector DrawConvertWorldMapCoordsToLocalCoords(long,const C3DVector&)const=0;
 virtual long DrawGetMyPlayerMapNumber()const=0;
 virtual bool DrawFindLastUnblockedGroundPositionAndReportCollision(const C3DVector&,const C3DVector&,C3DVector&)const=0;
 virtual bool DrawFindLastUnblockedThemePositionAndReportCollision(const C3DVector&,const C3DVector&,C3DVector&)const=0;
 virtual long DrawGetFrame()const=0;
 virtual~CIDrawWorldMap();
};

// OriginalserviceAPI projection; unobserveddata/virtualtails are not claimed.
class CWorldMap:public CBaseClassNonCopyable,public CIDrawWorldMap{public:long GetLoadedRegion()const;const CRegion&GetRegion(long)const;};
// CWorld is used only as scopeforits staticGetFrame; no CWorld object isprojected.
class CWorld{public:static long __fastcall GetFrame();};
class CThingMarker;class CClass;
#define CThing CThingFields
// Begin shared header engine/_quarantine/CThing.h
#ifndef FABLE_ENGINE_CTHING_H
#define FABLE_ENGINE_CTHING_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CThing = 0xa8.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CThing = 0x98.
// Retail evidence: 5 trusted landed function(s); reconciliation 3 conflict(s) in 3 file(s) vs 2 agreeing.
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

class CCharStringData;
class CDefPointeeBase;
class CTCDriverBase;
struct CTCGraphicAppearance;
class CTCPhysicsBase;

#pragma pack(push, 1)
class CThing {
public:
    void*                 __vftable;                         // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char         _pad_0x04[0x4];                    // +0x04
    unsigned char         _pad_0x08[0x4];                    // +0x08
    unsigned char         _pad_0x0c[0x4];                    // +0x0c
    unsigned char         _pad_0x10[0x1];                    // +0x10
    unsigned char         _pad_0x11[0x7];                    // +0x11
    unsigned __int64      UniqueID;                          // +0x18
    unsigned char         UsingComponents[0x24];             // +0x20 bitset<274>
    unsigned char         Components[0x10];                  // +0x44 CVectorMap<ETCInterfaceType,CTCBase_*,CKeyPairCompareLess<ETCInterfaceType,CTCBase_*>_>
    unsigned char         UpdateComponentsList[0x4];         // +0x54 list<CTCBase_*,std::allocator<CTCBase_*>_>
    CCharStringData*      SerialisationSectionName_PStringData; // +0x58 CCharString::PStringData
    CTCDriverBase*        PTCDriver;                         // +0x5c
    CTCPhysicsBase*       PhysicsTC;                         // +0x60
    CTCGraphicAppearance* PTCGraphicAppearance;              // +0x64
    long                  WorldFrameCreatedAt;               // +0x68
    unsigned long         ThingTypeFlags;                    // +0x6c
    CDefPointeeBase*      PBaseDef_Object;                   // +0x70 CDefPointer<CThingBaseDef_const_>::Object
    CCharStringData*      ScriptName_PStringData;            // +0x74 CCharString::PStringData
    CCharStringData*      ScriptData_PStringData;            // +0x78 CCharString::PStringData
    CThing*               RenderListPrev;                    // +0x7c
    CThing*               RenderListNext;                    // +0x80
    CThing*               UpdateListPrev;                    // +0x84
    CThing*               UpdateListNext;                    // +0x88
    unsigned short        DefGlobalIndex;                    // +0x8c
    short                 CurrentMapNumber;                  // +0x8e
    signed char           PlayerNumber;                      // +0x90
    unsigned char         CreatedBySerialisation[0x1];       // +0x91 bitfield group: uchar:1
    unsigned char         PausedDuringCutscenes[0x1];        // +0x92 bitfield group: uchar:1
    unsigned char         AddingTCsPermitted[0x1];           // +0x93 bitfield group: uchar:1
    unsigned char         InitialisationStage[0x4];          // +0x94 bitfield group: uchar:3
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CThing) == 0x98);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, UniqueID) == 0x18);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, UsingComponents) == 0x20);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, Components) == 0x44);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, UpdateComponentsList) == 0x54);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, SerialisationSectionName_PStringData) == 0x58);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, PTCDriver) == 0x5c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, PhysicsTC) == 0x60);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, PTCGraphicAppearance) == 0x64);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, WorldFrameCreatedAt) == 0x68);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, ThingTypeFlags) == 0x6c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, PBaseDef_Object) == 0x70);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, ScriptName_PStringData) == 0x74);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, ScriptData_PStringData) == 0x78);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, RenderListPrev) == 0x7c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, RenderListNext) == 0x80);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, UpdateListPrev) == 0x84);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, UpdateListNext) == 0x88);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, DefGlobalIndex) == 0x8c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, CurrentMapNumber) == 0x8e);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, PlayerNumber) == 0x90);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, CreatedBySerialisation) == 0x91);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, PausedDuringCutscenes) == 0x92);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, AddingTCsPermitted) == 0x93);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, InitialisationStage) == 0x94);
#endif
#endif

#endif // FABLE_ENGINE_CTHING_H

// End shared header engine/_quarantine/CThing.h
#undef CThing
class CThing:public CThingFields{public:bool IsIncludedInThingScans()const;};
namespace std{
struct _List_node_base{_List_node_base*_M_next,*_M_prev;};
template<class T>struct _List_node:public _List_node_base{T _M_data;};
struct _List_iterator_base{_List_node_base*_M_node;_List_iterator_base(_List_node_base*p):_M_node(p){};};
template<class T>struct _Const_traits{typedef const T&reference;typedef const T*pointer;};
template<class T,class Traits>struct _List_iterator:public _List_iterator_base{typedef _List_node<T>_Node;_List_iterator(_Node*p):_List_iterator_base(p){}typename Traits::reference operator*()const{return static_cast<_Node*>(_M_node)->_M_data;}_List_iterator&operator++(){_M_node=_M_node->_M_next;return*this;}bool operator!=(const _List_iterator&p)const{return _M_node!=p._M_node;}};
template<class T,class A>class _List_base{public:_List_base():_M_node(allocator<_List_node<T> >(),0){}protected:typedef _List_node<T>_Node;_STLP_alloc_proxy<_Node*,_Node,allocator<_Node> >_M_node;};
template<class T,class A=allocator<T> >class list:public _List_base<T,A>{public:typedef _List_iterator<T,_Const_traits<T> >const_iterator;const_iterator begin()const{return const_iterator(static_cast<_List_node<T>*>(this->_M_node._M_data->_M_next));}const_iterator end()const{return const_iterator(this->_M_node._M_data);}};
}

class CThingFilter_HasDefName{private:CCharString Name;public:CThingFilter_HasDefName(const CCharString&name):Name(name){}bool operator()(const CThing&)const;};
class CThingFilter_HasScriptData{private:CCharString Data;public:CThingFilter_HasScriptData(const CCharString&data):Data(data){}bool operator()(const CThing&)const;};
template<class A,class B>class CFilter_Bind2{protected:const A&F1;const B&F2;public:CFilter_Bind2(const A&a,const B&b):F1(a),F2(b){}template<class T>bool operator()(const T&item)const{return F1(reinterpret_cast<const CThing&>(item))&&F2(reinterpret_cast<const CThing&>(item));}};
#define CThingSearchTools CThingSearchToolsFields
// Begin shared header engine/CThingSearchTools.h
#ifndef FABLE_ENGINE_CTHINGSEARCHTOOLS_H
#define FABLE_ENGINE_CTHINGSEARCHTOOLS_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CThingSearchTools = 0x1c.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CThingSearchTools = 0x18.
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

class CThingManager;
struct CThingMapManager;
class CWorldMap;

#pragma pack(push, 1)
class CThingSearchTools {
public:
    CWorldMap*        WorldMap;                              // +0x00
    CThingMapManager* PThingMapManager;                      // +0x04
    CThingManager*    ThingManager;                          // +0x08
    unsigned char     AreaMaps[0xc];                         // +0x0c vector<NThingMap::CThingMap_*,std::allocator<NThingMap::CThingMap_*>_>
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CThingSearchTools) == 0x18);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingSearchTools, WorldMap) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingSearchTools, PThingMapManager) == 0x4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingSearchTools, ThingManager) == 0x8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingSearchTools, AreaMaps) == 0xc);
#endif
#endif

#endif // FABLE_ENGINE_CTHINGSEARCHTOOLS_H

// End shared header engine/CThingSearchTools.h
#undef CThingSearchTools
class CThingSearchTools:public CThingSearchToolsFields{protected:const std::list<CClass*>&PeekTypeList(long)const;public:template<class F,class T>bool GetPFirstLeafThing(const F&,T**)const;};
template<>__declspec(noinline)bool CThingSearchTools::GetPFirstLeafThing<CFilter_Bind2<CThingFilter_HasDefName,CThingFilter_HasScriptData>,CThingMarker>(const CFilter_Bind2<CThingFilter_HasDefName,CThingFilter_HasScriptData>&filter,CThingMarker**out)const{
 const std::list<CClass*>&things=PeekTypeList(11);
 for(std::list<CClass*>::const_iterator it=things.begin();it!=things.end();++it){CThingMarker*item=reinterpret_cast<CThingMarker*>(*it);if(reinterpret_cast<const CThing*>(item)->IsIncludedInThingScans()&&filter(*item)){*out=item;return true;}}
 return false;
}
class CTCBase:public CBaseClassNonCopyable{public:CTCBase(CThing&t):ParentThing(t){}protected:CThing&ParentThing;private:bool ToBeRemoved,IsInGlobalUpdateTCs;protected:CWorld&GetWorld()const;const CWorldMap&PeekWorldMap()const;const CThingSearchTools&PeekThingSearchTools()const;};
class CTCHeroStats:public CTCBase{public:CTCHeroStats(CThing&t):CTCBase(t){}
 struct MirroredCriminalStatus{long Crimes[10];long LastTimeSeenByGuard,LastTimeExited;bool ExitedUsingTeleporter;float EntrancePosX,EntrancePosY,EntrancePosZ,EntranceAngle;};
 std::map<CCharString,MirroredCriminalStatus>OutstandingCrimes;
 void GetOutstandingFineAndFramesLeft(const CCharString&,long&,long&);
 void PrepareToKickHeroToMostRecentSafeRegion();
};
#define CTCHeroStats CTCHeroStatsFields
// Begin shared header engine/CTCHeroStats.h
#ifndef FABLE_ENGINE_CTCHEROSTATS_H
#define FABLE_ENGINE_CTCHEROSTATS_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CTCHeroStats = 0x1fc.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CTCHeroStats = 0x1dc.
// Retail evidence: 2 trusted landed function(s); reconciliation 1 conflict(s) in 1 file(s) vs 1 agreeing; PINNED by 1 retyped file(s) despite conflicts.
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

class CCharStringData;
class CDefPointeeBase;

#pragma pack(push, 1)
class CTCHeroStats {
public:
    void*            __vftable;                              // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char    _pad_0x04[0x4];                         // +0x04
    unsigned char    _pad_0x08[0x4];                         // +0x08
    unsigned char    OutstandingCrimes[0xc];                 // +0x0c map<CCharString,CTCHeroStats::MirroredCriminalStatus,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CTCHeroStats::MirroredCriminalStatus>_>_>
    unsigned char    ExperienceSpentOn[0xc];                 // +0x18 vector<long,std::allocator<long>_>
    long             NumberOfDecapitations;                  // +0x24
    long             Morality;                               // +0x28
    bool             MoralityChangingEnabled;                // +0x2c
    unsigned char    _pad_0x2d[0x3];                         // +0x2d
    float            Age;                                    // +0x30
    float            SunTan;                                 // +0x34
    float            Fatness;                                // +0x38
    long             Money;                                  // +0x3c
    long             MaxMoney;                               // +0x40
    long             ConfiscatedMoney;                       // +0x44
    long             TotalMoneyEarned;                       // +0x48
    long             TotalMoneySpent;                        // +0x4c
    long             RewardMoney;                            // +0x50
    long             RentMoney;                              // +0x54
    long             Stamina;                                // +0x58
    long             MaxStamina;                             // +0x5c
    long             FramesUntilStaminaRefills;              // +0x60
    long             FramesUntilHealthRefills;               // +0x64
    bool             ScriptOverridePreventStaminaRefill;     // +0x68
    unsigned char    _pad_0x69[0x3];                         // +0x69
    long             FramesUntilFatnessReduces;              // +0x6c
    long             RenownLevel;                            // +0x70
    long             RenownMaxLevel;                         // +0x74
    long             RenownPointsInLevel;                    // +0x78
    long             RenownTotal;                            // +0x7c
    float            Agreeableness;                          // +0x80
    float            InherentAttractiveness;                 // +0x84
    float            InherentScariness;                      // +0x88
    float            ExtraMaxHealth;                         // +0x8c
    long             ExtraMaxStamina;                        // +0x90
    long             FramesLeftForSpeedMultiplier;           // +0x94
    long             MaximumMultiplier;                      // +0x98
    float            Attractiveness;                         // +0x9c
    float            Scariness;                              // +0xa0
    float            AppearanceGoodstrength;                 // +0xa4
    float            SoundRadiusMultiplier;                  // +0xa8
    float            VisibilityMultiplier;                   // +0xac
    long             FrameUpdatedClothingStats;              // +0xb0
    long             FrameUpdatedTattooStats;                // +0xb4
    long             NumFailedAQuest;                        // +0xb8
    long             NumCompletedAQuest;                     // +0xbc
    long             NumCompletedACoreQuest;                 // +0xc0
    long             NumCompletedAFeatQuest;                 // +0xc4
    long             NumCompletedAnOptionalQuest;            // +0xc8
    long             NumBoastsTaken;                         // +0xcc
    long             NumBoastsCompleted;                     // +0xd0
    long             NumMountedTrophies;                     // +0xd4
    long             NumTrophyGamePlayed;                    // +0xd8
    long             BestTrophyWitnesses;                    // +0xdc
    long             NumberOfShopsOwned;                     // +0xe0
    long             NumberOfHousesOwned;                    // +0xe4
    long             NumberOfBuildingsRented;                // +0xe8
    long             NumberOfVomits;                         // +0xec
    long             NumberOfDrinks;                         // +0xf0
    long             NumberOfCrimes;                         // +0xf4
    long             NumberOfCrimesOfMurder;                 // +0xf8
    long             TotalFine;                              // +0xfc
    long             FinePaid;                               // +0x100
    long             TimesThrownOut;                         // +0x104
    long             TotalBribeAmount;                       // +0x108
    long             NumTeleports;                           // +0x10c
    float            MaxChickenThrow;                        // +0x110
    unsigned char    TrainingStatus[0x4];                    // +0x114 EHeroTrainingStatus
    unsigned char    HeroStatsExperience[0xc];               // +0x118 vector<long,std::allocator<long>_>
    unsigned char    RegionTime[0xc];                        // +0x124 vector<long,std::allocator<long>_>
    unsigned char    NumberOfTimesUsedAbility[0xc];          // +0x130 vector<long,std::allocator<long>_>
    long             TotalSpentExperience;                   // +0x13c
    long             NumLoversAcquired;                      // +0x140
    long             NumWeddings;                            // +0x144
    long             NumSpousesMurdered;                     // +0x148
    long             NumSpousesDied;                         // +0x14c
    long             NumDivorces;                            // +0x150
    long             NumSex;                                 // +0x154
    bool             HadScriptedSex;                         // +0x158
    bool             HadScriptedGaySex;                      // +0x159
    unsigned char    _pad_0x15a[0x2];                        // +0x15a
    float            BiggestFish;                            // +0x15c
    unsigned char    HousePositions[0xc];                    // +0x160 vector<std::pair<CRegion_const_*,C3DVector>,std::allocator<std::pair<CRegion_const_*,C3DVector>_>_>
    bool             InfiniteStamina;                        // +0x16c
    unsigned char    _pad_0x16d[0x3];                        // +0x16d
    CCharStringData* DeathRecoveryMarkerName_PStringData;    // +0x170 CCharString::PStringData
    bool             FailQuestOnDeath;                       // +0x174
    unsigned char    _pad_0x175[0x3];                        // +0x175
    float            BestTimePairs;                          // +0x178
    float            BestTimeSorting;                        // +0x17c
    float            BestScoreBlackjack;                     // +0x180
    float            BestScoreCoinGolfOakVale;               // +0x184
    float            BestScoreCoinGolfSnowSpire;             // +0x188
    float            BestScoreShoveHaPenny;                  // +0x18c
    float            BestTimeGuessTheAddition;               // +0x190
    bool             PrizeGivenPairs;                        // +0x194
    bool             PrizeGivenSorting;                      // +0x195
    bool             PrizeGivenBlackjack;                    // +0x196
    bool             PrizeGivenCoinGolfOakVale;              // +0x197
    bool             PrizeGivenCoinGolfSnowSpire;            // +0x198
    bool             PrizeGivenShoveHaPenny;                 // +0x199
    bool             PrizeGivenGuessTheAddition;             // +0x19a
    unsigned char    _pad_0x19b[0x1];                        // +0x19b
    float            Drunkenness;                            // +0x19c
    float            BeerReservoir;                          // +0x1a0
    float            DrinkAffectMultiplier;                  // +0x1a4
    float            DrinkAffectMultiplierMultiplier;        // +0x1a8
    long             FrameToVomit;                           // +0x1ac
    long             DeedHandle;                             // +0x1b0
    long             FrameEndVomit;                          // +0x1b4
    bool             UpdateDrunkenness;                      // +0x1b8
    unsigned char    _pad_0x1b9[0x3];                        // +0x1b9
    CDefPointeeBase* PDef_Object;                            // +0x1bc CDefPointer<CSpecialAbilitiesDrunkennessDef_const_>::Object
    unsigned char    PrimitiveHandle[0x8];                   // +0x1c0 CEnginePrimitiveHandle
    float            UCounter;                               // +0x1c8
    float            VCounter;                               // +0x1cc
    float            RotCounter;                             // +0x1d0
    bool             IsInCutSceneFirstTime;                  // +0x1d4
    unsigned char    _pad_0x1d5[0x1];                        // +0x1d5
    unsigned char    _pad_0x1d6[0x2];                        // +0x1d6
    unsigned char    Marriages[0x4];                         // +0x1d8 list<CHeroMarriageInfo,std::allocator<CHeroMarriageInfo>_>
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CTCHeroStats) == 0x1dc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, OutstandingCrimes) == 0xc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, ExperienceSpentOn) == 0x18);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumberOfDecapitations) == 0x24);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, Morality) == 0x28);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, MoralityChangingEnabled) == 0x2c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, Age) == 0x30);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, SunTan) == 0x34);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, Fatness) == 0x38);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, Money) == 0x3c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, MaxMoney) == 0x40);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, ConfiscatedMoney) == 0x44);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, TotalMoneyEarned) == 0x48);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, TotalMoneySpent) == 0x4c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, RewardMoney) == 0x50);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, RentMoney) == 0x54);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, Stamina) == 0x58);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, MaxStamina) == 0x5c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, FramesUntilStaminaRefills) == 0x60);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, FramesUntilHealthRefills) == 0x64);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, ScriptOverridePreventStaminaRefill) == 0x68);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, FramesUntilFatnessReduces) == 0x6c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, RenownLevel) == 0x70);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, RenownMaxLevel) == 0x74);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, RenownPointsInLevel) == 0x78);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, RenownTotal) == 0x7c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, Agreeableness) == 0x80);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, InherentAttractiveness) == 0x84);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, InherentScariness) == 0x88);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, ExtraMaxHealth) == 0x8c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, ExtraMaxStamina) == 0x90);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, FramesLeftForSpeedMultiplier) == 0x94);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, MaximumMultiplier) == 0x98);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, Attractiveness) == 0x9c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, Scariness) == 0xa0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, AppearanceGoodstrength) == 0xa4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, SoundRadiusMultiplier) == 0xa8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, VisibilityMultiplier) == 0xac);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, FrameUpdatedClothingStats) == 0xb0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, FrameUpdatedTattooStats) == 0xb4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumFailedAQuest) == 0xb8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumCompletedAQuest) == 0xbc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumCompletedACoreQuest) == 0xc0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumCompletedAFeatQuest) == 0xc4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumCompletedAnOptionalQuest) == 0xc8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumBoastsTaken) == 0xcc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumBoastsCompleted) == 0xd0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumMountedTrophies) == 0xd4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumTrophyGamePlayed) == 0xd8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, BestTrophyWitnesses) == 0xdc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumberOfShopsOwned) == 0xe0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumberOfHousesOwned) == 0xe4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumberOfBuildingsRented) == 0xe8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumberOfVomits) == 0xec);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumberOfDrinks) == 0xf0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumberOfCrimes) == 0xf4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumberOfCrimesOfMurder) == 0xf8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, TotalFine) == 0xfc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, FinePaid) == 0x100);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, TimesThrownOut) == 0x104);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, TotalBribeAmount) == 0x108);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumTeleports) == 0x10c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, MaxChickenThrow) == 0x110);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, TrainingStatus) == 0x114);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, HeroStatsExperience) == 0x118);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, RegionTime) == 0x124);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumberOfTimesUsedAbility) == 0x130);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, TotalSpentExperience) == 0x13c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumLoversAcquired) == 0x140);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumWeddings) == 0x144);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumSpousesMurdered) == 0x148);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumSpousesDied) == 0x14c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumDivorces) == 0x150);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumSex) == 0x154);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, HadScriptedSex) == 0x158);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, HadScriptedGaySex) == 0x159);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, BiggestFish) == 0x15c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, HousePositions) == 0x160);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, InfiniteStamina) == 0x16c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, DeathRecoveryMarkerName_PStringData) == 0x170);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, FailQuestOnDeath) == 0x174);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, BestTimePairs) == 0x178);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, BestTimeSorting) == 0x17c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, BestScoreBlackjack) == 0x180);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, BestScoreCoinGolfOakVale) == 0x184);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, BestScoreCoinGolfSnowSpire) == 0x188);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, BestScoreShoveHaPenny) == 0x18c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, BestTimeGuessTheAddition) == 0x190);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, PrizeGivenPairs) == 0x194);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, PrizeGivenSorting) == 0x195);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, PrizeGivenBlackjack) == 0x196);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, PrizeGivenCoinGolfOakVale) == 0x197);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, PrizeGivenCoinGolfSnowSpire) == 0x198);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, PrizeGivenShoveHaPenny) == 0x199);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, PrizeGivenGuessTheAddition) == 0x19a);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, Drunkenness) == 0x19c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, BeerReservoir) == 0x1a0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, DrinkAffectMultiplier) == 0x1a4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, DrinkAffectMultiplierMultiplier) == 0x1a8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, FrameToVomit) == 0x1ac);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, DeedHandle) == 0x1b0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, FrameEndVomit) == 0x1b4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, UpdateDrunkenness) == 0x1b8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, PDef_Object) == 0x1bc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, PrimitiveHandle) == 0x1c0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, UCounter) == 0x1c8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, VCounter) == 0x1cc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, RotCounter) == 0x1d0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, IsInCutSceneFirstTime) == 0x1d4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, Marriages) == 0x1d8);
#endif
#endif

#endif // FABLE_ENGINE_CTCHEROSTATS_H

// End shared header engine/CTCHeroStats.h
#undef CTCHeroStats
#define CRegion CRegionFields
// Begin shared header engine/CRegion.h
#ifndef FABLE_ENGINE_CREGION_H
#define FABLE_ENGINE_CREGION_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CRegion = 0x64.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CRegion = 0x58.
// Retail evidence: 2 trusted landed function(s); reconciliation clean.
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

class CCharStringData;
class CDefPointeeBase;

#pragma pack(push, 1)
class CRegion {
public:
    unsigned char    Maps[0xc];                              // +0x00 vector<long,std::allocator<long>_>
    unsigned char    VisMaps[0xc];                           // +0x0c vector<long,std::allocator<long>_>
    CCharStringData* Name_PStringData;                       // +0x18 CCharString::PStringData
    CCharStringData* DisplayName_PStringData;                // +0x1c CCharString::PStringData
    CCharStringData* RegionDefName_PStringData;              // +0x20 CCharString::PStringData
    CDefPointeeBase* PDef_Object;                            // +0x24 CDefPointer<CRegionDef_const_>::Object
    CCharStringData* MiniMapGraphic_PStringData;             // +0x28 CCharString::PStringData
    float            MiniMapScale;                           // +0x2c
    float            MiniMapOffset_X;                        // +0x30 C2DVector::X
    float            MiniMapOffset_Y;                        // +0x34 C2DVector::Y
    unsigned char    MiniMapRegionExitTextOffsets[0xc];      // +0x38 map<CCharString,C2DVector,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,C2DVector>_>_>
    float            WorldMapOffset_X;                       // +0x44 C2DVector::X
    float            WorldMapOffset_Y;                       // +0x48 C2DVector::Y
    float            NameGraphicOffset_X;                    // +0x4c C2DVector::X
    float            NameGraphicOffset_Y;                    // +0x50 C2DVector::Y
    bool             AppearOnWorldMap;                       // +0x54
    bool             CreatureGenerationEnabled;              // +0x55
    bool             SoundThemesEnabled;                     // +0x56
    unsigned char    _pad_0x57[0x1];                         // +0x57
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CRegion) == 0x58);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CRegion, Maps) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CRegion, VisMaps) == 0xc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CRegion, Name_PStringData) == 0x18);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CRegion, DisplayName_PStringData) == 0x1c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CRegion, RegionDefName_PStringData) == 0x20);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CRegion, PDef_Object) == 0x24);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CRegion, MiniMapGraphic_PStringData) == 0x28);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CRegion, MiniMapScale) == 0x2c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CRegion, MiniMapOffset_X) == 0x30);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CRegion, MiniMapOffset_Y) == 0x34);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CRegion, MiniMapRegionExitTextOffsets) == 0x38);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CRegion, WorldMapOffset_X) == 0x44);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CRegion, WorldMapOffset_Y) == 0x48);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CRegion, NameGraphicOffset_X) == 0x4c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CRegion, NameGraphicOffset_Y) == 0x50);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CRegion, AppearOnWorldMap) == 0x54);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CRegion, CreatureGenerationEnabled) == 0x55);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CRegion, SoundThemesEnabled) == 0x56);
#endif
#endif

#endif // FABLE_ENGINE_CREGION_H

// End shared header engine/CRegion.h
#undef CRegion
#define CTCBase CTCBaseFields
// Begin shared header engine/CTCBase.h
#ifndef FABLE_ENGINE_CTCBASE_H
#define FABLE_ENGINE_CTCBASE_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CTCBase = 0xc.
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

class CThing;

#pragma pack(push, 1)
class CTCBase {
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    CThing*       ParentThing;                               // +0x04
    bool          ToBeRemoved;                               // +0x08
    bool          IsInGlobalUpdateTCs;                       // +0x09
    unsigned char _pad_0x0a[0x2];                            // +0x0a
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CTCBase) == 0xc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCBase, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCBase, ParentThing) == 0x4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCBase, ToBeRemoved) == 0x8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCBase, IsInGlobalUpdateTCs) == 0x9);
#endif
#endif

#endif // FABLE_ENGINE_CTCBASE_H

// End shared header engine/CTCBase.h
#undef CTCBase
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CTCBase)==sizeof(CTCBaseFields));
#endif
template<class A,class B>inline CFilter_Bind2<A,B>__fastcall GFFilter_Bind2(const A&a,const B&b){return CFilter_Bind2<A,B>(a,b);}
void CTCHeroStats::PrepareToKickHeroToMostRecentSafeRegion(){
 long mostRecentExit=0;
 CCharString currentRegion=PeekWorldMap().GetRegion(PeekWorldMap().GetLoadedRegion()).GetName();
 CThingMarker*marker;
 if(PeekThingSearchTools().GetPFirstLeafThing(GFFilter_Bind2(CThingFilter_HasDefName("REGION_KICK_TO_POINT"),CThingFilter_HasScriptData(currentRegion)),&marker)){}else{
  for(std::map<CCharString,MirroredCriminalStatus>::iterator it=OutstandingCrimes.begin();it!=OutstandingCrimes.end();++it){
   long fine,framesLeft;
   GetOutstandingFineAndFramesLeft((*it).first,fine,framesLeft);
   if(fine==0&&(*it).first!=currentRegion){if((*it).second.LastTimeExited>mostRecentExit)mostRecentExit=(*it).second.LastTimeExited;}
  }
  if(mostRecentExit!=0&&OutstandingCrimes[currentRegion].LastTimeExited>0)OutstandingCrimes[currentRegion].LastTimeExited=GetWorld().GetFrame();
 }
}

#undef private
#undef protected
static void check(bool v){if(!v)abort();}
CBaseClass::CBaseClass(){}CBaseClass::~CBaseClass(){}CBaseClassNonCopyable::CBaseClassNonCopyable(){}CBaseClassNonCopyable::~CBaseClassNonCopyable(){}CIDrawWorldMap::~CIDrawWorldMap(){}
class C3DVector{};class C2DBoxI{};class CDrawMapInfo{};
class TestWorldMap:public CWorldMap{public:long DrawGetNoMaps()const{abort();return 0;}CDrawMapInfo DrawGetMapInfo(long)const{abort();return CDrawMapInfo();}C2DBoxI DrawGetDimensions()const{abort();return C2DBoxI();}long DrawGetGranularity()const{abort();return 0;}bool DrawIsPosInMap(const C3DVector&)const{abort();return false;}float DrawGetSizeZAt(const C3DVector&,float)const{abort();return 0;}C3DVector DrawConvertWorldMapCoordsToLocalCoords(long,const C3DVector&)const{abort();return C3DVector();}long DrawGetMyPlayerMapNumber()const{abort();return 0;}bool DrawFindLastUnblockedGroundPositionAndReportCollision(const C3DVector&,const C3DVector&,C3DVector&)const{abort();return false;}bool DrawFindLastUnblockedThemePositionAndReportCollision(const C3DVector&,const C3DVector&,C3DVector&)const{abort();return false;}long DrawGetFrame()const{abort();return 0;}};
struct TextStore{char*text;int refs;};static int strings,searches,mapServices,loadedServices,regionServices,worldServices,frameServices,fineCalls,indexCalls;static long now;static CTCHeroStats*hero;static CRegion*region;static CWorldMap*worldMap;static CWorld*world;static CThingSearchTools*searchTools;
static const char*text(const CCharString&s){return reinterpret_cast<TextStore*>(s.PStringData)->text;}
CCharString::CCharString(const char*t,long n){if(n<0)n=strlen(t);TextStore*p=new TextStore;p->text=new char[n+1];memcpy(p->text,t,n);p->text[n]=0;p->refs=1;PStringData=reinterpret_cast<CCharStringData*>(p);++strings;}
CCharString::CCharString(const CCharString&s):PStringData(s.PStringData){++reinterpret_cast<TextStore*>(PStringData)->refs;}
CCharString::~CCharString(){TextStore*p=reinterpret_cast<TextStore*>(PStringData);if(--p->refs==0){delete[]p->text;delete p;--strings;}}
bool CCharString::operator!=(const CCharString&s)const{return strcmp(text(*this),text(s))!=0;}
CWorld&CTCBase::GetWorld()const{check(this==static_cast<CTCBase*>(hero));++worldServices;return*world;}
const CWorldMap&CTCBase::PeekWorldMap()const{check(this==static_cast<CTCBase*>(hero));++mapServices;return*worldMap;}
const CThingSearchTools&CTCBase::PeekThingSearchTools()const{check(this==static_cast<CTCBase*>(hero));return*searchTools;}
long __fastcall CWorld::GetFrame(){++frameServices;return now;}
long CWorldMap::GetLoadedRegion()const{check(this==worldMap);++loadedServices;return 2;}
const CRegion&CWorldMap::GetRegion(long n)const{check(this==worldMap&&n==2);++regionServices;return*region;}
class CThingMarker:public CThing{};
static std::list<CClass*>things;static CThingMarker markers[3];static bool scanned[3],rightDef[3],rightData[3];
static int markerIndex(const CThing*p){for(int i=0;i<3;++i)if(p==&markers[i])return i;abort();return 0;}
bool CThing::IsIncludedInThingScans()const{return scanned[markerIndex(this)];}
bool CThingFilter_HasDefName::operator()(const CThing&t)const{check(strcmp(text(Name),"REGION_KICK_TO_POINT")==0);return rightDef[markerIndex(&t)];}
bool CThingFilter_HasScriptData::operator()(const CThing&t)const{check(strcmp(text(Data),"Current")==0);return rightData[markerIndex(&t)];}
const std::list<CClass*>&CThingSearchTools::PeekTypeList(long type)const{check(this==searchTools&&type==11);++searches;return things;}
static long fines[3];
void CTCHeroStats::GetOutstandingFineAndFramesLeft(const CCharString&name,long&f,long&frames){check(this==hero);int i=strcmp(text(name),"Current")==0?0:strcmp(text(name),"Other1")==0?1:2;f=fines[i];frames=17;++fineCalls;}
namespace std{
template<class T,class A>_Vector_base<T,A>::_Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}template<class T,class A>_Vector_base<T,A>::~_Vector_base(){}
template<class V,class A>_Rb_tree_base<V,A>::_Rb_tree_base(const allocator_type&a):_M_header(_M_node_allocator_type(a),0){}template<class V,class A>_Rb_tree_base<V,A>::~_Rb_tree_base(){}
template<class D>typename _Rb_global<D>::_Base_ptr __cdecl _Rb_global<D>::_M_increment(_Base_ptr x){if(x->_M_right){x=x->_M_right;while(x->_M_left)x=x->_M_left;}else{_Base_ptr y=x->_M_parent;while(x==y->_M_right){x=y;y=y->_M_parent;}if(x->_M_right!=y)x=y;}return x;}
template<class K,class V,class C,class A>V&map<K,V,C,A>::operator[](const K&key){++indexCalls;for(iterator it=begin();it!=end();++it)if(!((*it).first!=key))return(*it).second;abort();return(*begin()).second;}
}
typedef CTCHeroStats::MirroredCriminalStatus Status;typedef std::pair<const CCharString,Status>Value;typedef std::_Rb_tree_node<Value>Node;
int main(){unsigned cases=0;{
 CThing thing;CTCHeroStats h(thing);hero=&h;CRegion r;region=&r;TestWorldMap map;worldMap=&map;CWorld w;world=&w;CThingSearchTools search;searchTools=&search;
 Status status;memset(&status,0,sizeof status);CCharString key0("Current"),key1("Other1"),key2("Other2");Node n0(Value(key0,status)),n1(Value(key1,status)),n2(Value(key2,status));Node*nodes[]={&n0,&n1,&n2};std::_Rb_tree_node_base sentinel;
 h.OutstandingCrimes._M_t._M_header._M_data=reinterpret_cast<Node*>(&sentinel);
 std::_List_node<CClass*>listHead,listNodes[3];things._M_node._M_data=&listHead;
 const long exits[]={-3,0,5,100};const long frames[]={0,73,1000};
 for(unsigned markerCase=0;markerCase<8;++markerCase)for(unsigned empty=0;empty<2;++empty)for(unsigned fineMask=0;fineMask<4;++fineMask)for(unsigned current=0;current<4;++current)for(unsigned other1=0;other1<4;++other1)for(unsigned other2=0;other2<4;++other2)for(unsigned frame=0;frame<3;++frame){
  listHead._M_next=&listNodes[0];listHead._M_prev=&listNodes[2];bool found=false;
  for(unsigned i=0;i<3;++i){listNodes[i]._M_next=i==2?&listHead:&listNodes[i+1];listNodes[i]._M_prev=i?&listNodes[i-1]:&listHead;listNodes[i]._M_data=reinterpret_cast<CClass*>(&markers[i]);scanned[i]=true;rightDef[i]=(i==1);rightData[i]=(i==1);}
  if(markerCase==0)listHead._M_next=listHead._M_prev=&listHead;
  if(markerCase==1)scanned[1]=false;if(markerCase==2)rightDef[1]=false;if(markerCase==3)rightData[1]=false;
  if(markerCase>=4){rightDef[1]=rightData[1]=false;unsigned pick=(markerCase-4)%3;rightDef[pick]=rightData[pick]=true;found=true;}
  memset(&sentinel,0,sizeof sentinel);sentinel._M_color=true;
  if(empty)sentinel._M_left=sentinel._M_right=&sentinel;else{sentinel._M_parent=&n1;sentinel._M_left=&n0;sentinel._M_right=&n2;for(int i=0;i<3;++i){nodes[i]->_M_left=nodes[i]->_M_right=0;nodes[i]->_M_parent=&n1;}n1._M_parent=&sentinel;n1._M_left=&n0;n1._M_right=&n2;}
  n0._M_value_field.second.LastTimeExited=exits[current];n1._M_value_field.second.LastTimeExited=exits[other1];n2._M_value_field.second.LastTimeExited=exits[other2];fines[0]=9;fines[1]=(fineMask&1)?5:0;fines[2]=(fineMask&2)?7:0;now=frames[frame];
  bool eligible=(!fines[1]&&exits[other1]>0)||(!fines[2]&&exits[other2]>0);bool update=!found&&!empty&&eligible&&exits[current]>0;unsigned expectedIndex=(!found&&!empty&&eligible)?(update?2:1):0;
  searches=mapServices=loadedServices=regionServices=worldServices=frameServices=fineCalls=indexCalls=0;int before=strings;
  h.PrepareToKickHeroToMostRecentSafeRegion();check(strings==before&&searches==1&&mapServices==2&&loadedServices==1&&regionServices==1);check(fineCalls==(!found&&!empty?3:0)&&indexCalls==(int)expectedIndex&&worldServices==(update?1:0)&&frameServices==(update?1:0));check(n0._M_value_field.second.LastTimeExited==(update?now:exits[current]));check(n1._M_value_field.second.LastTimeExited==exits[other1]&&n2._M_value_field.second.LastTimeExited==exits[other2]);++cases;
 }
 }check(strings==0&&cases==12288);puts("0057de57_TEST PASS");return 0;
}