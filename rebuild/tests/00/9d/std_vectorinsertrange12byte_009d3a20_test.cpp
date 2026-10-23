// 009d3a20 N009d3a20::vector<Element>::_M_fill_insert (STLport; catalog: vector_insert_range_12byte)
// Flags: /O2 /Oy /W3
// Leaf: _M_fill_insert
// Layout: oracle pointer stride 12 bytes; vector offsets +0,+4,+8.
namespace N009d3a20 {

struct __false_type { __false_type() {} }; struct random_access_iterator_tag { random_access_iterator_tag() {} };
struct Element { unsigned int W0; unsigned int W1; unsigned int W2; };
#pragma auto_inline(off)
template<class T> __declspec(noinline) T* __fastcall __uninitialized_copy(T* first,T* last,T* out,const __false_type&) { for(;first!=last;++first,++out) *out=*first; return out; }
template<class T> __declspec(noinline) T* __fastcall __copy_backward(T* first,T* last,T* out,const random_access_iterator_tag&,int*) { for(int n=int(last-first);n>0;--n) *--out=*--last; return out; }
template<class T> __declspec(noinline) void __fastcall fill(T* first,T* last,const T& x) { for(;first!=last;++first) *first=x; }
template<class T> __declspec(noinline) T* __fastcall __uninitialized_fill_n(T* first,unsigned int n,const T& x) { for(;n>0;--n,++first) *first=x; return first; }
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

#include <cstdio>
#include <cstring>
namespace N009d3a20 {
static int overflowCalls;
template<class T> void vector<T>::_M_insert_overflow(T*,const T&,const __false_type&,unsigned int,bool) { ++overflowCalls; }
}
int main() {
 using namespace N009d3a20;
 Element a[12]; std::memset(a,0,sizeof a); for(int i=0;i<5;++i) a[i].W0=i+1;
 Element x; std::memset(&x,0,sizeof x); x.W0=99;
 vector<Element> v={a,a+5,a+12};
 v._M_fill_insert(a+1,2,x);
 unsigned int want[]={1,99,99,2,3,4,5};
 if(v._M_finish-a!=7) return 1; for(int j=0;j<7;++j) if(a[j].W0!=want[j]) return 2;
 Element b[12]; std::memset(b,0,sizeof b); for(int k=0;k<3;++k) b[k].W0=k+1;
 vector<Element> w={b,b+3,b+12}; w._M_fill_insert(b+2,4,x);
 unsigned int want2[]={1,2,99,99,99,99,3};
 if(w._M_finish-b!=7) return 3; for(int q=0;q<7;++q) if(b[q].W0!=want2[q]) return 4;
 w._M_fill_insert(b,0,x); if(w._M_finish-b!=7) return 5;
 w._M_end_of_storage=w._M_finish; w._M_fill_insert(b,1,x); if(overflowCalls!=1||w._M_finish-b!=7) return 6;
 std::printf("009d3a20_TEST PASS\n"); return 0;
}