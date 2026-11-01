#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <new>
#ifndef _MSC_VER
#define __forceinline inline
#define __cdecl
#define __fastcall
#define __declspec(x)
#define __int64 long
#endif
#define private public
#define protected public
// 009c8990 NGameText::CDataBank::GetAllTextEntriesForNarrator
// Leaf: ?GetAllTextEntriesForNarrator@CDataBank@NGameText@@QBEXABVCCharString@@AAV?$set@KU?$less@K@std@@V?$allocator@K@2@@std@@K@Z
// Flags:
#include <new>
#include <string.h>
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
template<class V>struct _Rb_tree_node:public _Rb_tree_node_base{V _M_value_field;};
template<class Dummy>class _Rb_global{public:typedef _Rb_tree_node_base*_Base_ptr;static _Base_ptr __cdecl _M_increment(_Base_ptr);static _Base_ptr __cdecl _M_decrement(_Base_ptr);};
typedef _Rb_global<bool>_Rb_global_inst;
struct _Rb_tree_base_iterator{typedef _Rb_tree_node_base*_Base_ptr;typedef bidirectional_iterator_tag iterator_category;typedef ptrdiff_t difference_type;_Base_ptr _M_node;bool operator==(const _Rb_tree_base_iterator&x)const{return _M_node==x._M_node;}bool operator!=(const _Rb_tree_base_iterator&x)const{return _M_node!=x._M_node;}};
template<class T>struct _Nonconst_traits{typedef T value_type;typedef T&reference;typedef T*pointer;};
template<class T>struct _Const_traits{typedef T value_type;typedef const T&reference;typedef const T*pointer;};
template<class V,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator{
 typedef V value_type;typedef typename Traits::reference reference;typedef typename Traits::pointer pointer;typedef _Rb_tree_iterator<V,Traits>_Self;typedef _Rb_tree_node<V>*_Link_type;
 _Rb_tree_iterator(){_M_node=0;}_Rb_tree_iterator(_Link_type p){_M_node=p;}_Rb_tree_iterator(const _Rb_tree_iterator<V,_Nonconst_traits<V> >&p){_M_node=p._M_node;}
 reference operator*()const{return _Link_type(_M_node)->_M_value_field;}
 _Self&operator++(){_M_node=_Rb_global_inst::_M_increment(_M_node);return*this;}_Self&operator--(){_M_node=_Rb_global_inst::_M_decrement(_M_node);return*this;}
};
template<class V,class A>struct _Rb_tree_base{
 typedef _Rb_tree_node<V>_Node;typedef typename _Alloc_traits<V,A>::allocator_type allocator_type;
 _Rb_tree_base(const allocator_type& = allocator_type());~_Rb_tree_base();
protected:typedef typename _Alloc_traits<_Node,A>::allocator_type _M_node_allocator_type;_STLP_alloc_proxy<_Node*,_Node,_M_node_allocator_type>_M_header;
};
template<class K,class V,class Key,class Compare,class A=allocator<V> >class _Rb_tree:public _Rb_tree_base<V,A>{
 typedef _Rb_tree_base<V,A>_Base;
protected:typedef _Rb_tree_node_base*_Base_ptr;typedef _Rb_tree_node<V>_Node;typedef _Rb_tree_Color_type _Color_type;
public:typedef K key_type;typedef V value_type;typedef V*pointer;typedef const V*const_pointer;typedef V&reference;typedef const V&const_reference;typedef _Rb_tree_node<V>*_Link_type;typedef size_t size_type;typedef ptrdiff_t difference_type;typedef bidirectional_iterator_tag _Iterator_category;typedef typename _Base::allocator_type allocator_type;
protected:size_type _M_node_count;Compare _M_key_compare;
 _Link_type&_M_rightmost()const{return(_Link_type&)this->_M_header._M_data->_M_right;}
 static _Link_type&__cdecl _S_right(_Link_type p){return(_Link_type&)p->_M_right;}
 static reference __cdecl _S_value(_Link_type p){return p->_M_value_field;}
 static const K&__cdecl _S_key(_Link_type p){return Key()(_S_value(p));}
 static _Link_type&__cdecl _S_right(_Base_ptr p){return(_Link_type&)p->_M_right;}
 static reference __cdecl _S_value(_Base_ptr p){return((_Link_type)p)->_M_value_field;}
 static const K&__cdecl _S_key(_Base_ptr p){return Key()(_S_value(_Link_type(p)));}
public:typedef _Rb_tree_iterator<V,_Nonconst_traits<V> >iterator;
private:void _M_erase(_Link_type);iterator _M_insert(_Base_ptr,_Base_ptr,const value_type&,_Base_ptr=0);
public:void clear(){if(_M_node_count){_M_erase((_Link_type)this->_M_header._M_data->_M_parent);this->_M_header._M_data->_M_left=this->_M_header._M_data;this->_M_header._M_data->_M_parent=0;this->_M_header._M_data->_M_right=this->_M_header._M_data;_M_node_count=0;}}size_type size()const{return _M_node_count;}pair<iterator,bool>insert_unique(const value_type&);iterator insert_unique(iterator,const value_type&);
};

