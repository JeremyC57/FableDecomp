// Grade: DIFFER(426v428) -- esi/edi roles swapped (retail esi=this, edi=&PTarget); frame 0xbc v 0xcc (Fire action is larger than modelled); retail fetches the Fire target GetPItem + creature before the IP argument copy
// CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets::Update() -- strafe into 15-unit range of the live target, then aim, fire and reload at it while it stays alive; always terminate
// Leaf: Update@CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets
#include "engine/CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets.h"
struct CThing { unsigned char _pad[0x91]; unsigned char m_flags91; };
struct CThingCreatureBase;
struct CAIBrainRetail { unsigned char _pad[0x18]; CThingCreatureBase* Creature; };
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer();            // 0x00A01C10
    CThing* Data;
    CBaseIntelligentPointer();                     // 0x00A01B10
    CThing* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CThing*);                        // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {    // CIntelligentPointer<CThing const>, vtable 0x01237F1C
    CIntelligentPointerThing(CThing* p) { SetPItem(p); }
    CIntelligentPointerThing(const CIntelligentPointerThing& p) { SetPItem(p.GetPItem()); }
};
inline bool __fastcall IS_THING_ALIVE(const CIntelligentPointerThing& p)   // out of line at 0x0069CE20 once the inline budget is spent
{
    if (p.GetPItem()) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CCreatureActionBase {
    unsigned char _pad[0xa8];
    ~CCreatureActionBase();                        // 0x00693EF0
};
struct CCreatureAction_AimMissileWeapon : CCreatureActionBase {
    CBaseIntelligentPointer PTarget;               // +0xa8
    CCreatureAction_AimMissileWeapon(CThingCreatureBase*);   // 0x00718650
};
struct CCreatureAction_FireMissileWeapon : CCreatureActionBase {
    unsigned char _pad2[0x10];
    CCreatureAction_FireMissileWeapon(CThingCreatureBase*, CThing*);   // 0x008D5110
    ~CCreatureAction_FireMissileWeapon();          // 0x0078ADF0
};
struct CCreatureAction_ReloadMissileWeapon : CCreatureActionBase {
    CBaseIntelligentPointer PAmmo;                 // +0xa8
    CCreatureAction_ReloadMissileWeapon(CThingCreatureBase*);   // 0x00718710
    ~CCreatureAction_ReloadMissileWeapon();        // 0x00929E60
};
enum EAIActionReturn { AI_ACTION_RETURN_0, AI_ACTION_RETURN_1, AI_ACTION_DONE };
enum EAIActionRepeat { AI_REPEAT_0, AI_REPEAT_1 };
enum EMovementType { MOVE_0, MOVE_1, MOVE_2, MOVE_3, MOVE_4, MOVE_5 };
struct RangedMethods : CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets {
    void Update();
    void Terminate();                              // 0x008FCF40
    EAIActionReturn AIAction_ProjectileAttackAndStrafeToThingProximity(EAIActionRepeat, CIntelligentPointerThing, CIntelligentPointerThing, float, bool*, EMovementType, bool);   // 0x0097EF60
    EAIActionReturn AIAction_PerformActionTargetFaceAndWait(CCreatureActionBase*, CIntelligentPointerThing, bool);   // 0x0097DCB0
    __forceinline CIntelligentPointerThing& Target() { return *(CIntelligentPointerThing*)PTarget; }
    __forceinline CThingCreatureBase* GetCreature() { return (*(CAIBrainRetail**)_pad_0x04)->Creature; }
};
void RangedMethods::Update()
{
    if (IS_THING_ALIVE(Target())) {
        bool inRange;
        if (AIAction_ProjectileAttackAndStrafeToThingProximity(AI_REPEAT_1, Target(), Target(), 15.0f, &inRange, MOVE_5, true) == AI_ACTION_DONE
            && IS_THING_ALIVE(Target())) {
            EAIActionReturn aimed = AIAction_PerformActionTargetFaceAndWait(&CCreatureAction_AimMissileWeapon(GetCreature()), Target(), false);
            if (aimed == AI_ACTION_DONE && IS_THING_ALIVE(Target())) {
                EAIActionReturn fired = AIAction_PerformActionTargetFaceAndWait(&CCreatureAction_FireMissileWeapon(GetCreature(), Target().GetPItem()), Target(), false);
                if (fired == AI_ACTION_DONE && IS_THING_ALIVE(Target()))
                    AIAction_PerformActionTargetFaceAndWait(&CCreatureAction_ReloadMissileWeapon(GetCreature()), Target(), false);
            }
        }
    }
    Terminate();
}
