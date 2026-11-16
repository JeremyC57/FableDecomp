// std::__find_if(list<CIntelligentPointer<CThingAICreature> >::iterator first, last, HasGuardProperty pred, const input_iterator_tag&) --
// STLport 4.6 _algobase.c input-iterator search: advance first until last or pred(*first) (the element copied by value for pred)
// Leaf: ??$__find_if@U?$_List_iterator@V?$CIntelligentPointer@VCThingAICreature@@@@U?$_Nonconst_traits@V?$CIntelligentPointer@VCThingAICreature@@@@@std@@@std@@UHasGuardProperty@@@std@@YI?AU?$_List_iterator@V?$CIntelligentPointer@VCThingAICreature@@@@U?$_Nonconst_traits@V?$CIntelligentPointer@VCThingAICreature@@@@@std@@@0@U10@0UHasGuardProperty@@ABUinput_iterator_tag@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065F9AB, 53 bytes, `ret 0Ch`. Caller: find_if 0x0066000A. Callees: CIntelligentPointer
// <CThingAICreature> copy ctor @ 0x0050F410, HasGuardProperty::operator()(CIntelligentPointer<CThingAICreature>) const @ 0x0065F152.
// Identity (NF42, round 5): the claim from both PDBs (HasGuardProperty) and STLport 4.6.2 source; descriptive-by-library. Was spelled
// over a local CListIterator model; now an explicit instantiation of stlport/fable_algorithm.h FABLE_RETAIL_STLP_FIND_IF_GUARD_BODY_PASS23.
#pragma optimize("s", on)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_OUTOFLINE
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#define FABLE_RETAIL_STLP_FIND_IF_GUARD_BODY_PASS23
#include "rebuild_abi.h"
#include "engine/CIntelligentPointer.h"
#include "engine/HasGuardProperty.h"
#include "stlport/fable_algorithm.h"
class CThingAICreature;
namespace std { typedef list<CIntelligentPointer<CThingAICreature> >::iterator _GuardListIterator; }
template std::_GuardListIterator FABLE_FASTCALL std::__find_if(std::_GuardListIterator, std::_GuardListIterator, HasGuardProperty, const std::input_iterator_tag&);
