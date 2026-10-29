#include "engine/CReactionFrequencyTraits_LinearTime.h"  // retyped onto the PDB layout; byte parity re-verified
struct CReactionFrequencyTraits_LinearTime; class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870 ?GetFrame@CWorld@@SIJXZ
class CThingAICreature; struct CReactionFrequencyTraits_LinearTime_Methods : CReactionFrequencyTraits_LinearTime {
    void LogReaction(const CThingAICreature&, bool);
};
void CReactionFrequencyTraits_LinearTime_Methods::LogReaction(const CThingAICreature&, bool){ this->FrameHappened = CWorld::GetFrame(); }