// behaviour test for 0x008BEAB0 CCreatureAction_TeleportOut::CCreatureAction_TeleportOut(const CCreatureAction_TeleportOut&) (model; the base copy constructor is a stub)
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
class CCreatureAction_TeleportOut : public CCombatActionBase {
public:
    CCreatureAction_TeleportOut() { }
    CCreatureAction_TeleportOut(const CCreatureAction_TeleportOut& other);
    C3DVector TeleportPos;                    // +0x114 (PDB +300)
    bool CreateEffects;                       // +0x120
};
CCreatureAction_TeleportOut::CCreatureAction_TeleportOut(const CCreatureAction_TeleportOut& other) : CCombatActionBase(other)
    , TeleportPos(other.TeleportPos)
    , CreateEffects(other.CreateEffects) { }
int main()
{
    CCreatureAction_TeleportOut a;
    a.TeleportPos.X = 1.5f; a.TeleportPos.Y = 2.5f; a.TeleportPos.Z = 3.0f;
    a.CreateEffects = true;
    CCreatureAction_TeleportOut b(a);
    int ok = g_copies == 1 && b.TeleportPos.X == 1.5f && b.TeleportPos.Z == 3.0f && b.CreateEffects;
    if (!ok) { std::printf("008beab0_TEST FAIL\n"); return 1; }
    std::printf("008beab0_TEST PASS\n");
    return 0;
}