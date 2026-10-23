// 00a97f70 RetailVector::FillInsert
// Flags: /O2 /Oy /W3
// Leaf: FillInsert
// Layout: counted pointer data/control at0/4 and 8-byte stride; a95890 uses ecx/edx plus one stack input and ret4. Tagged copy helpers remain distinct.
#include <new>
namespace N00a97f70 {
struct FalseTag { FalseTag() {} }; struct RandomTag { RandomTag() {} };
struct RefBlock { unsigned int Refs; void (__fastcall *Destroy)(void*); void* Data; };
struct Base { virtual void v00(); ~Base(); };
template<class T> struct Counted {
 T* Data; RefBlock* Block;
 __forceinline Counted():Data(0),Block(0) {}
 __forceinline Counted(const Counted& o):Data(o.Data),Block(o.Block) {if(Block) ++Block->Refs;}
 __forceinline ~Counted() { Release(); Data=0;Block=0; }
 __forceinline void Release() { RefBlock* block=Block;if(block) {if(--block->Refs==0) {block->Destroy(block->Data); ::operator delete(block);}} }
 __forceinline Counted& operator=(const Counted& o) {T* data=o.Data; RefBlock* block=o.Block; if(Block!=block){Release();Data=data;Block=block;if(block)++block->Refs;}return *this;}
};
template<class T> struct PolyCounted:Base {
 T* Data;RefBlock* Block;
 __forceinline PolyCounted():Data(0),Block(0) {}
 __forceinline PolyCounted(const PolyCounted& o):Data(o.Data),Block(o.Block) {if(Block)++Block->Refs;}
 __forceinline ~PolyCounted(){if(Block){--Block->Refs;if(Block->Refs==0){Block->Destroy(Block->Data);::operator delete(Block);}}Data=0;Block=0;}
 __forceinline PolyCounted& operator=(const PolyCounted& o){T* data=o.Data;RefBlock* block=o.Block;if(Block!=block){if(Block){--Block->Refs;if(Block->Refs==0){Block->Destroy(Block->Data);::operator delete(Block);}}Data=data;Block=block;if(block)++block->Refs;}return *this;}
};
struct String {unsigned int Key; String();String(const String&);~String();String& operator=(const String&);bool FastCompare(const String&) const;};
struct WideString {unsigned int Key; WideString();WideString(const WideString&);~WideString();WideString& operator=(const WideString&);};
struct Element {Counted<void> P;};
__forceinline void Construct(Element* p,const Element& x){if(p)new(p)Element(x);}
#pragma auto_inline(off)
template<class T> __declspec(noinline) T* __fastcall UninitCopy(T* first,T* last,T* out,const FalseTag&) { for(;first!=last;++first,++out) Construct(out,*first); return out; }
template<class T> __declspec(noinline) T* __fastcall CopyBackward(T* first,T* last,T* out,const RandomTag&,int*) { for(int n=int(last-first);n>0;--n) *--out=*--last; return out; }
template<class T> __declspec(noinline) void __fastcall Fill(T* first,T* last,const T& x) { for(;first!=last;++first) *first=x; }
template<class T> __declspec(noinline) T* __fastcall UninitFill(T* first,unsigned int n,const T& x) { for(;n>0;--n,++first) Construct(first,x); return first; }
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
    UninitFill(Finish,n-after,x_copy);
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
#include <cstdlib>
namespace N00a97f70 {
void Base::v00(){} Base::~Base(){}
static int destroyed,liveControls,overflowCalls;
void __fastcall Dispose(void* p){delete (int*)p;++destroyed;--liveControls;}
void SetPointer(Element& e,int id){e.P.Data=new int(id);e.P.Block=new RefBlock;e.P.Block->Refs=1;e.P.Block->Destroy=Dispose;e.P.Block->Data=e.P.Data;++liveControls;}

template<class T>void RetailVector<T>::Overflow(T* pos,const T& x,const FalseTag&,unsigned int n,bool){
 ++overflowCalls;T copy(x);unsigned int oldSize=(unsigned int)(Finish-Start),offset=(unsigned int)(pos-Start);unsigned int extra=oldSize>n?oldSize:n;
 T* fresh=(T*)std::malloc((oldSize+extra)*sizeof(T));T* end=fresh;T* p;
 for(p=Start;p!=pos;++p,++end)new(end)T(*p);
 for(unsigned int i=0;i<n;++i,++end)new(end)T(copy);
 for(p=pos;p!=Finish;++p,++end)new(end)T(*p);
 for(p=Start;p!=Finish;++p)p->~T();std::free(Start);Start=fresh;Finish=end;End=fresh+oldSize+extra;
 if((unsigned int)(Finish-Start)!=oldSize+n||offset>oldSize)++overflowCalls;
}
struct Owner {RetailVector<Element> V;Owner(){V.Start=(Element*)std::malloc(7*sizeof(Element));V.Finish=V.Start;V.End=V.Start+7;}~Owner(){for(Element* p=V.Start;p!=V.Finish;++p)p->~Element();std::free(V.Start);}};
int ID(const Element& e){return *(int*)e.P.Data;}
}
int main(){using namespace N00a97f70;
 {Counted<void> first,second;first.Data=new int(41);first.Block=new RefBlock;first.Block->Refs=1;first.Block->Destroy=Dispose;first.Block->Data=first.Data;++liveControls;
 second.Data=new int(42);second.Block=new RefBlock;second.Block->Refs=1;second.Block->Destroy=Dispose;second.Block->Data=second.Data;++liveControls;
 first=second;if(destroyed!=1||second.Block->Refs!=2)return 1;first=first;second=first;if(second.Block->Refs!=2)return 2;}
 if(destroyed!=2||liveControls!=0)return 3;
 {Element probe;SetPointer(probe,77);Construct(0,probe);if(probe.P.Block->Refs!=1||liveControls!=1||destroyed!=2)return 11;Element* copy=(Element*)::operator new(sizeof(Element));Construct(copy,probe);if(copy->P.Data!=probe.P.Data||copy->P.Block!=probe.P.Block||probe.P.Block->Refs!=2)return 12;copy->~Element();::operator delete(copy);if(probe.P.Block->Refs!=1||destroyed!=2)return 13;}
 if(destroyed!=3||liveControls)return 14;
 {Owner owner;RetailVector<Element>& v=owner.V;for(int i=0;i<3;++i){new(v.Finish)Element;SetPointer(*v.Finish,i+1);++v.Finish;}RefBlock* shared=v.Start[1].P.Block;
 v.FillInsert(v.Start,1,v.Start[1]);if(v.Finish-v.Start!=4||shared->Refs!=2||ID(v.Start[0])!=2||ID(v.Start[1])!=1||ID(v.Start[2])!=2||ID(v.Start[3])!=3)return 4;
 v.FillInsert(v.Start+3,3,v.Start[0]);if(v.Finish-v.Start!=7||shared->Refs!=5||ID(v.Start[6])!=3)return 5;
 for(int i=2;i<6;++i)if(ID(v.Start[i])!=2)return 6;
 v.FillInsert(v.Start,0,v.Start[0]);if(shared->Refs!=5||overflowCalls!=0)return 7;
 v.FillInsert(v.Start+2,2,v.Start[0]);if(v.Finish-v.Start!=9||shared->Refs!=7||overflowCalls!=1||ID(v.Start[1])!=1||ID(v.Start[8])!=3)return 8;
 for(int i=2;i<8;++i)if(ID(v.Start[i])!=2)return 9;
 }
 if(destroyed!=6||liveControls!=0)return 10;
 std::printf("00a97f70_TEST PASS\n");return 0;}