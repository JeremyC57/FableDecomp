#include <cstdio>
#include <cstring>
// behaviour test: CTCHeroCentreDoorMarker::UpdateParticleTeleporter kills a live particle when the world flag is off and
// creates + attaches the "MARKTELEPORTER" particle (attach type 0x20, empty helper) when the flag is on and none is alive
struct C3DVector { float X, Y, Z; };
struct CTCDParticleEmitter;
struct CThing {
    bool Dead; bool hasEmitter; CTCDParticleEmitter* emitter; C3DVector Pos; int kills;
    const C3DVector& GetPosition() const { return Pos; }
    void Kill(bool) { ++kills; Dead = true; }
    unsigned long HasTC(long type) const { return type == 0x4b && hasEmitter ? 1u : 0u; }
    template<class T> T* GetTC(long) { return (T*)emitter; }
};
struct CCharString { const char* s; CCharString(const char* t) : s(t) {} };
static CThing g_created; static char g_name[32]; static int g_creates = 0;
static CThing* Create(const CCharString& name, const C3DVector&, bool) { ++g_creates; std::strcpy(g_name, name.s); return &g_created; }
struct CTCDParticleEmitter {
    CThing* attached; long type; const char* helper;
    void AttachToThing(CThing* t, long a, const CCharString& h, long, float) { attached = t; type = a; helper = h.s; }
};
struct CIntelligentPointer {
    CThing* p;
    CIntelligentPointer(CThing* t) : p(t) {}
    CThing* GetPItem() const { return p; }
};
static bool IS_THING_ALIVE(const CIntelligentPointer& p) { return p.GetPItem() && !p.GetPItem()->Dead; }
struct CWorld { bool WorldFlagD7; };
static CWorld g_world;
struct CTCHeroCentreDoorMarker {
    CThing* ParentThing; CIntelligentPointer PParticle;
    CTCHeroCentreDoorMarker() : PParticle(0) {}
    CWorld* GetWorld() const { return &g_world; }
    void UpdateParticleTeleporter()
    {
        if (IS_THING_ALIVE(PParticle)) {
            if (!GetWorld()->WorldFlagD7) {
                PParticle.GetPItem()->Kill(true);
                PParticle = CIntelligentPointer(0);
            }
        } else if (GetWorld()->WorldFlagD7) {
            PParticle = CIntelligentPointer(Create("MARKTELEPORTER", ParentThing->GetPosition(), false));
            if (IS_THING_ALIVE(PParticle)) {
                CTCDParticleEmitter* emitter = 0;
                CThing* particle = PParticle.GetPItem();
                if (particle->HasTC(0x4b))
                    emitter = particle->GetTC<CTCDParticleEmitter>(0x4b);
                emitter->AttachToThing(ParentThing, 0x20, "", 0, 0.0f);
            }
        }
    }
};
int main()
{
    CThing me = {false, false, 0, {1, 2, 3}, 0};
    CTCDParticleEmitter em = {0, 0, 0};
    g_created.Dead = false; g_created.hasEmitter = true; g_created.emitter = &em; g_created.kills = 0;
    CTCHeroCentreDoorMarker m; m.ParentThing = &me;
    g_world.WorldFlagD7 = false;
    m.UpdateParticleTeleporter();                       // off, no particle: nothing
    bool ok = g_creates == 0 && m.PParticle.p == 0;
    g_world.WorldFlagD7 = true;
    m.UpdateParticleTeleporter();                       // on: create and attach
    ok = ok && g_creates == 1 && std::strcmp(g_name, "MARKTELEPORTER") == 0 && m.PParticle.p == &g_created
        && em.attached == &me && em.type == 0x20 && em.helper[0] == 0;
    m.UpdateParticleTeleporter();                       // on and alive: no second create
    ok = ok && g_creates == 1;
    g_world.WorldFlagD7 = false;
    m.UpdateParticleTeleporter();                       // off and alive: kill and clear
    ok = ok && g_created.kills == 1 && m.PParticle.p == 0;
    if (!ok) { std::printf("0077f7e0_TEST FAIL\n"); return 1; }
    std::printf("0077f7e0_TEST PASS\n");
    return 0;
}
