// 00618420 RetailVector::Overflow
// Flags: /O2 /Oy /W3
// Leaf: Overflow
// Layout: vector pointers +0,+4,+8; element stride and constructor fields from retail call and copy sequence.
namespace N00618420 {

extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
inline void* __cdecl operator_new_place(unsigned int,void* p) {return p;}
struct FalseTag { FalseTag() {} };
struct Element { unsigned int W0; unsigned int Data[9]; };
void* __cdecl Place(unsigned int,void*);
__forceinline void Construct(Element* p,const Element& x) { if(p) *p=x; }
__forceinline void Destroy(Element*) {}

__forceinline Element* __fastcall UninitCopy(Element* first,Element* last,Element* out,const FalseTag&) {for(;first!=last;++first,++out) Construct(out,*first);return out;}
__forceinline Element* __fastcall UninitFill(Element* out,unsigned int n,const Element& x,const FalseTag&) {for(;n>0;--n,++out) Construct(out,x);return out;}

template<class T> struct RetailVector { T* Start; T* Finish; T* End; void Overflow(T*,const T&,const FalseTag&,unsigned int,bool); };
template<class T> __forceinline const T& Max(const T& a,const T& b) {return a<b?b:a;}
template<class T> void RetailVector<T>::Overflow(T* pos,const T& x,const FalseTag&,unsigned int n,bool atend) {
 const unsigned int old_size=Finish-Start;
 const unsigned int len=old_size+Max(old_size,n);
 T* new_start=len ? (T*)malloc(len*sizeof(T)) : 0;
 T* new_finish=UninitCopy(Start,pos,new_start,FalseTag());
 if(n==1) { Construct(new_finish,x); ++new_finish; }
 else new_finish=UninitFill(new_finish,n,x,FalseTag());
 if(!atend) new_finish=UninitCopy(pos,Finish,new_finish,FalseTag());
 for(T* p=Start;p!=Finish;++p) Destroy(p);
 if(Start) free(Start);
 Finish=new_finish; Start=new_start; End=new_start+len;
}
template void RetailVector<Element>::Overflow(Element*,const Element&,const FalseTag&,unsigned int,bool);

}

#include <cstdio>
#include <cstring>
#include <cstdlib>
namespace N00618420 {  }
int main() {using namespace N00618420;
 Element* a=(Element*)std::malloc(5*sizeof(Element)); std::memset(a,0,5*sizeof(Element)); for(int i=0;i<4;++i) a[i].W0=i+1;
 Element* x=(Element*)std::malloc(sizeof(Element)); std::memset(x,0,sizeof(Element)); x->W0=99;
 RetailVector<Element> v={a,a+4,a+5}; v.Overflow(a+2,*x,FalseTag(),2,false);
 unsigned int want[]={1,2,99,99,3,4}; if(v.Finish-v.Start!=6 || v.End-v.Start!=8) return 1;
 for(int j=0;j<6;++j) if(v.Start[j].W0!=want[j]) return 2;
 v.Overflow(v.Finish,*x,FalseTag(),1,true); if(v.Finish-v.Start!=7||v.Start[6].W0!=99)return 3;
 std::free(v.Start);std::free(x); std::printf("00618420_TEST PASS\n");return 0;
}