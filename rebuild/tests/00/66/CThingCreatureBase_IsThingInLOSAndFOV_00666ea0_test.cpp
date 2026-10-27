// 00666ea0 behaviour test for CThingCreatureBase::IsThingInLOSAndFOV(const CThing&) const
#include <cstdio>
#include <cstdlib>
#ifndef _MSC_VER
#ifndef __fastcall
#define __fastcall
#endif
#ifndef __cdecl
#define __cdecl
#endif
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { std::printf("check failed at line %d\n", __LINE__); g_fail = 1; } } while (0)
static int g_losCalls = 0, g_fovCalls = 0;
class CWorldMap;
class CThing {
public:
    unsigned char Dead : 1;
    bool InFOV;                                             // test inputs
    bool InLOS;
    CThing() : Dead(0), InFOV(false), InLOS(false) { }
    bool IsDead() const { return Dead; }
    const CWorldMap& PeekWorldMap() const;
};
class CWorldMap {
public:
    bool IsLineOfSight(const CThing& from, const CThing& to, bool flag) const { (void)from; ++g_losCalls; return flag && to.InLOS; }
};
static CWorldMap g_map;
const CWorldMap& CThing::PeekWorldMap() const { return g_map; }
class CThingCreatureBase : public CThing {
public:
    bool IsThingInFOV(const CThing& thing, bool flag) const { ++g_fovCalls; return flag && thing.InFOV; }
    bool IsThingInLOS(const CThing& thing) const { return PeekWorldMap().IsLineOfSight(*this, thing, true); }
    bool IsThingInLOSAndFOV(const CThing& thing) const;
};
bool CThingCreatureBase::IsThingInLOSAndFOV(const CThing& thing) const
{
    if (thing.IsDead())
        return false;
    if (!IsThingInFOV(thing, true))
        return false;
    if (!IsThingInLOS(thing))
        return false;
    return true;
}
static void run()
{
    CThingCreatureBase me; CThing t;
    t.InFOV = true; t.InLOS = true;
    CHECK(me.IsThingInLOSAndFOV(t));
    t.Dead = 1;
    CHECK(!me.IsThingInLOSAndFOV(t));
    CHECK(g_fovCalls == 1 && g_losCalls == 1);                  // a dead thing is rejected before either query
    t.Dead = 0; t.InFOV = false;
    CHECK(!me.IsThingInLOSAndFOV(t));
    CHECK(g_losCalls == 1);                                     // out of view: no line-of-sight query
    t.InFOV = true; t.InLOS = false;
    CHECK(!me.IsThingInLOSAndFOV(t));
}

int main()
{
    run();
    if (g_fail) {
        std::printf("00666ea0_TEST FAIL\n");
        return 1;
    }
    std::printf("00666ea0_TEST PASS\n");
    return 0;
}