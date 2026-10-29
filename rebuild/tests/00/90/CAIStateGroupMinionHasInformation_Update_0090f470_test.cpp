// Behaviour test for CAIStateGroup_MinionHasInformation::Update (0x0090F470): model copy, scripted stubs.
#include <cstdio>
#include <cstring>

static long g_frame, g_framesPerSecond = 30;
class CWorld { public: static long __fastcall GetFrame() { return g_frame; } };
inline bool HasTimePassed(const long& lastFrame, long seconds)
{
    long fps = g_framesPerSecond;
    return CWorld::GetFrame() - lastFrame > seconds * fps;
}
static int g_sounds;
class CCharString { public: const char* text; CCharString(const char* t) : text(t) { } };
struct CTCSound {
    void PlaySound(const CCharString& name, long, long, long, long d)
    {
        if (std::strcmp(name.text, "ATTRACT_HERO_ATTENTION") == 0 && d == -1) ++g_sounds;
    }
};
class CThing { public:
    unsigned char m_flags91;
    CTCSound* sound;
    bool GetInterface(CTCSound** out, long type) { if (type != 0x10 || !sound) return false; *out = sound; return true; }
};
class CThingAICreature : public CThing { public:
    bool wander, wave;
    bool GetWanderWithInformation() const { return wander; }
    bool GetWaveWithInformation() const { return wave; }
};
struct CIntelligentPointer { const CThing* item; CIntelligentPointer(const CThing* t) : item(t) { } };
static CThing* g_hero;
class CAIBrain { public: CThingAICreature* PCreature; };

static int g_wants[512], g_wi, g_look[512], g_li, g_nothing[512], g_ni, g_wanderOk[512], g_wo, g_terminated, g_lastFrames;
static float g_lastSpeed;
static const CThing* g_lastLooked;
struct CAIStateGroup_MinionHasInformation {
    CAIBrain* PBrain;
    long LastFrameWaved, LastFrameWandered, FrameInitialised, WavesWithoutCalling;
    CThingAICreature* GetCreature() const { return PBrain->PCreature; }
    CThing* GetHero() { return g_hero; }
    void Terminate() { ++g_terminated; }
    bool CreatureWantsPlayersAttention() { return g_wants[g_wi++] != 0; }
    bool Wander() { return g_wanderOk[g_wo++] != 0; }
    long AIAction_LookAtThing(long repeat, CIntelligentPointer thing, long frames, float speed)
    {
        g_lastLooked = thing.item; g_lastFrames = frames; g_lastSpeed = speed; if (repeat != 1) return -1; return g_look[g_li++];
    }
    long AIAction_DoNothing() { ++g_frame; return g_nothing[g_ni++]; }
    void Update();
};

void CAIStateGroup_MinionHasInformation::Update()
{
    CThing* hero = GetHero();
    if (hero == 0 || (hero->m_flags91 & 1)) {
        Terminate();
        return;
    }
    do {
        if (!CreatureWantsPlayersAttention()) {
            Terminate();
            return;
        }
        if (AIAction_LookAtThing(1, CIntelligentPointer(hero), 0xb, 0.013888889f) != 2)
            return;
        if (GetCreature()->GetWaveWithInformation()) {
            if (LastFrameWaved == 0 || HasTimePassed(LastFrameWaved, 5)) {
                LastFrameWaved = CWorld::GetFrame();
                bool attract = true;
                if (LastFrameWaved - FrameInitialised > 20 * g_framesPerSecond) {
                    if (++WavesWithoutCalling > 4)
                        WavesWithoutCalling = 0;
                    else
                        attract = false;
                }
                CTCSound* sound;
                if (attract && GetCreature()->GetInterface(&sound, 0x10)) {
                    CCharString name("ATTRACT_HERO_ATTENTION");
                    sound->PlaySound(name, 0, 0, 0, -1);
                }
            }
        }
        if (GetCreature()->GetWanderWithInformation()) {
            if (LastFrameWandered == 0 || HasTimePassed(LastFrameWandered, 10)) {
                LastFrameWandered = CWorld::GetFrame();
                if (!Wander()) {
                    Terminate();
                    return;
                }
            }
        }
    } while (AIAction_DoNothing() == 4);
}

static void Reset(CAIStateGroup_MinionHasInformation& s)
{
    g_wi = g_li = g_ni = g_wo = g_terminated = g_sounds = 0;
    for (int i = 0; i < 512; ++i) { g_wants[i] = 1; g_look[i] = 2; g_nothing[i] = 4; g_wanderOk[i] = 1; }
    s.LastFrameWaved = s.LastFrameWandered = s.FrameInitialised = s.WavesWithoutCalling = 0;
}

int main()
{
    int ok = 1;
    CTCSound sound;
    CThing hero = { 0, 0 };
    CThingAICreature me; me.m_flags91 = 0; me.sound = &sound; me.wander = false; me.wave = true;
    CAIBrain brain = { &me };
    CAIStateGroup_MinionHasInformation s; s.PBrain = &brain;
    // no hero / dead hero: terminate at once
    Reset(s); g_hero = 0; s.Update(); if (g_terminated != 1 || g_wi) ok = 0;
    Reset(s); g_hero = &hero; hero.m_flags91 = 1; s.Update(); if (g_terminated != 1 || g_wi) ok = 0;
    hero.m_flags91 = 0;
    // waves: first at frame 1, then only after more than 150 frames (5 s at 30 fps); stop after 200 frames
    Reset(s); g_frame = 1; s.FrameInitialised = 1; g_nothing[199] = 2; s.Update();
    if (g_terminated || g_sounds != 2 || s.LastFrameWaved != 152) ok = 0;
    if (g_lastLooked != &hero || g_lastFrames != 0xb || g_lastSpeed != 0.013888889f) ok = 0;
    // after 20 s only every fifth wave calls out
    Reset(s); g_frame = 1000; s.FrameInitialised = 1; g_nothing[0] = 2; s.WavesWithoutCalling = 2; s.Update();
    if (g_sounds != 0 || s.WavesWithoutCalling != 3) ok = 0;
    Reset(s); g_frame = 1000; s.FrameInitialised = 1; g_nothing[0] = 2; s.WavesWithoutCalling = 4; s.Update();
    if (g_sounds != 1 || s.WavesWithoutCalling != 0) ok = 0;
    // no sound component: no call, still waves
    me.sound = 0; Reset(s); g_frame = 5; g_nothing[0] = 2; s.Update(); if (g_sounds || s.LastFrameWaved != 5) ok = 0;
    me.sound = &sound;
    // look fails: leave without terminating
    Reset(s); g_look[0] = 3; s.Update(); if (g_terminated || g_ni) ok = 0;
    // wanting stops: terminate
    Reset(s); g_wants[1] = 0; s.Update(); if (g_terminated != 1 || g_ni != 1) ok = 0;
    // wandering: the failed wander terminates; wander repeats only after 10 s
    me.wave = false; me.wander = true;
    Reset(s); g_frame = 10; g_wanderOk[0] = 0; s.Update(); if (g_terminated != 1 || g_wo != 1 || s.LastFrameWandered != 10) ok = 0;
    Reset(s); g_frame = 10; g_nothing[400] = 2; s.Update(); if (g_terminated || g_wo != 2 || s.LastFrameWandered != 311) ok = 0;
    if (!ok) { std::printf("0090f470_TEST FAIL\n"); return 1; }
    std::printf("0090f470_TEST PASS\n");
    return 0;
}