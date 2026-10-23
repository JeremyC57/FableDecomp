// 005e31f0 Row005e31f0::Vector::InsertOverflow
// Flags: /O2 /Oy /W3 
// Leaf: InsertOverflow
#include <stdlib.h>
#include <new>
namespace Row005e31f0 {
// Layout: oracle uses a 12-byte STLport vector; element stride 20 with counted pointer at final dword.
struct Ref {unsigned vtable;int refs;}; struct Element {unsigned a,b,c,d;Ref*ref;Element():a(0),b(0),c(0),d(0),ref(0){}__forceinline Element(const Element&v):a(v.a),b(v.b),c(v.c),d(v.d),ref(v.ref){if(ref)++ref->refs;}};
struct Tag {Tag(){}};
Element* __fastcall Copy(Element*,Element*,Element*,const bool&);
Element* __fastcall Fill(Element*,unsigned,const Element&,const bool&);
void __fastcall Destroy(Element*,Element*,const bool&);
__forceinline const unsigned& Max(const unsigned&a,const unsigned&b){return a<b?b:a;}
template<class T> struct Vector {T*first,*last,*limit;void InsertOverflow(T*,const T&,const Tag&,unsigned,bool);};
template<class T> void Vector<T>::InsertOverflow(T*pos,const T&value,const Tag&,unsigned n,bool atEnd) {
 unsigned old=last-first; unsigned capacity=old+Max(old,n);
 T* buffer=capacity?(T*)malloc(capacity*sizeof(T)):0;
 T* finish=Copy(first,pos,buffer,atEnd);
 if(n==1){if(finish)new(finish)T(value);++finish;}
 else finish=Fill(finish,n,value,atEnd);
 if(!atEnd)finish=Copy(pos,last,finish,atEnd);
 Destroy(first,last,atEnd);if(first)free(first);
 first=buffer;last=finish;limit=buffer+capacity;
}
template struct Vector<Element>;
}