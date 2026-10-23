// 0085e570 RetailVector::FillInsert
// Flags: /O2 /Oy /W3
// Leaf: FillInsert
// Layout:85db50/85d7a0 null-check destination then construct by five wordcopies; model actual object lifetimes with placementconstruction and explicit20-byte copy.
// Layout: oracle pointer stride 20 bytes; vector offsets +0,+4,+8.
#include <new>
namespace N0085e570 {

struct FalseTag { FalseTag() {} }; struct RandomTag { RandomTag() {} };
struct Element { unsigned int W0; unsigned int W1; unsigned int W2; unsigned int W3; unsigned int W4; Element(){} __forceinline Element(const Element& x){W0=x.W0;W1=x.W1;W2=x.W2;W3=x.W3;W4=x.W4;} };
#pragma auto_inline(off)
template<class T> __declspec(noinline) T* __fastcall UninitCopy(T* first,T* last,T* out,const FalseTag&) { for(;first!=last;++first,++out) new(out) T(*first); return out; }
template<class T> __declspec(noinline) T* __fastcall CopyBackward(T* first,T* last,T* out,const RandomTag&,int*) { for(int n=int(last-first);n>0;--n) *--out=*--last; return out; }
template<class T> __declspec(noinline) void __fastcall Fill(T* first,T* last,const T& x) { for(;first!=last;++first) *first=x; }
template<class T> __declspec(noinline) T* __fastcall UninitFill(T* first,unsigned int n,const T& x,const FalseTag&) { for(;n>0;--n,++first) new(first) T(x); return first; }
#pragma auto_inline(on)
template<class T> __forceinline T* CopyBack(T* first,T* last,T* out,const FalseTag&) { return CopyBackward(first,last,out,RandomTag(),(int*)0); }
template<class T> __forceinline T* UninitializedFillN(T* first,unsigned int n,const T& x){return UninitFill(first,n,x,FalseTag());}
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
namespace N0085e570 {
static int overflowCalls,allocations;
static Element make(unsigned int id){Element e;e.W0=id;e.W1=id+100;e.W2=id+200;e.W3=id+300;e.W4=id+400;return e;}
static bool same(const Element& a,const Element& b){return a.W0==b.W0&&a.W1==b.W1&&a.W2==b.W2&&a.W3==b.W3&&a.W4==b.W4;}
static bool fields(const Element& e){return e.W1==e.W0+100&&e.W2==e.W0+200&&e.W3==e.W0+300&&e.W4==e.W0+400;}
template<class T> void RetailVector<T>::Overflow(T* pos,const T& x,const FalseTag&,unsigned int n,bool){++overflowCalls;T snapshot(x);unsigned int before=(unsigned int)(pos-Start),size=(unsigned int)(Finish-Start),cap=size+n+4;T* storage=(T*)::operator new(cap*sizeof(T));++allocations;unsigned int at=0;for(unsigned int i=0;i<before;++i)new(storage+at++)T(Start[i]);for(unsigned int j=0;j<n;++j)new(storage+at++)T(snapshot);for(unsigned int k=before;k<size;++k)new(storage+at++)T(Start[k]);for(T* p=Start;p!=Finish;++p)p->~T();::operator delete(Start);--allocations;Start=storage;Finish=storage+at;End=storage+cap;}
static RetailVector<Element> create(unsigned int size,unsigned int capacity){Element* p=(Element*)::operator new(sizeof(Element)*capacity);++allocations;for(unsigned int i=0;i<size;++i)new(p+i)Element(make(i+1));RetailVector<Element> v={p,p+size,p+capacity};return v;}
static void destroy(RetailVector<Element>& v){for(Element* p=v.Start;p!=v.Finish;++p)p->~Element();::operator delete(v.Start);--allocations;v.Start=v.Finish=v.End=0;}
static bool sequence(const RetailVector<Element>& v,const unsigned int* keys,unsigned int n){if((unsigned int)(v.Finish-v.Start)!=n)return false;for(unsigned int i=0;i<n;++i)if(v.Start[i].W0!=keys[i]||!fields(v.Start[i]))return false;return true;}
}
int main(){using namespace N0085e570;Element x=make(99);RetailVector<Element> v=create(5,12);v.FillInsert(v.Start+1,2,x);unsigned int want[]={1,99,99,2,3,4,5};if(!sequence(v,want,7))return 1;v.FillInsert(v.Start,0,v.Start[1]);if(!sequence(v,want,7))return 2;destroy(v);RetailVector<Element> w=create(3,12);w.FillInsert(w.Start+2,4,w.Start[2]);unsigned int want2[]={1,2,3,3,3,3,3};if(!sequence(w,want2,7))return 3;w.End=w.Finish;w.FillInsert(w.Start+1,2,w.Start[0]);unsigned int want3[]={1,1,1,2,3,3,3,3,3};if(overflowCalls!=1||!sequence(w,want3,9))return 4;destroy(w);RetailVector<Element> empty=create(0,4);empty.FillInsert(empty.Start,2,x);unsigned int want4[]={99,99};if(!sequence(empty,want4,2)||!same(empty.Start[0],x))return 5;destroy(empty);if(allocations)return 6;std::printf("0085e570_TEST PASS\n");return 0;}