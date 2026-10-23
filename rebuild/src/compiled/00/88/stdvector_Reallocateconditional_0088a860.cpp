// 0088a860 RetailVector::FillInsert
// Flags: /O2 /Oy /W3
// Leaf: FillInsert
#include <stdlib.h>
#include <string.h>
#include <new>
namespace N0088a860 {
struct FalseTag{FalseTag(){}};struct RandomTag{RandomTag(){}};
// Layout: 8894a0 copies a word,76 words,30 words,then three pairs of12-byte vectors.
// Layout: 8899c0/889880 inner stride500;6b67a0 releases six inner buffers in reverse order.
// The propagated CFeatDef label conflicts with PDB size100; actual payload identity is uncertain.
struct UIntVector {
 unsigned*Start,*Finish,*End;
 __forceinline UIntVector():Start(0),Finish(0),End(0){}
 __forceinline UIntVector(const UIntVector&v){unsigned n=v.Start?unsigned(v.Finish-v.Start):0;Start=n?(unsigned*)malloc(n*4):0;Finish=Start;End=n?Start+n:0;if(n){memmove(Start,v.Start,n*4);Finish+=n;}}
 __forceinline ~UIntVector(){if(Start)free(Start);}
 __forceinline UIntVector&operator=(const UIntVector&v){if(this!=&v){unsigned n=v.Start?unsigned(v.Finish-v.Start):0;unsigned cap=Start?unsigned(End-Start):0;if(n>cap){unsigned*p=n?(unsigned*)malloc(n*4):0;if(Start)free(Start);Start=p;End=n?Start+n:0;}if(n)memmove(Start,v.Start,n*4);Finish=n?Start+n:Start;}return *this;}
};
struct Pair{UIntVector A,B;};
struct Inner500 {
 unsigned Key,WordsA[76],WordsB[30];Pair Arrays[3];
 __forceinline Inner500():Key(0){memset(WordsA,0,sizeof(WordsA));memset(WordsB,0,sizeof(WordsB));}
 __forceinline Inner500(const Inner500&v):Key(v.Key){memcpy(WordsA,v.WordsA,sizeof(WordsA));memcpy(WordsB,v.WordsB,sizeof(WordsB));for(unsigned i=0;i<3;++i){Arrays[i].A=v.Arrays[i].A;Arrays[i].B=v.Arrays[i].B;}}
 __forceinline Inner500&operator=(const Inner500&v){if(this!=&v){Key=v.Key;memcpy(WordsA,v.WordsA,sizeof(WordsA));memcpy(WordsB,v.WordsB,sizeof(WordsB));for(unsigned i=0;i<3;++i){Arrays[i].A=v.Arrays[i].A;Arrays[i].B=v.Arrays[i].B;}}return *this;}
};
template<class T> __declspec(noinline) void __fastcall DestroyRange(T*first,T*last,const FalseTag&){for(;first!=last;++first)first->~T();}
struct Buffer {
 Inner500*Start,*Finish,*End;
 __forceinline Buffer():Start(0),Finish(0),End(0){}
 __declspec(noinline) Buffer(const Buffer&v){unsigned n=v.Start?unsigned(v.Finish-v.Start):0;Start=0;Finish=0;End=0;Inner500*p=n?(Inner500*)malloc(n*sizeof(Inner500)):0;Start=p;Finish=p;End=n?p+n:0;for(Inner500*i=v.Start;i!=v.Finish;++i,++Finish)if(Finish)new(Finish)Inner500(*i);}
 __declspec(noinline) Buffer&operator=(const Buffer&v){if(this!=&v){unsigned n=v.Start?unsigned(v.Finish-v.Start):0;unsigned cap=Start?unsigned(End-Start):0;if(n>cap){Inner500*p=n?(Inner500*)malloc(n*sizeof(Inner500)):0;Inner500*q=p;for(Inner500*i=v.Start;i!=v.Finish;++i,++q)if(q)new(q)Inner500(*i);DestroyRange(Start,Finish,FalseTag());if(Start)free(Start);Start=p;Finish=q;End=n?p+n:0;}else{Inner500*i=v.Start;Inner500*q=Start;for(;i!=v.Finish&&q!=Finish;++i,++q)*q=*i;if(i==v.Finish){DestroyRange(q,Finish,FalseTag());Finish=q;}else{for(;i!=v.Finish;++i,++q)if(q)new(q)Inner500(*i);Finish=q;}}}return *this;}
 __forceinline ~Buffer(){Inner500*first=Start;DestroyRange(first,Finish,FalseTag());if(first)free(first);}
};
struct Element{Buffer B;};
__forceinline void Construct(Element*p,const Element&v){if(p)new(p)Element(v);}
#pragma auto_inline(off)
template<class T> __declspec(noinline) T* __fastcall UninitCopy(T* first,T* last,T* out,const FalseTag&) { for(;first!=last;++first,++out) Construct(out,*first); return out; }
template<class T> __declspec(noinline) T* __fastcall CopyBackward(T* first,T* last,T* out,const RandomTag&,int*) { for(int n=int(last-first);n>0;--n) *--out=*--last; return out; }
template<class T> __declspec(noinline) void __fastcall Fill(T* first,T* last,const T& x) { for(;first!=last;++first) *first=x; }
template<class T> __declspec(noinline) T* __fastcall UninitFill(T* first,unsigned int n,const T& x) { for(;n>0;--n,++first) Construct(first,x); return first; }
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