template<class T>struct _Identity:public unary_function<T,T>{const T&operator()(const T&v)const{return v;}};
template<class K,class V,class C=less<K>,class A=allocator<pair<const K,V> > >class map{
public:typedef pair<const K,V>value_type;
private:typedef _Rb_tree<K,value_type,_Select1st<value_type>,C,A>_Rep_type;_Rep_type _M_t;
};
template<class K,class C=less<K>,class A=allocator<K> >class set{
private:typedef _Rb_tree<K,K,_Identity<K>,C,A>_Rep_type;_Rep_type _M_t;
public:typedef _Rb_tree_iterator<K,_Const_traits<K> >iterator;void clear(){_M_t.clear();}
pair<iterator,bool>insert(const K&x){pair<typename _Rep_type::iterator,bool>p=_M_t.insert_unique(x);return pair<iterator,bool>(p.first,p.second);}
};
}
// Layout: original char-string data is17 packed bytes (13-byte Data followed by unaccessed reference count).
// Only the address of Data is used here; byte storage preserves the original size without new packing.
template<class T>class CBasicString{unsigned char unobservedStringState[13];public:bool operator==(const T*)const;};
class CCharStringData{public:CBasicString<char>Data;private:unsigned char unobservedReferenceCount[4];};
class CCharString{protected:CCharStringData*PStringData;public:bool operator==(const char*text)const{if(!PStringData){if(strcmp(text,"")==0)return true;return false;}return PStringData->Data==text;}};
namespace std{
template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;public:_Vector_base():_M_start(0),_M_finish(0),_M_end_of_storage(A(),0){}};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:unsigned int size()const{return this->_M_finish-this->_M_start;}const T&operator[](unsigned int n)const{return this->_M_start[n];}};
}
template<class T>class CArray:public std::vector<T>{public:const T&operator[](unsigned int n)const{return std::vector<T>::operator[](n);}};
#define CIVCountedPointeeBase CIVCountedPointeeBaseFields
// Begin shared header engine/CIVCountedPointeeBase.h
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
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CIVCountedPointeeBase) == 0x8);
#endif
#endif
#else // flattened Ego_r layout (the landed default)
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
class CIVCountedPointeeBase {
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned long IVRefCount;                                // +0x04
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CIVCountedPointeeBase) == 0x8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CIVCountedPointeeBase, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CIVCountedPointeeBase, IVRefCount) == 0x4);
#endif
#endif

#endif // FABLE_RETAIL_DEF_CLASS_BASE

#endif // FABLE_ENGINE_CIVCOUNTEDPOINTEEBASE_H

