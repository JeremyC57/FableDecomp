// 0086e5c0 RetailVector::FillInsert
// Flags: /O2 /Oy /W3
// Leaf: FillInsert
// Layout: oracle element stride 24 bytes and local copy/destruction call offsets; uncertain element identity.
namespace N0086e5c0 {
struct FalseTag { FalseTag() {} }; struct RandomTag { RandomTag() {} };

extern "C" void __cdecl free(void*);
struct Payload12 {unsigned int A,B,C;};
struct Buffer {
 Payload12* Start;Payload12* Finish;Payload12* End;
 Buffer(); Buffer(const Buffer&); Buffer& operator=(const Buffer&);
 __forceinline ~Buffer() { if(Start) free(Start); }
};
struct String { unsigned int W0; String(); String(const String&); ~String(); String& operator=(const String&); };
struct EmptyAllocator {EmptyAllocator(){}};
struct BufferWithFlag:Buffer {EmptyAllocator Allocator;unsigned char Flag;};
struct Element {unsigned int W0,W1;BufferWithFlag B;};
void Construct(Element*,const Element&);
#pragma auto_inline(off)
template<class T> __declspec(noinline) T* __fastcall UninitCopy(T* first,T* last,T* out,const FalseTag&) { for(;first!=last;++first,++out) Construct(out,*first); return out; }
template<class T> __declspec(noinline) T* __fastcall CopyBackward(T* first,T* last,T* out,const RandomTag&,int*) { for(int n=int(last-first);n>0;--n) *--out=*--last; return out; }
template<class T> __declspec(noinline) void __fastcall Fill(T* first,T* last,const T& x) { for(;first!=last;++first) *first=x; }
template<class T> __declspec(noinline) T* __fastcall UninitFill(T* first,unsigned int n,const T& x,const FalseTag&) { for(;n>0;--n,++first) Construct(first,x); return first; }
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