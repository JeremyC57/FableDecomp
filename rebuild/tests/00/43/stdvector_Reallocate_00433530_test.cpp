// 00433530 RetailVector::Overflow
// Flags: /O2 /Oy /W3
// Leaf: Overflow
// Layout: vector pointers +0,+4,+8; element stride and constructor fields from retail call and copy sequence.
#include <new>
namespace N00433530 {

extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
inline void* __cdecl operator_new_place(unsigned int,void* p) {return p;}
struct FalseTag { FalseTag() {} };
struct StringBlock;struct Element {StringBlock* Data;Element();Element(const Element&);~Element();Element& operator=(const Element&);};
__forceinline void CopyConstruct(Element* p,const Element& x) { new(p) Element(x); }
void* __cdecl Place(unsigned int,void*);
__forceinline void Construct(Element* p,const Element& x) { if(p) CopyConstruct(p,x); }
__forceinline void Destroy(Element* p) {p->~Element();}

__forceinline Element* __fastcall UninitCopy(Element* first,Element* last,Element* out,const FalseTag&) {for(;first!=last;++first,++out) Construct(out,*first);return out;}
__forceinline Element* __fastcall UninitFill(Element* out,unsigned int n,const Element& x,const FalseTag&) {for(;n>0;--n,++out) Construct(out,x);return out;}

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
namespace N00433530 {

#pragma pack(push,1)
struct StringBlock {unsigned int Value;unsigned char Padding[9];unsigned int Refs;};
#pragma pack(pop)
static int liveStringBlocks,destroyedStringBlocks;
Element::Element():Data(0){}Element::Element(const Element& o):Data(o.Data){if(Data)++Data->Refs;}
Element::~Element(){if(Data&&--Data->Refs==0){delete Data;--liveStringBlocks;++destroyedStringBlocks;}}
Element& Element::operator=(const Element& o){if(Data!=o.Data){if(o.Data)++o.Data->Refs;if(Data&&--Data->Refs==0){delete Data;--liveStringBlocks;++destroyedStringBlocks;}Data=o.Data;}return *this;}
void SetElement(Element& s,unsigned int value){Element t;t.Data=new StringBlock;std::memset(t.Data,0,sizeof *t.Data);t.Data->Value=value;t.Data->Refs=1;++liveStringBlocks;s=t;}

}


namespace N00433530 {
bool Cross255(){
 if(sizeof(StringBlock)!=17)return false;
 StringBlock probe;if((unsigned int)((char*)&probe.Refs-(char*)&probe)!=13)return false;
 int initialLive=liveStringBlocks,initialDestroyed=destroyedStringBlocks;
 { Element root;SetElement(root,123);Element* copies=(Element*)::operator new(300*sizeof(Element));
 for(int i=0;i<300;++i)new(copies+i)Element(root);
 if(root.Data->Refs!=301||liveStringBlocks!=initialLive+1)return false;
 copies[0]=root;if(root.Data->Refs!=301)return false;
 for(int j=0;j<45;++j)copies[j].~Element();
 if(root.Data->Refs!=256||liveStringBlocks!=initialLive+1)return false;
 copies[45].~Element();if(root.Data->Refs!=255||liveStringBlocks!=initialLive+1)return false;
 for(int k=46;k<300;++k)copies[k].~Element();
 if(root.Data->Refs!=1||liveStringBlocks!=initialLive+1)return false;
 ::operator delete(copies);
 }
 return liveStringBlocks==initialLive&&destroyedStringBlocks==initialDestroyed+1;
}
}

int main(){using namespace N00433530;if(!Cross255())return 100;{Element* a=(Element*)std::malloc(5*sizeof(Element));for(int i=0;i<4;++i){new(a+i)Element();SetElement(a[i],i+1);}Element x;SetElement(x,99);RetailVector<Element> v={a,a+4,a+5};v.Overflow(a+2,x,FalseTag(),2,false);unsigned int want[]={1,2,99,99,3,4};if(v.Finish-v.Start!=6||v.End-v.Start!=8)return 1;for(int j=0;j<6;++j)if(v.Start[j].Data->Value!=want[j])return 2;if(x.Data->Refs!=3)return 3;v.Overflow(v.Finish,x,FalseTag(),1,true);if(v.Finish-v.Start!=7||v.Start[6].Data!=x.Data||x.Data->Refs!=4)return 4;for(Element* p=v.Start;p!=v.Finish;++p)p->~Element();std::free(v.Start);}if(liveStringBlocks!=0)return 5;std::printf("00433530_TEST PASS\n");return 0;}