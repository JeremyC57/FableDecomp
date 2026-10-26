// Behaviour test for CTCSpecialAbilities::CancelCurrentMutexAbility (0x0072B420): the same model and body as the
// source, with the interface lookup and every cancel member implemented to record which component was cancelled.
#include <cstdio>
#include <cstring>

class CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* it = first;
        while (it != last && it->type < *type)
            ++it;
        return it;
    }
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T>
    void GetInterface(long type, T** out)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        *out = (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
    template <class T>
    T* GetTC(long type, T*)
    {
        T* tc;
        GetInterface(type, &tc);
        return tc;
    }
};

static long g_cancelledTag = 0;
static void* g_cancelledThis = 0;
#define CANCEL_CLASS(Name, Method, Tag) \
    struct Name { long tag; void Method() { g_cancelledTag = Tag; g_cancelledThis = this; } };
CANCEL_CLASS(CTCForcePushPower, CancelMove, 0x96)
CANCEL_CLASS(CTCBulletTime, CancelMove, 0x92)
CANCEL_CLASS(CTCWillEnflame, CancelMove, 0x95)
CANCEL_CLASS(CTCPhysicalShieldSpell, CancelMove, 0x93)
CANCEL_CLASS(CTCTurncoatSpell, CancelMove, 0x9e)
CANCEL_CLASS(CTCDrainLife, CancelMove, 0x97)
CANCEL_CLASS(CTCSummonSpell, CancelMove, 0x91)
CANCEL_CLASS(CTCWillLightning, CancelMove, 0x94)
CANCEL_CLASS(CTCBattleCharge, CancelMove, 0xa8)
CANCEL_CLASS(CTCAssassinRush, CancelMove, 0xa9)
CANCEL_CLASS(CTCHealLife, CancelMove, 0x99)
CANCEL_CLASS(CTCGhostSword, CancelMove, 0x9a)
CANCEL_CLASS(CTCFireballSpell, CancelMove, 0x9d)
CANCEL_CLASS(CTCDivineWrath, CancelAbilities, 0x85)
CANCEL_CLASS(CTCUnholyPower, CancelAbilities, 0x86)

class CTCSpecialAbilities { public:
    void*   __vftable;
    CThing* ParentThing;
    long    _base08;
    bool    AbilityMutexLock;
    bool    UsingBed;
    long    CurrentAbility;
    void CancelCurrentMutexAbility();
};

void CTCSpecialAbilities::CancelCurrentMutexAbility()
{
    if (!AbilityMutexLock)
        return;
    switch (CurrentAbility) {
    case 1: { CThing* t = ParentThing; if (t->HasTC(0x96)) t->GetTC(0x96, (CTCForcePushPower*)0)->CancelMove(); break; }
    case 2: { CThing* t = ParentThing; if (t->HasTC(0x92)) t->GetTC(0x92, (CTCBulletTime*)0)->CancelMove(); break; }
    case 3: { CThing* t = ParentThing; if (t->HasTC(0x95)) t->GetTC(0x95, (CTCWillEnflame*)0)->CancelMove(); break; }
    case 4: { CThing* t = ParentThing; if (t->HasTC(0x93)) t->GetTC(0x93, (CTCPhysicalShieldSpell*)0)->CancelMove(); break; }
    case 5: { CThing* t = ParentThing; if (t->HasTC(0x9e)) t->GetTC(0x9e, (CTCTurncoatSpell*)0)->CancelMove(); break; }
    case 6: { CThing* t = ParentThing; if (t->HasTC(0x97)) t->GetTC(0x97, (CTCDrainLife*)0)->CancelMove(); break; }
    case 10: { CThing* t = ParentThing; if (t->HasTC(0x91)) t->GetTC(0x91, (CTCSummonSpell*)0)->CancelMove(); break; }
    case 11: { CThing* t = ParentThing; if (t->HasTC(0x94)) t->GetTC(0x94, (CTCWillLightning*)0)->CancelMove(); break; }
    case 12: { CThing* t = ParentThing; if (t->HasTC(0xa8)) t->GetTC(0xa8, (CTCBattleCharge*)0)->CancelMove(); break; }
    case 13: { CThing* t = ParentThing; if (t->HasTC(0xa9)) t->GetTC(0xa9, (CTCAssassinRush*)0)->CancelMove(); break; }
    case 14: { CThing* t = ParentThing; if (t->HasTC(0x99)) t->GetTC(0x99, (CTCHealLife*)0)->CancelMove(); break; }
    case 15: { CThing* t = ParentThing; if (t->HasTC(0x9a)) t->GetTC(0x9a, (CTCGhostSword*)0)->CancelMove(); break; }
    case 16: { CThing* t = ParentThing; if (t->HasTC(0x9d)) t->GetTC(0x9d, (CTCFireballSpell*)0)->CancelMove(); break; }
    case 18: { CThing* t = ParentThing; if (t->HasTC(0x85)) t->GetTC(0x85, (CTCDivineWrath*)0)->CancelAbilities(); break; }
    case 19: { CThing* t = ParentThing; if (t->HasTC(0x86)) t->GetTC(0x86, (CTCUnholyPower*)0)->CancelAbilities(); break; }
    }
}

