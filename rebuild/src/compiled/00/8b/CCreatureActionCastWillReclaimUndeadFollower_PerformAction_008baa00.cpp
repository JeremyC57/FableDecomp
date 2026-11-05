// CCreatureAction_CastWillReclaimUndeadFollower::PerformAction() -- if the undead follower is alive, hand it back to the caster's special-abilities component (interface 0xa7) to reclaim
// Leaf: ?PerformAction@CCreatureAction_CastWillReclaimUndeadFollower@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BAA00, length 138 bytes, plain `ret`. Vtable slot of
//   CCreatureAction_CastWillReclaimUndeadFollower (vtable 0x0127D5BC, slot referenced from 0x0127D5C8).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020,
//          CTCSpecialAbilities::ReclaimUndeadFollower(CThingGameObject&) @ 0x0072B3B0 (Ego_r.pdb
//          `?ReclaimUndeadFollower@CTCSpecialAbilities@@QAEXAAVCThingGameObject@@@Z`).
// Shape: PUndeadFollower's address is taken once (edi); IS_THING_ALIVE inlined; the out-parameter GetInterface
//   template on PCreature (+8) for bit 0xa7 (`mov cl,[eax+34h] ; test cl,cl ; jns`), the out pointer left
//   uninitialised (the absent path reads the dead frame word into esi); the follower is re-fetched for the reference
//   argument after the lookup and the call is tail-duplicated into both paths.
// Layout: CCombatActionBase (retail 0x114 bytes, PCreature at +8); Ego_r.pdb puts PUndeadFollower at +0x12c (PDB base
//   0x12c), retail +0x114 (see the constructor 0x008BC5E0).
class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.Get() != 0) {
        if (!p.Get()->IsDead())
            return true;
    }
    return false;
}

class CThingGameObject;

class CTCSpecialAbilities { public:                // interface type 0xa7 component
    void ReclaimUndeadFollower(CThingGameObject& follower);   // 0x0072B3B0
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];
};

class CCreatureAction_CastWillReclaimUndeadFollower : public CCombatActionBase { public:
    CIntelligentPointer<CThing> PUndeadFollower;   // +0x114 CIntelligentPointer<CThingGameObject>
    virtual void PerformAction();
};

void CCreatureAction_CastWillReclaimUndeadFollower::PerformAction()
{
    if (IS_THING_ALIVE(PUndeadFollower)) {
        CTCSpecialAbilities* abilities;
        PCreature.Get()->GetTC(TCI_SPECIAL_ABILITIES, &abilities);
        abilities->ReclaimUndeadFollower(*(CThingGameObject*)PUndeadFollower.Get());
    }
}