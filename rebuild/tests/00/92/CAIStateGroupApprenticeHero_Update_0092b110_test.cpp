// Behaviour test for 0092b110 CAIStateGroup_ApprenticeHero::Update: a villager apprentice keeps picking one of five
// practice activities (world-seed LCG 0x24A1/0x24DF + ror 13, modulo 5) while the village's cook time lasts; a failed
// activity or a stopping group terminates at once, otherwise the next practice frame is set one minute ahead and the
// group terminates. No village-member component or no village: nothing happens.
#include <cstdio>
typedef unsigned int u32;
static u32 GFROR13(u32 v) { return (v >> 13) | (v << 19); }
static long g_framesPerSecond = 30;
static long g_currentFrame = 1000;
static long GFGetCurrentFrame() { return g_currentFrame; }
class CTCVillage { public: int cookChecks; int cookLimit; bool IsCookTime() { return cookChecks++ < cookLimit; } };
class CTCVillageMember { public: CTCVillage* village; CTCVillage* GetPVillage() { return village; } };
class CThing { public:
    bool hasVillageMember; CTCVillageMember* member;
    bool HasTC(long) const { return hasVillageMember; }
};
struct CAIStateGroup_ApprenticeHero {
    CThing* creature; u32 seed; int calls[5]; bool result; bool stopping; int terminated; long NextFramePractice;
    CThing* GetPCreature() const { return creature; }
    CTCVillageMember* GetVillageMember() { return GetPCreature()->member; }
    bool IsStopping() { return stopping; }
    void Terminate() { ++terminated; }
    u32 Random(u32 range) { u32 next = seed * 0x24A1U + 0x24DFU; seed = next; u32 r = GFROR13(next); seed = r; return r % range; }
    bool PracticeSkills() { ++calls[0]; return result; }
    bool PracticeAgainstApprentice() { ++calls[1]; return result; }
    bool GoToLibrary() { ++calls[2]; return result; }
    bool LookAtLeaderBoard() { ++calls[3]; return result; }
    bool LookAtMap() { ++calls[4]; return result; }
    void Update()
    {
        CThing* c = GetPCreature();
        if (!c->HasTC(0x23)) return;
        CTCVillage* village = GetVillageMember()->GetPVillage();
        if (village == 0) return;
        while (village->IsCookTime()) {
            bool ok = false;
            switch (Random(5)) {
            case 0: ok = PracticeSkills(); break;
            case 1: ok = PracticeAgainstApprentice(); break;
            case 2: ok = GoToLibrary(); break;
            case 3: ok = LookAtLeaderBoard(); break;
            case 4: ok = LookAtMap(); break;
            }
            if (!ok || IsStopping()) { Terminate(); return; }
        }
        long fps = g_framesPerSecond;
        NextFramePractice = GFGetCurrentFrame() + fps * 60;
        Terminate();
    }
};
static void Reset(CAIStateGroup_ApprenticeHero& g, CThing* c)
{
    g.creature = c; g.seed = 12345; for (int i = 0; i < 5; ++i) g.calls[i] = 0;
    g.result = true; g.stopping = false; g.terminated = 0; g.NextFramePractice = -1;
}
int main()
{
    int ok = 1;
    CTCVillage v; CTCVillageMember m; m.village = &v;
    CThing c; c.hasVillageMember = true; c.member = &m;
    CAIStateGroup_ApprenticeHero g;
    /* three successful activities, then cook time ends */
    Reset(g, &c); v.cookChecks = 0; v.cookLimit = 3;
    u32 seed = 12345; int want[5] = { 0, 0, 0, 0, 0 };
    for (int i = 0; i < 3; ++i) { seed = GFROR13(seed * 0x24A1U + 0x24DFU); ++want[seed % 5]; }
    g.Update();
    for (int i = 0; i < 5; ++i) if (g.calls[i] != want[i]) ok = 0;
    if (g.seed != seed || g.terminated != 1 || g.NextFramePractice != 1000 + 30 * 60) ok = 0;
    /* a failing activity terminates at once without setting the frame */
    Reset(g, &c); v.cookChecks = 0; v.cookLimit = 10; g.result = false;
    g.Update();
    if (g.terminated != 1 || g.NextFramePractice != -1 || v.cookChecks != 1) ok = 0;
    /* a stopping group too */
    Reset(g, &c); v.cookChecks = 0; v.cookLimit = 10; g.stopping = true;
    g.Update();
    if (g.terminated != 1 || g.NextFramePractice != -1) ok = 0;
    /* no village / no component: nothing */
    Reset(g, &c); m.village = 0; g.Update();
    if (g.terminated != 0) ok = 0;
    m.village = &v; c.hasVillageMember = false; Reset(g, &c); g.Update();
    if (g.terminated != 0 || g.NextFramePractice != -1) ok = 0;
    if (!ok) { std::printf("0092b110_TEST FAIL\n"); return 1; }
    std::printf("0092b110_TEST PASS\n");
    return 0;
}