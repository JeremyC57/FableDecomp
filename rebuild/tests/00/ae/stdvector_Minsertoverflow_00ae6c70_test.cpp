#include <stdio.h>
#include <string.h>
// std::vector::_M_insert_overflow(T* position,const T& value,const __false_type&,unsigned int count,bool atEnd) -- Grow storage and copy the prefix, inserted values and suffix.
// Leaf: ?_M_insert_overflow@?$vector@VCUnprovenRecord_00ae6c70@@V?$allocator@VCUnprovenRecord_00ae6c70@@@std@@@std@@QAEXPAVCUnprovenRecord_00ae6c70@@ABV3@ABU__false_type@2@I_N@Z
// Identity: descriptive; Only caller00ae7290 is an unreferenced12-byte POD push_back. No use establishes record semantics. Three copied words and12-byte stride proven, record identity unresolved; helpers00ae4ea0/00ae4ee0 separate from other families.
// Layout:12-byte POD element, real STLport vector12. Nested outer names provide only type scope, no outer fields touched.
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
class CUnprovenRecord_00ae6c70 {public:unsigned int Word0,Word1,Word2;};
typedef CUnprovenRecord_00ae6c70 Element;
template void std::vector<Element>::_M_insert_overflow(Element*,const Element&,const std::__false_type&,unsigned int,bool);

static void init(Element& x,int n){x.Word0=n;x.Word1=n+10;x.Word2=n+20;}
static bool same(const Element& x,const Element& y){return x.Word0==y.Word0 && x.Word1==y.Word1 && x.Word2==y.Word2;}
static bool run(unsigned int count,bool atEnd){std::vector<Element> v;v._M_start=(Element*)malloc(3*sizeof(Element));v._M_finish=v._M_start+3;v._M_end_of_storage._M_data=v._M_finish;Element originals[3],input;
for(int i=0;i<3;++i){init(originals[i],i+1);new(v._M_start+i)Element(originals[i]);}init(input,9);unsigned int pos=atEnd?3:1;
v._M_insert_overflow(v._M_start+pos,input,std::__false_type(),count,atEnd);bool ok=(unsigned int)(v._M_finish-v._M_start)==3+count && (unsigned int)(v._M_end_of_storage._M_data-v._M_start)==3+(3<count?count:3);
for(unsigned int i=0;i<pos;++i)ok=ok && same(v._M_start[i],originals[i]);for(unsigned int j=0;j<count;++j)ok=ok && same(v._M_start[pos+j],input);if(!atEnd)for(unsigned int k=pos;k<3;++k)ok=ok && same(v._M_start[count+k],originals[k]);
for(Element* p=v._M_start;p!=v._M_finish;++p)p->~Element();free(v._M_start);return ok;}
int main(){bool ok=run(1,false)&&run(4,false)&&run(1,true)&&run(0,false)&&run(5,true);puts(ok?"00ae6c70_TEST PASS":"00ae6c70_TEST FAIL");return ok?0:1;}