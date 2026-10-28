// Behaviour test: CCreatureAction_FireMissileWeapon(creature, target, damage, accuracy, buildup, decapitate, special
// shot def, c, d) (0x008D5020) -- the base action plays "PROJECTILE_WEAPON_FIRE"; the shot fields take the arguments
// (the second float is the accuracy, the first the damage multiplier); the def is shared (count bumped); delay 0.
#include <cstdio>
#include <cstring>
struct CDef { int RefCount; };
struct CDefPointer {
    CDef* P;
    explicit CDefPointer(CDef* p) : P(p) { if (P) ++P->RefCount; }
    CDefPointer(const CDefPointer& o) : P(o.P) { if (P) ++P->RefCount; }
    ~CDefPointer() { if (P) --P->RefCount; }
};
struct CFire {
    const char* Anim; int C, D;
    const void* PTarget;
    float ShotAccuracyPercentage, ShotDamageMultiplier, ShotBuildupTime;
    bool Decapitate; int DelayTimer; CDefPointer PSpecialShotDef;
    CFire(const void* target, float damage, float accuracy, float buildup, bool decap, CDefPointer def, int c, int d)
        : Anim("PROJECTILE_WEAPON_FIRE"), C(c), D(d), PTarget(target), ShotAccuracyPercentage(accuracy),
          ShotDamageMultiplier(damage), ShotBuildupTime(buildup), Decapitate(decap), DelayTimer(0), PSpecialShotDef(def) {}
};
int main()
{
    CDef def = { 1 };
    int target = 0;
    bool ok;
    {
        CDefPointer p(&def);                                 // count 2
        CFire f(&target, 2.0f, 75.0f, 0.5f, true, p, 3, 4);
        ok = f.ShotAccuracyPercentage == 75.0f && f.ShotDamageMultiplier == 2.0f && f.ShotBuildupTime == 0.5f
            && f.Decapitate && f.DelayTimer == 0 && f.PTarget == &target && f.PSpecialShotDef.P == &def
            && def.RefCount == 3 && f.C == 3 && f.D == 4 && std::strcmp(f.Anim, "PROJECTILE_WEAPON_FIRE") == 0;
    }
    ok = ok && def.RefCount == 1;
    if (!ok) { std::printf("008d5020_TEST FAIL\n"); return 1; }
    std::printf("008d5020_TEST PASS\n");
    return 0;
}