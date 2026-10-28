#include <stdio.h>
// std::vector<CCombatAction>::operator=(const vector&) -- Assign existing elements or rebuild storage as required.
// Leaf: ??4?$vector@VCCombatAction@@V?$allocator@VCCombatAction@@@std@@@std@@QAEAAV01@ABV01@@Z
// Identity: proven. Own destruction008b3010 reused by original CCombatAction vector growth008b3430; original CCombatAction8 consists of CCountedPointer<CCombatActionBase>. Original STLport vector copy assignment with allocate/copy/destroy/free paths. Real typed base/proxy hierarchy and genuine helper loops; tag parameters unnamed and never read or written.
// Layout: vector12, original element8, exact scalar malloc/free thunks and explicit nontrivial element semantics. No direct concrete element this layout beyond recovered original members.
#include <stdlib.h>
#include <stddef.h>
#ifndef _MSC_VER
#define __fastcall
#define __declspec(x)
#endif
inline void* operator new(size_t,void* p){return p;}
class CCPPointerInfo;template<class T>class CCountedPointer{public:T* Data;CCPPointerInfo* Info;CCountedPointer();CCountedPointer(const CCountedPointer&);CCountedPointer& operator=(const CCountedPointer&);~CCountedPointer();};namespace NCameraModes{class CSubComponentBase;}class CCombatActionBase;class CCombatAction{public:CCountedPointer<CCombatActionBase> CreatureAction;};
namespace std {
template<class F,class S>struct pair {F first;S second;};
struct __false_type {__false_type(){}};
struct random_access_iterator_tag {random_access_iterator_tag(){}};
template<class T,class U>inline void _Construct(T* p,const U& x){new(p)T(x);}
template<class T>inline void _Destroy(T* p){p->~T();}
#pragma auto_inline(off)
template<class Input,class Output>__declspec(noinline) Output __fastcall __uninitialized_copy(Input first,Input last,Output result,const __false_type&){Output cur=result;for(;first!=last;++first,++cur)_Construct(cur,*first);return cur;}
template<class Iter>__declspec(noinline) void __fastcall __destroy_aux(Iter first,Iter last,const __false_type&){for(;first!=last;++first)_Destroy(first);}
template<class Input,class Output,class Distance>__declspec(noinline) Output __fastcall __copy(Input first,Input last,Output result,const random_access_iterator_tag&,Distance*){for(Distance n=last-first;n>0;--n){*result=*first;++first;++result;}return result;}
#pragma auto_inline(on)
template<class Input,class Output>inline Output __copy_ptrs(Input first,Input last,Output result,const __false_type&){return __copy(first,last,result,random_access_iterator_tag(),(int*)0);}
template<class T>class allocator {public:T* allocate(unsigned int n){return n?(T*)malloc(n*sizeof(T)):0;}void deallocate(T* p,unsigned int){if(p)free(p);}};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
template<class T,class A>class _Vector_base{protected:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:using _Vector_base<T,A>::_M_start;using _Vector_base<T,A>::_M_finish;using _Vector_base<T,A>::_M_end_of_storage;
 unsigned int size()const{return (unsigned int)(_M_finish-_M_start);}unsigned int capacity()const{return (unsigned int)(_M_end_of_storage._M_data-_M_start);}
 T* _M_allocate_and_copy(unsigned int n,const T* first,const T* last){T* result=_M_end_of_storage.allocate(n);__uninitialized_copy(first,last,result,__false_type());return result;}
 void _M_clear(){__destroy_aux(_M_start,_M_finish,__false_type());_M_end_of_storage.deallocate(_M_start,capacity());}
 vector& operator=(const vector& x){if(&x!=this){const unsigned int xlen=x.size();
 if(xlen>capacity()){T* tmp=_M_allocate_and_copy(xlen,(const T*)x._M_start+0,(const T*)x._M_finish+0);_M_clear();_M_start=tmp;_M_end_of_storage._M_data=_M_start+xlen;}
 else if(size()>=xlen){T* i=__copy_ptrs((const T*)x._M_start+0,(const T*)x._M_finish+0,_M_start,__false_type());__destroy_aux(i,_M_finish,__false_type());}
 else{__copy_ptrs((const T*)x._M_start,(const T*)x._M_start+size(),_M_start,__false_type());__uninitialized_copy((const T*)x._M_start+size(),(const T*)x._M_finish+0,_M_finish,__false_type());}
 _M_finish=_M_start+xlen;}return *this;}
};
}
typedef CCombatAction Element;template std::vector<Element>& std::vector<Element>::operator=(const std::vector<Element>&);
static int live,copies,assigns;template<class T>CCountedPointer<T>::CCountedPointer():Data(0),Info(0){++live;}template<class T>CCountedPointer<T>::CCountedPointer(const CCountedPointer& p):Data(p.Data),Info(p.Info){++live;++copies;}template<class T>CCountedPointer<T>& CCountedPointer<T>::operator=(const CCountedPointer& p){Data=p.Data;Info=p.Info;++assigns;return *this;}template<class T>CCountedPointer<T>::~CCountedPointer(){--live;}
static void Init(std::vector<Element>& v,unsigned int count,unsigned int capacity){v._M_start=(Element*)malloc(capacity*sizeof(Element));v._M_finish=v._M_start+count;v._M_end_of_storage._M_data=v._M_start+capacity;for(unsigned int i=0;i<count;++i)new(v._M_start+i)Element();}static void Clear(std::vector<Element>& v){for(Element* p=v._M_start;p!=v._M_finish;++p)p->~Element();free(v._M_start);}int main(){bool ok=true;CCPPointerInfo* tags[4];int data[4];for(int i=0;i<4;++i)tags[i]=reinterpret_cast<CCPPointerInfo*>(data+i);for(int path=0;path<4;++path){std::vector<Element> a,b;Init(b,4,4);Init(a,path==0?1:path==1?6:2,path==0?1:6);for(int j=0;j<4;++j)b._M_start[j].CreatureAction.Info=tags[j];if(path==3){Element* prior=a._M_start;int before=copies+assigns;ok=ok&&&(a=a)==&a&&prior==a._M_start&&before==copies+assigns;}else{ok=ok&&&(a=b)==&a&&a.size()==4;for(int j=0;j<4;++j)ok=ok&&a._M_start[j].CreatureAction.Info==tags[j];}Clear(a);Clear(b);}ok=ok&&live==0;puts(ok?"008b3180_TEST PASS":"008b3180_TEST FAIL");return ok?0:1;}