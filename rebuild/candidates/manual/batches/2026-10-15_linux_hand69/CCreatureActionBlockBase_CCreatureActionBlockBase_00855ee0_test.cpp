// Behaviour test for CCreatureAction_BlockBase::CCreatureAction_BlockBase(const CCreatureAction_BlockBase&) @ 0x00855EE0
#include <cstdio>

static int g_baseCopies = 0;

struct C3DVector {
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};

struct CCombatActionBase {
    virtual ~CCombatActionBase() { }
    long m_state;
    CCombatActionBase() : m_state(0) { }
    CCombatActionBase(const CCombatActionBase& other) : m_state(other.m_state) { ++g_baseCopies; }
};

struct CCreatureAction_BlockBase : CCombatActionBase {
    long      BlowStrength;
    C3DVector MovementDirection;
    CCreatureAction_BlockBase() : BlowStrength(0) { }
    CCreatureAction_BlockBase(const CCreatureAction_BlockBase& other)
        : CCombatActionBase(other), BlowStrength(other.BlowStrength), MovementDirection(other.MovementDirection) { }
};

int main()
{
    CCreatureAction_BlockBase a;
    a.m_state = 3; a.BlowStrength = 250;
    a.MovementDirection.X = 0.0f; a.MovementDirection.Y = 1.0f; a.MovementDirection.Z = -0.5f;
    CCreatureAction_BlockBase b(a);
    if (g_baseCopies != 1 || b.m_state != 3 || b.BlowStrength != 250) { std::printf("00855ee0_TEST FAIL\n"); return 1; }
    if (b.MovementDirection.X != 0.0f || b.MovementDirection.Y != 1.0f || b.MovementDirection.Z != -0.5f) { std::printf("00855ee0_TEST FAIL\n"); return 1; }
    std::printf("00855ee0_TEST PASS\n");
    return 0;
}
