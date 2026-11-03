#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __int64 long
#endif
#define FABLE_RETAIL_CTHING_CONSTRUCTORS
#define FABLE_STLPORT_CONSTRUCTORS


#define FABLE_RETAIL_VALIDPOSITIONS
#define FABLE_STLPORT_CREATURE_POSITIONS
#define FABLE_STLPORT_VISIBLE_POSITIONS_OVERFLOW
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#ifdef _MSC_VER
#include <math.h>
#else
extern "C" double cos(double);extern "C" double sin(double);
#endif
#pragma intrinsic(cos,sin)
#ifdef FABLE_RETAIL_CTHING
#ifndef FABLE_ENGINE_CTHING_H
#define FABLE_ENGINE_CTHING_H


#ifndef FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCLASS
#endif
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#endif
#ifndef FABLE_ENGINE_CCLASS_H
#define FABLE_ENGINE_CCLASS_H










#ifdef FABLE_RETAIL_CCLASS
#include <stddef.h>
#pragma once



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
#ifdef _MSC_VER
#define FABLE_STATIC_ASSERT(expression) \
    typedef char FABLE_JOIN(fable_sassert_, __LINE__)[(expression) ? 1 : -1]
#else
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(_test_size_,__LINE__)[1]
#endif

#ifndef FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_BASE_OBJECT
#endif
#ifndef FABLE_ENGINE_CBASEOBJECT_H
#define FABLE_ENGINE_CBASEOBJECT_H
















#ifdef FABLE_RETAIL_BASE_OBJECT

#ifndef FABLE_ENGINE_CBASECLASS_H
#define FABLE_ENGINE_CBASECLASS_H









class CBaseClass {
public:
    CBaseClass();                                                        
    virtual ~CBaseClass();                                               
};
FABLE_STATIC_ASSERT(sizeof(CBaseClass) == 0x4);

#endif 

class CBaseObjectPointer;
class CBaseObject : public CBaseClass {
protected:
    CBaseObject();                                                       
    CBaseObject(const CBaseObject&);                                     
    CBaseObject& operator=(const CBaseObject&);                          
public:
    void ClearIntelligentPointers();                                     
    virtual void Validate() const;                                       
protected:
    virtual ~CBaseObject();                                              
    void InternalValidate() const;                                       
public:
    CBaseObjectPointer* IntelligentPointer;                              
};
FABLE_STATIC_ASSERT(sizeof(CBaseObject) == 0x8);
#else 




#include <stddef.h>


struct CBaseObjectPointer;

#pragma pack(push, 1)
class CBaseObject {
public:
    void*               __vftable;                           
    CBaseObjectPointer* IntelligentPointer;                  
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CBaseObject) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CBaseObject, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CBaseObject, IntelligentPointer) == 0x4);

#endif 
#endif 

class CCharString;
class CClassFactory;
class CClass : public CBaseObject {
public:
    virtual void Save(CCharString&);                                     
    void Unregister(CClassFactory&);                                     
    void PostConstructionInit(CClassFactory&);                           
    unsigned long GetType() const { return Type; }                       
    unsigned char ClassListPos[0x4];                                     
    unsigned char TypeListPos[0x4];                                      
    unsigned char Type;                                                  
};
FABLE_STATIC_ASSERT(sizeof(CClass) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CClass, Type) == 0x10);
#else 





#include <stddef.h>


#pragma pack(push, 1)
class CClass {
public:
    void*         __vftable;                                 
    unsigned char _base_0x04[0x4];                           
    unsigned char ClassListPos[0x4];                         
    unsigned char TypeListPos[0x4];                          
    unsigned char Type;                                      
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CClass) == 0x11);
FABLE_STATIC_ASSERT(offsetof(CClass, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CClass, ClassListPos) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CClass, TypeListPos) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CClass, Type) == 0x10);

#endif 
#endif 

#ifndef FABLE_ENGINE_CCHARSTRING_H
#define FABLE_ENGINE_CCHARSTRING_H


#if defined(FABLE_RETAIL_SCOREBOARD) || defined(FABLE_RETAIL_PERSIST_OPINION) || defined(FABLE_RETAIL_SUBTITLE)
#define FABLE_RETAIL_CCHARSTRING_DATA
#pragma once

#include <stddef.h>



#ifndef FABLE_RETAIL_CCHARSTRING_DATA
class CWideStringData
{ public:
    wchar_t* text;
    fable_u32 unknown04;
    fable_u32 unknown08;
    fable_i32 owners;
};

#endif

#pragma pack(push, 1)
#ifdef FABLE_RETAIL_CCHARSTRING_DATA



template<class T> class CBasicString {
protected:
    T* PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool UseFastExtend : 1;
public:
    const T* PeekData() const { return PData; }
#ifdef FABLE_RETAIL_SUBTITLE
    unsigned long GetLength() const { return StringLength; }
#endif
#ifdef FABLE_RETAIL_PERSIST_OPINION
    bool operator==(const T* text) const;
#endif
};
class CCharStringData {
public:
    CBasicString<char> Data;
protected:
    long NoRefs;
};
#else
class CCharStringData
{ public:
    char* text;
    fable_u32 unknown04;
    fable_u32 unknown08;
    fable_u8 flags0C;
    fable_i32 owners;
};
#endif
#pragma pack(pop)

#ifndef FABLE_RETAIL_CCHARSTRING_DATA

class CCharString;

class CWideString
{
public:
    CWideString();
    CWideString(const CWideString& other);
    explicit CWideString(const wchar_t* text);
    ~CWideString();
    operator const wchar_t*() const;
    const wchar_t* GetAsCharArray() const;                     
    long GetLength() const;
    long FindCharPos(wchar_t c) const;                         
    void __cdecl Sprintf(const wchar_t* format, ...);          
    void SetLength(long length);                               
    const CWideString& operator=(const CWideString& other);
    const CWideString& operator=(const wchar_t* text);
    const CWideString& operator+=(const CWideString& other);
    const CWideString& operator+=(const wchar_t* text);
    bool operator!=(const wchar_t* other) const;

    static CWideString FABLE_FASTCALL CreateFromCharString(
        const CCharString& text);

private:
    explicit CWideString(const CCharString& text);
    void UnassignString();

protected:
    CWideStringData* AllocStringData(                          
        const wchar_t* text,
        long length);

private:

    CWideStringData* storage_;
};

CWideString FABLE_FASTCALL operator+(
    const CWideString& left,
    const CWideString& right);
CWideString FABLE_FASTCALL operator+(
    const CWideString& left,
    const wchar_t* right);
CWideString FABLE_FASTCALL operator+(
    const wchar_t* left,
    const CWideString& right);

class CCharString
{
public:
    CCharString();
    CCharString(const char* text, long length);
    CCharString(const CCharString& other);                     
    ~CCharString();
    operator const char*() const;
    const CCharString& operator=(const CCharString& other);
    const CCharString& operator=(const char* text);
    CWideString ToWideString() const;

protected:
    void UnassignString();                                     
    CCharStringData* AllocStringData(                          
        const char* text,
        long length);

private:
    CCharStringData* storage_;
};

CCharString FABLE_FASTCALL operator+(
    const CCharString& left,
    const char* right);



extern fable_i32 g_CWideStringInstanceCount_013BCA20;
extern fable_i32 g_CCharStringInstanceCount_013BD800;
extern wchar_t g_FableEmptyWideString_0129A8E0[1];

FABLE_STATIC_ASSERT(sizeof(CWideStringData) == 0x10);
FABLE_STATIC_ASSERT(sizeof(CCharStringData) == 0x11);
FABLE_STATIC_ASSERT(sizeof(CWideString) == 0x04);
FABLE_STATIC_ASSERT(sizeof(CCharString) == 0x04);

#endif

#endif
class CCharStringData;
class CCharString {
public:
    CCharString();
    CCharString(const char* text, long length = -1);
    CCharString(const CCharString& other);
#if defined(FABLE_RETAIL_SCOREBOARD) || defined(FABLE_RETAIL_SUBTITLE)
#ifdef FABLE_RETAIL_SUBTITLE
    const char* GetAsCharArray() const { return PStringData==0 ? &NullChar : PStringData->Data.PeekData(); }
#else
    const char* GetAsCharArray() const { const char* result=&NullChar; if(PStringData) result=PStringData->Data.PeekData(); return result; }
#endif
#endif
#ifdef FABLE_RETAIL_EFFECT_LOOKUP
    unsigned long GetCRC() const; 
#endif
#ifdef FABLE_RETAIL_PERSIST_OPINION
    bool EqualsCaseInsensitive(const CCharString&) const;
    bool operator==(const char* text) const;
#endif
#ifdef FABLE_RETAIL_SUBTITLE
    long GetLength() const { return PStringData==0 ? 0 : PStringData->Data.GetLength(); }
    inline bool StartsWith(const CCharString& str) const { return strncmp(GetAsCharArray(),str.GetAsCharArray(),str.GetLength())==0; }
    inline bool EndsWith(const CCharString& str) const { long offset=GetLength()-str.GetLength();if(offset>=0)return strcmp(GetAsCharArray()+offset,str.GetAsCharArray())==0;return false; }
#endif
    ~CCharString();
protected:
#if defined(FABLE_RETAIL_SCOREBOARD) || defined(FABLE_RETAIL_SUBTITLE)
    static const char NullChar;
#endif
    CCharStringData* PStringData;
};
#endif

#ifndef FABLE_ENGINE_CDEFPOINTER_H
#define FABLE_ENGINE_CDEFPOINTER_H























#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#error engine/CDefPointer.h is the retail def model: define FABLE_RETAIL_DEF_CLASS_BASE before including it
#else

#ifndef FABLE_ENGINE_CDEFPOINTEEBASE_H
#define FABLE_ENGINE_CDEFPOINTEEBASE_H




#include <stddef.h>

#ifndef FABLE_ENGINE_CRESOURCE_H
#define FABLE_ENGINE_CRESOURCE_H







#ifdef FABLE_RETAIL_DEF_CLASS_BASE
#include <stddef.h>

#ifndef FABLE_ENGINE_CIVCOUNTEDPOINTEEBASE_H
#define FABLE_ENGINE_CIVCOUNTEDPOINTEEBASE_H















#ifdef FABLE_RETAIL_DEF_CLASS_BASE
#include <stddef.h>
class CIVCountedPointeeBase {
public:
#ifdef FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE
    CIVCountedPointeeBase();
#endif
    virtual ~CIVCountedPointeeBase() { }                                 
    virtual void Delete() const;                                         
    void AddRef() const { ++IVRefCount; }                                
    long Release() const { return --IVRefCount; }                        
    long GetRefCount() const { return IVRefCount; }                      
#ifdef FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE
private:
#endif
    mutable unsigned long IVRefCount;                                    
};
FABLE_STATIC_ASSERT(sizeof(CIVCountedPointeeBase) == 0x8);
#else 
#include <stddef.h>


#pragma pack(push, 1)
class CIVCountedPointeeBase {
public:
    void*         __vftable;                                 
    unsigned long IVRefCount;                                
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CIVCountedPointeeBase) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CIVCountedPointeeBase, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CIVCountedPointeeBase, IVRefCount) == 0x4);

#endif 

#endif 

class CResourceList;
class CResource : public CIVCountedPointeeBase {
public:
    CResource();                                                         
    virtual ~CResource();                                                
    virtual bool UnloadResource();                                       
    virtual unsigned long GetUnloadableMemory() const;                   
    virtual bool IsResourceLoaded() const;                               
    virtual void OnResourceLoaded();                                     
    virtual bool CanAbortResourceLoad(long) const;                       
    virtual void OnResourceLoadAborted();                                
    virtual unsigned long GetDebugMemoryInUse() const;                   
    virtual void ResetDebugMemoryInUse();                                
    CResourceList* ResourceList;                                         
    CResource*     PrevResource;                                         
    CResource*     NextResource;                                         
    unsigned long  ResourceSize;                                         
    unsigned long  LastUsedFrame;                                        
};
FABLE_STATIC_ASSERT(sizeof(CResource) == 0x1c);
#else 
#include <stddef.h>


class CResourceList;

#pragma pack(push, 1)
class CResource {
public:
    void*          __vftable;                                
    unsigned char  _pad_0x04[0x4];                           
    CResourceList* ResourceList;                             
    CResource*     PrevResource;                             
    CResource*     NextResource;                             
    unsigned long  ResourceSize;                             
    unsigned long  LastUsedFrame;                            
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CResource) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CResource, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CResource, ResourceList) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CResource, PrevResource) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CResource, NextResource) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CResource, ResourceSize) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CResource, LastUsedFrame) == 0x18);

#endif 

#endif 


class CDefPointeeBase : public CResource {
public:
    virtual ~CDefPointeeBase() { }                                       
};
FABLE_STATIC_ASSERT(sizeof(CDefPointeeBase) == 0x1c);

#endif 

#if defined(_MSC_VER)
#define FABLE_DEFPTR_INLINE __forceinline
#else
#define FABLE_DEFPTR_INLINE inline
#endif

enum ENullDef { NULL_DEF };                                          

