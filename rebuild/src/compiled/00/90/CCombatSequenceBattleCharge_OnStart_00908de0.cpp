// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `f28` -> `m_frameOfLastUse`.
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
#include "engine/CCombatSequence_BattleCharge.h"  // retyped onto the PDB layout; byte parity re-verified
#include "rebuild_abi.h"
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    long GetCurrentWorldFrame() const;             // 0x004167C8 (Ego_r.pdb ?GetCurrentWorldFrame@CMainGameComponent@@QBEJXZ)
};
struct CCombatSequence_BattleCharge_Methods : CCombatSequence_BattleCharge {
    void OnStart();
};
void CCombatSequence_BattleCharge_Methods::OnStart(){ this->m_frameOfLastUse = CMainGameComponent::Get()->GetCurrentWorldFrame(); }