// Behaviour test for 0x00EB2DC0 NScript::CV_MurderTwistFinalScript::CMurderTwistSheriffGuard::DoAskingHeroToSpeakToBossAgain: talked to by the hero ->
//   paused "TEXT_QST_B07_GUARD_TASK_INCOMPLETE" + Name + NextLine (1..2); hit by the hero (or by a special ability other than 14) and not enflamed ->
//   paused "TEXT_QST_B07_GUARD_TASK_INCOMPLETE_ATTACKED" + Name + NextLine; NextLine advanced after each line; a terminating thread stops before NextLine moves.
#include <cstdio>
#include <cstring>

struct Mock {
    bool talked, hit, hitAny, hitSpecial14, enflamed, terminating;
    float health;
    int pauseDepth, movieDepth, speakCount, framesLeft;
    char lastLine[96];
};
static Mock g;

struct CMurderTwistSheriffGuard {
    unsigned long NextLine;
    char Name[16];
    void Speak(const char* text) { std::strcpy(g.lastLine, text); ++g.speakCount; g.framesLeft = 1; }
    bool IsPerformingScriptTask() const { return g.framesLeft > 0; }
    void NewScriptFrame() { --g.framesLeft; }
    struct Paused {   // the movie sequence plus CWideScreenMagicPauseEntities, released on every exit
        Paused() { ++g.movieDepth; ++g.pauseDepth; }
        ~Paused() { --g.pauseDepth; --g.movieDepth; }
    };
    void Line(const char* prefix)
    {
        Paused paused;
        if (NextLine > 2) NextLine = 1;
        char text[96];
        std::sprintf(text, "%s%s%lu", prefix, Name, NextLine);
        if (g.health > 0.0f) {
            Speak(text);
            while (IsPerformingScriptTask()) {
                NewScriptFrame();
                if (g.terminating) return;
            }
            if (g.terminating) return;
        }
        NextLine++;
    }
    void DoAskingHeroToSpeakToBossAgain()
    {
        if (g.talked) {
            if (g.terminating) return;
            Line("TEXT_QST_B07_GUARD_TASK_INCOMPLETE");
        }
        if ((g.hit || (g.hitAny && !g.hitSpecial14)) && !g.enflamed) {
            if (g.terminating) return;
            Line("TEXT_QST_B07_GUARD_TASK_INCOMPLETE_ATTACKED");
        }
    }
};

int main()
{
    CMurderTwistSheriffGuard guard;
    guard.NextLine = 3;
    std::strcpy(guard.Name, "_A");
    std::memset(&g, 0, sizeof(g));
    g.talked = true; g.health = 1.0f;
    guard.DoAskingHeroToSpeakToBossAgain();
    bool ok = guard.NextLine == 2 && std::strcmp(g.lastLine, "TEXT_QST_B07_GUARD_TASK_INCOMPLETE_A1") == 0 && g.pauseDepth == 0 && g.movieDepth == 0;
    guard.DoAskingHeroToSpeakToBossAgain();
    ok = ok && guard.NextLine == 3 && std::strcmp(g.lastLine, "TEXT_QST_B07_GUARD_TASK_INCOMPLETE_A2") == 0;
    std::memset(&g, 0, sizeof(g));
    g.hitAny = true; g.health = 1.0f;
    guard.DoAskingHeroToSpeakToBossAgain();
    ok = ok && guard.NextLine == 2 && std::strcmp(g.lastLine, "TEXT_QST_B07_GUARD_TASK_INCOMPLETE_ATTACKED_A1") == 0 && g.pauseDepth == 0;
    std::memset(&g, 0, sizeof(g));
    g.hitAny = true; g.hitSpecial14 = true; g.health = 1.0f;
    guard.DoAskingHeroToSpeakToBossAgain();
    ok = ok && guard.NextLine == 2 && g.speakCount == 0;
    std::memset(&g, 0, sizeof(g));
    g.hit = true; g.enflamed = true; g.health = 1.0f;
    guard.DoAskingHeroToSpeakToBossAgain();
    ok = ok && guard.NextLine == 2 && g.speakCount == 0;
    std::memset(&g, 0, sizeof(g));
    g.talked = true; g.health = 0.0f;
    guard.DoAskingHeroToSpeakToBossAgain();
    ok = ok && guard.NextLine == 3 && g.speakCount == 0 && g.pauseDepth == 0;
    if (!ok) {
        std::printf("00eb2dc0_TEST FAIL\n");
        return 1;
    }
    std::printf("00eb2dc0_TEST PASS\n");
    return 0;
}