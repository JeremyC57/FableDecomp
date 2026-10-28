// std::vector::_M_insert_overflow(Element* position,const Element& value,const __false_type&,unsigned int count,bool atEnd) -- Grow storage and insert copies.
// Leaf: ?_M_insert_overflow@?$vector@VCVertexShaderConstant@@V?$allocator@VCVertexShaderConstant@@@std@@@std@@QAEXPAVCVertexShaderConstant@@ABV3@ABU__false_type@2@I_N@Z
// Identity: Own uninitialized-copy00bdb6d0 also used by CEngineVSConstantLayoutBasic ctor00bdbb70, original ReservedPresets at+320; fill00bdb710 is N148.
// Layout: real STLport vector12, implicit sixteen-byte element copy, original class keys and fields.
#include <new>
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
namespace std {
struct __false_type {__false_type(){}};
template<class T>class allocator{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A {public:V _M_data;};
template<class T,class A>class _Vector_base {public:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;};
template<class T,class U>__forceinline void _Construct(T* p,const U& value){new(p)T(value);}
template<class Input,class Output>__declspec(noinline) Output __fastcall __uninitialized_copy(Input first,Input last,Output result,const __false_type&){for(;first!=last;++first,++result)_Construct(result,*first);return result;}
template<class Iter,class Size,class T>__declspec(noinline) Iter __fastcall __uninitialized_fill_n(Iter first,Size count,const T& value,const __false_type&){for(;count>0;--count,++first)_Construct(first,value);return first;}
template<class T>__forceinline const T& max(const T& a,const T& b){return a<b?b:a;}
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A> {public:
void _M_insert_overflow(T* position,const T& value,const __false_type&,unsigned int count,bool atEnd){
const unsigned int oldSize=this->_M_finish-this->_M_start;const unsigned int length=oldSize+max(oldSize,count);
T* start=length?(T*)malloc(length*sizeof(T)):0;
T* finish=__uninitialized_copy(this->_M_start,position,start,__false_type());
if(count==1){new(finish)T(value);++finish;}else finish=__uninitialized_fill_n(finish,count,value,__false_type());
if(!atEnd)finish=__uninitialized_copy(position,this->_M_finish,finish,__false_type());
for(T* p=this->_M_start;p!=this->_M_finish;++p)p->~T();if(this->_M_start)free(this->_M_start);
this->_M_finish=finish;this->_M_start=start;this->_M_end_of_storage._M_data=start+length;
}
};
}
class CVertexShaderConstant{public:float X,Y,Z,W;};
typedef CVertexShaderConstant Element;
template void std::vector<Element>::_M_insert_overflow(Element*,const Element&,const std::__false_type&,unsigned int,bool);