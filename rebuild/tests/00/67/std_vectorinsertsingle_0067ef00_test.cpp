// 0067ef00 N0067ef00::vector<Element>::_M_fill_insert (STLport; catalog: vector_insert_single)
// Flags: /O2 /Oy /W3
// Leaf: _M_fill_insert
// Layout: oracle pointer stride 28 bytes; vector offsets +0,+4,+8.
namespace N0067ef00 {

struct __false_type { __false_type() {} }; struct random_access_iterator_tag { random_access_iterator_tag() {} };
struct Element { unsigned int W0; unsigned int Data[6]; };
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
#include <cstdlib>
#include <new>
namespace N0067ef00 {
static int overflowCalls,liveAllocations,allocationCalls,freeCalls;
static unsigned lastOldSize,lastCapacity,lastNewSize,lastPosition;static bool lastAtEnd;
static Element value(unsigned int key){Element e;e.W0=key;for(unsigned int i=0;i<6;++i)e.Data[i]=key*100+i+1;return e;}
static bool payload(const Element& e,unsigned int key){if(e.W0!=key)return false;for(unsigned int i=0;i<6;++i)if(e.Data[i]!=key*100+i+1)return false;return true;}
static void* allocate(unsigned bytes){void* p=std::malloc(bytes);if(p){++liveAllocations;++allocationCalls;}return p;}
static void release(void* p){if(p){std::free(p);--liveAllocations;++freeCalls;}}
template<class T>void vector<T>::_M_insert_overflow(T* pos,const T& x,const __false_type&,unsigned int n,bool atEnd){
 ++overflowCalls;unsigned old=(unsigned)(_M_finish-_M_start);unsigned prefix=(unsigned)(pos-_M_start);unsigned capacity=old+(old<n?n:old);T snapshot(x);
 T* buffer=capacity?(T*)allocate(capacity*sizeof(T)):0;T* out=buffer;
 for(T* p=_M_start;p!=pos;++p,++out)if(out)new(out)T(*p);
 if(n==1){if(out)new(out)T(snapshot);++out;}else for(unsigned i=0;i<n;++i,++out)if(out)new(out)T(snapshot);
 if(!atEnd)for(T* p=pos;p!=_M_finish;++p,++out)if(out)new(out)T(*p);
 for(T* p=_M_start;p!=_M_finish;++p)p->~T();release(_M_start);_M_start=buffer;_M_finish=out;_M_end_of_storage=buffer+capacity;
 lastOldSize=old;lastCapacity=capacity;lastNewSize=(unsigned)(_M_finish-_M_start);lastPosition=prefix;lastAtEnd=atEnd;
}
static vector<Element> create(unsigned count,unsigned capacity){Element* raw=(Element*)allocate(sizeof(Element)*(capacity?capacity:1));for(unsigned i=0;i<count;++i)new(raw+i)Element(value(i+1));vector<Element> v={raw,raw+count,raw+capacity};return v;}
static void destroy(vector<Element>& v){for(Element* p=v._M_start;p!=v._M_finish;++p)p->~Element();release(v._M_start);v._M_start=v._M_finish=v._M_end_of_storage=0;}
static bool sequence(const vector<Element>& v,const unsigned* keys,unsigned count){if((unsigned)(v._M_finish-v._M_start)!=count)return false;for(unsigned i=0;i<count;++i)if(!payload(v._M_start[i],keys[i]))return false;return true;}
}
int main(){using namespace N0067ef00;if(sizeof(Element)!=28)return 1;Element x=value(99);
vector<Element> a=create(5,12);a._M_fill_insert(a._M_start+1,2,x);unsigned want[]={1,99,99,2,3,4,5};if(!sequence(a,want,7)||overflowCalls)return 2;
a._M_fill_insert(a._M_start+2,1,a._M_start[0]);unsigned alias[]={1,99,1,99,2,3,4,5};if(!sequence(a,alias,8)||overflowCalls)return 3;Element* unchangedStart=a._M_start;Element* unchangedFinish=a._M_finish;Element* unchangedEnd=a._M_end_of_storage;a._M_fill_insert(a._M_start,0,x);if(a._M_start!=unchangedStart||a._M_finish!=unchangedFinish||a._M_end_of_storage!=unchangedEnd||!sequence(a,alias,8)||overflowCalls)return 4;destroy(a);
vector<Element> w=create(3,7);w._M_fill_insert(w._M_start+2,4,x);unsigned want2[]={1,2,99,99,99,99,3};if(!sequence(w,want2,7)||w._M_end_of_storage!=w._M_finish||overflowCalls)return 5;
unsigned oldAddress=(unsigned)w._M_start;int freesBefore=freeCalls;w._M_fill_insert(w._M_start,1,x);unsigned want3[]={99,1,2,99,99,99,99,3};if(!sequence(w,want3,8)||w._M_end_of_storage-w._M_start!=14||overflowCalls!=1||lastOldSize!=7||lastNewSize!=8||lastCapacity!=14||lastPosition||lastAtEnd||(unsigned)w._M_start==oldAddress||freeCalls!=freesBefore+1)return 6;
/* Alias points into the old suffix; old storage must survive copying its value. */
w._M_fill_insert(w._M_start+2,11,w._M_start[7]);unsigned want4[19];want4[0]=99;want4[1]=1;for(int i=2;i<13;++i)want4[i]=3;unsigned suffix[]={2,99,99,99,99,3};for(int j=0;j<6;++j)want4[13+j]=suffix[j];if(!sequence(w,want4,19)||w._M_end_of_storage-w._M_start!=19||overflowCalls!=2||lastOldSize!=8||lastNewSize!=19||lastCapacity!=19||lastPosition!=2||lastAtEnd)return 7;
w._M_insert_overflow(w._M_finish,x,__false_type(),1,true);unsigned want5[20];for(int k=0;k<19;++k)want5[k]=want4[k];want5[19]=99;if(!sequence(w,want5,20)||w._M_end_of_storage-w._M_start!=38||overflowCalls!=3||!lastAtEnd||lastPosition!=19)return 8;destroy(w);
vector<Element> empty=create(0,0);empty._M_fill_insert(empty._M_start,2,x);unsigned wantEmpty[]={99,99};if(!sequence(empty,wantEmpty,2)||empty._M_end_of_storage-empty._M_start!=2||overflowCalls!=4||lastOldSize||lastNewSize!=2||lastCapacity!=2)return 9;destroy(empty);
if(liveAllocations||allocationCalls!=freeCalls)return 10;std::printf("0067ef00_TEST PASS\n");return 0;}