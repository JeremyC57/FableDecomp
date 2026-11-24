// CTCInventoryAbilities::AddLearnedExpression(long expressionIndex) -- append the newly learned expression to ReceivedExpressionOrder (+0x1BC)
// Leaf: ?AddLearnedExpression@CTCInventoryAbilities@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005EEDCF, 19 bytes, thiscall, `ret 4`: `lea eax,[esp+4] ; push eax ; add ecx,1BCh ; call
//   vector<long>::push_back 0x00485FF3`.
// Name (NF348, 2026-11-24): Ego_r.pdb + FableWin.pdb ?AddLearnedExpression@CTCInventoryAbilities@@QAEXJ@Z; Ego_r
//   CThingPlayerCreature::LearnExpression calls it, as the landed binder 0x006AC430 does (table witness). Was the stand-in
//   ?AddLearnedExpression@FableAddLearned_005eedcf@@QAEXJ@Z.
// Layout: the shared engine/CTCInventoryAbilities.h retail branch (ReceivedExpressionOrder std::vector<long> +0x1BC, asserted in the
//   generated branch) with its existing opt-in FABLE_RETAIL_CTCINVENTORYABILITIES_LEARNEDEXPRESSION; push_back is called out of line
//   (FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE), as retail.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYABILITIES_LEARNEDEXPRESSION
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CTCInventoryAbilities.h"

void CTCInventoryAbilities::AddLearnedExpression(long expressionIndex)
{
    ReceivedExpressionOrder.push_back(expressionIndex);
}
