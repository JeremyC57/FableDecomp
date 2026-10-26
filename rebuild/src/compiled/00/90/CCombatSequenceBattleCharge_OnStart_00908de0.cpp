// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `f28` -> `m_frameOfLastUse`.
#include "engine/CCombatSequence_BattleCharge.h"  // retyped onto the PDB layout; byte parity re-verified
#include "rebuild_abi.h"
struct G { int Helper(); };
extern G* g_908de0;
struct CCombatSequence_BattleCharge_Methods : CCombatSequence_BattleCharge {
    void OnStart();
};
void CCombatSequence_BattleCharge_Methods::OnStart(){ this->m_frameOfLastUse = g_908de0->Helper(); }