// 0051c590 std::__uninitialized_copy (catalog: ContainerArray_CopyMultiDimensional; element types unknown)
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
namespace std { NCopy0051c590::B* __fastcall __uninitialized_copy(const NCopy0051c590::B*,const NCopy0051c590::B*,NCopy0051c590::B*,const __false_type&); } // 0051ac00
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
 __forceinline VB(const VB& x):Storage<B>(x.finish-x.start) {finish=std::__uninitialized_copy(x.start,x.finish,start,__false_type());}
};
struct Pair { VA a; VB b; __forceinline Pair():a(),b(){} __forceinline Pair(const Pair& x):a(x.a),b(x.b){} };
}
namespace std {
using namespace NCopy0051c590;
Pair* __fastcall __uninitialized_copy(const Pair* first,const Pair* last,Pair* out,const __false_type&) {
 Pair* result=out;for(;first!=last;++first,++result) if(result) new(result) Pair(*first); return result;
}
}
#include <assert.h>
#include <stdio.h>
#include <stddef.h>
namespace NCopy0051c590 {
static int nodes,sentinels;
List::List(){head=new(malloc(sizeof(Node)))Node;head->next=head->prev=head;++sentinels;}
static void Append(List&l,const A&v){Node*n=new(malloc(sizeof(Node)))Node;n->value=v;n->next=l.head;n->prev=l.head->prev;l.head->prev->next=n;l.head->prev=n;++nodes;}
List::List(const List&l){head=new(malloc(sizeof(Node)))Node;head->next=head->prev=head;++sentinels;for(Node*n=l.head->next;n!=l.head;n=n->next)Append(*this,n->value);}
List::~List(){Node*n=head->next;while(n!=head){Node*next=n->next;n->~Node();free(n);n=next;--nodes;}head->~Node();free(head);--sentinels;}
}
namespace std {
using namespace NCopy0051c590;
B* __fastcall __uninitialized_copy(const B*f,const B*e,B*out,const __false_type&){for(;f!=e;++f,++out)if(out)new(out)B(*f);return out;}
}
int main(){using namespace NCopy0051c590;using namespace std;assert(sizeof(B)==16&&offsetof(B,list)==8&&sizeof(Node)==24&&sizeof(Pair)==24);
 {Pair input[2];A av[2];av[0]=A(1,2,3,4);av[1]=A(5,6,7,8);B bv[2];bv[0].key=((unsigned __int64)7<<32)|11;bv[1].key=19;Append(bv[0].list,A(13,17,23,29));Append(bv[0].list,A(31,37,41,43));
 input[0].a.start=av;input[0].a.finish=input[0].a.end=av+2;input[0].b.start=bv;input[0].b.finish=input[0].b.end=bv+2;
 Pair*out=(Pair*)malloc(sizeof(Pair)*2);__false_type tag;assert(__uninitialized_copy(input,input,out,tag)==out);assert(__uninitialized_copy(input,input+2,out,tag)==out+2);
 assert(out[0].a.finish-out[0].a.start==2&&out[0].b.finish-out[0].b.start==2);assert(out[0].a.start!=av&&out[0].b.start!=bv);assert(out[0].b.start[0].key==bv[0].key&&out[0].b.start[1].key==19);
 List&copied=out[0].b.start[0].list;assert(copied.head!=bv[0].list.head&&copied.head->next!=bv[0].list.head->next);assert(copied.head->next->value.a==13&&copied.head->prev->value.d==43&&copied.head->next->next==copied.head->prev);assert(out[0].b.start[1].list.head->next==out[0].b.start[1].list.head);
 bv[0].list.head->next->value.a=99;av[1].d=101;assert(copied.head->next->value.a==13&&out[0].a.start[1].d==8);assert(nodes==4&&sentinels==4);assert(!out[1].a.start&&!out[1].b.start);
 for(B*b=out[0].b.start;b!=out[0].b.finish;++b)b->~B();for(A*a=out[0].a.start;a!=out[0].a.finish;++a)a->~A();free(out[0].a.start);free(out[0].b.start);out[0].~Pair();out[1].~Pair();free(out);assert(nodes==2&&sentinels==2);
 }
 assert(nodes==0&&sentinels==0);puts("0051c590_TEST PASS");
}