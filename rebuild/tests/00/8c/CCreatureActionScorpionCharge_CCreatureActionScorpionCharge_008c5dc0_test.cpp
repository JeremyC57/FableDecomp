// behaviour test for 0x008C5DC0 CCreatureAction_ScorpionCharge::CCreatureAction_ScorpionCharge(const CCreatureAction_ScorpionCharge&) (model; the base copy constructor is a stub)
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
class CCreatureAction_ScorpionCharge : public CCombatActionBase {
public:
    CCreatureAction_ScorpionCharge() { }
    CCreatureAction_ScorpionCharge(const CCreatureAction_ScorpionCharge& other);
    float ChargeDistance;                     // +0x114 (PDB +300)
    C3DVector StartPos;                       // +0x118
};
CCreatureAction_ScorpionCharge::CCreatureAction_ScorpionCharge(const CCreatureAction_ScorpionCharge& other) : CCombatActionBase(other)
    , ChargeDistance(other.ChargeDistance)
    , StartPos(other.StartPos) { }
int main()
{
    CCreatureAction_ScorpionCharge a;
    a.ChargeDistance = 2.25f;
    a.StartPos.X = 1.5f; a.StartPos.Y = 2.5f; a.StartPos.Z = 4.0f;
    CCreatureAction_ScorpionCharge b(a);
    int ok = g_copies == 1 && b.ChargeDistance == 2.25f && b.StartPos.X == 1.5f && b.StartPos.Z == 4.0f;
    if (!ok) { std::printf("008c5dc0_TEST FAIL\n"); return 1; }
    std::printf("008c5dc0_TEST PASS\n");
    return 0;
}