#include <stdio.h>
#define protected public
// std::vector<CArray<unsigned short>>::_M_fill_insert(pointer,unsigned int,const value_type&) -- Insert independent copies of nested arrays.
// Leaf: ?_M_fill_insert@?$vector@V?$CArray@G@@V?$allocator@V?$CArray@G@@@std@@@std@@QAEXPAV?$CArray@G@@IABV3@@Z
// Flags: /Gr
// Identity: proven. Own CTriangleStripifier2::BuildVertexAndPolyMaps00f3e6b0 calls both target fills. Original PDB signature has CArray<CArray<long>> and CArray<CArray<unsigned short>> output maps; full inner vector assignment callees00454886/00f3b640 and copy constructors00514160/00a87090 establish scalar widths and genuine template identity. First copy is inherited vector<long>, second explicit original CArray<unsigned short> copy; inner destructor frees its data through CRT free.
// Layout: outer vector12 and inner CArray/vector12 with genuine base/proxy inheritance. No fabricated twelve-byte record.
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#define NOINLINE __attribute__((noinline))
typedef __SIZE_TYPE__ PlacementSize;
#else
#define NOINLINE __declspec(noinline)
typedef unsigned int PlacementSize;
#endif
inline void* __cdecl operator new(PlacementSize,void* p){return p;}

namespace std{
struct __false_type{__false_type(){}};struct input_iterator_tag{};struct forward_iterator_tag:input_iterator_tag{};struct bidirectional_iterator_tag:forward_iterator_tag{};struct random_access_iterator_tag:bidirectional_iterator_tag{};
template<class T>class allocator{public:T* allocate(unsigned int n){return n?(T*)malloc(n*sizeof(T)):0;}void deallocate(T* p,unsigned int){if(p)free(p);}};template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};template<class T,class A>class _Vector_base{protected:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;};
template<class T,class V>__forceinline void __fastcall _Construct(T* p,const V& x){new(p)T(x);}
template<class I,class O>NOINLINE O __fastcall __uninitialized_copy(I first,I last,O result,const __false_type&){O current=result;for(;first!=last;++first,++current)_Construct(&*current,*first);return current;}
template<class I,class N,class T>NOINLINE I __fastcall __uninitialized_fill_n(I first,N n,const T& value,const __false_type&){I current=first;for(;n>0;--n,++current)_Construct(&*current,value);return current;}
template<class I,class O,class D>NOINLINE O __fastcall __copy_backward(I first,I last,O result,const random_access_iterator_tag&,D*){for(D n=last-first;n>0;--n)*--result=*--last;return result;}
template<class I,class T>NOINLINE void __fastcall fill(I first,I last,const T& x){for(;first!=last;++first)*first=x;}
template<class T,class A=allocator<T> >class vector:protected _Vector_base<T,A>{public:typedef unsigned int size_type;vector(){this->_M_start=this->_M_finish=this->_M_end_of_storage._M_data=0;}vector(const vector&);vector& operator=(const vector&);__forceinline ~vector(){for(T* p=this->_M_start;p!=this->_M_finish;++p)p->~T();if(this->_M_start)free(this->_M_start);}void _M_fill_insert(T*,size_type,const T&);protected:NOINLINE void _M_insert_overflow(T*,const T&,const __false_type&,size_type,bool=false);};
template<class T,class A>void vector<T,A>::_M_fill_insert(T* position,size_type n,const T& value){if(n!=0){if((size_type)(this->_M_end_of_storage._M_data-this->_M_finish)>=n){T copy=value;const size_type elemsAfter=(size_type)(this->_M_finish-position);T* oldFinish=this->_M_finish;if(elemsAfter>n){__uninitialized_copy(this->_M_finish-n,this->_M_finish,this->_M_finish,__false_type());this->_M_finish+=n;__copy_backward(position,oldFinish-n,oldFinish,random_access_iterator_tag(),(int*)0);fill(position,position+n,copy);}else{__uninitialized_fill_n(this->_M_finish,n-elemsAfter,copy,__false_type());this->_M_finish+=n-elemsAfter;__uninitialized_copy(position,oldFinish,this->_M_finish,__false_type());this->_M_finish+=elemsAfter;fill(position,oldFinish,copy);}}else _M_insert_overflow(position,value,__false_type(),n,false);}}
}
template<class T>class CArray:public std::vector<T>{};template<>class CArray<unsigned short>:public std::vector<unsigned short>{public:CArray(){}CArray(const CArray&);};
template void std::vector<CArray<unsigned short> >::_M_fill_insert(CArray<unsigned short>*,unsigned int,const CArray<unsigned short>&);

