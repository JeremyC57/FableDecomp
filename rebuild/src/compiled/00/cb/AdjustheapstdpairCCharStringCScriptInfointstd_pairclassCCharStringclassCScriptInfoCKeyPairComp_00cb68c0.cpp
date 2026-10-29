// 00cb68c0 N00cb68c0::__adjust_heap<Element,CKeyPairCompareLess> (STLport; catalog: _Adjust_heap<std::pair<CCharString,CScriptInfo>*,int,std::_Adjust_heap<std::pair<CCharString,CScriptInfo>*,int,std::pair<class_CCharString,class_CScriptInfo>,CKeyPairCompareLess<CCharString,CScriptInf)
// Flags: /O2 /Oy /W3 /Gr
// Leaf: __adjust_heap
// Layout: cb68e3-cb68fc compares shared payload pointers then calls CBasicString at429950 with payload receiver. PDB lines6834-6843: wrapper4, packed BasicString13 and NoRefs32 at13; Element stride24.
#include <new>
namespace N00cb68c0 {

struct RefBlock { unsigned int Refs; void (__fastcall *Destroy)(void*); void* Data; };
struct Base { virtual void v00(); ~Base(); };
template<class T> struct Counted {
 T* Data; RefBlock* Block;
 __forceinline Counted():Data(0),Block(0) {}
 __forceinline Counted(const Counted& o):Data(o.Data),Block(o.Block) {if(Block) ++Block->Refs;}
 __forceinline ~Counted() { Release(); Data=0;Block=0; }
 __forceinline void Release() { if(Block) {--Block->Refs; if(Block->Refs==0) {Block->Destroy(Block->Data); ::operator delete(Block);}} }
 __forceinline Counted& operator=(const Counted& o) {T* data=o.Data; RefBlock* block=o.Block; if(Block!=block){Release();Data=data;Block=block;if(block)++block->Refs;}return *this;}
};
template<class T> struct PolyCounted:Base {
 T* Data;RefBlock* Block;
 __forceinline PolyCounted():Data(0),Block(0) {}
 __forceinline PolyCounted(const PolyCounted& o):Data(o.Data),Block(o.Block) {if(Block)++Block->Refs;}
 __forceinline ~PolyCounted(){if(Block){--Block->Refs;if(Block->Refs==0){Block->Destroy(Block->Data);::operator delete(Block);}}Data=0;Block=0;}
 __forceinline PolyCounted& operator=(const PolyCounted& o){T* data=o.Data;RefBlock* block=o.Block;if(Block!=block){if(Block){--Block->Refs;if(Block->Refs==0){Block->Destroy(Block->Data);::operator delete(Block);}}Data=data;Block=block;if(block)++block->Refs;}return *this;}
};
#pragma pack(push,1)
struct CBasicString {char* PData;unsigned int StringLength;unsigned int DataLength:31;unsigned int Reserved:1;unsigned char UseFastExtend:1;bool operator<(const CBasicString&)const;};
struct StringData {CBasicString Data;unsigned int NoRefs;};
#pragma pack(pop)
} // (hoisted to global scope: naming wave 9)
class CCharString { public:N00cb68c0::StringData* Data;CCharString();CCharString(const CCharString&);~CCharString();const CCharString& operator=(const CCharString&);
 __forceinline bool operator<(const CCharString& other)const {if(other.Data==Data)return false;if(!Data)return true;if(!other.Data)return false;return Data->Data<other.Data->Data;}
};
namespace N00cb68c0 {
struct CWideString {unsigned int Key; CWideString();CWideString(const CWideString&);~CWideString();CWideString& operator=(const CWideString&);};
struct Element {CCharString S;unsigned int A,B;CCharString Other;unsigned int C;unsigned char Flag;};
struct CKeyPairCompareLess {unsigned int State; __forceinline bool operator()(const Element& a,const Element& b) const {return a.S<b.S;}};

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