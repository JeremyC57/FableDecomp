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

#include <assert.h>
#include <stdio.h>
namespace N0088a860 {
template<class T>void RetailVector<T>::Overflow(T*pos,const T&value,const FalseTag&,unsigned n,bool atend){
 (void)atend;unsigned old=Start?unsigned(Finish-Start):0;unsigned where=Start?unsigned(pos-Start):0;
 unsigned cap=old+(old<n?n:old);T*next=(T*)malloc(cap*sizeof(T));assert(next);
 T*out=next;for(unsigned i=0;i<where;++i,++out)new(out)T(Start[i]);
 for(unsigned j=0;j<n;++j,++out)new(out)T(value);
 for(unsigned k=where;k<old;++k,++out)new(out)T(Start[k]);
 for(T*p=Start;p!=Finish;++p)p->~T();if(Start)free(Start);Start=next;Finish=out;End=next+cap;
}
void FillVector(UIntVector&v,unsigned key){v.Start=(unsigned*)malloc(8);assert(v.Start);v.Start[0]=key;v.Start[1]=key+1000;v.Finish=v.Start+2;v.End=v.Finish;}
void SetKey(Element&e,unsigned key){e.B.Start=(Inner500*)malloc(2*sizeof(Inner500));assert(e.B.Start);e.B.Finish=e.B.Start;e.B.End=e.B.Start+2;for(unsigned i=0;i<2;++i,++e.B.Finish){new(e.B.Finish)Inner500();Inner500&v=*e.B.Finish;v.Key=key+i*100;for(unsigned a=0;a<76;++a)v.WordsA[a]=v.Key+a;for(unsigned b=0;b<30;++b)v.WordsB[b]=v.Key+200+b;for(unsigned j=0;j<3;++j){FillVector(v.Arrays[j].A,v.Key+j*10);FillVector(v.Arrays[j].B,v.Key+j*10+5);}}}
unsigned Key(const Element&e){return e.B.Start[0].Key;}
void Check(const Element&e,unsigned key){assert(e.B.Finish-e.B.Start==2);for(unsigned i=0;i<2;++i){const Inner500&v=e.B.Start[i];unsigned expected=key+i*100;assert(v.Key==expected);for(unsigned a=0;a<76;++a)assert(v.WordsA[a]==expected+a);for(unsigned b=0;b<30;++b)assert(v.WordsB[b]==expected+200+b);for(unsigned j=0;j<3;++j){const UIntVector&x=v.Arrays[j].A;const UIntVector&y=v.Arrays[j].B;assert(x.Finish-x.Start==2&&y.Finish-y.Start==2);assert(x.Start[0]==expected+j*10&&x.Start[1]==expected+j*10+1000);assert(y.Start[0]==expected+j*10+5&&y.Start[1]==expected+j*10+1005);}}}
void Distinct(const Element&a,const Element&b){assert(a.B.Start!=b.B.Start);for(unsigned i=0;i<2;++i)for(unsigned j=0;j<3;++j){assert(a.B.Start[i].Arrays[j].A.Start!=b.B.Start[i].Arrays[j].A.Start);assert(a.B.Start[i].Arrays[j].B.Start!=b.B.Start[i].Arrays[j].B.Start);}}
}
int main(){using namespace N0088a860;assert(sizeof(Inner500)==500&&sizeof(Buffer)==12&&sizeof(Element)==12);
 Element*a=(Element*)malloc(12*sizeof(Element));assert(a);for(unsigned i=0;i<5;++i){new(a+i)Element();SetKey(a[i],i+1);}
 Element x;SetKey(x,99);RetailVector<Element>v={a,a+5,a+12};v.FillInsert(a+1,2,x);
 unsigned want[]={1,99,99,2,3,4,5};assert(v.Finish-a==7);for(unsigned j=0;j<7;++j)Check(a[j],want[j]);Distinct(a[1],x);Distinct(a[1],a[2]);
 for(unsigned k=0;k<7;++k)a[k].~Element();for(unsigned m=0;m<3;++m){new(a+m)Element();SetKey(a[m],m+1);}v.Finish=a+3;v.FillInsert(a+2,4,x);
 unsigned want2[]={1,2,99,99,99,99,3};assert(v.Finish-a==7);for(unsigned q=0;q<7;++q)Check(a[q],want2[q]);
 v.FillInsert(a+2,1,a[0]);assert(v.Finish-a==8);Check(a[2],1);Distinct(a[0],a[2]);
 v.FillInsert(a,0,x);assert(v.Finish-a==8);v.End=v.Finish;v.FillInsert(a+1,3,x);
 unsigned want3[]={1,99,99,99,2,1,99,99,99,99,3};assert(v.Finish-v.Start==11);for(unsigned r=0;r<11;++r)Check(v.Start[r],want3[r]);Distinct(v.Start[1],x);
 for(Element*p=v.Start;p!=v.Finish;++p)p->~Element();free(v.Start);
 Buffer empty;Buffer copy(empty);assert(copy.Start==0&&copy.Finish==0&&copy.End==0);Element copyElement;copyElement.B=x.B;Check(copyElement,99);copyElement.B=copyElement.B;Check(copyElement,99);
 puts("0088a860_TEST PASS");return 0;
}