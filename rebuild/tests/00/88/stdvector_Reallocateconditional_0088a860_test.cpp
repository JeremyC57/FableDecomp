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

#include <assert.h>
#include <stdio.h>
namespace N0088a860 {
template<class T>void vector<T>::_M_insert_overflow(T*pos,const T&value,const __false_type&,unsigned n,bool atend){
 (void)atend;unsigned old=_M_start?unsigned(_M_finish-_M_start):0;unsigned where=_M_start?unsigned(pos-_M_start):0;
 unsigned cap=old+(old<n?n:old);T*next=(T*)malloc(cap*sizeof(T));assert(next);
 T*out=next;for(unsigned i=0;i<where;++i,++out)new(out)T(_M_start[i]);
 for(unsigned j=0;j<n;++j,++out)new(out)T(value);
 for(unsigned k=where;k<old;++k,++out)new(out)T(_M_start[k]);
 for(T*p=_M_start;p!=_M_finish;++p)p->~T();if(_M_start)free(_M_start);_M_start=next;_M_finish=out;_M_end_of_storage=next+cap;
}
void FillVector(UIntVector&v,unsigned key){v._M_start=(unsigned*)malloc(8);assert(v._M_start);v._M_start[0]=key;v._M_start[1]=key+1000;v._M_finish=v._M_start+2;v._M_end_of_storage=v._M_finish;}
void SetKey(Element&e,unsigned key){e.B._M_start=(Inner500*)malloc(2*sizeof(Inner500));assert(e.B._M_start);e.B._M_finish=e.B._M_start;e.B._M_end_of_storage=e.B._M_start+2;for(unsigned i=0;i<2;++i,++e.B._M_finish){new(e.B._M_finish)Inner500();Inner500&v=*e.B._M_finish;v.Key=key+i*100;for(unsigned a=0;a<76;++a)v.WordsA[a]=v.Key+a;for(unsigned b=0;b<30;++b)v.WordsB[b]=v.Key+200+b;for(unsigned j=0;j<3;++j){FillVector(v.Arrays[j].A,v.Key+j*10);FillVector(v.Arrays[j].B,v.Key+j*10+5);}}}
unsigned Key(const Element&e){return e.B._M_start[0].Key;}
void Check(const Element&e,unsigned key){assert(e.B._M_finish-e.B._M_start==2);for(unsigned i=0;i<2;++i){const Inner500&v=e.B._M_start[i];unsigned expected=key+i*100;assert(v.Key==expected);for(unsigned a=0;a<76;++a)assert(v.WordsA[a]==expected+a);for(unsigned b=0;b<30;++b)assert(v.WordsB[b]==expected+200+b);for(unsigned j=0;j<3;++j){const UIntVector&x=v.Arrays[j].A;const UIntVector&y=v.Arrays[j].B;assert(x._M_finish-x._M_start==2&&y._M_finish-y._M_start==2);assert(x._M_start[0]==expected+j*10&&x._M_start[1]==expected+j*10+1000);assert(y._M_start[0]==expected+j*10+5&&y._M_start[1]==expected+j*10+1005);}}}
void Distinct(const Element&a,const Element&b){assert(a.B._M_start!=b.B._M_start);for(unsigned i=0;i<2;++i)for(unsigned j=0;j<3;++j){assert(a.B._M_start[i].Arrays[j].A._M_start!=b.B._M_start[i].Arrays[j].A._M_start);assert(a.B._M_start[i].Arrays[j].B._M_start!=b.B._M_start[i].Arrays[j].B._M_start);}}
}
int main(){using namespace N0088a860;assert(sizeof(Inner500)==500&&sizeof(Buffer)==12&&sizeof(Element)==12);
 Element*a=(Element*)malloc(12*sizeof(Element));assert(a);for(unsigned i=0;i<5;++i){new(a+i)Element();SetKey(a[i],i+1);}
 Element x;SetKey(x,99);vector<Element>v={a,a+5,a+12};v._M_fill_insert(a+1,2,x);
 unsigned want[]={1,99,99,2,3,4,5};assert(v._M_finish-a==7);for(unsigned j=0;j<7;++j)Check(a[j],want[j]);Distinct(a[1],x);Distinct(a[1],a[2]);
 for(unsigned k=0;k<7;++k)a[k].~Element();for(unsigned m=0;m<3;++m){new(a+m)Element();SetKey(a[m],m+1);}v._M_finish=a+3;v._M_fill_insert(a+2,4,x);
 unsigned want2[]={1,2,99,99,99,99,3};assert(v._M_finish-a==7);for(unsigned q=0;q<7;++q)Check(a[q],want2[q]);
 v._M_fill_insert(a+2,1,a[0]);assert(v._M_finish-a==8);Check(a[2],1);Distinct(a[0],a[2]);
 v._M_fill_insert(a,0,x);assert(v._M_finish-a==8);v._M_end_of_storage=v._M_finish;v._M_fill_insert(a+1,3,x);
 unsigned want3[]={1,99,99,99,2,1,99,99,99,99,3};assert(v._M_finish-v._M_start==11);for(unsigned r=0;r<11;++r)Check(v._M_start[r],want3[r]);Distinct(v._M_start[1],x);
 for(Element*p=v._M_start;p!=v._M_finish;++p)p->~Element();free(v._M_start);
 Buffer empty;Buffer copy(empty);assert(copy._M_start==0&&copy._M_finish==0&&copy._M_end_of_storage==0);Element copyElement;copyElement.B=x.B;Check(copyElement,99);copyElement.B=copyElement.B;Check(copyElement,99);
 puts("0088a860_TEST PASS");return 0;
}