// End shared header engine/CIVCountedPointeeBase.h
#undef CIVCountedPointeeBase
#define CResource CResourceFields
// Begin shared header engine/CResource.h
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
// Begin shared header engine/CIVCountedPointeeBase.h
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
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CIVCountedPointeeBase) == 0x8);
#endif
#endif
#else // flattened Ego_r layout (the landed default)
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
class CIVCountedPointeeBase {
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned long IVRefCount;                                // +0x04
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CIVCountedPointeeBase) == 0x8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CIVCountedPointeeBase, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CIVCountedPointeeBase, IVRefCount) == 0x4);
#endif
#endif

#endif // FABLE_RETAIL_DEF_CLASS_BASE

#endif // FABLE_ENGINE_CIVCOUNTEDPOINTEEBASE_H

// End shared header engine/CIVCountedPointeeBase.h
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
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CResource) == 0x1c);
#endif
#endif
#else // flattened Ego_r layout (the landed default)
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

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CResource) == 0x1c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CResource, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CResource, ResourceList) == 0x8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CResource, PrevResource) == 0xc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CResource, NextResource) == 0x10);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CResource, ResourceSize) == 0x14);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CResource, LastUsedFrame) == 0x18);
#endif
#endif

#endif // FABLE_RETAIL_DEF_CLASS_BASE

#endif // FABLE_ENGINE_CRESOURCE_H

// End shared header engine/CResource.h
#undef CResource
class CIVCountedPointeeBase{private:mutable unsigned long IVRefCount;public:virtual~CIVCountedPointeeBase();virtual void Delete()const;void AddRef()const{++IVRefCount;}long Release()const{return --IVRefCount;}long GetRefCount()const{return IVRefCount;}};
class CResource:public CIVCountedPointeeBase{unsigned char unobservedResourceState[sizeof(CResourceFields)-sizeof(CIVCountedPointeeBase)];};
template<class T>class CIVCountedPointer{private:CIVCountedPointeeBase*Object;public:CIVCountedPointer(T*p=0):Object(const_cast<CIVCountedPointeeBase*>(static_cast<const CIVCountedPointeeBase*>(p))){if(Object)Object->AddRef();}
 CIVCountedPointer(const CIVCountedPointer&p):Object(p.Object){if(Object)Object->AddRef();}
 ~CIVCountedPointer(){Release();}void Release(){if(Object){CIVCountedPointeeBase*object=Object;if(object->Release()==0)object->Delete();Object=0;}}T*Get()const{return static_cast<T*>(Object);}T*operator->()const{return Get();}};
#define CBankFile CBankFileFields
// Begin shared header engine/CBankFile.h
#ifndef FABLE_ENGINE_CBANKFILE_H
#define FABLE_ENGINE_CBANKFILE_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py, then hand-corrected to retail (RETAIL_MARK below).
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CBankFile = 0x174.
// Retail layout: STLport 4.6 containers (CArray/map/multimap 0xc, CVectorMap 0x10), sizeof 0x110. Every member to
// WarningFlag +0xf4 is at the offset the landed CBankFile::ClearEntries 0x009CCC00 model uses (InternalClose
// 0x009CD850 likewise to SymbolCRCIndices +0xd0); ChecksumCache (0x18) ends at 0x110, the CBankFileAsync base
// boundary of ctor 0x009D5F80 and RTTI mdisp CResourceBank@0x164 in CGraphicDataBank / NGameText::CDataBank.
// The Ego_r Dependencies / CurrentEntryDependencies members are absent from the retail (TLC) object.
// FABLE_RETAIL_BANK_FILE (the former opt-in) is now the only layout; defining it is harmless.
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

class CBankFileEntryDependencies;
class CCPPointerInfo;
class CCharStringData;
class CDiskFileWin32;
struct CProgressCallback;
class CWideStringData;

