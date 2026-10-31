#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __declspec(x)
#endif
#include <stdio.h>
#include <new>
// 006f50b0 std::_Rb_tree::erase
// std::_Rb_tree<unsigned64,pair<constunsigned64,CCombatantInfo> >::erase key
// Leaf: ?erase@?$_Rb_tree@_KU?$pair@$$CB_KUCCombatantInfo@@@std@@U?$_Select1st@U?$pair@$$CB_KUCCombatantInfo@@@std@@@2@U?$less@_K@2@V?$allocator@U?$pair@$$CB_KUCCombatantInfo@@@std@@@2@@std@@QAEIAB_K@Z
// Flags:
class CThing;class CThingCreatureBase;class CCombatSequenceBase;class CAttackPatternDef;class CDefPointeeBase;// Layout: original CBaseObjectPointer is classV; private PData is one pointer after vptr. Native erase works on full original typed tree nodes; donor4byte container shrink applied only where original retail offsets prove it.
class CBaseObjectPointer;class CCPPointerInfo;struct CCombatantInfo;
#ifdef _MSC_VER
typedef unsigned __int64 _TreeKey;
#else
typedef unsigned int _TreeKey __attribute__((mode(DI),aligned(8)));
#endif
#include <stddef.h>
namespace std {
typedef ::size_t size_t;typedef ::ptrdiff_t ptrdiff_t;
struct __true_type {};struct __false_type {};
template<class T>class allocator {public:typedef T value_type;typedef T*pointer;typedef const T*const_pointer;typedef T&reference;typedef const T&const_reference;typedef size_t size_type;typedef ptrdiff_t difference_type;template<class U>struct rebind {typedef allocator<U>other;};allocator(){}allocator(const allocator&){}template<class U>allocator(const allocator<U>&){}~allocator(){}};
template<class T,class A>struct _Alloc_traits {typedef A _Orig;typedef A allocator_type;};template<class T,class U>struct _Alloc_traits<T,allocator<U> > {typedef allocator<U>_Orig;typedef allocator<T>allocator_type;};
template<class V,class T,class A>class _STLP_alloc_proxy:public A {typedef A _Base;typedef _STLP_alloc_proxy<V,T,A>_Self;public:V _M_data;_STLP_alloc_proxy(const A&a,V p):A(a),_M_data(p){}};
template<class A,class R>struct unary_function {typedef A argument_type;typedef R result_type;};template<class A,class B,class R>struct binary_function {typedef A first_argument_type;typedef B second_argument_type;typedef R result_type;};
template<class T>struct less:public binary_function<T,T,bool>{bool operator()(const T&a,const T&b)const{return a<b;}};
template<class T>struct _Identity:public unary_function<T,T>{const T&operator()(const T&v)const{return v;}};
template<class P>struct _Select1st:public unary_function<P,typename P::first_type>{const typename P::first_type&operator()(const P&v)const{return v.first;}};
template<class A,class B>struct pair {typedef A first_type;typedef B second_type;A first;B second;pair():first(A()),second(B()){}pair(const A&a,const B&b):first(a),second(b){}template<class C,class D>pair(const pair<C,D>&p):first(p.first),second(p.second){}};
struct input_iterator_tag {};struct forward_iterator_tag:public input_iterator_tag {};struct bidirectional_iterator_tag:public forward_iterator_tag {};
template<class T>struct _Nonconst_traits {typedef T value_type;typedef T*pointer;typedef T&reference;};template<class T>struct _Const_traits {typedef T value_type;typedef const T*pointer;typedef const T&reference;typedef _Nonconst_traits<T>_Non_const_traits;};
template<class I>struct iterator_traits {typedef typename I::value_type value_type;typedef typename I::pointer pointer;typedef typename I::reference reference;typedef typename I::iterator_category iterator_category;typedef typename I::difference_type difference_type;};
struct _Rb_tree_node_base {typedef _Rb_tree_node_base*_Base_ptr;bool _M_color;_Base_ptr _M_parent,_M_left,_M_right;};
template<class V>struct _Rb_tree_node:public _Rb_tree_node_base {V _M_value_field;};
template<class Dummy>class _Rb_global {public:typedef _Rb_tree_node_base*_Base_ptr;static _Base_ptr __cdecl _M_increment(_Base_ptr);};typedef _Rb_global<bool>_Rb_global_inst;
struct _Rb_tree_base_iterator {typedef _Rb_tree_node_base*_Base_ptr;typedef bidirectional_iterator_tag iterator_category;typedef ptrdiff_t difference_type;_Base_ptr _M_node;bool operator==(const _Rb_tree_base_iterator&x)const{return _M_node==x._M_node;}bool operator!=(const _Rb_tree_base_iterator&x)const{return _M_node!=x._M_node;}};
template<class V,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator {typedef V value_type;typedef typename Traits::reference reference;typedef typename Traits::pointer pointer;typedef _Rb_tree_iterator<V,Traits>_Self;typedef _Rb_tree_node<V>*_Link_type;_Rb_tree_iterator(){_M_node=0;}_Rb_tree_iterator(_Link_type x){_M_node=x;}_Rb_tree_iterator(const _Rb_tree_iterator<V,_Nonconst_traits<V> >&x){_M_node=x._M_node;}reference operator*()const{return static_cast<_Link_type>(_M_node)->_M_value_field;}_Self&operator++(){_M_node=_Rb_global_inst::_M_increment(_M_node);return *this;}};
#pragma auto_inline(off)
template<class I>__declspec(noinline) typename iterator_traits<I>::difference_type __cdecl __distance(const I&first,const I&last,const input_iterator_tag&){typename iterator_traits<I>::difference_type n=0;I it(first);while(it!=last){++it;++n;}return n;}
#pragma auto_inline(on)
template<class I>inline typename iterator_traits<I>::difference_type __cdecl distance(const I&first,const I&last){return __distance(first,last,typename iterator_traits<I>::iterator_category());}
template<class T,class A>struct _Rb_tree_base {typedef _Rb_tree_node<T>_Node;typedef typename _Alloc_traits<T,A>::allocator_type allocator_type;_Rb_tree_base(const allocator_type&);~_Rb_tree_base();protected:typedef typename _Alloc_traits<_Node,A>::allocator_type _M_node_allocator_type;_STLP_alloc_proxy<_Node*,_Node,_M_node_allocator_type>_M_header;};
template<class K,class V,class Select,class Compare,class A=allocator<V> >class _Rb_tree:public _Rb_tree_base<V,A> {typedef _Rb_tree_base<V,A>_Base;protected:typedef _Rb_tree_node_base*_Base_ptr;typedef _Rb_tree_node<V>_Node;public:typedef K key_type;typedef V value_type;typedef V*pointer;typedef const V*const_pointer;typedef V&reference;typedef const V&const_reference;typedef _Rb_tree_node<V>*_Link_type;typedef size_t size_type;typedef ptrdiff_t difference_type;typedef bidirectional_iterator_tag _Iterator_category;typedef typename _Base::allocator_type allocator_type;protected:size_type _M_node_count;Compare _M_key_compare;
 _Link_type&_M_root()const{return (_Link_type&)this->_M_header._M_data->_M_parent;}
 static _Link_type&__cdecl _S_left(_Link_type x){return (_Link_type&)x->_M_left;}
 static _Link_type&__cdecl _S_right(_Link_type x){return (_Link_type&)x->_M_right;}
 static reference __cdecl _S_value(_Link_type x){return x->_M_value_field;}
 static const K&__cdecl _S_key(_Link_type x){return Select()(_S_value(x));}
private:
 _Link_type _M_lower_bound(const key_type&key)const{_Link_type y=this->_M_header._M_data;_Link_type x=_M_root();while(x){if(!_M_key_compare(_S_key(x),key)){y=x;x=_S_left(x);}else x=_S_right(x);}return y;}
 _Link_type _M_upper_bound(const key_type&key)const{_Link_type y=this->_M_header._M_data;_Link_type x=_M_root();while(x){if(_M_key_compare(key,_S_key(x))){y=x;x=_S_left(x);}else x=_S_right(x);}return y;}
public:typedef _Rb_tree_iterator<V,_Nonconst_traits<V> >iterator;typedef _Rb_tree_iterator<V,_Const_traits<V> >const_iterator;_Rb_tree();~_Rb_tree();iterator lower_bound(const key_type&key){return iterator(_M_lower_bound(key));}iterator upper_bound(const key_type&key){return iterator(_M_upper_bound(key));}__declspec(noinline) pair<iterator,iterator>equal_range(const key_type&key){return pair<iterator,iterator>(lower_bound(key),upper_bound(key));}void erase(iterator,iterator);size_type erase(const key_type&key){pair<iterator,iterator>p=equal_range(key);size_type n=distance(p.first,p.second);erase(p.first,p.second);return n;}};
}



