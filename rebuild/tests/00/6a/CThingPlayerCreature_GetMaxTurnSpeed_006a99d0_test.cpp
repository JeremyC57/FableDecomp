#include <cstdio>
struct CThing;
static bool g_heavy = false;
bool IsCarryingTooHeavyWeapon(CThing* thing) { (void)thing; return g_heavy; }
struct CPlayerMovementDef { float MaxTurnSpeed; float MaxTurnSpeedHeavyWeapon; };
template <class T> struct CDefPointer {
    T* m_pDef;
    T* operator->() const { return m_pDef; }
};
struct CThingPlayerCreature {
    CDefPointer<CPlayerMovementDef>  PPlayerMovementDef;
    float GetMaxTurnSpeed();
};
float CThingPlayerCreature::GetMaxTurnSpeed()
{
    if (IsCarryingTooHeavyWeapon((CThing*)this))
        return PPlayerMovementDef->MaxTurnSpeedHeavyWeapon;
    return PPlayerMovementDef->MaxTurnSpeed;
}
int main()
{
    int bad = 0;
    CPlayerMovementDef d; d.MaxTurnSpeed = 3.0f; d.MaxTurnSpeedHeavyWeapon = 1.5f;
    CThingPlayerCreature c; c.PPlayerMovementDef.m_pDef = &d;
    if (c.GetMaxTurnSpeed() != 3.0f) bad = 1;
    g_heavy = true;
    if (c.GetMaxTurnSpeed() != 1.5f) bad = 2;
    if (bad) { std::printf("006a99d0_TEST FAIL\n"); return 1; }
    std::printf("006a99d0_TEST PASS\n");
    return 0;
}