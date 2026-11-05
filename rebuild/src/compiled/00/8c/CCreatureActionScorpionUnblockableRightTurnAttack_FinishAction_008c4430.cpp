// CCreatureAction_ScorpionUnblockableRightTurnAttack::FinishAction() -- base FinishAction, then if the live target carries a physical shield spell (interface type 0x93), tell the shield the unblockable attack by this creature has ended
// Leaf: ?FinishAction@CCreatureAction_ScorpionUnblockableRightTurnAttack@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C4430, length 124 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PTarget at +0xa8 three times, PCreature at +8 once), CVectorMap::LowerBound
// @ 0x0040F020 (the inlined GetInterface lookup for type 0x93 = bit 19 of the bitset dword at +0x30, the CTCPhysicalShieldSpell component:
// its constructor is 0x0079E5F0), the shield component's unblockable-attack notification @ 0x0079E580 (attacker creature, starting=false flag; the manifest
// "CCreatureAction_SetDirection" label is propagated), CCombatActionBase::FinishAction @ 0x00859A20 (direct base call, first).
// Shape: the landed IS_THING_ALIVE idiom on PTarget (pointer set, dead bit +0x91 bit 0 clear), then the out-parameter GetInterface as the
// inner if (its presence test is the `test dword ptr [eax+30h],80000h` on the third GetPItem result); the component is loaded into esi before
// the creature argument is fetched (arguments right to left: the flag, the creature). `this` rides in edi, &PTarget in esi.
// The PDB header (CCreatureAction_ScorpionUnblockableRightTurnAttack.h) is 0x24 bytes high against retail (CCombatActionBase is 0x114 bytes, PTarget at +0xa8),
// so `this` is modelled locally like the landed 0x008C8080.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined, re-fetching GetPItem).
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.Get() != 0) {
        if (!p.Get()->IsDead())
            return true;
    }
    return false;
}

class CTCPhysicalShieldSpell { public:             // interface type 0x93 component
public:
    void AllowReactionsOnHitFromAttacker(CThing*, bool);   // 0x0079E580
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();              // 0x00857520
    virtual void v02(); virtual void v03();
    protected: virtual void FinishAction(); public:            // slot 4: 0x00859A20
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10 rest of CCreatureActionBase (0xa8 bytes in retail)
    CIntelligentPointer<CThing> PTarget;        // +0xa8 CIntelligentPointer<CThing>
    unsigned char _padb0[0x114 - 0xb0];     // +0xb0
};

class CCreatureAction_ScorpionUnblockableRightTurnAttack : public CCombatActionBase { public:
    virtual void FinishAction();
};

void CCreatureAction_ScorpionUnblockableRightTurnAttack::FinishAction()
{
    CCombatActionBase::FinishAction();
    if (IS_THING_ALIVE(PTarget)) {
        CTCPhysicalShieldSpell* pShield;
        if (PTarget.Get()->GetTC(TCI_PHYSICAL_SHIELD_SPELL, &pShield))
            pShield->AllowReactionsOnHitFromAttacker(PCreature.Get(), false);
    }
}