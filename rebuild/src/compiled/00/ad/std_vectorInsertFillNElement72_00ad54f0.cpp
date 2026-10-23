// 00ad54f0 N00ad54f0::vector<Element>::_M_fill_insert (STLport; catalog: vector_InsertFillN_Element72)
// Flags: /O2 /Oy /W3
// Leaf: _M_fill_insert
// Layout: oracle element stride 72 bytes and local copy/destruction call offsets; uncertain element identity.
#include <new>
extern "C" void* __cdecl malloc(unsigned int);
namespace N00ad54f0 {
struct __false_type { __false_type() {} }; struct random_access_iterator_tag { random_access_iterator_tag() {} };

extern "C" void __cdecl free(void*);
struct Buffer {
 unsigned int* _M_start; unsigned int* _M_finish; unsigned int* _M_end_of_storage;
 Buffer(); Buffer(const Buffer&); Buffer& operator=(const Buffer&);
 __forceinline ~Buffer() { if(_M_start) free(_M_start); }
};
struct String { unsigned int W0; String(); String(const String&); ~String(); String& operator=(const String&); };
struct Element { unsigned int W0; unsigned int Data[17]; Element(); Element(const Element&); Element& operator=(const Element&); };
void _Construct(Element*,const Element&);
#pragma auto_inline(off)
template<class T> __declspec(noinline) T* __fastcall __uninitialized_copy(T* first,T* last,T* out,const __false_type&) { for(;first!=last;++first,++out) _Construct(out,*first); return out; }
template<class T> __declspec(noinline) T* __fastcall __copy_backward(T* first,T* last,T* out,const random_access_iterator_tag&) { for(int n=int(last-first);n>0;--n) *--out=*--last; return out; }
template<class T> __declspec(noinline) void __fastcall fill(T* first,T* last,const T& x) { for(;first!=last;++first) *first=x; }
template<class T> __declspec(noinline) T* __fastcall __uninitialized_fill_n(T* first,unsigned int n,const T& x) { for(;n>0;--n,++first) _Construct(first,x); return first; }
#pragma auto_inline(on)
template<class T> __forceinline T* __copy_backward_ptrs(T* first,T* last,T* out,const __false_type&) { return __copy_backward(first,last,out,random_access_iterator_tag()); }
template<class T> __forceinline T* UninitializedFillN(T* first,unsigned int n,const T& x){return __uninitialized_fill_n(first,n,x);}
template<class T> struct vector {
 T* _M_start; T* _M_finish; T* _M_end_of_storage;
 template<class Tag> __declspec(noinline) void _M_insert_overflow(T* pos,const T& x,const Tag&,unsigned int n,bool atend=false);
 void _M_fill_insert(T* pos,unsigned int n,const T& x);
};
template<class T>template<class Tag> __declspec(noinline) void vector<T>::_M_insert_overflow(T* pos,const T& x,const Tag&,unsigned int n,bool atend){
 unsigned int old_size=(unsigned int)(_M_finish-_M_start);unsigned int grow=old_size<n?n:old_size;unsigned int len=old_size+grow;T* start=len?(T*)malloc(sizeof(T)*len):0;T* out=start;
 for(T* p=_M_start;p!=pos;++p,++out)if(out)new(out)T(*p);
 for(unsigned int i=0;i<n;++i,++out)if(out)new(out)T(x);
 if(!atend)for(T* p=pos;p!=_M_finish;++p,++out)if(out)new(out)T(*p);
 for(T* p=_M_start;p!=_M_finish;++p)p->~T();if(_M_start)free(_M_start);_M_start=start;_M_finish=out;_M_end_of_storage=start+len;
}
template<class T> void vector<T>::_M_fill_insert(T* pos,unsigned int n,const T& x) {
 if(n!=0) {
  if((unsigned int)(_M_end_of_storage-_M_finish)>=n) {
   T x_copy=x;
   const unsigned int after=_M_finish-pos;
   T* old_finish=_M_finish;
   if(after>n) {
    __uninitialized_copy(_M_finish-n,_M_finish,_M_finish,__false_type());
    _M_finish+=n;
    __copy_backward_ptrs(pos,old_finish-n,old_finish,__false_type());
    fill(pos,pos+n,x_copy);
   } else {
    UninitializedFillN(_M_finish,n-after,x_copy);
    _M_finish+=n-after;
    __uninitialized_copy(pos,old_finish,_M_finish,__false_type());
    _M_finish+=after;
    fill(pos,old_finish,x_copy);
   }
  } else _M_insert_overflow(pos,x,__false_type(),n);
 }
}
template void vector<Element>::_M_fill_insert(Element*,unsigned int,const Element&);

}