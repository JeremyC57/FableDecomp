#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __declspec(x)
#endif
#define FABLE_STATIC_ASSERT(x)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CONSTRUCTORS
#define FABLE_RETAIL_CWIDESTRING_CTOR
#ifndef FABLE_ENGINE_CDEFSTRING_H
#define FABLE_ENGINE_CDEFSTRING_H


class CCharString;
class CDefString {
public:
 CDefString():TablePos(-1){}
 CDefString(const CCharString&);
private:
 long TablePos;
};
#endif



#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPERSISTCONTEXT
#ifndef FABLE_ENGINE_NUISYSTEM_CUIMISCTHINGSDEF_H
#define FABLE_ENGINE_NUISYSTEM_CUIMISCTHINGSDEF_H


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

#ifndef FABLE_ENGINE_CPARENTDEFCLASSBASE_H
#define FABLE_ENGINE_CPARENTDEFCLASSBASE_H

























#ifdef FABLE_RETAIL_DEF_CLASS_BASE
#include <stddef.h>

#ifndef FABLE_ENGINE_CDEFCLASSBASE_H
#define FABLE_ENGINE_CDEFCLASSBASE_H























#ifdef FABLE_RETAIL_DEF_CLASS_BASE
#include <stddef.h>

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
#else 
#include <stddef.h>


#pragma pack(push, 1)
class CIVCountedPointeeBase {
public:
    void*         __vftable;                                 
    unsigned long IVRefCount;                                
};
#pragma pack(pop)


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


#endif 

#endif 


class CDefPointeeBase : public CResource {
public:
    virtual ~CDefPointeeBase() { }                                       
};

#endif 

class CCharString;
class CDefString;
class CDefinitionManager;
class CPersistContext;
class CDataInputStream;
class CDataOutputStream;
class CValidateDefinitionInitBase;
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class C> class CVectorMap;
template <class T> class CDefPointer;
#pragma pack(push, 1)
class CDefClassBase : public CDefPointeeBase {
#ifdef FABLE_RETAIL_DEF_CONSTRUCTORS
private:
    CDefClassBase(); 
    friend class CParentDefClassBase;
#endif
public:
    class CSubDefInfo {                                                  
    public:
        long DefIndex;                                                   
        long OriginalParentDefIndex;                                     
    };
    CDefClassBase(const CDefClassBase&);                                 
    CDefClassBase& operator=(const CDefClassBase&);                      
    template <class T> bool GetSubDef(CDefPointer<const T>*) const;      
    bool AreDefaultValsApplied() const { return DefaultValsApplied; }    
    void SetDefaultValsApplied(bool value) { DefaultValsApplied = value; }   
    virtual ~CDefClassBase() { }                                         
    virtual bool UnloadResource();                                       
    virtual CDefString GetInstantiationName() const;                     
    virtual void SetInstantiationName(CDefString);                       
    virtual void AddSubDef(const CCharString&, long, long);              
    virtual void RemoveSubDef(const CCharString&);                       
    virtual CSubDefInfo* GetPSubDefInfo(const CCharString&);             
    virtual const CSubDefInfo* GetPSubDefInfo(const CCharString&) const; 
    virtual bool IsParentDef() const;                                    
    virtual void PostTransfer();                                         
    virtual void Transfer(CPersistContext&) = 0;                         
    virtual void Copy(const CDefClassBase*) = 0;                         
    virtual unsigned long GetSizeofClass() = 0;                          
    virtual void InternalSaveBinary(CDataOutputStream&);                 
    virtual void InternalLoadBinary(CDataInputStream&);                  
    virtual void Validate(const CValidateDefinitionInitBase&, const CCharString&) const;  
    virtual CVectorMap<unsigned long, CSubDefInfo, CKeyPairCompareLess<unsigned long, CSubDefInfo> >*
        GetPSubDefInfoMap();                                             
    CDefinitionManager* PDefManager;                                     
    unsigned long       GlobalIndex;                                     
    unsigned char       Setup : 1;                                       
    unsigned char       Template : 1;
    unsigned char       DefaultValsApplied : 1;
};
#pragma pack(pop)
#else 
#include <stddef.h>


class CDefinitionManager;

#pragma pack(push, 1)
class CDefClassBase {
public:
    void*               __vftable;                           
    unsigned char       _base_0x04[0x18];                    
    CDefinitionManager* PDefManager;                         
    unsigned long       GlobalIndex;                         
    unsigned char       DefaultValsApplied[0x1];             
};
#pragma pack(pop)


#endif 

#endif 

class CDataInputStream;
class CDataOutputStream;
#pragma pack(push, 1)
class CParentDefClassBase : public CDefClassBase {
public:
    CParentDefClassBase();                                               
    CParentDefClassBase(const CParentDefClassBase&);                     
    CParentDefClassBase& operator=(const CParentDefClassBase&);          
    virtual ~CParentDefClassBase();                                      
    virtual CDefString GetInstantiationName() const;                     
    virtual void SetInstantiationName(CDefString);                       
    virtual void AddSubDef(const CCharString&, long, long);              
    virtual void RemoveSubDef(const CCharString&);                       
    virtual CSubDefInfo* GetPSubDefInfo(const CCharString&);             
    virtual const CSubDefInfo* GetPSubDefInfo(const CCharString&) const; 
    virtual bool IsParentDef() const;                                    
    virtual void InternalSaveBinary(CDataOutputStream&);                 
    virtual void InternalLoadBinary(CDataInputStream&);                  
    virtual CVectorMap<unsigned long, CSubDefInfo, CKeyPairCompareLess<unsigned long, CSubDefInfo> >*
        GetPSubDefInfoMap();                                             
#ifdef FABLE_PDCB_TYPED_MEMBERS
    CDefString InstantiationName;                                        
    CVectorMap<unsigned long, CSubDefInfo, CKeyPairCompareLess<unsigned long, CSubDefInfo> > SubDefInfoMap;   
#else
    long          InstantiationName_TablePos;                            
    unsigned char SubDefInfoMap[0x10];                                   
#endif
};
#pragma pack(pop)
#else 
#include <stddef.h>


