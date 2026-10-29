// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `f28` -> `WFNextHappen`.
#include "engine/CCombatSequence_TrollOpeningGroundPound.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCombatSequence_TrollOpeningGroundPound; class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870 ?GetFrame@CWorld@@SIJXZ
bool __fastcall IsValid(CCombatSequence_TrollOpeningGroundPound* sequence){ return CWorld::GetFrame() > sequence->WFNextHappen ? true : false; }