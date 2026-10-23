// 005e31f0 Row005e31f0::vector<Element>::_M_insert_overflow (STLport; catalog: CTargetPointVector_ReserveAndCopy, but element is 20 bytes, CTargetPoint is 28)
// Flags: /O2 /Oy /W3 
// Leaf: _M_insert_overflow
#include <stdlib.h>
#include <new>
namespace Row005e31f0 {
// Layout: oracle uses a 12-byte STLport vector; element stride 20 with counted pointer at final dword.
struct Ref {unsigned vtable;int refs;}; struct Element {unsigned a,b,c,d;Ref*ref;Element():a(0),b(0),c(0),d(0),ref(0){}__forceinline Element(const Element&v):a(v.a),b(v.b),c(v.c),d(v.d),ref(v.ref){if(ref)++ref->refs;}};
struct __false_type {__false_type(){}};
Element* __fastcall __uninitialized_copy(Element*,Element*,Element*,const bool&);
Element* __fastcall __uninitialized_fill_n(Element*,unsigned,const Element&,const bool&);
void __fastcall _Destroy(Element*,Element*,const bool&);
__forceinline const unsigned& max(const unsigned&a,const unsigned&b){return a<b?b:a;}
template<class T> struct vector {T*_M_start,*_M_finish,*_M_end_of_storage;void _M_insert_overflow(T*,const T&,const __false_type&,unsigned,bool);};
template<class T> void vector<T>::_M_insert_overflow(T*pos,const T&value,const __false_type&,unsigned n,bool atEnd) {
 unsigned old=_M_finish-_M_start; unsigned capacity=old+max(old,n);
 T* buffer=capacity?(T*)malloc(capacity*sizeof(T)):0;
 T* finish=__uninitialized_copy(_M_start,pos,buffer,atEnd);
 if(n==1){if(finish)new(finish)T(value);++finish;}
 else finish=__uninitialized_fill_n(finish,n,value,atEnd);
 if(!atEnd)finish=__uninitialized_copy(pos,_M_finish,finish,atEnd);
 _Destroy(_M_start,_M_finish,atEnd);if(_M_start)free(_M_start);
 _M_start=buffer;_M_finish=finish;_M_end_of_storage=buffer+capacity;
}
template struct vector<Element>;
}