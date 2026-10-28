// Behaviour test: CCreatureAction_NymphTeleportOutOfLimbo::Initialise (0x008CA170) -- play the teleport effect, reset
// and force the creature to the new position, set the "TELEPORT_OUTOF_LIMBO" animation, run the base Initialise and
// stop the nymph flying around for the action's total time.
#include <cstdio>
#include <cstring>
static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct V3 { float X, Y, Z; };
class CTCNymph { public: int StopTime; void StopFlyingAround(int t) { StopTime = t; Log("stop"); } };
struct CCreature { V3 ResetPos; V3 ForcedPos; bool ForcedFlag; CTCNymph* Nymph; };
struct CTeleport {
    CCreature* PCreature;
    int TotalTime;
    V3 NewPos;
    char Anim[32];
    void Initialise()
    {
        Log("effect");
        PCreature->ResetPos = NewPos; Log("reset");
        PCreature->ForcedPos = NewPos; PCreature->ForcedFlag = false; Log("force");
        std::strcpy(Anim, "TELEPORT_OUTOF_LIMBO"); Log("anim");
        Log("base");
        if (PCreature->Nymph) PCreature->Nymph->StopFlyingAround(TotalTime);
    }
};
int main()
{
    CTCNymph nymph = { 0 };
    CCreature c; std::memset(&c, 0, sizeof(c)); c.Nymph = &nymph; c.ForcedFlag = true;
    CTeleport t; std::memset(&t, 0, sizeof(t)); t.PCreature = &c; t.TotalTime = 42; t.NewPos.Y = 5;
    t.Initialise();
    bool ok = std::strcmp(g_log, "effect;reset;force;anim;base;stop;") == 0 && nymph.StopTime == 42 && c.ResetPos.Y == 5
        && c.ForcedPos.Y == 5 && !c.ForcedFlag && std::strcmp(t.Anim, "TELEPORT_OUTOF_LIMBO") == 0;
    g_log[0] = 0; c.Nymph = 0; t.Initialise();
    ok = ok && std::strcmp(g_log, "effect;reset;force;anim;base;") == 0;
    if (!ok) { std::printf("008ca170_TEST FAIL\n"); return 1; }
    std::printf("008ca170_TEST PASS\n");
    return 0;
}