#pragma pack(push, 1)
class CBankFile {
public:
    void*                       __vftable;                   // +0x00 vptr, or first dword of a flattened base subobject
    unsigned long               Size;                        // +0x04
    unsigned char               Symbols[0xc];                // +0x08 CArray<CCharString>
    unsigned char               Checksums[0xc];              // +0x14 CArray<unsigned_long>
    unsigned char               RuntimeData[0xc];            // +0x20 CArray<CBankFile::CRuntimeData>
    unsigned char               UpdateData[0xc];             // +0x2c CArray<CBankFileEntryUpdateData_*>
    unsigned char               PackedDataOffset[0x10];      // +0x38 CPackedUIntArray
    unsigned char               PackedDataSize[0x10];        // +0x48 CPackedUIntArray
    unsigned char               PackedDataType[0x10];        // +0x58 CPackedUIntArray
    unsigned char               PackedValid[0x10];           // +0x68 CPackedUIntArray
    unsigned long               OpenFlags;                   // +0x78
    CDiskFileWin32*             BankFile_Data;               // +0x7c CCountedPointer<CDiskFileWin32>::Data
    CCPPointerInfo*             BankFile_Info;               // +0x80 CCountedPointer<CDiskFileWin32>::Info
    CWideStringData*            Filename_PStringData;        // +0x84 CWideString::PStringData
    CCharStringData*            BankHandle_PStringData;      // +0x88 CCharString::PStringData
    bool                        RetailMode;                  // +0x8c
    bool                        ReadOnly;                    // +0x8d
    bool                        WrittenToFile;               // +0x8e
    bool                        Solid;                       // +0x8f
    unsigned long               ChangeCount;                 // +0x90
    bool                        FileValid;                   // +0x94
    bool                        UpdatingFlag;                // +0x95
    unsigned char               _pad_0x96[0x2];              // +0x96
    unsigned long               Alignment;                   // +0x98
    unsigned long               BankFormatVersion;           // +0x9c
    unsigned long               HeaderSize;                  // +0xa0
    unsigned long               FileInformationSize;         // +0xa4
    unsigned long               FileSize;                    // +0xa8
    unsigned char               SymbolIndices[0xc];          // +0xac map<CCharString,unsigned_long,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,unsigned_long>_>_>
    unsigned char               OldSymbolIndices[0xc];       // +0xb8 map<CCharString,unsigned_long,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,unsigned_long>_>_>
    unsigned char               FilenameIndices[0xc];        // +0xc4 multimap<CCharString,unsigned_long,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,unsigned_long>_>_>
    unsigned char               SymbolCRCIndices[0x10];      // +0xd0 CVectorMap<unsigned_long,unsigned_long,CKeyPairCompareLess<unsigned_long,unsigned_long>_>
    unsigned char               BlockOrder[0xc];             // +0xe0 CArray<unsigned_long>
    CProgressCallback*          ProgressCallback;            // +0xec
    CCharStringData*            HeaderEnumerationType_PStringData; // +0xf0 CCharString::PStringData
    bool                        WarningFlag;                 // +0xf4
    unsigned char               _pad_0xf5[0x3];              // +0xf5
    unsigned char               ChecksumCache[0x18];         // +0xf8 CChecksumCacheFile
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CBankFile) == 0x110);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBankFile, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBankFile, Size) == 0x4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBankFile, Symbols) == 0x8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBankFile, Checksums) == 0x14);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBankFile, RuntimeData) == 0x20);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBankFile, UpdateData) == 0x2c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBankFile, PackedDataOffset) == 0x38);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBankFile, PackedDataSize) == 0x48);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBankFile, PackedDataType) == 0x58);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBankFile, PackedValid) == 0x68);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBankFile, OpenFlags) == 0x78);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBankFile, BankFile_Data) == 0x7c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBankFile, BankFile_Info) == 0x80);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBankFile, Filename_PStringData) == 0x84);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBankFile, BankHandle_PStringData) == 0x88);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBankFile, RetailMode) == 0x8c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBankFile, ReadOnly) == 0x8d);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBankFile, WrittenToFile) == 0x8e);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBankFile, Solid) == 0x8f);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBankFile, ChangeCount) == 0x90);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBankFile, FileValid) == 0x94);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBankFile, UpdatingFlag) == 0x95);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBankFile, Alignment) == 0x98);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBankFile, BankFormatVersion) == 0x9c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBankFile, HeaderSize) == 0xa0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBankFile, FileInformationSize) == 0xa4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBankFile, FileSize) == 0xa8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBankFile, SymbolIndices) == 0xac);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBankFile, OldSymbolIndices) == 0xb8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBankFile, FilenameIndices) == 0xc4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBankFile, SymbolCRCIndices) == 0xd0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBankFile, BlockOrder) == 0xe0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBankFile, ProgressCallback) == 0xec);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBankFile, HeaderEnumerationType_PStringData) == 0xf0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBankFile, WarningFlag) == 0xf4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBankFile, ChecksumCache) == 0xf8);
#endif
#endif

