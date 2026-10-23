// 00ad7e80 RetailVector::FillInsert
// Flags: /O2 /Oy /W3
// Leaf: FillInsert
// Layout: record flag0, set<long> A+4 and B+16; set Head0/Count4/Compare8 (12 bytes). Callee00ad3a10 allocates20-byte sentinel and copies tree;00ad4a70 destroys both sets.
namespace N00ad7e80 {
struct FalseTag { FalseTag() {} }; struct RandomTag { RandomTag() {} };

extern "C" void __cdecl free(void*);
struct TreeNode {unsigned char Colour;TreeNode* Parent;TreeNode* Left;TreeNode* Right;int Key;};
struct RetailSet {TreeNode* Head;unsigned int Count;unsigned char Compare;
 RetailSet();RetailSet(const RetailSet&);RetailSet& operator=(const RetailSet&);~RetailSet();
 void Insert(int);bool Contains(int)const;
};
struct String { unsigned int W0; String(); String(const String&); ~String(); String& operator=(const String&); };
struct Element { unsigned char Flag; RetailSet A,B; ~Element(); };
void Construct(Element*,const Element&);
#pragma auto_inline(off)
template<class T> __declspec(noinline) T* __fastcall UninitCopy(T* first,T* last,T* out,const FalseTag&) { for(;first!=last;++first,++out) Construct(out,*first); return out; }
template<class T> __declspec(noinline) T* __fastcall CopyBackward(T* first,T* last,T* out,const RandomTag&,int*) { for(int n=int(last-first);n>0;--n) *--out=*--last; return out; }
template<class T> __declspec(noinline) void __fastcall Fill(T* first,T* last,const T& x) { for(;first!=last;++first) *first=x; }
template<class T> __declspec(noinline) T* __fastcall UninitFill(T* first,unsigned int n,const T& x,const FalseTag&) { for(;n>0;--n,++first) Construct(first,x); return first; }
#pragma auto_inline(on)
template<class T> __forceinline T* CopyBack(T* first,T* last,T* out,const FalseTag&) { return CopyBackward(first,last,out,RandomTag(),(int*)0); }
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
    UninitFill(Finish,n-after,x_copy,FalseTag());
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