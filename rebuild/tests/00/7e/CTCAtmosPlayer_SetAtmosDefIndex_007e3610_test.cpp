#include <cstdio>
#include <cmath>
// Behaviour test: CTCAtmosPlayer::SetAtmosDefIndex -- stores the index; a non-zero index creates an emitter at the parent position (when a sound system exists) and plays the def name on it.
#define CHECK(c) if (!(c)) { std::printf("007e3610_TEST FAIL\n"); return 1; }
class C3DVector { public: float X, Y, Z; };
struct CEmitter { int plays; const char* last; CEmitter() : plays(0), last(0) { } void Play(const char* name) { ++plays; last = name; } };
static CEmitter gEmitter;
static int g_created = 0;
static C3DVector g_createdAt;
struct CSoundSystem { CEmitter* CreateEmitter(const C3DVector& p) { ++g_created; g_createdAt = p; return &gEmitter; } };
static CSoundSystem g_system;
static CSoundSystem* g_pSoundSystem = 0;
static const char* g_names[3] = { "", "ATMOS_FOREST", "ATMOS_CAVE" };
class CThing { public: C3DVector Position; const C3DVector& GetPosition() const { return Position; } };
struct CTCAtmosPlayer {
    CThing*   ParentThing;
    long      AtmosDefIndex;
    CEmitter* PSoundEmitter;
    void SetAtmosDefIndex(long index)
    {
        AtmosDefIndex = index;
        if (index) {
            if (g_pSoundSystem)
                PSoundEmitter = g_pSoundSystem->CreateEmitter(ParentThing->GetPosition());
            if (PSoundEmitter)
                PSoundEmitter->Play(g_names[AtmosDefIndex]);
        }
    }
};
int main()
{
    CThing thing; thing.Position.X = 1.0f; thing.Position.Y = 2.0f; thing.Position.Z = 3.0f;
    CTCAtmosPlayer p; p.ParentThing = &thing; p.AtmosDefIndex = 0; p.PSoundEmitter = 0;
    p.SetAtmosDefIndex(1);
    CHECK(p.AtmosDefIndex == 1 && p.PSoundEmitter == 0 && gEmitter.plays == 0)
    g_pSoundSystem = &g_system;
    p.SetAtmosDefIndex(0);
    CHECK(p.AtmosDefIndex == 0 && g_created == 0)
    p.SetAtmosDefIndex(2);
    CHECK(g_created == 1 && g_createdAt.Y == 2.0f && p.PSoundEmitter == &gEmitter && gEmitter.plays == 1 && gEmitter.last == g_names[2])
    std::printf("007e3610_TEST PASS\n");
    return 0;
}