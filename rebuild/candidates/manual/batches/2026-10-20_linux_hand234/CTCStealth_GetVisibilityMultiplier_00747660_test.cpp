// Behaviour test for 0x00747660 CTCStealth::GetVisibilityMultiplier (copy of the source logic over spy components).
#include <cstdio>
#include <cstring>
struct CTCBase { virtual ~CTCBase() {} };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const { const SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> T* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
    template <class T> bool GetInterface(long type, T** out) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CGameTimeManager {
    int phase;
    bool IsDay() const { return phase == 0; }
    bool IsNightTime() const { return phase == 1; }
};
struct CWorld {
    unsigned char     _pad00[0x1c];
    CGameTimeManager* GameTimeManager;                  // +0x1c
};
struct CStealthDef {
    float DayTimeLampVisibilityMultiplier;                  // +0x38
    float NightTimeLampVisibilityMultiplier;                // +0x3c
};
struct CTCStealthModifier27 : CTCBase {                           // interface 0x27 (named descriptively)
    bool          AffectsVisibility;                    // +0x1c
};
struct CTCVisibility68 : CTCBase {
    float m; float GetVisibilityMultiplier() const { return m; }
};
struct CTCVisibility4 : CTCBase {
    float         VisibilityFactor;
};
struct CTCStealth {
    CThing*       ParentThing;
    const CStealthDef* PDef;
    CWorld* world;
    CWorld* GetWorld() const { return world; }
    const CStealthDef* GetDef() const { return PDef; }
    float GetTimeOfDayVisibilityMultiplier() const
    {
        if (GetWorld()->GameTimeManager->IsDay())
            return GetDef()->DayTimeLampVisibilityMultiplier;
        if (GetWorld()->GameTimeManager->IsNightTime())
            return GetDef()->NightTimeLampVisibilityMultiplier;
        return (GetDef()->NightTimeLampVisibilityMultiplier + GetDef()->DayTimeLampVisibilityMultiplier) * 0.5f;
    }
    float GetVisibilityMultiplier() const;
};

float CTCStealth::GetVisibilityMultiplier() const
{
    float multiplier = 1.0f;
    CTCStealthModifier27* modifier;
    if (ParentThing->GetInterface(0x27, &modifier) && modifier->AffectsVisibility)
        multiplier = GetTimeOfDayVisibilityMultiplier();
    if (ParentThing->HasTC(0x68))
        multiplier *= ParentThing->GetTC<CTCVisibility68>(0x68)->GetVisibilityMultiplier();
    CTCVisibility4* visibility;
    if (ParentThing->GetInterface(4, &visibility))
        return multiplier * visibility->VisibilityFactor;
    return multiplier;
}

static int fail() { std::printf("00747660_TEST FAIL\n"); return 1; }
int main()
{
    CTCStealthModifier27 mod; mod.AffectsVisibility = true;
    CTCVisibility68 v68; v68.m = 0.5f;
    CTCVisibility4 v4; v4.VisibilityFactor = 3.0f;
    CInterfaceMap::SEntry e[3] = { { 4, &v4 }, { 0x27, &mod }, { 0x68, &v68 } };
    CThing hero; std::memset(hero.m_presentBits, 0, sizeof hero.m_presentBits);
    hero.m_interfaces.first = e; hero.m_interfaces.last = e + 3;
    CGameTimeManager gtm; gtm.phase = 0;
    CWorld world; world.GameTimeManager = &gtm;
    CStealthDef def; def.DayTimeLampVisibilityMultiplier = 2.0f; def.NightTimeLampVisibilityMultiplier = 0.25f;
    CTCStealth s; s.ParentThing = &hero; s.PDef = &def; s.world = &world;
    if (s.GetVisibilityMultiplier() != 1.0f) return fail();                 // no components
    hero.m_presentBits[0x27 >> 5] |= 1UL << (0x27 & 31);
    if (s.GetVisibilityMultiplier() != 2.0f) return fail();                 // day
    gtm.phase = 1; if (s.GetVisibilityMultiplier() != 0.25f) return fail(); // night
    gtm.phase = 2; if (s.GetVisibilityMultiplier() != 1.125f) return fail(); // twilight: (0.25 + 2) * 0.5
    hero.m_presentBits[0x68 >> 5] |= 1UL << (0x68 & 31);
    if (s.GetVisibilityMultiplier() != 0.5625f) return fail();
    hero.m_presentBits[0] |= 1UL << 4;
    if (s.GetVisibilityMultiplier() != 1.6875f) return fail();
    mod.AffectsVisibility = false;
    if (s.GetVisibilityMultiplier() != 1.5f) return fail();                 // 1 * 0.5 * 3
    std::printf("00747660_TEST PASS\n");
    return 0;
}
