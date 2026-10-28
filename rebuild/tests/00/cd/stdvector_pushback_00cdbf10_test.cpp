#include <stdio.h>
// std::vector::push_back(...) -- Append a record, growing storage if necessary.
// Leaf: ?push_back@?$vector@VCElement@CDataMarkerOutput@@V?$allocator@VCElement@CDataMarkerOutput@@@std@@@std@@QAEXABVCElement@CDataMarkerOutput@@@Z
// Identity: Own parser00cd5170 and GetStringFromDataMarker00cd4ff0 prove CDataMarkerOutput::CElement, PDB original VariableName/DataAsString/DataAsULONG and class V. Replaces old descriptive CMarkerDataEntry with original name. Helpers00cd4e60/00cd4ea0/_M_clear00cdbcc0.
// Layout: original element12 with two CCharString pointers then one word; outer class used only as nested type scope; real STLport vector12.
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
__declspec(noinline) void _M_insert_overflow(T* position,const T& value,const __false_type&,unsigned int count,bool atEnd){
const unsigned int oldSize=this->_M_finish-this->_M_start;const unsigned int length=oldSize+max(oldSize,count);
T* start=length?(T*)malloc(length*sizeof(T)):0;
T* finish=__uninitialized_copy(this->_M_start,position,start,__false_type());
if(count==1){new(finish)T(value);++finish;}else finish=__uninitialized_fill_n(finish,count,value,__false_type());
if(!atEnd)finish=__uninitialized_copy(position,this->_M_finish,finish,__false_type());
_M_clear();
this->_M_finish=finish;this->_M_start=start;this->_M_end_of_storage._M_data=start+length;
}
void push_back(const T& value){if(this->_M_finish!=this->_M_end_of_storage._M_data){new(this->_M_finish)T(value);++this->_M_finish;}else _M_insert_overflow(this->_M_finish,value,__false_type(),1,true);}
protected:__declspec(noinline) void _M_clear(){T* last=this->_M_finish;for(T* p=this->_M_start;p!=last;++p)p->~T();if(this->_M_start)free(this->_M_start);}
};
}
class CCharStringData;class CCharString{public:CCharStringData* PStringData;CCharString();CCharString(const CCharString&);~CCharString();};
class CDataMarkerOutput{public:class CElement{public:CCharString VariableName,DataAsString;unsigned long DataAsULONG;};};
typedef CDataMarkerOutput::CElement Element;
template void std::vector<Element>::push_back(const Element&);

class CCharStringData{public:int Refs;};static CCharStringData names[32];static int copies,destroyed;
CCharString::CCharString():PStringData(0){}CCharString::CCharString(const CCharString& other):PStringData(other.PStringData){++copies;if(PStringData)++PStringData->Refs;}CCharString::~CCharString(){++destroyed;if(PStringData)--PStringData->Refs;}
static void init(Element& x,int n){x.VariableName.PStringData=&names[n];++names[n].Refs;x.DataAsString.PStringData=&names[n+12];++names[n+12].Refs;x.DataAsULONG=n;}
static bool same(const Element& x,const Element& y){return x.VariableName.PStringData==y.VariableName.PStringData&&x.DataAsString.PStringData==y.DataAsString.PStringData&&x.DataAsULONG==y.DataAsULONG;}
static bool run(){std::vector<Element> v;v._M_start=(Element*)malloc(2*sizeof(Element));v._M_finish=v._M_start;v._M_end_of_storage._M_data=v._M_start+2;Element inputs[3];for(int i=0;i<3;++i)init(inputs[i],i+1);Element* initial=v._M_start;v.push_back(inputs[0]);bool ok=v._M_start==initial&&v._M_finish==initial+1&&same(initial[0],inputs[0]);v.push_back(inputs[1]);ok=ok&&v._M_start==initial&&v._M_finish==initial+2;v.push_back(inputs[2]);ok=ok&&v._M_finish-v._M_start==3&&v._M_end_of_storage._M_data-v._M_start==4;for(int j=0;j<3;++j){ok=ok&&same(v._M_start[j],inputs[j]);v._M_start[j].~Element();}free(v._M_start);return ok;}
int main(){bool ok=run();for(int n=0;n<32;++n)ok=ok&&names[n].Refs==0;puts(ok?"00cdbf10_TEST PASS":"00cdbf10_TEST FAIL");return ok?0:1;}