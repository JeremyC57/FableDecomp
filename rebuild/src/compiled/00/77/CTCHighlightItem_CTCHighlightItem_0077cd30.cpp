// CTCHighlightItem::~CTCHighlightItem() -- pick-up highlight component dtor: stop the highlight run, then ~PParticleEmitter and ~CTCBase
// Leaf: ??1CTCHighlightItem@@UAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0077CD30.
// Identity: RTTI vtable 0x0126AF14 (CTCHighlightItem : CTCBase), Ego_r.pdb ??1CTCHighlightItem@@UAE@XZ (catalog name was wrong).
// Callees: CTCHighlightItem::DoStopRunning 0x0077CCA0 (Ego_r IAE), ~CIntelligentPointer, ~CTCBase.
// Shape: `DoStopRunning();` then the member and base destructors (overlay FABLE_RETAIL_CTCHIGHLIGHTITEM).
#define FABLE_RETAIL_CTCHIGHLIGHTITEM
#include "engine/CTCHighlightItem.h"

CTCHighlightItem::~CTCHighlightItem()
{
    DoStopRunning();
}