namespace std {
template<class T,class A>class _Vector_base {public:typedef typename _Alloc_traits<T,A>::allocator_type allocator_type;_Vector_base(const A&);~_Vector_base();protected:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A> {public:typedef typename _Vector_base<T,A>::allocator_type allocator_type;explicit vector(const allocator_type&a=allocator_type());vector(const vector&);~vector();};
template<class K,class T,class C=less<K>,class A=allocator<pair<const K,T> > >class map {private:typedef _Rb_tree<K,pair<const K,T>,_Select1st<pair<const K,T> >,C,A>_Rep_type;_Rep_type _M_t;public:map();map(const map&);~map();};
}
 #pragma pack(push,1)
class C2DVector {public:float X,Y;};
class C3DVector {public:float X,Y,Z;C3DVector();C3DVector(const C3DVector&);};
#pragma pack(pop)
class CBaseIntelligentPointer {private:CBaseObjectPointer*PData;public:CBaseIntelligentPointer();virtual ~CBaseIntelligentPointer();};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer {};
#define FABLE_RETAIL_DEF_CLASS_BASE
// Embedded dependency: engine/CDefPointer.h
#ifndef FABLE_ENGINE_CDEFPOINTER_H
#define FABLE_ENGINE_CDEFPOINTER_H
// Retail model (hand-written 2026-10-30, class-model migration wave 2): the counted def handle CDefPointer<T>.
// Layout: Ego_r.pdb CDefPointer<T> is 4 bytes, `CDefPointeeBase* Object` at +0 (ghidra_out/struct_layouts_egor.tsv; every
// instantiation). Ego_r inlines every member; the member set is FableWin.pdb's (debug, nothing inlined):
//   ??0..QAE@W4ENullDef@@@Z (332 instantiations), copy ??0..QAE@ABV0@@Z (52), the template converting ctor ??$?0 (e.g.
//   CDefPointer<const CDefClassBase>::CDefPointer<CDefClassBase>(const CDefPointer<CDefClassBase>&)), ??1 (249), ??4 (31),
//   the template ??$?4 operator=<U>, ?Release@..QAEXXZ (249), ?InternalGetObject@..QBEPAVCDefPointeeBase@@XZ (245),
//   ??$SetDynamicCast@$$CBVCDefClassBase@@@..QAEXABV?$CDefPointer@$$CBVCDefClassBase@@@@@Z (246), ??C operator-> (229), ??D
//   operator* (42), ?Get@..QBEPBV..XZ (73), ?IsNull@..QBE_NXZ (42), ?Reset@..QAEXPBV..@Z (18), ?GetRefCount@..QBEKXZ (1).
// Inline bodies (retail bytes): retail expands all of them. One template gives both release shapes the landed GetSubDef<T>
// rows show (0x008C79B0 and family): the manager's CDefPointer<CDefClassBase> temporary releases as `mov edi,[eax+4]; ...
// dec edi; ... mov [eax],edi; jne`, the CDefPointer<const T> assignment and destructor as `dec dword ptr [ecx+4]; jne`;
// Release() must hold the pointee in a local (the temporary's Delete call reuses ecx; reloading Object differs).
// The assignment shape (`cmp; test; dec [ecx+4]; jne; call [eax+4]; and [esi],0; inc [eax+4]`) is the out-of-line
// operator= rows (e.g. 0x00592844). SetDynamicCast<U>: FableWin.pdb pairs it with GFDebugPDynamicCast<CDefPointeeBase*,
// const T*> (a debug-only type check); release code is the plain assignment.
// Forced inline: retail never calls a CDefPointer member out of line (no relocation binding names one; the four
// out-of-line operator= bodies, e.g. 0x00592844, are unreferenced COMDATs), size-optimised TUs included (landed GetSubDef<T>
// rows 0x007BE88C / 0x007BE92E: plain `inline` is called out of line there, 112 vs 162 bytes); __forceinline reproduces that.
// ENullDef constructor (added 2026-10-31, owner request): FableWin.pdb's ??0?$CDefPointer@..@@QAE@W4ENullDef@@@Z (332
// instantiations) is the null constructor; no ??0..@@QAE@XZ default constructor exists in either PDB although classes
// default-construct CDefPointer members, so the enum parameter carries a default argument (inferred from that absence).
// The enumerator's name is in neither PDB name list: NULL_DEF is a placeholder spelling (naming queue), the enum's type
// name ENullDef is the PDB's. Retail bytes: `and/mov dword ptr [..], 0` — the body is `Object = 0`. Unproven: Reset's body.
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#error engine/CDefPointer.h is the retail def model: define FABLE_RETAIL_DEF_CLASS_BASE before including it
#else
// Embedded dependency: rebuild_abi.h


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
#define FABLE_STATIC_ASSERT(expression) \


// Embedded dependency: engine/CDefPointeeBase.h
#ifndef FABLE_ENGINE_CDEFPOINTEEBASE_H
#define FABLE_ENGINE_CDEFPOINTEEBASE_H
// Retail layout (hand-written; no Ego_r layout row: the class adds no members). RTTI: CDefPointeeBase : CResource.
// Vtable 0x01230BA0: slot 0 scalar deleting dtor 0x0042D9A0, slots 1-9 inherited from CResource unchanged.
// Only the FABLE_RETAIL_DEF_CLASS_BASE opt-in uses it (included by engine/CDefClassBase.h). The dtor is inline: 0x0042D9A0
// stores this vtable and calls ~CResource 0x009FC550; only FableWin.pdb (debug) has ??1CDefPointeeBase@@UAE@XZ.
#include <stddef.h>

// Embedded dependency: engine/CResource.h
#ifndef FABLE_ENGINE_CRESOURCE_H
#define FABLE_ENGINE_CRESOURCE_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py then hand-corrected to retail (RETAIL_MARK below).
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CResource = 0x1c.
// Retail evidence: 1 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
// Retail opt-in: define FABLE_RETAIL_DEF_CLASS_BASE for the polymorphic retail class (RTTI CResource :
// CIVCountedPointeeBase; vtable 0x012354A4, slots 2-9 below with their retail addresses; Ego_r.pdb signatures).
#ifdef FABLE_RETAIL_DEF_CLASS_BASE
#include <stddef.h>

// Embedded dependency: engine/CIVCountedPointeeBase.h
#ifndef FABLE_ENGINE_CIVCOUNTEDPOINTEEBASE_H
#define FABLE_ENGINE_CIVCOUNTEDPOINTEEBASE_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py then hand-corrected to retail (RETAIL_MARK below).
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CIVCountedPointeeBase = 0x8.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
// Retail opt-in: define FABLE_RETAIL_DEF_CLASS_BASE before the include for the polymorphic retail class (vtable
// 0x012354D0: slot 0 scalar deleting dtor 0x004664E0, slot 1 Delete 0x0042D930). The dtor is inline (0x004664E0 is only the
// vtable store + conditional delete; ~CResource 0x009FC550 ends with the same store; only FableWin.pdb has the ??1 symbol).
// The const reference-count accessors AddRef / Release (returns the new count, long) / GetRefCount are FableWin.pdb-only
// (Ego_r and retail inline them everywhere; no retail body). Retail shapes: `inc [eax+4]`, `dec [ecx+4]; jne` and, for the
// CDefPointer<CDefClassBase> temporary, `mov edi,[eax+4]; dec edi; mov [eax],edi` (engine/CDefPointer.h; landed GetSubDef<T>
// rows, wave 2). Whether Lionhead wrote IVRefCount `mutable` or const_cast in the accessors is not visible in the bytes (the
// two compile identically); `mutable` follows the landed local models.
// The flattened layout below stays the
// default so the landed includers are unchanged.
#ifdef FABLE_RETAIL_DEF_CLASS_BASE
#include <stddef.h>
class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase() { }                                 // slot 0  0x004664E0 (scalar deleting; inline dtor)
    virtual void Delete() const;                                         // slot 1  0x0042D930
    void AddRef() const { ++IVRefCount; }                                // FableWin.pdb ?AddRef@CIVCountedPointeeBase@@QBEXXZ (inline)
    long Release() const { return --IVRefCount; }                        // FableWin.pdb ?Release@CIVCountedPointeeBase@@QBEJXZ (inline)
    long GetRefCount() const { return IVRefCount; }                      // FableWin.pdb ?GetRefCount@CIVCountedPointeeBase@@QBEJXZ (inline)
    mutable unsigned long IVRefCount;                                    // +0x04 (mutable: the const accessors count; see above)
};
FABLE_STATIC_ASSERT(sizeof(CIVCountedPointeeBase) == 0x8);
#else // flattened Ego_r layout (the landed default)
#include <stddef.h>