template <class T> class CDefPointer {
public:
    FABLE_DEFPTR_INLINE CDefPointer(ENullDef = NULL_DEF) : Object(0) {}             
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
    void Reset(T*);                                                      
    FABLE_DEFPTR_INLINE void Release()
    {
        if (Object) {
            CDefPointeeBase* object = Object;
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
    CDefPointeeBase* Object;                                             
private:
    FABLE_DEFPTR_INLINE void Assign(CDefPointeeBase* object)                                 
    {
        if (Object != object) {
            Release();
            Object = object;
            if (Object) Object->AddRef();
        }
    }
};
FABLE_STATIC_ASSERT(sizeof(CDefPointer<CDefPointeeBase>) == 0x4);

#endif 
#endif 

#ifndef FABLE_ENGINE_CVECTORMAP_H
#define FABLE_ENGINE_CVECTORMAP_H
#ifdef FABLE_VECTOR_MAP_GEOMETRY


#ifndef FABLE_STLPORT46_GEOMETRY_H
#define FABLE_STLPORT46_GEOMETRY_H

#include <stddef.h>
#include <stdlib.h>
#ifdef _MSC_VER
inline void* __cdecl operator new(size_t,void* p){return p;}
#else
#include <new>
#endif
namespace std {
struct __false_type {};
template<class T>class allocator {
public:
 allocator(){}
 T* allocate(size_t n,const void* =0){return n?(T*)malloc(n*sizeof(T)):0;}
 void deallocate(T* p,size_t){if(p)free(p);}
};
template<class V,class T,class A>class _STLP_alloc_proxy:public A {
public:V _M_data;_STLP_alloc_proxy(const A& a,V p):A(a),_M_data(p){}
};
template<class A,class B>struct pair {
 typedef A first_type;typedef B second_type;
 A first;B second;
 pair():first(A()),second(B()){}
 pair(const A&a,const B&b):first(a),second(b){}
 template<class C,class D>pair(const pair<C,D>&p):first(p.first),second(p.second){}
};
template<class A,class B,class R>struct binary_function{typedef A first_argument_type;typedef B second_argument_type;typedef R result_type;};
template<class A,class R>struct unary_function{typedef A argument_type;typedef R result_type;};
template<class T>struct less:binary_function<T,T,bool>{bool operator()(const T&a,const T&b)const{return a<b;}};
template<class T>struct _Identity:unary_function<T,T>{const T&operator()(const T&v)const{return v;}};
template<class P>struct _Select1st:unary_function<P,typename P::first_type>{const typename P::first_type&operator()(const P&v)const{return v.first;}};
template<class T>inline const T& (max)(const T&a,const T&b){return a<b?b:a;}
template<class T>inline const T& (min)(const T&a,const T&b){return b<a?b:a;}
template<class T>inline void swap(T&a,T&b){T temporary=a;a=b;b=temporary;}
template<class T,class U>inline void _Construct(T*p,const U&v){new(p)T(v);}
template<class T>inline void _Destroy(T*p){p->~T();}
template<class I>inline void _Destroy(I first,I last){for(;first!=last;++first)_Destroy(&*first);}
template<class T,class A>class _Vector_base {
public:
 _Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}
 ~_Vector_base(){_M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start);}
protected:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;
};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{
public:
 typedef T value_type;typedef T*iterator;typedef const T*const_iterator;typedef unsigned int size_type;typedef A allocator_type;
 explicit vector(const A&a=A()):_Vector_base<T,A>(a){}
 ~vector(){_Destroy(this->_M_start,this->_M_finish);}
 iterator begin(){return this->_M_start;}const_iterator begin()const{return this->_M_start;}
 iterator end(){return this->_M_finish;}const_iterator end()const{return this->_M_finish;}
 size_type size()const{return size_type(this->_M_finish-this->_M_start);}
 size_type capacity()const{return size_type(this->_M_end_of_storage._M_data-this->_M_start);}
 T&operator[](size_type i){return this->_M_start[i];}const T&operator[](size_type i)const{return this->_M_start[i];}
 void reserve(size_type);
 void push_back(const T&v){if(this->_M_finish!=this->_M_end_of_storage._M_data){_Construct(this->_M_finish,v);++this->_M_finish;}else _M_insert_overflow(this->_M_finish,v,__false_type(),1,true);}
protected:
 void _M_insert_overflow(T*,const T&,const __false_type&,size_type,bool=false);
#ifdef FABLE_GEOMETRY_VECTOR_BODIES
 void _M_clear(){_Destroy(this->_M_start,this->_M_finish);this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);}
 void _M_set(T* s,T* f,T* e){this->_M_start=s;this->_M_finish=f;this->_M_end_of_storage._M_data=e;}
 template<class I>T* _M_allocate_and_copy(size_type n,I first,I last){T* result=this->_M_end_of_storage.allocate(n);T* out=result;for(;first!=last;++first,++out)_Construct(out,*first);return result;}
#endif
};
#ifdef FABLE_GEOMETRY_VECTOR_BODIES
template<class T,class A>
__declspec(noinline) void vector<T,A>::reserve(size_type n){
 if(capacity()<n){const size_type old_size=size();T* temp;if(this->_M_start){temp=_M_allocate_and_copy(n,this->_M_start,this->_M_finish);_M_clear();}else temp=this->_M_end_of_storage.allocate(n);_M_set(temp,temp+old_size,temp+n);}
}
template<class T,class A>
__declspec(noinline) void vector<T,A>::_M_insert_overflow(T* position,const T& value,const __false_type&,size_type fill_len,bool at_end){
 const size_type old_size=size();const size_type length=old_size+(max)(old_size,fill_len);
 T* new_start=this->_M_end_of_storage.allocate(length);T* new_finish=new_start;
 for(T* first=this->_M_start;first!=position;++first,++new_finish)_Construct(new_finish,*first);
 if(fill_len==1){_Construct(new_finish,value);++new_finish;}else for(size_type count=0;count<fill_len;++count,++new_finish)_Construct(new_finish,value);
 if(!at_end)for(T* first=position;first!=this->_M_finish;++first,++new_finish)_Construct(new_finish,*first);
 _M_clear();_M_set(new_start,new_finish,new_start+length);
}
#endif
struct _Rb_tree_node_base {
 typedef bool _Color_type;typedef _Rb_tree_node_base*_Base_ptr;
 _Color_type _M_color;_Base_ptr _M_parent,_M_left,_M_right;
};
template<class T>struct _Rb_tree_node:public _Rb_tree_node_base{T _M_value_field;};
template<class Dummy>class _Rb_global{
public:typedef _Rb_tree_node_base*_Base_ptr;
 static _Base_ptr __cdecl _M_increment(_Base_ptr);
};
template<class Dummy>
_Rb_tree_node_base* __cdecl _Rb_global<Dummy>::_M_increment(_Base_ptr node){
 if(node->_M_right){node=node->_M_right;while(node->_M_left)node=node->_M_left;}
 else{_Base_ptr y=node->_M_parent;while(node==y->_M_right){node=y;y=y->_M_parent;}if(node->_M_right!=y)node=y;}
 return node;
}
typedef _Rb_global<bool> _Rb_global_inst;
template<class T>struct _Nonconst_traits{typedef T value_type;typedef T&reference;typedef T*pointer;};
template<class T>struct _Const_traits{typedef T value_type;typedef const T&reference;typedef const T*pointer;};
struct _Rb_tree_base_iterator{typedef _Rb_tree_node_base*_Base_ptr;_Base_ptr _M_node;
 bool operator==(const _Rb_tree_base_iterator& y)const{return _M_node==y._M_node;}
 bool operator!=(const _Rb_tree_base_iterator& y)const{return _M_node!=y._M_node;}
};
template<class T,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator{
 typedef _Rb_tree_iterator<T,Traits>_Self;typedef _Rb_tree_iterator<T,_Nonconst_traits<T> >iterator;
 typedef typename Traits::reference reference;typedef typename Traits::pointer pointer;
 _Rb_tree_iterator(){_M_node=0;} _Rb_tree_iterator(_Rb_tree_node<T>*n){_M_node=n;}
 _Rb_tree_iterator(const iterator&i){_M_node=i._M_node;}
 reference operator*()const{return ((_Rb_tree_node<T>*)_M_node)->_M_value_field;}
 pointer operator->()const{return &(operator*());}
 _Self&operator++(){_M_node=_Rb_global_inst::_M_increment(_M_node);return *this;}
 _Self operator++(int){_Self old=*this;_M_node=_Rb_global_inst::_M_increment(_M_node);return old;}
};
template<class T,class A>struct _Rb_tree_base{
 typedef _Rb_tree_node<T>_Node;
 _Rb_tree_base(const A&a):_M_header(allocator<_Node>(),(_Node*)0){_M_header._M_data=_M_header.allocate(1);}
 ~_Rb_tree_base(){_M_header.deallocate(_M_header._M_data,1);}
protected:_STLP_alloc_proxy<_Node*,_Node,allocator<_Node> >_M_header;
};
template<class K,class V,class KeyOfValue,class Compare,class A>class _Rb_tree:public _Rb_tree_base<V,A>{
public:
 typedef K key_type;typedef V value_type;typedef _Rb_tree_node<V>*_Link_type;typedef _Rb_tree_node_base*_Base_ptr;
 typedef _Rb_tree_iterator<V,_Nonconst_traits<V> >iterator;typedef _Rb_tree_iterator<V,_Const_traits<V> >const_iterator;
 _Rb_tree():_Rb_tree_base<V,A>(A()),_M_node_count(0),_M_key_compare(Compare()){
  this->_M_header._M_data->_M_color=false;_M_root()=0;_M_leftmost()=this->_M_header._M_data;_M_rightmost()=this->_M_header._M_data;
 }
 _Rb_tree(const _Rb_tree&);
 ~_Rb_tree(){clear();}
 iterator begin(){return iterator(_M_leftmost());}const_iterator begin()const{return const_iterator(_M_leftmost());}
 iterator end(){return iterator(this->_M_header._M_data);}const_iterator end()const{return const_iterator(this->_M_header._M_data);}
 bool empty()const{return _M_node_count==0;}unsigned int size()const{return _M_node_count;}
 void clear(){if(_M_node_count){_M_erase(_M_root());_M_leftmost()=this->_M_header._M_data;_M_root()=0;_M_rightmost()=this->_M_header._M_data;_M_node_count=0;}}
 pair<iterator,bool>insert_unique(const V&);
 iterator find(const K&);
 void erase(iterator);
protected:
 unsigned int _M_node_count;Compare _M_key_compare;
 _Link_type&_M_root()const{return (_Link_type&)this->_M_header._M_data->_M_parent;}
 _Link_type&_M_leftmost()const{return (_Link_type&)this->_M_header._M_data->_M_left;}
 _Link_type&_M_rightmost()const{return (_Link_type&)this->_M_header._M_data->_M_right;}
protected:
 static _Link_type& __cdecl _S_left(_Base_ptr x){return (_Link_type&)x->_M_left;}
 static _Link_type& __cdecl _S_right(_Base_ptr x){return (_Link_type&)x->_M_right;}
private:void _M_erase(_Link_type);
};
template<class K,class V,class KeyOfValue,class Compare,class A>
void _Rb_tree<K,V,KeyOfValue,Compare,A>::_M_erase(_Link_type x){
 while(x){_M_erase(_S_right(x));_Link_type y=_S_left(x);_Destroy(&x->_M_value_field);this->_M_header.deallocate(x,1);x=y;}
}
template<class K,class Compare=less<K>,class A=allocator<K> >class set{
 typedef _Rb_tree<K,K,_Identity<K>,Compare,A>_Rep_type;_Rep_type _M_t;
public:
 typedef typename _Rep_type::const_iterator iterator;typedef iterator const_iterator;
 iterator begin()const{return _M_t.begin();}iterator end()const{return _M_t.end();}
 unsigned int size()const{return _M_t.size();}bool empty()const{return _M_t.empty();}void clear(){_M_t.clear();}
 pair<iterator,bool>insert(const K&v){typedef typename _Rep_type::iterator _Rep_iterator;pair<_Rep_iterator,bool>p=_M_t.insert_unique(v);return pair<iterator,bool>(p.first,p.second);}
};
template<class K,class T,class Compare=less<K>,class A=allocator<pair<const K,T> > >class map{
 typedef _Rb_tree<K,pair<const K,T>,_Select1st<pair<const K,T> >,Compare,A>_Rep_type;_Rep_type _M_t;
public:
 typedef pair<const K,T>value_type;typedef typename _Rep_type::iterator iterator;typedef typename _Rep_type::const_iterator const_iterator;
 iterator begin(){return _M_t.begin();}iterator end(){return _M_t.end();}
 iterator find(const K&k){return _M_t.find(k);}void erase(iterator i){_M_t.erase(i);}T&operator[](const K&);
};
template<class K,class T,class Compare=less<K>,class A=allocator<pair<const K,T> > >class multimap{
 typedef _Rb_tree<K,pair<const K,T>,_Select1st<pair<const K,T> >,Compare,A>_Rep_type;_Rep_type _M_t;
public:typedef typename _Rep_type::iterator iterator;void erase(iterator i){_M_t.erase(i);}
};
}
#endif

#else
#ifndef FABLE_STLPORT_CONTAINERS_H
#define FABLE_STLPORT_CONTAINERS_H



#include <stddef.h>
#if defined(FABLE_STLPORT_UI_ACTIONS) || defined(FABLE_STLPORT_CREATURE_POSITIONS) || defined(FABLE_STLPORT_CAMERA_POINTS)
#include <new>
#endif
#include <stdlib.h>
#if defined(FABLE_STLPORT_VISIBLE_TRUE_OVERFLOW) || defined(FABLE_RETAIL_CTHINGMANAGER_BINARY)
#include <string.h>
#endif
#if defined(FABLE_STLPORT_VISIBLE_PUSH_BACK) || defined(FABLE_STLPORT_HTTP)
#include <new>
#include <string.h>
#endif
namespace std {
#ifdef FABLE_RETAIL_CTHINGMANAGER_BINARY
inline void fill(unsigned char* first,unsigned char* last,const unsigned char& value) {
    unsigned char tmp=value;memset(first,tmp,last-first);
}
template<class Size> inline unsigned char* fill_n(unsigned char* first,Size n,const unsigned char& value) {
    fill(first,first+n,value);return first+n;
}
#endif
struct __true_type { };
#ifdef FABLE_STLPORT_CAMERA_POINTS
struct __false_type { };
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_BINARY
struct __false_type { };
template<class T> struct __type_traits { typedef __false_type is_POD_type; };
template<> struct __type_traits<unsigned char> { typedef __true_type is_POD_type; };
template<class Iterator> struct iterator_traits;
template<class T> struct iterator_traits<T*> { typedef T value_type; };
template<class ForwardIterator,class Size,class T>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator first,Size n,const T& value,const __true_type&) {
    return std::fill_n(first,n,value);
}
template<class ForwardIterator,class Size,class T>
inline ForwardIterator uninitialized_fill_n(ForwardIterator first,Size n,const T& value) {
    typedef typename iterator_traits<ForwardIterator>::value_type Value;
    typedef typename __type_traits<Value>::is_POD_type IsPOD;
    return __uninitialized_fill_n(first,n,value,IsPOD());
}
#endif
#ifdef FABLE_STLPORT_HTTP
struct __false_type { __false_type() { } };
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
template<class T> inline const T& (max)(const T& a,const T& b) { return a<b?b:a; }
template<class T> inline const T& (min)(const T& a,const T& b) { return b<a?b:a; }
template<class T,class U> inline void _Construct(T* p,const U& v) { new(p) T(v); }
template<class T> inline void _Destroy(T* p) { p->~T(); }
inline void _Destroy(char*,char*) { }
#ifdef FABLE_RETAIL_QUEST_COMPLETION
template<class T> struct __type_traits;
template<class I> struct iterator_traits;
template<class T> struct iterator_traits<T*> {
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;
};
template<class I> inline void __destroy_aux(I first,I last,const __false_type&) {
    for(;first!=last;++first) _Destroy(&*first);
}
template<class I> inline void __destroy_aux(I,I,const __true_type&) { }
template<class I,class T> inline void __destroy(I first,I last,T*) {
    typedef typename __type_traits<T>::has_trivial_destructor Trivial;
    __destroy_aux(first,last,Trivial());
}
template<class I> inline void _Destroy(I first,I last) {
    __destroy(first,last,(typename iterator_traits<I>::value_type*)0);
}
#else
template<class I> inline void _Destroy(I first,I last) { for(;first!=last;++first) _Destroy(&*first); }
#endif
template<class I,class O> inline O __uninitialized_copy(I first,I last,O result,const __false_type&) {
    O cur=result;for(;first!=last;++first,++cur) _Construct(&*cur,*first);return cur;
}
template<class I,class N,class T> inline I __uninitialized_fill_n(I first,N n,const T& x,const __false_type&) {
    I cur=first;for(;n>0;--n,++cur) _Construct(&*cur,x);return cur;
}
#endif
#ifdef FABLE_STLPORT_HTTP
template<class T> struct __type_traits { typedef __false_type has_trivial_assignment_operator; typedef __false_type has_trivial_destructor; };
template<> struct __type_traits<char> { typedef __true_type has_trivial_assignment_operator; };
#ifdef FABLE_RETAIL_QUEST_COMPLETION
template<> struct __type_traits<long> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
template<> struct __type_traits<unsigned long> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
#endif
inline void* __copy_trivial(const void* first,const void* last,void* result) {
    return last==first?result:(char*)memmove(result,first,(const char*)last-(const char*)first)+((const char*)last-(const char*)first);
}
template<class T> inline T* __uninitialized_copy(const T* first,const T* last,T* result,const __true_type&) {
    return (T*)__copy_trivial(first,last,result);
}
template<class I,class T> inline void fill(I first,I last,const T& x) { for(;first!=last;++first) *first=x; }
template<class I,class N,class T> inline I fill_n(I first,N n,const T& x) { for(;n>0;--n,++first) *first=x;return first; }
template<class T> inline T* __copy_backward_ptrs(const T* first,const T* last,T* result,const __true_type&) {
    ptrdiff_t n=last-first;return (T*)memmove(result-n,first,n*sizeof(T));
}
template<class T> inline T* __copy_backward_ptrs(const T* first,const T* last,T* result,const __false_type&) {
    while(first!=last) *--result=*--last;return result;
}
#endif
#ifdef FABLE_STLPORT_CREATURE_POSITIONS
#ifndef FABLE_STLPORT_HTTP
template<class T> inline const T& (max)(const T& a,const T& b) { return a<b?b:a; }
template<class T> inline const T& (min)(const T& a,const T& b) { return b<a?b:a; }
struct __false_type { __false_type() { } };
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };
#endif
template<class Input,class Output,class Distance>
Output __fastcall __copy(Input,Input,Output,const random_access_iterator_tag&,Distance*);
template<class Input,class Output> inline Output __copy_ptrs(Input first,Input last,Output result,const __false_type&) {
    return __copy(first,last,result,random_access_iterator_tag(),(ptrdiff_t*)0);
}
#endif
template<class _Tp> class allocator {
public:
#ifdef FABLE_STLPORT_HTTP
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(size_t n,const void* =0) { return n?(_Tp*)malloc(n*sizeof(_Tp)):0; }
    void deallocate(_Tp* p,size_t) { if(p) free(p); }
#else
#ifdef FABLE_STLPORT_CONSTRUCTORS
    _Tp* allocate(size_t n,const void* =0) { return n?(_Tp*)malloc(n*sizeof(_Tp)):0; }
#endif
    void deallocate(_Tp* p, size_t) { free(p); }
#endif
};
template<class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& a, _Value p)
        : _MaybeReboundAlloc(a), _M_data(p) { }
};
template<class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a,0) { }
#if defined(FABLE_STLPORT_UI) || defined(FABLE_RETAIL_CTHINGMANAGER_BINARY)
    _Vector_base(size_t n,const _Alloc& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a,0) {
        _M_start=n?(_Tp*)malloc(n*sizeof(_Tp)):0;
        _M_finish=_M_start;
        _M_end_of_storage._M_data=_M_start+n;
    }
#endif
    ~_Vector_base() {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data-_M_start);
    }
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*,_Tp,_Alloc> _M_end_of_storage;
};
template<class _Tp, class _Alloc=allocator<_Tp> >
class vector : public _Vector_base<_Tp,_Alloc> {
public:
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    typedef _Tp value_type;
    typedef size_t size_type;
    explicit vector(const _Alloc& a=_Alloc()) : _Vector_base<_Tp,_Alloc>(a) { }
#ifdef FABLE_RETAIL_CTHINGMANAGER_BINARY
    explicit vector(size_type n) : _Vector_base<_Tp,_Alloc>(n,_Alloc()) {
        this->_M_finish=uninitialized_fill_n(this->_M_start,n,_Tp());
    }
    _Tp& front() { return *this->_M_start; }
#endif
#ifdef FABLE_STLPORT_UI
    vector(const vector&);
    _Alloc get_allocator() const { return _Alloc(this->_M_end_of_storage); }
#endif
#ifdef FABLE_STLPORT_HTTP
    ~vector() { _Destroy(this->_M_start,this->_M_finish); }
#endif
    iterator begin() { return this->_M_start; }
    const_iterator begin() const { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const_iterator end() const { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish-this->_M_start); }
    bool empty() const { return this->_M_start==this->_M_finish; }
    _Tp& operator[](size_type n) { return *(begin()+n); }
    const _Tp& operator[](size_type n) const { return *(begin()+n); }
#ifdef FABLE_STLPORT_HTTP
    typedef typename __type_traits<_Tp>::has_trivial_assignment_operator _IsPODType;
#ifdef FABLE_RETAIL_QUEST_COMPLETION
protected:
    void _M_insert_overflow(iterator position,const _Tp& x,const __false_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);_Tp* newFinish=newStart;
        newFinish=__uninitialized_copy(this->_M_start,position,newStart,__false_type());
        if(fillLength==1) {_Construct(newFinish,x);++newFinish;}
        else newFinish=__uninitialized_fill_n(newFinish,fillLength,x,__false_type());
        if(!atEnd) newFinish=__uninitialized_copy(position,this->_M_finish,newFinish,__false_type());
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
#ifndef FABLE_RETAIL_QUEST_COMPLETION
    __declspec(noinline)
#endif
    void _M_insert_overflow(iterator,const _Tp&,const __true_type&,size_type,bool);
public:
#endif
#ifdef FABLE_RETAIL_QUEST_COMPLETION
    __forceinline
#endif
    void push_back(const _Tp& x) {
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish,x); ++this->_M_finish;
        } else _M_insert_overflow(this->_M_finish,x,_IsPODType(),1,true);
    }
