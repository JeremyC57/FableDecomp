// Behaviour test for CMurderTwistSheriff::Main (0x00EAEF30): the state machine driven by a fake game interface.
#include <cstdio>
#include <cstring>

static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ","); }

struct Parent { int SheriffState; int AssassinState; int SheriffSet; int HandleSet; void PlayBeingRumbledCutscene() { Log("cut"); } };

struct Sheriff {
    Parent* ParentClass;
    bool HasBeingRumbledCutscenePlayed;
    int frames, terminateAt, startFails;
    float distance;
    bool scripting;
    void NewScriptFrame() { ++frames; Log("f"); }
    bool IsActiveThreadTerminating() const { return frames >= terminateAt; }
    bool StartScriptingEntity() { if (startFails > 0) { --startFails; return false; } return true; }
    void DoAskingHeroToKillAssassin() { Log("ask"); ParentClass->SheriffState = 1; }
    void DoRemindingHeroToKillAssassin() { Log("remA"); ParentClass->AssassinState = 3; }
    void DoBecomingSuspiciousOfHero() { Log("susp"); ParentClass->AssassinState = 4; }
    void DoRemindingHeroToKillSheriff() { Log("remS"); ParentClass->SheriffState = 3; }
    void DoSuspicious() { Log("s3"); ParentClass->SheriffState = 2; }
    void DoFightHero() { Log("fight"); }
    void Main()
    {
        Log("pred");
        NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        Log("brain");
        if (scripting) { scripting = false; Log("rel"); }
        while (!StartScriptingEntity()) {
            NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        ParentClass->SheriffSet = 1;
        ParentClass->HandleSet = 1;
        while (ParentClass->SheriffState == 0) {
            NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            DoAskingHeroToKillAssassin();
        }
        if (IsActiveThreadTerminating()) return;
        while (ParentClass->AssassinState == 0 || ParentClass->AssassinState == 1) {
            NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            DoRemindingHeroToKillAssassin();
        }
        if (IsActiveThreadTerminating()) return;
        while (ParentClass->SheriffState == 1) {
            NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            if (!HasBeingRumbledCutscenePlayed && distance <= 49.0f) {
                if (IsActiveThreadTerminating()) return;
                ParentClass->PlayBeingRumbledCutscene();
                HasBeingRumbledCutscenePlayed = true;
            }
            if (ParentClass->AssassinState == 3 || ParentClass->AssassinState == 2) {
                if (IsActiveThreadTerminating()) return;
                DoBecomingSuspiciousOfHero();
            }
            if (ParentClass->AssassinState == 4) {
                if (IsActiveThreadTerminating()) return;
                DoRemindingHeroToKillSheriff();
            }
        }
        if (IsActiveThreadTerminating()) return;
        while (ParentClass->SheriffState == 3) {
            NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            DoSuspicious();
        }
        if (IsActiveThreadTerminating()) return;
        if (ParentClass->SheriffState == 2) {
            if (IsActiveThreadTerminating()) return;
            DoFightHero();
        }
        do {
            NewScriptFrame();
        } while (!IsActiveThreadTerminating());
    }
};

int main()
{
    Parent p; p.SheriffState = 0; p.AssassinState = 0; p.SheriffSet = 0; p.HandleSet = 0;
    Sheriff s; s.ParentClass = &p; s.HasBeingRumbledCutscenePlayed = false; s.frames = 0; s.terminateAt = 12;
    s.startFails = 1; s.distance = 10.0f; s.scripting = true;
    g_log[0] = 0;
    s.Main();
    const char* want = "pred,f,brain,rel,f,f,ask,f,remA,f,cut,susp,remS,f,s3,fight,f,f,f,f,f,f,";
    if (std::strcmp(g_log, want) != 0 || !s.HasBeingRumbledCutscenePlayed || p.SheriffSet != 1 || p.HandleSet != 1) {
        std::printf("%s\n00eaef30_TEST FAIL\n", g_log); return 1;
    }
    Sheriff t = s; t.frames = 0; t.terminateAt = 1; g_log[0] = 0; p.SheriffState = 0;
    t.Main();
    if (std::strcmp(g_log, "pred,f,") != 0) { std::printf("00eaef30_TEST FAIL\n"); return 1; }
    std::printf("00eaef30_TEST PASS\n");
    return 0;
}