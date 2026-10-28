// CTCCreatureGenerator::RemoveCreatureTriggerer(CThingCreatureBase& thing) -- remove every occurrence of the thing from the intelligent-pointer list.
// Leaf: ?RemoveCreatureTriggerer@CTCCreatureGenerator@@QAEXAAVCThingCreatureBase@@@Z
// Identity: proven; PDB name/reference prototype and CreatureTriggerers donor+40 ->retail+28; own CGSI::CreatureGeneratorRemoveTriggerer0x89ECC0 caller; find/remove_copy/erase helpers corroborate exact element8.
// Layout: reused generated header data under alias; its opaque named vector is viewed as the exact retail STLport vector. CTCSolidToThing has no header and uses the actual CTCBase member chain.
#define CTCCreatureGenerator CTCCreatureGenerator_HeaderLayout
#include "engine/CTCCreatureGenerator.h"
#undef CTCCreatureGenerator
#ifndef _MSC_VER
#define __fastcall
#define NOINLINE __attribute__((noinline))
#else
#define NOINLINE __declspec(noinline)
#endif
class CThing {}; class CThingCreatureBase:public CThing {}; class CBaseObject; class CBaseObjectPointer;
class CBaseIntelligentPointer { public: CBaseIntelligentPointer(); virtual ~CBaseIntelligentPointer(); CBaseObjectPointer* PData; CBaseObject* GetPItem() const; void SetPItem(CBaseObject*); };
template<class T> class CIntelligentPointer:public CBaseIntelligentPointer { public: CIntelligentPointer(T* item=0):CBaseIntelligentPointer() { SetPItem(item); } T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); } void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); } bool operator==(const CIntelligentPointer& other) const { return GetPItem()==other.GetPItem(); } CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; } };
namespace std {
struct input_iterator_tag {}; struct forward_iterator_tag:input_iterator_tag {}; struct bidirectional_iterator_tag:forward_iterator_tag {}; struct random_access_iterator_tag:bidirectional_iterator_tag { random_access_iterator_tag() {} };
template<class Input,class T> NOINLINE Input __fastcall __find(Input first,Input last,const T& value,const random_access_iterator_tag&) { int n=(last-first)>>2; for(;n>0;--n) { if(*first==value) return first; ++first; if(*first==value) return first; ++first; if(*first==value) return first; ++first; if(*first==value) return first; ++first; } switch(last-first) { case 3: if(*first==value) return first; ++first; case 2: if(*first==value) return first; ++first; case 1: if(*first==value) return first; default: return last; } }
template<class Input,class Output,class T> NOINLINE Output __fastcall remove_copy(Input first,Input last,Output result,const T& value) { for(;first!=last;++first) if(!(*first==value)) { *result=*first; ++result; } return result; }
template<class Input,class T> inline Input remove(Input first,Input last,const T& value) { first=__find(first,last,value,random_access_iterator_tag()); return first==last?first:remove_copy(first+1,last,first,value); }
template<class T> class allocator {};
template<class V,class T,class Alloc> class _STLP_alloc_proxy:public Alloc { public: V _M_data; };
template<class T,class Alloc=allocator<T> > class vector { public: T* _M_start; T* _M_finish; _STLP_alloc_proxy<T*,T,Alloc> _M_end_of_storage; T* begin() { return _M_start; } T* end() { return _M_finish; } T* erase(T*,T*); };
}
class CTCCreatureGenerator:public CTCCreatureGenerator_HeaderLayout { public: void RemoveCreatureTriggerer(CThingCreatureBase& thing); };
void CTCCreatureGenerator::RemoveCreatureTriggerer(CThingCreatureBase& thing) { reinterpret_cast<std::vector<CIntelligentPointer<CThingCreatureBase> >*>(&CreatureTriggerers)->erase(std::remove(reinterpret_cast<std::vector<CIntelligentPointer<CThingCreatureBase> >*>(&CreatureTriggerers)->begin(),reinterpret_cast<std::vector<CIntelligentPointer<CThingCreatureBase> >*>(&CreatureTriggerers)->end(),CIntelligentPointer<CThingCreatureBase>(&thing)),reinterpret_cast<std::vector<CIntelligentPointer<CThingCreatureBase> >*>(&CreatureTriggerers)->end()); }