// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `f28` -> `WFNextHappen`.
// Leaf: ?IsValid@CCombatSequence_TrollOpeningGroundPound@@UBE_NXZ
#define FABLE_RETAIL_CCOMBATSEQUENCE_TROLLOPENINGGROUNDPOUND_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequence_TrollOpeningGroundPound.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCombatSequence_TrollOpeningGroundPound; class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870 ?GetFrame@CWorld@@SIJXZ
bool CCombatSequence_TrollOpeningGroundPound::IsValid() const
{
    CCombatSequence_TrollOpeningGroundPound* sequence = const_cast<CCombatSequence_TrollOpeningGroundPound*>(this); return CWorld::GetFrame() > sequence->WFNextHappen ? true : false; }
// Flatten route (flatten_pilot v5, NF329): CCombatSequence_TrollOpeningGroundPound_IsValid -> CCombatSequence_TrollOpeningGroundPound::IsValid, the RTTI-placed PDB member; class model: engine/CCombatSequence_TrollOpeningGroundPound.h under FABLE_RETAIL_CCOMBATSEQUENCE_TROLLOPENINGGROUNDPOUND_VIRTUAL_MODEL, FABLE_RETAIL_CCOMBATSEQUENCEBASE.