#ifdef FABLE_RETAIL_QUEST_COMPLETION
    iterator insert(iterator position,const _Tp& x) {
        size_type n=position-begin();
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            if(position==end()) {
                _Construct(this->_M_finish,x);
                ++this->_M_finish;
            } else {
                _Construct(this->_M_finish,*(this->_M_finish-1));
                ++this->_M_finish;
                _Tp x_copy=x;
                __copy_backward_ptrs(position,this->_M_finish-2,this->_M_finish-1,_IsPODType());
                *position=x_copy;
            }
        } else _M_insert_overflow(position,x,_IsPODType(),1UL,false);
        return begin()+n;
    }
#endif
    iterator erase(iterator first,iterator last) {
        iterator result=first;
        for(iterator source=last;source!=this->_M_finish;++source,++result) *result=*source;
        _Destroy(result,this->_M_finish);this->_M_finish=result;return first;
    }
    void clear() { erase(begin(),end()); }
#ifdef FABLE_RETAIL_SCOREBOARD
    __declspec(noinline) void resize(size_type n,const _Tp& x);
#else
    void resize(size_type n,_Tp x) {
        if(n<size()) erase(begin()+n,end()); else _M_fill_insert(end(),n-size(),x);
    }
#endif
    void resize(size_type n) { resize(n,_Tp()); }
#ifndef FABLE_RETAIL_QUEST_COMPLETION
protected:
    void _M_insert_overflow(iterator position,const _Tp& x,const __false_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);_Tp* newFinish=newStart;
        newFinish=__uninitialized_copy(this->_M_start,position,newStart,__false_type());
        if(fillLength==1) {_Construct(newFinish,x);++newFinish;}
        else newFinish=__uninitialized_fill_n(newFinish,fillLength,x,__false_type());
        if(!atEnd) newFinish=__uninitialized_copy(position,this->_M_finish,newFinish,__false_type());
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
#ifndef FABLE_RETAIL_QUEST_COMPLETION
    __declspec(noinline)
#endif
    void _M_insert_overflow(iterator position,const _Tp& x,const __true_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);
        _Tp* newFinish=(_Tp*)__copy_trivial(this->_M_start,position,newStart);
        newFinish=fill_n(newFinish,fillLength,x);
        if(!atEnd) newFinish=(_Tp*)__copy_trivial(position,this->_M_finish,newFinish);
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
#endif
protected:
    void _M_clear() {
        _Destroy(this->_M_start,this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);
    }
    void _M_set(iterator s,iterator f,iterator e) {
        this->_M_start=s;this->_M_finish=f;this->_M_end_of_storage._M_data=e;
    }
public:
    __declspec(noinline) void _M_fill_insert(iterator position,size_type n,const _Tp& x) {
        if(n!=0) {
            if(size_type(this->_M_end_of_storage._M_data-this->_M_finish)>=n) {
                _Tp copy=x;const size_type after=this->_M_finish-position;iterator oldFinish=this->_M_finish;
                if(after>n) {
                    __uninitialized_copy(this->_M_finish-n,this->_M_finish,this->_M_finish,_IsPODType());
                    this->_M_finish+=n;
                    __copy_backward_ptrs(position,oldFinish-n,oldFinish,_IsPODType());
                    std::fill(position,position+n,copy);
                } else {
                    __uninitialized_fill_n(this->_M_finish,n-after,copy,__false_type());
                    this->_M_finish+=n-after;
                    __uninitialized_copy(position,oldFinish,this->_M_finish,_IsPODType());
                    this->_M_finish+=after;std::fill(position,oldFinish,copy);
                }
            } else _M_insert_overflow(position,x,_IsPODType(),n,false);
        }
    }
#elif defined(FABLE_STLPORT_CREATURE_POSITIONS)
    
    void push_back(const _Tp& x) {
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            new((void*)this->_M_finish) _Tp(x); ++this->_M_finish;
        } else _M_insert_overflow(this->_M_finish,x,__false_type(),1,true);
    }
    iterator erase(iterator first,iterator last) {
        iterator i=std::__copy_ptrs(last,this->_M_finish,first,__false_type());
        for(iterator d=i;d!=this->_M_finish;++d) d->~_Tp();
        this->_M_finish=i;
        return first;
    }
    void clear() { erase(begin(),end()); }
protected:
#ifdef FABLE_STLPORT_VISIBLE_POSITIONS_OVERFLOW
    
    void _M_insert_overflow(iterator position,const _Tp& value,const __false_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();
        const size_type length=oldSize+(oldSize<fillLength?fillLength:oldSize);
        _Tp* newStart=(_Tp*)malloc(length*sizeof(_Tp));
        _Tp* newFinish=newStart;
        for(iterator first=this->_M_start;first!=position;++first,++newFinish) new((void*)newFinish) _Tp(*first);
        for(size_type count=fillLength;count>0;--count,++newFinish) new((void*)newFinish) _Tp(value);
        if(!atEnd) for(iterator first=position;first!=this->_M_finish;++first,++newFinish) new((void*)newFinish) _Tp(*first);
        for(iterator first=this->_M_start;first!=this->_M_finish;++first) first->~_Tp();
        if(this->_M_start) free(this->_M_start);
        this->_M_start=newStart;this->_M_finish=newFinish;this->_M_end_of_storage._M_data=newStart+length;
    }
#else
    void _M_insert_overflow(iterator,const _Tp&,const __false_type&,size_type,bool);
#endif
#elif defined(FABLE_STLPORT_VISIBLE_PUSH_BACK)
    void push_back(const _Tp& x) {
        if (this->_M_finish!=this->_M_end_of_storage._M_data) {
            new ((void*)this->_M_finish) _Tp(x);
            ++this->_M_finish;
        } else {
            _M_insert_overflow(end(),x,__true_type(),size_type(1),true);
        }
    }
protected:
#ifdef FABLE_STLPORT_VISIBLE_TRUE_OVERFLOW
    
    void _M_insert_overflow(iterator position,const _Tp& x,
                                                const __true_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();
        const size_type len=oldSize+(oldSize<fillLength ? fillLength : oldSize);
        _Tp* newStart=(_Tp*)malloc(len*sizeof(_Tp));
        size_type before=position-this->_M_start;
        memmove(newStart,this->_M_start,before*sizeof(_Tp));
        _Tp* newFinish=newStart+before;
        for(size_type n=fillLength;n>0;--n,++newFinish) *newFinish=x;
        if(!atEnd) {
            size_type after=this->_M_finish-position;
            memmove(newFinish,position,after*sizeof(_Tp));
            newFinish+=after;
        }
        if(this->_M_start) free(this->_M_start);
        this->_M_start=newStart;
        this->_M_finish=newFinish;
        this->_M_end_of_storage._M_data=newStart+len;
    }
#else
    void _M_insert_overflow(iterator,const _Tp&,const __true_type&,size_type,bool);
#endif
#else
    void push_back(const _Tp&);
#endif
};
#ifdef FABLE_RETAIL_QUEST_COMPLETION
template<class _Tp,class _Alloc> inline void vector<_Tp,_Alloc>::_M_insert_overflow(iterator position,const _Tp& x,const __true_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);
        _Tp* newFinish=(_Tp*)__copy_trivial(this->_M_start,position,newStart);
        newFinish=fill_n(newFinish,fillLength,x);
        if(!atEnd) newFinish=(_Tp*)__copy_trivial(position,this->_M_finish,newFinish);
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
#endif
template<class _T1,class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
#ifdef FABLE_STLPORT_CAMERA_POINTS
    pair(const _T1& a,const _T2& b):first(a),second(b) { }
    pair(const pair& p):first(p.first),second(p.second) { }
#endif
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template<class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template<class _Tp> struct _Nonconst_traits { typedef _Tp* pointer; typedef _Tp& reference; };
template<class _Tp> struct _Const_traits { typedef const _Tp* pointer; typedef const _Tp& reference; };
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* p) : _M_node(p) { }
    _List_iterator_base() { }
    void _M_incr() { _M_node=_M_node->_M_next; }
    bool operator==(const _List_iterator_base& y) const { return _M_node==y._M_node; }
    bool operator!=(const _List_iterator_base& y) const { return _M_node!=y._M_node; }
};
template<class _Tp,class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef typename _Traits::reference reference;
    typedef typename _Traits::pointer pointer;
    typedef _List_iterator<_Tp,_Nonconst_traits<_Tp> > iterator;
#if defined(FABLE_STLPORT_UI_CHANGE_STATE) || defined(FABLE_STLPORT_CAMERA_POINTS)
    _List_iterator(_List_node<_Tp>* x) : _List_iterator_base(x) { }
#else
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) { }
#endif
    _List_iterator() { }
#if defined(FABLE_STLPORT_UI_CHANGE_STATE) || defined(FABLE_STLPORT_CAMERA_POINTS)
    _List_iterator(const iterator& x) : _List_iterator_base(x._M_node) { }
#endif
    reference operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    pointer operator->() const { return &operator*(); }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
#ifdef FABLE_STLPORT_CAMERA_POINTS
    _List_iterator operator++(int) { _List_iterator tmp=*this; this->_M_incr(); return tmp; }
#endif
};
template<class _Tp,class _Alloc> class _List_base {
public:
#ifdef FABLE_STLPORT_CONSTRUCTORS
    _List_base(const _Alloc&) : _M_node(allocator<_List_node<_Tp> >(),0) {
        _List_node<_Tp>* n=_M_node.allocate(1);n->_M_next=n;n->_M_prev=n;_M_node._M_data=n;
    }
    ~_List_base() { clear();_M_node.deallocate(_M_node._M_data,1); }
#endif
    _STLP_alloc_proxy<_List_node<_Tp>*,_List_node<_Tp>,allocator<_List_node<_Tp> > > _M_node;
#ifdef FABLE_STLPORT_CAMERA_POINTS
    void clear() {
        _List_node<_Tp>* cur=(_List_node<_Tp>*)_M_node._M_data->_M_next;
        while(cur!=_M_node._M_data) {
            _List_node<_Tp>* tmp=cur;
            cur=(_List_node<_Tp>*)cur->_M_next;
            tmp->_M_data.~_Tp();
            _M_node.deallocate(tmp,1);
        }
        _M_node._M_data->_M_next=_M_node._M_data;
        _M_node._M_data->_M_prev=_M_node._M_data;
    }
#else
    void clear();
#endif
};
template<class _Tp,class _Alloc=allocator<_Tp> > class list : public _List_base<_Tp,_Alloc> {
public:
#ifdef FABLE_STLPORT_CONSTRUCTORS
    explicit list(const _Alloc& a=_Alloc()) : _List_base<_Tp,_Alloc>(a) { }
#endif
    typedef _List_iterator<_Tp,_Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp,_Const_traits<_Tp> > const_iterator;
#if defined(FABLE_STLPORT_UI_CHANGE_STATE) || defined(FABLE_STLPORT_CAMERA_POINTS)
    iterator begin() { return iterator((_List_node<_Tp>*)this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator((_List_node<_Tp>*)this->_M_node._M_data->_M_next); }
#else
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
#endif
    iterator end() { return iterator(this->_M_node._M_data); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    bool empty() const { return this->_M_node._M_data->_M_next==this->_M_node._M_data; }
#ifdef FABLE_STLPORT_CAMERA_POINTS
    _Alloc get_allocator() const { return _Alloc(); }
    list(const list& x):_List_base<_Tp,_Alloc>(x.get_allocator()) {
        insert(begin(),x.begin(),x.end());
    }
    ~list() { }
    template<class Input> void insert(iterator p,Input first,Input last) {
        _M_insert_dispatch(p,first,last,__false_type());
    }
    template<class Input> __declspec(noinline) void _M_insert_dispatch(iterator position,Input first,Input last,const __false_type&) {
        for(;first!=last;++first) insert(position,*first);
    }
#endif
#if defined(FABLE_STLPORT_UI_ACTIONS) || defined(FABLE_STLPORT_CAMERA_POINTS)
protected:
    _List_node<_Tp>* _M_create_node(const _Tp& x) {
        _List_node<_Tp>* p=this->_M_node.allocate(1);
        new((void*)&p->_M_data) _Tp(x);
        return p;
    }
public:
    iterator insert(iterator position,const _Tp& x) {
        _List_node<_Tp>* tmp=_M_create_node(x);
        _List_node_base* n=position._M_node;
        _List_node_base* p=n->_M_prev;
        tmp->_M_next=n;tmp->_M_prev=p;p->_M_next=tmp;n->_M_prev=tmp;
        return iterator(tmp);
    }
    void push_back(const _Tp& x) { insert(end(),x); }
#endif
};
template<size_t _Nw> struct _Base_bitset {
    typedef unsigned long _WordT;
    _WordT _M_w[_Nw];
    static size_t _S_whichword(size_t n) { return n/(8*sizeof(_WordT)); }
    static size_t _S_whichbit(size_t n) { return n%(8*sizeof(_WordT)); }
    static _WordT _S_maskbit(size_t n) { return _WordT(1)<<_S_whichbit(n); }
    _WordT _M_getword(size_t n) const { return _M_w[_S_whichword(n)]; }
};
void __stl_throw_out_of_range(const char*);
template<size_t _Nb> class bitset : public _Base_bitset<(_Nb+8*sizeof(unsigned long)-1)/(8*sizeof(unsigned long))> {
public:
    bool _Unchecked_test(size_t n) const { return (this->_M_getword(n)&this->_S_maskbit(n))!=0; }
    bool test(size_t n) const { if(n>=_Nb) __stl_throw_out_of_range("bitset"); return _Unchecked_test(n); }
};
template<class _Arg,class _Result> struct unary_function {
    typedef _Arg argument_type;
    typedef _Result result_type;
};
template<class _RandomAccessIter,class _RandomNumberGenerator>
void __fastcall random_shuffle(_RandomAccessIter,_RandomAccessIter,_RandomNumberGenerator&);
}
#endif

#endif


template<class T> class CArray : public std::vector<T,std::allocator<T> > { };
template<class K,class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K,V>& a,const std::pair<K,V>& b) const { return a.first<b.first; }
};
template<class K,class V,class CompareType=CKeyPairCompareLess<K,V> >
class CVectorMap : public CArray<std::pair<K,V> > {
public:
    typedef std::pair<K,V> value_type;
    typedef value_type* iterator;
    typedef const value_type* const_iterator;
#ifdef FABLE_RETAIL_VISIBLE_LOWERBOUND
    __declspec(noinline) iterator LowerBound(const K& key) {
        iterator first=this->begin();
        long count=this->end()-first;
        while(count>0) {
            long half=count/2;
            iterator middle=first+half;
            if(middle->first<key) { first=middle+1; count-=half+1; }
            else count=half;
        }
        return first;
    }
    __declspec(noinline) const_iterator LowerBound(const K& key) const {
        const_iterator first=this->begin();
        long count=this->end()-first;
        while(count>0) {
            long half=count/2;
            const_iterator middle=first+half;
            if(middle->first<key) { first=middle+1; count-=half+1; }
            else count=half;
        }
        return first;
    }
#else
    iterator LowerBound(const K&);
    const_iterator LowerBound(const K&) const;
#endif
    iterator Find(const K& key) {
        iterator i=LowerBound(key);
        return i==this->end() || key<i->first ? this->end() : i;
    }
    const_iterator Find(const K& key) const {
        const_iterator i=LowerBound(key);
        return i==this->end() || key<i->first ? this->end() : i;
    }
private:
    CompareType Compare;
    bool Dirty;
};
#endif

#ifndef FABLE_ENGINE_ETCINTERFACETYPE_H
#define FABLE_ENGINE_ETCINTERFACETYPE_H

