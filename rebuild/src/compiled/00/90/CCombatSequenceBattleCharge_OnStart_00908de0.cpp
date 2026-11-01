// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `f28` -> `m_frameOfLastUse`.
#include "engine/CCombatSequence_BattleCharge.h"  // retyped onto the PDB layout; byte parity re-verified
#include "rebuild_abi.h"
class CMainGameComponent { public:
    long GetCurrentWorldFrame() const;             // 0x004167C8 (Ego_r.pdb ?GetCurrentWorldFrame@CMainGameComponent@@QBEJXZ)
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
struct CCombatSequence_BattleCharge_Methods : CCombatSequence_BattleCharge {
    void OnStart();
};
void CCombatSequence_BattleCharge_Methods::OnStart(){ this->m_frameOfLastUse = g_pMainGameComponent->GetCurrentWorldFrame(); }