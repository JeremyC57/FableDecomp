// Leaf: ?OnActivate@CAIStateGroup_KidWatchHopscotch@@QAEXXZ
#include "rebuild_abi.h"
extern "C" void FABLE_FASTCALL Act_940bd0_A();
class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870 ?GetFrame@CWorld@@SIJXZ
struct CAIStateGroup_KidWatchHopscotch { char p[0x30]; int StartFrame; char p2[0x3c-0x34]; int ActivationCount; void OnActivate(); };
void CAIStateGroup_KidWatchHopscotch::OnActivate(){ this->ActivationCount++; Act_940bd0_A(); this->StartFrame = CWorld::GetFrame(); }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?OnActivate@CAIStateGroup_KidWatchHopscotch@@MAEXXZ); evidence: RTTI vtable slot + PDB name set.
