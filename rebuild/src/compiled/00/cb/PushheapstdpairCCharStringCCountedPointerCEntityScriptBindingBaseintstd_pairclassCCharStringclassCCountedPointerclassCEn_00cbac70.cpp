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