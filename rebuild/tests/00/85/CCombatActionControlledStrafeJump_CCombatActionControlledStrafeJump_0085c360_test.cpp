// Behaviour test for 0x0085C360 CCombatAction_ControlledStrafeJump copy constructor: base copy + six facing floats + flag.
#include <cstdio>
static int g_baseCopies = 0;
struct CCombatActionBase {
    virtual ~CCombatActionBase() { }
    long m_baseValue;
    CCombatActionBase() : m_baseValue(0) { }
    CCombatActionBase(const CCombatActionBase& other) : m_baseValue(other.m_baseValue) { ++g_baseCopies; }
};
struct CCombatAction_ControlledStrafeJump : CCombatActionBase {
    float RequiredFacing_X, RequiredFacing_Y, RequiredFacing_Z;
    float OriginalFacing_X, OriginalFacing_Y, OriginalFacing_Z;
    bool AddedRollMode;
    CCombatAction_ControlledStrafeJump() { }
    CCombatAction_ControlledStrafeJump(const CCombatAction_ControlledStrafeJump& other)
        : CCombatActionBase(other),
          RequiredFacing_X(other.RequiredFacing_X), RequiredFacing_Y(other.RequiredFacing_Y),
          RequiredFacing_Z(other.RequiredFacing_Z), OriginalFacing_X(other.OriginalFacing_X),
          OriginalFacing_Y(other.OriginalFacing_Y), OriginalFacing_Z(other.OriginalFacing_Z),
          AddedRollMode(other.AddedRollMode) { }
};
int main()
{
    CCombatAction_ControlledStrafeJump a;
    a.m_baseValue = 7; a.RequiredFacing_X = 1.0f; a.RequiredFacing_Y = 2.0f; a.RequiredFacing_Z = 3.0f;
    a.OriginalFacing_X = -1.0f; a.OriginalFacing_Y = -2.0f; a.OriginalFacing_Z = -3.0f; a.AddedRollMode = true;
    CCombatAction_ControlledStrafeJump b(a);
    if (g_baseCopies != 1 || b.m_baseValue != 7 || b.RequiredFacing_X != 1.0f || b.RequiredFacing_Y != 2.0f ||
        b.RequiredFacing_Z != 3.0f || b.OriginalFacing_X != -1.0f || b.OriginalFacing_Y != -2.0f ||
        b.OriginalFacing_Z != -3.0f || !b.AddedRollMode) { std::printf("0085c360_TEST FAIL\n"); return 1; }
    std::printf("0085c360_TEST PASS\n");
    return 0;
}