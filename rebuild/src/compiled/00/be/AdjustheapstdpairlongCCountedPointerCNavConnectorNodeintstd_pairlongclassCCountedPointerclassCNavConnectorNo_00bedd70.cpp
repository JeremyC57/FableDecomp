// 00bedd70 N00bedd70::__adjust_heap<Element,CKeyPairCompareLess> (STLport; catalog: _Adjust_heap<std::pair<long,CCountedPointer<CNavConnectorNode>_>*,int,std::_Adjust_heap<std::pair<long,CCountedPointer<CNavConnectorNode>_>*,int,std::pair<long,class_CCountedPointer<class_CNavConnecto)
// Flags: /O2 /Oy /W3 /Gr
// Leaf: __adjust_heap
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
struct CWideString {unsigned int Key; CWideString();CWideString(const CWideString&);~CWideString();CWideString& operator=(const CWideString&);};
struct Element {int Key;CCompositeSurfaceInfo Info;};
struct CKeyPairCompareLess {unsigned int State; __forceinline bool operator()(const Element& a,const Element& b) const {return a.Key<b.Key;}};

template<class T,class C> void __fastcall __push_heap(T* first,int hole,int top,T value,C comp);
template<class T,class C> void __fastcall __adjust_heap(T* first,int hole,int length,T value,C comp) {
 int top=hole;
 int second=2*hole+2;
 while(second<length) {
  if(comp(first[second],first[second-1])) --second;
  first[hole]=first[second];
  hole=second;second=2*(second+1);
 }
 if(second==length) {first[hole]=first[second-1];hole=second-1;}
 __push_heap(first,hole,top,value,comp);
}

template void __fastcall __adjust_heap<Element,CKeyPairCompareLess>(Element*,int,int,Element,CKeyPairCompareLess);
}