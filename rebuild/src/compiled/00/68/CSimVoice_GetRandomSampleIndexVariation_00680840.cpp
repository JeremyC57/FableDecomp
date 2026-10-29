// Original typed voice variation selection and real integer random helper.
// Leaf: ?GetRandomSampleIndexVariation@CSimVoice@@QBEJW4EReactionSpeechType@@AAK@Z
// Flags:
#include <stddef.h>
#include "rebuild_abi.h"
class CBaseClass {public:virtual~CBaseClass();};
class CInitBaseClass:public CBaseClass {public:virtual~CInitBaseClass();virtual void Validate()const;bool Valid;};
enum EReactionSpeechType {};
long __fastcall GFROR13(unsigned long);
__forceinline unsigned long __fastcall GFRandom(unsigned long range,unsigned long*seed){*seed=*seed*0x24a1+0x24df;*seed=GFROR13(*seed);return range?*seed%range:0;}
namespace std {
template<class K,class V>struct pair {typedef K first_type;typedef V second_type;K first;V second;pair():first(K()),second(V()){}pair(const K&k,const V&v):first(k),second(v){}};
template<class T>class allocator {};
template<class V,class T,class A>class _STLP_alloc_proxy:public A {public:V _M_data;};
template<class T,class A>class _Vector_base {protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A> {public:typedef T*iterator;typedef const T*const_iterator;iterator begin(){return this->_M_start;}const_iterator begin()const{return this->_M_start;}iterator end(){return this->_M_finish;}const_iterator end()const{return this->_M_finish;}};
}
template<class T>class CArray:public std::vector<T> {};
template<class K,class V>class CKeyPairCompareLess {public:bool operator()(const K&a,const K&b)const{return a<b;}};
template<class K,class V,class C=CKeyPairCompareLess<K,V> >class CVectorMap:public CArray<std::pair<K,V> > {public:typedef std::pair<K,V>value_type;typedef const value_type*const_iterator;C Compare;bool Dirty;const_iterator LowerBound(const K&)const;const_iterator UpperBound(const K&)const;};
// Real helpers006804C0/00680500: complete observed read-only binary searches.
// Their out-of-line calls are retained while the compiler can inspect capture.
#pragma auto_inline(off)
template<class K,class V,class C>__declspec(noinline) typename CVectorMap<K,V,C>::const_iterator CVectorMap<K,V,C>::LowerBound(const K&key)const{const_iterator first=this->begin();ptrdiff_t len=this->end()-first;while(len>0){ptrdiff_t half=len/2;const_iterator mid=first+half;if(Compare(mid->first,key)){first=mid+1;len-=half+1;}else len=half;}return first;}
template<class K,class V,class C>__declspec(noinline) typename CVectorMap<K,V,C>::const_iterator CVectorMap<K,V,C>::UpperBound(const K&key)const{const_iterator first=this->begin();ptrdiff_t len=this->end()-first;while(len>0){ptrdiff_t half=len/2;const_iterator mid=first+half;if(!Compare(key,mid->first)){first=mid+1;len-=half+1;}else len=half;}return first;}
#pragma auto_inline(on)
class CSimVoice:public CInitBaseClass {public:CVectorMap<EReactionSpeechType,long>Voices;virtual~CSimVoice();long GetNoOfSampleIndexVariations(EReactionSpeechType type)const{return Voices.UpperBound(type)-Voices.LowerBound(type);}long GetSampleIndexVariation(EReactionSpeechType type,long index)const{return (Voices.LowerBound(type)+index)->second;}long GetRandomSampleIndexVariation(EReactionSpeechType,unsigned long&)const;};
FABLE_STATIC_ASSERT(sizeof(CBaseClass)==4);FABLE_STATIC_ASSERT(sizeof(CInitBaseClass)==8);FABLE_STATIC_ASSERT(sizeof(std::pair<EReactionSpeechType,long>)==8);FABLE_STATIC_ASSERT(sizeof(CVectorMap<EReactionSpeechType,long>)==16);FABLE_STATIC_ASSERT(sizeof(CSimVoice)==24);
long CSimVoice::GetRandomSampleIndexVariation(EReactionSpeechType type,unsigned long&seed)const{return GetSampleIndexVariation(type,GFRandom(GetNoOfSampleIndexVariations(type),&seed));}