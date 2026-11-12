// Behaviour test for 0x007B0360 CTCFishingSpot::FrameUpdate: while enabled, each missing particle (two ripples at attach type 0x40,
// offset 0, the underwater fish at 0x80, offset -1) is created at the spot's position and, when the created thing lives, its particle
// emitter is attached to the spot and set to always update; an existing particle is left alone; disabled -> RemoveParticles.
#include <cstdio>
#include <cstring>
struct Emitter { int Attached; long AttachType; float Offset; bool AlwaysUpdate; };
struct Thing { bool HasEmitter; Emitter E; };
static Thing g_Things[8];
static int g_Created = 0;
static bool g_FailCreate = false;
static const char* g_Names[8];
static Thing* Create(const char* name)
{
    if (g_FailCreate) return 0;
    g_Names[g_Created] = name;
    Thing* t = &g_Things[g_Created++];
    t->HasEmitter = true; t->E.Attached = 0; t->E.AlwaysUpdate = false;
    return t;
}
struct FishingSpot {
    Thing* PParticle1; Thing* PParticle2; Thing* PParticleFish; bool Enabled; int Removed;
    void CreateParticle(Thing*& particle, const char* name, long attachType, float offset)
    {
        if (!particle) {
            particle = Create(name);
            if (particle) {
                Emitter* emitter = 0;
                if (particle->HasEmitter) emitter = &particle->E;
                emitter->Attached++; emitter->AttachType = attachType; emitter->Offset = offset;
                emitter->AlwaysUpdate = true;
            }
        }
    }
    void FrameUpdate()
    {
        if (Enabled) {
            CreateParticle(PParticle1, "FISHING_SPOT_01", 0x40, 0.0f);
            CreateParticle(PParticle2, "FISHING_SPOT_02", 0x40, 0.0f);
            CreateParticle(PParticleFish, "UNDERWATER_FISH_01", 0x80, -1.0f);
        } else
            ++Removed;
    }
};
int main()
{
    FishingSpot s = { 0, 0, 0, true, 0 };
    s.FrameUpdate();
    bool ok = g_Created == 3 && s.PParticle1 == &g_Things[0] && s.PParticleFish == &g_Things[2] &&
              std::strcmp(g_Names[2], "UNDERWATER_FISH_01") == 0 && g_Things[0].E.AttachType == 0x40 &&
              g_Things[2].E.AttachType == 0x80 && g_Things[2].E.Offset == -1.0f && g_Things[1].E.AlwaysUpdate;
    s.PParticle2 = 0; s.FrameUpdate();
    ok = ok && g_Created == 4 && s.PParticle2 == &g_Things[3] && std::strcmp(g_Names[3], "FISHING_SPOT_02") == 0 && g_Things[0].E.Attached == 1;
    s.PParticle1 = 0; g_FailCreate = true; s.FrameUpdate();
    ok = ok && s.PParticle1 == 0 && g_Created == 4;
    s.Enabled = false; s.FrameUpdate();
    ok = ok && s.Removed == 1;
    if (!ok) { std::printf("007b0360_TEST FAIL\n"); return 1; }
    std::printf("007b0360_TEST PASS\n");
    return 0;
}