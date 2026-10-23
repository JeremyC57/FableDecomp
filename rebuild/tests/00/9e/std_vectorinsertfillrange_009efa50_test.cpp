// 009efa50 RetailVector::Overflow
// Flags: /O2 /Oy /W3
// Leaf: Overflow
// Layout: vector pointers +0,+4,+8; element stride and constructor fields from retail call and copy sequence.
#include <new>
namespace N009efa50 {

extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
inline void* __cdecl operator_new_place(unsigned int,void* p) {return p;}
struct FalseTag { FalseTag() {} };
struct StringBlock;struct String {StringBlock* Data;String();String(const String&);~String();String& operator=(const String&);};
struct Element {Element(){}virtual void dtor(unsigned int);unsigned int W0;String S;unsigned int A,B;unsigned char Flag;unsigned int C;__forceinline Element(const Element& o):W0(o.W0),S(o.S),A(o.A),B(o.B),Flag(o.Flag),C(o.C){}};
void* __cdecl Place(unsigned int,void*);
__forceinline void Construct(Element* p,const Element& x) {if(p) new(p) Element(x);}
__forceinline void Destroy(Element* p) {p->dtor(0);}
#pragma auto_inline(off)

__declspec(noinline) Element* __fastcall UninitCopy(Element* first,Element* last,Element* out,const FalseTag&) {for(;first!=last;++first,++out) Construct(out,*first);return out;}
__declspec(noinline) Element* __fastcall UninitFill(Element* out,unsigned int n,const Element& x,const FalseTag&) {for(;n>0;--n,++out) Construct(out,x);return out;}

#pragma auto_inline(on)
template<class T> __forceinline void DestroyStorage(T* first,T* last){for(;first!=last;++first)Destroy(first);}
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
 DestroyStorage(Start,Finish);
 if(Start) free(Start);
 Finish=new_finish; Start=new_start; End=new_start+len;
}
template void RetailVector<Element>::Overflow(Element*,const Element&,const FalseTag&,unsigned int,bool);

}

#include <cstdio>
#include <cstring>
#include <cstdlib>
namespace N009efa50 {

#pragma pack(push,1)
struct StringBlock {unsigned int Value;unsigned char Padding[9];unsigned int Refs;};
#pragma pack(pop)
static int liveStringBlocks,destroyedStringBlocks;
String::String():Data(0){}String::String(const String& o):Data(o.Data){if(Data)++Data->Refs;}
String::~String(){if(Data&&--Data->Refs==0){delete Data;--liveStringBlocks;++destroyedStringBlocks;}}
String& String::operator=(const String& o){if(Data!=o.Data){if(o.Data)++o.Data->Refs;if(Data&&--Data->Refs==0){delete Data;--liveStringBlocks;++destroyedStringBlocks;}Data=o.Data;}return *this;}
void SetString(String& s,unsigned int value){String t;t.Data=new StringBlock;std::memset(t.Data,0,sizeof *t.Data);t.Data->Value=value;t.Data->Refs=1;++liveStringBlocks;s=t;}

void Element::dtor(unsigned int flags){if(flags&1)delete this;else this->~Element();}
void Init(Element& e,unsigned int n){e.W0=n;e.A=n+100;e.B=n+200;e.C=n+300;e.Flag=(unsigned char)(n&1);SetString(e.S,n);}
}

namespace N009efa50 {
bool Cross255(){
 if(sizeof(StringBlock)!=17)return false;
 StringBlock probe;if((unsigned int)((char*)&probe.Refs-(char*)&probe)!=13)return false;
 int initialLive=liveStringBlocks,initialDestroyed=destroyedStringBlocks;
 { String root;SetString(root,123);String* copies=(String*)::operator new(300*sizeof(String));
 for(int i=0;i<300;++i)new(copies+i)String(root);
 if(root.Data->Refs!=301||liveStringBlocks!=initialLive+1)return false;
 copies[0]=root;if(root.Data->Refs!=301)return false;
 for(int j=0;j<45;++j)copies[j].~String();
 if(root.Data->Refs!=256||liveStringBlocks!=initialLive+1)return false;
 copies[45].~String();if(root.Data->Refs!=255||liveStringBlocks!=initialLive+1)return false;
 for(int k=46;k<300;++k)copies[k].~String();
 if(root.Data->Refs!=1||liveStringBlocks!=initialLive+1)return false;
 ::operator delete(copies);
 }
 return liveStringBlocks==initialLive&&destroyedStringBlocks==initialDestroyed+1;
}
}

int main(){using namespace N009efa50;if(!Cross255())return 100;{Element* a=(Element*)std::malloc(5*sizeof(Element));for(int i=0;i<4;++i){new(a+i)Element();Init(a[i],i+1);}Element x;Init(x,99);RetailVector<Element> v={a,a+4,a+5};v.Overflow(a+2,x,FalseTag(),2,false);unsigned int want[]={1,2,99,99,3,4};if(v.Finish-v.Start!=6||v.End-v.Start!=8)return 1;for(int j=0;j<6;++j)if(v.Start[j].W0!=want[j]||v.Start[j].A!=want[j]+100||v.Start[j].S.Data->Value!=want[j])return 2;if(x.S.Data->Refs!=3)return 3;v.Overflow(v.Finish,x,FalseTag(),1,true);if(v.Finish-v.Start!=7||v.Start[6].S.Data!=x.S.Data||x.S.Data->Refs!=4)return 4;for(Element* p=v.Start;p!=v.Finish;++p)p->~Element();std::free(v.Start);}if(liveStringBlocks!=0)return 5;std::printf("009efa50_TEST PASS\n");return 0;}