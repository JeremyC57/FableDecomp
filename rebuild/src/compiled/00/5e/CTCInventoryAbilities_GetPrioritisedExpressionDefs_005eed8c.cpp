// CTCInventoryAbilities::GetPrioritisedExpressionDefs(std::vector<long>& defs) const -- appends the received-expression order (the expression defs in priority order) to the caller's vector
#pragma optimize("s", on)
// Leaf: ?GetPrioritisedExpressionDefs@CTCInventoryAbilities@@QBEXAAV?$vector@JV?$allocator@J@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EED8C, length 67 bytes.
// Name (agentNF3, 2026-11-10): Ego_r.pdb + FableWin.pdb ?GetPrioritisedExpressionDefs@CTCInventoryAbilities@@QBEXAAV?$vector@JV?$allocator@J@std@@@std@@@Z
//   (const, the vector by reference; engine/CTCInventoryAbilities.h FABLE_RETAIL_CTCINVENTORYABILITIES_PRIORITISED). Was landed
//   non-const with a vector<long>* parameter on a local CTCInventoryAbilities_Retail layout.
// Callees: std::vector<long>::push_back(const long&) @ 0x00485FF3 (STLport COMDAT, called out of line in this TU:
//   FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE).
// Pragma: optimize("s", on) -- size TU (unaligned loop head, `sar ; je` entry test).
// Layout: engine/CTCInventoryAbilities.h retail branch: ReceivedExpressionOrder (vector<long>) at +0x1BC.
// Shape: an unsigned index over size() re-evaluated per iteration; the element is passed by reference
//        (`lea eax,[eax+edi*4]`) and the output reference re-read from its argument slot each time.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYABILITIES_PRIORITISED
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#include "engine/CTCInventoryAbilities.h"

void CTCInventoryAbilities::GetPrioritisedExpressionDefs(std::vector<long>& defs) const
{
    for (unsigned long i = 0; i < ReceivedExpressionOrder.size(); ++i)
        defs.push_back(ReceivedExpressionOrder[i]);
}