enum ETCInterfaceType {
    TCI_NULL = 0,
    TCI_PERFORM_ACTION = 1,
    TCI_PHYSICS = 2,
    TCI_APPEARANCE_MORPH = 3,
    TCI_HERO_STATS = 4,
    TCI_WOUND = 5,
    TCI_SMOKE_GENERATOR = 6,
    TCI_NAVIGATION = 7,
    TCI_TARGETING = 8,
    TCI_TARGETED = 9,
    TCI_CREATURE_STATS = 10,
    TCI_CREATURE_AI = 11,
    TCI_CREATURE_EXPRESSION = 12,
    TCI_ON_ACTION_USE = 13,
    TCI_CARRIED_ON_ACTION_USE = 14,
    TCI_TALK = 15,
    TCI_SOUND_PLAYER = 16,
    TCI_INVENTORY = 17,
    TCI_INVENTORY_CLOTHING = 18,
    TCI_INVENTORY_WEAPONS = 19,
    TCI_INVENTORY_STATS = 20,
    TCI_INVENTORY_MAGIC = 21,
    TCI_INVENTORY_EXPERIENCE = 22,
    TCI_INVENTORY_TRADE = 23,
    TCI_INVENTORY_AUGMENT = 24,
    TCI_INVENTORY_QUESTS = 25,
    TCI_INVENTORY_ITEM = 26,
    TCI_INVENTORY_MAP = 27,
    TCI_EDITOR = 28,
    TCI_EDITOR_ANIMATION_THING = 29,
    TCI_EDITOR_ANIMATION_CREATURE = 30,
    TCI_SCRIPTED_CONTROL = 31,
    TCI_MESH_AUTOMATIC_ENTITY_CREATOR = 32,
    TCI_THING_OWNER = 33,
    TCI_VILLAGE = 34,
    TCI_VILLAGE_MEMBER = 35,
    TCI_JACK_SHIELD = 36,
    TCI_JACK_PILLAR = 37,
    TCI_DOOR = 38,
    TCI_LIGHT = 39,
    TCI_CLOCK = 40,
    TCI_HERO = 41,
    TCI_ROTATE = 42,
    TCI_RESPAWN = 43,
    TCI_WEATHERVANE = 44,
    TCI_SINGLE_USER_ACTION_POINTS = 45,
    TCI_NAMED_USER_ACTION_POINTS = 46,
    TCI_EXCLUSIVE_USE = 47,
    TCI_COMBAT = 48,
    TCI_ENTITY_MODE_MANAGER = 49,
    TCI_CREATED_ENTITY = 50,
    TCI_OWNED_ENTITY = 51,
    TCI_PERCEIVE_THING = 52,
    TCI_BED = 53,
    TCI_STEALTH = 54,
    TCI_NOISE = 55,
    TCI_CUPBOARD = 56,
    TCI_TROPHY = 57,
    TCI_MAPWHO = 58,
    TCI_DRIVER_QUEST = 59,
    TCI_CONTAINER = 60,
    TCI_CREATURE_GENERATOR = 61,
    TCI_DRIVER_CAMERA_POINTS = 62,
    TCI_CAMERA_POINT = 63,
    TCI_DISCOVERY = 64,
    TCI_EXPLOSION = 65,
    TCI_ENTITY_EVENTS = 66,
    TCI_LOOK = 67,
    TCI_SHAPE_MANAGER = 68,
    TCI_EMOTEICON = 69,
    TCI_CARRYING = 70,
    TCI_CARRYABLE = 71,
    TCI_WEAPON = 72,
    TCI_ENEMY = 73,
    TCI_BANDIT = 74,
    TCI_PARTICLE_EMITTER = 75,
    TCI_WEAPON_TRAIL = 76,
    TCI_SOLID_TO_THING = 77,
    TCI_SMASHABLE = 78,
    TCI_CREATURE_OPINION_OF_HERO = 79,
    TCI_OPINION_SOURCE = 80,
    TCI_HERO_OPINION_DEED_LOG = 81,
    TCI_HERO_PODIUM = 82,
    TCI_SHOP = 83,
    TCI_STOCK_ITEM = 84,
    TCI_BLACKSMITHS = 85,
    TCI_TIME_APPEARANCE_FADE = 86,
    TCI_FAMILY_MEMBER = 87,
    TCI_NAVIGATION_SEED = 88,
    TCI_GIFT = 89,
    TCI_ANIMATION = 90,
    TCI_GRAPHIC_APPEARANCE_NEW = 91,
    TCI_GRAPHIC_APPEARANCE_BASIC_COMPONENT = 92,
    TCI_HERO_SUIT = 93,
    TCI_HERO_ATTACHABLE_APPEARANCE_MODIFIERS = 94,
    TCI_HIGHLIGHT_ITEM = 95,
    TCI_EMPTY_DRIVER = 96,
    TCI_CREATURE_ITEM_LEVITATION = 97,
    TCI_CREATURE_HIVE = 98,
    TCI_REGION_ENTRANCE = 99,
    TCI_REGION_EXIT = 100,
    TCI_ACTIVATION_RECEPTOR = 101,
    TCI_ACTIVATION_TRIGGER = 102,
    TCI_DEGRADABLE = 103,
    TCI_HERO_EXPERIENCE = 104,
    TCI_EXPERIENCE_ORB = 105,
    TCI_HERO_CENTRE = 106,
    TCI_HERO_GUIDE = 107,
    TCI_QUEST_CARD = 108,
    TCI_FLAMMABLE = 109,
    TCI_HERO_CENTRE_DOOR_MARKER = 110,
    TCI_HERO_ABILITIES = 111,
    TCI_BOASTING_PODIUM = 112,
    TCI_BOASTING_POSITION = 113,
    TCI_BOASTING_AREA = 114,
    TCI_BOASTING_CROWD_CONTROL = 115,
    TCI_CREATURE_GROUP_BOASTING_CROWD = 116,
    TCI_VOLUME_CONTAINMENT_TRACKER = 117,
    TCI_DEAD_SHOT_IN_THING_TRACKER = 118,
    TCI_SHOT_DEFINITION = 119,
    TCI_SKELETAL_MORPH = 120,
    TCI_SWITCHABLE_NAVIGATION = 121,
    TCI_REGION_FOLLOWER = 122,
    TCI_GATHER_POINT = 123,
    TCI_DAY_OR_NIGHT_ONLY_SUPPORT = 124,
    TCI_TRAP = 125,
    TCI_UNDEAD_SOUL = 126,
    TCI_HIT_LOCATIONS = 127,
    TCI_MONEY_RECEIVER = 128,
    TCI_HERO_RANGED_ATTACKS = 129,
    TCI_PARTICLE_ATTACHER = 130,
    TCI_ANIMATING_OBJECT = 131,
    TCI_BERSERK = 132,
    TCI_DIVINE_WRATH = 133,
    TCI_UNHOLY_POWER = 134,
    TCI_MULTI_STRIKE = 135,
    TCI_MULTI_ARROW = 136,
    TCI_BURNT_EFFECT = 137,
    TCI_ELECTROCUTED_EFFECT = 138,
    TCI_CREATURE_TINT_EFFECT = 139,
    TCI_HASTE = 140,
    TCI_FORCE_PUSHED = 141,
    TCI_REGION_DISPLAY = 142,
    TCI_EXPRESSION = 143,
    TCI_SHOP_KEEPER = 144,
    TCI_SUMMON_SPELL = 145,
    TCI_BULLET_TIME = 146,
    TCI_PHYSICAL_SHIELD_SPELL = 147,
    TCI_WILL_LIGHTNING = 148,
    TCI_WILL_ENFLAME = 149,
    TCI_FORCE_PUSH_POWER = 150,
    TCI_DRAIN_LIFE = 151,
    TCI_LIFE_DRAINABLE = 152,
    TCI_HEAL_LIFE = 153,
    TCI_GHOST_SWORD = 154,
    TCI_EFFECT_ON_DIE = 155,
    TCI_WILL_RESPONSE = 156,
    TCI_FIREBALL_SPELL = 157,
    TCI_TURNCOAT_SPELL = 158,
    TCI_TURNCOAT = 159,
    TCI_SUMMONABLE_CREATURE = 160,
    TCI_SUMMONNED_CREATURE = 161,
    TCI_INTERNAL_MARKER = 162,
    TCI_CREATURE_GROUP = 163,
    TCI_CREATURE_GROUP_TAG = 164,
    TCI_MESH_REPLACER = 165,
    TCI_PRE_CALCULATED_NAVIGATION_ROUTE = 166,
    TCI_SPECIAL_ABILITIES = 167,
    TCI_BATTLE_CHARGE = 168,
    TCI_ASSASSIN_RUSH = 169,
    TCI_AI_SCRATCHPAD = 170,
    TCI_TAVERN = 171,
    TCI_SINGLE_PERSON_OCCUPIABLE = 172,
    TCI_BOSS = 173,
    TCI_ROCK_TROLL_SHIELD = 174,
    TCI_COMBAT_LEADER = 175,
    TCI_FISHING_SPOT = 176,
    TCI_FISHING = 177,
    TCI_STEALABLE_ITEM_LOCATION = 178,
    TCI_INTERESTING_TO_VILLAGERS = 179,
    TCI_GUARD = 180,
    TCI_CRATE_STACK = 181,
    TCI_WATER_WADER = 182,
    TCI_OVERHEAD_DISPLAY = 183,
    TCI_TAVERN_TABLE = 184,
    TCI_TAVERN_BAR = 185,
    TCI_TAVERN_TANKARD = 186,
    TCI_TAVERN_JUG = 187,
    TCI_FOLLOWED = 188,
    TCI_TRESPASSABLE = 189,
    TCI_TRESPASSER = 190,
    TCI_OBJECT_AUGMENTATIONS = 191,
    TCI_AUGMENTATION = 192,
    TCI_GOSSIP = 193,
    TCI_ACTION_USE_SCRIPTED_HOOK = 194,
    TCI_DRUNKENNESS = 195,
    TCI_ON_HERO_PUSH = 196,
    TCI_VILLAGE_CRIME = 197,
    TCI_IN_GAME_MENU = 198,
    TCI_COOP_SPIRIT = 199,
    TCI_HIT_ON_COLLISION = 200,
    TCI_GENERATES_EXPERIENCE_ON_KILLING = 201,
    TCI_CREATURE_GENERATOR_CREATOR = 202,
    TCI_HERO_RECEIVE_ITEMS = 203,
    TCI_GOLD = 204,
    TCI_CREATURE_HIT_NOTIFICATION = 205,
    TCI_AICREATURE_WILL_POWER_INDICATOR = 206,
    TCI_TAVERN_GAME = 207,
    TCI_GAME_PLAYER = 208,
    TCI_DECAPITATION = 209,
    TCI_TAVERN_GAME_MASTER = 210,
    TCI_IDLE_SCHEDULER = 211,
    TCI_SLEEP = 212,
    TCI_BUYABLE_HOUSE = 213,
    TCI_DUMMY_VILLAGER = 214,
    TCI_WIFE = 215,
    TCI_PARTICLE_ATTACHMENT_TRACKER = 216,
    TCI_CREATURE_FLY_ON_SPLINE = 217,
    TCI_WALL_MOUNT = 218,
    TCI_WALL_MOUNT_EFFECTS = 219,
    TCI_FISH = 220,
    TCI_TELEPORTER = 221,
    TCI_EXPLODING_OBJECT = 222,
    TCI_HERO_SPECIAL_MOVEMENT = 223,
    TCI_HERO_REMEMBER_CRIMES = 224,
    TCI_RESURRECTION_ITEM = 225,
    TCI_KRAKEN = 226,
    TCI_KRAKEN_TENTACLE = 227,
    TCI_CREATURE_FLY_IN_CIRCLES = 228,
    TCI_INFO_DISPLAY = 229,
    TCI_SPOT_LIGHT = 230,
    TCI_AMBIENT_BIRD = 231,
    TCI_BOAST_UI = 232,
    TCI_TEXTURE_DECAL = 233,
    TCI_FISHING_ROD = 234,
    TCI_THING_UID_DUMMY_FOR_SERIALISATION = 235,
    TCI_RUMBLE = 236,
    TCI_PHYSICAL_OBSTRUCTION = 237,
    TCI_ELECTROCUTE_ON_TOUCH = 238,
    TCI_DIGGING_SPOT = 239,
    TCI_DICE_GAME = 240,
    TCI_PLOT = 241,
    TCI_BOB = 242,
    TCI_FINE_DIALOGUE = 243,
    TCI_SOUND_ATMOSPHERE = 244,
    TCI_NYMPH = 245,
    TCI_QUICK_ACCESS_MENU = 246,
    TCI_WOBBLE = 247,
    TCI_CAMERA_COLLISION = 248,
    TCI_FADE_OUT_AND_IN = 249,
    TCI_AI_SUMMON = 250,
    TCI_FLASH_ON_HIT = 251,
    TCI_QUEST_COMPLETION_UI = 252,
    TCI_DESTROY_THING_ON_ACTION_FINISH = 253,
    TCI_OBSTACLE = 254,
    TCI_ATMOS_PLAYER = 255,
    TCI_FLOURISH_TARGET = 256,
    TCI_CREDITS_UI = 257,
    TCI_BETTING = 258,
    TCI_LIFE_TIMER = 259,
    TCI_FIRE_RANGED_WEAPON_ON_LEAVING_STATE_GROUP = 260,
    TCI_AVOID_REGION_EXIT = 261,
    TCI_GRAPHIC_APPEARANCE_STATIC_MESH_FADE_OUT_ON_PROXIMITY = 262,
    TCI_ORACLE_MINIGAME = 263,
    TCI_FIREHEART_MINIGAME = 264,
    TCI_DESTROY_THING_ON_SPECIAL_ABILITY_FINISH = 265,
    TCI_LIGHTNING_ORB = 266,
    TCI_LIGHTNING_NODE = 267,
    TCI_HAS_NAME = 268,
    TCI_HERO_ONLINE_SCOREBOARD = 269,
    TCI_PHOTOJOURNAL_CAPTURE = 270,
    TCI_EXPLOSIVE_TRAIL = 271,
    TCI_ABSORB_HITS = 272,
    TCI_GENERATED_CREATURE = 273,
    MAX_NO_TCI_TYPES = 274
};
#endif

class C3DVector; class CTCPhysicsBase; class CTCBase; class CTCDriverBase;
class CThingBaseDef; class CWorld; class CPlayerManager;class CTCParamsBase;
namespace NGraphicAppearance { class CTCGraphicAppearance; }
#ifdef FABLE_RETAIL_TYPED_GETTC


