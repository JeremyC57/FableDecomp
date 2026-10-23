// Behaviour test for 0x00EB4F80 CMurderTwistAssassin::DoRemindingHeroToKillSheriff: talk -> cycling task line (1..2) while alive,
// NextLine advanced, pause/unpause paired; hit by hero (or by a special ability other than 14) -> AssassinState 2.
#include <cstdio>
#include <cstring>

struct Mock {
    bool talked, hit, hitAny, hitSpecial14, terminating;
    float health;
    int pauseDepth, speakCount, framesLeft;
    char lastLine[64];
    int assassinState;
};
static Mock g;

struct CMurderTwistAssassin {
    unsigned long NextLine;
    void Speak(const char* text) { std::strcpy(g.lastLine, text); ++g.speakCount; g.framesLeft = 2; }
    bool IsPerformingScriptTask() const { return g.framesLeft > 0; }
    void NewScriptFrame() { --g.framesLeft; }
    void Pause(bool p) { g.pauseDepth += p ? 1 : -1; }
    void DoRemindingHeroToKillSheriff()
    {
        if (g.talked) {
            if (g.terminating) return;
            Pause(true);
            if (NextLine > 2) NextLine = 1;
            char text[64];
            std::sprintf(text, "TEXT_QST_B07_ASSASSIN_TASK_INCOMPLETE%lu", NextLine);
            if (g.health > 0.0f) {
                Speak(text);
                while (IsPerformingScriptTask()) {
                    NewScriptFrame();
                    if (g.terminating) { Pause(false); return; }
                }
                if (g.terminating) { Pause(false); return; }
            }
            NextLine++;
            Pause(false);
        }
        if (g.hit || (g.hitAny && !g.hitSpecial14)) {
            if (g.terminating) return;
            Pause(true);
            if (g.health > 0.0f) {
                Speak("TEXT_QST_B07_ASSASSIN_DOUBLE_CROSSED");
                while (IsPerformingScriptTask()) {
                    NewScriptFrame();
                    if (g.terminating) { Pause(false); return; }
                }
                if (g.terminating) { Pause(false); return; }
            }
            g.assassinState = 2;
            Pause(false);
        }
    }
};

int main()
{
    CMurderTwistAssassin a;
    a.NextLine = 2;
    std::memset(&g, 0, sizeof(g));
    g.talked = true; g.health = 10.0f;
    a.DoRemindingHeroToKillSheriff();
    bool ok = a.NextLine == 3 && std::strcmp(g.lastLine, "TEXT_QST_B07_ASSASSIN_TASK_INCOMPLETE2") == 0 && g.pauseDepth == 0;
    a.DoRemindingHeroToKillSheriff();
    ok = ok && a.NextLine == 2 && std::strcmp(g.lastLine, "TEXT_QST_B07_ASSASSIN_TASK_INCOMPLETE1") == 0;
    std::memset(&g, 0, sizeof(g));
    g.hitAny = true; g.hitSpecial14 = true; g.health = 1.0f;
    a.DoRemindingHeroToKillSheriff();
    ok = ok && g.assassinState == 0 && g.speakCount == 0;
    g.hitSpecial14 = false;
    a.DoRemindingHeroToKillSheriff();
    ok = ok && g.assassinState == 2 && g.speakCount == 1 && g.pauseDepth == 0;
    if (!ok) {
        std::printf("00eb4f80_TEST FAIL\n");
        return 1;
    }
    std::printf("00eb4f80_TEST PASS\n");
    return 0;
}