#pragma pack(push, 1)
class CIVCountedPointeeBase {
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned long IVRefCount;                                // +0x04
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CIVCountedPointeeBase) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CIVCountedPointeeBase, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CIVCountedPointeeBase, IVRefCount) == 0x4);

#endif // FABLE_RETAIL_DEF_CLASS_BASE

#endif // FABLE_ENGINE_CIVCOUNTEDPOINTEEBASE_H


class CResourceList;
class CResource : public CIVCountedPointeeBase {
public:
    CResource();                                                         // 0x009FBEC0 (??0CResource@@QAE@XZ, bound)
    virtual ~CResource();                                                // slot 0  0x004664FF (scalar deleting); ~CResource 0x009FC550
    virtual bool UnloadResource();                                       // slot 2  0x0042D940
    virtual unsigned long GetUnloadableMemory() const;                   // slot 3  0x009FBEF0
    virtual bool IsResourceLoaded() const;                               // slot 4  0x0042D950
    virtual void OnResourceLoaded();                                     // slot 5  0x0042D960
    virtual bool CanAbortResourceLoad(long) const;                       // slot 6  0x0042D970
    virtual void OnResourceLoadAborted();                                // slot 7  0x0042D980
    virtual unsigned long GetDebugMemoryInUse() const;                   // slot 8  0x009FBF00
    virtual void ResetDebugMemoryInUse();                                // slot 9  0x009FBF10
    CResourceList* ResourceList;                                         // +0x08
    CResource*     PrevResource;                                         // +0x0c
    CResource*     NextResource;                                         // +0x10
    unsigned long  ResourceSize;                                         // +0x14
    unsigned long  LastUsedFrame;                                        // +0x18
};
FABLE_STATIC_ASSERT(sizeof(CResource) == 0x1c);
#else // flattened Ego_r layout (the landed default)
#include <stddef.h>


