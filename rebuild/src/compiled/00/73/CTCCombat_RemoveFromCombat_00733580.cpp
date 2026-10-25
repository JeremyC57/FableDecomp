// CTCCombat::RemoveFromCombat() -- clear this combat component's transient state and, if the parent creature is in
// melee as an attacker, deregister it from the combat manager
// Leaf: ?RemoveFromCombat@CTCCombat@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00733580, oracle length 34 bytes.
// Retail bytes:  push esi ; mov esi,ecx ; call 0x007334F0 ; mov ecx,[esi+24h] ; call 0x00662410 ; test al,al ; je R ;
//   mov eax,[esi+24h] ; mov ecx,[esi+20h] ; push eax ; call 0x006ED9C0 ; R: pop esi ; ret
// Callees (FINDINGS chunk 391: "landed 0x733580 calls 0x7334F0 by a wrong name"; chunk 407: 0x662410 is
//   IsInMeleeAsAttacker): CTCCombat::ClearCombatVariables 0x007334F0 (private, Ego_r.pdb
//   `?ClearCombatVariables@CTCCombat@@AAEXXZ`; was "ResetCombatState"), CThingCreatureBase::IsInMeleeAsAttacker() const
//   0x00662410 (`?IsInMeleeAsAttacker@CThingCreatureBase@@QBE_NXZ`; was "IsRegisteredCombatant"),
//   CCombatManager::DeregisterCombatant(CThingCreatureBase&) 0x006ED9C0 (`?DeregisterCombatant@CCombatManager@@QAEXAAV
//   CThingCreatureBase@@@Z`; the creature by reference, not by pointer). The method is now a real CTCCombat member
//   (`?RemoveFromCombat@CTCCombat@@QAEXXZ`; the landed `CTCCombat_Methods : CTCCombat` helper class mangled it under
//   another class).
// Layout: CombatManager +0x20 and ParentCreature +0x24 (struct_layouts_egor, the same in retail: they precede the
//   STLport containers). The class is declared locally (engine/CTCCombat.h is the flattened PDB record and cannot take
//   member functions; the retail layout as the landed CTCCombat::ClearCombatVariables 0x007334F0).
class CThingCreatureBase {
public:
    bool IsInMeleeAsAttacker() const;           // 0x00662410
};

class CCombatManager {
public:
    void DeregisterCombatant(CThingCreatureBase& creature);   // 0x006ED9C0
};

struct CDefPointeeBase;

class CTCCombat {                               // retail layout (STLport containers), 0x98 bytes
public:
    virtual void v00();
    void*               ParentThing;                    // +0x04 (CTCBase)
    bool                ToBeRemoved;                    // +0x08
    bool                IsInGlobalUpdateTCs;            // +0x09
    long                ComboChainPos;                  // +0x0c
    long                ComboChainID;                   // +0x10
    long                ComboChainGracePeriodCountdown; // +0x14
    long                HandednessGracePeriodCountdown; // +0x18
    long                UnarmedCombatIdleCountdown;     // +0x1c
    CCombatManager*     CombatManager;                  // +0x20
    CThingCreatureBase* ParentCreature;                 // +0x24
    void RemoveFromCombat();
private:
    void ClearCombatVariables();                        // 0x007334F0
};

void CTCCombat::RemoveFromCombat()
{
    ClearCombatVariables();
    if (ParentCreature->IsInMeleeAsAttacker())
        CombatManager->DeregisterCombatant(*ParentCreature);
}
