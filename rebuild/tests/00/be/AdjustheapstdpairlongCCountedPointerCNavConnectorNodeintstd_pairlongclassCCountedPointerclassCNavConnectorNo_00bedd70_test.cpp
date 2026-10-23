// 00bedd70 AdjustHeap
// Flags: /O2 /Oy /W3 /Gr
// Leaf: AdjustHeap
// Layout:RTTI vtable12a8154 CCompositeSurfaceInfo,1231710 CBaseClass;PDB147422 info12/Cache4. Basevirtualdtor4371f0 calls99a2e0;compositedeletingdtorbeda30 releases8-byteCache thenbase.
// Layout: SGI heap iterator stride and by-value element/comparator sizes are taken from oracle indexed accesses and ret cleanup.
#include <new>
namespace N00bedd70 {

struct RefBlock { unsigned int Refs; void (__fastcall *Destroy)(void*); void* Data; };
struct CBaseClass { virtual ~CBaseClass(); };
template<class T> struct Counted {
 T* Data; RefBlock* Block;
 __forceinline Counted():Data(0),Block(0) {}
 __forceinline Counted(const Counted& o):Data(o.Data),Block(o.Block) {if(Block) ++Block->Refs;}
 __forceinline ~Counted() { Release(); Data=0;Block=0; }
 __forceinline RefBlock* GetControl()const {return Block;}
 __forceinline void Release() { if(Block) {--Block->Refs; if(Block->Refs==0) {Block->Destroy(Block->Data); ::operator delete(GetControl());}} }
 __forceinline Counted& operator=(const Counted& o) {RefBlock* block=o.Block; T* data=o.Data; if(Block!=block){Release();Data=data;Block=block;if(block)++block->Refs;}return *this;}
};
struct CCompositeSurfaceCache;
struct CCompositeSurfaceInfo:CBaseClass {Counted<CCompositeSurfaceCache> Cache;};
struct String {unsigned int Key; String();String(const String&);~String();String& operator=(const String&);bool FastCompare(const String&) const;};
struct WideString {unsigned int Key; WideString();WideString(const WideString&);~WideString();WideString& operator=(const WideString&);};
struct Element {int Key;CCompositeSurfaceInfo Info;};
struct Compare {unsigned int State; __forceinline bool operator()(const Element& a,const Element& b) const {return a.Key<b.Key;}};

template<class T,class C> void __fastcall PushHeap(T* first,int hole,int top,T value,C comp);
template<class T,class C> void __fastcall AdjustHeap(T* first,int hole,int length,T value,C comp) {
 int top=hole;
 int second=2*hole+2;
 while(second<length) {
  if(comp(first[second],first[second-1])) --second;
  first[hole]=first[second];
  hole=second;second=2*(second+1);
 }
 if(second==length) {first[hole]=first[second-1];hole=second-1;}
 PushHeap(first,hole,top,value,comp);
}

template void __fastcall AdjustHeap<Element,Compare>(Element*,int,int,Element,Compare);
}

#include <cstdio>
#include <cstring>
namespace N00bedd70 {
struct CCompositeSurfaceCache {int ID;};static int liveControls,livePayloads,destroyCalls,baseDestructs;static RefBlock* controls[20];static int controlCount;
CBaseClass::~CBaseClass(){++baseDestructs;}
void __fastcall destroyCache(void* p){CCompositeSurfaceCache* c=(CCompositeSurfaceCache*)p;if(c){delete c;--livePayloads;++destroyCalls;}}
static void init(Element& e,int key,int id){e.Key=key;RefBlock* b=new RefBlock;b->Refs=1;b->Destroy=destroyCache;b->Data=new CCompositeSurfaceCache;((CCompositeSurfaceCache*)b->Data)->ID=id;e.Info.Cache.Data=(CCompositeSurfaceCache*)b->Data;e.Info.Cache.Block=b;controls[controlCount++]=b;++liveControls;++livePayloads;}
template<class T,class C> void __fastcall PushHeap(T* first,int hole,int top,T value,C comp){int parent=(hole-1)/2;while(hole>top&&comp(first[parent],value)){first[hole]=first[parent];hole=parent;parent=(hole-1)/2;}first[hole]=value;}
static int id(const Element& e){return e.Info.Cache.Data?e.Info.Cache.Data->ID:0;}
static bool owns(const Element* p,int n,const Element& x,const Element& y,const Element& z){for(int i=0;i<controlCount;++i){RefBlock* b=controls[i];unsigned int expected=0;for(int j=0;j<n;++j)if(p[j].Info.Cache.Block==b)++expected;if(x.Info.Cache.Block==b)++expected;if(y.Info.Cache.Block==b)++expected;if(z.Info.Cache.Block==b)++expected;if(expected&&b->Refs!=expected)return false;}return true;}
}
int main(){using namespace N00bedd70;if(sizeof(CCompositeSurfaceInfo)!=12||sizeof(Element)!=16)return 1;{Element a[7];for(int i=0;i<7;++i)init(a[i],7-i,100+i);Compare c;c.State=0;Element x=a[0];x.Key=1;Element y=a[0];Element z=a[0];AdjustHeap(a,0,7,x,c);if(a[0].Key!=6||id(a[0])!=101||a[1].Key!=4||id(a[1])!=103||a[3].Key!=1||id(a[3])!=100||!owns(a,7,x,y,z))return 2;y=a[0];y.Key=10;PushHeap(a,6,0,y,c);if(a[0].Key!=10||id(a[0])!=101||!owns(a,7,x,y,z))return 3;z=a[0];z.Key=20;AdjustHeap(a,0,1,z,c);if(a[0].Key!=20||id(a[0])!=101||!owns(a,7,x,y,z))return 4;RefBlock* shared=a[0].Info.Cache.Block;unsigned int before=shared->Refs;a[0]=a[0];if(shared->Refs!=before)return 5;Element alias=a[0];before=shared->Refs;alias=a[0];if(shared->Refs!=before)return 6;}if(livePayloads||destroyCalls!=7||baseDestructs<10)return 7;{Element one[2];init(one[0],1,201);init(one[1],9,202);Compare c;c.State=0;Element candidate=one[0];candidate.Key=4;AdjustHeap(one,0,2,candidate,c);if(one[0].Key!=9||id(one[0])!=202||one[1].Key!=4||id(one[1])!=201)return 8;}if(livePayloads||destroyCalls!=9)return 9;std::printf("00bedd70_TEST PASS\n");return 0;}