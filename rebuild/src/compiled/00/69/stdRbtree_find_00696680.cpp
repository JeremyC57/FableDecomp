// std::_Rb_tree<EActionRegisteredType, pair<const EActionRegisteredType, CCountedPointer<CCreatureActionBase> >, ...>::find(const EActionRegisteredType&) -- lower-bound walk, then end() unless the key matches
// Leaf: ?find@?$_Rb_tree@W4EActionRegisteredType@@U?$pair@$$CBW4EActionRegisteredType@@V?$CCountedPointer@VCCreatureActionBase@@@@@std@@U?$_Select1st@U?$pair@$$CBW4EActionRegisteredType@@V?$CCountedPointer@VCCreatureActionBase@@@@@std@@@3@U?$less@W4EActionRegisteredType@@@3@V?$allocator@U?$pair@$$CBW4EActionRegisteredType@@V?$CCountedPointer@VCCreatureActionBase@@@@@std@@@3@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBW4EActionRegisteredType@@V?$CCountedPointer@VCCreatureActionBase@@@@@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EActionRegisteredType@@V?$CCountedPointer@VCCreatureActionBase@@@@@std@@@2@@2@ABW4EActionRegisteredType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00696680, 71 bytes, thiscall `ret 8` (hidden iterator result + key reference).
// Retail bytes:  mov ecx,[ecx] ; mov eax,[ecx+4] ; test eax,eax ; push esi ; push edi ; mov edi,[esp+10h] ; mov edx,ecx ; je E ; mov esi,[edi] ;
//   L: cmp [eax+10h],esi ; jl R ; mov edx,eax ; mov eax,[eax+8] ; ... (STLport 4.6.1 _tree.h find -> _M_find, signed key at +0x10).
// Identity (NF56, round 7, queue q10452): the STLport map::find over CCreatureActionBase::RegisteredActions
//   (std::map<EActionRegisteredType, CCountedPointer<CCreatureActionBase> >, engine/CCreatureActionBase.h; dynamic initializer 0x013BA8C8,
//   insert_unique 0x006F5400): the landed NF52 ports 0x00692E50 / 0x00857430 call 0x00696680 through that member and bind this spelling.
//   Was the generated stand-in CRetailTree_longkey::find (one of 27 bodies of this shape; no Leaf line). The explicit instantiation of the
//   shared stlport/fable_map.h _Rb_tree::find (non-template, the default branch) replaces the local tree model.
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
#include "engine/EActionRegisteredType.h"
#include "engine/CCountedPointer.h"
class CCreatureActionBase;

template std::_Rb_tree<EActionRegisteredType, std::pair<const EActionRegisteredType, CCountedPointer<CCreatureActionBase> >,
    std::_Select1st<std::pair<const EActionRegisteredType, CCountedPointer<CCreatureActionBase> > >, std::less<EActionRegisteredType>,
    std::allocator<std::pair<const EActionRegisteredType, CCountedPointer<CCreatureActionBase> > > >::iterator
std::_Rb_tree<EActionRegisteredType, std::pair<const EActionRegisteredType, CCountedPointer<CCreatureActionBase> >,
    std::_Select1st<std::pair<const EActionRegisteredType, CCountedPointer<CCreatureActionBase> > >, std::less<EActionRegisteredType>,
    std::allocator<std::pair<const EActionRegisteredType, CCountedPointer<CCreatureActionBase> > > >::find(const EActionRegisteredType& key);
