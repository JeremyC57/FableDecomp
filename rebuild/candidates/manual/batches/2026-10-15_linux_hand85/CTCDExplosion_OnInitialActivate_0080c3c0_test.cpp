// Test for 0080c3c0 CTCDExplosion::OnInitialActivate: sub-def fetched, radius/damage/fire/lifetime copied, rings reset.
#include <cstdio>
struct CExplosionDef { float Damage; float ExplosionRadius; long FireDamage; float ExplosionLifeTime; };
struct CThingDef {
    CExplosionDef* Sub;
    bool GetSubDef(CExplosionDef** out) const { *out = Sub; return true; }
};
struct CThing { CThingDef* PDef; };
long g_GameFramesPerSecond = 15;
struct CTCDExplosion {
    CThing* ParentThing;
    float DamageRadius, PhysicsRadius, MaxDamage; long FireDamage; long ExplosionLifeTimer;
    CExplosionDef* PExplosionDef;
    long ExplosionEvolutionTimer, CurrentExplosionRing;
    void OnInitialActivate()
    {
        ParentThing->PDef->GetSubDef(&PExplosionDef);
        DamageRadius = PExplosionDef->ExplosionRadius;
        PhysicsRadius = PExplosionDef->ExplosionRadius;
        MaxDamage = PExplosionDef->Damage;
        FireDamage = PExplosionDef->FireDamage;
        ExplosionLifeTimer = (long)(PExplosionDef->ExplosionLifeTime * g_GameFramesPerSecond);
        ExplosionEvolutionTimer = 0;
        CurrentExplosionRing = -1;
    }
};
int main()
{
    CExplosionDef d = { 80.0f, 6.5f, 12, 2.5f };
    CThingDef td = { &d };
    CThing t = { &td };
    CTCDExplosion e; e.ParentThing = &t; e.PExplosionDef = 0; e.ExplosionEvolutionTimer = 7; e.CurrentExplosionRing = 3;
    e.OnInitialActivate();
    if (e.PExplosionDef != &d || e.DamageRadius != 6.5f || e.PhysicsRadius != 6.5f || e.MaxDamage != 80.0f
        || e.FireDamage != 12 || e.ExplosionLifeTimer != 37 || e.ExplosionEvolutionTimer != 0
        || e.CurrentExplosionRing != -1) { std::printf("0080c3c0_TEST FAIL\n"); return 1; }
    std::printf("0080c3c0_TEST PASS\n");
    return 0;
}
