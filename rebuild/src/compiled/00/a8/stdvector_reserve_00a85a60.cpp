// std::vector::reserve(n) -- Grow capacity without changing the number of elements.
// Leaf: ?reserve@?$vector@VCNavigationLayer@CNavQuadTree@@V?$allocator@VCNavigationLayer@CNavQuadTree@@@std@@@std@@QAEXI@Z
#include <stddef.h>
#include <stdlib.h>
inline void* operator new(size_t,void* p){return p;}
namespace NAnimationEvents { class CEvent; }
template<class T,int N> class CSmallVector {
public:
    T* Start; int Size;
    CSmallVector(); CSmallVector(const CSmallVector&); ~CSmallVector();
};
class CCPPointerInfo;
class CRegionConnectionNode;
template<class T> class CCountedPointer {
public:
    T* Data; CCPPointerInfo* Info;
    CCountedPointer(); CCountedPointer(const CCountedPointer&); ~CCountedPointer();
};
class CNavQuadTree {
public:
    // Layout: retail reserve00a85a60 divides pointer differences by40; owned CNavigationLayer is40bytes.
    class CNavigationLayer {
    public:
        unsigned char _body[40];
        CNavigationLayer();CNavigationLayer(const CNavigationLayer&);~CNavigationLayer();
    };
};
namespace std {
struct __false_type {__false_type(){}};
template<class A,class B> struct pair {
    A first;B second;
    pair():first(),second(){}
    pair(const pair& x):first(x.first),second(x.second){}
};
template<class A,class B> inline void ConstructOne(A* p,const B& x){new(p) A(x);}
template<class T> inline void DestroyOne(T* p){p->~T();}
#pragma auto_inline(off)
template<class Input,class Output>
__declspec(noinline) Output __fastcall __uninitialized_copy(Input first,Input last,Output result,const __false_type&) {
    Output cur=result;for(;first!=last;++first,++cur)ConstructOne(cur,*first);
    return cur;
}
template<class Input> __declspec(noinline) void __fastcall __destroy_aux(Input first,Input last,const __false_type&) {
    for(;first!=last;++first) DestroyOne(first);
}
#pragma auto_inline(on)
template<class T> class allocator {
public:T* allocate(unsigned int n){return n ? (T*)malloc(n*sizeof(T)) : 0;}
    void deallocate(T* p,unsigned int){if(p)free(p);}
};
template<class V,class T,class A> class _STLP_alloc_proxy : public A {public:V _M_data;};
template<class T,class A=allocator<T> > class vector {
public:
    T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;
    unsigned int size()const{return (unsigned int)(_M_finish-_M_start);}
    unsigned int capacity()const{return (unsigned int)(_M_end_of_storage._M_data-_M_start);}
    T* _M_allocate_and_copy(unsigned int n,const T* first,const T* last) {
        T* result=_M_end_of_storage.allocate(n);
        __uninitialized_copy(first,last,result,__false_type());return result;
    }
    void _M_clear(){__destroy_aux(_M_start,_M_finish,__false_type());_M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start);}
    void reserve(unsigned int n) {
        if(capacity()<n) {
            const unsigned int old_size=size();T* tmp;
            if(_M_start){tmp=_M_allocate_and_copy(n,_M_start,_M_finish);_M_clear();}
            else tmp=_M_end_of_storage.allocate(n);
            _M_start=tmp;_M_finish=tmp+old_size;_M_end_of_storage._M_data=tmp+n;
        }
    }
};
}
template void std::vector<CNavQuadTree::CNavigationLayer >::reserve(unsigned int);