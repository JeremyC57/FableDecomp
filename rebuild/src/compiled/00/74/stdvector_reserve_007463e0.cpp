// std::vector::reserve(unsigned int n) -- Allocate enough capacity and preserve the trivial element range.
// Leaf: ?reserve@?$vector@PBVCThing@@V?$allocator@PBVCThing@@@std@@@std@@QAEXI@Z
// Identity: CTCStealth::GetNumberOfAttackersOverMaximum00747890 and CTCPerceiveThing::UpdateLineOfSight00745120 use vector<const CThing*>. Retail malloc/memmove/free thunks preserved.
#include <stdlib.h>
#include <string.h>
class CThing;
// Layout: CDefinitionManager is a nested-name scope only; no owner or pointee instance is accessed by reserve.
class CDefinitionManager {public:class CInstantiatedDefInfo;};
namespace std {
template<class T>class allocator {public:T* allocate(unsigned int n){return n?(T*)malloc(n*sizeof(T)):0;}void deallocate(T* p,unsigned int){if(p)free(p);}};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
inline void* __copy_trivial(const void* first,const void* last,void* result){if(last!=first)memmove(result,first,(const char*)last-(const char*)first);return (char*)result+((const char*)last-(const char*)first);}
template<class T,class A=allocator<T> >class vector {
public:
    T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;
    template<class Input>T* _M_allocate_and_copy(unsigned int n,Input first,Input last){T* result=_M_end_of_storage.allocate(n);__copy_trivial(first,last,result);return result;}
    void reserve(unsigned int n){
        if((unsigned int)(_M_end_of_storage._M_data-_M_start)<n){
            const unsigned int oldSize=(unsigned int)(_M_finish-_M_start);T* result;
            if(_M_start){result=_M_allocate_and_copy(n,_M_start,_M_finish);_M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start);}
            else result=_M_end_of_storage.allocate(n);
            _M_start=result;_M_finish=result+oldSize;_M_end_of_storage._M_data=result+n;
        }
    }
};
}
typedef const CThing* Element;
template void std::vector<Element>::reserve(unsigned int);