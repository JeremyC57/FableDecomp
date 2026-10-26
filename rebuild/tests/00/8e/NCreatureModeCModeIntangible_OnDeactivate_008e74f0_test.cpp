// Behaviour test for NCreatureMode::CModeIntangible::OnDeactivate (0x008E74F0): the call order, the bit/flag stores and
// the refused-action fallback, on a host copy of the model.
#include <cstdio>
#include <cstring>

static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }

class CTCBase { public: int dummy; };
class CTCMapwho : public CTCBase { public: unsigned char InMapwho : 1; };
class CTCTargeted : public CTCBase { public: bool Targetable; };
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
};
class CTCPhysicsBase { public:
    bool active, bit85;
    int resets;
    void SetFlag85Bit2(bool on) { bit85 = on; Log("bit85"); }
    void SetPhysicsActive(bool on) { active = on; Log("active"); }
    void ResetPhysicsFlagsToDef() { ++resets; Log("reset"); }
};
struct CModeBase;
class CCreatureActionBase { public:
    int refused;
    void OnRefused() { ++refused; Log("refused"); }
};
template <class T> class CCountedPointer { public:
    T* Data;
    void* Info;
    T* operator->() const { return Data; }
};
class CThing { public:
    unsigned char m_type;
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    CTCPhysicsBase* PhysicsTC;
    unsigned char Damageable : 1;
    bool drawable, suspended, accept;
    CModeBase* suspender;
    void SetAsDrawable(bool d) { drawable = d; Log("drawable"); }
    template <class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    void SetSuspendAnimation(bool s, CModeBase* mode) { suspended = s; suspender = mode; Log("suspend"); }
    bool SetCurrentAction(const CCountedPointer<CCreatureActionBase>&) { Log("setaction"); return accept; }
};
static bool g_willable = true;
static void SetThingAsNotBloodyWillableAtAll(CThing*, bool notWillable) { g_willable = !notWillable; Log("will"); }

struct CModeBase {
    CThing* ParentThing;
    bool Active;
    void OnDeactivate() { Active = false; }
};
struct CModeIntangible : CModeBase {
    long Timer, BossPhase;
    bool WasDamageable;
    CCountedPointer<CCreatureActionBase> PActionOnDeactivate;
    void OnDeactivate()
    {
        ParentThing->PhysicsTC->SetPhysicsActive(true);
        CModeBase::OnDeactivate();
        ParentThing->SetAsDrawable(false);
        CTCMapwho* mapwho;
        if (ParentThing->GetInterface(&mapwho, 0x3a))
            mapwho->InMapwho = 1;
        ParentThing->PhysicsTC->SetFlag85Bit2(true);
        ParentThing->PhysicsTC->ResetPhysicsFlagsToDef();
        SetThingAsNotBloodyWillableAtAll(ParentThing, false);
        CTCTargeted* targeted;
        if (ParentThing->GetInterface(&targeted, 9))
            targeted->Targetable = true;
        CThing* thing = ParentThing;
        if (thing->m_type == 1) {
            thing->SetSuspendAnimation(false, this);
            thing->Damageable = WasDamageable;
            if (PActionOnDeactivate.Data) {
                if (!thing->SetCurrentAction(PActionOnDeactivate))
                    PActionOnDeactivate->OnRefused();
            }
        }
    }
};

static int Run(unsigned char type, bool accept, bool hasAction)
{
    CTCTargeted targeted; targeted.Targetable = false;
    CTCMapwho mapwho; mapwho.InMapwho = 0;
    CInterfaceMap::SEntry entries[3] = { { 9, &targeted }, { 0x3a, &mapwho }, { 0, 0 } };
    CTCPhysicsBase physics; physics.active = false; physics.bit85 = false; physics.resets = 0;
    CThing thing; std::memset(thing.m_presentBits, 0, sizeof(thing.m_presentBits));
    thing.m_presentBits[0] = 1UL << 9; thing.m_presentBits[1] = 1UL << 26;
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 2;
    thing.PhysicsTC = &physics; thing.m_type = type; thing.Damageable = 0; thing.drawable = true;
    thing.suspended = true; thing.accept = accept; thing.suspender = 0;
    CCreatureActionBase action; action.refused = 0;
    CModeIntangible mode; mode.ParentThing = &thing; mode.Active = true; mode.WasDamageable = true;
    mode.PActionOnDeactivate.Data = hasAction ? &action : 0; mode.PActionOnDeactivate.Info = 0;
    g_log[0] = 0; g_willable = false;
    mode.OnDeactivate();
    if (mode.Active || !physics.active || !physics.bit85 || physics.resets != 1 || thing.drawable) return 1;
    if (mapwho.InMapwho != 1 || !targeted.Targetable || !g_willable) return 2;
    if (type == 1) {
        if (thing.suspended || thing.suspender != &mode || thing.Damageable != 1) return 3;
        int expect = (hasAction && !accept) ? 1 : 0;
        if (action.refused != expect) return 4;
    } else if (!thing.suspended || thing.Damageable != 0 || action.refused) return 5;
    return 0;
}

int main()
{
    if (Run(1, false, true) || Run(1, true, true) || Run(1, false, false) || Run(0, false, true)) {
        std::printf("008e74f0_TEST FAIL\n"); return 1;
    }
    Run(1, false, true);
    if (std::strcmp(g_log, "active;drawable;bit85;reset;will;suspend;setaction;refused;") != 0) {
        std::printf("008e74f0_TEST FAIL\n"); return 1;
    }
    std::printf("008e74f0_TEST PASS\n");
    return 0;
}