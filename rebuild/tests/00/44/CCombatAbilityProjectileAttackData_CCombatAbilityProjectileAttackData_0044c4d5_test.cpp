// behaviour test for 0x0044C4D5 CCombatAbilityProjectileAttackData::CCombatAbilityProjectileAttackData (model: the member defaults)
#include <cstdio>
struct Obj { bool AbleToStrafeAndShoot; int ShotAccuracyPercentage; Obj() : AbleToStrafeAndShoot(false), ShotAccuracyPercentage(30) { } };
int main()
{
    Obj o;
    int ok = o.AbleToStrafeAndShoot == false && o.ShotAccuracyPercentage == 30;
    if (!ok) { std::printf("0044c4d5_TEST FAIL\n"); return 1; }
    std::printf("0044c4d5_TEST PASS\n");
    return 0;
}