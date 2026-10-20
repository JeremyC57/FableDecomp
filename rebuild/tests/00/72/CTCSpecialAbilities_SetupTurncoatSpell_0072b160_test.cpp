// Behaviour test for 0x0072B160 CTCSpecialAbilities::SetupTurncoatSpell (copy of the source logic over spy callees).
#include <cstdio>
#include <cstring>
struct CTCBase { virtual ~CTCBase() {} };
struct CCharString { const char* text; CCharString(const char* t, long = -1) : text(t) {} };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
    const SEntry* LowerBound(const long* type) const { const SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
struct CTCParamsBase;
class CThing;
struct CTCTargeter : CTCBase { CThing* target; CThing* GetPTarget() { return target; } };
static CTCBase* g_added = 0; static const char* g_addedName = 0; static bool g_addedActive = false; static int g_adds = 0;
class CThing {
public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned long ThingTypeFlags;
    unsigned char m_flags91;
    CTCBase* AddTC(const CCharString& name, bool activate, CTCParamsBase*) { ++g_adds; g_addedName = name.text; g_addedActive = activate; return g_added; }
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> T* GetTC(long type)
    {
        long key = type; CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
    template <class T> T* GetTC(long type) const
    {
        long key = type; const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
class CThingCreatureBase : public CThing {};
static bool g_enemies = true;
static bool AreThingsEnemies(const CThing*, const CThing*) { return g_enemies; }
struct CTCTurncoat : CTCBase {
    bool turncoatable, turncoated;
    bool IsTurncoatable() { return turncoatable; }
    bool IsTurncoated() { return turncoated; }
};
struct CTCTurncoatSpell : CTCBase { CThingCreatureBase* target; void SetPTarget(CThingCreatureBase* t) { target = t; } };
struct CTCSpecialAbilities {
    CThing* ParentThing;
    CThingCreatureBase* Creature;
    bool SetupTurncoatSpell();
};
bool CTCSpecialAbilities::SetupTurncoatSpell()
{
    const CThing* parent = ParentThing;
    if (parent->HasTC(8)) {
        CThing* target = parent->GetTC<CTCTargeter>(8)->GetPTarget();
        if (target && !(target->m_flags91 & 1) && (target->ThingTypeFlags & 8)
            && AreThingsEnemies(ParentThing, target) && target->HasTC(0x9f)) {
            CTCTurncoat* turncoat = target->GetTC<CTCTurncoat>(0x9f);
            if (turncoat->IsTurncoatable() && !turncoat->IsTurncoated()) {
                CTCTurncoatSpell* spell = (CTCTurncoatSpell*)Creature->AddTC("CTCTurncoatSpell", true, 0);
                spell->SetPTarget((CThingCreatureBase*)target);
            }
        }
    }
    return false;
}
static int fail() { std::printf("0072b160_TEST FAIL\n"); return 1; }
int main()
{
    CThingCreatureBase victim; std::memset(victim.m_presentBits, 0, sizeof victim.m_presentBits);
    CTCTurncoat tc; tc.turncoatable = true; tc.turncoated = false;
    CInterfaceMap::SEntry ve[1] = { { 0x9f, &tc } };
    victim.m_interfaces.first = ve; victim.m_interfaces.last = ve + 1;
    victim.m_presentBits[0x9f >> 5] = 1UL << (0x9f & 31); victim.ThingTypeFlags = 8; victim.m_flags91 = 0;
    CTCTargeter targeter; targeter.target = &victim;
    CThing hero; std::memset(hero.m_presentBits, 0, sizeof hero.m_presentBits);
    CInterfaceMap::SEntry he[1] = { { 8, &targeter } };
    hero.m_interfaces.first = he; hero.m_interfaces.last = he + 1; hero.m_presentBits[0] = 1UL << 8;
    CThingCreatureBase heroCreature;
    CTCTurncoatSpell spell; spell.target = 0; g_added = &spell;
    CTCSpecialAbilities sa; sa.ParentThing = &hero; sa.Creature = &heroCreature;
    if (sa.SetupTurncoatSpell()) return fail();
    if (g_adds != 1 || std::strcmp(g_addedName, "CTCTurncoatSpell") != 0 || !g_addedActive || spell.target != &victim) return fail();
    tc.turncoated = true;  if (sa.SetupTurncoatSpell() || g_adds != 1) return fail();      // already turned
    tc.turncoated = false; g_enemies = false; sa.SetupTurncoatSpell(); if (g_adds != 1) return fail();   // not an enemy
    g_enemies = true; victim.ThingTypeFlags = 0; sa.SetupTurncoatSpell(); if (g_adds != 1) return fail(); // not a creature
    victim.ThingTypeFlags = 8; victim.m_flags91 = 1; sa.SetupTurncoatSpell(); if (g_adds != 1) return fail(); // dead
    victim.m_flags91 = 0; sa.SetupTurncoatSpell(); if (g_adds != 2) return fail();
    std::printf("0072b160_TEST PASS\n");
    return 0;
}