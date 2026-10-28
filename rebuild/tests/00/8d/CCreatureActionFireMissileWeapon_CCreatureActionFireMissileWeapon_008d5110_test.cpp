// Behaviour test: CCreatureAction_FireMissileWeapon(creature, target) (0x008D5110) -- accuracy from the creature's
// GetShotAccuracyPercentage (long, converted to float), damage and build-up 1.0, no decapitation, delay 0, no def.
#include <cstdio>
struct CCreature { int Accuracy; int GetShotAccuracyPercentage() const { return Accuracy; } };
struct CFire {
    const CCreature* PCreature; const void* PTarget;
    float ShotAccuracyPercentage, ShotDamageMultiplier, ShotBuildupTime;
    bool Decapitate; int DelayTimer; const void* PSpecialShotDef;
    CFire(const CCreature& creature, const void* target)
        : PCreature(&creature), PTarget(target), DelayTimer(0), PSpecialShotDef(0)
    {
        ShotAccuracyPercentage = (float)PCreature->GetShotAccuracyPercentage();
        ShotDamageMultiplier = 1.0f;
        ShotBuildupTime = 1.0f;
        Decapitate = false;
    }
};
int main()
{
    CCreature c = { 85 };
    int target = 0;
    CFire f(c, &target);
    if (!(f.ShotAccuracyPercentage == 85.0f && f.ShotDamageMultiplier == 1.0f && f.ShotBuildupTime == 1.0f
          && !f.Decapitate && f.DelayTimer == 0 && f.PSpecialShotDef == 0 && f.PTarget == &target)) {
        std::printf("008d5110_TEST FAIL\n"); return 1;
    }
    std::printf("008d5110_TEST PASS\n");
    return 0;
}