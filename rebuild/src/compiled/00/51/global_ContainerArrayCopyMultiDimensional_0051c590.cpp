// 0051c590 NCopy0051c590::UninitializedCopyPairs
// Flags: /O2 /Oy /W3 /Gr
// Leaf: UninitializedCopyPairs
#include <new>
#include <stdlib.h>
namespace NCopy0051c590 {
// Layout: retail 0051c590 steps pairs by 24 and elements by 16; vectors are three pointers.
struct A { unsigned int a,b,c,d; A(){} A(unsigned int w,unsigned int x,unsigned int y,unsigned int z):a(w),b(x),c(y),d(z){} __forceinline A(const A& x):a(x.a),b(x.b),c(x.c),d(x.d){} };
// Layout: 0x0051ac00 copies two key dwords and deep-copies list at+0x8; B stride0x10.
// 0x005152c0 allocates0x18-byte nodes with four-dword payload at+0x8.
struct Node {Node*next,*prev;A value;};
struct List {Node*head;List();List(const List&);~List();};
struct B {unsigned __int64 key;List list;};
struct Tag { Tag(){} };
B* __fastcall CopyB(const B*,const B*,B*,const Tag&); // 0051ac00
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
 __forceinline VB(const VB& x):Storage<B>(x.finish-x.start) {finish=CopyB(x.start,x.finish,start,Tag());}
};
struct Pair { VA a; VB b; __forceinline Pair():a(),b(){} __forceinline Pair(const Pair& x):a(x.a),b(x.b){} };
Pair* __fastcall UninitializedCopyPairs(const Pair* first,const Pair* last,Pair* out,const Tag&) {
 Pair* result=out;for(;first!=last;++first,++result) if(result) new(result) Pair(*first); return result;
}
}