#undef protected
template<class T,class A>std::vector<T,A>::vector(const vector& x){unsigned int n=(unsigned int)(x._M_finish-x._M_start);this->_M_start=(T*)malloc(n*sizeof(T));this->_M_finish=this->_M_start+n;this->_M_end_of_storage._M_data=this->_M_finish;for(unsigned int i=0;i<n;++i)new(this->_M_start+i)T(x._M_start[i]);}template<class T,class A>std::vector<T,A>& std::vector<T,A>::operator=(const vector& x){if(this!=&x){vector tmp(x);T* a=this->_M_start;T* b=this->_M_finish;T* c=this->_M_end_of_storage._M_data;this->_M_start=tmp._M_start;this->_M_finish=tmp._M_finish;this->_M_end_of_storage._M_data=tmp._M_end_of_storage._M_data;tmp._M_start=a;tmp._M_finish=b;tmp._M_end_of_storage._M_data=c;}return *this;}CArray<unsigned short>::CArray(const CArray& x):std::vector<unsigned short>(x){}
static int grows;template<class T,class A>void std::vector<T,A>::_M_insert_overflow(T* position,const T& value,const __false_type&,size_type n,bool atEnd){++grows;size_type size=(size_type)(this->_M_finish-this->_M_start);T* first=this->_M_end_of_storage.allocate(size+n);T* out=__uninitialized_copy(this->_M_start,position,first,__false_type());out=__uninitialized_fill_n(out,n,value,__false_type());if(!atEnd)out=__uninitialized_copy(position,this->_M_finish,out,__false_type());for(T* p=this->_M_start;p!=this->_M_finish;++p)p->~T();this->_M_end_of_storage.deallocate(this->_M_start,0);this->_M_start=first;this->_M_finish=out;this->_M_end_of_storage._M_data=out;}
typedef CArray<unsigned short> Element;static void Set(Element& x,unsigned int n){x._M_start=(unsigned short*)malloc(2*sizeof(unsigned short));x._M_finish=x._M_start+2;x._M_end_of_storage._M_data=x._M_finish;x._M_start[0]=(unsigned short)n;x._M_start[1]=(unsigned short)(n+100);}static bool Run(unsigned int cap,unsigned int index,unsigned int n,bool alias){std::vector<Element> v;v._M_start=(Element*)malloc(cap*sizeof(Element));v._M_finish=v._M_start+4;v._M_end_of_storage._M_data=v._M_start+cap;for(int i=0;i<4;++i){new(v._M_start+i)Element;Set(v._M_start[i],10+i);}Element x;Set(x,71);const Element& input=alias?v._M_start[3]:x;int wanted=(int)input._M_start[0];v._M_fill_insert(v._M_start+index,n,input);bool ok=(unsigned int)(v._M_finish-v._M_start)==4+n;for(unsigned int i=0;i<4+n;++i){int e=i<index?10+i:i<index+n?wanted:10+i-n;ok=ok&&v._M_start[i]._M_finish-v._M_start[i]._M_start==2&&v._M_start[i]._M_start[0]==e&&v._M_start[i]._M_start[1]==e+100;for(unsigned int j=0;j<i;++j)ok=ok&&v._M_start[i]._M_start!=v._M_start[j]._M_start;}return ok;}int main(){bool ok=Run(12,1,1,false)&&Run(12,2,3,false)&&Run(4,2,2,false)&&Run(12,1,0,false)&&Run(12,1,2,true)&&Run(12,4,2,false)&&grows==1;puts(ok?"00f3f960_TEST PASS":"00f3f960_TEST FAIL");return ok?0:1;}