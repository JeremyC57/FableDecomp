// 00cbb4d0 N00cbb4d0::__adjust_heap<Element,CCompareString> (STLport; catalog: _Adjust_heap<std::pair<CCharString,CCountedPointer<CEntityScriptBindingBase>_>*,int,std::_Adjust_heap<std::pair<CCharString,CCountedPointer<CEntityScriptBindingBase>_>*,int,std::pair<class_CCharString)
// Flags: /O2 /Oy /W3 /Gr
// Leaf: __adjust_heap
// Layout: SGI heap iterator stride and by-value element/comparator sizes are taken from oracle indexed accesses and ret cleanup.
#include <new>
#include <string.h>
void* __cdecl operator new[](unsigned int);
void __cdecl operator delete[](void*);
#pragma intrinsic(memcpy)
namespace N00cbb4d0 {

struct RefBlock { unsigned int Refs; void (__fastcall *Destroy)(void*); void* Data; };
struct Base { virtual void v00(); ~Base(); };
template<class T> struct Counted {
 T* Data; RefBlock* Block;
 __forceinline Counted():Data(0),Block(0) {}
 __forceinline Counted(const Counted& o) {Data=o.Data;Block=o.Block;if(Block) ++Block->Refs;}
 __forceinline ~Counted() { Release(); Data=0;Block=0; }
 __forceinline RefBlock* GetControl()const{return Block;}
 __forceinline void Release() { if(Block) {--Block->Refs; if(Block->Refs==0) {Block->Destroy(Block->Data); ::operator delete(GetControl());}} }
 __forceinline Counted& operator=(const Counted& o) {RefBlock* block=o.Block; T* data=o.Data; if(Block!=block){Release();Data=data;Block=block;if(block)++block->Refs;}return *this;}
};
template<class T> struct PolyCounted:Base {
 T* Data;RefBlock* Block;
 __forceinline PolyCounted():Data(0),Block(0) {}
 __forceinline PolyCounted(const PolyCounted& o):Data(o.Data),Block(o.Block) {if(Block)++Block->Refs;}
 __forceinline ~PolyCounted(){if(Block){--Block->Refs;if(Block->Refs==0){Block->Destroy(Block->Data);::operator delete(Block);}}Data=0;Block=0;}
 __forceinline PolyCounted& operator=(const PolyCounted& o){RefBlock* block=o.Block;T* data=o.Data;if(Block!=block){if(Block){--Block->Refs;if(Block->Refs==0){Block->Destroy(Block->Data);::operator delete(Block);}}Data=data;Block=block;if(block)++block->Refs;}return *this;}
};
// Layout:99ec30 copy shares data and reads refcount+13;99e9b0 releases chararray then17-byte block.
#pragma pack(push,1)
struct StringData{char*Data;unsigned Length,Capacity;unsigned char Fast;int Refs;};
#pragma pack(pop)
extern int StringInstances;
struct CCharString {
 StringData*Data;
 __declspec(noinline) void Clear();
 __declspec(noinline) CCharString();
 __declspec(noinline) CCharString(const CCharString&v);
 __declspec(noinline) ~CCharString();
 __declspec(noinline) CCharString&operator=(const CCharString&v);
 __declspec(noinline) bool FastCompare(const CCharString&v)const;
};
struct CWideString {unsigned int Key; CWideString();CWideString(const CWideString&);~CWideString();CWideString& operator=(const CWideString&);};
struct Element {CCharString S;Counted<void> P;};
struct CCompareString {unsigned int State; __forceinline bool operator()(const Element& a,const Element& b) const {return a.S.FastCompare(b.S);}};

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

template void __fastcall __adjust_heap<Element,CCompareString>(Element*,int,int,Element,CCompareString);
}