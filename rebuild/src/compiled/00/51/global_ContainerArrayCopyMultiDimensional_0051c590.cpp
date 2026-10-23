// 0051c590 _STL::__uninitialized_copy (catalog: ContainerArray_CopyMultiDimensional; element types unknown)
// Flags: /O2 /Oy /W3 /Gr
// Leaf: __uninitialized_copy
#include <new>
#include <stdlib.h>
struct __false_type { __false_type(){} };
namespace NCopy0051c590 {
// Layout: retail 0051c590 steps pairs by 24 and elements by 16; vectors are three pointers.
struct A { unsigned int a,b,c,d; A(){} A(unsigned int w,unsigned int x,unsigned int y,unsigned int z):a(w),b(x),c(y),d(z){} __forceinline A(const A& x):a(x.a),b(x.b),c(x.c),d(x.d){} };
// Layout: 0x0051ac00 copies two key dwords and deep-copies list at+0x8; B stride0x10.
// 0x005152c0 allocates0x18-byte nodes with four-dword payload at+0x8.
struct Node {Node*next,*prev;A value;};
struct List {Node*head;List();List(const List&);~List();};
struct B {unsigned __int64 key;List list;};
}
namespace _STL { NCopy0051c590::B* __fastcall __uninitialized_copy(const NCopy0051c590::B*,const NCopy0051c590::B*,NCopy0051c590::B*,const __false_type&); } // 0051ac00
namespace NCopy0051c590 {
__forceinline A* CopyA(A* out,const A* first,const A* last){for(;first!=last;++first,++out)if(out)new(out)A(*first);return out;}
template<class T> struct Storage {
 T *start,*finish,*end;
 __forceinline Storage():start(0),finish(0),end(0){}
 __forceinline Storage(int n) {T* p=0;start=p;finish=p;end=p;if(n)p=(T*)malloc(n*sizeof(T));start=p;finish=p;end=p+n;}
};
struct VA:Storage<A> {
 __forceinline VA():Storage<A>(){}
 __forceinline VA(const VA& x):Storage<A>(x.finish-x.start) {
  finish=CopyA(start,x.start,x.finish);
 }
};
struct VB:Storage<B> {
 __forceinline VB():Storage<B>(){}
 __forceinline VB(const VB& x):Storage<B>(x.finish-x.start) {finish=_STL::__uninitialized_copy(x.start,x.finish,start,__false_type());}
};
struct Pair { VA a; VB b; __forceinline Pair():a(),b(){} __forceinline Pair(const Pair& x):a(x.a),b(x.b){} };
}
namespace _STL {
using namespace NCopy0051c590;
Pair* __fastcall __uninitialized_copy(const Pair* first,const Pair* last,Pair* out,const __false_type&) {
 Pair* result=out;for(;first!=last;++first,++result) if(result) new(result) Pair(*first); return result;
}
}