#pragma pack(push, 1)
class CParentDefClassBase {
public:
    void*         __vftable;                                 
    unsigned char _base_0x04[0x21];                          
    long          InstantiationName_TablePos;                
    unsigned char SubDefInfoMap[0x10];                       
};
#pragma pack(pop)


#endif 

#endif 

#ifndef FABLE_RETAIL_HEADER_CWIDESTRING
#define FABLE_RETAIL_HEADER_CWIDESTRING

class CCharString;
class CWideStringData;
class CWideString {
public:
#if defined(FABLE_RETAIL_CWIDESTRING_CTOR)
    CWideString();
#endif
#ifdef FABLE_RETAIL_CWIDESTRING_COPY
    CWideString(const CWideString&);
#endif
#ifdef FABLE_RETAIL_SCOREBOARD
    CCharString ToCharString() const;
#endif
    ~CWideString();
protected:
    CWideStringData* PStringData;
};
#endif

#ifndef FABLE_ENGINE_CCHARSTRING_H
#define FABLE_ENGINE_CCHARSTRING_H


#ifdef FABLE_RETAIL_SCOREBOARD
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


#endif

#endif
class CCharStringData;
class CCharString {
public:
    CCharString();
    CCharString(const char* text, long length = -1);
    CCharString(const CCharString& other);
#ifdef FABLE_RETAIL_SCOREBOARD
    const char* GetAsCharArray() const { const char* result=&NullChar; if(PStringData) result=PStringData->Data.PeekData(); return result; }
#endif
    ~CCharString();
protected:
#ifdef FABLE_RETAIL_SCOREBOARD
    static const char NullChar;
#endif
    CCharStringData* PStringData;
};
#endif

#ifndef FABLE_ENGINE_C2DVECTOR_H
#define FABLE_ENGINE_C2DVECTOR_H


#ifdef FABLE_RETAIL_C2DVECTOR_UI