class CResourceList;

#pragma pack(push, 1)
class CResource {
public:
    void*          __vftable;                                // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char  _pad_0x04[0x4];                           // +0x04
    CResourceList* ResourceList;                             // +0x08
    CResource*     PrevResource;                             // +0x0c
    CResource*     NextResource;                             // +0x10
    unsigned long  ResourceSize;                             // +0x14
    unsigned long  LastUsedFrame;                            // +0x18
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CResource) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CResource, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CResource, ResourceList) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CResource, PrevResource) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CResource, NextResource) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CResource, ResourceSize) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CResource, LastUsedFrame) == 0x18);

#endif // FABLE_RETAIL_DEF_CLASS_BASE

#endif // FABLE_ENGINE_CRESOURCE_H



class CDefPointeeBase : public CResource {
public:
    virtual ~CDefPointeeBase() { }                                       // slot 0  0x0042D9A0 (scalar deleting; inline dtor)
};
FABLE_STATIC_ASSERT(sizeof(CDefPointeeBase) == 0x1c);

#endif // FABLE_ENGINE_CDEFPOINTEEBASE_H


#if defined(_MSC_VER)
#define FABLE_DEFPTR_INLINE __forceinline
#else
#define FABLE_DEFPTR_INLINE inline
#endif

enum ENullDef { NULL_DEF };                                          // W4ENullDef@@ (global); enumerator name unproven

