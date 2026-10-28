#include <stdio.h>
// std::vector::reserve(unsigned int n) -- Allocate enough capacity and preserve the trivial element range.
// Leaf: ?reserve@?$vector@PAVCInstantiatedDefInfo@CDefinitionManager@@V?$allocator@PAVCInstantiatedDefInfo@CDefinitionManager@@@std@@@std@@QAEXI@Z
// Identity: Own CArray::DeallocateUnusedMemory009b6eb0 and PDB CDefinitionManager::InstantiatedDefs establish original nested pointee type; older global CInstantiatedDefInfo name omitted its scope. Retail malloc/memmove/free thunks preserved.
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
typedef CDefinitionManager::CInstantiatedDefInfo* Element;
template void std::vector<Element>::reserve(unsigned int);
class CDefinitionManager::CInstantiatedDefInfo {};

int main(){
    std::vector<Element> v;Element* data=(Element*)malloc(2*sizeof(Element));if(!data)return 1;CDefinitionManager::CInstantiatedDefInfo first,second;data[0]=&first;data[1]=&second;
    v._M_start=data;v._M_finish=data+2;v._M_end_of_storage._M_data=data+2;
    v.reserve(1);int ok=v._M_start==data && v._M_finish==data+2;
    v.reserve(5);ok=ok && v._M_start!=data && v._M_finish==v._M_start+2 && v._M_end_of_storage._M_data==v._M_start+5 && v._M_start[0]==&first && v._M_start[1]==&second;
    Element* grown=v._M_start;v.reserve(5);ok=ok && grown==v._M_start;free(v._M_start);
    v._M_start=v._M_finish=v._M_end_of_storage._M_data=0;v.reserve(0);ok=ok && !v._M_start;
    v.reserve(3);ok=ok && v._M_start && v._M_finish==v._M_start && v._M_end_of_storage._M_data==v._M_start+3;free(v._M_start);
    puts(ok ? "009b4f30_TEST PASS" : "009b4f30_TEST FAIL");return ok ? 0 : 1;
}