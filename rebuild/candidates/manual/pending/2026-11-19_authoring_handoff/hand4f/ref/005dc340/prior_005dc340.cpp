// CAnimationSet::GetAnim(const CCharString& name, long index, const CAnimFilterBase& filter) -- from the first entry named `name`, return the index-th entry (counting only those the filter accepts)
// Grade: DIFFER(56v66) -- retail saves four registers and keeps the (dead) `this` in edi across the FindFirstAnimWithName call (`mov edi,ecx`), holds index in ebp and aligns the loop head (`lea ebx,[ebx+0]`); ours saves three (index in edi, no dead this, no alignment). 12 normalised diff lines (ChatGPT: 16). The siblings GetNoAnims 0x005DC290 / AnimExists 0x005DC2E0 (landed) have the same prologue but really use this (`cmp [edi+4]` end checks). next: the dead edi suggests an end check VC7.1 removed after register allocation; find a landed row with a dead saved receiver, or try a range-checked loop (`it != Anims.end()`) whose exit the PDB's FableWin assert removed; otherwise lane gap (register allocation).
// Leaf: ?GetAnim@CAnimationSet@@QBEABVCAnimationEntry@@ABVCCharString@@JABVCAnimFilterBase@@@Z
// Retail: Fable.exe @ 0x005DC340, 66 bytes (`ret 0Ch`); callee FindFirstAnimWithName 0x005DC020 (landed; its body sits in
//   this TU under auto_inline(off), as the landed 0x005DC020 source has it: CCharString::GetCRC inline + LowerBound call).
#include <stddef.h>
#include "rebuild_abi.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CIVCountedPointeeBase.h"
template<class T>class CIVCountedPointer{private:CIVCountedPointeeBase*Object;};
class CAnimComponentMap;class CCharStringData{public:const char*Data;unsigned long Length;};extern const char g_EmptyCharString[];
class CCRC{public:static unsigned long __fastcall Calc(unsigned long,const void*,unsigned long);};
class CCharString{public:CCharStringData*PStringData;const char*GetAsCharArray()const{return PStringData==0?g_EmptyCharString:PStringData->Data;}unsigned long GetLength()const{return PStringData==0?0:PStringData->Length;}unsigned long GetCRC()const{return CCRC::Calc(0,GetAsCharArray(),GetLength());}};
class CDefString {private:long TablePos;};
class CAnimationEntry {public:long BankIndex;CDefString AnimName,GroupName;private:CIVCountedPointer<CAnimComponentMap>Components;public:CAnimationEntry();CAnimationEntry(const CAnimationEntry&x):BankIndex(x.BankIndex),AnimName(x.AnimName),GroupName(x.GroupName),Components(x.Components){}~CAnimationEntry(){}};
namespace std {
typedef ::size_t size_t;
template<class K,class V>struct pair {typedef K first_type;typedef V second_type;K first;V second;pair(const K&k,const V&v):first(k),second(v){}};
template<class T>class allocator {};
template<class V,class T,class A>class _STLP_alloc_proxy:public A {public:V _M_data;};
template<class T,class A>class _Vector_base {protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A> {public:void push_back(const T&);const T*end()const{return this->_M_finish;}};
}
template<class T>class CArray:public std::vector<T> {};
template<class K,class V>class CKeyPairCompareLess {};
template<class K,class V,class C=CKeyPairCompareLess<K,V> >class CVectorMap:public CArray<std::pair<K,V> > {public:typedef std::pair<K,V>value_type;private:C Compare;bool Dirty;public:const value_type*LowerBound(const K&)const;void Insert(const K&key,const V&value){this->push_back(value_type(key,value));Dirty=true;}};
FABLE_STATIC_ASSERT(sizeof(CAnimationEntry)==16);FABLE_STATIC_ASSERT(sizeof(CIVCountedPointer<CAnimComponentMap>)==4);FABLE_STATIC_ASSERT(sizeof(std::pair<unsigned long,CAnimationEntry>)==20);FABLE_STATIC_ASSERT(sizeof(CVectorMap<unsigned long,CAnimationEntry>)==16);
class CAnimFilterBase {public:virtual bool operator()(const CAnimationEntry&)const=0;virtual CAnimFilterBase*Clone()const=0;virtual~CAnimFilterBase();};
class CAnimationSet {public:typedef CVectorMap<unsigned long,CAnimationEntry>::value_type value_type;private:CVectorMap<unsigned long,CAnimationEntry>Anims;long DefaultFlags,DefaultTransitionTime,DefaultDelay;CDefString DefaultGroup;public:const value_type*FindFirstAnimWithName(const CCharString&)const;const CAnimationEntry&GetAnim(const CCharString&,long,const CAnimFilterBase&)const;};
FABLE_STATIC_ASSERT(sizeof(CAnimationSet)==32);
#pragma auto_inline(off)
const CAnimationSet::value_type*CAnimationSet::FindFirstAnimWithName(const CCharString&name)const{
 unsigned long key=name.GetCRC();
 const value_type*it=Anims.LowerBound(key);if(it==Anims.end()||it->first!=key)return Anims.end();return it;
}
#pragma auto_inline(on)
const CAnimationEntry&CAnimationSet::GetAnim(const CCharString&name,long index,const CAnimFilterBase&filter)const{
 const value_type*it=FindFirstAnimWithName(name);
 long n=index;
 for(;;){if(filter(it->second)){if(n==0)break;--n;}++it;}
 return it->second;
}
