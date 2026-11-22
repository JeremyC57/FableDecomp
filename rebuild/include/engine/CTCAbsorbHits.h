#ifndef FABLE_ENGINE_CTCABSORBHITS_H
#define FABLE_ENGINE_CTCABSORBHITS_H
// Original classes/signatures: both PDB type records in evidence/00839940.
#include "engine/CTCBase.h"
class CTCAbsorbHits:public CTCBase {public:
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_ABSORB_HITS;}
 long DecrementHitsRemaining(){return --HitsRemaining;}
#ifdef FABLE_RETAIL_CTCABSORBHITS_ADD_PASS28
 // FableWin 0x0280BEC0 and retail OnActivate's in-place addition at +0x0c.
 void AddHitsRemaining(long hits) { HitsRemaining += hits; }
#endif
private:long HitsRemaining;
};
FABLE_STATIC_ASSERT_AT(10, sizeof(CTCAbsorbHits)==0x10);
#endif
