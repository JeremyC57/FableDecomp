// CPlayerGui::AddHitByMelee(CThing& attacker, CThing& victim, const C3DVector& pos) -- forward to the combat-multiplier drawer: `mov ecx,[ecx+150h] ; jmp 0x00643D8C`
// Leaf: ?AddHitByMelee@CPlayerGui@@QAEXAAVCThing@@0ABVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00437990 (11 bytes).
// NF348 (2026-11-24): moved off its T::Run / Sub::Run_00643d8c stand-ins onto the shared engine/CPlayerGui.h, which already declares
//   AddHitByMelee unconditionally (opt-in FABLE_RETAIL_CPLAYERGUI_MEMBERS for PDrawCombatMultiplier +0x150; no header change). Name:
//   Ego_r.pdb ?AddHitByMelee@CPlayerGui@@QAEXAAVCThing@@0ABVC3DVector@@@Z (0x0044A640, after AddAutoPickup 0x0044A630 = retail 0x00437980;
//   it jumps to CDrawCombatMultiplier::AddHitByMelee) + FableWin.pdb (0x01A80380: CCountedPointer<CDrawCombatMultiplier>::operator->,
//   then the same call); the callee is Ego_r.pdb / FableWin.pdb ?AddHitByMelee@CDrawCombatMultiplier@NPlayerGui@@QAEXAAVCThing@@0ABVC3DVector@@@Z
//   (retail 0x00643D8C, respelled with this row as one unit). Caller: CThingCreatureBase::OnHit 0x00667D30 (0x00667F48, the GUI
//   singleton 0x013B8790). NPlayerGui::CDrawCombatMultiplier has no shared header: its member used here is declared below. Preamble
//   from the landed 0x004378F0 / 0x00437980 (the same MEMBERS opt-in). Catalog label _global::Forward_InitParticleEmitter_via0x150 was descriptive.
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_MEMBERS
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_LAYOUT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_SCOPED_PTR
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CPlayerGui.h"

namespace NPlayerGui {
class CDrawCombatMultiplier {
public:
    void AddHitByMelee(CThing& attacker, CThing& victim, const C3DVector& pos);   // 0x00643D8C
};
}

void CPlayerGui::AddHitByMelee(CThing& attacker, CThing& victim, const C3DVector& pos)
{
    PDrawCombatMultiplier->AddHitByMelee(attacker, victim, pos);
}
