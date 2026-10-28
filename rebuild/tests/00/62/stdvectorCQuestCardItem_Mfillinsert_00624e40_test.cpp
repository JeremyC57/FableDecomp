#include <stdio.h>
#define protected public
// std::vector<CQuestCardItem>::_M_fill_insert(pointer,unsigned int,const value_type&) -- Insert copies within spare capacity or grow the vector.
// Leaf: ?_M_fill_insert@?$vector@VCQuestCardItem@@V?$allocator@VCQuestCardItem@@@std@@@std@@QAEXPAVCQuestCardItem@@IABV3@@Z
// Flags: /Gr
// Identity: proven. Own resize006253d0 ->CTCInventoryQuests::OnCreate0061a6a0 atcall0061a731 establishes original QuestCardItemList and CQuestCardItem classV, ItemIndex/ItemText. Eight-byte element with original CWideString copy ctor/dtor; original SGI/STLport fill-insert branch semantics and fastcall free helpers. All declared helper targets retain original template names and typed signatures; unused outer owner instance layout omitted.
// Layout: original vector12 with _Vector_base and _STLP_alloc_proxy; original element8. Helpers have visible real loop bodies, unnamed tag parameters, and no writes through input references.
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
class CWideStringData;class CWideString{public:CWideStringData* PStringData;CWideString();CWideString(const CWideString&);~CWideString();CWideString& operator=(const CWideString&);};
class CQuestCardItem{public:long ItemIndex;CWideString ItemText;};
namespace std{
struct __false_type{__false_type(){}};struct input_iterator_tag{};struct forward_iterator_tag:input_iterator_tag{};struct bidirectional_iterator_tag:forward_iterator_tag{};struct random_access_iterator_tag:bidirectional_iterator_tag{};
template<class T>class allocator{public:T* allocate(unsigned int n){return n?(T*)malloc(n*sizeof(T)):0;}void deallocate(T* p,unsigned int){if(p)free(p);}};template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};template<class T,class A>class _Vector_base{protected:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;};
template<class T,class V>__forceinline void __fastcall _Construct(T* p,const V& x){new(p)T(x);}
template<class I,class O>NOINLINE O __fastcall __uninitialized_copy(I first,I last,O result,const __false_type&){O current=result;for(;first!=last;++first,++current)_Construct(&*current,*first);return current;}
template<class I,class N,class T>NOINLINE I __fastcall uninitialized_fill_n(I first,N n,const T& value){I current=first;for(;n>0;--n,++current)_Construct(&*current,value);return current;}
template<class I,class O,class D>NOINLINE O __fastcall __copy_backward(I first,I last,O result,const random_access_iterator_tag&,D*){for(D n=last-first;n>0;--n)*--result=*--last;return result;}
template<class I,class T>NOINLINE void __fastcall fill(I first,I last,const T& x){for(;first!=last;++first)*first=x;}
template<class T,class A=allocator<T> >class vector:protected _Vector_base<T,A>{public:typedef unsigned int size_type;void _M_fill_insert(T*,size_type,const T&);protected:NOINLINE void _M_insert_overflow(T*,const T&,const __false_type&,size_type,bool=false);};
template<class T,class A>void vector<T,A>::_M_fill_insert(T* position,size_type n,const T& value){if(n!=0){if((size_type)(this->_M_end_of_storage._M_data-this->_M_finish)>=n){T copy=value;const size_type elemsAfter=(size_type)(this->_M_finish-position);T* oldFinish=this->_M_finish;if(elemsAfter>n){__uninitialized_copy(this->_M_finish-n,this->_M_finish,this->_M_finish,__false_type());this->_M_finish+=n;__copy_backward(position,oldFinish-n,oldFinish,random_access_iterator_tag(),(int*)0);fill(position,position+n,copy);}else{uninitialized_fill_n(this->_M_finish,n-elemsAfter,copy);this->_M_finish+=n-elemsAfter;__uninitialized_copy(position,oldFinish,this->_M_finish,__false_type());this->_M_finish+=elemsAfter;fill(position,oldFinish,copy);}}else _M_insert_overflow(position,value,__false_type(),n,false);}}
}
template void std::vector<CQuestCardItem>::_M_fill_insert(CQuestCardItem*,unsigned int,const CQuestCardItem&);

#undef protected
static int alive,copies,assigns,grows;CWideString::CWideString():PStringData(0){++alive;}CWideString::CWideString(const CWideString& x):PStringData(x.PStringData){++alive;++copies;}CWideString::~CWideString(){--alive;}CWideString& CWideString::operator=(const CWideString& x){++assigns;PStringData=x.PStringData;return *this;}
template<class T,class A>void std::vector<T,A>::_M_insert_overflow(T* position,const T& x,const __false_type&,size_type n,bool atEnd){++grows;size_type old=(size_type)(this->_M_finish-this->_M_start);T* start=this->_M_end_of_storage.allocate(old+(old<n?n:old));T* out=start;out=__uninitialized_copy(this->_M_start,position,out,__false_type());out=uninitialized_fill_n(out,n,x);if(!atEnd)out=__uninitialized_copy(position,this->_M_finish,out,__false_type());for(T* p=this->_M_start;p!=this->_M_finish;++p)p->~T();this->_M_end_of_storage.deallocate(this->_M_start,0);this->_M_start=start;this->_M_finish=out;this->_M_end_of_storage._M_data=start+old+(old<n?n:old);}
typedef CQuestCardItem Element;static bool run(unsigned int count,unsigned int capacity,unsigned int index,unsigned int n,bool alias){std::vector<Element> v;Element* start=(Element*)malloc(sizeof(Element)*capacity);for(unsigned int i=0;i<count;++i){new(start+i)Element;start[i].ItemIndex=10+i;}v._M_start=start;v._M_finish=start+count;v._M_end_of_storage._M_data=start+capacity;Element value;value.ItemIndex=71;const Element& input=alias?start[count-1]:value;long want=(long)input.ItemIndex;v._M_fill_insert(start+index,n,input);bool ok=(unsigned int)(v._M_finish-v._M_start)==count+n;for(unsigned int i=0;i<count+n;++i){long expected=i<index?10+i:(i<index+n?want:10+i-n);if((long)v._M_start[i].ItemIndex!=expected)ok=false;}for(Element* p=v._M_start;p!=v._M_finish;++p)p->~Element();free(v._M_start);return ok;}int main(){bool ok=run(4,12,1,1,false)&&run(4,12,2,3,false)&&run(4,4,2,2,false)&&run(4,12,2,0,false)&&run(4,12,1,2,true)&&run(4,12,4,2,false);ok=ok&&alive==0&&grows==1&&copies>0&&assigns>0;puts(ok?"00624e40_TEST PASS":"00624e40_TEST FAIL");return ok?0:1;}