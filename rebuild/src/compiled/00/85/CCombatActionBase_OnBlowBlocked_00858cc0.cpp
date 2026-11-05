// CCombatActionBase::OnBlowBlocked(bool flag) -- after the creature-action handling, when both the target and the attacker are alive, the attacker's primary-slot weapon (if it carries anything) is alive, and the attacker is in state 2 or its combat type does not ignore blocks, finish this action and make the attacker recoil (CCreatureAction_GeneralInterruptedAttack "STRIKE_RECOIL")
// Retry 10b (chain-split sweep, FIXES B-16): the `GetPItem()` result is a named local `item` before it is dereferenced, which gives retail's register colouring/order (the chained call gives the parked difference).
// Leaf: ?OnBlowBlocked@CCombatActionBase
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +4.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00858CC0, length 317 bytes (`ret 4`).
// Callees: CCreatureActionBase::OnBlowBlocked(bool) 0x006927F0, CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap::
//          LowerBound const 0x004365B0 (interface 0x46 = CTCCarrying), CTCCarrying::GetThingInPrimarySlot 0x006A1440,
//          CThingCreatureBase::GetCombatTypeDef 0x00666D10, this's slot 4 (+0x10, FinishAction), CCharString 0x0099EBF0,
//          CCreatureAction_GeneralInterruptedAttack(CThingCreatureBase*, CThing*, CCharString) 0x0087CF90 (landed),
//          CThingCreatureBase::SetCurrentAction 0x006644F0, ~CCombatActionBase 0x0062E900; literal 0x012764F0.
// Layout: the retail CCombatActionBase (PCreature +8, PTarget +0xa8, 0x114 bytes); the creature's state byte +0x10;
//          the combat type def's block response +0x98.
class CTCBase;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCCarrying { public:
    CThing* GetThingInPrimarySlot() const;             // 0x006A1440
};
class CCombatTypeDef { public:
    unsigned char _pad[0x98];
    long          BlockedResponse;               // +0x98
};

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CSoundPair { public: long FirstSound, LastSound; };
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.Get() != 0) {
        if (!p.Get()->IsDead())
            return true;
    }
    return false;
}
class CThingCreatureBase;
class CCombatActionBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();                 // slot 4 (+0x10)
    unsigned char           _pad04[4];
    CIntelligentPointer<CThing> PCreature;           // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CIntelligentPointer<CThing> PTarget;             // +0xa8
    unsigned char           _padb0[0x114 - 0xb0];
    virtual ~CCombatActionBase();                        // 0x0062E900
    void CreatureOnBlowBlocked(bool flag);       // 0x006927F0 (CCreatureActionBase::OnBlowBlocked)
    void OnBlowBlocked(bool flag);
};
class CCreatureAction_GeneralInterruptedAttack : public CCombatActionBase { public:   // vtable 0x01277A14
    CCreatureAction_GeneralInterruptedAttack(CThingCreatureBase* owner, CThing* target, CCharString animName);   // 0x0087CF90
};
class CThingCreatureBase : public CThing { public:
    CCombatTypeDef* GetCombatTypeDef();          // 0x00666D10
    bool SetCurrentAction(const CCombatActionBase& action);   // 0x006644F0
};

void CCombatActionBase::OnBlowBlocked(bool flag)
{
    CreatureOnBlowBlocked(flag);
    if (IS_THING_ALIVE(PTarget) && IS_THING_ALIVE(PCreature)) {
        bool canRecoil = true;
        const CTCCarrying* carrying;
        if (PCreature.Get()->PeekTC(TCI_CARRYING, &carrying)) {
            CThing* held = carrying->GetThingInPrimarySlot();
            if (!held || held->IsDead())
                canRecoil = false;
        }
        CThing* item = PCreature.Get();
        if ((item->GetType() == 2 || ((CThingCreatureBase*)PCreature.Get())->GetCombatTypeDef()->BlockedResponse != 2) && canRecoil) {
            CThing* target = PTarget.Get();
            CThingCreatureBase* creature = (CThingCreatureBase*)PCreature.Get();
            FinishAction();
            creature->SetCurrentAction(CCreatureAction_GeneralInterruptedAttack(creature, target, "STRIKE_RECOIL"));
        }
    }
}