class C2DVector {
public:
    float X,Y;
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

#ifndef FABLE_ENGINE_EMUSICSETTYPE_H
#define FABLE_ENGINE_EMUSICSETTYPE_H

enum EMusicSetType {
    MUSIC_SET_NULL=1,
    MUSIC_SET_FRESCO_WEDDING=2,
    MUSIC_SET_CHAPEL_OF_EVIL=3,
    MUSIC_SET_GUILD=4,
    MUSIC_SET_GUILD_DAY=5,
    MUSIC_SET_GUILD_NIGHT=6,
    MUSIC_SET_HALL_OF_HEROES=7,
    MUSIC_SET_TEMPLE_OF_LIGHT=8,
    MUSIC_SET_ARENA_FANFARE=9,
    MUSIC_SET_BANDIT_CAMP=10,
    MUSIC_SET_BOWERSTONE=11,
    MUSIC_SET_CAVES=12,
    MUSIC_SET_DARKWOOD=13,
    MUSIC_SET_GRAVEYARD=14,
    MUSIC_SET_GRAVEYARD_PASSAGE=15,
    MUSIC_SET_GREATWOOD=16,
    MUSIC_SET_HAUNTED_HOUSE=17,
    MUSIC_SET_LOOKOUT_POINT=18,
    MUSIC_SET_OAKVALE=19,
    MUSIC_SET_WITCHWOOD=20,
    MUSIC_SET_QUEST_SUCCEEDED=21,
    MUSIC_SET_QUEST_FAILED=22,
    MUSIC_SET_BOSS=23,
    MUSIC_SET_DANGER=24,
    MUSIC_SET_PRISON=25,
    MUSIC_SET_HOOK_COAST=26,
    MUSIC_SET_KNOTHOLE_GLADE=27,
    MUSIC_SET_EXECUTION_TREE=28,
    MUSIC_SET_GIBBET_WOODS=29,
    MUSIC_SET_KRAKEN_CHAMBER=30,
    MUSIC_SET_INTERLUDE=31,
    MUSIC_SET_DANGER_ONLY=32,
    MUSIC_SET_DRAGON=33,
    MUSIC_SET_ARENA_FIGHT=34,
    MUSIC_SET_ARENA_FANFARE_01=35,
    MUSIC_SET_ARENA_FANFARE_02=36,
    MUSIC_SET_ARENA_FANFARE_03=37,
    MUSIC_SET_ARENA_FANFARE_04=38,
    MUSIC_SET_ARENA_FANFARE_05=39,
    MUSIC_SET_ARENA_FANFARE_06=40,
    MUSIC_SET_ARENA_FANFARE_07=41,
    MUSIC_SET_ARENA_FANFARE_08=42,
    MUSIC_SET_ARENA_FANFARE_09=43,
    MUSIC_SET_ARENA_FANFARE_10=44,
    MUSIC_SET_CUTSCENE_DEAD_DAD=45,
    MUSIC_SET_CUTSCENE_FEET=46,
    MUSIC_SET_CUTSCENE_GUILD_CEREMONY=47,
    MUSIC_SET_CUTSCENE_TWINBLADE=48,
    MUSIC_SET_CUTSCENE_THERESA_01=49,
    MUSIC_SET_CUTSCENE_THERESA_02=50,
    MUSIC_SET_CUTSCENE_WIZARD_BATTLE_INTRO=51,
    MUSIC_SET_CUTSCENE_WIZARD_BATTLE_OUTRO=52,
    MUSIC_SET_CUTSCENE_PRISON_MOTHER=53,
    MUSIC_SET_CUTSCENE_JACK_BOSS_FIGHT_INTRO=54,
    MUSIC_SET_CUTSCENE_JACK_BOSS_FIGHT_OUTRO=55,
    MUSIC_SET_CUTSCENE_JACK_CAPTURES=56,
    MUSIC_SET_CUTSCENE_GUILD_ARRIVAL=57,
    MUSIC_SET_CUTSCENE_COLLECT_FIREHEART=58,
    MUSIC_SET_CUTSCENE_DRAGON_DEATH=59,
    MUSIC_SET_CUTSCENE_DRAGON_FIGHT_INTRO=60,
    MUSIC_SET_CUTSCENE_DRAGON_FIGHT_OUTRO_EVIL=61,
    MUSIC_SET_CUTSCENE_DRAGON_FIGHT_OUTRO_GOOD=62,
    MUSIC_SET_CUTSCENE_DRAGON_FIGHT_OUTRO_CHOICE=63,
    MUSIC_SET_CUTSCENE_ONE_YEAR_LATER=64,
    MUSIC_SET_CUTSCENE_ORACLE_AWAKENS=65,
    MUSIC_SET_CUTSCENE_SCYTHE_MESSAGE=66,
    MUSIC_SET_CUTSCENE_SOUL2_MOTHER_INTRO=67,
    MUSIC_SET_CUTSCENE_SOUL2_MOTHER_SUCCESS=68,
    MUSIC_SET_CUTSCENE_SOUL3_GUILDMASTER_INTRO=69,
    MUSIC_SET_CUTSCENE_SUMMON_SHIP_INTRO=70,
    MUSIC_SET_CUTSCENE_SUMMON_SHIP_OUTRO=71,
    MUSIC_SET_CUTSCENE_SOUL3_GUILDMASTER_OUTRO_EVIL=72,
    MUSIC_SET_CUTSCENE_SOUL3_GUILDMASTER_OUTRO_GOOD=73,
    MUSIC_SET_CUTSCENE_GATE_OPENS_BRIAR_ROSE=74,
    MUSIC_SET_CUTSCENE_GATE_OPENS_SCYTHE=75,
    MUSIC_SET_INTRO=76,
    NO_MUSIC_SETS=77
};
#endif

namespace NUISystem {


class CUIMiscThingsDef : public CParentDefClassBase {
public:
    CUIMiscThingsDef();
    CUIMiscThingsDef(const CUIMiscThingsDef&);
    virtual ~CUIMiscThingsDef();
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    CWideString SpaceSeparator; 
    CWideString CommaSeparator; 
    CWideString ColonSeparator; 
    CWideString NewLineSeparator; 
    CWideString OpenBracket; 
    CWideString CloseBracket; 
    CWideString Positive; 
    CWideString WeaponValueString; 
    CWideString WeaponAugString; 
    CWideString WeaponAugNone; 
    CWideString WeaponWeightString; 
    CWideString WeaponLightString; 
    CWideString WeaponHeavyString; 
    CWideString WeaponKillsString; 
    CWideString WeaponCatMeleeString; 
    CWideString WeaponCatRangedString; 
    CWideString WeaponDamageString; 
    CWideString TradeCostString; 
    CWideString TradeProfitString; 
    CWideString TradeLossString; 
    CWideString TradeAlreadyOwnsString; 
    CWideString TradeNumberInStockString; 
    CWideString TradeDeliveryString; 
    CWideString TradeNoDeliveryString; 
    CWideString TradeDaysString; 
    CWideString TradeBuyString; 
    CWideString TradeSellString; 
    CWideString TradeWantedString; 
    CWideString QuestFailedString; 
    CWideString FailedString; 
    CWideString SucceededString; 
    CWideString Plus; 
    CWideString Minus; 
    unsigned long CoreGraphic; 
    unsigned long VignetteGraphic; 
    unsigned long OptionalGraphic; 
    unsigned long FeatGraphic; 
    CWideString ObjectsRewardString; 
    CWideString NoneString; 
    CWideString CheckGuildString; 
    CWideString QuestStartingString; 
    C2DVector RingCenter; 
    C2DVector PCRingCenter; 
    C2DVector WorldMapOffset; 
    float WorldMapWidth; 
    float WorldMapHeight; 
    CWideString YouString; 
    CWideString OwnString; 
    CWideString NoString; 
    CWideString HousesString; 
    CWideString HouseString; 
    CWideString InString; 
    CWideString ShopsString; 
    CWideString ShopString; 
    CWideString ThereString; 
    CWideString AreString; 
    CWideString IsString; 
    CWideString ForString; 
    CWideString SaleString; 
    CWideString GeneralString; 
    CWideString TitleString; 
    CWideString BarberString; 
    CWideString TatooString; 
    CWideString LevelString; 
    unsigned long TotalSpellsInPalettes; 
    unsigned long TotalSpellsInContainer; 
    CWideString LogBookBasicsCategoryString; 
    CWideString LogBookObjectsCategoryString; 
    CWideString LogBookTownsCategoryString; 
    CWideString LogBookHeroCategoryString; 
    CWideString LogBookCombatCategoryString; 
    CWideString LogBookQuestCategoryString; 
    CWideString LogBookStoryCategoryString; 
    CWideString LogBookBasicsCategoryNameString; 
    CWideString LogBookObjectsCategoryNameString; 
    CWideString LogBookTownsCategoryNameString; 
    CWideString LogBookHeroCategoryNameString; 
    CWideString LogBookCombatCategoryNameString; 
    CWideString LogBookQuestCategoryNameString; 
    CWideString LogBookStoryCategoryNameString; 
    std::map<unsigned long,unsigned long,std::less<unsigned long>,std::allocator<std::pair<unsigned long const ,unsigned long> > > MapPaths; 
    CCharString SoundUpDown; 
    CCharString SoundSlider; 
    CCharString SoundBack; 
    CCharString SoundForward; 
    CCharString SoundError; 
    CCharString SoundExit; 
    CCharString CountUpSound; 
    CCharString SoundKeyboardUp; 
    CCharString SoundKeyboardDown; 
    CCharString SoundKeyboardLeft; 
    CCharString SoundKeyboardRight; 
    CCharString SoundKeyboardEnterCharacter; 
    CCharString SoundKeyboardDeleteCharacter; 
    CCharString SoundKeyboardDone; 
    CWideString FrontEndMusic; 
    C2DVector HeroDollTL; 
    C2DVector HeroDollBR; 
    float HeroDollSphereRadius; 
    C2DVector HeroDollTL_PC; 
    C2DVector HeroDollBR_PC; 
    float HeroDollSphereRadius_PC; 
    C2DVector HeroDollFrameTL_PC; 
    float HeroDollFrameEmulateListOffset; 
    EMusicSetType QuestStartScreenMusic; 
    EMusicSetType QuestCompleteScreenMusic; 
    EMusicSetType QuestFailureScreenMusic; 
    EMusicSetType DeathScreenMusic; 
    float DigitCountTime; 
    unsigned long SaveHeroGraphicIndex; 
    std::map<CCharString,long,std::less<CCharString>,std::allocator<std::pair<CCharString const ,long> > > MiniMapGraphics; 
    long KeyboardSmallKeyGraphic; 
    long KeyboardLargeKeyGraphic; 
    unsigned long TotalAssignableThings; 
    float TimeInSecsForFade; 
    float BackBufferFilterSaturation; 
    float BackBufferFilterContrast; 
    float BackBufferFilterBrightness; 
    float BackBufferFilterTintR; 
    float BackBufferFilterTintG; 
    float BackBufferFilterTintB; 
    float BackBufferFilterTintScale; 
    float BackBufferDiffuseScale; 
    float BackBufferAmbientScale; 
    float MinimumFilterColor; 
};
}
#endif

#ifdef FABLE_RETAIL_CPERSISTCONTEXT
#ifndef FABLE_ENGINE_CPERSISTCONTEXT_H
#define FABLE_ENGINE_CPERSISTCONTEXT_H

class CDefinitionManager;
class CSymbolMap;
class CStringParser;
class CCharString;
class CMemoryDataInputStream;
class CDataOutputStream;
namespace std { template<class T> class allocator; template<class A,class B> struct pair; template<class T> struct less; template<class K,class V,class C,class A> class map; template<class T,class A> class vector; }


class CPersistContext {
public:
    enum EMode { MODE_LOAD_TEXT=0,MODE_SAVE_TEXT=1,MODE_LOAD_BINARY=2,MODE_SAVE_BINARY=3,MODE_CLEAR=4 };
    template<class T> void Transfer(T&);
    template<class T> void TransferVector(std::vector<T,std::allocator<T> >&);
    template<class V,class K> void TransferMap(std::map<K,V,std::less<K>,std::allocator<std::pair<const K,V> > >&);
    template<class T> void TransferVectorOfSubComponents(std::vector<T,std::allocator<T> >&);
protected:
    const CDefinitionManager* PDefinitionManager;
    const CSymbolMap* PSymbolMap;
    CStringParser* PParser;
    void (__fastcall *ErrorCallback)(const CCharString&);
    CCharString* PStringToClean;
    bool UseDefaults,UseCRCCheck,SingleTransfer,Finished;
    EMode Mode;
    bool Binary;
    CCharString* PStringToSaveTo;
    CMemoryDataInputStream* PLoadStream;
    CDataOutputStream* PSaveStream;
};
#endif

#else
#pragma once



class CPersistContext
{
public:
    template <class T>
    void Transfer(const char* name, T& value, const T& defaultValue);

