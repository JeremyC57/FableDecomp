#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __declspec(x) __attribute__((x))
#endif
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

static int copies,destructs;
template<class T,int N> CSmallVector<T,N>::CSmallVector():Start(0),Size(0){}
template<class T,int N> CSmallVector<T,N>::CSmallVector(const CSmallVector& x):Start(x.Start),Size(x.Size){++copies;}
template<class T,int N> CSmallVector<T,N>::~CSmallVector(){++destructs;}
template<class T> CCountedPointer<T>::CCountedPointer():Data(0),Info(0){}
template<class T> CCountedPointer<T>::CCountedPointer(const CCountedPointer& x):Data(x.Data),Info(x.Info){++copies;}
template<class T> CCountedPointer<T>::~CCountedPointer(){++destructs;}
CNavQuadTree::CNavigationLayer::CNavigationLayer(){memset(_body,0,sizeof(_body));}
CNavQuadTree::CNavigationLayer::CNavigationLayer(const CNavigationLayer& x){memcpy(_body,x._body,sizeof(_body));++copies;}
CNavQuadTree::CNavigationLayer::~CNavigationLayer(){++destructs;}
int main() {
    typedef CNavQuadTree::CNavigationLayer Element;
    std::vector<Element> v;
    Element* buffer=(Element*)malloc(2*sizeof(Element));new(buffer) Element;new(buffer+1) Element;
    memset(buffer[0]._body,17,40);memset(buffer[1]._body,29,40);
    v._M_start=buffer;v._M_finish=buffer+2;v._M_end_of_storage._M_data=buffer+2;
    copies=destructs=0;v.reserve(1);
    int ok=v._M_start==buffer && v.size()==2 && v.capacity()==2 && copies==0 && destructs==0;
    v.reserve(5);ok=ok && v._M_start!=buffer && v.size()==2 && v.capacity()==5 && copies==2 && destructs==2 && v._M_start[0]._body[0]==17 && v._M_start[0]._body[39]==17 && v._M_start[1]._body[0]==29 && v._M_start[1]._body[39]==29;
    Element* grown=v._M_start;v.reserve(5);ok=ok && v._M_start==grown && copies==2 && destructs==2;
    std::vector<Element> empty;empty._M_start=empty._M_finish=empty._M_end_of_storage._M_data=0;
    empty.reserve(0);ok=ok && empty._M_start==0;
    empty.reserve(3);ok=ok && empty._M_start!=0 && empty.size()==0 && empty.capacity()==3 && copies==2 && destructs==2;
    grown[0].~Element();grown[1].~Element();free(grown);free(empty._M_start);
    puts(ok ? "00a85a60_TEST PASS" : "00a85a60_TEST FAIL");return ok ? 0 : 1;
}