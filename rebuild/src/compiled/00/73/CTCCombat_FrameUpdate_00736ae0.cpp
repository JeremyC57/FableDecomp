// CTCCombat::FrameUpdate() -- per-frame combat update: tick the combat timers, refresh the usable melee abilities, update the flourish particles
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00736AE0, length 23 bytes, `jmp` tail. Identity: CTCCombat vtable 0x0126724C
// slot 9 (rtti-vtables.tsv) -- slot 9 is FrameUpdate across the CTC family (landed CTCHeroStats 0x0057D5A0, CTCRotate
// 0x0072A570, CTCPhysicsBase 0x006AFD60 ...). The manifest label (_Dest_val<allocator<pair<CCharString,CSymbolInfo>>>) is
// propagated over the byte-identical shape (also the landed CTCKrakenTentacle 0x007F0890).
// Callees: CTCCombat::UpdateCombatTimers @ 0x007339E0, UpdateUsableMeleeCombatAbilities @ 0x00736590,
// UpdateFlourishParticles @ 0x00736760 (address-unique manifest labels inside the CTCCombat code range).
// Layout: engine/CTCCombat.h with a method carrier (the header is data-only, its vptr flattened).
// Shape: three thiscalls on `this` (kept in esi), the last becomes the tail jmp.
#include "engine/CTCCombat.h"
struct CTCCombat_Methods : CTCCombat {
    void UpdateCombatTimers();                               // 0x007339E0
    void UpdateUsableMeleeCombatAbilities();                 // 0x00736590
    void UpdateFlourishParticles();                          // 0x00736760
    void FrameUpdate();
};
void CTCCombat_Methods::FrameUpdate()
{
    UpdateCombatTimers();
    UpdateUsableMeleeCombatAbilities();
    UpdateFlourishParticles();
}