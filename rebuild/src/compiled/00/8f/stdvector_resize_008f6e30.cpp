// std::vector::resize(unsigned int n, const T& value) -- Shrink the range or insert copies of value.
// Leaf: ?resize@?$vector@VCHeightMapCell@@V?$allocator@VCHeightMapCell@@@std@@@std@@QAEXIABVCHeightMapCell@@@Z
// Identity: Own height-map resize caller008f63c0 and PDB12-byte cell layout. Own copy008f65c0 and fill-insert008f6c90.
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
#include "engine/CHeightMapCell.h"
typedef CHeightMapCell Element;
template void std::vector<Element>::resize(unsigned int,const Element&);