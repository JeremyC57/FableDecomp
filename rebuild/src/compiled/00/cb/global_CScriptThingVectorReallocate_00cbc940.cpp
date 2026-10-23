// 00cbc940 Row00cbc940::Vector::InsertOverflow
// Flags: /O2 /Oy /W3 
// Leaf: InsertOverflow
#include <stdlib.h>
#include <new>
namespace Row00cbc940 {
// Layout: oracle uses a 12-byte STLport vector; element stride 12 with counted pointer at final dword.
struct String {void*p;String();String(const char*);String(const String&);~String();}; struct Ref{int refs;};struct Element{String text;unsigned a;Ref*ref;Element():a(0),ref(0){}__forceinline Element(const Element&v):text(v.text),a(v.a),ref(v.ref){if(ref)++ref->refs;}};
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