template<class From,class To> inline To __fastcall GFDebugPDynamicCast(From value,To) { return static_cast<To>(value); }
#endif
class CThing : public CClass {
#ifdef FABLE_ONHIT_TEST_ACCESS
    friend class COnHitFixture;
#endif

#ifdef FABLE_AUTOPLACE_TEST_ACCESS
    friend class CAutoPlaceFixture;
#endif
protected:
#ifdef FABLE_RETAIL_CTHING_CONSTRUCTORS
    CThing(unsigned long);
#endif
    unsigned __int64 UniqueID;
    std::bitset<274> UsingComponents;
    CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> > Components;
    std::list<CTCBase*> UpdateComponentsList;
    CCharString SerialisationSectionName;
    CTCDriverBase* PTCDriver;
    CTCPhysicsBase* PhysicsTC;
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;
    long WorldFrameCreatedAt;
    unsigned long ThingTypeFlags;
    CDefPointer<const CThingBaseDef> PBaseDef;
    CCharString ScriptName;
    CCharString ScriptData;
public:
    CThing* RenderListPrev;
    CThing* RenderListNext;
    CThing* UpdateListPrev;
    CThing* UpdateListNext;
protected:
    unsigned short DefGlobalIndex;
    short CurrentMapNumber;
    signed char PlayerNumber;

protected:
    unsigned char Dead : 1;
    unsigned char CreatedByOwnerThingCreation : 1;
    unsigned char IncludeInThingScans : 1;
    unsigned char Activated : 1;
    unsigned char GloballyLoaded : 1;
    unsigned char InLimbo : 1;
    unsigned char KillLocked : 1;
    unsigned char CreatedBySerialisation : 1;
    unsigned char DeletedOnOwnerLevelOutOfScope : 1;
    unsigned char Drawable : 1;
    unsigned char Dying : 1;
    unsigned char ForceFrameUpdate : 1;
    unsigned char LevelPersistent : 1;
    unsigned char GameStatePersisted : 1;
    unsigned char PersistOnUnloadWhenInLimbo : 1;
    unsigned char PausedDuringCutscenes : 1;
    unsigned char NotPausedDuringCutscenes : 1;
    unsigned char DeathPersistent : 1;
    unsigned char KillOnLevelUnload : 1;
    unsigned char ActivatedScript : 1;
    unsigned char LevelIsUnloading : 1;
    unsigned char IsAffectedByBulletTimeOverride : 1;
    unsigned char AddingTCsPermitted : 1;

private:
    unsigned char InitialisationStage : 3;
public:
    void Kill(bool); 
    CTCBase* AddTC(const CCharString&,bool,const CTCParamsBase*);
    CDefPointer<const CThingBaseDef> GetPBaseDef() const { return PBaseDef; }
    bool IsDead() const { return Dead!=0; }
    bool IsIncludedInThingScans() const { return IncludeInThingScans && Activated && !Dead && !InLimbo; }
    bool IsAlive() const { return !Dead; }
    bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
#ifdef FABLE_RETAIL_TYPED_GETTC
    template<class T> __forceinline bool GetTC(T** result) {
        if(HasTC(T::StatGetInterfaceType())) {
            *result=GFDebugPDynamicCast(Components.Find(T::StatGetInterfaceType())->second,(T*)0);
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool PeekTC(T** result) const {
        if(HasTC(T::StatGetInterfaceType())) {
            *result=GFDebugPDynamicCast(static_cast<const CTCBase*>(Components.Find(T::StatGetInterfaceType())->second),(T*)0);
            return true;
        }
        return false;
    }
#endif
#ifdef FABLE_RETAIL_TYPED_GETTCDRIVER
    
    
    template<class T> __forceinline bool GetTCDriver(T** result) {
        const bool& present=HasTC(T::StatGetInterfaceType());
        if(present) *result=GFDebugPDynamicCast(&GetTC(T::StatGetInterfaceType()),(T*)0);
        return present;
    }
#endif
#ifdef FABLE_RETAIL_FORCE_GETTC
    __forceinline CTCBase& GetTC(ETCInterfaceType type) { return *Components.Find(type)->second; }
    __forceinline const CTCBase& GetTC(ETCInterfaceType type) const { return *Components.Find(type)->second; }
#else
    CTCBase& GetTC(ETCInterfaceType type) { return *Components.Find(type)->second; }
    const CTCBase& GetTC(ETCInterfaceType type) const { return *Components.Find(type)->second; }
#endif
    CTCPhysicsBase* GetPhysicsTC() { return PhysicsTC; }
    static void CheckRetailLayout() {
        FABLE_STATIC_ASSERT(offsetof(CThing,UniqueID)==0x18);
        FABLE_STATIC_ASSERT(offsetof(CThing,UsingComponents)==0x20);
        FABLE_STATIC_ASSERT(offsetof(CThing,Components)==0x44);
        FABLE_STATIC_ASSERT(offsetof(CThing,PhysicsTC)==0x60);
        FABLE_STATIC_ASSERT(offsetof(CThing,PBaseDef)==0x70);
    }
    const CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
    const C3DVector& GetPos() const;
    C3DVector GetPhysPos() const;
protected:
    CWorld& GetWorld();
    const CWorld& PeekWorld() const;
    unsigned long* GetPWorldSeed() const;
    CPlayerManager& GetPlayerManager();
};
FABLE_STATIC_ASSERT(sizeof(CThing)==0x98);

#endif
#else
#ifndef FABLE_ENGINE_CTHING_H
#define FABLE_ENGINE_CTHING_H






#include <stddef.h>


class CCharStringData;
class CDefPointeeBase;
class CTCDriverBase;
struct CTCGraphicAppearance;
class CTCPhysicsBase;

#pragma pack(push, 1)
class CThing {
public:
    void*                 __vftable;                         
    unsigned char         _pad_0x04[0x4];                    
    unsigned char         _pad_0x08[0x4];                    
    unsigned char         _pad_0x0c[0x4];                    
    unsigned char         _pad_0x10[0x1];                    
    unsigned char         _pad_0x11[0x7];                    
    unsigned __int64      UniqueID;                          
    unsigned char         UsingComponents[0x24];             
    unsigned char         Components[0x10];                  
    unsigned char         UpdateComponentsList[0x4];         
    CCharStringData*      SerialisationSectionName_PStringData; 
    CTCDriverBase*        PTCDriver;                         
    CTCPhysicsBase*       PhysicsTC;                         
    CTCGraphicAppearance* PTCGraphicAppearance;              
    long                  WorldFrameCreatedAt;               
    unsigned long         ThingTypeFlags;                    
    CDefPointeeBase*      PBaseDef_Object;                   
    CCharStringData*      ScriptName_PStringData;            
    CCharStringData*      ScriptData_PStringData;            
    CThing*               RenderListPrev;                    
    CThing*               RenderListNext;                    
    CThing*               UpdateListPrev;                    
    CThing*               UpdateListNext;                    
    unsigned short        DefGlobalIndex;                    
    short                 CurrentMapNumber;                  
    signed char           PlayerNumber;                      
    unsigned char         CreatedBySerialisation[0x1];       
    unsigned char         PausedDuringCutscenes[0x1];        
    unsigned char         AddingTCsPermitted[0x1];           
    unsigned char         InitialisationStage[0x4];          
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CThing) == 0x98);
FABLE_STATIC_ASSERT(offsetof(CThing, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CThing, UniqueID) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CThing, UsingComponents) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CThing, Components) == 0x44);
FABLE_STATIC_ASSERT(offsetof(CThing, UpdateComponentsList) == 0x54);
FABLE_STATIC_ASSERT(offsetof(CThing, SerialisationSectionName_PStringData) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CThing, PTCDriver) == 0x5c);
FABLE_STATIC_ASSERT(offsetof(CThing, PhysicsTC) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CThing, PTCGraphicAppearance) == 0x64);
FABLE_STATIC_ASSERT(offsetof(CThing, WorldFrameCreatedAt) == 0x68);
FABLE_STATIC_ASSERT(offsetof(CThing, ThingTypeFlags) == 0x6c);
FABLE_STATIC_ASSERT(offsetof(CThing, PBaseDef_Object) == 0x70);
FABLE_STATIC_ASSERT(offsetof(CThing, ScriptName_PStringData) == 0x74);
FABLE_STATIC_ASSERT(offsetof(CThing, ScriptData_PStringData) == 0x78);
FABLE_STATIC_ASSERT(offsetof(CThing, RenderListPrev) == 0x7c);
FABLE_STATIC_ASSERT(offsetof(CThing, RenderListNext) == 0x80);
FABLE_STATIC_ASSERT(offsetof(CThing, UpdateListPrev) == 0x84);
FABLE_STATIC_ASSERT(offsetof(CThing, UpdateListNext) == 0x88);
FABLE_STATIC_ASSERT(offsetof(CThing, DefGlobalIndex) == 0x8c);
FABLE_STATIC_ASSERT(offsetof(CThing, CurrentMapNumber) == 0x8e);
FABLE_STATIC_ASSERT(offsetof(CThing, PlayerNumber) == 0x90);
FABLE_STATIC_ASSERT(offsetof(CThing, CreatedBySerialisation) == 0x91);
FABLE_STATIC_ASSERT(offsetof(CThing, PausedDuringCutscenes) == 0x92);
FABLE_STATIC_ASSERT(offsetof(CThing, AddingTCsPermitted) == 0x93);
FABLE_STATIC_ASSERT(offsetof(CThing, InitialisationStage) == 0x94);

#endif 

#endif

#ifdef FABLE_RETAIL_CTCPHYSICSBASE
#ifndef FABLE_ENGINE_CTCPHYSICSBASE_H
#define FABLE_ENGINE_CTCPHYSICSBASE_H
class CPhysicsMeshObject;

#ifndef FABLE_ENGINE_CTCBASE_H
#define FABLE_ENGINE_CTCBASE_H
















#ifdef FABLE_RETAIL_CTCBASE
#include <stddef.h>

#ifndef FABLE_ENGINE_CBASECLASSNONCOPYABLE_H
#define FABLE_ENGINE_CBASECLASSNONCOPYABLE_H






class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();
    virtual ~CBaseClassNonCopyable();                                    
private:
    CBaseClassNonCopyable(const CBaseClassNonCopyable&);
    CBaseClassNonCopyable& operator=(const CBaseClassNonCopyable&);
};
FABLE_STATIC_ASSERT(sizeof(CBaseClassNonCopyable) == 0x4);

#endif 



#ifndef FABLE_ENGINE_C3DVECTOR_H
#define FABLE_ENGINE_C3DVECTOR_H


#ifdef _MSC_VER
#define FABLE_VECTOR_INLINE __forceinline
#else
#define FABLE_VECTOR_INLINE inline
#endif
#ifdef FABLE_RETAIL_VECTOR_GEOMETRY
#ifndef FABLE_ENGINE_GFMATHS_H
#define FABLE_ENGINE_GFMATHS_H
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
extern const float DELTA_NOTIONAL_ZERO;
extern unsigned long GFastOneOverSqrtTable[128];
inline float __fastcall GFFloatAbs(float n){
#ifdef _MSC_VER
 *(unsigned long*)&n &= 0x7fffffffUL;
#else
 *(unsigned int*)&n &= 0x7fffffffU;
#endif
 return n;
}
#ifdef _MSC_VER
__forceinline
#else
inline
#endif
float __fastcall GFFastOneOverSqrt(float n){
#ifdef _MSC_VER
 typedef unsigned long word_type;
#else
 typedef unsigned int word_type;
#endif
 word_type bits=*(word_type*)&n;
 word_type exponent=(0x17c-((bits>>23)&0xff))<<22;
 word_type mantissa=(word_type)GFastOneOverSqrtTable[(bits>>17)&0x7f]<<15;
 float estimate;
 *(word_type*)&estimate=(exponent&0xff800000)|mantissa;
 return (3.0f-estimate*estimate*n)*estimate*0.5f;
}
#endif

#endif
#ifdef FABLE_RETAIL_VALIDPOSITIONS
#ifndef FABLE_ENGINE_C2DCOORDI_H
#define FABLE_ENGINE_C2DCOORDI_H


class C2DCoordI { public: long X,Y;C2DCoordI(){}C2DCoordI(long x,long y):X(x),Y(y){} };
FABLE_STATIC_ASSERT(sizeof(C2DCoordI)==8);
#endif

#endif
class C3DVector {
public:
    float X,Y,Z;
    C3DVector() {}
#ifdef FABLE_RETAIL_VECTOR_SPLINE_CTOR
    C3DVector(float x,float y,float z){X=x;Y=y;Z=z;}
#else
#ifdef FABLE_RETAIL_VALIDPOSITIONS
    C3DVector(float x,float y,float z){Initialise(x,y,z);}
#else
    C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
#endif
#endif
#if defined(FABLE_RETAIL_VECTOR_GEOMETRY) || defined(FABLE_RETAIL_VECTOR_SPLINE_COPY)
    C3DVector(const C3DVector& p){*this=p;}
#else
    C3DVector(const C3DVector& other):X(other.X),Y(other.Y),Z(other.Z){}
#endif
    FABLE_VECTOR_INLINE void Initialise(float x,float y,float z){X=x;Y=y;Z=z;}
    FABLE_VECTOR_INLINE const C3DVector operator-(const C3DVector& other)const{return C3DVector(X-other.X,Y-other.Y,Z-other.Z);}
    FABLE_VECTOR_INLINE const C3DVector operator+(const C3DVector& other)const{return C3DVector(X+other.X,Y+other.Y,Z+other.Z);}
    FABLE_VECTOR_INLINE const C3DVector operator*(float scale)const{return C3DVector(X*scale,Y*scale,Z*scale);}
    FABLE_VECTOR_INLINE bool operator==(const C3DVector& other)const{return X==other.X&&Y==other.Y&&Z==other.Z;}
    FABLE_VECTOR_INLINE float GetSquaredMagnitude()const{return X*X+Z*Z+Y*Y;}
    FABLE_VECTOR_INLINE float GetSquaredMagnitude2D()const{return X*X+Y*Y;}
    FABLE_VECTOR_INLINE const float Dot(const C3DVector& other)const{return X*other.X+Y*other.Y+Z*other.Z;}
    FABLE_VECTOR_INLINE const C3DVector Cross(const C3DVector& other)const{return C3DVector(Y*other.Z-Z*other.Y,Z*other.X-X*other.Z,X*other.Y-Y*other.X);}
#ifdef FABLE_RETAIL_VECTOR_GEOMETRY
    FABLE_VECTOR_INLINE void Clear(){X=Y=Z=0.0f;}
    FABLE_VECTOR_INLINE const C3DVector& operator+=(const C3DVector& v){X+=v.X;Y+=v.Y;Z+=v.Z;return *this;}
    FABLE_VECTOR_INLINE const C3DVector& operator*=(float v){X*=v;Y*=v;Z*=v;return *this;}
    FABLE_VECTOR_INLINE float GetMagnitude()const{return (float)sqrt(GetSquaredMagnitude());}
    FABLE_VECTOR_INLINE void LimitLower(const C3DVector& v){if(X<v.X)X=v.X;if(Y<v.Y)Y=v.Y;if(Z<v.Z)Z=v.Z;}
    FABLE_VECTOR_INLINE void LimitUpper(const C3DVector& v){if(X>v.X)X=v.X;if(Y>v.Y)Y=v.Y;if(Z>v.Z)Z=v.Z;}
    FABLE_VECTOR_INLINE bool IsNonZero()const{return DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO<X*X+Y*Y+Z*Z;}
    FABLE_VECTOR_INLINE C3DVector& Normalise(){float one_on_magnitude=GFFastOneOverSqrt(GetSquaredMagnitude());X*=one_on_magnitude;Y*=one_on_magnitude;Z*=one_on_magnitude;return *this;}
#endif
#ifdef FABLE_RETAIL_VECTOR_SPLINE
    FABLE_VECTOR_INLINE const C3DVector& operator+=(const C3DVector& v){X+=v.X;Y+=v.Y;Z+=v.Z;return *this;}
    FABLE_VECTOR_INLINE const C3DVector& operator*=(float v){X*=v;Y*=v;Z*=v;return *this;}
    FABLE_VECTOR_INLINE const C3DVector operator/(float v)const{float reciprocal=1.0f/v;return C3DVector(X*reciprocal,Y*reciprocal,Z*reciprocal);}
    FABLE_VECTOR_INLINE const C3DVector& operator/=(float v){float reciprocal=1.0f/v;X*=reciprocal;Y*=reciprocal;Z*=reciprocal;return *this;}
    FABLE_VECTOR_INLINE float GetMagnitude()const{return (float)sqrt(GetSquaredMagnitude());}
#endif
#ifdef FABLE_RETAIL_VALIDPOSITIONS
    inline C2DCoordI ToC2DCoordI() const { return C2DCoordI((long)X,(long)Y); }
#endif
    C3DVector& AccurateNormalise();
    void Rotate(const C3DVector&,float);
};
#undef FABLE_VECTOR_INLINE
#endif

class CEnginePrimitivePrepareInfo;
class CInterfaceType;
class CPersistContext;
class CTCParamsBase;
namespace NTCMessages { class CMessageBase; }
class CTCBase : public CBaseClassNonCopyable {
public:
#ifdef FABLE_RETAIL_CTHING_CONSTRUCTORS
    CTCBase(CThing&);
#endif
protected:
    virtual void OnSerialise(CPersistContext &,bool);
public:
    virtual ~CTCBase();
public:
    virtual void OnRemove();
public:
    virtual void ConstructFromParams(const CTCParamsBase &);
public:
    virtual void OnCreate();
public:
    virtual void InitialActivate();
public:
    virtual void OnInitialActivate();
public:
    virtual void RenderUpdate(float);
public:
    virtual bool HasFrameUpdate() const;
public:
    virtual void FrameUpdate();
public:
    virtual void BulletTimeFrameUpdate();
public:
    virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo &);
public:
    virtual void Copy(const CTCBase &);
public:
    virtual void OnDie();
public:
    virtual void OnKill();
public:
    virtual void OnLevelUnload();
public:
    virtual void OnSetInLimbo(bool);
public:
    virtual bool Validate(CCharString &) const;
public:
    virtual bool IsFreeForDeletion() const;
public:
    virtual bool IsSerialiseCreatable() const;
public:
    virtual bool IsSerialisableForHeroState() const;
public:
    virtual ETCInterfaceType GetInterfaceType() const=0;
public:
    virtual CCharString GetClassTypeName() const=0;
public:
    virtual bool IsComponentSerialisable() const;
public:
    virtual bool IsDriver() const;
public:
    virtual bool IsCopyable() const;
public:
    virtual void GlobalUpdate(bool);
public:
    virtual void HandleMessage(const NTCMessages::CMessageBase &);
protected:
    CThing& ParentThing;
private:
    bool ToBeRemoved;
    bool IsInGlobalUpdateTCs;
};
FABLE_STATIC_ASSERT(sizeof(CTCBase)==12);
#else 
#include <stddef.h>


class CThing;

#pragma pack(push, 1)
class CTCBase {
public:
    void*         __vftable;                                 
    CThing*       ParentThing;                               
    bool          ToBeRemoved;                               
    bool          IsInGlobalUpdateTCs;                       
    unsigned char _pad_0x0a[0x2];                            
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CTCBase) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CTCBase, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CTCBase, ParentThing) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CTCBase, ToBeRemoved) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CTCBase, IsInGlobalUpdateTCs) == 0x9);

#endif 

#endif 




