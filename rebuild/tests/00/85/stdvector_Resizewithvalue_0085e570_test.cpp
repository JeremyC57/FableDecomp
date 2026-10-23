// 0085e570 N0085e570::vector<Element>::_M_fill_insert (STLport; catalog: Resize_with_value)
// Flags: /O2 /Oy /W3
// Leaf: _M_fill_insert
// Layout:85db50/85d7a0 null-check destination then construct by five wordcopies; model actual object lifetimes with placementconstruction and explicit20-byte copy.
// Layout: oracle pointer stride 20 bytes; vector offsets +0,+4,+8.
#include <new>
namespace N0085e570 {

struct __false_type { __false_type() {} }; struct random_access_iterator_tag { random_access_iterator_tag() {} };
struct Element { unsigned int W0; unsigned int W1; unsigned int W2; unsigned int W3; unsigned int W4; Element(){} __forceinline Element(const Element& x){W0=x.W0;W1=x.W1;W2=x.W2;W3=x.W3;W4=x.W4;} };
#pragma auto_inline(off)
template<class T> __declspec(noinline) T* __fastcall __uninitialized_copy(T* first,T* last,T* out,const __false_type&) { for(;first!=last;++first,++out) new(out) T(*first); return out; }
template<class T> __declspec(noinline) T* __fastcall __copy_backward(T* first,T* last,T* out,const random_access_iterator_tag&,int*) { for(int n=int(last-first);n>0;--n) *--out=*--last; return out; }
template<class T> __declspec(noinline) void __fastcall fill(T* first,T* last,const T& x) { for(;first!=last;++first) *first=x; }
template<class T> __declspec(noinline) T* __fastcall __uninitialized_fill_n(T* first,unsigned int n,const T& x,const __false_type&) { for(;n>0;--n,++first) new(first) T(x); return first; }
#pragma auto_inline(on)
template<class T> __forceinline T* __copy_backward_ptrs(T* first,T* last,T* out,const __false_type&) { return __copy_backward(first,last,out,random_access_iterator_tag(),(int*)0); }
template<class T> __forceinline T* UninitializedFillN(T* first,unsigned int n,const T& x){return __uninitialized_fill_n(first,n,x,__false_type());}
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
namespace N0085e570 {
static int overflowCalls,allocations;
static Element make(unsigned int id){Element e;e.W0=id;e.W1=id+100;e.W2=id+200;e.W3=id+300;e.W4=id+400;return e;}
static bool same(const Element& a,const Element& b){return a.W0==b.W0&&a.W1==b.W1&&a.W2==b.W2&&a.W3==b.W3&&a.W4==b.W4;}
static bool fields(const Element& e){return e.W1==e.W0+100&&e.W2==e.W0+200&&e.W3==e.W0+300&&e.W4==e.W0+400;}
template<class T> void vector<T>::_M_insert_overflow(T* pos,const T& x,const __false_type&,unsigned int n,bool){++overflowCalls;T snapshot(x);unsigned int before=(unsigned int)(pos-_M_start),size=(unsigned int)(_M_finish-_M_start),cap=size+n+4;T* storage=(T*)::operator new(cap*sizeof(T));++allocations;unsigned int at=0;for(unsigned int i=0;i<before;++i)new(storage+at++)T(_M_start[i]);for(unsigned int j=0;j<n;++j)new(storage+at++)T(snapshot);for(unsigned int k=before;k<size;++k)new(storage+at++)T(_M_start[k]);for(T* p=_M_start;p!=_M_finish;++p)p->~T();::operator delete(_M_start);--allocations;_M_start=storage;_M_finish=storage+at;_M_end_of_storage=storage+cap;}
static vector<Element> create(unsigned int size,unsigned int capacity){Element* p=(Element*)::operator new(sizeof(Element)*capacity);++allocations;for(unsigned int i=0;i<size;++i)new(p+i)Element(make(i+1));vector<Element> v={p,p+size,p+capacity};return v;}
static void destroy(vector<Element>& v){for(Element* p=v._M_start;p!=v._M_finish;++p)p->~Element();::operator delete(v._M_start);--allocations;v._M_start=v._M_finish=v._M_end_of_storage=0;}
static bool sequence(const vector<Element>& v,const unsigned int* keys,unsigned int n){if((unsigned int)(v._M_finish-v._M_start)!=n)return false;for(unsigned int i=0;i<n;++i)if(v._M_start[i].W0!=keys[i]||!fields(v._M_start[i]))return false;return true;}
}
int main(){using namespace N0085e570;Element x=make(99);vector<Element> v=create(5,12);v._M_fill_insert(v._M_start+1,2,x);unsigned int want[]={1,99,99,2,3,4,5};if(!sequence(v,want,7))return 1;v._M_fill_insert(v._M_start,0,v._M_start[1]);if(!sequence(v,want,7))return 2;destroy(v);vector<Element> w=create(3,12);w._M_fill_insert(w._M_start+2,4,w._M_start[2]);unsigned int want2[]={1,2,3,3,3,3,3};if(!sequence(w,want2,7))return 3;w._M_end_of_storage=w._M_finish;w._M_fill_insert(w._M_start+1,2,w._M_start[0]);unsigned int want3[]={1,1,1,2,3,3,3,3,3};if(overflowCalls!=1||!sequence(w,want3,9))return 4;destroy(w);vector<Element> empty=create(0,4);empty._M_fill_insert(empty._M_start,2,x);unsigned int want4[]={99,99};if(!sequence(empty,want4,2)||!same(empty._M_start[0],x))return 5;destroy(empty);if(allocations)return 6;std::printf("0085e570_TEST PASS\n");return 0;}