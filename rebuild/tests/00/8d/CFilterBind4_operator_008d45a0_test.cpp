#include <cstdio>
#include <cstring>
// Model of CFilter_Bind4<ScriptName, HitEventWithWeapon, CreatedBeforeOrOn, CreatedAfter>::operator()<CEventHitBy*>
// @ 0x008D45A0: the short-circuit conjunction of the four bound filters on a hit event.
class CThing {
public:
    unsigned short m_defIndex;
    unsigned char  m_flags91;                           // bit 0: flagged dead
    bool IsFlaggedDead() const { return (m_flags91 & 1) != 0; }
};
static int g_scriptNameCalls = 0;
class CEventBase {
public:
    virtual ~CEventBase() { }
    int WorldFrame;
};
class CEventHitBase : public CEventBase {
public:
    CThing*     Weapon;
    const char* HitterScriptName;
    CThing* GetPWeaponUsed() const { return Weapon; }   // 0x00810FD0
};
class CEventHitBy : public CEventHitBase { };
class CIsHitEventHitByThingWithScriptName {             // stand-in for the out-of-line 0x00811100
public:
    const char* ScriptName;
    bool operator()(const CEventHitBase* event) const
    {
        ++g_scriptNameCalls;
        return event->HitterScriptName != 0 && std::strcmp(event->HitterScriptName, ScriptName) == 0;
    }
};
class CIsEventCreatedBeforeOrOn {
public:
    int WorldFrame;
    bool operator()(const CEventBase* event) const { return event->WorldFrame <= WorldFrame; }
};
class CIsEventCreatedAfter {
public:
    int WorldFrame;
    bool operator()(const CEventBase* event) const { return event->WorldFrame > WorldFrame; }
};
class CEventFilter_HitEventWithWeapon {
public:
    int WeaponDefIndex;
    bool operator()(const CEventHitBase* event) const
    {
        CThing* weapon = event->GetPWeaponUsed();
        if (weapon && !weapon->IsFlaggedDead() && weapon->m_defIndex == WeaponDefIndex)
            return true;
        return false;
    }
};
template <class T1, class T2, class T3, class T4>
class CFilter_Bind4 {
public:
    const T1* F1;
    const T2* F2;
    const T3* F3;
    const T4* F4;
    template <class T>
    bool operator()(const T& t) const
    {
        return (*F1)(t) && (*F2)(t) && (*F3)(t) && (*F4)(t);
    }
};
typedef CFilter_Bind4<CIsHitEventHitByThingWithScriptName, CEventFilter_HitEventWithWeapon,
                      CIsEventCreatedBeforeOrOn, CIsEventCreatedAfter> CHitFilter;
int main()
{
    bool ok = true;
    CIsHitEventHitByThingWithScriptName byName = { "Bandit" };
    CEventFilter_HitEventWithWeapon withWeapon = { 12 };
    CIsEventCreatedBeforeOrOn beforeOrOn = { 100 };
    CIsEventCreatedAfter after = { 50 };
    CHitFilter filter;
    filter.F1 = &byName;
    filter.F2 = &withWeapon;
    filter.F3 = &beforeOrOn;
    filter.F4 = &after;
    CThing sword = { 12, 0 };
    CThing deadSword = { 12, 1 };
    CThing axe = { 13, 0 };
    CEventHitBy hit;
    hit.WorldFrame = 100;                               // on the upper bound: still accepted
    hit.Weapon = &sword;
    hit.HitterScriptName = "Bandit";
    CEventHitBy* p = &hit;
    if (!filter(p)) ok = false;
    hit.WorldFrame = 50;                                // not after 50
    if (filter(p)) ok = false;
    hit.WorldFrame = 75;
    hit.Weapon = &deadSword;
    if (filter(p)) ok = false;
    hit.Weapon = &axe;
    if (filter(p)) ok = false;
    hit.Weapon = 0;
    if (filter(p)) ok = false;
    hit.Weapon = &sword;
    hit.HitterScriptName = "Hero";
    int calls = g_scriptNameCalls;
    if (filter(p) || g_scriptNameCalls != calls + 1) ok = false;
    if (!ok) { std::printf("008d45a0_TEST FAIL\n"); return 1; }
    std::printf("008d45a0_TEST PASS\n");
    return 0;
}