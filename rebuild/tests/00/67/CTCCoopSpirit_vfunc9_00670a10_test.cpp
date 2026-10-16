// behaviour test for 0x00670A10 CTCCoopSpirit::vfunc_9 (frame update)
#include <cstdio>
struct CThing { unsigned char flags91; float life; int kills; void Kill(bool) { ++kills; } };
struct Ptr { CThing* p; CThing* GetPItem() const { return p; } };
static int g_log[16]; static int g_n = 0;
struct Spirit {
    CThing* ParentThing; Ptr PParticleEmitter; long particleName;
    void SetAppearanceParticle(long) { g_log[g_n++] = 1; }
    void UpdateMovement() { g_log[g_n++] = 2; }
    void UpdateLocalJostling() { g_log[g_n++] = 3; }
    void UpdateGravity() { g_log[g_n++] = 4; }
    void UpdateScore() { g_log[g_n++] = 5; }
    void vfunc_9()
    {
        if (!(PParticleEmitter.GetPItem() && !(PParticleEmitter.GetPItem()->flags91 & 1))) SetAppearanceParticle(particleName);
        UpdateMovement(); UpdateLocalJostling(); UpdateGravity();
        if (ParentThing && ParentThing->life <= 0.0f) ParentThing->Kill(true);
        UpdateScore();
    }
};
int main()
{
    CThing me = { 0, 1.0f, 0 }, em = { 0, 1.0f, 0 };
    Spirit s; s.ParentThing = &me; s.PParticleEmitter.p = &em; s.particleName = 7;
    s.vfunc_9();
    bool ok = g_n == 4 && g_log[0] == 2 && g_log[3] == 5 && me.kills == 0;
    g_n = 0; em.flags91 = 1; me.life = 0.0f;
    s.vfunc_9();
    ok = ok && g_n == 5 && g_log[0] == 1 && me.kills == 1;
    g_n = 0; s.PParticleEmitter.p = 0; s.ParentThing = 0;
    s.vfunc_9();
    ok = ok && g_n == 5 && g_log[0] == 1;
    if (!ok) { std::printf("00670a10_TEST FAIL\n"); return 1; }
    std::printf("00670a10_TEST PASS\n");
    return 0;
}