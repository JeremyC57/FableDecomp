// 00ab3810 CVertexStreamFormat::AddDataElement
// Flags: /Gr
// Leaf: ?AddDataElement@CVertexStreamFormat@@QAEXKW4EVertexStreamDataType@@@Z
#include <new>
#include <stdlib.h>
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#endif
enum EVertexStreamDataType{};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
namespace std{
 struct __false_type{};
 template<class T>class allocator{};
 template<class P,class T,class A>class _STLP_alloc_proxy:public A{public:P _M_data;};
 template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
 template<class T1,class T2>__forceinline void _Construct(T1*p,const T2&x){new(p)T1(x);}
 template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{
 protected:
  __declspec(noinline) void _M_insert_overflow(T*,const T&,const __false_type&,unsigned int,bool=false);
 public:
  unsigned int size()const{return this->_M_finish-this->_M_start;}
  __forceinline void push_back(const T&x){if(this->_M_finish!=this->_M_end_of_storage._M_data){_Construct(this->_M_finish,x);++this->_M_finish;}else _M_insert_overflow(this->_M_finish,x,__false_type(),1,true);}
 };
 template<class T,class A>__declspec(noinline)void vector<T,A>::_M_insert_overflow(T*position,const T&x,const __false_type&,unsigned int fill,bool atEnd){
  unsigned int old_size=size();unsigned int capacity=old_size+(old_size<fill?fill:old_size);
  T*new_start=(T*)malloc(capacity*sizeof(T));if(!new_start)throw bad_alloc();
  T*new_finish=new_start;
  for(T*p=this->_M_start;p!=position;++p,++new_finish)new(new_finish)T(*p);
  for(unsigned int i=0;i<fill;++i,++new_finish)new(new_finish)T(x);
  if(!atEnd)for(T*p=position;p!=this->_M_finish;++p,++new_finish)new(new_finish)T(*p);
  for(T*p=this->_M_start;p!=this->_M_finish;++p)p->~T();
  if(this->_M_start)free(this->_M_start);
  this->_M_start=new_start;this->_M_finish=new_finish;this->_M_end_of_storage._M_data=new_start+capacity;
 }
}
template<class T>class CArray:public std::vector<T>{};
// Layout: original CBaseClass4 and STLport12-byte CArray DataElements4; real
// nested CDataElement Register0/DataType4, size8. No matching engine header.
class CVertexStreamFormat:public CBaseClass{public:
 class CDataElement{public:unsigned long Register;EVertexStreamDataType DataType;
  __forceinline CDataElement(unsigned long r,EVertexStreamDataType t):Register(r),DataType(t){}
 };
 CArray<CDataElement>DataElements;
 void AddDataElement(unsigned long,EVertexStreamDataType);
};
void CVertexStreamFormat::AddDataElement(unsigned long r,EVertexStreamDataType t){DataElements.push_back(CDataElement(r,t));}