template <class T> class CDefPointer {
public:
    FABLE_DEFPTR_INLINE CDefPointer(ENullDef = NULL_DEF) : Object(0) {}             // ??0..QAE@W4ENullDef@@@Z
    FABLE_DEFPTR_INLINE CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    template <class U> FABLE_DEFPTR_INLINE CDefPointer(const CDefPointer<U>& other) : Object(other.InternalGetObject())
    {
        if (Object) Object->AddRef();
    }
    FABLE_DEFPTR_INLINE ~CDefPointer() { Release(); }
    FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer& other) { Assign(other.Object); return *this; }
    template <class U> FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); return *this; }
    template <class U> FABLE_DEFPTR_INLINE void SetDynamicCast(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); }
    CDefPointeeBase* InternalGetObject() const { return Object; }
    T* Get() const { return static_cast<T*>(Object); }
    T* operator->() const { return Get(); }
    T& operator*() const { return *Get(); }
    bool IsNull() const { return Object == 0; }
    unsigned long GetRefCount() const { return Object->GetRefCount(); }
    void Reset(T*);                                                      // unproven body (FableWin ?Reset@..QAEXPBV..@Z)
    FABLE_DEFPTR_INLINE void Release()
    {
        if (Object) {
            CDefPointeeBase* object = Object;
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
    CDefPointeeBase* Object;                                             // +0x00
private:
    FABLE_DEFPTR_INLINE void Assign(CDefPointeeBase* object)                                 // the inline body of operator= / SetDynamicCast
    {
        if (Object != object) {
            Release();
            Object = object;
            if (Object) Object->AddRef();
        }
    }
};
FABLE_STATIC_ASSERT(sizeof(CDefPointer<CDefPointeeBase>) == 0x4);

#endif // FABLE_RETAIL_DEF_CLASS_BASE
#endif // FABLE_ENGINE_CDEFPOINTER_H


// Embedded dependency: engine/CCountedPointer.h
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

#endif // FABLE_ENGINE_CCOUNTEDPOINTER_H


class CAttackingCombatantInfo {public:CThingCreatureBase*Attacked;long AttackerNumber,CombatWheelRingIndex;bool PreferredAttackDollySet;long PreferredAttackDollyTimer;C2DVector PreferredAttackDollyVector;bool UsingCombatWheel;long CurrentSequence;std::vector<CCountedPointer<CCombatSequenceBase> >CombatSequences;CAttackingCombatantInfo();CAttackingCombatantInfo(const CAttackingCombatantInfo&);~CAttackingCombatantInfo();};
class CDefendingCombatantInfo {public:class CCombatRing;class CCombatWheel {private:std::vector<CCombatRing>CombatRings;};std::map<_TreeKey,CThingCreatureBase*>Attackers;CCombatWheel CombatWheel;C3DVector LastVictimWheelUpdatePos;long LastWheelUpdateWorldFrame;CIntelligentPointer<CThing>PLastAttacker;CDefPointer<const CAttackPatternDef>PAttackPattern;long AttackPatternIndex,FrameForNextInstruction;CDefendingCombatantInfo();CDefendingCombatantInfo(const CDefendingCombatantInfo&);~CDefendingCombatantInfo();};
struct CCombatantInfo {CThingCreatureBase*Parent;CAttackingCombatantInfo AttackerInfo;CDefendingCombatantInfo DefenderInfo;CCombatantInfo();CCombatantInfo(const CCombatantInfo&);~CCombatantInfo();};

typedef std::pair<const _TreeKey,CCombatantInfo> _TreeValue;
namespace std {template _Rb_tree<_TreeKey,_TreeValue,_Select1st<_TreeValue>,less<_TreeKey> >::size_type _Rb_tree<_TreeKey,_TreeValue,_Select1st<_TreeValue>,less<_TreeKey> >::erase(const _TreeKey&);}

// Dependency setup mocks only. Erase/search/count use the exact source bodies above;
// no combat object operations are called by those tested original functions.
namespace std {template<class T,class A>_Vector_base<T,A>::_Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,static_cast<T*>(0)){}template<class T,class A>_Vector_base<T,A>::~_Vector_base(){}template<class T,class A>vector<T,A>::vector(const allocator_type&a):_Vector_base<T,A>(a){}template<class T,class A>vector<T,A>::vector(const vector&):_Vector_base<T,A>(A()){}template<class T,class A>vector<T,A>::~vector(){}template<class K,class T,class C,class A>map<K,T,C,A>::map(){}template<class K,class T,class C,class A>map<K,T,C,A>::map(const map&){}template<class K,class T,class C,class A>map<K,T,C,A>::~map(){}}
C3DVector::C3DVector():X(0),Y(0),Z(0){}C3DVector::C3DVector(const C3DVector&o):X(o.X),Y(o.Y),Z(o.Z){}
CBaseIntelligentPointer::CBaseIntelligentPointer():PData(0){}CBaseIntelligentPointer::~CBaseIntelligentPointer(){}
CAttackingCombatantInfo::CAttackingCombatantInfo(){}CAttackingCombatantInfo::CAttackingCombatantInfo(const CAttackingCombatantInfo&){}CAttackingCombatantInfo::~CAttackingCombatantInfo(){}
CDefendingCombatantInfo::CDefendingCombatantInfo(){}CDefendingCombatantInfo::CDefendingCombatantInfo(const CDefendingCombatantInfo&){}CDefendingCombatantInfo::~CDefendingCombatantInfo(){}
CCombatantInfo::CCombatantInfo():Parent(0){}CCombatantInfo::CCombatantInfo(const CCombatantInfo&):Parent(0){}CCombatantInfo::~CCombatantInfo(){}
static std::_Rb_tree_node<_TreeValue> header,node1,node2;static int totalNodes,expectedRemoved,eraseCalls,increments;static bool bad;static void*expectedOwner;
namespace std {
template<class T,class A>_Rb_tree_base<T,A>::_Rb_tree_base(const allocator_type&a):_M_header(_M_node_allocator_type(a),static_cast<_Node*>(0)){}template<class T,class A>_Rb_tree_base<T,A>::~_Rb_tree_base(){}
template<class K,class V,class S,class C,class A>_Rb_tree<K,V,S,C,A>::_Rb_tree():_Rb_tree_base<V,A>(allocator_type()),_M_node_count(0),_M_key_compare(C()){}template<class K,class V,class S,class C,class A>_Rb_tree<K,V,S,C,A>::~_Rb_tree(){}
template<class D>typename _Rb_global<D>::_Base_ptr __cdecl _Rb_global<D>::_M_increment(_Base_ptr x){++increments;if(x->_M_right){x=x->_M_right;while(x->_M_left)x=x->_M_left;}else{_Base_ptr y=x->_M_parent;while(x==y->_M_right){x=y;y=y->_M_parent;}if(x->_M_right!=y)x=y;}return x;}
template<class K,class V,class S,class C,class A>void _Rb_tree<K,V,S,C,A>::erase(iterator first,iterator last){++eraseCalls;if(this!=expectedOwner||first._M_node!=(expectedRemoved?&node1:&header)||last._M_node!=&header||_M_node_count!=static_cast<size_t>(totalNodes))bad=true;_M_node_count-=expectedRemoved;}
}
// Original dependency constructors are fixture setup mocks; the candidate's equal-range,
// unsigned comparisons, count loop and actual RB successor bodies all execute unchanged.
class TestTree:public std::_Rb_tree<_TreeKey,_TreeValue,std::_Select1st<_TreeValue>,std::less<_TreeKey> >{public:void configure(int n,const _TreeKey&key){totalNodes=n;_M_node_count=n;_M_header._M_data=&header;header._M_color=false;node1._M_color=node2._M_color=true;header._M_left=n?&node1:&header;header._M_right=n==2?&node2:(n?&node1:&header);header._M_parent=n==2?&node2:(n?&node1:0);node1._M_left=node1._M_right=node2._M_right=0;node1._M_parent=n==2?&node2:&header;node2._M_parent=&header;node2._M_left=&node1;node1._M_value_field.~_TreeValue();::new(static_cast<void*>(&node1._M_value_field))_TreeValue(key,CCombatantInfo());node2._M_value_field.~_TreeValue();::new(static_cast<void*>(&node2._M_value_field))_TreeValue(key,CCombatantInfo());}unsigned int size()const{return _M_node_count;}};
struct C2DVectorAlignmentProbe {char Before;C2DVector Value;};
int main(){if(sizeof(C2DVectorAlignmentProbe)!=9||offsetof(C2DVectorAlignmentProbe,Value)!=1)return 89;TestTree tree;expectedOwner=&tree;_TreeKey key=1;key<<=40;key|=7;tree.configure(2,key);expectedRemoved=2;if(tree.erase(key)!=2||eraseCalls!=1||increments!=2||tree.size()||bad)return 1;
tree.configure(2,key);key=1;key<<=63;key|=9;expectedRemoved=0;if(tree.erase(key)!=0||eraseCalls!=2||increments!=2||tree.size()!=2||bad)return 2;
tree.configure(0,key);if(tree.erase(key)!=0||eraseCalls!=3||increments!=2||tree.size()||bad)return 3;
key=~key;tree.configure(1,key);expectedRemoved=1;if(tree.erase(key)!=1||eraseCalls!=4||increments!=3||tree.size()||bad)return 4;if(sizeof(void*)==4 && (sizeof(CAttackingCombatantInfo)!=48||sizeof(CDefendingCombatantInfo)!=60||sizeof(CCombatantInfo)!=112||sizeof(_TreeValue)!=120||sizeof(std::_Rb_tree_node<_TreeValue>)!=136||sizeof(std::_Rb_tree_node_base)!=16||sizeof(TestTree)!=12))return 5;puts("006f50b0_TEST PASS");return 0;}