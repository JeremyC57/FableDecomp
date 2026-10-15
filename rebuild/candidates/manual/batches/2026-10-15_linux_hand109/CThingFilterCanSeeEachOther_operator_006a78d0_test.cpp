// Behaviour test for CThingFilter_CanSeeEachOther::operator() (0x006A78D0)
#include <cstdio>
#include <cstring>

struct CThingCreatureBase;
struct CThing {
    unsigned char m_typeFlags;
    const CThingCreatureBase* AsCreature() const
    {
        const CThingCreatureBase* creature = 0;
        if (m_typeFlags & 0x10)
            creature = (const CThingCreatureBase*)this;
        return creature;
    }
};
struct CThingCreatureBase : CThing { const CThingCreatureBase* m_canSee; };
static int g_calls = 0;
static bool CanCreatureSeeCreature(const CThingCreatureBase* viewer, const CThingCreatureBase* target, bool)
{
    ++g_calls;
    return viewer->m_canSee == target;
}
inline bool CanSeeEachOther(const CThingCreatureBase* a, const CThingCreatureBase* b)
{
    return CanCreatureSeeCreature(a, b, false) && CanCreatureSeeCreature(b, a, false);
}
struct CThingFilter_CanSeeEachOther {
    const CThing* m_thing;
    bool operator()(const CThingCreatureBase& creature) const
    {
        const CThingCreatureBase* a = m_thing->AsCreature();
        const CThingCreatureBase* b = creature.AsCreature();
        if (a && b)
            return CanSeeEachOther(a, b);
        return false;
    }
};
#define CHECK(c) if (!(c)) { std::printf("006a78d0_TEST FAIL\n"); return 1; }
int main()
{
    CThingCreatureBase a, b; a.m_typeFlags = 0x10; b.m_typeFlags = 0x10; a.m_canSee = &b; b.m_canSee = &a;
    CThingFilter_CanSeeEachOther f = { &a };
    CHECK(f(b) && g_calls == 2);
    b.m_canSee = 0; g_calls = 0;
    CHECK(!f(b) && g_calls == 2);
    a.m_canSee = 0; g_calls = 0;
    CHECK(!f(b) && g_calls == 1);                   // short-circuit after the first refusal
    b.m_typeFlags = 0; a.m_canSee = &b; b.m_canSee = &a; g_calls = 0;
    CHECK(!f(b) && g_calls == 0);                   // not a creature: no visibility test
    std::printf("006a78d0_TEST PASS\n");
    return 0;
}