#endif // FABLE_ENGINE_CBANKFILE_H

// End shared header engine/CBankFile.h
#undef CBankFile
class CBankFile:public CBankFileFields{public:unsigned long GetEntryDataType(unsigned long)const;};

#pragma auto_inline(off)
namespace std{
template<class K,class V,class Key,class Compare,class A>
__declspec(noinline)pair<_Rb_tree_iterator<V,_Nonconst_traits<V> >,bool>_Rb_tree<K,V,Key,Compare,A>::insert_unique(const V&v){
 _Link_type y=this->_M_header._M_data;_Link_type x=(_Link_type)y->_M_parent;bool comp=true;
 while(x){y=x;comp=_M_key_compare(Key()(v),_S_key(x));x=(_Link_type)(comp?x->_M_left:x->_M_right);}
 iterator j(y);
 if(comp){if(j._M_node==this->_M_header._M_data->_M_left)return pair<iterator,bool>(_M_insert(y,y,v),true);else --j;}
 if(_M_key_compare(_S_key(j._M_node),Key()(v)))return pair<iterator,bool>(_M_insert(x,y,v),true);
 return pair<iterator,bool>(j,false);
}
}
#pragma auto_inline(on)
namespace NGameText{
class CTextEntry;
class CDataBankEntry{};
class CDataBankTextEntry:public CDataBankEntry{private:CIVCountedPointer<CTextEntry>PText;long NarratorIndex;public:long GetNarratorIndex()const{return NarratorIndex;}};
class CGroupEntry:public CResource{private:CArray<long>Entries;public:const CArray<long>&GetTextEntries()const{return Entries;}};
// Layout: original CBankFileAsync first base and CResourceBank second base flatten into this1e0-byte prefix.
// Original Entries is CArray<CDataBankEntry*>; donor254 becomes native1e0 after container/base shrinkage.
class CDataBank:public CBankFile{private:unsigned char unobservedBankState[0x1e0-sizeof(CBankFile)];CArray<CDataBankEntry*>Entries;
public:long ConvertNarratorNameToIndex(const CCharString&)const;CIVCountedPointer<const CGroupEntry>GetPGroupEntry(unsigned long,bool)const;
 const CDataBankTextEntry*GetTextInfo(unsigned long n)const{if(GetEntryDataType(n)==0)return static_cast<const CDataBankTextEntry*>(Entries[n]);return 0;}
 void GetAllTextEntriesForNarrator(const CCharString&,std::set<unsigned long>&,unsigned long)const;
};


void CDataBank::GetAllTextEntriesForNarrator(const CCharString& narrator, std::set<unsigned long>& entries, unsigned long minIndex) const
{
    entries.clear();
    long narratorIndex = ConvertNarratorNameToIndex(narrator);
    if (narratorIndex == -1 && !(narrator == ""))
        return;
    long count = Entries.size();
    for (long i = 1; i < count; ++i) {
        if (GetEntryDataType(i) == 0) {
            const CDataBankTextEntry* entry = GetTextInfo(i);
            if (entry) {
                long speaker = entry->GetNarratorIndex();
                if ((speaker == narratorIndex || (narrator == "" && speaker == -1)) && (unsigned long)i >= minIndex)
                    entries.insert(i);
            }
        } else if (GetEntryDataType(i) == 1) {
            const CArray<long>& members = GetPGroupEntry(i, true)->GetTextEntries();
            for (unsigned long j = 0; j < members.size(); ++j) {
                const CDataBankTextEntry* entry = GetTextInfo(members[j]);
                if (entry) {
                    long speaker = entry->GetNarratorIndex();
                    if (speaker == narratorIndex || (narrator == "" && speaker == -1)) {
 long value=members[j];if((unsigned long)value>=minIndex)entries.insert((unsigned long)value);
 }
                }
            }
        }
    }
}
}

