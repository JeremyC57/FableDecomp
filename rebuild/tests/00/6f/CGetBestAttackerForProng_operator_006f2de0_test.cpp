// Behaviour test for 0x006F2DE0 `anonymous namespace'::CGetBestAttackerForProng::operator(): a copy of the row's statements (and of its
// same-TU GetPreferredCombatDistanceForAttacker / GetAttackerProngSelectionCompareValue / IsOuterRing / GetRing / GetRingRadius text)
// over stand-ins with the retail layouts. The row object cannot be linked by a test outside one fixed path: it calls the
// anonymous-namespace GetAttackerProngSelectionCurrentWeaponBaseCompareValue 0x006E9F60, whose ?A0x<hash> name depends on the compile
// path (the 0x0055F710 precedent). The real object is exercised by test/006f2de0_realobj.cpp (mut.py's fixed path; the mutant run).
// Same fixtures and checks as that test: the prong angle, the weapon's wanted kind, the best attacker and its score against a
// reference model, with short prong directions that put the prong line at the 1e-4 threshold so the preferred distance shows.
#include <stdio.h>
#include <string.h>
#include <math.h>
static const float DELTA_NOTIONAL_ZERO = 0.0001f;
static int g_fail = 0, g_run = 0;
static void Check(bool ok, int line) { if (!ok) { g_fail = 1; printf("check failed at line %d (run %d)\n", line, g_run); } }
#define CHECK(c) Check((c) ? true : false, __LINE__)
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    const C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    const C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    const C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
};
class C2DVector { public: float X, Y; };
namespace std { template <class A, class B> struct pair { A first; B second; pair(const A& a, const B& b) : first(a), second(b) {} }; }
class CCombatTypeDef { public:
    unsigned char p00[0x48]; long DefenderWheelOuterRingStartIndex; unsigned char p4c[0x6C - 0x4C];
    float FrontFarLimit, FrontMiddleLimit; unsigned char p74[0x80 - 0x74]; float PreferredCombatDistance;
};
class CTCPhysicsBase { public: unsigned char p00[0x34]; float Radius; float GetRadius() const { return Radius; } };
class CThing { public:
    unsigned char p00[0x60]; CTCPhysicsBase* PhysicsTC; C3DVector Pos; CCombatTypeDef* Def; float Weapon;
    const CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
    const C3DVector& GetPos() const { return Pos; }
};
class CThingCreatureBase : public CThing { public: const CCombatTypeDef& GetCombatTypeDef() const { return *Def; } };
class CAttackingCombatantInfo { public: void* Attacked; long AttackerNumber, CombatWheelRingIndex; bool PreferredAttackDollySet;
    long PreferredAttackDollyTimer; C2DVector PreferredAttackDollyVector; };
