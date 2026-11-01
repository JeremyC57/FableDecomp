// 005dc340 CAnimationSet::GetAnim
// Leaf: ?GetAnim@CAnimationSet@@QBEABVCAnimationEntry@@ABVCCharString@@JABVCAnimFilterBase@@@Z
// Flags:
#include <stddef.h>
#include "rebuild_abi.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CIVCountedPointeeBase.h"
template<class T>class CIVCountedPointer{private:CIVCountedPointeeBase*Object;};
class CAnimComponentMap;class CCharString;
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
class CCharString{public:const char*GetAsCharArray()const;long GetLength()const;};
class CCRC{public:static unsigned long __fastcall Calc(unsigned long,const void*,unsigned long);};
#pragma auto_inline(off)
const CAnimationSet::value_type*CAnimationSet::FindFirstAnimWithName(const CCharString&name)const{
 unsigned long key=CCRC::Calc(0,name.GetAsCharArray(),name.GetLength());
 const value_type*it=Anims.LowerBound(key);if(it==Anims.end()||it->first!=key)return Anims.end();return it;
}
#pragma auto_inline(on)
const CAnimationEntry&CAnimationSet::GetAnim(const CCharString&name,long index,const CAnimFilterBase&filter)const{const value_type*it=FindFirstAnimWithName(name);for(;;){if(filter(it->second)){if(index==0)break;--index;}++it;}return it->second;}
