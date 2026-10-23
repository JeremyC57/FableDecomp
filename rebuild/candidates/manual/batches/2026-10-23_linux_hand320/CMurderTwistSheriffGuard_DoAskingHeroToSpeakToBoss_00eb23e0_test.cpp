// Behaviour test for 0x00EB23E0 CMurderTwistSheriffGuard::DoAskingHeroToSpeakToBoss: talk -> greeting (or task-declined) prefix
// + Name + NextLine (1..2); hit (not special 14) and not enflamed -> "attacked" + Name + NextLine; NextLine advanced each time.
#include <cstdio>
#include <cstring>

struct Mock {
    bool talked, hit, hitAny, hitSpecial14, enflamed, terminating, taskDeclined;
    float health;
    int pauseDepth, speakCount, framesLeft;
    char lastLine[96];
};
static Mock g;

struct CMurderTwistSheriffGuard {
    unsigned long NextLine;
    char Name[16];
    void Speak(const char* text) { std::strcpy(g.lastLine, text); ++g.speakCount; g.framesLeft = 1; }
    bool IsPerformingScriptTask() const { return g.framesLeft > 0; }
    void NewScriptFrame() { --g.framesLeft; }
    void Pause(bool p) { g.pauseDepth += p ? 1 : -1; }
    bool Line(const char* prefix)
    {
        if (NextLine > 2) NextLine = 1;
        char text[96];
        std::sprintf(text, "%s%s%lu", prefix, Name, NextLine);
        if (g.health > 0.0f) {
            Speak(text);
            while (IsPerformingScriptTask()) {
                NewScriptFrame();
                if (g.terminating) return false;
            }
            if (g.terminating) return false;
        }
        NextLine++;
        return true;
    }
    void DoAskingHeroToSpeakToBoss()
    {
        if (g.talked) {
            if (g.terminating) return;
            const char* prefix = g.taskDeclined ? "TEXT_QST_B07_GUARD_TASK_DECLINED" : "TEXT_QST_B07_GUARD_GREETING";
            Pause(true);
            bool done = Line(prefix);
            Pause(false);
            if (!done) return;
        }
        if ((g.hit || (g.hitAny && !g.hitSpecial14)) && !g.enflamed) {
            if (g.terminating) return;
            Pause(true);
            Line("TEXT_QST_B07_GUARD_ATTACKED");
            Pause(false);
        }
    }
};

int main()
{
    CMurderTwistSheriffGuard guard;
    guard.NextLine = 0;
    std::strcpy(guard.Name, "_B");
    std::memset(&g, 0, sizeof(g));
    g.talked = true; g.health = 2.0f;
    guard.DoAskingHeroToSpeakToBoss();
    bool ok = guard.NextLine == 1 && std::strcmp(g.lastLine, "TEXT_QST_B07_GUARD_GREETING_B0") == 0 && g.pauseDepth == 0;
    g.taskDeclined = true;
    guard.DoAskingHeroToSpeakToBoss();
    ok = ok && guard.NextLine == 2 && std::strcmp(g.lastLine, "TEXT_QST_B07_GUARD_TASK_DECLINED_B1") == 0;
    std::memset(&g, 0, sizeof(g));
    g.hit = true; g.health = 2.0f;
    guard.DoAskingHeroToSpeakToBoss();
    ok = ok && guard.NextLine == 3 && std::strcmp(g.lastLine, "TEXT_QST_B07_GUARD_ATTACKED_B2") == 0 && g.pauseDepth == 0;
    if (!ok) {
        std::printf("00eb23e0_TEST FAIL\n");
        return 1;
    }
    std::printf("00eb23e0_TEST PASS\n");
    return 0;
}