typedef char InfoCheck[(size_t)&((CAttackingCombatantInfo*)0)->PreferredAttackDollyVector == 0x14 ? 1 : -1];
static float g_lastAngle; static int g_lastWant, g_weaponCalls, g_prongCalls;
class CDefendingCombatantInfo { public:
    class CProng { public: C3DVector Dir;
        C3DVector GetProngVector(float angle) const { g_lastAngle = angle; ++g_prongCalls; return Dir; } };
    class CCombatRing { public:
        bool IsOuterRing(const CCombatTypeDef& defender_combat_type_def) const;
        long GetIndex() const { return Index; }
        float GetRotationAngle() const { return RotationAngle; }
        float RotationAngle; float DeltaRotation; long Index; float RingThickness; void* Prongs[3]; };
    class CCombatWheel { public:
        struct Rings { CCombatRing* s; CCombatRing* f; CCombatRing* e;
            unsigned long size() const { return f - s; } const CCombatRing& operator[](unsigned long i) const { return s[i]; } } CombatRings;
        const CCombatRing* GetRing(long index) const;
        float GetRingRadius(long ring) const; };
};
class CWorldMap;
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
float __fastcall GetAttackerProngSelectionCompareValue(const C3DVector& attackerPos, const C3DVector& defenderPos,
                                                       const C3DVector& prongVector, const C3DVector& placementVector)
{
    C3DVector flatPlacement(placementVector.X, placementVector.Y, 0.0f);
    C3DVector flatProng(prongVector.X, prongVector.Y, 0.0f);
    float lengthSquared = flatPlacement.X * flatPlacement.X + flatPlacement.Y * flatPlacement.Y + flatPlacement.Z * flatPlacement.Z;
    if (lengthSquared < 0.0001f)
        return 0.0001f;
    float inverse = 1.0f / (float)sqrt(lengthSquared);
    flatPlacement.X = inverse * flatPlacement.X;
    flatPlacement.Y = inverse * flatPlacement.Y;
    flatPlacement.Z = inverse * flatPlacement.Z;
    lengthSquared = flatProng.X * flatProng.X + flatProng.Y * flatProng.Y + flatProng.Z * flatProng.Z;
    if (lengthSquared < 0.0001f)
        return 0.0001f;
    inverse = 1.0f / (float)sqrt(lengthSquared);
    flatProng.X = inverse * flatProng.X;
    flatProng.Y = inverse * flatProng.Y;
    flatProng.Z = inverse * flatProng.Z;
    return flatProng.Z * flatPlacement.Z + (flatProng.X * flatPlacement.X + flatProng.Y * flatPlacement.Y) + 1.0f;
}
float __fastcall GetAttackerProngSelectionCurrentWeaponBaseCompareValue(const CThing& attacker, bool want)
{ g_lastWant = want; ++g_weaponCalls; return want ? attacker.Weapon : -attacker.Weapon; }
}
inline bool CDefendingCombatantInfo::CCombatRing::IsOuterRing(const CCombatTypeDef& defender_combat_type_def) const
{
    return Index >= defender_combat_type_def.DefenderWheelOuterRingStartIndex;
}
inline const CDefendingCombatantInfo::CCombatRing* CDefendingCombatantInfo::CCombatWheel::GetRing(long index) const
{
    if (index >= 0 && (unsigned long)index < CombatRings.size())
        return &CombatRings[index];
    return 0;
}
float CDefendingCombatantInfo::CCombatWheel::GetRingRadius(long ring) const
{
    float radius = 0.0f;
    for (long i = ring; i >= 0; --i)
        radius += GetRing(i)->RingThickness;
    return radius;
}
namespace {
class CGetBestAttackerForProng { public:
    CGetBestAttackerForProng(const CWorldMap& world_map, const CDefendingCombatantInfo::CCombatWheel& wheel,
                             const CDefendingCombatantInfo::CCombatRing& ring, const CDefendingCombatantInfo::CProng& prong,
                             const CThingCreatureBase& defender)
        : Ring(ring), Wheel(wheel), Prong(prong), Defender(defender), WorldMap(world_map), BestCompareVal(-1.0f), PBestAttacker(0) {}
    void operator()(std::pair<CThingCreatureBase*, CAttackingCombatantInfo*> attacker);
    const CDefendingCombatantInfo::CCombatRing& Ring; const CDefendingCombatantInfo::CCombatWheel& Wheel;
    const CDefendingCombatantInfo::CProng& Prong; const CThingCreatureBase& Defender; const CWorldMap& WorldMap;
    float BestCompareVal; CThingCreatureBase* PBestAttacker;
};
void CGetBestAttackerForProng::operator()(std::pair<CThingCreatureBase*, CAttackingCombatantInfo*> attacker)
{
    bool outer_ring = false;
    float preferred_distance = GetPreferredCombatDistanceForAttacker(Defender, *attacker.first);
    if (Ring.IsOuterRing(Defender.GetCombatTypeDef())) {
        outer_ring = true;
        preferred_distance = Wheel.GetRingRadius(Ring.GetIndex());
    }
    C3DVector prong_line = Prong.GetProngVector(Ring.GetRotationAngle()) * preferred_distance;
    C3DVector prong_pos = Defender.GetPos() + prong_line;
    {
        C3DVector attacker_pos = attacker.first->GetPos();
        C3DVector my_line = attacker_pos - Defender.GetPos();
        my_line.Z = 0.0f;
        if (attacker.second->PreferredAttackDollySet)
            my_line = C3DVector(attacker.second->PreferredAttackDollyVector.X, attacker.second->PreferredAttackDollyVector.Y, 0.0f);
        float compare_val = GetAttackerProngSelectionCompareValue(Defender.GetPos(), attacker.first->GetPos(), prong_line, my_line);
        compare_val += GetAttackerProngSelectionCurrentWeaponBaseCompareValue(*attacker.first, outer_ring);
        if (compare_val > BestCompareVal) {
            PBestAttacker = attacker.first;
            BestCompareVal = compare_val;
        }
    }
}
}
typedef CThingCreatureBase Thing; typedef CAttackingCombatantInfo Info; typedef CDefendingCombatantInfo::CCombatRing Ring;
static float Model(const CGetBestAttackerForProng& f, const Thing& a, const Info& info, bool& outer)
{
    float sep = f.Defender.PhysicsTC->Radius;
    float dist = a.Def->PreferredCombatDistance;
    if (dist < 0.0001f) dist = (a.Def->FrontMiddleLimit + a.Def->FrontFarLimit) / 2.0f;
    float pref = sep + dist;
    outer = f.Ring.Index >= f.Defender.Def->DefenderWheelOuterRingStartIndex;
    if (outer) { pref = 0; for (long i = f.Ring.Index; i >= 0; --i) pref += f.Wheel.CombatRings.s[i].RingThickness; }
    float plx = f.Prong.Dir.X * pref, ply = f.Prong.Dir.Y * pref;
    float mlx = a.Pos.X - f.Defender.Pos.X, mly = a.Pos.Y - f.Defender.Pos.Y;
    if (info.PreferredAttackDollySet) { mlx = info.PreferredAttackDollyVector.X; mly = info.PreferredAttackDollyVector.Y; }
    float l1 = mlx * mlx + mly * mly, l2 = plx * plx + ply * ply, cv;
    if (l1 < 0.0001f || l2 < 0.0001f) cv = 0.0001f;
    else cv = (plx * mlx + ply * mly) / (float)sqrt(l1) / (float)sqrt(l2) + 1.0f;
    return cv + (outer ? a.Weapon : -a.Weapon);
}
static void Run(CGetBestAttackerForProng& f, Thing& a, Info& info)
{
    ++g_run; g_weaponCalls = 0; g_prongCalls = 0;
    bool outer; float expect = Model(f, a, info, outer);
    float oldBest = f.BestCompareVal; Thing* oldA = f.PBestAttacker;
    f(std::pair<CThingCreatureBase*, CAttackingCombatantInfo*>(&a, &info));
    CHECK(g_weaponCalls == 1); CHECK(g_prongCalls == 1);
    CHECK(g_lastWant == (outer ? 1 : 0)); CHECK(g_lastAngle == f.Ring.RotationAngle);
    if (expect > oldBest) { CHECK(f.PBestAttacker == &a); CHECK(fabs(f.BestCompareVal - expect) < 1e-4); }
    else { CHECK(f.PBestAttacker == oldA); CHECK(f.BestCompareVal == oldBest); }
}
#define F(name, r) CGetBestAttackerForProng name(*(CWorldMap*)0, wheel, rings[r], prong, defender)
int main()
{
    CCombatTypeDef defD, defA, defB; memset(&defD, 0, sizeof defD); memset(&defA, 0, sizeof defA); memset(&defB, 0, sizeof defB);
    defD.DefenderWheelOuterRingStartIndex = 2; defD.FrontFarLimit = 6; defD.FrontMiddleLimit = 4;
    defA.DefenderWheelOuterRingStartIndex = 9; defA.FrontFarLimit = 8; defA.FrontMiddleLimit = 2; defA.PreferredCombatDistance = 3;
    defB = defA; defB.PreferredCombatDistance = 0.00001f;
    CTCPhysicsBase phD; memset(&phD, 0, sizeof phD); phD.Radius = 1.5f;
    Ring rings[3]; memset(rings, 0, sizeof rings);
    for (int i = 0; i < 3; ++i) { rings[i].RotationAngle = 0.25f * (i + 1); rings[i].Index = i; rings[i].RingThickness = 2.0f + i; }
    CDefendingCombatantInfo::CCombatWheel wheel; wheel.CombatRings.s = rings; wheel.CombatRings.f = rings + 3; wheel.CombatRings.e = rings + 3;
    CDefendingCombatantInfo::CProng prong; prong.Dir = C3DVector(1, 0, 0);
    Thing defender; memset(&defender, 0, sizeof defender); defender.PhysicsTC = &phD; defender.Def = &defD; defender.Pos = C3DVector(10, 20, 5);
    Thing a1; memset(&a1, 0, sizeof a1); a1.Def = &defA; a1.Pos = C3DVector(14, 20, 9); a1.Weapon = 0.5f;
    Thing a2; memset(&a2, 0, sizeof a2); a2.Def = &defB; a2.Pos = C3DVector(10, 24, 0); a2.Weapon = 2.0f;
    Info i1; memset(&i1, 0, sizeof i1);
    Info i2; memset(&i2, 0, sizeof i2); i2.PreferredAttackDollySet = true; i2.PreferredAttackDollyVector.X = -3; i2.PreferredAttackDollyVector.Y = 0;
    F(f, 0); Run(f, a1, i1); CHECK(f.PBestAttacker == &a1);
    Run(f, a2, i1); CHECK(f.PBestAttacker == &a1);
    Run(f, a1, i2);
    F(g, 2); Run(g, a2, i1); CHECK(g.PBestAttacker == &a2);
    prong.Dir = C3DVector(0, 1, 0);
    Run(g, a1, i1); CHECK(g.PBestAttacker == &a2);
    Thing a3 = a1; a3.Pos = C3DVector(10, 20, 0);
    F(h, 1); Run(h, a3, i1); CHECK(h.PBestAttacker == &a3);
    Thing a5 = a1; a5.Def = &defB; a5.Weapon = 0.1f;
    float mags[5] = { 0.0025f, 0.0015f, 0.00125f, 0.004f, 0.002f };
    for (int m = 0; m < 5; ++m) {
        prong.Dir = C3DVector(mags[m], 0, 0);
        for (int r = 0; r < 3; ++r) { F(k, r); Run(k, a1, i1); F(k2, r); Run(k2, a5, i1); }
    }
    prong.Dir = C3DVector(1, 0, 0);
    Info i3; memset(&i3, 0, sizeof i3); i3.PreferredAttackDollySet = true; i3.PreferredAttackDollyVector.X = 2; i3.PreferredAttackDollyVector.Y = 1;
    F(d1, 0); Run(d1, a2, i3); CHECK(d1.PBestAttacker == &a2);
    Thing a4 = a1; a4.Weapon = 0.4f;
    F(c1, 0); Run(c1, a1, i1); Run(c1, a4, i1); CHECK(c1.PBestAttacker == &a4);
    if (!g_fail) printf("006f2de0_TEST PASS\n");
    return g_fail;
}