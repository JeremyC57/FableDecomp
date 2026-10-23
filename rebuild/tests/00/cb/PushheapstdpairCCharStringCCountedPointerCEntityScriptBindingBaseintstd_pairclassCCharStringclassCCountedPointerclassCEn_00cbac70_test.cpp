// 00cbac70 N00cbac70::__push_heap<Element,CCompareString> (STLport; catalog: _Push_heap<std::pair<CCharString,CCountedPointer<CEntityScriptBindingBase>_>*,int,std::_Push_heap<std::pair<CCharString,CCountedPointer<CEntityScriptBindingBase>_>*,int,std::pair<class_CCharString,cla)
// Flags: /O2 /Oy /W3 /Gr
// Leaf: __push_heap
// Layout: SGI heap iterator stride and by-value element/comparator sizes are taken from oracle indexed accesses and ret cleanup.
#include <new>
namespace N00cbac70 {

struct RefBlock { unsigned int Refs; void (__fastcall *Destroy)(void*); void* Data; };
struct Base { virtual void v00(); ~Base(); };
template<class T> struct Counted {
 T* Data; RefBlock* Block;
 __forceinline Counted():Data(0),Block(0) {}
 __forceinline Counted(const Counted& o):Data(o.Data),Block(o.Block) {if(Block) ++Block->Refs;}
 __forceinline ~Counted() { Release(); Data=0;Block=0; }
 __forceinline void Release() { if(Block) {--Block->Refs; if(Block->Refs==0) {Block->Destroy(Block->Data); ::operator delete(Block);}} }
 __forceinline Counted& operator=(const Counted& o) {RefBlock* block=o.Block; T* data=o.Data; if(Block!=block){Release();Data=data;Block=block;if(block)++block->Refs;}return *this;}
};
template<class T> struct PolyCounted:Base {
 T* Data;RefBlock* Block;
 __forceinline PolyCounted():Data(0),Block(0) {}
 __forceinline PolyCounted(const PolyCounted& o):Data(o.Data),Block(o.Block) {if(Block)++Block->Refs;}
 __forceinline ~PolyCounted(){if(Block){--Block->Refs;if(Block->Refs==0){Block->Destroy(Block->Data);::operator delete(Block);}}Data=0;Block=0;}
 __forceinline PolyCounted& operator=(const PolyCounted& o){RefBlock* block=o.Block;T* data=o.Data;if(Block!=block){if(Block){--Block->Refs;if(Block->Refs==0){Block->Destroy(Block->Data);::operator delete(Block);}}Data=data;Block=block;if(block)++block->Refs;}return *this;}
};
struct CCharStringData;struct CCharString {CCharStringData* Data;CCharString();CCharString(const CCharString&);~CCharString();CCharString& operator=(const CCharString&);bool FastCompare(const CCharString&)const;};
struct CWideString {unsigned int Key; CWideString();CWideString(const CWideString&);~CWideString();CWideString& operator=(const CWideString&);};
struct Element {CCharString S;Counted<void> P;};
struct CCompareString {unsigned int State; __forceinline bool operator()(const Element& a,const Element& b) const {return a.S.FastCompare(b.S);}};

template<class T,class C> void __fastcall __push_heap(T* first,int hole,int top,T value,C comp) {
 int parent=(hole-1)/2;
 while(hole>top && comp(first[parent],value)) {first[hole]=first[parent];hole=parent;parent=(hole-1)/2;}
 first[hole]=value;
}

template void __fastcall __push_heap<Element,CCompareString>(Element*,int,int,Element,CCompareString);
}

