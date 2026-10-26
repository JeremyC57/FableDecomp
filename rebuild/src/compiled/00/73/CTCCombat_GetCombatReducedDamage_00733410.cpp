// CTCCombat::GetCombatReducedDamage(float damage, CThing* attacker) -- 0 when the parent creature (virtual slot 77 true) is performing the action whose code pair is (100, 250); otherwise the damage unchanged
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00733410, length 72 bytes.
// Callees: CThingCreatureBase virtual slot 77 (+0x134), CThingCreatureBase::IsPerformingAnyAction @
// 0x006622A0 (the current action at +0x100 is set and its +0x61 flag clear). Float pool: 0.0f @ 0x0122DEDC.
// Layout = engine/CTCCombat.h: ParentCreature +0x24; the creature's current action at +0x100, whose
// action type +0x20 and sub-type +0x24 are compared with 100 and 250.
// Shape: one `&&` chain: the parent creature is re-read from `this` for every test (esi = this), the
// action pointer is read once for the two compares, both exits load their float (`fld [pool]` for the
// zero, `fld [esp+8]` for the argument); the second argument is unused (`ret 8`).
#include "engine/CTCCombat.h"

class CThing;

class CCreatureActionBase { public:
    unsigned char _pad[0x20];
    long          ActionType;                               // +0x20
    long          ActionSubType;                            // +0x24
};

class CThingCreatureBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64();
    virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74();
    virtual void v75(); virtual void v76();
    virtual bool IsInCombatStance();                        // slot 77 (+0x134)
    unsigned char        _pad04[0xfc];
    CCreatureActionBase* CurrentAction;                     // +0x100
public:
    bool IsPerformingAnyAction() const;   // 0x006622A0
};

struct CTCCombat_Methods : CTCCombat {
    float GetCombatReducedDamage(float damage, CThing* attacker);
};

float CTCCombat_Methods::GetCombatReducedDamage(float damage, CThing* attacker)
{
    if (ParentCreature->IsInCombatStance() && ParentCreature->IsPerformingAnyAction()) {
        CCreatureActionBase* action = ParentCreature->CurrentAction;
        if (action->ActionType == 100 && action->ActionSubType == 250)
            return 0.0f;
    }
    return damage;
}