#undef private
#undef protected
#define CHECK(x) do{if(!(x)){printf("failure line %d\n",__LINE__);exit(1);}}while(0)
static unsigned allocations,frees,cleared,groupCalls,deleted,conversions,comparisons;static long requestedNarrator;static bool emptyName;static const CCharString*inputName;
namespace std{
template<class V,class A>_Rb_tree_base<V,A>::_Rb_tree_base(const allocator_type&a):_M_header(_M_node_allocator_type(a),(_Node*)0){_Node*h=static_cast<_Node*>(malloc(sizeof(_Node)));CHECK(h);++allocations;h->_M_color=false;h->_M_parent=0;h->_M_left=h->_M_right=h;_M_header._M_data=h;}
template<class V,class A>_Rb_tree_base<V,A>::~_Rb_tree_base(){free(_M_header._M_data);++frees;}
template<class K,class V,class Key,class Compare,class A>void _Rb_tree<K,V,Key,Compare,A>::_M_erase(_Link_type p){if(!p)return;_M_erase((_Link_type)p->_M_left);_M_erase((_Link_type)p->_M_right);free(p);++frees;++cleared;}
template<class D>typename _Rb_global<D>::_Base_ptr __cdecl _Rb_global<D>::_M_decrement(_Base_ptr p){if(p->_M_color==false&&p->_M_parent&&p->_M_parent->_M_parent==p)p=p->_M_right;else if(p->_M_left){p=p->_M_left;while(p->_M_right)p=p->_M_right;}else{_Base_ptr q=p->_M_parent;while(p==q->_M_left){p=q;q=q->_M_parent;}p=q;}return p;}
template<class K,class V,class Key,class Compare,class A>typename _Rb_tree<K,V,Key,Compare,A>::iterator _Rb_tree<K,V,Key,Compare,A>::_M_insert(_Base_ptr x,_Base_ptr y,const V&v,_Base_ptr w){
 _Link_type n=static_cast<_Link_type>(malloc(sizeof(_Node)));CHECK(n);++allocations;n->_M_value_field=v;n->_M_color=true;n->_M_parent=y;n->_M_left=n->_M_right=0;
 _Link_type h=this->_M_header._M_data;
 if(y==h||(w==0&&(x!=0||_M_key_compare(Key()(v),_S_key(y))))){y->_M_left=n;if(y==h){h->_M_parent=n;h->_M_right=n;}else if(y==h->_M_left)h->_M_left=n;}else{y->_M_right=n;if(y==h->_M_right)h->_M_right=n;}
 ++_M_node_count;return iterator(n);
}
}
namespace NGameText{class CTextEntry:public CResource{};}
CIVCountedPointeeBase::~CIVCountedPointeeBase(){}void CIVCountedPointeeBase::Delete()const{++deleted;}
template<class T>bool CBasicString<T>::operator==(const T*text)const{CHECK(text[0]==0);++comparisons;return emptyName;}
static unsigned long types[16];static NGameText::CGroupEntry*groups[16];static const NGameText::CDataBank*bank;
unsigned long CBankFile::GetEntryDataType(unsigned long index)const{CHECK(this==bank&&index<16);return types[index];}
long NGameText::CDataBank::ConvertNarratorNameToIndex(const CCharString&name)const{CHECK(this==bank&&&name==inputName);++conversions;return requestedNarrator;}
CIVCountedPointer<const NGameText::CGroupEntry>NGameText::CDataBank::GetPGroupEntry(unsigned long index,bool pending)const{CHECK(this==bank&&index<16&&types[index]==1&&pending&&groups[index]);++groupCalls;return CIVCountedPointer<const CGroupEntry>(groups[index]);}
static void collect(std::_Rb_tree_node_base*p,bool*out){if(!p)return;collect(p->_M_left,out);unsigned long v=static_cast<std::_Rb_tree_node<unsigned long>*>(p)->_M_value_field;CHECK(v<16);out[v]=true;collect(p->_M_right,out);}
int main(){unsigned cases=0;
 {NGameText::CDataBank data;bank=&data;CCharString name;CCharStringData nameData;inputName=&name;
 NGameText::CDataBankTextEntry text[10];NGameText::CDataBankEntry*entries[10];NGameText::CGroupEntry groupA,groupB;
 long membersA[]={1,3,3,4,5,7,8,0};long membersB[]={2,4,7,8,5};
 groupA.Entries._M_start=membersA;groupA.Entries._M_finish=membersA+8;groupA.Entries._M_end_of_storage._M_data=membersA+8;
 groupB.Entries._M_start=membersB;groupB.Entries._M_finish=membersB+5;groupB.Entries._M_end_of_storage._M_data=membersB+5;
 for(unsigned i=0;i<10;++i){entries[i]=&text[i];text[i].NarratorIndex=(long)(i%3)-1;types[i]=0;groups[i]=0;}
 types[2]=types[6]=1;groups[2]=&groupA;groups[6]=&groupB;types[9]=2;
 data.Entries._M_start=entries;data.Entries._M_finish=entries+10;data.Entries._M_end_of_storage._M_data=entries+10;
 std::set<unsigned long>result;result._M_t._M_node_count=0;
 for(unsigned empty=0;empty<3;++empty)for(long narrator=-1;narrator<=2;++narrator)for(unsigned minimum=0;minimum<=11;++minimum)for(unsigned missing=0;missing<4;++missing)for(unsigned count=1;count<=10;++count){
  emptyName=empty!=0;name.PStringData=empty==2?0:&nameData;requestedNarrator=narrator;data.Entries._M_finish=entries+count;
  entries[3]=(missing&1)?0:&text[3];entries[7]=(missing&2)?0:&text[7];groupA.IVRefCount=2;groupB.IVRefCount=3;groupCalls=deleted=conversions=comparisons=0;
  unsigned long stale=15;result.insert(stale);unsigned beforeCleared=cleared;
  data.GetAllTextEntriesForNarrator(name,result,minimum);++cases;CHECK(conversions==1&&cleared>beforeCleared&&deleted==0&&groupA.IVRefCount==2&&groupB.IVRefCount==3);
  bool expected[16],actual[16];memset(expected,0,sizeof expected);memset(actual,0,sizeof actual);unsigned expectedGroups=0;
  if(!(narrator==-1&&!emptyName))for(unsigned i=1;i<count;++i){
   if(types[i]==0){if(entries[i]){long speaker=text[i].NarratorIndex;if((speaker==narrator||(emptyName&&speaker==-1))&&i>=minimum)expected[i]=true;}}
   else if(types[i]==1){++expectedGroups;const long*begin=i==2?membersA:membersB;unsigned n=i==2?8:5;for(unsigned j=0;j<n;++j){unsigned idx=(unsigned)begin[j];if(types[idx]==0&&entries[idx]){long speaker=text[idx].NarratorIndex;if((speaker==narrator||(emptyName&&speaker==-1))&&idx>=minimum)expected[idx]=true;}}}
  }
  collect(result._M_t._M_header._M_data->_M_parent,actual);unsigned total=0;for(unsigned i=0;i<16;++i){CHECK(expected[i]==actual[i]);total+=expected[i]?1:0;}CHECK(result._M_t._M_node_count==total&&groupCalls==expectedGroups);result.clear();
 }
 CHECK(cases==5760);}
 CHECK(allocations==frees);puts("009c8990_TEST PASS");return 0;
}