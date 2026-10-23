// 009d3a20 RetailVector::FillInsert
// Flags: /O2 /Oy /W3
// Leaf: FillInsert
// Layout: oracle pointer stride 12 bytes; vector offsets +0,+4,+8.
namespace N009d3a20 {

struct FalseTag { FalseTag() {} }; struct RandomTag { RandomTag() {} };
struct Element { unsigned int W0; unsigned int W1; unsigned int W2; };
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
namespace N009d3a20 {
static int overflowCalls;
template<class T> void RetailVector<T>::Overflow(T*,const T&,const FalseTag&,unsigned int,bool) { ++overflowCalls; }
}
int main() {
 using namespace N009d3a20;
 Element a[12]; std::memset(a,0,sizeof a); for(int i=0;i<5;++i) a[i].W0=i+1;
 Element x; std::memset(&x,0,sizeof x); x.W0=99;
 RetailVector<Element> v={a,a+5,a+12};
 v.FillInsert(a+1,2,x);
 unsigned int want[]={1,99,99,2,3,4,5};
 if(v.Finish-a!=7) return 1; for(int j=0;j<7;++j) if(a[j].W0!=want[j]) return 2;
 Element b[12]; std::memset(b,0,sizeof b); for(int k=0;k<3;++k) b[k].W0=k+1;
 RetailVector<Element> w={b,b+3,b+12}; w.FillInsert(b+2,4,x);
 unsigned int want2[]={1,2,99,99,99,99,3};
 if(w.Finish-b!=7) return 3; for(int q=0;q<7;++q) if(b[q].W0!=want2[q]) return 4;
 w.FillInsert(b,0,x); if(w.Finish-b!=7) return 5;
 w.End=w.Finish; w.FillInsert(b,1,x); if(overflowCalls!=1||w.Finish-b!=7) return 6;
 std::printf("009d3a20_TEST PASS\n"); return 0;
}