    void TransferSignedChar(
        const char* name,
        signed char& value,
        const signed char& defaultValue);
};

#endif

template<> void CPersistContext::Transfer<CWideString>(CWideString&);
template<> void CPersistContext::Transfer<CCharString>(CCharString&);
template<> void CPersistContext::Transfer<float>(float&);
template<> void CPersistContext::Transfer<long>(long&);
template<> void CPersistContext::Transfer<unsigned long>(unsigned long&);
template<> void CPersistContext::Transfer<EMusicSetType>(EMusicSetType&);
template<> void CPersistContext::TransferMap<unsigned long,unsigned long>(std::map<unsigned long,unsigned long>&);
template<> void CPersistContext::TransferMap<long,CCharString>(std::map<CCharString,long>&);
namespace NUISystem {
void CUIMiscThingsDef::Transfer(CPersistContext& persist)
{
    persist.Transfer(SpaceSeparator);
    persist.Transfer(CommaSeparator);
    persist.Transfer(NewLineSeparator);
    persist.Transfer(OpenBracket);
    persist.Transfer(CloseBracket);
    persist.Transfer(Positive);
    persist.Transfer(WeaponValueString);
    persist.Transfer(WeaponAugString);
    persist.Transfer(WeaponAugNone);
    persist.Transfer(WeaponWeightString);
    persist.Transfer(WeaponLightString);
    persist.Transfer(WeaponHeavyString);
    persist.Transfer(WeaponKillsString);
    persist.Transfer(WeaponCatMeleeString);
    persist.Transfer(WeaponCatRangedString);
    persist.Transfer(WeaponDamageString);
    persist.Transfer(TradeCostString);
    persist.Transfer(ColonSeparator);
    persist.Transfer(TradeProfitString);
    persist.Transfer(TradeLossString);
    persist.Transfer(TradeAlreadyOwnsString);
    persist.Transfer(TradeNumberInStockString);
    persist.Transfer(TradeDeliveryString);
    persist.Transfer(TradeNoDeliveryString);
    persist.Transfer(TradeDaysString);
    persist.Transfer(TradeBuyString);
    persist.Transfer(TradeSellString);
    persist.Transfer(TradeWantedString);
    persist.Transfer(QuestFailedString);
    persist.Transfer(FailedString);
    persist.Transfer(SucceededString);
    persist.Transfer(Plus);
    persist.Transfer(Minus);
    persist.Transfer(CoreGraphic);
    persist.Transfer(VignetteGraphic);
    persist.Transfer(OptionalGraphic);
    persist.Transfer(FeatGraphic);
    persist.Transfer(ObjectsRewardString);
    persist.Transfer(NoneString);
    persist.Transfer(CheckGuildString);
    persist.Transfer(QuestStartingString);
    persist.Transfer(RingCenter.X);
    persist.Transfer(RingCenter.Y);
    persist.Transfer(PCRingCenter.X);
    persist.Transfer(PCRingCenter.Y);
    persist.Transfer(WorldMapOffset.X);
    persist.Transfer(WorldMapOffset.Y);
    persist.Transfer(WorldMapWidth);
    persist.Transfer(WorldMapHeight);
    persist.Transfer(YouString);
    persist.Transfer(OwnString);
    persist.Transfer(NoString);
    persist.Transfer(HousesString);
    persist.Transfer(HouseString);
    persist.Transfer(InString);
    persist.Transfer(ShopsString);
    persist.Transfer(ShopString);
    persist.Transfer(ThereString);
    persist.Transfer(AreString);
    persist.Transfer(IsString);
    persist.Transfer(ForString);
    persist.Transfer(SaleString);
    persist.Transfer(GeneralString);
    persist.Transfer(TatooString);
    persist.Transfer(BarberString);
    persist.Transfer(TitleString);
    persist.Transfer(LevelString);
    persist.Transfer(TotalSpellsInPalettes);
    persist.Transfer(TotalSpellsInContainer);
    persist.Transfer(TotalAssignableThings);
    persist.Transfer(LogBookBasicsCategoryString);
    persist.Transfer(LogBookObjectsCategoryString);
    persist.Transfer(LogBookTownsCategoryString);
    persist.Transfer(LogBookHeroCategoryString);
    persist.Transfer(LogBookCombatCategoryString);
    persist.Transfer(LogBookQuestCategoryString);
    persist.Transfer(LogBookStoryCategoryString);
    persist.Transfer(LogBookBasicsCategoryNameString);
    persist.Transfer(LogBookObjectsCategoryNameString);
    persist.Transfer(LogBookTownsCategoryNameString);
    persist.Transfer(LogBookHeroCategoryNameString);
    persist.Transfer(LogBookCombatCategoryNameString);
    persist.Transfer(LogBookQuestCategoryNameString);
    persist.Transfer(LogBookStoryCategoryNameString);
    persist.TransferMap<unsigned long,unsigned long>(MapPaths);
    persist.Transfer(SoundUpDown);
    persist.Transfer(SoundSlider);
    persist.Transfer(SoundBack);
    persist.Transfer(SoundForward);
    persist.Transfer(SoundError);
    persist.Transfer(SoundExit);
    persist.Transfer(HeroDollTL.X);
    persist.Transfer(HeroDollTL.Y);
    persist.Transfer(HeroDollBR.X);
    persist.Transfer(HeroDollBR.Y);
    persist.Transfer(HeroDollSphereRadius);
    persist.Transfer(HeroDollTL_PC.X);
    persist.Transfer(HeroDollTL_PC.Y);
    persist.Transfer(HeroDollBR_PC.X);
    persist.Transfer(HeroDollBR_PC.Y);
    persist.Transfer(HeroDollSphereRadius_PC);
    persist.Transfer(HeroDollFrameTL_PC.X);
    persist.Transfer(HeroDollFrameTL_PC.Y);
    persist.Transfer(HeroDollFrameEmulateListOffset);
    persist.Transfer(QuestStartScreenMusic);
    persist.Transfer(QuestCompleteScreenMusic);
    persist.Transfer(QuestFailureScreenMusic);
    persist.Transfer(DeathScreenMusic);
    persist.Transfer(CountUpSound);
    persist.Transfer(DigitCountTime);
    persist.Transfer(SaveHeroGraphicIndex);
    persist.TransferMap<long,CCharString>(MiniMapGraphics);
    persist.Transfer(SoundKeyboardUp);
    persist.Transfer(SoundKeyboardDown);
    persist.Transfer(SoundKeyboardLeft);
    persist.Transfer(SoundKeyboardRight);
    persist.Transfer(SoundKeyboardEnterCharacter);
    persist.Transfer(SoundKeyboardDeleteCharacter);
    persist.Transfer(SoundKeyboardDone);
    persist.Transfer(FrontEndMusic);
    persist.Transfer(KeyboardSmallKeyGraphic);
    persist.Transfer(KeyboardLargeKeyGraphic);
    persist.Transfer(TimeInSecsForFade);
    persist.Transfer(BackBufferFilterSaturation);
    persist.Transfer(BackBufferFilterContrast);
    persist.Transfer(BackBufferFilterBrightness);
    persist.Transfer(BackBufferFilterTintR);
    persist.Transfer(BackBufferFilterTintG);
    persist.Transfer(BackBufferFilterTintB);
    persist.Transfer(BackBufferFilterTintScale);
    persist.Transfer(BackBufferDiffuseScale);
    persist.Transfer(BackBufferAmbientScale);
    persist.Transfer(MinimumFilterColor);
}
}

void CIVCountedPointeeBase::Delete() const { throw 1; }
CResource::CResource() : ResourceList(0),PrevResource(0),NextResource(0),ResourceSize(0),LastUsedFrame(0) { IVRefCount=0; }
CResource::~CResource() { }
bool CResource::UnloadResource(){throw 1;}
unsigned long CResource::GetUnloadableMemory()const{throw 1;}
bool CResource::IsResourceLoaded()const{throw 1;}
void CResource::OnResourceLoaded(){throw 1;}
bool CResource::CanAbortResourceLoad(long)const{throw 1;}
void CResource::OnResourceLoadAborted(){throw 1;}
unsigned long CResource::GetDebugMemoryInUse()const{throw 1;}
void CResource::ResetDebugMemoryInUse(){throw 1;}
CDefClassBase::CDefClassBase():PDefManager(0),GlobalIndex(0),Setup(0),Template(0),DefaultValsApplied(0){}
bool CDefClassBase::UnloadResource(){throw 1;}
CDefString CDefClassBase::GetInstantiationName()const{throw 1;}
void CDefClassBase::SetInstantiationName(CDefString){throw 1;}
void CDefClassBase::AddSubDef(const CCharString&,long,long){throw 1;}
void CDefClassBase::RemoveSubDef(const CCharString&){throw 1;}
CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&){throw 1;}
const CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&)const{throw 1;}
bool CDefClassBase::IsParentDef()const{throw 1;}
void CDefClassBase::PostTransfer(){throw 1;}
void CDefClassBase::InternalSaveBinary(CDataOutputStream&){throw 1;}
void CDefClassBase::InternalLoadBinary(CDataInputStream&){throw 1;}
void CDefClassBase::Validate(const CValidateDefinitionInitBase&,const CCharString&)const{throw 1;}
CVectorMap<unsigned long,CDefClassBase::CSubDefInfo,CKeyPairCompareLess<unsigned long,CDefClassBase::CSubDefInfo> >* CDefClassBase::GetPSubDefInfoMap(){throw 1;}
CParentDefClassBase::CParentDefClassBase(){InstantiationName_TablePos=-1;memset(SubDefInfoMap,0,sizeof(SubDefInfoMap));}
CParentDefClassBase::~CParentDefClassBase(){}
CDefString CParentDefClassBase::GetInstantiationName()const{throw 1;}
void CParentDefClassBase::SetInstantiationName(CDefString){throw 1;}
void CParentDefClassBase::AddSubDef(const CCharString&,long,long){throw 1;}
void CParentDefClassBase::RemoveSubDef(const CCharString&){throw 1;}
CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&){throw 1;}
const CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&)const{throw 1;}
bool CParentDefClassBase::IsParentDef()const{throw 1;}
void CParentDefClassBase::InternalSaveBinary(CDataOutputStream&){throw 1;}
void CParentDefClassBase::InternalLoadBinary(CDataInputStream&){throw 1;}
CVectorMap<unsigned long,CDefClassBase::CSubDefInfo,CKeyPairCompareLess<unsigned long,CDefClassBase::CSubDefInfo> >* CParentDefClassBase::GetPSubDefInfoMap(){throw 1;}
CCharString::CCharString():PStringData(0){}
CCharString::~CCharString(){}
CWideString::CWideString():PStringData(0){}
CWideString::~CWideString(){}
NUISystem::CUIMiscThingsDef::CUIMiscThingsDef(){}
NUISystem::CUIMiscThingsDef::~CUIMiscThingsDef(){}
void NUISystem::CUIMiscThingsDef::Copy(const CDefClassBase*){throw 1;}
unsigned long NUISystem::CUIMiscThingsDef::GetSizeofClass(){return sizeof(*this);}
struct TestField { void* address;unsigned int type; };
static TestField expected[133];
static unsigned int visits=0,seed=0;
static bool loading=false;
static CPersistContext* expectedContext=0;
static int textTokens[1024];
static void fail(int n){fprintf(stderr,"misc transfer check %d at %u\n",n,visits);exit(n);}
static void store(TestField f,unsigned int index,unsigned int valueSeed){
 unsigned int value=(index*7+valueSeed*11)%511;
 switch(f.type){
 case 0:case 1:{void* ptr=textTokens+value;memcpy(f.address,&ptr,sizeof(ptr));break;}
 case 2:*(float*)f.address=((int)value-255)*0.5f;break;
 case 3:*(long*)f.address=(long)value-255;break;
 case 4:*(unsigned long*)f.address=value;break;
 case 5:*(EMusicSetType*)f.address=(EMusicSetType)(1+value%40);break;
 }
}
static void check(TestField f,unsigned int index,unsigned int valueSeed){
 unsigned int value=(index*7+valueSeed*11)%511;
 switch(f.type){
 case 0:case 1:{void* ptr=0;memcpy(&ptr,f.address,sizeof(ptr));if(ptr!=textTokens+value)fail(2);break;}
 case 2:if(*(float*)f.address!=((int)value-255)*0.5f)fail(3);break;
 case 3:if(*(long*)f.address!=(long)value-255)fail(4);break;
 case 4:if(*(unsigned long*)f.address!=value)fail(5);break;
 case 5:if(*(EMusicSetType*)f.address!=(EMusicSetType)(1+value%40))fail(6);break;
 }
}
static void visit(CPersistContext* context,void* address,unsigned int type){
 if(context!=expectedContext||visits>=133||expected[visits].address!=address||expected[visits].type!=type)fail(1);
 if(loading)store(expected[visits],visits,seed+1);else check(expected[visits],visits,seed);
 ++visits;
}
template<> void CPersistContext::Transfer<CWideString>(CWideString& value){visit(this,&value,0);}
template<> void CPersistContext::Transfer<CCharString>(CCharString& value){visit(this,&value,1);}
template<> void CPersistContext::Transfer<float>(float& value){visit(this,&value,2);}
template<> void CPersistContext::Transfer<long>(long& value){visit(this,&value,3);}
template<> void CPersistContext::Transfer<unsigned long>(unsigned long& value){visit(this,&value,4);}
template<> void CPersistContext::Transfer<EMusicSetType>(EMusicSetType& value){visit(this,&value,5);}
template<> void CPersistContext::TransferMap<unsigned long,unsigned long>(std::map<unsigned long,unsigned long>& value){visit(this,&value,6);}
template<> void CPersistContext::TransferMap<long,CCharString>(std::map<CCharString,long>& value){visit(this,&value,7);}
static void run(unsigned int sequence){
 NUISystem::CUIMiscThingsDef def;CPersistContext context;expectedContext=&context;
 unsigned int n=0;
 expected[n].address=&def.SpaceSeparator;expected[n++].type=0;
 expected[n].address=&def.CommaSeparator;expected[n++].type=0;
 expected[n].address=&def.NewLineSeparator;expected[n++].type=0;
 expected[n].address=&def.OpenBracket;expected[n++].type=0;
 expected[n].address=&def.CloseBracket;expected[n++].type=0;
 expected[n].address=&def.Positive;expected[n++].type=0;
 expected[n].address=&def.WeaponValueString;expected[n++].type=0;
 expected[n].address=&def.WeaponAugString;expected[n++].type=0;
 expected[n].address=&def.WeaponAugNone;expected[n++].type=0;
 expected[n].address=&def.WeaponWeightString;expected[n++].type=0;
 expected[n].address=&def.WeaponLightString;expected[n++].type=0;
 expected[n].address=&def.WeaponHeavyString;expected[n++].type=0;
 expected[n].address=&def.WeaponKillsString;expected[n++].type=0;
 expected[n].address=&def.WeaponCatMeleeString;expected[n++].type=0;
 expected[n].address=&def.WeaponCatRangedString;expected[n++].type=0;
 expected[n].address=&def.WeaponDamageString;expected[n++].type=0;
 expected[n].address=&def.TradeCostString;expected[n++].type=0;
 expected[n].address=&def.ColonSeparator;expected[n++].type=0;
 expected[n].address=&def.TradeProfitString;expected[n++].type=0;
 expected[n].address=&def.TradeLossString;expected[n++].type=0;
 expected[n].address=&def.TradeAlreadyOwnsString;expected[n++].type=0;
 expected[n].address=&def.TradeNumberInStockString;expected[n++].type=0;
 expected[n].address=&def.TradeDeliveryString;expected[n++].type=0;
 expected[n].address=&def.TradeNoDeliveryString;expected[n++].type=0;
 expected[n].address=&def.TradeDaysString;expected[n++].type=0;
 expected[n].address=&def.TradeBuyString;expected[n++].type=0;
 expected[n].address=&def.TradeSellString;expected[n++].type=0;
 expected[n].address=&def.TradeWantedString;expected[n++].type=0;
 expected[n].address=&def.QuestFailedString;expected[n++].type=0;
 expected[n].address=&def.FailedString;expected[n++].type=0;
 expected[n].address=&def.SucceededString;expected[n++].type=0;
 expected[n].address=&def.Plus;expected[n++].type=0;
 expected[n].address=&def.Minus;expected[n++].type=0;
 expected[n].address=&def.CoreGraphic;expected[n++].type=4;
 expected[n].address=&def.VignetteGraphic;expected[n++].type=4;
 expected[n].address=&def.OptionalGraphic;expected[n++].type=4;
 expected[n].address=&def.FeatGraphic;expected[n++].type=4;
 expected[n].address=&def.ObjectsRewardString;expected[n++].type=0;
 expected[n].address=&def.NoneString;expected[n++].type=0;
 expected[n].address=&def.CheckGuildString;expected[n++].type=0;
 expected[n].address=&def.QuestStartingString;expected[n++].type=0;
 expected[n].address=&def.RingCenter.X;expected[n++].type=2;
 expected[n].address=&def.RingCenter.Y;expected[n++].type=2;
 expected[n].address=&def.PCRingCenter.X;expected[n++].type=2;
 expected[n].address=&def.PCRingCenter.Y;expected[n++].type=2;
 expected[n].address=&def.WorldMapOffset.X;expected[n++].type=2;
 expected[n].address=&def.WorldMapOffset.Y;expected[n++].type=2;
 expected[n].address=&def.WorldMapWidth;expected[n++].type=2;
 expected[n].address=&def.WorldMapHeight;expected[n++].type=2;
 expected[n].address=&def.YouString;expected[n++].type=0;
 expected[n].address=&def.OwnString;expected[n++].type=0;
 expected[n].address=&def.NoString;expected[n++].type=0;
 expected[n].address=&def.HousesString;expected[n++].type=0;
 expected[n].address=&def.HouseString;expected[n++].type=0;
 expected[n].address=&def.InString;expected[n++].type=0;
 expected[n].address=&def.ShopsString;expected[n++].type=0;
 expected[n].address=&def.ShopString;expected[n++].type=0;
 expected[n].address=&def.ThereString;expected[n++].type=0;
 expected[n].address=&def.AreString;expected[n++].type=0;
 expected[n].address=&def.IsString;expected[n++].type=0;
 expected[n].address=&def.ForString;expected[n++].type=0;
 expected[n].address=&def.SaleString;expected[n++].type=0;
 expected[n].address=&def.GeneralString;expected[n++].type=0;
 expected[n].address=&def.TatooString;expected[n++].type=0;
 expected[n].address=&def.BarberString;expected[n++].type=0;
 expected[n].address=&def.TitleString;expected[n++].type=0;
 expected[n].address=&def.LevelString;expected[n++].type=0;
 expected[n].address=&def.TotalSpellsInPalettes;expected[n++].type=4;
 expected[n].address=&def.TotalSpellsInContainer;expected[n++].type=4;
 expected[n].address=&def.TotalAssignableThings;expected[n++].type=4;
 expected[n].address=&def.LogBookBasicsCategoryString;expected[n++].type=0;
 expected[n].address=&def.LogBookObjectsCategoryString;expected[n++].type=0;
 expected[n].address=&def.LogBookTownsCategoryString;expected[n++].type=0;
 expected[n].address=&def.LogBookHeroCategoryString;expected[n++].type=0;
 expected[n].address=&def.LogBookCombatCategoryString;expected[n++].type=0;
 expected[n].address=&def.LogBookQuestCategoryString;expected[n++].type=0;
 expected[n].address=&def.LogBookStoryCategoryString;expected[n++].type=0;
 expected[n].address=&def.LogBookBasicsCategoryNameString;expected[n++].type=0;
 expected[n].address=&def.LogBookObjectsCategoryNameString;expected[n++].type=0;
 expected[n].address=&def.LogBookTownsCategoryNameString;expected[n++].type=0;
 expected[n].address=&def.LogBookHeroCategoryNameString;expected[n++].type=0;
 expected[n].address=&def.LogBookCombatCategoryNameString;expected[n++].type=0;
 expected[n].address=&def.LogBookQuestCategoryNameString;expected[n++].type=0;
 expected[n].address=&def.LogBookStoryCategoryNameString;expected[n++].type=0;
 expected[n].address=&def.MapPaths;expected[n++].type=6;
 expected[n].address=&def.SoundUpDown;expected[n++].type=1;
 expected[n].address=&def.SoundSlider;expected[n++].type=1;
 expected[n].address=&def.SoundBack;expected[n++].type=1;
 expected[n].address=&def.SoundForward;expected[n++].type=1;
 expected[n].address=&def.SoundError;expected[n++].type=1;
 expected[n].address=&def.SoundExit;expected[n++].type=1;
 expected[n].address=&def.HeroDollTL.X;expected[n++].type=2;
 expected[n].address=&def.HeroDollTL.Y;expected[n++].type=2;
 expected[n].address=&def.HeroDollBR.X;expected[n++].type=2;
 expected[n].address=&def.HeroDollBR.Y;expected[n++].type=2;
 expected[n].address=&def.HeroDollSphereRadius;expected[n++].type=2;
 expected[n].address=&def.HeroDollTL_PC.X;expected[n++].type=2;
 expected[n].address=&def.HeroDollTL_PC.Y;expected[n++].type=2;
 expected[n].address=&def.HeroDollBR_PC.X;expected[n++].type=2;
 expected[n].address=&def.HeroDollBR_PC.Y;expected[n++].type=2;
 expected[n].address=&def.HeroDollSphereRadius_PC;expected[n++].type=2;
 expected[n].address=&def.HeroDollFrameTL_PC.X;expected[n++].type=2;
 expected[n].address=&def.HeroDollFrameTL_PC.Y;expected[n++].type=2;
 expected[n].address=&def.HeroDollFrameEmulateListOffset;expected[n++].type=2;
 expected[n].address=&def.QuestStartScreenMusic;expected[n++].type=5;
 expected[n].address=&def.QuestCompleteScreenMusic;expected[n++].type=5;
 expected[n].address=&def.QuestFailureScreenMusic;expected[n++].type=5;
 expected[n].address=&def.DeathScreenMusic;expected[n++].type=5;
 expected[n].address=&def.CountUpSound;expected[n++].type=1;
 expected[n].address=&def.DigitCountTime;expected[n++].type=2;
 expected[n].address=&def.SaveHeroGraphicIndex;expected[n++].type=4;
 expected[n].address=&def.MiniMapGraphics;expected[n++].type=7;
 expected[n].address=&def.SoundKeyboardUp;expected[n++].type=1;
 expected[n].address=&def.SoundKeyboardDown;expected[n++].type=1;
 expected[n].address=&def.SoundKeyboardLeft;expected[n++].type=1;
 expected[n].address=&def.SoundKeyboardRight;expected[n++].type=1;
 expected[n].address=&def.SoundKeyboardEnterCharacter;expected[n++].type=1;
 expected[n].address=&def.SoundKeyboardDeleteCharacter;expected[n++].type=1;
 expected[n].address=&def.SoundKeyboardDone;expected[n++].type=1;
 expected[n].address=&def.FrontEndMusic;expected[n++].type=0;
 expected[n].address=&def.KeyboardSmallKeyGraphic;expected[n++].type=3;
 expected[n].address=&def.KeyboardLargeKeyGraphic;expected[n++].type=3;
 expected[n].address=&def.TimeInSecsForFade;expected[n++].type=2;
 expected[n].address=&def.BackBufferFilterSaturation;expected[n++].type=2;
 expected[n].address=&def.BackBufferFilterContrast;expected[n++].type=2;
 expected[n].address=&def.BackBufferFilterBrightness;expected[n++].type=2;
 expected[n].address=&def.BackBufferFilterTintR;expected[n++].type=2;
 expected[n].address=&def.BackBufferFilterTintG;expected[n++].type=2;
 expected[n].address=&def.BackBufferFilterTintB;expected[n++].type=2;
 expected[n].address=&def.BackBufferFilterTintScale;expected[n++].type=2;
 expected[n].address=&def.BackBufferDiffuseScale;expected[n++].type=2;
 expected[n].address=&def.BackBufferAmbientScale;expected[n++].type=2;
 expected[n].address=&def.MinimumFilterColor;expected[n++].type=2;
 if(n!=133)fail(7);seed=sequence;unsigned int i;
 for(i=0;i<133;++i)store(expected[i],i,seed);
 visits=0;loading=false;def.Transfer(context);if(visits!=133)fail(8);
 for(i=0;i<133;++i)check(expected[i],i,seed);
 visits=0;loading=true;def.Transfer(context);if(visits!=133)fail(9);
 for(i=0;i<133;++i)check(expected[i],i,seed+1);
}
int main(){for(unsigned int i=0;i<64;++i)run(i);puts("00430900_TEST PASS");return 0;}