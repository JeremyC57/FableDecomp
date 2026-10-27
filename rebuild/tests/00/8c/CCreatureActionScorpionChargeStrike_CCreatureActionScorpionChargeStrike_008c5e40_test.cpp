// behaviour test for 0x008C5E40 CCreatureAction_ScorpionChargeStrike::CCreatureAction_ScorpionChargeStrike(const CCreatureAction_ScorpionChargeStrike&) (model; the base copy constructor is a stub)
#include <cstdio>
#include <cstring>
static int g_copies = 0;
class CCreatureActionBase { public: virtual ~CCreatureActionBase() { } };
class CCombatActionBase : public CCreatureActionBase {
public:
    CCombatActionBase() { }
    CCombatActionBase(const CCombatActionBase&) { ++g_copies; }
};
class C3DVector { public: float X, Y, Z; C3DVector() { } C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { } };
class CCreatureAction_ScorpionChargeStrike : public CCombatActionBase {
public:
    CCreatureAction_ScorpionChargeStrike() { }
    CCreatureAction_ScorpionChargeStrike(const CCreatureAction_ScorpionChargeStrike& other);
    float ChargeDistance;                     // +0x114 (PDB +300)
    C3DVector StartPos;                       // +0x118
};
CCreatureAction_ScorpionChargeStrike::CCreatureAction_ScorpionChargeStrike(const CCreatureAction_ScorpionChargeStrike& other) : CCombatActionBase(other)
    , ChargeDistance(other.ChargeDistance)
    , StartPos(other.StartPos) { }
int main()
{
    CCreatureAction_ScorpionChargeStrike a;
    a.ChargeDistance = 2.25f;
    a.StartPos.X = 1.5f; a.StartPos.Y = 2.5f; a.StartPos.Z = 4.0f;
    CCreatureAction_ScorpionChargeStrike b(a);
    int ok = g_copies == 1 && b.ChargeDistance == 2.25f && b.StartPos.X == 1.5f && b.StartPos.Z == 4.0f;
    if (!ok) { std::printf("008c5e40_TEST FAIL\n"); return 1; }
    std::printf("008c5e40_TEST PASS\n");
    return 0;
}