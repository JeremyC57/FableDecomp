// 0085ebb0 RetailVector::FillInsert
// Flags: /O2 /Oy /W3
// Leaf: FillInsert
// Layout: oracle element stride 12 bytes and local copy/destruction call offsets; uncertain element identity.
#include <new>
extern "C" void* __cdecl malloc(unsigned int);
namespace N0085ebb0 {
struct FalseTag { FalseTag() {} }; struct RandomTag { RandomTag() {} };

extern "C" void __cdecl free(void*);
struct Buffer {
 unsigned int* Start; unsigned int* Finish; unsigned int* End;
 Buffer(); Buffer(const Buffer&); Buffer& operator=(const Buffer&);
 __forceinline ~Buffer() { if(Start) free(Start); }
};
struct String { unsigned int W0; String(); String(const String&); ~String(); String& operator=(const String&); };
struct Element { Buffer B; unsigned int Key() const {return B.Start ? *B.Start : 0;} };
void Construct(Element*,const Element&);
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
 template<class Tag> __declspec(noinline) void Overflow(T* pos,const T& x,const Tag&,unsigned int n,bool atend=false);
 void FillInsert(T* pos,unsigned int n,const T& x);
};
template<class T>template<class Tag> __declspec(noinline) void RetailVector<T>::Overflow(T* pos,const T& x,const Tag&,unsigned int n,bool atend){
 unsigned int old_size=(unsigned int)(Finish-Start);unsigned int grow=old_size<n?n:old_size;unsigned int len=old_size+grow;T* start=len?(T*)malloc(sizeof(T)*len):0;T* out=start;
 for(T* p=Start;p!=pos;++p,++out)if(out)new(out)T(*p);
 for(unsigned int i=0;i<n;++i,++out)if(out)new(out)T(x);
 if(!atend)for(T* p=pos;p!=Finish;++p,++out)if(out)new(out)T(*p);
 for(T* p=Start;p!=Finish;++p)p->~T();if(Start)free(Start);Start=start;Finish=out;End=start+len;
}
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
namespace N0085ebb0 {
static int overflowCalls;
String::String(){W0=0;} String::String(const String& o) { W0=o.W0; } String::~String() {} String& String::operator=(const String& o) { W0=o.W0; return *this; }
Buffer::Buffer(){Start=0;Finish=0;End=0;}
Buffer::Buffer(const Buffer& o) { unsigned int n=o.Start?(unsigned int)(o.Finish-o.Start):0; Start=n?(unsigned int*)std::malloc(n*4):0; Finish=n?Start+n:0; End=Finish; if(n) std::memcpy(Start,o.Start,n*4); }
Buffer& Buffer::operator=(const Buffer& o) { if(this!=&o) {if(Start) std::free(Start); unsigned int n=o.Start?(unsigned int)(o.Finish-o.Start):0; Start=n?(unsigned int*)std::malloc(n*4):0; Finish=n?Start+n:0; End=Finish; if(n) std::memcpy(Start,o.Start,n*4);} return *this; }

void Construct(Element* p,const Element& o) { new(p) Element(o); }

void SetKey(Element& e,unsigned int value) { e.B.Start=(unsigned int*)std::malloc(8);e.B.Start[0]=value;e.B.Start[1]=value+100;e.B.Finish=e.B.Start+2;e.B.End=e.B.Finish; }
unsigned int Key(const Element& e) { return *e.B.Start; }
}
int main() { using namespace N0085ebb0;
 Element* a=(Element*)std::malloc(sizeof(Element)*12);
 for(int i=0;i<5;++i) {new(a+i)Element();SetKey(a[i],i+1);}
 Element x;SetKey(x,99);
 RetailVector<Element> v={a,a+5,a+12}; v.FillInsert(a+1,2,x);
 unsigned int want[]={1,99,99,2,3,4,5}; if(v.Finish-a!=7) return 1; for(int j=0;j<7;++j) if(Key(a[j])!=want[j]) return 2;
 if(a[1].B.Start==x.B.Start||a[1].B.Start[1]!=199||a[2].B.Start==a[1].B.Start)return 7;
 for(int k=0;k<7;++k) a[k].~Element(); for(int u=0;u<3;++u) {new(a+u)Element();SetKey(a[u],u+1);}
 v.Finish=a+3; v.FillInsert(a+2,4,x); unsigned int want2[]={1,2,99,99,99,99,3}; if(v.Finish-a!=7) return 3; for(int q=0;q<7;++q) if(Key(a[q])!=want2[q]) return 4;
 v.FillInsert(a+2,1,a[0]);if(v.Finish-a!=8||Key(a[2])!=1)return 5;
 v.FillInsert(a,0,x);v.End=v.Finish;v.FillInsert(v.Finish,1,x);if(v.Finish-v.Start!=9||Key(v.Start[8])!=99)return 6;a=v.Start;
 for(int z=0;z<9;++z) a[z].~Element();std::free(a);
 std::printf("0085ebb0_TEST PASS\n"); return 0;
}