#include <cstdio>
#include <cstring>
namespace N00cbac70 {
void Base::v00(){}Base::~Base(){}

#pragma pack(push,1)
struct CCharStringData {unsigned int Value;unsigned char Padding[9];unsigned int Refs;};
#pragma pack(pop)
static int liveStringBlocks,destroyedStringBlocks;
CCharString::CCharString():Data(0){}CCharString::CCharString(const CCharString& o):Data(o.Data){if(Data)++Data->Refs;}
CCharString::~CCharString(){if(Data&&--Data->Refs==0){delete Data;--liveStringBlocks;++destroyedStringBlocks;}}
CCharString& CCharString::operator=(const CCharString& o){if(Data!=o.Data){if(o.Data)++o.Data->Refs;if(Data&&--Data->Refs==0){delete Data;--liveStringBlocks;++destroyedStringBlocks;}Data=o.Data;}return *this;}
void SetString(CCharString& s,unsigned int value){CCharString t;t.Data=new CCharStringData;std::memset(t.Data,0,sizeof *t.Data);t.Data->Value=value;t.Data->Refs=1;++liveStringBlocks;s=t;}

bool CCharString::FastCompare(const CCharString& o)const{return (Data?Data->Value:0)<(o.Data?o.Data->Value:0);}
static int destroyed,liveControls;
void __fastcall Dispose(void*){++destroyed;--liveControls;}
RefBlock* MakeBlock(unsigned int id){RefBlock* b=new RefBlock;b->Refs=1;b->Destroy=Dispose;b->Data=(void*)id;++liveControls;return b;}
}

namespace N00cbac70 {
bool Cross255(){
 if(sizeof(CCharStringData)!=17)return false;
 CCharStringData probe;if((unsigned int)((char*)&probe.Refs-(char*)&probe)!=13)return false;
 int initialLive=liveStringBlocks,initialDestroyed=destroyedStringBlocks;
 { CCharString root;SetString(root,123);CCharString* copies=(CCharString*)::operator new(300*sizeof(CCharString));
 for(int i=0;i<300;++i)new(copies+i)CCharString(root);
 if(root.Data->Refs!=301||liveStringBlocks!=initialLive+1)return false;
 copies[0]=root;if(root.Data->Refs!=301)return false;
 for(int j=0;j<45;++j)copies[j].~CCharString();
 if(root.Data->Refs!=256||liveStringBlocks!=initialLive+1)return false;
 copies[45].~CCharString();if(root.Data->Refs!=255||liveStringBlocks!=initialLive+1)return false;
 for(int k=46;k<300;++k)copies[k].~CCharString();
 if(root.Data->Refs!=1||liveStringBlocks!=initialLive+1)return false;
 ::operator delete(copies);
 }
 return liveStringBlocks==initialLive&&destroyedStringBlocks==initialDestroyed+1;
}
}

int main(){using namespace N00cbac70;if(!Cross255())return 100;
 {Counted<void> first;first.Block=MakeBlock(1);first.Data=(void*)1;Counted<void> second;second.Block=MakeBlock(2);second.Data=(void*)2;first=second;if(destroyed!=1||liveControls!=1||second.Block->Refs!=2||first.Data!=(void*)2)return 1;first=first;if(second.Block->Refs!=2)return 2;second=first;if(second.Block->Refs!=2)return 3;{Counted<void> copied(first);if(copied.Block->Refs!=3)return 4;}if(second.Block->Refs!=2)return 5;}
 if(destroyed!=2||liveControls!=0)return 6;
 {Element a[7];for(int i=0;i<7;++i){SetString(a[i].S,7-i);a[i].P.Block=MakeBlock(i+10);a[i].P.Data=(void*)(i+10);}CCompareString c;std::memset(&c,0,sizeof c);
 Element x=a[0];SetString(x.S,1);__push_heap(a,3,0,x,c);if(a[0].S.Data->Value!=7||a[3].S.Data->Value!=1||a[3].P.Data!=(void*)10||a[3].S.Data!=x.S.Data)return 7;
 Element y=a[0];SetString(y.S,10);__push_heap(a,6,0,y,c);if(a[0].S.Data->Value!=10||a[0].P.Data!=y.P.Data||a[0].S.Data!=y.S.Data)return 8;
 Element z=a[0];SetString(z.S,20);__push_heap(a,0,0,z,c);if(a[0].S.Data->Value!=20||a[0].P.Block!=z.P.Block)return 9;if(a[0].P.Block->Refs<2||z.S.Data->Refs!=2)return 10;
 }
 if(destroyed!=9||liveControls!=0||liveStringBlocks!=0)return 11;
 std::printf("00cbac70_TEST PASS\n");return 0;}