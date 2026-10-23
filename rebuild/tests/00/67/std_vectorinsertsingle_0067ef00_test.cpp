// 0067ef00 RetailVector::FillInsert
// Flags: /O2 /Oy /W3
// Leaf: FillInsert
// Layout: oracle pointer stride 28 bytes; vector offsets +0,+4,+8.
namespace N0067ef00 {

struct FalseTag { FalseTag() {} }; struct RandomTag { RandomTag() {} };
struct Element { unsigned int W0; unsigned int Data[6]; };
#pragma auto_inline(off)
template<class T> __declspec(noinline) T* __fastcall UninitCopy(T* first,T* last,T* out,const FalseTag&) { for(;first!=last;++first,++out) *out=*first; return out; }
template<class T> __declspec(noinline) T* __fastcall CopyBackward(T* first,T* last,T* out,const RandomTag&,int*) { for(int n=int(last-first);n>0;--n) *--out=*--last; return out; }
template<class T> __declspec(noinline) void __fastcall Fill(T* first,T* last,const T& x) { for(;first!=last;++first) *first=x; }
template<class T> __declspec(noinline) T* __fastcall UninitFill(T* first,unsigned int n,const T& x) { for(;n>0;--n,++first) *first=x; return first; }
#pragma auto_inline(on)
template<class T> __forceinline T* CopyBack(T* first,T* last,T* out,const FalseTag&) { return CopyBackward(first,last,out,RandomTag(),(int*)0); }
template<class T> __forceinline T* UninitializedFillN(T* first,unsigned int n,const T& x){return UninitFill(first,n,x);}
template<class T> struct RetailVector {
 T* Start; T* Finish; T* End;
 void Overflow(T* pos,const T& x,const FalseTag&,unsigned int n,bool atend=false);
 void FillInsert(T* pos,unsigned int n,const T& x);
};
template<class T> void RetailVector<T>::FillInsert(T* pos,unsigned int n,const T& x) {
 if(n!=0) {
  if((unsigned int)(End-Finish)>=n) {
   T x_copy=x;
   const unsigned int after=Finish-pos;
   T* old_finish=Finish;
   if(after>n) {
    UninitCopy(Finish-n,Finish,Finish,FalseTag());
    Finish+=n;
    CopyBack(pos,old_finish-n,old_finish,FalseTag());
    Fill(pos,pos+n,x_copy);
   } else {
    UninitializedFillN(Finish,n-after,x_copy);
    Finish+=n-after;
    UninitCopy(pos,old_finish,Finish,FalseTag());
    Finish+=after;
    Fill(pos,old_finish,x_copy);
   }
  } else Overflow(pos,x,FalseTag(),n);
 }
}
template void RetailVector<Element>::FillInsert(Element*,unsigned int,const Element&);

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
template<class T>void RetailVector<T>::Overflow(T* pos,const T& x,const FalseTag&,unsigned int n,bool atEnd){
 ++overflowCalls;unsigned old=(unsigned)(Finish-Start);unsigned prefix=(unsigned)(pos-Start);unsigned capacity=old+(old<n?n:old);T snapshot(x);
 T* buffer=capacity?(T*)allocate(capacity*sizeof(T)):0;T* out=buffer;
 for(T* p=Start;p!=pos;++p,++out)if(out)new(out)T(*p);
 if(n==1){if(out)new(out)T(snapshot);++out;}else for(unsigned i=0;i<n;++i,++out)if(out)new(out)T(snapshot);
 if(!atEnd)for(T* p=pos;p!=Finish;++p,++out)if(out)new(out)T(*p);
 for(T* p=Start;p!=Finish;++p)p->~T();release(Start);Start=buffer;Finish=out;End=buffer+capacity;
 lastOldSize=old;lastCapacity=capacity;lastNewSize=(unsigned)(Finish-Start);lastPosition=prefix;lastAtEnd=atEnd;
}
static RetailVector<Element> create(unsigned count,unsigned capacity){Element* raw=(Element*)allocate(sizeof(Element)*(capacity?capacity:1));for(unsigned i=0;i<count;++i)new(raw+i)Element(value(i+1));RetailVector<Element> v={raw,raw+count,raw+capacity};return v;}
static void destroy(RetailVector<Element>& v){for(Element* p=v.Start;p!=v.Finish;++p)p->~Element();release(v.Start);v.Start=v.Finish=v.End=0;}
static bool sequence(const RetailVector<Element>& v,const unsigned* keys,unsigned count){if((unsigned)(v.Finish-v.Start)!=count)return false;for(unsigned i=0;i<count;++i)if(!payload(v.Start[i],keys[i]))return false;return true;}
}
int main(){using namespace N0067ef00;if(sizeof(Element)!=28)return 1;Element x=value(99);
RetailVector<Element> a=create(5,12);a.FillInsert(a.Start+1,2,x);unsigned want[]={1,99,99,2,3,4,5};if(!sequence(a,want,7)||overflowCalls)return 2;
a.FillInsert(a.Start+2,1,a.Start[0]);unsigned alias[]={1,99,1,99,2,3,4,5};if(!sequence(a,alias,8)||overflowCalls)return 3;Element* unchangedStart=a.Start;Element* unchangedFinish=a.Finish;Element* unchangedEnd=a.End;a.FillInsert(a.Start,0,x);if(a.Start!=unchangedStart||a.Finish!=unchangedFinish||a.End!=unchangedEnd||!sequence(a,alias,8)||overflowCalls)return 4;destroy(a);
RetailVector<Element> w=create(3,7);w.FillInsert(w.Start+2,4,x);unsigned want2[]={1,2,99,99,99,99,3};if(!sequence(w,want2,7)||w.End!=w.Finish||overflowCalls)return 5;
unsigned oldAddress=(unsigned)w.Start;int freesBefore=freeCalls;w.FillInsert(w.Start,1,x);unsigned want3[]={99,1,2,99,99,99,99,3};if(!sequence(w,want3,8)||w.End-w.Start!=14||overflowCalls!=1||lastOldSize!=7||lastNewSize!=8||lastCapacity!=14||lastPosition||lastAtEnd||(unsigned)w.Start==oldAddress||freeCalls!=freesBefore+1)return 6;
/* Alias points into the old suffix; old storage must survive copying its value. */
w.FillInsert(w.Start+2,11,w.Start[7]);unsigned want4[19];want4[0]=99;want4[1]=1;for(int i=2;i<13;++i)want4[i]=3;unsigned suffix[]={2,99,99,99,99,3};for(int j=0;j<6;++j)want4[13+j]=suffix[j];if(!sequence(w,want4,19)||w.End-w.Start!=19||overflowCalls!=2||lastOldSize!=8||lastNewSize!=19||lastCapacity!=19||lastPosition!=2||lastAtEnd)return 7;
w.Overflow(w.Finish,x,FalseTag(),1,true);unsigned want5[20];for(int k=0;k<19;++k)want5[k]=want4[k];want5[19]=99;if(!sequence(w,want5,20)||w.End-w.Start!=38||overflowCalls!=3||!lastAtEnd||lastPosition!=19)return 8;destroy(w);
RetailVector<Element> empty=create(0,0);empty.FillInsert(empty.Start,2,x);unsigned wantEmpty[]={99,99};if(!sequence(empty,wantEmpty,2)||empty.End-empty.Start!=2||overflowCalls!=4||lastOldSize||lastNewSize!=2||lastCapacity!=2)return 9;destroy(empty);
if(liveAllocations||allocationCalls!=freeCalls)return 10;std::printf("0067ef00_TEST PASS\n");return 0;}