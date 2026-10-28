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