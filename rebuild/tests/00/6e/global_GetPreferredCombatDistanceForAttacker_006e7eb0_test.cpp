// Behaviour test for 0x006E7EB0 `anonymous namespace'::GetPreferredCombatDistanceForAttacker: a copy of the row's statements over
// stand-ins (the row is anonymous-namespace, so its ?A0x<hash> name depends on the compile path and vland's path is not fixed; the
// 0x006E7F00 / 0x006F2DE0 precedent). The same fixtures and checks as the real-object mutant oracle test/006e7eb0_realobj.cpp: the
// combat def is read from the attacker (once), the radius from the defender, and the result matches a reference model over preferred
// distances above, at and below DELTA_NOTIONAL_ZERO.
#include <stdio.h>
#include <math.h>
#ifndef _MSC_VER
#define __fastcall
#endif
static const float DELTA_NOTIONAL_ZERO = 0.0001f;
static int g_fail = 0, g_run = 0;
static void Check(bool ok, int line) { if (!ok) { g_fail = 1; printf("check failed at line %d (run %d)\n", line, g_run); } }
#define CHECK(c) Check((c) ? true : false, __LINE__)

class CCombatTypeDef { public: float FrontFarLimit, FrontMiddleLimit, PreferredCombatDistance; };
class CTCPhysicsBase { public: float Radius; float GetRadius() const { return Radius; } };
class CThingCreatureBase;
static const CThingCreatureBase* g_lastCTD; static int g_ctdCalls;
class CThingCreatureBase {
public:
    CTCPhysicsBase* PhysicsTC; CCombatTypeDef* Def;
    CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
    const CCombatTypeDef& GetCombatTypeDef() const { g_lastCTD = this; ++g_ctdCalls; return *Def; }
};
namespace {
float __fastcall GetPreferredCombatDistanceForAttacker(const CThingCreatureBase& defender, const CThingCreatureBase& attacker)
{
    float separation = defender.GetPhysicsTC()->GetRadius();
    const CCombatTypeDef& combat_def = attacker.GetCombatTypeDef();
    float distance = combat_def.PreferredCombatDistance;
    if (distance < DELTA_NOTIONAL_ZERO) {
        float far_range = combat_def.FrontFarLimit;
        float middle_range = combat_def.FrontMiddleLimit;
        distance = (middle_range + far_range) / 2.0f;
    }
    separation += distance;
    return separation;
}
}

static double Model(const CThingCreatureBase& d, const CThingCreatureBase& a)
{
    double dist = a.Def->PreferredCombatDistance;
    if (a.Def->PreferredCombatDistance < 0.0001f) dist = ((double)a.Def->FrontMiddleLimit + a.Def->FrontFarLimit) / 2.0;
    return d.PhysicsTC->Radius + dist;
}

static void Run(float dRadius, float aRadius, float pref, float farL, float midL)
{
    ++g_run; g_ctdCalls = 0; g_lastCTD = 0;
    CTCPhysicsBase dp, ap; CCombatTypeDef dd, ad; CThingCreatureBase d, a;
    dp.Radius = dRadius; ap.Radius = aRadius;
    dd.PreferredCombatDistance = 1000.0f; dd.FrontFarLimit = 2000.0f; dd.FrontMiddleLimit = 3000.0f;   // never read
    ad.PreferredCombatDistance = pref; ad.FrontFarLimit = farL; ad.FrontMiddleLimit = midL;
    d.PhysicsTC = &dp; d.Def = &dd; a.PhysicsTC = &ap; a.Def = &ad;
    double expect = Model(d, a);
    g_ctdCalls = 0; g_lastCTD = 0;
    double got = GetPreferredCombatDistanceForAttacker(d, a);
    CHECK(g_ctdCalls == 1); CHECK(g_lastCTD == &a);
    CHECK(fabs(got - expect) <= 1e-5 * (fabs(expect) + 1.0));
}

int main()
{
    Run(0.75f, 9.0f, 2.5f, 7.0f, 3.0f);         // preferred distance set
    Run(1.25f, 0.5f, 0.0001f, 7.0f, 3.0f);      // exactly the threshold: kept
    Run(1.25f, 0.5f, 0.00005f, 7.0f, 3.0f);     // below: the front limits' midpoint
    Run(2.0f, 4.0f, 0.0f, 10.5f, 1.25f);
    Run(0.0f, 4.0f, -3.0f, 6.0f, 2.0f);         // negative counts as unset
    Run(3.5f, 0.0f, 4.75f, -8.0f, 100.0f);
    Run(0.3f, 0.0f, 0.0f, 0.375f, 0.125f);
    if (g_fail) { printf("006e7eb0_TEST FAIL\n"); return 1; }
    printf("006e7eb0_TEST PASS (%d runs)\n", g_run);
    return 0;
}