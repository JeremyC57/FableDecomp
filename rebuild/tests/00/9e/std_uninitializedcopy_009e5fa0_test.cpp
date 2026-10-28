#include <stdio.h>
// std::__uninitialized_copy(Element* first,Element* last,Element* result,const __false_type&) -- Copy construct the range into raw storage.
// Leaf: ??$__uninitialized_copy@PAVCSymbolInfo@CSymbolMap@@PAV12@@std@@YIPAVCSymbolInfo@CSymbolMap@@PAV12@00ABU__false_type@0@@Z
// Flags: /Gr
// Identity: Own GetSymbolsFromGroupName009e5290 outputs CArray<CSymbolInfo>; original GroupName/EntryName/Val and class V. Helpers009e5fa0/009e5fe0/_M_clear009e7250.
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
template<class Input,class Output>__declspec(noinline) Output __fastcall __uninitialized_copy(Input first,Input last,Output result,const __false_type&){Output current=result;for(;first!=last;++first,++current)_Construct(current,*first);return current;}
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
class CSymbolMap{public:class CSymbolInfo{public:CCharString GroupName,EntryName;long Val;};};
typedef CSymbolMap::CSymbolInfo Element;
template Element* __fastcall std::__uninitialized_copy<Element*,Element*>(Element*,Element*,Element*,const std::__false_type&);

class CCharStringData{public:int Refs;};static CCharStringData names[32];static int copies,destroyed;
CCharString::CCharString():PStringData(0){}CCharString::CCharString(const CCharString& other):PStringData(other.PStringData){++copies;if(PStringData)++PStringData->Refs;}CCharString::~CCharString(){++destroyed;if(PStringData)--PStringData->Refs;}
static void init(Element& x,int n){x.GroupName.PStringData=&names[n];++names[n].Refs;x.EntryName.PStringData=&names[n+12];++names[n+12].Refs;x.Val=n;}
static bool same(const Element& x,const Element& y){return x.GroupName.PStringData==y.GroupName.PStringData&&x.EntryName.PStringData==y.EntryName.PStringData&&x.Val==y.Val;}
static bool run(unsigned int count){Element input[3];for(int j=0;j<3;++j)init(input[j],j+1);Element* output=(Element*)malloc(3*sizeof(Element));int before=copies;Element* end=std::__uninitialized_copy(input,input+count,output,std::__false_type());bool ok=end==output+count&&copies-before==(int)count*2;for(unsigned int j=0;j<count;++j){ok=ok&&same(output[j],input[j]);output[j].~Element();}free(output);return ok;}
int main(){bool ok=run(0)&&run(1)&&run(3);for(int n=0;n<32;++n)ok=ok&&names[n].Refs==0;puts(ok?"009e5fa0_TEST PASS":"009e5fa0_TEST FAIL");return ok?0:1;}