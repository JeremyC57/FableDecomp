// Original typed voice variation selection and real integer random helper.
// Leaf: ?GetRandomSampleIndexVariation@CSimVoice@@QBEJW4EReactionSpeechType@@AAK@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Flags:
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CInitBaseClass:public CBaseClass {public:virtual~CInitBaseClass();virtual void Validate()const;bool Valid;};
enum EReactionSpeechType {};
long __fastcall GFROR13(unsigned long);
__forceinline unsigned long __fastcall GFRandom(unsigned long range,unsigned long*seed){*seed=*seed*0x24a1+0x24df;*seed=GFROR13(*seed);return range?*seed%range:0;}
#include "engine/CArray.h"
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