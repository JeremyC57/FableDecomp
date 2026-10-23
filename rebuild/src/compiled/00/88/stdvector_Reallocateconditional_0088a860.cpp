// 0088a860 N0088a860::vector<Element>::_M_fill_insert (STLport; catalog: _Reallocate_conditional)
// Flags: /O2 /Oy /W3
// Leaf: _M_fill_insert
#include <stdlib.h>
#include <string.h>
#include <new>
namespace N0088a860 {
struct __false_type{__false_type(){}};struct random_access_iterator_tag{random_access_iterator_tag(){}};
// Layout: 8894a0 copies a word,76 words,30 words,then three pairs of12-byte vectors.
// Layout: 8899c0/889880 inner stride500;6b67a0 releases six inner buffers in reverse order.
// The propagated CFeatDef label conflicts with PDB size100; actual payload identity is uncertain.
struct UIntVector {
 unsigned*_M_start,*_M_finish,*_M_end_of_storage;
 __forceinline UIntVector():_M_start(0),_M_finish(0),_M_end_of_storage(0){}
 __forceinline UIntVector(const UIntVector&v){unsigned n=v._M_start?unsigned(v._M_finish-v._M_start):0;_M_start=n?(unsigned*)malloc(n*4):0;_M_finish=_M_start;_M_end_of_storage=n?_M_start+n:0;if(n){memmove(_M_start,v._M_start,n*4);_M_finish+=n;}}
 __forceinline ~UIntVector(){if(_M_start)free(_M_start);}
 __forceinline UIntVector&operator=(const UIntVector&v){if(this!=&v){unsigned n=v._M_start?unsigned(v._M_finish-v._M_start):0;unsigned cap=_M_start?unsigned(_M_end_of_storage-_M_start):0;if(n>cap){unsigned*p=n?(unsigned*)malloc(n*4):0;if(_M_start)free(_M_start);_M_start=p;_M_end_of_storage=n?_M_start+n:0;}if(n)memmove(_M_start,v._M_start,n*4);_M_finish=n?_M_start+n:_M_start;}return *this;}
};
struct Pair{UIntVector A,B;};
struct Inner500 {
 unsigned Key,WordsA[76],WordsB[30];Pair Arrays[3];
 __forceinline Inner500():Key(0){memset(WordsA,0,sizeof(WordsA));memset(WordsB,0,sizeof(WordsB));}
 __forceinline Inner500(const Inner500&v):Key(v.Key){memcpy(WordsA,v.WordsA,sizeof(WordsA));memcpy(WordsB,v.WordsB,sizeof(WordsB));for(unsigned i=0;i<3;++i){Arrays[i].A=v.Arrays[i].A;Arrays[i].B=v.Arrays[i].B;}}
 __forceinline Inner500&operator=(const Inner500&v){if(this!=&v){Key=v.Key;memcpy(WordsA,v.WordsA,sizeof(WordsA));memcpy(WordsB,v.WordsB,sizeof(WordsB));for(unsigned i=0;i<3;++i){Arrays[i].A=v.Arrays[i].A;Arrays[i].B=v.Arrays[i].B;}}return *this;}
};
template<class T> __declspec(noinline) void __fastcall DestroyRange(T*first,T*last,const __false_type&){for(;first!=last;++first)first->~T();}
struct Buffer {
 Inner500*_M_start,*_M_finish,*_M_end_of_storage;
 __forceinline Buffer():_M_start(0),_M_finish(0),_M_end_of_storage(0){}
 __declspec(noinline) Buffer(const Buffer&v){unsigned n=v._M_start?unsigned(v._M_finish-v._M_start):0;_M_start=0;_M_finish=0;_M_end_of_storage=0;Inner500*p=n?(Inner500*)malloc(n*sizeof(Inner500)):0;_M_start=p;_M_finish=p;_M_end_of_storage=n?p+n:0;for(Inner500*i=v._M_start;i!=v._M_finish;++i,++_M_finish)if(_M_finish)new(_M_finish)Inner500(*i);}
 __declspec(noinline) Buffer&operator=(const Buffer&v){if(this!=&v){unsigned n=v._M_start?unsigned(v._M_finish-v._M_start):0;unsigned cap=_M_start?unsigned(_M_end_of_storage-_M_start):0;if(n>cap){Inner500*p=n?(Inner500*)malloc(n*sizeof(Inner500)):0;Inner500*q=p;for(Inner500*i=v._M_start;i!=v._M_finish;++i,++q)if(q)new(q)Inner500(*i);DestroyRange(_M_start,_M_finish,__false_type());if(_M_start)free(_M_start);_M_start=p;_M_finish=q;_M_end_of_storage=n?p+n:0;}else{Inner500*i=v._M_start;Inner500*q=_M_start;for(;i!=v._M_finish&&q!=_M_finish;++i,++q)*q=*i;if(i==v._M_finish){DestroyRange(q,_M_finish,__false_type());_M_finish=q;}else{for(;i!=v._M_finish;++i,++q)if(q)new(q)Inner500(*i);_M_finish=q;}}}return *this;}
 __forceinline ~Buffer(){Inner500*first=_M_start;DestroyRange(first,_M_finish,__false_type());if(first)free(first);}
};
struct Element{Buffer B;};
__forceinline void _Construct(Element*p,const Element&v){if(p)new(p)Element(v);}
#pragma auto_inline(off)
template<class T> __declspec(noinline) T* __fastcall __uninitialized_copy(T* first,T* last,T* out,const __false_type&) { for(;first!=last;++first,++out) _Construct(out,*first); return out; }
template<class T> __declspec(noinline) T* __fastcall __copy_backward(T* first,T* last,T* out,const random_access_iterator_tag&,int*) { for(int n=int(last-first);n>0;--n) *--out=*--last; return out; }
template<class T> __declspec(noinline) void __fastcall fill(T* first,T* last,const T& x) { for(;first!=last;++first) *first=x; }
template<class T> __declspec(noinline) T* __fastcall __uninitialized_fill_n(T* first,unsigned int n,const T& x) { for(;n>0;--n,++first) _Construct(first,x); return first; }
#pragma auto_inline(on)
template<class T> __forceinline T* __copy_backward_ptrs(T* first,T* last,T* out,const __false_type&) { return __copy_backward(first,last,out,random_access_iterator_tag(),(int*)0); }
template<class T> __forceinline T* UninitializedFillN(T* first,unsigned int n,const T& x){return __uninitialized_fill_n(first,n,x);}
template<class T> struct vector {
 T* _M_start; T* _M_finish; T* _M_end_of_storage;
 void _M_insert_overflow(T* pos,const T& x,const __false_type&,unsigned int n,bool atend=false);
 void _M_fill_insert(T* pos,unsigned int n,const T& x);
};
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