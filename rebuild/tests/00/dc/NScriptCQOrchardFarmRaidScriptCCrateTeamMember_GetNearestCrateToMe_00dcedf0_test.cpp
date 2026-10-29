// Behaviour test for 0x00DCEDF0 NScript::CQ_OrchardFarmRaidScript::CCrateTeamMember::GetNearestCrateToMe: the crate nearest to
// the member is returned (the first one on ties), an empty list or crates 1e7 or more away give a null thing; the counted
// handles keep their reference counts balanced.
#include <cstdio>
#include <cmath>

class CCPPointerInfo { public: unsigned int RefCount; };
class CScriptThing { public:
    float X; CCPPointerInfo* Info;
    CScriptThing() : X(0.0f), Info(0) { }
    CScriptThing(const CScriptThing& o) : X(o.X), Info(o.Info) { if (Info) ++Info->RefCount; }
    CScriptThing& operator=(const CScriptThing& o)
    {
        CCPPointerInfo* info = o.Info; float x = o.X;
        if (Info != info) { if (Info) --Info->RefCount; X = x; Info = info; if (Info) ++Info->RefCount; }
        return *this;
    }
    ~CScriptThing() { if (Info) --Info->RefCount; Info = 0; }
};
static float GetDistanceBetweenThings(const CScriptThing& a, const CScriptThing& b) { return (float)std::fabs(a.X - b.X); }
struct CQ_OrchardFarmRaidScript { CScriptThing* CrateList; unsigned int Count;
    unsigned int size() const { return Count; } };
struct CCrateTeamMember {
    CScriptThing Me; CQ_OrchardFarmRaidScript* ParentClass;
    CScriptThing GetNearestCrateToMe()
    {
        float nearest = 10000000.0f;
        CScriptThing crate;
        for (unsigned int i = 0; i < ParentClass->size(); ++i) {
            float distance = GetDistanceBetweenThings(Me, ParentClass->CrateList[i]);
            if (distance < nearest) {
                nearest = distance;
                crate = ParentClass->CrateList[i];
            }
        }
        return crate;
    }
};

int main()
{
    CCPPointerInfo info[4] = { { 1 }, { 1 }, { 1 }, { 1 } };
    CScriptThing crates[4];
    float xs[4] = { 50.0f, 12.0f, 8.0f, 20000000.0f };
    for (int i = 0; i < 4; ++i) { crates[i].X = xs[i]; crates[i].Info = &info[i]; }
    CQ_OrchardFarmRaidScript q; q.CrateList = crates; q.Count = 3;
    CCrateTeamMember m; m.Me.X = 10.0f; m.ParentClass = &q;
    int ok = 1;
    {
        CScriptThing n = m.GetNearestCrateToMe();          // 12 and 8 are both 2 away: the first one wins
        ok = ok && n.Info == &info[1] && info[1].RefCount == 2 && info[0].RefCount == 1 && info[2].RefCount == 1;
    }
    ok = ok && info[1].RefCount == 1;
    q.CrateList = crates + 3; q.Count = 1;
    { CScriptThing n = m.GetNearestCrateToMe(); ok = ok && n.Info == 0 && info[3].RefCount == 1; }
    q.Count = 0;
    { CScriptThing n = m.GetNearestCrateToMe(); ok = ok && n.Info == 0; }
    if (!ok) { std::printf("00dcedf0_TEST FAIL\n"); return 1; }
    std::printf("00dcedf0_TEST PASS\n");
    return 0;
}