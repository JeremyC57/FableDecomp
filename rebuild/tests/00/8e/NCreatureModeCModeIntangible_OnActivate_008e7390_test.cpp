// Behaviour test for NCreatureMode::CModeIntangible::OnActivate (0x008E7390): the call order, the cleared flags, the
// remembered Damageable bit, the boss phase and the weapon-trail clear, on a host copy of the model.
#include <cstdio>
#include <cstring>

static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }

class CTCBase { public: int dummy; };
class CTCMapwho : public CTCBase { public: unsigned char InMapwho : 1; };
class CTCTargeted : public CTCBase { public: bool Targetable; };
class CTCBoss : public CTCBase { public: long phase; long GetBossPhase() { Log("phase"); return phase; } };
class CTCWeaponTrail : public CTCBase { public: int cleared; void ClearAllWeaponTrails() { ++cleared; Log("trails"); } };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
    const SEntry* LowerBound(const long* type) const
    {
        const SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
};
class CTCPhysicsBase { public:
    bool active, bit85;
    unsigned long flags;
    void SetFlag85Bit2(bool on) { bit85 = on; Log("bit85"); }
    void SetPhysicsActive(bool on) { active = on; Log("active"); }
    void SetPhysicsFlags(unsigned long f) { flags = f; Log("flags"); }
};
struct CModeBase;
class CThing { public:
    unsigned char m_type;
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    CTCPhysicsBase* PhysicsTC;
    unsigned char Damageable : 1;
    bool drawable, suspended;
    CModeBase* suspender;
    void SetAsDrawable(bool d) { drawable = d; Log("drawable"); }
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template <class T> T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        const CInterfaceMap::SEntry* sel = (it == map->last || interfaceType < it->type) ? map->last : it;
        return (T*)sel->component;
    }
    template <class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (HasTC(interfaceType)) {
            *ppOut = GetTC<T>(interfaceType);
            return true;
        }
        return false;
    }
    void SetSuspendAnimation(bool s, CModeBase* mode) { suspended = s; suspender = mode; Log("suspend"); }
};
static bool g_notWillable = false;
static void SetThingAsNotBloodyWillableAtAll(CThing*, bool notWillable) { g_notWillable = notWillable; Log("will"); }

struct CModeBase {
    CThing* ParentThing;
    bool Active;
    void OnActivate() { Active = true; Log("base"); }
};
struct CModeIntangible : CModeBase {
    long Timer, BossPhase;
    bool WasDamageable;
    void OnActivate()
    {
        ParentThing->PhysicsTC->SetPhysicsActive(false);
        CModeBase::OnActivate();
        ParentThing->SetAsDrawable(false);
        CTCMapwho* mapwho;
        if (ParentThing->GetInterface(&mapwho, 0x3a))
            mapwho->InMapwho = 0;
        ParentThing->PhysicsTC->SetFlag85Bit2(false);
        ParentThing->PhysicsTC->SetPhysicsFlags(0x20);
        SetThingAsNotBloodyWillableAtAll(ParentThing, true);
        CTCTargeted* targeted;
        if (ParentThing->GetInterface(&targeted, 9))
            targeted->Targetable = false;
        CThing* thing = ParentThing;
        if (thing->m_type == 1) {
            thing->SetSuspendAnimation(true, this);
            WasDamageable = thing->Damageable;
        }
        if (ParentThing->HasTC(0xad))
            BossPhase = ParentThing->GetTC<CTCBoss>(0xad)->GetBossPhase();
        if (ParentThing->HasTC(0x4c))
            ParentThing->GetTC<CTCWeaponTrail>(0x4c)->ClearAllWeaponTrails();
    }
};

int main()
{
    CTCTargeted targeted; targeted.Targetable = true;
    CTCMapwho mapwho; mapwho.InMapwho = 1;
    CTCWeaponTrail trail; trail.cleared = 0;
    CTCBoss boss; boss.phase = 3;
    CInterfaceMap::SEntry entries[5] = { { 9, &targeted }, { 0x3a, &mapwho }, { 0x4c, &trail }, { 0xad, &boss }, { 0, 0 } };
    CTCPhysicsBase physics; physics.active = true; physics.bit85 = true; physics.flags = 0;
    CThing thing; std::memset(thing.m_presentBits, 0, sizeof(thing.m_presentBits));
    thing.m_presentBits[0] = 1UL << 9; thing.m_presentBits[1] = 1UL << 26;
    thing.m_presentBits[0x4c >> 5] |= 1UL << (0x4c & 31); thing.m_presentBits[0xad >> 5] |= 1UL << (0xad & 31);
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 4;
    thing.PhysicsTC = &physics; thing.m_type = 1; thing.Damageable = 1; thing.drawable = true;
    thing.suspended = false; thing.suspender = 0;
    CModeIntangible mode; mode.ParentThing = &thing; mode.Active = false; mode.WasDamageable = false; mode.BossPhase = -1;
    g_log[0] = 0;
    mode.OnActivate();
    bool ok = mode.Active && !physics.active && !physics.bit85 && physics.flags == 0x20 && !thing.drawable
        && mapwho.InMapwho == 0 && !targeted.Targetable && g_notWillable && thing.suspended && thing.suspender == &mode
        && mode.WasDamageable && mode.BossPhase == 3 && trail.cleared == 1
        && std::strcmp(g_log, "active;base;drawable;bit85;flags;will;suspend;phase;trails;") == 0;
    // A non-player thing without boss or trail components: no suspend, phase untouched.
    thing.m_type = 0; thing.m_presentBits[0x4c >> 5] = 0; thing.m_presentBits[0xad >> 5] = 0; thing.suspended = false;
    mode.WasDamageable = false; mode.BossPhase = -1;
    mode.OnActivate();
    ok = ok && !thing.suspended && !mode.WasDamageable && mode.BossPhase == -1 && trail.cleared == 1;
    if (!ok) { std::printf("008e7390_TEST FAIL\n"); return 1; }
    std::printf("008e7390_TEST PASS\n");
    return 0;
}