int main()
{
    static const long abilityType[20] = { 0, 0x96, 0x92, 0x95, 0x93, 0x9e, 0x97, 0, 0, 0, 0x91, 0x94, 0xa8, 0xa9, 0x99,
                                          0x9a, 0x9d, 0, 0x85, 0x86 };
    long types[15]; long n = 0;
    for (long a = 1; a < 20; ++a)
        if (abilityType[a]) types[n++] = abilityType[a];
    // sort the component types for the interface map
    for (long i = 0; i < n; ++i)
        for (long j = i + 1; j < n; ++j)
            if (types[j] < types[i]) { long t = types[i]; types[i] = types[j]; types[j] = t; }
    long components[16];
    CInterfaceMap::SEntry entries[16];
    for (long i = 0; i < n; ++i) { components[i] = types[i]; entries[i].type = types[i]; entries[i].component = (CTCBase*)&components[i]; }
    entries[n].type = 0x7fffffff; entries[n].component = 0;

    CThing thing;
    std::memset(&thing, 0, sizeof(thing));
    thing.m_interfaces.first = entries;
    thing.m_interfaces.last = entries + n;

    CTCSpecialAbilities abilities;
    std::memset(&abilities, 0, sizeof(abilities));
    abilities.ParentThing = &thing;

    int bad = 0;
    for (long pass = 0; pass < 2; ++pass) {
        for (long a = 0; a < 22; ++a) {
            long type = a < 20 ? abilityType[a] : 0;
            std::memset(thing.m_presentBits, 0, sizeof(thing.m_presentBits));
            if (pass == 1 && type)
                thing.m_presentBits[type >> 5] |= 1UL << (type & 31);
            g_cancelledTag = 0; g_cancelledThis = 0;
            abilities.AbilityMutexLock = true;
            abilities.CurrentAbility = a;
            abilities.CancelCurrentMutexAbility();
            long want = pass == 1 ? type : 0;
            if (g_cancelledTag != want) bad = 1;
            if (want) {
                long* comp = 0;
                for (long i = 0; i < n; ++i) if (types[i] == want) comp = &components[i];
                if (g_cancelledThis != comp) bad = 1;
            }
            // lock not held: nothing happens even with the component present
            g_cancelledTag = 0;
            abilities.AbilityMutexLock = false;
            abilities.CancelCurrentMutexAbility();
            if (g_cancelledTag != 0) bad = 1;
        }
    }
    if (bad) { std::printf("0072b420_TEST FAIL\n"); return 1; }
    std::printf("0072b420_TEST PASS\n");
    return 0;
}