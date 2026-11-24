// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `f28` -> `m_frameOfLastUse`.
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Leaf: ?OnStart@CCombatSequence_BattleCharge@@UAEXXZ
// Class-model move (NF325, 2026-11-24): defined on the real class of engine/CCombatSequence_BattleCharge.h under FABLE_RETAIL_CCOMBATSEQUENCE_BATTLECHARGE_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CCombatSequence_BattleCharge_Methods::OnStart); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CCOMBATSEQUENCE_BATTLECHARGE_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
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
void CCombatSequence_BattleCharge::OnStart() { this->m_frameOfLastUse = CMainGameComponent::Get()->GetCurrentWorldFrame(); }
