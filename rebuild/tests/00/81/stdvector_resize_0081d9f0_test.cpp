#include <stdio.h>
#include <string.h>
// std::vector::resize(unsigned int n, const T& value) -- Shrink the range or insert copies of value.
// Leaf: ?resize@?$vector@U?$pair@KVCEntry@CSkeletalMorphs@@@std@@V?$allocator@U?$pair@KVCEntry@CSkeletalMorphs@@@std@@@2@@std@@QAEXIABU?$pair@KVCEntry@CSkeletalMorphs@@@2@@Z
// Identity: CSkeletalMorphs::Morphs and own Expand0081df20 caller; PDB8-byte value MorphName/Priority/SupressesLowerMorphs. Own copy00819440 and fill-insert0081ce50.
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
namespace std {
template<class K,class V>struct pair{K first;V second;};
struct input_iterator_tag{};struct forward_iterator_tag:input_iterator_tag{};struct bidirectional_iterator_tag:forward_iterator_tag{};struct random_access_iterator_tag:bidirectional_iterator_tag{random_access_iterator_tag(){}};
template<class Input,class Output,class Distance> Output __fastcall __copy(Input,Input,Output,const random_access_iterator_tag&,Distance*);
template<class T>class allocator{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
template<class T,class A=allocator<T> >class vector {
public:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;
    unsigned int size()const{return (unsigned int)(_M_finish-_M_start);}
    __forceinline T* erase(T* first,T* last){_M_finish=__copy(last,_M_finish,first,random_access_iterator_tag(),(int*)0);return first;}
    void resize(unsigned int n,const T& value){if(n<size())erase(_M_start+n,_M_finish);else _M_fill_insert(_M_finish,n-size(),value);}
protected:void _M_fill_insert(T*,unsigned int,const T&);
};
}
class CDefString {public:long TablePos;};
// Layout: outer CSkeletalMorphs is only a nested-name scope. Its CEntry is8bytes: MorphName+0,Priority+4,SupressesLowerMorphs+6; unsigned-key pair12bytes.
class CSkeletalMorphs {public:class CEntry {public:CDefString MorphName;short Priority;bool SupressesLowerMorphs;};};
typedef std::pair<unsigned long,CSkeletalMorphs::CEntry> Element;
template void std::vector<Element>::resize(unsigned int,const Element&);

static int copies,fills,bad;static const void* expectedLast;static const void* expectedResult;static const void* expectedValue;static unsigned int expectedCount;
namespace std {
template<class Input,class Output,class Distance>Output __fastcall __copy(Input first,Input last,Output result,const random_access_iterator_tag&,Distance*){++copies;if(first!=expectedLast || last!=expectedLast || result!=expectedResult)bad=1;for(;first!=last;++first,++result)*result=*first;return result;}
template<class T,class A>void vector<T,A>::_M_fill_insert(T* pos,unsigned int n,const T& value){++fills;if(pos!=_M_finish || n!=expectedCount || &value!=expectedValue)bad=1;for(unsigned int i=0;i<n;++i)*_M_finish++=value;}
}
int main(){
    Element data[6],value;memset(data,0,sizeof(data));memset(&value,0,sizeof(value));std::vector<Element> v;v._M_start=data;v._M_finish=data+4;v._M_end_of_storage._M_data=data+6;
    expectedLast=data+4;expectedResult=data+1;v.resize(1,value);int ok=!bad && copies==1 && !fills && v._M_finish==data+1;
    expectedValue=&value;expectedCount=3;v.resize(4,value);ok=ok && !bad && copies==1 && fills==1 && v._M_finish==data+4;
    expectedCount=0;v.resize(4,value);ok=ok && !bad && fills==2 && copies==1;
    expectedLast=data+4;expectedResult=data;v.resize(0,value);ok=ok && !bad && copies==2 && v._M_finish==data;
    ok=ok && v._M_start==data && v._M_end_of_storage._M_data==data+6;
    puts(ok ? "0081d9f0_TEST PASS" : "0081d9f0_TEST FAIL");return ok ? 0 : 1;
}