class C2DVector;
class CInterfaceType;
class CPersistContext;
class CPhysicsCollideResponse;
class CPhysicsDebugInfo;
class CRightHandedSet;
class CTCParamsBase;
class CThingPhysicalDebugInfo;
class CThingPhysicsMeshInfo;
class CTCPhysicsBase : public CTCBase {
#ifdef FABLE_AUTOPLACE_TEST_ACCESS
    friend class CAutoPlaceFixture;
#endif
public:
#ifdef FABLE_RETAIL_CTHING_CONSTRUCTORS
    CTCPhysicsBase(CThing&);
#endif
    virtual ETCInterfaceType GetInterfaceType() const;
protected:
    virtual C2DVector GetOffsetVector() const;
protected:
    virtual C3DVector GetOffsetVector3D() const;
public:
    virtual void SetPosition(const C3DVector &);
public:
    virtual void ResetPosition(const C3DVector &);
public:
    virtual void ResetRHSet(const CRightHandedSet &);
public:
    virtual void SetPositionToNavigateTo(const C3DVector &);
public:
    virtual void SetForcedToMovePos(const C3DVector &,bool);
public:
    virtual void ResetMovementState();
public:
    virtual void EnablePhysics(bool);
public:
    virtual bool IsPhysicsEnabled() const;
public:
    virtual void EnableGravity(bool);
public:
    virtual bool IsGravityEnabled() const;
public:
    virtual void EnableNavigator(bool);
public:
    virtual bool IsNavigationEnabled() const;
public:
    virtual bool HasNavigationLayers() const;
public:
    virtual void SetInteractionFlags(long);
public:
    virtual void ReturnToDefaultInteractionFlags();
public:
    virtual void OnDroppedFromUpdateList();
public:
    virtual void OnAddedToUpdateList();
public:
    virtual bool IsAlwaysFacingMovementDirection() const;
public:
    virtual bool IsAffectedByGravity() const;
public:
    virtual bool IsAffectedByAir() const;
public:
    virtual bool IsAffectedByWind() const;
public:
    virtual bool IsAngledWithGround() const;
public:
    virtual bool IsImmoveable() const;
public:
    virtual bool IsOnAnySurface() const;
public:
    virtual bool IsOnAnyStationarySurface() const;
public:
    virtual bool IsOnMeshSurface() const;
public:
    virtual bool IsOnGroundSurface() const;
public:
    virtual bool CanCollideWithThings() const;
public:
    virtual bool CanCollideWithThingsOneWay() const;
public:
    virtual bool CanCollideWithLandscape() const;
public:
    virtual bool CanInteractWithThings() const;
public:
    virtual bool IsExcludingCreatures() const;
public:
    virtual bool IsPhysicsMeshImpassable() const;
public:
    virtual float GetFacingAngleXY() const;
public:
    virtual float GetFacingAngleYZ() const;
public:
    virtual float GetFacingAngleXZ() const;
public:
    virtual void SetFacingAngleXY(float);
public:
    virtual void SetFacingAngleYZ(float);
public:
    virtual void SetFacingAngleXZ(float);
public:
    virtual C3DVector GetFacingVector(float) const;
public:
    virtual C3DVector GetFacingVector() const;
public:
    virtual const CRightHandedSet & GetRHSet() const;
public:
    virtual CRightHandedSet GetRHSet(float) const;
public:
    virtual const CRightHandedSet & GetLastRHSet() const;
public:
    virtual void SetFacingAnglesFromRHSet(const CRightHandedSet &);
public:
    virtual void SetRHSet(const CRightHandedSet &);
public:
    virtual C3DVector GetImpulse() const;
public:
    virtual void SetRHSetAsFlushOnNormal(const C3DVector &);
public:
    virtual void SetRHSetAsFlushOnGround();
public:
    virtual float GetRenderZOffset(float) const;
public:
    virtual void ResetInterpolation();
public:
    virtual void ApplyControlledImpulse(const C3DVector &);
public:
    virtual void ApplyControlledAcceleration(const C3DVector &,float);
public:
    virtual bool IsPushableByHero() const;
public:
    virtual void SetAsPushableByHero(bool);
public:
    virtual void FrameUpdate();
public:
    virtual bool HasFrameUpdate() const;
public:
    virtual void SetPhysicsMeshBankIndex(long);
public:
    virtual void SetScaleFromOwner();
public:
    virtual void ConstructFromParams(const CTCParamsBase &);
public:
    virtual void OnCreate();
public:
    virtual void InitialActivate();
public:
    virtual void Move();
public:
    virtual bool DrawGetPhysicsDebugInfo(CThingPhysicalDebugInfo &) const;
public:
    virtual void OnSerialise(CPersistContext &,bool);
public:
    virtual bool IsComponentSerialisable() const;
public:
    virtual bool IsSerialisableForHeroState() const;
public:
    virtual bool IsCopyable() const;
public:
    virtual void FillPhysicsDebugInfoForSelf(CPhysicsDebugInfo &) const;
public:
    virtual bool HasCollidedWithThingFromTo(const CTCPhysicsBase &,const C3DVector &,const C3DVector &,CPhysicsCollideResponse &) const;
public:
    virtual bool HasCollidedWithThingAt(const CTCPhysicsBase &,const C3DVector &,CPhysicsCollideResponse &) const;
public:
    virtual bool HasCollidedWithLandscapeAt(const C3DVector &,CPhysicsCollideResponse &) const;
public:
    virtual const CThingPhysicsMeshInfo * GetPhysicsMeshInfo() const;
protected:
    virtual void UpdateInterpolation();
protected:
    virtual void UpdateOldRHSet();
protected:
    virtual void UpdateOldPos();
public:
    virtual ~CTCPhysicsBase();
protected:
    C3DVector Position;
    C3DVector OldPosition;
    long OldPositionLastFrameSet;
    C3DVector Velocity;
    float Radius;
    unsigned char PhysicsDef[4];
    bool Moving:1,OldMoving:1;
public:
    void GetBoundingInfo(C3DVector&,float&) const;
    const C3DVector& GetPosition() const { return Position; }
    float GetRadius() const { return Radius; }
    CPhysicsMeshObject GetPhysicsMeshObject() const;
};
FABLE_STATIC_ASSERT(sizeof(CTCPhysicsBase)==0x40);
#endif
#else
#ifndef FABLE_ENGINE_CTCPHYSICSBASE_H
#define FABLE_ENGINE_CTCPHYSICSBASE_H





#include <stddef.h>


class CDefPointeeBase;

#pragma pack(push, 1)
class CTCPhysicsBase {
public:
    void*            __vftable;                              
    long             sub;                                    
    unsigned char    _pad_0x08[0x4];                         
    float            Position_X;                             
    float            Position_Y;                             
    float            Position_Z;                             
    float            OldPosition_X;                          
    float            OldPosition_Y;                          
    float            OldPosition_Z;                          
    long             OldPositionLastFrameSet;                
    float            Velocity_X;                             
    float            Velocity_Y;                             
    float            Velocity_Z;                             
    float            Radius;                                 
    CDefPointeeBase* PhysicsDef_Object;                      
    unsigned char    OldMoving[0x4];                         
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CTCPhysicsBase) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, sub) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, Position_X) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, Position_Y) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, Position_Z) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, OldPosition_X) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, OldPosition_Y) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, OldPosition_Z) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, OldPositionLastFrameSet) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, Velocity_X) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, Velocity_Y) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, Velocity_Z) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, Radius) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, PhysicsDef_Object) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, OldMoving) == 0x3c);

#endif 

#endif

#ifndef FABLE_ENGINE_CPHYSICSMESHOBJECT_H
#define FABLE_ENGINE_CPHYSICSMESHOBJECT_H

class CCharString;class C3DVector;class CMeshIntersectionData;


class CPhysicsMeshObject : public CBaseClass {
private:
    unsigned char Mesh[4]; 
    unsigned char PositionMatrix[48]; 
    float Scale;
    float OO_Scale;
public:
#ifdef FABLE_RETAIL_CTHING_CONSTRUCTORS
    CPhysicsMeshObject();
#endif
    virtual ~CPhysicsMeshObject();
    bool HasContainmentVolume(const CCharString&) const;
#ifdef FABLE_RETAIL_VALIDPOSITIONS
    bool GetPointIntersectionFromTo(const C3DVector&,const C3DVector&,CMeshIntersectionData&) const;
#endif
};
FABLE_STATIC_ASSERT(sizeof(CPhysicsMeshObject)==0x40);
#endif

#ifndef FABLE_ENGINE_CMESHINTERSECTIONDATA_H
#define FABLE_ENGINE_CMESHINTERSECTIONDATA_H



class CMeshIntersectionData { public:C3DVector IntersectionPosition,Normal;float DistanceSquared; };
FABLE_STATIC_ASSERT(sizeof(CMeshIntersectionData)==28);
#endif

#ifdef FABLE_RETAIL_VALIDPOSITIONS
#ifndef FABLE_ENGINE_CWORLDMAP_H
#define FABLE_ENGINE_CWORLDMAP_H

#ifndef FABLE_ENGINE_CIDRAWWORLDMAP_H
#define FABLE_ENGINE_CIDRAWWORLDMAP_H

class CDrawMapInfo;class C2DBoxI;class C3DVector;

class CIDrawWorldMap {
public: virtual long DrawGetNoMaps() const =0;
public: virtual CDrawMapInfo DrawGetMapInfo(long) const =0;
public: virtual C2DBoxI DrawGetDimensions() const =0;
public: virtual long DrawGetGranularity() const =0;
public: virtual bool DrawIsPosInMap(const C3DVector &) const =0;
public: virtual float DrawGetSizeZAt(const C3DVector &,float) const =0;
public: virtual C3DVector DrawConvertWorldMapCoordsToLocalCoords(long,const C3DVector &) const =0;
public: virtual long DrawGetMyPlayerMapNumber() const =0;
public: virtual bool DrawFindLastUnblockedGroundPositionAndReportCollision(const C3DVector &,const C3DVector &,C3DVector &) const =0;
public: virtual bool DrawFindLastUnblockedThemePositionAndReportCollision(const C3DVector &,const C3DVector &,C3DVector &) const =0;
public: virtual long DrawGetFrame() const =0;
public: virtual ~CIDrawWorldMap();
};
FABLE_STATIC_ASSERT(sizeof(CIDrawWorldMap)==4);
#endif

#ifndef FABLE_ENGINE_CCOUNTEDPOINTER_H
#define FABLE_ENGINE_CCOUNTEDPOINTER_H











































class CCPPointerInfo {
public:
    unsigned long RefCount;                                              
    void (FABLE_FASTCALL* DeleteFunc)(void*);                             
    void* Data;                                                          
    CCPPointerInfo(unsigned long refCount, void (FABLE_FASTCALL* deleteFunc)(void*), void* data)
        : RefCount(refCount), DeleteFunc(deleteFunc), Data(data) { }     
};
FABLE_STATIC_ASSERT(sizeof(CCPPointerInfo) == 0xc);

template <class T> class CCountedPointer {
public:
    T* Data;                                                             
    CCPPointerInfo* Info;                                                

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
    unsigned long GetReferenceCount() const { return Info ? Info->RefCount : 0; }   
    void Validate() const;                                               
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
    template <class U> void SetStaticCast(const CCountedPointer<U>& other)   
    {
        ShareData(static_cast<T*>(other.Data), other.Info);              
    }
    void reset(T* data) { Reset(data); }                                 
    static void FABLE_FASTCALL DeleteData(void* data) { delete static_cast<T*>(data); }                    
};

#endif 


class CAFile;class CASuspendableProcess;class CMap;class CCharString;class C2DCoordI;
namespace std { template<class T>struct less;template<class K,class Compare,class A>class set; }

class CWorldMap : public CBaseClassNonCopyable, public CIDrawWorldMap {
public: virtual ~CWorldMap();
public: virtual void DeleteAllLevels();
public: virtual void DeleteAllRegions();
public: virtual void LoadFromFile(CAFile &,long);
public: virtual void SaveToFile(CAFile &);
public: virtual void RemoveLevel(long);
public: virtual void LoadTopologyFromFile(long,CCountedPointer<CAFile>,CASuspendableProcess &);
public: virtual void PostLoadTopology(long);
public: virtual void ForceLoad(long);
public: virtual void ForceUnload(long);
public: virtual bool IsMapLoaded(long) const;
public: virtual bool IsMapUsed(long) const;
public: virtual long GetGranularity() const;
public: virtual const C2DBoxI & GetDimensions() const;
public: virtual CMap & GetMapAt(const C2DCoordI &);
public: virtual const CMap & PeekMapAt(const C2DCoordI &) const;
public: virtual long GetMapNumberAt(const C3DVector &) const;
public: virtual long GetMapNumberAt(const C2DCoordI &) const;
public: virtual CMap & GetMap(long);
public: virtual const CMap & PeekMap(long) const;
public: virtual void GetAllMapsInArea(const C2DBoxI &,std::vector<unsigned long,std::allocator<unsigned long> > &) const;
public: virtual void GetAllLoadedMapsInArea(const C2DBoxI &,std::vector<unsigned long,std::allocator<unsigned long> > &) const;
public: virtual void PostRegionLoad(long);
public: virtual C2DCoordI GetMapPos(long) const;
public: virtual bool IsMapChangeable(long) const;
public: virtual bool IsPosChangeable(const C2DCoordI &) const;
public: virtual std::set<CCharString,std::less<CCharString>,std::allocator<CCharString> > GetLoadableSections() const;
public: virtual void Update();
public: virtual void HeightAreaChanged(const C2DBoxI *);
public: virtual long DrawGetFrame() const;
public: virtual long DrawGetNoMaps() const;
public: virtual CDrawMapInfo DrawGetMapInfo(long) const;
public: virtual C2DBoxI DrawGetDimensions() const;
public: virtual long DrawGetGranularity() const;
public: virtual bool DrawIsPosInMap(const C3DVector &) const;
public: virtual float DrawGetSizeZAt(const C3DVector &,float) const;
public: virtual C3DVector DrawConvertWorldMapCoordsToLocalCoords(long,const C3DVector &) const;
public: virtual long DrawGetMyPlayerMapNumber() const;
public: virtual bool DrawFindLastUnblockedGroundPositionAndReportCollision(const C3DVector &,const C3DVector &,C3DVector &) const;
public: virtual bool DrawFindLastUnblockedThemePositionAndReportCollision(const C3DVector &,const C3DVector &,C3DVector &) const;
protected: virtual void LoadScriptBrushesFromFile(long,CCountedPointer<CAFile>);
protected: virtual long AllocateNewMap();

public:
    bool IsPosInMap(const C3DVector&) const;
    bool IsPosPassable(const C2DCoordI&) const;
    float GetGroundSizeZAt(const C3DVector&) const;
protected:
    unsigned char _pad_0x08[0xd0]; 
};
FABLE_STATIC_ASSERT(sizeof(CWorldMap)==0xd8);
#endif
#else
#ifndef FABLE_ENGINE_CWORLDMAP_H
#define FABLE_ENGINE_CWORLDMAP_H






#include <stddef.h>


class CCPPointerInfo;
class CLevelLoader;
class CMainGameComponent;
class CRegionConnectionGraph;
struct CThingMapManager;
class CWADBank;
class CWorld;

#pragma pack(push, 1)
class CWorldMap {
public:
    void*                   __vftable;                       
    long                    f4;                              
    CWorld*                 World;                           
    CMainGameComponent*     Component;                       
    CThingMapManager*       PThingMapManager_ptr;            
    unsigned char           LoadedScriptedMapBrushes[0xc];   
    unsigned char           Maps[0xc];                       
    unsigned char           Regions[0xc];                    
    unsigned char           RegionTime[0xc];                 
    CRegionConnectionGraph* PRegionGraph_Data;               
    CCPPointerInfo*         PRegionGraph_Info;               
    bool                    SoundThemesDisabled;             
    unsigned char           _pad_0x4d[0x3];                  
    long                    Granularity;                     
    long                    GranularityShift;                
    long                    MapPlacementPitch;               
    long                    Area_TLX;                        
    long                    Area_TLY;                        
    long                    Area_BRX;                        
    long                    Area_BRY;                        
    unsigned char           MapPlacement[0xc];               
    unsigned char           PlayerPositionInfo[0xc];         
    unsigned char           HeightChangedAreas[0xc];         
    unsigned char           LoadedMaps[0xc];                 
    long                    LoadedRegion;                    
    CWADBank*               LevelWADBank_Data;               
    CCPPointerInfo*         LevelWADBank_Info;               
    bool                    UseLevelWADBank;                 
    unsigned char           _pad_0xa9[0x3];                  
    unsigned long           CurrentMapUIDCount;              
    unsigned char           DestroyedThemePositions[0xc];    
    CLevelLoader*           PLevelLoader_Data;               
    CCPPointerInfo*         PLevelLoader_Info;               
    unsigned char           PCameraRegionParticle[0x8];      
    unsigned char           PHeroBreathParticle[0x8];        
    bool                    RegionUnloading;                 
    bool                    PassabilityBeingShown;           
    unsigned char           _pad_0xd6[0x2];                  
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CWorldMap) == 0xd8);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, f4) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, World) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, Component) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, PThingMapManager_ptr) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, LoadedScriptedMapBrushes) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, Maps) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, Regions) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, RegionTime) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, PRegionGraph_Data) == 0x44);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, PRegionGraph_Info) == 0x48);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, SoundThemesDisabled) == 0x4c);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, Granularity) == 0x50);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, GranularityShift) == 0x54);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, MapPlacementPitch) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, Area_TLX) == 0x5c);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, Area_TLY) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, Area_BRX) == 0x64);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, Area_BRY) == 0x68);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, MapPlacement) == 0x6c);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, PlayerPositionInfo) == 0x78);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, HeightChangedAreas) == 0x84);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, LoadedMaps) == 0x90);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, LoadedRegion) == 0x9c);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, LevelWADBank_Data) == 0xa0);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, LevelWADBank_Info) == 0xa4);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, UseLevelWADBank) == 0xa8);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, CurrentMapUIDCount) == 0xac);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, DestroyedThemePositions) == 0xb0);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, PLevelLoader_Data) == 0xbc);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, PLevelLoader_Info) == 0xc0);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, PCameraRegionParticle) == 0xc4);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, PHeroBreathParticle) == 0xcc);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, RegionUnloading) == 0xd4);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, PassabilityBeingShown) == 0xd5);

#endif 

#endif

#ifdef FABLE_RETAIL_VALIDPOSITIONS
#ifndef FABLE_ENGINE_CTHINGSEARCHTOOLS_H
#define FABLE_ENGINE_CTHINGSEARCHTOOLS_H


class CWorldMap;class CThingManager;class C2DBoxI;
namespace NThingMap { class CThingMap;class CThingMapManager; }

class CThingSearchTools {
protected:
    CWorldMap& WorldMap;
    NThingMap::CThingMapManager* PThingMapManager;
    CThingManager& ThingManager;
    std::vector<NThingMap::CThingMap*> AreaMaps;
public:
    CThingSearchTools(CThingManager&,CWorldMap&);
    ~CThingSearchTools();
    template<class T,class Container,class Filter>
    void GetAllThingsInRoughArea(const C2DBoxI&,const Filter&,Container&) const;
};
FABLE_STATIC_ASSERT(sizeof(CThingSearchTools)==0x18);
#endif
#else
#ifndef FABLE_ENGINE_CTHINGSEARCHTOOLS_H
#define FABLE_ENGINE_CTHINGSEARCHTOOLS_H






#include <stddef.h>


class CThingManager;
struct CThingMapManager;
class CWorldMap;

