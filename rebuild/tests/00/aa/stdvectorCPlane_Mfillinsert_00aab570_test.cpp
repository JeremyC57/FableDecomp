#include <stdio.h>
#define protected public
// std::vector<CPlane>::_M_fill_insert(pointer,unsigned int,const value_type&) -- Insert copied records with spare-capacity reuse or growth.
// Leaf: ?_M_fill_insert@?$vector@VCPlane@@V?$allocator@VCPlane@@@std@@@std@@QAEXPAVCPlane@@IABV3@@Z
// Flags: /Gr
// Identity: proven. Own C3DMeshFileConvexVolumeChunk::ReadFromFile00aaab10 invokes target for Planes48, same growth00a13720 and nonconst copy00a11210; CPlane Normal+Distance type independently confirmed. Memberwise user copy constructors and inline uninitialized_fill_n preserve the sixteen-byte local and placement-new null guard; exact original STLport typed helpers.
// Layout: element16 and genuine vector12/base/proxy. No target engine header.
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
class C3DVector{public:float X,Y,Z;C3DVector():X(0),Y(0),Z(0){}C3DVector(const C3DVector& p):X(p.X),Y(p.Y),Z(p.Z){}};class CPlane{public:C3DVector Normal;float Distance;CPlane():Distance(0){}CPlane(const CPlane& p):Normal(p.Normal),Distance(p.Distance){}};
namespace std{
struct __false_type{__false_type(){}};struct input_iterator_tag{};struct forward_iterator_tag:input_iterator_tag{};struct bidirectional_iterator_tag:forward_iterator_tag{};struct random_access_iterator_tag:bidirectional_iterator_tag{};
template<class T>class allocator{public:T* allocate(unsigned int n){return n?(T*)malloc(n*sizeof(T)):0;}void deallocate(T* p,unsigned int){if(p)free(p);}};template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};template<class T,class A>class _Vector_base{protected:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;};
template<class T,class V>__forceinline void __fastcall _Construct(T* p,const V& x){new(p)T(x);}
template<class I,class O>NOINLINE O __fastcall __uninitialized_copy(I first,I last,O result,const __false_type&){O current=result;for(;first!=last;++first,++current)_Construct(&*current,*first);return current;}
template<class I,class N,class T>__forceinline I __fastcall uninitialized_fill_n(I first,N n,const T& value){I current=first;for(;n>0;--n,++current)_Construct(&*current,value);return current;}
template<class I,class O,class D>NOINLINE O __fastcall __copy_backward(I first,I last,O result,const random_access_iterator_tag&,D*){for(D n=last-first;n>0;--n)*--result=*--last;return result;}
template<class I,class T>NOINLINE void __fastcall fill(I first,I last,const T& x){for(;first!=last;++first)*first=x;}
template<class T,class A=allocator<T> >class vector:protected _Vector_base<T,A>{public:typedef unsigned int size_type;void _M_fill_insert(T*,size_type,const T&);protected:NOINLINE void _M_insert_overflow(T*,const T&,const __false_type&,size_type,bool=false);};
template<class T,class A>void vector<T,A>::_M_fill_insert(T* position,size_type n,const T& value){if(n!=0){if((size_type)(this->_M_end_of_storage._M_data-this->_M_finish)>=n){T copy=value;const size_type elemsAfter=(size_type)(this->_M_finish-position);T* oldFinish=this->_M_finish;if(elemsAfter>n){__uninitialized_copy(this->_M_finish-n,this->_M_finish,this->_M_finish,__false_type());this->_M_finish+=n;__copy_backward(position,oldFinish-n,oldFinish,random_access_iterator_tag(),(int*)0);fill(position,position+n,copy);}else{uninitialized_fill_n(this->_M_finish,n-elemsAfter,copy);this->_M_finish+=n-elemsAfter;__uninitialized_copy(position,oldFinish,this->_M_finish,__false_type());this->_M_finish+=elemsAfter;fill(position,oldFinish,copy);}}else _M_insert_overflow(position,value,__false_type(),n,false);}}
}
template void std::vector<CPlane>::_M_fill_insert(CPlane*,unsigned int,const CPlane&);

#undef protected
static int grows;template<class T,class A>void std::vector<T,A>::_M_insert_overflow(T* position,const T& value,const __false_type&,size_type n,bool atEnd){++grows;size_type size=(size_type)(this->_M_finish-this->_M_start);T* first=this->_M_end_of_storage.allocate(size+n);T* out=__uninitialized_copy(this->_M_start,position,first,__false_type());out=uninitialized_fill_n(out,n,value);if(!atEnd)out=__uninitialized_copy(position,this->_M_finish,out,__false_type());this->_M_end_of_storage.deallocate(this->_M_start,0);this->_M_start=first;this->_M_finish=out;this->_M_end_of_storage._M_data=first+size+n;}
typedef CPlane Element;static bool Run(unsigned int count,unsigned int cap,unsigned int index,unsigned int n,bool alias){std::vector<Element> v;v._M_start=(Element*)malloc(sizeof(Element)*cap);v._M_finish=v._M_start+count;v._M_end_of_storage._M_data=v._M_start+cap;for(unsigned int i=0;i<count;++i){new(v._M_start+i)Element;v._M_start[i].Distance=(float)(10+i);v._M_start[i].Normal.X=(float)(20+i);}Element value;value.Distance=71.0f;value.Normal.X=81.0f;const Element& input=alias?v._M_start[count-1]:value;float expected=input.Distance,expectedX=input.Normal.X;v._M_fill_insert(v._M_start+index,n,input);bool ok=(unsigned int)(v._M_finish-v._M_start)==count+n;for(unsigned int i=0;i<count+n;++i){float e=i<index?(float)(10+i):i<index+n?expected:(float)(10+i-n);float ex=i<index?(float)(20+i):i<index+n?expectedX:(float)(20+i-n);ok=ok&&v._M_start[i].Distance==e&&v._M_start[i].Normal.X==ex;}free(v._M_start);return ok;}int main(){bool ok=Run(4,12,1,1,false)&&Run(4,12,2,3,false)&&Run(4,4,2,2,false)&&Run(4,12,1,0,false)&&Run(4,12,1,2,true)&&Run(4,12,4,2,false)&&grows==1;puts(ok?"00aab570_TEST PASS":"00aab570_TEST FAIL");return ok?0:1;}