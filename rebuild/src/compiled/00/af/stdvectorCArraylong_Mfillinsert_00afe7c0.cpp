// std::vector<CArray<long>>::_M_fill_insert(pointer,unsigned int,const value_type&) -- Insert independent copies of nested arrays.
// Leaf: ?_M_fill_insert@?$vector@V?$CArray@J@@V?$allocator@V?$CArray@J@@@std@@@std@@QAEXPAV?$CArray@J@@IABV3@@Z
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
template void std::vector<CArray<long> >::_M_fill_insert(CArray<long>*,unsigned int,const CArray<long>&);