#pragma pack(push, 1)
class CThingSearchTools {
public:
#ifdef FABLE_RETAIL_CTHINGSEARCHTOOLS
#ifdef FABLE_RETAIL_VISIBLE_THINGSEARCH
    template<class Filter,class T> __declspec(noinline) bool GetPFirstLeafThing(const Filter& filter,T** out) const {
        const std::list<CClass*>& things=PeekTypeList(T::StatGetType());
        for(std::list<CClass*>::const_iterator it=things.begin();it!=things.end();++it) {
            T* thing=static_cast<T*>(*it);
            if(thing->IsIncludedInThingScans() && filter(*thing)) {
                *out=thing;
                return true;
            }
        }
        return false;
    }
#else
    template<class Filter,class T> bool GetPFirstLeafThing(const Filter&,T**) const;
#endif
protected:
    const std::list<CClass*>& PeekTypeList(long) const;
public:
#endif
    CWorldMap*        WorldMap;                              
    CThingMapManager* PThingMapManager;                      
    CThingManager*    ThingManager;                          
    unsigned char     AreaMaps[0xc];                         
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CThingSearchTools) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CThingSearchTools, WorldMap) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CThingSearchTools, PThingMapManager) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CThingSearchTools, ThingManager) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CThingSearchTools, AreaMaps) == 0xc);

#endif 

#endif

#ifndef FABLE_ENGINE_CTHINGFILTER_NONE_H
#define FABLE_ENGINE_CTHINGFILTER_NONE_H

class CThing;

class CThingFilter_None : public std::unary_function<CThing,bool> {
public: bool operator()(const CThing&) const { return true; }
};
#endif

#ifndef FABLE_ENGINE_CTCSHAPEMANAGER_H
#define FABLE_ENGINE_CTCSHAPEMANAGER_H

#ifndef FABLE_ENGINE_C2DBOXF_H
#define FABLE_ENGINE_C2DBOXF_H

class C2DBoxF { public: float TLX,TLY,BRX,BRY; C2DBoxF() {} };
#endif


class CTCShapeManager : public CTCBase {
public:
#ifdef FABLE_RETAIL_CTHING_CONSTRUCTORS
    CTCShapeManager(CThing&);
#endif
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
    bool IsPosInAnyShape(const C3DVector&) const;
private:
    unsigned char PersistantShapes[12]; 
    unsigned char SelectedShape[4]; 
    bool Activated,NewShapeActivated,LeavingShape;
    C2DBoxF ClosedShapeBounds;
};
FABLE_STATIC_ASSERT(sizeof(CTCShapeManager)==0x30);
#endif

#ifndef FABLE_ENGINE_C2DBOXI_H
#define FABLE_ENGINE_C2DBOXI_H

#ifdef FABLE_RETAIL_VALIDPOSITIONS

#endif
class C2DBoxI { public: long TLX,TLY,BRX,BRY; C2DBoxI() {}
#ifdef FABLE_RETAIL_VALIDPOSITIONS

C2DBoxI(const C2DCoordI& p,long radius){long x=p.X;long y=p.Y;Initialise(x-radius,y-radius,x+radius,y+radius);}
void Initialise(long tlx,long tly,long brx,long bry){TLX=tlx;TLY=tly;BRX=brx;BRY=bry;}
#endif
};
#endif


extern const double TWO_PI;
extern const float DELTA_NOTIONAL_ZERO;
namespace NCreatureHiding {
void __fastcall GetValidPositions(std::vector<C3DVector>& positions,const CWorldMap& wmap,
 const CThingSearchTools& search_tools,const C3DVector& about_position,float radius,
 bool get_hidden_positions,const CTCShapeManager* pExclusionArea)
{
    positions.clear();
    const float d_rad=2.0f<(0.5f>radius*0.125f?0.5f:radius*0.125f)?2.0f:(0.5f>radius*0.125f?0.5f:radius*0.125f);
    std::vector<const CThing*> thing_list;
    search_tools.GetAllThingsInRoughArea<const CThing>(C2DBoxI(about_position.ToC2DCoordI(),(long)radius),CThingFilter_None(),thing_list);
    for(float rad=0.0f;rad<radius+DELTA_NOTIONAL_ZERO;rad+=d_rad) {
        long n_thet=(long)(rad*2.0f+1.0f);
        for(long c1=0;c1<n_thet;++c1) {
            float thet=((float)c1/(float)n_thet)*TWO_PI;
            C3DVector r=C3DVector((float)cos(thet),(float)sin(thet),0.0f)*rad+about_position;
            if(!wmap.IsPosInMap(r)) continue;
            bool got_collision=false;
            std::vector<const CThing*>::iterator it=thing_list.begin();
            while(!got_collision && it!=thing_list.end()) {
                const CThing* pthing=*it++;
                if(pthing && pthing->GetPhysicsTC()->GetPhysicsMeshInfo()) {
                CMeshIntersectionData int_dat;
                if(pthing->GetPhysicsTC()->GetPhysicsMeshObject().GetPointIntersectionFromTo(
                    C3DVector(r.X,r.Y,r.Z+5.0f),C3DVector(r.X,r.Y,r.Z-5.0f),int_dat)) {
                    got_collision=true;
                    r=int_dat.IntersectionPosition;
                    r.Z+=0.1f;
                }
                }
            }
            float ground_height=wmap.GetGroundSizeZAt(r);
            if(!got_collision || ground_height>r.Z) r.Z=ground_height;
            if(pExclusionArea && pExclusionArea->IsPosInAnyShape(r)) continue;
            bool hidable=get_hidden_positions && !wmap.IsPosPassable(r.ToC2DCoordI());
            bool usable=!get_hidden_positions && wmap.IsPosPassable(r.ToC2DCoordI());
            if(hidable&&!got_collision) positions.push_back(r);
            else if(usable) positions.push_back(r);
        }
    }
}
}

#ifndef FABLE_STLPORT_MAP_H
#define FABLE_STLPORT_MAP_H



namespace std {
template<class A,class B,class R> struct binary_function { typedef A first_argument_type;typedef B second_argument_type;typedef R result_type; };
template<class T> struct less:public binary_function<T,T,bool>{bool operator()(const T& a,const T& b)const{return a<b;}};
template<class P> struct _Select1st:public unary_function<P,typename P::first_type>{const typename P::first_type& operator()(const P& p)const{return p.first;}};
typedef bool _Rb_tree_Color_type;
struct _Rb_tree_node_base {
    _Rb_tree_Color_type _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template<class V> struct _Rb_tree_node:public _Rb_tree_node_base { V _M_value_field; };
#ifdef FABLE_RETAIL_QUEST_COMPLETION
template<class Dummy> class _Rb_global {
public:
    static _Rb_tree_node_base* __cdecl _M_increment(_Rb_tree_node_base* node) {
        if(node->_M_right!=0) {
            node=node->_M_right;
            while(node->_M_left!=0) node=node->_M_left;
        } else {
            _Rb_tree_node_base* parent=node->_M_parent;
            while(node==parent->_M_right) {node=parent;parent=parent->_M_parent;}
            if(node->_M_right!=parent) node=parent;
        }
        return node;
    }
};
typedef _Rb_global<bool> _Rb_global_inst;
#endif
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    bool operator==(const _Rb_tree_base_iterator& y)const{return _M_node==y._M_node;}
    bool operator!=(const _Rb_tree_base_iterator& y)const{return _M_node!=y._M_node;}
};
template<class V,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator {
    typedef typename Traits::reference reference;
    typedef typename Traits::pointer pointer;
    _Rb_tree_iterator(){_M_node=0;}
    _Rb_tree_iterator(_Rb_tree_node<V>* p){_M_node=p;}
    _Rb_tree_iterator(const _Rb_tree_iterator<V,_Nonconst_traits<V> >& p){_M_node=p._M_node;}
    reference operator*()const{return static_cast<_Rb_tree_node<V>*>(_M_node)->_M_value_field;}
    pointer operator->()const{return &operator*();}
#ifdef FABLE_RETAIL_QUEST_COMPLETION
    _Rb_tree_iterator& operator++(){this->_M_node=_Rb_global_inst::_M_increment(this->_M_node);return *this;}
#endif
};
template<class V,class A>struct _Rb_tree_base {
protected:
    _STLP_alloc_proxy<_Rb_tree_node<V>*,_Rb_tree_node<V>,allocator<_Rb_tree_node<V> > > _M_header;
};
template<class K,class V,class KeyOfValue,class Compare,class A>
class _Rb_tree:public _Rb_tree_base<V,A> {
protected:
    size_t _M_node_count;
    Compare _M_key_compare;
public:
    typedef _Rb_tree_iterator<V,_Nonconst_traits<V> > iterator;
    typedef _Rb_tree_iterator<V,_Const_traits<V> > const_iterator;
#ifdef FABLE_RETAIL_QUEST_COMPLETION
    iterator begin(){return iterator(static_cast<_Rb_tree_node<V>*>(this->_M_header._M_data->_M_left));}
    const_iterator begin()const{return const_iterator(static_cast<_Rb_tree_node<V>*>(this->_M_header._M_data->_M_left));}
    iterator end(){return iterator(this->_M_header._M_data);}
#endif
    const_iterator find(const K&) const;
    const_iterator end()const{return const_iterator(this->_M_header._M_data);}
};
template<class K,class V,class Compare=less<K>,class A=allocator<pair<const K,V> > >
class map {
public:typedef pair<const K,V> value_type;
private:
    typedef _Rb_tree<K,value_type,_Select1st<value_type>,Compare,A> _Rep_type;
    _Rep_type _M_t;
public:
    typedef typename _Rep_type::const_iterator const_iterator;
#ifdef FABLE_RETAIL_QUEST_COMPLETION
    typedef typename _Rep_type::iterator iterator;
    iterator begin(){return _M_t.begin();}
    const_iterator begin()const{return _M_t.begin();}
    iterator end(){return _M_t.end();}
    V& operator[](const K&);
#endif
    const_iterator find(const K& key)const{return _M_t.find(key);}
    const_iterator end()const{return _M_t.end();}
};
}
FABLE_STATIC_ASSERT(sizeof(std::map<unsigned long,long>)==12);
#endif

#ifndef FABLE_ENGINE_C2DVECTOR_H
#define FABLE_ENGINE_C2DVECTOR_H


#ifdef FABLE_RETAIL_SUBTITLE
extern const float DELTA_NOTIONAL_ZERO;
#endif
#if defined(FABLE_RETAIL_C2DVECTOR_UI) || defined(FABLE_RETAIL_SUBTITLE)


class C2DVector {
public:
    float X,Y;
#ifdef FABLE_RETAIL_SUBTITLE
    float GetSquaredMagnitude() const { return X*X+Y*Y; }
    inline bool IsNonZero() const { return DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO<Y*Y+X*X; }
#endif
    C2DVector() { }
    C2DVector(float x,float y) : X(x),Y(y) { }
    C2DVector(const C2DVector& p) : X(p.X),Y(p.Y) { }
    C2DVector operator+(const C2DVector& vector) const { return C2DVector(X+vector.X,Y+vector.Y); }
    C2DVector operator-(const C2DVector& vector) const { return C2DVector(X-vector.X,Y-vector.Y); }
};
#else
class C2DVector { public: float X,Y; };
#endif
#endif

#ifndef FABLE_ENGINE_CRIGHTHANDEDSET_H
#define FABLE_ENGINE_CRIGHTHANDEDSET_H



class CRightHandedSet { protected: C3DVector Up,Forward; public: const C3DVector& GetForward() const { return Forward; } };
#endif


#include <assert.h>
#include <stdio.h>
#include <string.h>

