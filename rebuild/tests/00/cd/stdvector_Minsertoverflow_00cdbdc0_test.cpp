#include <stdio.h>
// std::vector::_M_insert_overflow(...) -- Grow storage and insert string-bearing records.
// Leaf: ?_M_insert_overflow@?$vector@VCElement@CDataMarkerOutput@@V?$allocator@VCElement@CDataMarkerOutput@@@std@@@std@@QAEXPAVCElement@CDataMarkerOutput@@ABV34@ABU__false_type@2@I_N@Z
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
template void std::vector<Element>::_M_insert_overflow(Element*,const Element&,const std::__false_type&,unsigned int,bool);

class CCharStringData{public:int Refs;};static CCharStringData names[32];static int copies,destroyed;
CCharString::CCharString():PStringData(0){}CCharString::CCharString(const CCharString& other):PStringData(other.PStringData){++copies;if(PStringData)++PStringData->Refs;}CCharString::~CCharString(){++destroyed;if(PStringData)--PStringData->Refs;}
static void init(Element& x,int n){x.VariableName.PStringData=&names[n];++names[n].Refs;x.DataAsString.PStringData=&names[n+12];++names[n+12].Refs;x.DataAsULONG=n;}
static bool same(const Element& x,const Element& y){return x.VariableName.PStringData==y.VariableName.PStringData&&x.DataAsString.PStringData==y.DataAsString.PStringData&&x.DataAsULONG==y.DataAsULONG;}
static bool run(unsigned int count,bool atEnd){std::vector<Element> v;v._M_start=(Element*)malloc(3*sizeof(Element));v._M_finish=v._M_start+3;v._M_end_of_storage._M_data=v._M_finish;Element originals[3],input;for(int i=0;i<3;++i){init(originals[i],i+1);new(v._M_start+i)Element(originals[i]);}init(input,9);unsigned int pos=atEnd?3:1;int before=copies;v._M_insert_overflow(v._M_start+pos,input,std::__false_type(),count,atEnd);bool ok=(unsigned int)(v._M_finish-v._M_start)==3+count&&(unsigned int)(v._M_end_of_storage._M_data-v._M_start)==3+(3<count?count:3)&&copies-before==(int)(3+count)*2;for(unsigned int i=0;i<pos;++i)ok=ok&&same(v._M_start[i],originals[i]);for(unsigned int j=0;j<count;++j)ok=ok&&same(v._M_start[pos+j],input);if(!atEnd)for(unsigned int k=pos;k<3;++k)ok=ok&&same(v._M_start[count+k],originals[k]);for(Element* p=v._M_start;p!=v._M_finish;++p)p->~Element();free(v._M_start);return ok;}
int main(){bool ok=run(1,false)&&run(4,false)&&run(1,true)&&run(0,false)&&run(5,true);for(int n=0;n<32;++n)ok=ok&&names[n].Refs==0;puts(ok?"00cdbdc0_TEST PASS":"00cdbdc0_TEST FAIL");return ok?0:1;}