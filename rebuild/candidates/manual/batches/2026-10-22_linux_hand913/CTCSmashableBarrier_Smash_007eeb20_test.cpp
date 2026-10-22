#include <cstdio>

struct C3DVector { float X, Y, Z; };
struct CSmashableDef { long SmashParticleEmitter; };
struct CThing { C3DVector pos; const C3DVector& GetPosition() const { return pos; } };
static int g_effects; static long g_lastEmitter; static float g_lastX;
static void CreateEffectThing(long emitter, const C3DVector* pos, bool) { ++g_effects; g_lastEmitter = emitter; g_lastX = pos->X; }
struct CTCSmashableBase {
    CSmashableDef* def; CThing* ParentThing; int baseSmashes;
    CSmashableDef* GetDef() { return def; }
    CThing* GetThing() const { return ParentThing; }
    void BaseSmash() { ++baseSmashes; }
    void Smash()
    {
        BaseSmash();
        if (GetDef()->SmashParticleEmitter)
            CreateEffectThing(GetDef()->SmashParticleEmitter, &GetThing()->GetPosition(), false);
    }
};
static bool Run()
{
    CSmashableDef d; d.SmashParticleEmitter = 0; CThing t; t.pos.X = 4.0f; t.pos.Y = 0; t.pos.Z = 0;
    CTCSmashableBase s; s.def = &d; s.ParentThing = &t; s.baseSmashes = 0;
    s.Smash();
    bool ok = s.baseSmashes == 1 && g_effects == 0;
    d.SmashParticleEmitter = 12; s.Smash();
    return ok && s.baseSmashes == 2 && g_effects == 1 && g_lastEmitter == 12 && g_lastX == 4.0f;
}

int main()
{
    if (!Run()) { std::printf("007eeb20_TEST FAIL\n"); return 1; }
    std::printf("007eeb20_TEST PASS\n");
    return 0;
}
