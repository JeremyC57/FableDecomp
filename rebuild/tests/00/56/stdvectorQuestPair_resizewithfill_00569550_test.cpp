// 00569550 RetailVector::FillInsert
// Flags: /O2 /Oy /W3
// Leaf: FillInsert
// Layout: oracle element stride 16 bytes and local copy/destruction call offsets; uncertain element identity.
namespace N00569550 {
struct FalseTag { FalseTag() {} }; struct RandomTag { RandomTag() {} };

extern "C" void __cdecl free(void*);
struct Buffer {
 unsigned int* Start; unsigned int* Finish; unsigned int* End;
 Buffer(); Buffer(const Buffer&); Buffer& operator=(const Buffer&);
 __forceinline ~Buffer() { if(Start) free(Start); }
};
struct StringBlock;
struct String {StringBlock* Data;String();String(const String&);~String();String& operator=(const String&);};
struct Element { String A,B,C; unsigned char Flag,Flag2; };
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

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
namespace N00569550 {

#pragma pack(push,1)
struct StringBlock {unsigned int Value;unsigned char Padding[9];unsigned int Refs;};
#pragma pack(pop)
static int liveStringBlocks,destroyedStringBlocks;
String::String():Data(0){}String::String(const String& o):Data(o.Data){if(Data)++Data->Refs;}
String::~String(){if(Data&&--Data->Refs==0){delete Data;--liveStringBlocks;++destroyedStringBlocks;}}
String& String::operator=(const String& o){if(Data!=o.Data){if(o.Data)++o.Data->Refs;if(Data&&--Data->Refs==0){delete Data;--liveStringBlocks;++destroyedStringBlocks;}Data=o.Data;}return *this;}
void SetString(String& s,unsigned int value){String t;t.Data=new StringBlock;std::memset(t.Data,0,sizeof *t.Data);t.Data->Value=value;t.Data->Refs=1;++liveStringBlocks;s=t;}

static int overflowCalls;
void Construct(Element* p,const Element& o){new(p)Element(o);}
template<class T>void RetailVector<T>::Overflow(T*,const T&,const FalseTag&,unsigned int,bool){++overflowCalls;}
void Init(Element& e,unsigned int n){SetString(e.A,n);SetString(e.B,n+100);SetString(e.C,n+200);e.Flag=(unsigned char)(n&1);e.Flag2=(unsigned char)((n+1)&1);}
unsigned int Key(const Element& e){return e.A.Data->Value;}
bool Payload(const Element& e,unsigned int n){return e.A.Data->Value==n&&e.B.Data->Value==n+100&&e.C.Data->Value==n+200&&e.Flag==(n&1)&&e.Flag2==((n+1)&1);}
}


namespace N00569550 {
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

int main(){using namespace N00569550;if(!Cross255())return 100;{Element* a=(Element*)std::malloc(12*sizeof(Element));for(int i=0;i<5;++i){new(a+i)Element();Init(a[i],i+1);}Element x;Init(x,99);RetailVector<Element> v={a,a+5,a+12};v.FillInsert(a+1,2,x);unsigned int want[]={1,99,99,2,3,4,5};for(int j=0;j<7;++j)if(!Payload(a[j],want[j]))return 1;if(a[1].A.Data!=x.A.Data||x.A.Data->Refs!=3)return 2;for(int k=0;k<7;++k)a[k].~Element();for(int q=0;q<3;++q){new(a+q)Element();Init(a[q],q+1);}v.Finish=a+3;v.FillInsert(a+2,4,x);if(v.Finish-a!=7||!Payload(a[6],3)||x.A.Data->Refs!=5)return 3;v.FillInsert(a+2,1,a[0]);if(v.Finish-a!=8||!Payload(a[2],1)||a[0].A.Data->Refs!=2)return 4;v.FillInsert(a,0,x);v.End=v.Finish;v.FillInsert(a,1,x);if(overflowCalls!=1)return 5;for(int z=0;z<8;++z)a[z].~Element();std::free(a);}if(liveStringBlocks!=0)return 6;std::printf("00569550_TEST PASS\n");return 0;}