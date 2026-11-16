// std::find_if(list<CIntelligentPointer<CThingAICreature> >::iterator first, last, HasGuardProperty pred) -- STLport 4.6 _algobase.h:
// forward to __find_if(first, last, pred, input_iterator_tag()) (0x0065F9AB, the tag by reference in edx)
// Leaf: ??$find_if@U?$_List_iterator@V?$CIntelligentPointer@VCThingAICreature@@@@U?$_Nonconst_traits@V?$CIntelligentPointer@VCThingAICreature@@@@@std@@@std@@UHasGuardProperty@@@std@@YI?AU?$_List_iterator@V?$CIntelligentPointer@VCThingAICreature@@@@U?$_Nonconst_traits@V?$CIntelligentPointer@VCThingAICreature@@@@@std@@@0@U10@0UHasGuardProperty@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066000A, 42 bytes, /Gr __fastcall (class arguments on the stack), frame pointer kept.
// Identity (NF42, round 5): STLport 4.6.2 find_if over the CTCVillage guard list (HasGuardProperty, engine/HasGuardProperty.h, both
// PDBs); the catalog already names this instantiation. Was the placeholder `CRangeString_0066000a::Construct` (a wide-string range
// shape); now an explicit instantiation of stlport/fable_algorithm.h's FABLE_RETAIL_STLP_FIND_IF_GUARD_BODY_PASS23 text (written for
// this row). Descriptive-by-library.
#pragma optimize("s", on)
#pragma optimize("y", off)
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
template std::_GuardListIterator FABLE_FASTCALL std::find_if(std::_GuardListIterator, std::_GuardListIterator, HasGuardProperty);
