// 005e38c0 CVectorMap::Insert
// Leaf: ?Insert@?$CVectorMap@KVCAnimationEntry@@V?$CKeyPairCompareLess@KVCAnimationEntry@@@@@@QAEXABKABVCAnimationEntry@@@Z
// Flags:
#include <new>
#include "rebuild_abi.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CIVCountedPointeeBase.h"
template<class T>class CIVCountedPointer {
private:CIVCountedPointeeBase*Object;
public:CIVCountedPointer():Object(0){}
 CIVCountedPointer(const CIVCountedPointer&p):Object(p.Object){if(Object)Object->AddRef();}
 ~CIVCountedPointer(){Release();}
 void Release(){CIVCountedPointeeBase*p=Object;if(p){if(p->Release()==0)p->Delete();Object=0;}}
};
class CDefString{private:long TablePos;};
class CAnimComponentMap;
class CAnimationEntry{public:long BankIndex;CDefString AnimName,GroupName;private:CIVCountedPointer<CAnimComponentMap>Components;};
namespace std{
template<class A,class B>struct pair{A first;B second;pair(const A&a,const B&b):first(a),second(b){}};
template<class T>class allocator{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
template<class T,class A>class _Vector_base{protected:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
struct __false_type;
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:void push_back(const T&);protected:void _M_insert_overflow(T*,const T&,const __false_type&,unsigned int,bool);};
}
template<class T>class CArray:public std::vector<T>{};
template<class K,class V>class CKeyPairCompareLess{};
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
// Analysis-only visible overflow context reads each input and never publishes the input reference.
namespace std{
template<class T,class A>__declspec(noinline)void vector<T,A>::_M_insert_overflow(T*position,const T&x,const __false_type&,unsigned int fill,bool atEnd){
 unsigned int oldSize=this->_M_finish-this->_M_start;
 unsigned int length=oldSize+(oldSize>fill?oldSize:fill);
 T*start=static_cast<T*>(malloc(length*sizeof(T)));T*finish=start;
 for(T*p=this->_M_start;p!=position;++p,++finish)new(finish)T(*p);
 for(unsigned int n=0;n<fill;++n,++finish)new(finish)T(x);
 if(!atEnd)for(T*p=position;p!=this->_M_finish;++p,++finish)new(finish)T(*p);
 for(T*p=this->_M_start;p!=this->_M_finish;++p)p->~T();
 free(this->_M_start);
 this->_M_start=start;this->_M_finish=finish;this->_M_end_of_storage._M_data=start+length;
}
}
namespace std{
struct __false_type{};
template<class T,class A>__declspec(noinline)void vector<T,A>::push_back(const T&x){
 if(this->_M_finish!=this->_M_end_of_storage._M_data){new(this->_M_finish)T(x);++this->_M_finish;}
 else _M_insert_overflow(this->_M_finish,x,__false_type(),1,true);
}
}
// Layout: original public CArray base shrinks16 donor bytes to12 retail; Compare12 and Dirty13.
template<class K,class V,class C>class CVectorMap:public CArray<std::pair<K,V> >{
private:C Compare;bool Dirty;
public:typedef std::pair<K,V>value_type;
 void Insert(const K&key,const V&value){this->push_back(value_type(key,value));Dirty=true;}
};
template void CVectorMap<unsigned long,CAnimationEntry,CKeyPairCompareLess<unsigned long,CAnimationEntry> >::Insert(const unsigned long&,const CAnimationEntry&);