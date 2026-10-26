// Behaviour test for 0x006655A0 (copy of the model + stand-in callee bodies).
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
// CThingCreatureBase::GetAnimationSpeedMultiplier() -- the creature's animation speed: turning-via-animation speed,
// haste (squared) for action animations, and the hero's melee-strike speed scaled by the primary weapon's speed value
// and clamped to the hero's range for strike animations; square-rooted, and divided by the squared bullet-time scale
// when bullet time is on and the creature is affected
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
class CTCBase;
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
class CWorld { public:
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
class CCreatureActionBase { public:
#define VF(n) virtual void v##n() { }
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
class CTCLook { public:
    bool IsCurrentlyTurningViaAnimation();      // 0x00753B40
    float GetTurnAnimationSpeed();              // 0x00751B70
};
class CTCHaste { public: float GetActionSpeedMultiplier(); };          // 0x007FFFB0
class CTCHeroExperience { public:
    float GetMeleeStrikeAnimationSpeedMultiplier();             // 0x006D6870
    float GetMinStrikeSpeed();                                  // 0x006D6530
    float GetMaxStrikeSpeed();                                  // 0x006D6540
};
class CTCCarrying { public: CThing* GetThingInPrimarySlot(); };        // 0x006A1440
class CTCWeapon { public: float GetAnimationSpeedValue(); };           // 0x005DA850
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

CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* it = first;
    while (it != last && it->type < *type)
        ++it;
    return it;
}
static float g_timeScale = 0.5f;
float CBulletTime::GetTimeScale() { return g_timeScale; }
static CBulletTime g_bt; static CWorld g_world;
CWorld* CThing::GetWorld() { return &g_world; }
static int g_affected = 1;
bool CThing::IsAffectedByBulletTime() { return g_affected != 0; }
static int g_attack = 0, g_useAction = 0, g_useStrike = 0, g_turning = 0;
bool CCreatureActionBase::IsAttack() { return g_attack != 0; }
bool CCreatureActionBase::UsesActionSpeed() { return g_useAction != 0; }
bool CCreatureActionBase::UsesStrikeSpeed() { return g_useStrike != 0; }
bool CTCLook::IsCurrentlyTurningViaAnimation() { return g_turning != 0; }
float CTCLook::GetTurnAnimationSpeed() { return 4.0f; }
float CTCHaste::GetActionSpeedMultiplier() { return 3.0f; }
float CTCHeroExperience::GetMeleeStrikeAnimationSpeedMultiplier() { return 1.5f; }
float CTCHeroExperience::GetMinStrikeSpeed() { return 0.25f; }
float CTCHeroExperience::GetMaxStrikeSpeed() { return 4.0f; }
static CThing* g_weapon = 0;
CThing* CTCCarrying::GetThingInPrimarySlot() { return g_weapon; }
static float g_weaponValue = 0.0f;
float CTCWeapon::GetAnimationSpeedValue() { return g_weaponValue; }
static void SetBit(CThing* t, long k) { t->UsingComponents[k >> 5] |= 1UL << (k & 31); }
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

int main()
{
    static CThingCreatureBase c; static CThing weapon; static CCreatureActionBase action;
    static CTCLook look; static CTCHaste haste; static CTCHeroExperience xp; static CTCCarrying carry; static CTCWeapon wtc;
    std::memset(&c, 0, sizeof(c)); std::memset(&weapon, 0, sizeof(weapon));
    g_world.BulletTime = &g_bt; g_bt.Active = false; g_bt.Frames = 0;
    CInterfaceMap::SEntry ce[5];
    ce[0].type = 0x43; ce[0].component = (CTCBase*)&look;
    ce[1].type = 0x46; ce[1].component = (CTCBase*)&carry;
    ce[2].type = 0x68; ce[2].component = (CTCBase*)&xp;
    ce[3].type = 0x8c; ce[3].component = (CTCBase*)&haste;
    ce[4].type = 0x90; ce[4].component = 0;
    c.Components.first = ce; c.Components.last = ce + 5;
    CInterfaceMap::SEntry we[1]; we[0].type = 0x48; we[0].component = (CTCBase*)&wtc;
    weapon.Components.first = we; weapon.Components.last = we + 1;
    // nothing: sqrt(1)
    if (!Near(c.GetAnimationSpeedMultiplier(), 1.0f)) { std::printf("006655a0_TEST FAIL\n"); return 1; }
    // turning via animation: sqrt(4) = 2
    SetBit(&c, 0x43); g_turning = 1;
    if (!Near(c.GetAnimationSpeedMultiplier(), 2.0f)) { std::printf("006655a0_TEST FAIL\n"); return 1; }
    g_turning = 0;
    // action speed with haste: 1*3*3 -> 3
    c.CurrentAction = &action; action.Finished = false; g_useAction = 1; SetBit(&c, 0x8c);
    if (!Near(c.GetAnimationSpeedMultiplier(), 3.0f)) { std::printf("006655a0_TEST FAIL\n"); return 1; }
    // finished action: ignored
    action.Finished = true;
    if (!Near(c.GetAnimationSpeedMultiplier(), 1.0f)) { std::printf("006655a0_TEST FAIL\n"); return 1; }
    action.Finished = false; g_useAction = 0;
    // category 100/100 -> both speeds; no xp/carrying: scale 1 in [0.5, 2] -> 9 -> 3
    action.Category = 100; action.SubCategory = 100;
    if (!Near(c.GetAnimationSpeedMultiplier(), 3.0f)) { std::printf("006655a0_TEST FAIL\n"); return 1; }
    action.Category = 0;
    // strike speed only, hero experience: scale 1.5 -> sqrt(1.5)
    g_useStrike = 1; SetBit(&c, 0x68);
    float r = c.GetAnimationSpeedMultiplier();
    if (!Near(r * r, 1.5f)) { std::printf("006655a0_TEST FAIL\n"); return 1; }
    // carrying a weapon with value 0 (< 1): ((1-0.25)*(0+1)+0.25)*1.5 = 1.5
    SetBit(&c, 0x46); g_weapon = &weapon; SetBit(&weapon, 0x48); g_weaponValue = 0.0f;
    r = c.GetAnimationSpeedMultiplier(); if (!Near(r * r, 1.5f)) { std::printf("006655a0_TEST FAIL\n"); return 1; }
    // value -0.5: (0.75*0.5+0.25)*1.5 = 0.9375
    g_weaponValue = -0.5f; r = c.GetAnimationSpeedMultiplier(); if (!Near(r * r, 0.9375f)) { std::printf("006655a0_TEST FAIL\n"); return 1; }
    // value 2: ((4-1)*2+1)*1.5 = 10.5 -> clamped to 4
    g_weaponValue = 2.0f; r = c.GetAnimationSpeedMultiplier(); if (!Near(r * r, 4.0f)) { std::printf("006655a0_TEST FAIL\n"); return 1; }
    // value -3: (0.75*-2+0.25)*1.5 = -1.875 -> clamped to 0.25
    g_weaponValue = -3.0f; r = c.GetAnimationSpeedMultiplier(); if (!Near(r * r, 0.25f)) { std::printf("006655a0_TEST FAIL\n"); return 1; }
    // dead weapon: scale stays 1.5
    weapon.Flags91 = 1; r = c.GetAnimationSpeedMultiplier(); if (!Near(r * r, 1.5f)) { std::printf("006655a0_TEST FAIL\n"); return 1; }
    weapon.Flags91 = 0; g_weapon = 0; g_useStrike = 0;
    // bullet time: 1 / 0.25 = 4 -> 2; not affected -> 1
    g_bt.Frames = 3;
    if (!Near(c.GetAnimationSpeedMultiplier(), 2.0f)) { std::printf("006655a0_TEST FAIL\n"); return 1; }
    g_affected = 0;
    if (!Near(c.GetAnimationSpeedMultiplier(), 1.0f)) { std::printf("006655a0_TEST FAIL\n"); return 1; }
    g_affected = 1; g_bt.Frames = 0; g_bt.Active = true;
    if (!Near(c.GetAnimationSpeedMultiplier(), 2.0f)) { std::printf("006655a0_TEST FAIL\n"); return 1; }
    std::printf("006655a0_TEST PASS\n");
    return 0;
}