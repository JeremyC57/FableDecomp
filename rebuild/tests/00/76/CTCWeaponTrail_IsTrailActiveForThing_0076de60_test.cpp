#include <cstdio>
struct CThing { int id; };
struct CTCWeaponTrail;
struct CCreatureActionBase {
    CTCWeaponTrail* m_trail; long m_index;
    virtual CTCWeaponTrail* GetWeaponTrailUsedInStrikeAction(long& trailIndex) const { trailIndex = m_index; return m_trail; }
    virtual ~CCreatureActionBase() { }
};
struct CThingCreatureBase { CCreatureActionBase* PCurrentAction; };
struct CTCWeaponTrail {
    bool IsTrailActiveForThing(long trailIndex, const CThing* thing, const CThingCreatureBase* creature) const
    {
        if (thing) return true;
        if (creature && creature->PCurrentAction) {
            long index;
            if (creature->PCurrentAction->GetWeaponTrailUsedInStrikeAction(index) == this && index == trailIndex) return true;
        }
        return false;
    }
};
int main()
{
    CTCWeaponTrail w, other; CThing t; CCreatureActionBase a; a.m_trail = &w; a.m_index = 2; CThingCreatureBase c; c.PCurrentAction = &a;
    if (!w.IsTrailActiveForThing(0, &t, 0)) { std::printf("0076de60_TEST FAIL\n"); return 1; }
    if (w.IsTrailActiveForThing(0, 0, 0)) { std::printf("0076de60_TEST FAIL\n"); return 1; }
    if (!w.IsTrailActiveForThing(2, 0, &c)) { std::printf("0076de60_TEST FAIL\n"); return 1; }
    if (w.IsTrailActiveForThing(1, 0, &c)) { std::printf("0076de60_TEST FAIL\n"); return 1; }
    if (other.IsTrailActiveForThing(2, 0, &c)) { std::printf("0076de60_TEST FAIL\n"); return 1; }
    c.PCurrentAction = 0; if (w.IsTrailActiveForThing(2, 0, &c)) { std::printf("0076de60_TEST FAIL\n"); return 1; }
    std::printf("0076de60_TEST PASS\n");
    return 0;
}