// Behaviour test for 0x00880120 CCombatAction_KickThingOnGround copy constructor: base copy + seven floats.
#include <cstdio>
static int g_baseCopies = 0;
struct CCombatActionBase {
    virtual ~CCombatActionBase() { }
    long m_baseValue;
    CCombatActionBase() : m_baseValue(0) { }
    CCombatActionBase(const CCombatActionBase& other) : m_baseValue(other.m_baseValue) { ++g_baseCopies; }
};
struct CCombatAction_KickThingOnGround : CCombatActionBase {
    float MoveToPos_X, MoveToPos_Y, MoveToPos_Z, Movement_X, Movement_Y, Movement_Z, MaxRunningSpeed;
    CCombatAction_KickThingOnGround() { }
    CCombatAction_KickThingOnGround(const CCombatAction_KickThingOnGround& other)
        : CCombatActionBase(other),
          MoveToPos_X(other.MoveToPos_X), MoveToPos_Y(other.MoveToPos_Y), MoveToPos_Z(other.MoveToPos_Z),
          Movement_X(other.Movement_X), Movement_Y(other.Movement_Y), Movement_Z(other.Movement_Z),
          MaxRunningSpeed(other.MaxRunningSpeed) { }
};
int main()
{
    CCombatAction_KickThingOnGround a;
    a.m_baseValue = 3; a.MoveToPos_X = 1; a.MoveToPos_Y = 2; a.MoveToPos_Z = 3;
    a.Movement_X = 4; a.Movement_Y = 5; a.Movement_Z = 6; a.MaxRunningSpeed = 7.5f;
    CCombatAction_KickThingOnGround b(a);
    if (g_baseCopies != 1 || b.m_baseValue != 3 || b.MoveToPos_X != 1 || b.MoveToPos_Y != 2 || b.MoveToPos_Z != 3 ||
        b.Movement_X != 4 || b.Movement_Y != 5 || b.Movement_Z != 6 || b.MaxRunningSpeed != 7.5f) {
        std::printf("00880120_TEST FAIL\n"); return 1;
    }
    std::printf("00880120_TEST PASS\n");
    return 0;
}