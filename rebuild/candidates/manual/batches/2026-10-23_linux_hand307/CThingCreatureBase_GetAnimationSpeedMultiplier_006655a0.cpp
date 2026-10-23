// CThingCreatureBase::GetAnimationSpeedMultiplier() -- the creature's animation speed: turning-via-animation speed,
// haste (squared) for action animations, and the hero's melee-strike speed scaled by the primary weapon's speed value
// and clamped to the hero's range for strike animations; square-rooted, and divided by the squared bullet-time scale
// when bullet time is on and the creature is affected
// Leaf: ?GetAnimationSpeedMultiplier@CThingCreatureBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x006655A0, plain `ret` (float in st0).
// Callees: CVectorMap::LowerBound const 0x004365B0; the current action's virtuals +0x3c, +0x78, +0x7c;
//   CTCLook::IsCurrentlyTurningViaAnimation 0x00753B40 / turn speed 0x00751B70 (TC 0x43); CTCHaste::
//   GetActionSpeedMultiplier 0x007FFFB0 (TC 0x8c); CTCHeroExperience::GetMeleeStrikeAnimationSpeedMultiplier 0x006D6870
//   and its min/max getters 0x006D6530 / 0x006D6540 (TC 0x68); CTCCarrying::GetThingInPrimarySlot 0x006A1440 (TC 0x46);
//   CTCWeapon::GetAnimationSpeedValue 0x005DA850 (TC 0x48); CThing::GetWorld 0x004C7AB0, IsAffectedByBulletTime
//   0x004C9920, the bullet-time time scale 0x004C5FA0.
// Layout: retail CThing interface bitset +0x20 / map +0x44, dead flag +0x91 bit 0; retail CThingCreatureBase current
//   action +0x100 (PDB view 0x10 larger). Action: +0x20 / +0x24 longs, +0x61 finished byte. World +0x68 bullet time
//   (+0x4 dword, +0xc byte).
#include <math.h>
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0
};
struct CBulletTime {
    unsigned char _pad00[4];
    long          Frames;                       // +0x04
    unsigned char _pad08[4];
    bool          Active;                       // +0x0c
    float GetTimeScale();                       // 0x004C5FA0
};
struct CWorld {
    unsigned char _pad[0x68];
    CBulletTime*  BulletTime;                   // +0x68
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];           // +0x20
    CInterfaceMap Components;                   // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                      // +0x91 bit 0 dead
    template<class T> __forceinline bool GetInterface(T** out, long interfaceType) const
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
    CWorld* GetWorld();                         // 0x004C7AB0
    bool IsAffectedByBulletTime();              // 0x004C9920
};
struct CCreatureActionBase {
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF(10) VF(11) VF(12) VF(13) VF(14)
    virtual bool IsAttack();                    // slot 15 (+0x3c)
    VF(16) VF(17) VF(18) VF(19) VF10(2)
    virtual bool UsesActionSpeed();             // slot 30 (+0x78)
    virtual bool UsesStrikeSpeed();             // slot 31 (+0x7c)
    long          ActionType;                   // +0x04
    unsigned char _pad08[0x20 - 0x08];
    long          Category;                     // +0x20
    long          SubCategory;                  // +0x24
    unsigned char _pad28[0x61 - 0x28];
    bool          Finished;                     // +0x61
};
struct CTCLook {
    bool IsCurrentlyTurningViaAnimation();      // 0x00753B40
    float GetTurnAnimationSpeed();              // 0x00751B70
};
struct CTCHaste { float GetActionSpeedMultiplier(); };          // 0x007FFFB0
struct CTCHeroExperience {
    float GetMeleeStrikeAnimationSpeedMultiplier();             // 0x006D6870
    float GetMinStrikeSpeed();                                  // 0x006D6530
    float GetMaxStrikeSpeed();                                  // 0x006D6540
};
struct CTCCarrying { CThing* GetThingInPrimarySlot(); };        // 0x006A1440
struct CTCWeapon { float GetAnimationSpeedValue(); };           // 0x005DA850
class CThingCreatureBase : public CThing {
public:
    unsigned char        _pad94[0x100 - 0x94];
    CCreatureActionBase* CurrentAction;         // +0x100
    float GetAnimationSpeedMultiplier();
};

float CThingCreatureBase::GetAnimationSpeedMultiplier()
{
    float multiplier = 1.0f;
    bool actionSpeed = false;
    bool strikeSpeed = false;
    CCreatureActionBase* action = CurrentAction;
    if (action && !action->Finished) {
        if (action->IsAttack() || (CurrentAction->Category == 100 && CurrentAction->SubCategory >= 100)) {
            actionSpeed = true;
            strikeSpeed = true;
        }
        if (CurrentAction->UsesActionSpeed())
            actionSpeed = true;
        if (CurrentAction->UsesStrikeSpeed())
            strikeSpeed = true;
    }
    CTCLook* look;
    if (GetInterface(&look, 0x43) && look->IsCurrentlyTurningViaAnimation())
        multiplier = look->GetTurnAnimationSpeed();
    CTCHaste* haste;
    if (actionSpeed && GetInterface(&haste, 0x8c)) {
        float h = haste->GetActionSpeedMultiplier();
        multiplier *= h;
        multiplier *= h;
    }
    if (strikeSpeed) {
        float scale = 1.0f;
        float minSpeed = 0.5f;
        float maxSpeed = 2.0f;
        CTCHeroExperience* experience;
        if (GetInterface(&experience, 0x68)) {
            scale = experience->GetMeleeStrikeAnimationSpeedMultiplier();
            minSpeed = experience->GetMinStrikeSpeed();
            maxSpeed = experience->GetMaxStrikeSpeed();
        }
        CTCCarrying* carrying;
        if (GetInterface(&carrying, 0x46)) {
            CThing* weapon = carrying->GetThingInPrimarySlot();
            CTCWeapon* weaponTC;
            if (weapon && !(weapon->Flags91 & 1) && weapon->GetInterface(&weaponTC, 0x48)) {
                float value = weaponTC->GetAnimationSpeedValue();
                if (value < 1.0f)
                    scale = ((1.0f - minSpeed) * (value + 1.0f) + minSpeed) * scale;
                else
                    scale = ((maxSpeed - 1.0f) * value + 1.0f) * scale;
            }
        }
        if (scale < minSpeed)
            scale = minSpeed;
        else if (scale > maxSpeed)
            scale = maxSpeed;
        multiplier *= scale;
    }
    CBulletTime* bulletTime = GetWorld()->BulletTime;
    if ((bulletTime->Active || bulletTime->Frames) && IsAffectedByBulletTime()) {
        float timeScale = GetWorld()->BulletTime->GetTimeScale();
        return sqrtf(multiplier / (timeScale * timeScale));
    }
    return sqrtf(multiplier);
}