class CThingManager {};
class CThingPhysicsMeshInfo {};
class CIDrawMap;
class CDrawMapInfo {
public: bool Valid,Loadable,GameLoadable;const CIDrawMap* PMap;CCharString StaticMeshName;C2DCoordI Pos;unsigned long UID;unsigned char Extents[8];
};
namespace std {
template<class T> struct _Identity:unary_function<T,T>{const T& operator()(const T& x)const{return x;}};
template<class K,class Compare,class A> class set { _Rb_tree<K,K,_Identity<K>,Compare,A> _M_t; };
}
void CTCBase::OnSerialise(CPersistContext &,bool) { assert(false); abort(); }
void CTCBase::OnRemove() { assert(false); abort(); }
void CTCBase::ConstructFromParams(const CTCParamsBase &) { assert(false); abort(); }
void CTCBase::OnCreate() { assert(false); abort(); }
void CTCBase::InitialActivate() { assert(false); abort(); }
void CTCBase::OnInitialActivate() { assert(false); abort(); }
void CTCBase::RenderUpdate(float) { assert(false); abort(); }
bool CTCBase::HasFrameUpdate() const { assert(false); abort(); }
void CTCBase::FrameUpdate() { assert(false); abort(); }
void CTCBase::BulletTimeFrameUpdate() { assert(false); abort(); }
void CTCBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo &) { assert(false); abort(); }
void CTCBase::Copy(const CTCBase &) { assert(false); abort(); }
void CTCBase::OnDie() { assert(false); abort(); }
void CTCBase::OnKill() { assert(false); abort(); }
void CTCBase::OnLevelUnload() { assert(false); abort(); }
void CTCBase::OnSetInLimbo(bool) { assert(false); abort(); }
bool CTCBase::Validate(CCharString &) const { assert(false); abort(); }
bool CTCBase::IsFreeForDeletion() const { assert(false); abort(); }
bool CTCBase::IsSerialiseCreatable() const { assert(false); abort(); }
bool CTCBase::IsSerialisableForHeroState() const { assert(false); abort(); }
bool CTCBase::IsComponentSerialisable() const { assert(false); abort(); }
bool CTCBase::IsDriver() const { assert(false); abort(); }
bool CTCBase::IsCopyable() const { assert(false); abort(); }
void CTCBase::GlobalUpdate(bool) { assert(false); abort(); }
void CTCBase::HandleMessage(const NTCMessages::CMessageBase &) { assert(false); abort(); }
ETCInterfaceType CTCPhysicsBase::GetInterfaceType() const { assert(false); abort(); }
C2DVector CTCPhysicsBase::GetOffsetVector() const { assert(false); abort(); }
C3DVector CTCPhysicsBase::GetOffsetVector3D() const { assert(false); abort(); }
void CTCPhysicsBase::SetPosition(const C3DVector &) { assert(false); abort(); }
void CTCPhysicsBase::ResetPosition(const C3DVector &) { assert(false); abort(); }
void CTCPhysicsBase::ResetRHSet(const CRightHandedSet &) { assert(false); abort(); }
void CTCPhysicsBase::SetPositionToNavigateTo(const C3DVector &) { assert(false); abort(); }
void CTCPhysicsBase::SetForcedToMovePos(const C3DVector &,bool) { assert(false); abort(); }
void CTCPhysicsBase::ResetMovementState() { assert(false); abort(); }
void CTCPhysicsBase::EnablePhysics(bool) { assert(false); abort(); }
bool CTCPhysicsBase::IsPhysicsEnabled() const { assert(false); abort(); }
void CTCPhysicsBase::EnableGravity(bool) { assert(false); abort(); }
bool CTCPhysicsBase::IsGravityEnabled() const { assert(false); abort(); }
void CTCPhysicsBase::EnableNavigator(bool) { assert(false); abort(); }
bool CTCPhysicsBase::IsNavigationEnabled() const { assert(false); abort(); }
bool CTCPhysicsBase::HasNavigationLayers() const { assert(false); abort(); }
void CTCPhysicsBase::SetInteractionFlags(long) { assert(false); abort(); }
void CTCPhysicsBase::ReturnToDefaultInteractionFlags() { assert(false); abort(); }
void CTCPhysicsBase::OnDroppedFromUpdateList() { assert(false); abort(); }
void CTCPhysicsBase::OnAddedToUpdateList() { assert(false); abort(); }
bool CTCPhysicsBase::IsAlwaysFacingMovementDirection() const { assert(false); abort(); }
bool CTCPhysicsBase::IsAffectedByGravity() const { assert(false); abort(); }
bool CTCPhysicsBase::IsAffectedByAir() const { assert(false); abort(); }
bool CTCPhysicsBase::IsAffectedByWind() const { assert(false); abort(); }
bool CTCPhysicsBase::IsAngledWithGround() const { assert(false); abort(); }
bool CTCPhysicsBase::IsImmoveable() const { assert(false); abort(); }
bool CTCPhysicsBase::IsOnAnySurface() const { assert(false); abort(); }
bool CTCPhysicsBase::IsOnAnyStationarySurface() const { assert(false); abort(); }
bool CTCPhysicsBase::IsOnMeshSurface() const { assert(false); abort(); }
bool CTCPhysicsBase::IsOnGroundSurface() const { assert(false); abort(); }
bool CTCPhysicsBase::CanCollideWithThings() const { assert(false); abort(); }
bool CTCPhysicsBase::CanCollideWithThingsOneWay() const { assert(false); abort(); }
bool CTCPhysicsBase::CanCollideWithLandscape() const { assert(false); abort(); }
bool CTCPhysicsBase::CanInteractWithThings() const { assert(false); abort(); }
bool CTCPhysicsBase::IsExcludingCreatures() const { assert(false); abort(); }
bool CTCPhysicsBase::IsPhysicsMeshImpassable() const { assert(false); abort(); }
float CTCPhysicsBase::GetFacingAngleXY() const { assert(false); abort(); }
float CTCPhysicsBase::GetFacingAngleYZ() const { assert(false); abort(); }
float CTCPhysicsBase::GetFacingAngleXZ() const { assert(false); abort(); }
void CTCPhysicsBase::SetFacingAngleXY(float) { assert(false); abort(); }
void CTCPhysicsBase::SetFacingAngleYZ(float) { assert(false); abort(); }
void CTCPhysicsBase::SetFacingAngleXZ(float) { assert(false); abort(); }
C3DVector CTCPhysicsBase::GetFacingVector(float) const { assert(false); abort(); }
C3DVector CTCPhysicsBase::GetFacingVector() const { assert(false); abort(); }
const CRightHandedSet & CTCPhysicsBase::GetRHSet() const { assert(false); abort(); }
CRightHandedSet CTCPhysicsBase::GetRHSet(float) const { assert(false); abort(); }
const CRightHandedSet & CTCPhysicsBase::GetLastRHSet() const { assert(false); abort(); }
void CTCPhysicsBase::SetFacingAnglesFromRHSet(const CRightHandedSet &) { assert(false); abort(); }
void CTCPhysicsBase::SetRHSet(const CRightHandedSet &) { assert(false); abort(); }
C3DVector CTCPhysicsBase::GetImpulse() const { assert(false); abort(); }
void CTCPhysicsBase::SetRHSetAsFlushOnNormal(const C3DVector &) { assert(false); abort(); }
void CTCPhysicsBase::SetRHSetAsFlushOnGround() { assert(false); abort(); }
float CTCPhysicsBase::GetRenderZOffset(float) const { assert(false); abort(); }
void CTCPhysicsBase::ResetInterpolation() { assert(false); abort(); }
void CTCPhysicsBase::ApplyControlledImpulse(const C3DVector &) { assert(false); abort(); }
void CTCPhysicsBase::ApplyControlledAcceleration(const C3DVector &,float) { assert(false); abort(); }
bool CTCPhysicsBase::IsPushableByHero() const { assert(false); abort(); }
void CTCPhysicsBase::SetAsPushableByHero(bool) { assert(false); abort(); }
void CTCPhysicsBase::FrameUpdate() { assert(false); abort(); }
bool CTCPhysicsBase::HasFrameUpdate() const { assert(false); abort(); }
void CTCPhysicsBase::SetPhysicsMeshBankIndex(long) { assert(false); abort(); }
void CTCPhysicsBase::SetScaleFromOwner() { assert(false); abort(); }
void CTCPhysicsBase::ConstructFromParams(const CTCParamsBase &) { assert(false); abort(); }
void CTCPhysicsBase::OnCreate() { assert(false); abort(); }
void CTCPhysicsBase::InitialActivate() { assert(false); abort(); }
void CTCPhysicsBase::Move() { assert(false); abort(); }
bool CTCPhysicsBase::DrawGetPhysicsDebugInfo(CThingPhysicalDebugInfo &) const { assert(false); abort(); }
void CTCPhysicsBase::OnSerialise(CPersistContext &,bool) { assert(false); abort(); }
bool CTCPhysicsBase::IsComponentSerialisable() const { assert(false); abort(); }
bool CTCPhysicsBase::IsSerialisableForHeroState() const { assert(false); abort(); }
bool CTCPhysicsBase::IsCopyable() const { assert(false); abort(); }
void CTCPhysicsBase::FillPhysicsDebugInfoForSelf(CPhysicsDebugInfo &) const { assert(false); abort(); }
bool CTCPhysicsBase::HasCollidedWithThingFromTo(const CTCPhysicsBase &,const C3DVector &,const C3DVector &,CPhysicsCollideResponse &) const { assert(false); abort(); }
bool CTCPhysicsBase::HasCollidedWithThingAt(const CTCPhysicsBase &,const C3DVector &,CPhysicsCollideResponse &) const { assert(false); abort(); }
bool CTCPhysicsBase::HasCollidedWithLandscapeAt(const C3DVector &,CPhysicsCollideResponse &) const { assert(false); abort(); }
const CThingPhysicsMeshInfo * CTCPhysicsBase::GetPhysicsMeshInfo() const { assert(false); abort(); }
void CTCPhysicsBase::UpdateInterpolation() { assert(false); abort(); }
void CTCPhysicsBase::UpdateOldRHSet() { assert(false); abort(); }
void CTCPhysicsBase::UpdateOldPos() { assert(false); abort(); }
void CWorldMap::DeleteAllLevels() { assert(false); abort(); }
void CWorldMap::DeleteAllRegions() { assert(false); abort(); }
void CWorldMap::LoadFromFile(CAFile &,long) { assert(false); abort(); }
void CWorldMap::SaveToFile(CAFile &) { assert(false); abort(); }
void CWorldMap::RemoveLevel(long) { assert(false); abort(); }
void CWorldMap::LoadTopologyFromFile(long,CCountedPointer<CAFile>,CASuspendableProcess &) { assert(false); abort(); }
void CWorldMap::PostLoadTopology(long) { assert(false); abort(); }
void CWorldMap::ForceLoad(long) { assert(false); abort(); }
void CWorldMap::ForceUnload(long) { assert(false); abort(); }
bool CWorldMap::IsMapLoaded(long) const { assert(false); abort(); }
bool CWorldMap::IsMapUsed(long) const { assert(false); abort(); }
long CWorldMap::GetGranularity() const { assert(false); abort(); }
const C2DBoxI & CWorldMap::GetDimensions() const { assert(false); abort(); }
CMap & CWorldMap::GetMapAt(const C2DCoordI &) { assert(false); abort(); }
const CMap & CWorldMap::PeekMapAt(const C2DCoordI &) const { assert(false); abort(); }
long CWorldMap::GetMapNumberAt(const C3DVector &) const { assert(false); abort(); }
long CWorldMap::GetMapNumberAt(const C2DCoordI &) const { assert(false); abort(); }
CMap & CWorldMap::GetMap(long) { assert(false); abort(); }
const CMap & CWorldMap::PeekMap(long) const { assert(false); abort(); }
void CWorldMap::GetAllMapsInArea(const C2DBoxI &,std::vector<unsigned long,std::allocator<unsigned long> > &) const { assert(false); abort(); }
void CWorldMap::GetAllLoadedMapsInArea(const C2DBoxI &,std::vector<unsigned long,std::allocator<unsigned long> > &) const { assert(false); abort(); }
void CWorldMap::PostRegionLoad(long) { assert(false); abort(); }
C2DCoordI CWorldMap::GetMapPos(long) const { assert(false); abort(); }
bool CWorldMap::IsMapChangeable(long) const { assert(false); abort(); }
bool CWorldMap::IsPosChangeable(const C2DCoordI &) const { assert(false); abort(); }
std::set<CCharString,std::less<CCharString>,std::allocator<CCharString> > CWorldMap::GetLoadableSections() const { assert(false); abort(); }
void CWorldMap::Update() { assert(false); abort(); }
void CWorldMap::HeightAreaChanged(const C2DBoxI *) { assert(false); abort(); }
long CWorldMap::DrawGetFrame() const { assert(false); abort(); }
long CWorldMap::DrawGetNoMaps() const { assert(false); abort(); }
CDrawMapInfo CWorldMap::DrawGetMapInfo(long) const { assert(false); abort(); }
C2DBoxI CWorldMap::DrawGetDimensions() const { assert(false); abort(); }
long CWorldMap::DrawGetGranularity() const { assert(false); abort(); }
bool CWorldMap::DrawIsPosInMap(const C3DVector &) const { assert(false); abort(); }
float CWorldMap::DrawGetSizeZAt(const C3DVector &,float) const { assert(false); abort(); }
C3DVector CWorldMap::DrawConvertWorldMapCoordsToLocalCoords(long,const C3DVector &) const { assert(false); abort(); }
long CWorldMap::DrawGetMyPlayerMapNumber() const { assert(false); abort(); }
bool CWorldMap::DrawFindLastUnblockedGroundPositionAndReportCollision(const C3DVector &,const C3DVector &,C3DVector &) const { assert(false); abort(); }
bool CWorldMap::DrawFindLastUnblockedThemePositionAndReportCollision(const C3DVector &,const C3DVector &,C3DVector &) const { assert(false); abort(); }
void CWorldMap::LoadScriptBrushesFromFile(long,CCountedPointer<CAFile>) { assert(false); abort(); }
long CWorldMap::AllocateNewMap() { assert(false); abort(); }
const double TWO_PI=6.283185482025146484375;
const float DELTA_NOTIONAL_ZERO=0.0000999999974737875163555145263671875f;
static bool TestInMap,TestPassable,TestExclude,TestMesh,TestHit;
static int TestMapCalls,TestGroundCalls,TestPassCalls,TestExcludeCalls,TestMeshCalls,TestHitCalls,TestSearchCalls,TestCopies;
static float TestGround=8.0f;
static C3DVector TestIntersection(1.0f,2.0f,4.0f);
static const CThing* TestThings[4];static int TestThingCount;
static C2DBoxI TestArea;
CBaseClass::CBaseClass(){} CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){} CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CBaseObject::CBaseObject():IntelligentPointer(0){} CBaseObject::~CBaseObject(){}
void CBaseObject::Validate()const{} void CClass::Save(CCharString&){}
CCharString::CCharString():PStringData(0){} CCharString::~CCharString(){}
CCharString::CCharString(const CCharString& x):PStringData(x.PStringData){}
CCharString::CCharString(const char*,long):PStringData(0){}
CThing::CThing(unsigned long type):UniqueID(0),PhysicsTC(0),PTCDriver(0),PTCGraphicAppearance(0),WorldFrameCreatedAt(0),ThingTypeFlags(type){Type=(unsigned char)type;Dead=0;memset(&UsingComponents,0,sizeof(UsingComponents));}
CTCBase::CTCBase(CThing& parent):ParentThing(parent),ToBeRemoved(false),IsInGlobalUpdateTCs(false){}
CTCBase::~CTCBase(){}
CTCPhysicsBase::CTCPhysicsBase(CThing& parent):CTCBase(parent),Position(1.0f,2.0f,4.0f),OldPosition(0.0f,0.0f,0.0f),Velocity(0.0f,0.0f,0.0f),Radius(8.0f){}
CTCPhysicsBase::~CTCPhysicsBase(){}
CPhysicsMeshObject::CPhysicsMeshObject():Scale(1.0f),OO_Scale(1.0f){memset(Mesh,0,sizeof(Mesh));memset(PositionMatrix,0,sizeof(PositionMatrix));}
CPhysicsMeshObject::~CPhysicsMeshObject(){}
CPhysicsMeshObject CTCPhysicsBase::GetPhysicsMeshObject()const{return CPhysicsMeshObject();}
bool CPhysicsMeshObject::GetPointIntersectionFromTo(const C3DVector& from,const C3DVector& to,CMeshIntersectionData& data)const{
 ++TestHitCalls;assert(from.X==to.X&&from.Y==to.Y);assert(from.Z-to.Z==10.0f);
 if(TestHit){data.IntersectionPosition=TestIntersection;return true;}return false;
}
CTCShapeManager::CTCShapeManager(CThing& t):CTCBase(t){}
ETCInterfaceType CTCShapeManager::GetInterfaceType()const{return TCI_SHAPE_MANAGER;}
CCharString CTCShapeManager::GetClassTypeName()const{return CCharString();}
bool CTCShapeManager::IsPosInAnyShape(const C3DVector&)const{++TestExcludeCalls;return TestExclude;}
CIDrawWorldMap::~CIDrawWorldMap(){}CWorldMap::~CWorldMap(){}
bool CWorldMap::IsPosInMap(const C3DVector&)const{++TestMapCalls;return TestInMap;}
float CWorldMap::GetGroundSizeZAt(const C3DVector&)const{++TestGroundCalls;return TestGround;}
bool CWorldMap::IsPosPassable(const C2DCoordI&)const{++TestPassCalls;return TestPassable;}
CThingSearchTools::CThingSearchTools(CThingManager& manager,CWorldMap& world):WorldMap(world),PThingMapManager(0),ThingManager(manager){}
CThingSearchTools::~CThingSearchTools(){}
namespace std {
template<class Input,class Output,class Distance>
Output __fastcall __copy(Input first,Input last,Output result,const random_access_iterator_tag&,Distance*){
 ++TestCopies;for(;first!=last;++first,++result)*result=*first;return result;
}
template<class T,class A>void _List_base<T,A>::clear(){
 _List_node_base* current=_M_node._M_data->_M_next;
 while(current!=_M_node._M_data){_List_node_base* next=current->_M_next;free(current);current=next;}
 _M_node._M_data->_M_next=_M_node._M_data;_M_node._M_data->_M_prev=_M_node._M_data;
}
}
template<class T,class Container,class Filter>
void CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area,const Filter&,Container& list)const{
 ++TestSearchCalls;TestArea=area;for(int n=0;n<TestThingCount;++n)list.push_back(TestThings[n]);
}
class CTestThing:public CThing{
public:CTestThing():CThing(2){}void SetPhysics(CTCPhysicsBase& physics){PhysicsTC=&physics;}
};
class CTestPhysics:public CTCPhysicsBase{
public:CTestPhysics(CThing& thing):CTCPhysicsBase(thing){}CCharString GetClassTypeName()const{return CCharString();}
 const CThingPhysicsMeshInfo* GetPhysicsMeshInfo()const{static CThingPhysicsMeshInfo info;++TestMeshCalls;return TestMesh?&info:0;}
};
static void reset(){
 TestInMap=TestPassable=true;TestExclude=TestMesh=TestHit=false;TestGround=8.0f;TestThingCount=0;
 TestMapCalls=TestGroundCalls=TestPassCalls=TestExcludeCalls=TestMeshCalls=TestHitCalls=TestSearchCalls=TestCopies=0;
}
int main(){
 CWorldMap world;CThingManager manager;CThingSearchTools search(manager,world);CTestThing thing;CTestPhysics physics(thing);thing.SetPhysics(physics);CTCShapeManager exclusion(thing);
 std::vector<C3DVector> positions;C3DVector centre(4.0f,8.0f,16.0f);
 reset();positions.push_back(C3DVector(32.0f,64.0f,128.0f));
 NCreatureHiding::GetValidPositions(positions,world,search,centre,-1.0f,false,0);
 assert(positions.empty()&&TestMapCalls==0&&TestSearchCalls==1&&TestCopies==1);
 reset();NCreatureHiding::GetValidPositions(positions,world,search,centre,0.0f,false,0);
 assert(positions.size()==1&&positions[0].X==4.0f&&positions[0].Y==8.0f&&positions[0].Z==8.0f&&TestMapCalls==1&&TestGroundCalls==1&&TestPassCalls==1);
 assert(TestArea.TLX==4&&TestArea.TLY==8&&TestArea.BRX==4&&TestArea.BRY==8);
 reset();TestInMap=false;NCreatureHiding::GetValidPositions(positions,world,search,centre,0.0f,false,0);assert(positions.empty()&&TestGroundCalls==0&&TestPassCalls==0);
 reset();TestPassable=false;NCreatureHiding::GetValidPositions(positions,world,search,centre,0.0f,false,0);assert(positions.empty()&&TestPassCalls==1);
 reset();TestPassable=false;NCreatureHiding::GetValidPositions(positions,world,search,centre,0.0f,true,0);assert(positions.size()==1&&TestPassCalls==1);
 reset();NCreatureHiding::GetValidPositions(positions,world,search,centre,0.0f,true,0);assert(positions.empty()&&TestPassCalls==1);
 reset();TestExclude=true;NCreatureHiding::GetValidPositions(positions,world,search,centre,0.0f,false,&exclusion);assert(positions.empty()&&TestExcludeCalls==1&&TestPassCalls==0);
 reset();TestThings[0]=0;TestThings[1]=&thing;TestThingCount=2;NCreatureHiding::GetValidPositions(positions,world,search,centre,0.0f,false,0);assert(positions.size()==1&&TestMeshCalls==1&&TestHitCalls==0);
 reset();TestThings[0]=&thing;TestThingCount=1;TestMesh=true;NCreatureHiding::GetValidPositions(positions,world,search,centre,0.0f,false,0);assert(positions.size()==1&&TestHitCalls==1&&positions[0].Z==8.0f);
 reset();TestThings[0]=&thing;TestThingCount=1;TestMesh=TestHit=true;TestGround=8.0f;NCreatureHiding::GetValidPositions(positions,world,search,centre,0.0f,false,0);assert(positions.size()==1&&positions[0].X==1.0f&&positions[0].Y==2.0f&&positions[0].Z==8.0f);
 reset();TestThings[0]=&thing;TestThingCount=1;TestMesh=TestHit=true;TestGround=2.0f;NCreatureHiding::GetValidPositions(positions,world,search,centre,0.0f,false,0);assert(positions.size()==1&&positions[0].Z>4.0f&&positions[0].Z<4.125f);
 reset();TestThings[0]=&thing;TestThingCount=1;TestMesh=TestHit=true;TestPassable=false;NCreatureHiding::GetValidPositions(positions,world,search,centre,0.0f,true,0);assert(positions.empty()&&TestHitCalls==1);
 reset();NCreatureHiding::GetValidPositions(positions,world,search,centre,0.5f,false,0);assert(positions.size()==3&&TestMapCalls==3&&TestGroundCalls==3&&TestPassCalls==3);
 reset();NCreatureHiding::GetValidPositions(positions,world,search,centre,8.0f,false,0);assert(positions.size()==81&&TestMapCalls==81&&TestPassCalls==81);
 reset();NCreatureHiding::GetValidPositions(positions,world,search,centre,32.0f,false,0);assert(positions.size()==561&&TestMapCalls==561&&TestPassCalls==561);
 puts("0074a5d0_TEST PASS");return 0;
}