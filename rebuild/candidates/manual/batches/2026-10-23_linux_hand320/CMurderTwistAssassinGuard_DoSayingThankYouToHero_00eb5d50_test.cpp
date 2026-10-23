// Behaviour test for 0x00EB3740 CMurderTwistAssassinGuard::DoSayingThankYouToHero: talk -> "grateful" + DataString + NextLine (1..2),
// NextLine advanced; hit (not special 14) and not enflamed -> attacked line and AssassinState 2; pause/unpause balanced.
#include <cstdio>
#include <cstring>

struct Mock {
    bool talked, hit, hitAny, hitSpecial14, enflamed, terminating;
    float health;
    int pauseDepth, speakCount, framesLeft, assassinState;
    char lastLine[96];
};
static Mock g;

struct CMurderTwistAssassinGuard {
    unsigned long NextLine;
    char DataString[16];
    void Speak(const char* text) { std::strcpy(g.lastLine, text); ++g.speakCount; g.framesLeft = 1; }
    bool IsPerformingScriptTask() const { return g.framesLeft > 0; }
    void NewScriptFrame() { --g.framesLeft; }
    void Pause(bool p) { g.pauseDepth += p ? 1 : -1; }
    void DoSayingThankYouToHero()
    {
        if (g.talked) {
            if (g.terminating) return;
            Pause(true);
            if (NextLine > 2) NextLine = 1;
            char text[96];
            std::sprintf(text, "TEXT_QST_B07_HENCHMAN_THANKS%s%lu", DataString, NextLine);
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
        if ((g.hit || (g.hitAny && !g.hitSpecial14)) && !g.enflamed) {
            if (g.terminating) return;
            Pause(true);
            if (g.health > 0.0f) {
                char text[96];
                std::sprintf(text, "TEXT_QST_B07_HENCHMAN_DOUBLE_CROSSED%s", DataString);
                Speak(text);
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
    CMurderTwistAssassinGuard guard;
    guard.NextLine = 5;
    std::strcpy(guard.DataString, "_A");
    std::memset(&g, 0, sizeof(g));
    g.talked = true; g.health = 3.0f;
    guard.DoSayingThankYouToHero();
    bool ok = guard.NextLine == 2 && std::strcmp(g.lastLine, "TEXT_QST_B07_HENCHMAN_THANKS_A1") == 0 && g.pauseDepth == 0;
    std::memset(&g, 0, sizeof(g));
    g.hit = true; g.enflamed = true; g.health = 3.0f;
    guard.DoSayingThankYouToHero();
    ok = ok && g.assassinState == 0 && g.speakCount == 0;
    g.enflamed = false;
    guard.DoSayingThankYouToHero();
    ok = ok && g.assassinState == 2 && std::strcmp(g.lastLine, "TEXT_QST_B07_HENCHMAN_DOUBLE_CROSSED_A") == 0 && g.pauseDepth == 0;
    if (!ok) {
        std::printf("00eb5d50_TEST FAIL\n");
        return 1;
    }
    std::printf("00eb5d50_TEST PASS\n");
    return 0;
}
