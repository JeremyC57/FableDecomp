// CTCCombat::RemoveFromCombat()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00733580, oracle length 34 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push esi ; mov esi,ecx ; call <reset state> ; mov ecx,[esi+24h] ; call <creature in combat?> ; test al,al ; je R ; mov eax,[esi+24h] ; mov ecx,[esi+20h] ; push eax ; call <manager deregister> ; R: pop esi ; ret
// engine/CTCCombat.h (PDB): +0x20 CombatManager, +0x24 ParentCreature. Resets this combat
// component (0x007334F0), and if the parent creature still counts as a combatant (0x00662410,
// __thiscall on the creature, returns bool) deregisters it from the combat manager (0x006ED9C0,
// __thiscall on the manager, argument = creature). Callee labels in the manifest are propagated
// names and were not used.
#include "engine/CTCCombat.h"

struct CThingCreatureBaseCalls {
    bool IsRegisteredCombatant();                          // 0x00662410
};

struct CCombatManagerCalls {
    void DeregisterCombatant(CThingCreatureBase* creature);   // 0x006ED9C0
};

struct CTCCombat_Methods : CTCCombat {
    void ResetCombatState();                               // 0x007334F0
    void RemoveFromCombat();
};

void CTCCombat_Methods::RemoveFromCombat()
{
    ResetCombatState();
    if (reinterpret_cast<CThingCreatureBaseCalls*>(this->ParentCreature)->IsRegisteredCombatant())
        reinterpret_cast<CCombatManagerCalls*>(this->CombatManager)->DeregisterCombatant(this->ParentCreature);
}
