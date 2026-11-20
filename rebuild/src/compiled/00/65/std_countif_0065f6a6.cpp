// std::count_if(list<CIntelligentPointer<CThingAICreature> >::iterator first, last, HasGuardProperty pred) -- count the village people that are guards (the predicate takes each CIntelligentPointer<CThingAICreature> by value)
// Leaf: ??$count_if@U?$_List_iterator@V?$CIntelligentPointer@VCThingAICreature@@@@U?$_Nonconst_traits@V?$CIntelligentPointer@VCThingAICreature@@@@@std@@@std@@UHasGuardProperty@@@std@@YIHU?$_List_iterator@V?$CIntelligentPointer@VCThingAICreature@@@@U?$_Nonconst_traits@V?$CIntelligentPointer@VCThingAICreature@@@@@std@@@0@0UHasGuardProperty@@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065F6A6, length 52 bytes, `ret 0Ch` (/Gr __fastcall: both 4-byte iterator classes and the
//   empty predicate go on the stack). Caller: CTCVillage::CalculateSecuritySweepTasks 0x00659800 (num_guards = count_if(VillagePeople.begin(),
//   VillagePeople.end(), HasGuardProperty())), which binds this spelling (reloc_bindings; the shared stlport/fable_count_if.h
//   COUNT_IF_CALLED_FASTCALL_PASS23 declaration).
// Callees: CIntelligentPointer<CThingAICreature>::CIntelligentPointer(const CIntelligentPointer<CThingAICreature>&) 0x0050F410 (the copy
//   that builds the predicate's by-value argument in its reserved slot), HasGuardProperty::operator()(CIntelligentPointer<CThingAICreature>)
//   const 0x0065F152 (`ret 8`: it owns and destroys its argument).
// Names (NF115 / NF119, PRIORITY q4497): Ego_r.pdb / FableWin.pdb ??RHasGuardProperty@@QBE_NV?$CIntelligentPointer@VCThingAICreature@@@@@Z
//   is the struct's only operator() (engine/HasGuardProperty.h; the 0x0065F152 self row defines it, the sibling __find_if 0x0065F9AB binds
//   it); FableWin.pdb count_if<..., HasGuardProperty> (YI under /Gr, int result = STLport difference_type). Was a local model with a
//   CIntelligentPointer<const CThing> parameter (a converting ctor spelling at 0x0050F410), a one-argument _List_iterator and __stdcall
//   (??$count_if@...@@YGJ...). STLport 4.6 _algo.h count_if (descriptive-by-library).
// Shape: the shared stlport/fable_count_if.h count_if body over the shared list iterator under /Gr, in a size-optimised TU (as the
//   sibling __find_if 0x0065F9AB over the same list). VC7.1 cannot explicitly instantiate it (C2785 on the dependent difference_type
//   return, four spellings), so the instance is emitted through the nf86 / nf88 address-taking scaffolding below.
#pragma optimize("s", on)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_OUTOFLINE
#define FABLE_RETAIL_STLP_FIND_IF_INPUT          // iterator_traits of the list iterator
#define FABLE_RETAIL_STLP_REMOVE_COPY_IF         // unary_negate (fable_count_if.h not1)
#include "rebuild_abi.h"
#include "engine/CIntelligentPointer.h"
#include "engine/HasGuardProperty.h"
#include "stlport/fable_count_if.h"
class CThingAICreature;
namespace std { typedef list<CIntelligentPointer<CThingAICreature> >::iterator _GuardListIterator; }

namespace {
// Fidelity: scaffolding, not retail: VC7.1 rejects an explicit instantiation of count_if, whose return type is the dependent
// iterator_traits<_InputIter>::difference_type (C2785, four spellings measured). Taking the instance's address makes this object emit
// the shared header's count_if body for list<CIntelligentPointer<CThingAICreature> >::iterator / HasGuardProperty as its pick-any COMDAT,
// the instance retail's CTCVillage::CalculateSecuritySweepTasks 0x00659800 calls. Internal to this TU.
std::iterator_traits<std::_GuardListIterator>::difference_type (*ReferenceGuardCountIf())(std::_GuardListIterator, std::_GuardListIterator,
    HasGuardProperty)
{
    return &std::count_if<std::_GuardListIterator, HasGuardProperty>;
}
}
