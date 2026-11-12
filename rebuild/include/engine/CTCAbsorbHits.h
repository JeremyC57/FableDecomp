#ifndef FABLE_ENGINE_CTCABSORBHITS_H
#define FABLE_ENGINE_CTCABSORBHITS_H
// Original classes/signatures: both PDB type records in evidence/00839940.
#include "engine/CTCBase.h"
class CTCAbsorbHits:public CTCBase {public:
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_ABSORB_HITS;}
 long DecrementHitsRemaining(){return --HitsRemaining;}
private:long HitsRemaining;
};
FABLE_STATIC_ASSERT(sizeof(CTCAbsorbHits)==0x10);
#endif
