// 0085ebb0 N0085ebb0::vector<Element>::_M_fill_insert (STLport; catalog: Resize_with_fill)
// Flags: /O2 /Oy /W3
// Leaf: _M_fill_insert
// Layout: oracle element stride 12 bytes and local copy/destruction call offsets; uncertain element identity.
#include <new>
extern "C" void* __cdecl malloc(unsigned int);
namespace N0085ebb0 {
struct __false_type { __false_type() {} }; struct random_access_iterator_tag { random_access_iterator_tag() {} };

extern "C" void __cdecl free(void*);
struct Buffer {
 unsigned int* _M_start; unsigned int* _M_finish; unsigned int* _M_end_of_storage;
 Buffer(); Buffer(const Buffer&); Buffer& operator=(const Buffer&);
 __forceinline ~Buffer() { if(_M_start) free(_M_start); }
};
struct String { unsigned int W0; String(); String(const String&); ~String(); String& operator=(const String&); };
struct Element { Buffer B; unsigned int Key() const {return B._M_start ? *B._M_start : 0;} };
void _Construct(Element*,const Element&);
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

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
namespace N0085ebb0 {
static int overflowCalls;
String::String(){W0=0;} String::String(const String& o) { W0=o.W0; } String::~String() {} String& String::operator=(const String& o) { W0=o.W0; return *this; }
Buffer::Buffer(){_M_start=0;_M_finish=0;_M_end_of_storage=0;}
Buffer::Buffer(const Buffer& o) { unsigned int n=o._M_start?(unsigned int)(o._M_finish-o._M_start):0; _M_start=n?(unsigned int*)std::malloc(n*4):0; _M_finish=n?_M_start+n:0; _M_end_of_storage=_M_finish; if(n) std::memcpy(_M_start,o._M_start,n*4); }
Buffer& Buffer::operator=(const Buffer& o) { if(this!=&o) {if(_M_start) std::free(_M_start); unsigned int n=o._M_start?(unsigned int)(o._M_finish-o._M_start):0; _M_start=n?(unsigned int*)std::malloc(n*4):0; _M_finish=n?_M_start+n:0; _M_end_of_storage=_M_finish; if(n) std::memcpy(_M_start,o._M_start,n*4);} return *this; }

void _Construct(Element* p,const Element& o) { new(p) Element(o); }

void SetKey(Element& e,unsigned int value) { e.B._M_start=(unsigned int*)std::malloc(8);e.B._M_start[0]=value;e.B._M_start[1]=value+100;e.B._M_finish=e.B._M_start+2;e.B._M_end_of_storage=e.B._M_finish; }
unsigned int Key(const Element& e) { return *e.B._M_start; }
}
int main() { using namespace N0085ebb0;
 Element* a=(Element*)std::malloc(sizeof(Element)*12);
 for(int i=0;i<5;++i) {new(a+i)Element();SetKey(a[i],i+1);}
 Element x;SetKey(x,99);
 vector<Element> v={a,a+5,a+12}; v._M_fill_insert(a+1,2,x);
 unsigned int want[]={1,99,99,2,3,4,5}; if(v._M_finish-a!=7) return 1; for(int j=0;j<7;++j) if(Key(a[j])!=want[j]) return 2;
 if(a[1].B._M_start==x.B._M_start||a[1].B._M_start[1]!=199||a[2].B._M_start==a[1].B._M_start)return 7;
 for(int k=0;k<7;++k) a[k].~Element(); for(int u=0;u<3;++u) {new(a+u)Element();SetKey(a[u],u+1);}
 v._M_finish=a+3; v._M_fill_insert(a+2,4,x); unsigned int want2[]={1,2,99,99,99,99,3}; if(v._M_finish-a!=7) return 3; for(int q=0;q<7;++q) if(Key(a[q])!=want2[q]) return 4;
 v._M_fill_insert(a+2,1,a[0]);if(v._M_finish-a!=8||Key(a[2])!=1)return 5;
 v._M_fill_insert(a,0,x);v._M_end_of_storage=v._M_finish;v._M_fill_insert(v._M_finish,1,x);if(v._M_finish-v._M_start!=9||Key(v._M_start[8])!=99)return 6;a=v._M_start;
 for(int z=0;z<9;++z) a[z].~Element();std::free(a);
 std::printf("0085ebb0_TEST PASS\n"); return 0;
}