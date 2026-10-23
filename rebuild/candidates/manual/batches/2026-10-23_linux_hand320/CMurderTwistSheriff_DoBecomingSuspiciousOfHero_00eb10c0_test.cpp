// Behaviour test for 0x00eb10c0 CMurderTwistSheriff::DoBecomingSuspiciousOfHero: talk -> first the single line and the flag, later the numbered line
// (NextLine 1..2, advanced); hit (not special 14) -> SheriffState 2; pause/unpause balanced.
#include <cstdio>
#include <cstring>

struct Mock {
    bool talked, hit, hitAny, hitSpecial14, terminating;
    float health;
    int pauseDepth, speakCount, framesLeft, sheriffState, enflameWaits;
    char lastLine[96];
};
static Mock g;

struct CMurderTwistSheriff {
    unsigned long NextLine;
    bool Flag;
    void Speak(const char* text) { std::strcpy(g.lastLine, text); ++g.speakCount; g.framesLeft = 2; }
    bool IsPerformingScriptTask() const { return g.framesLeft > 0; }
    void NewScriptFrame() { --g.framesLeft; }
    void Pause(bool p) { g.pauseDepth += p ? 1 : -1; }
    bool SpeakAndWait(const char* text)
    {
        if (g.health > 0.0f) {
            Speak(text);
            while (IsPerformingScriptTask()) {
                NewScriptFrame();
                if (g.terminating) return false;
            }
            if (g.terminating) return false;
        }
        return true;
    }
    void Run()
    {
        if (g.talked) {
            if (g.terminating) return;
            Pause(true);
            if (Flag) {
                if (g.terminating) { Pause(false); return; }
                if (NextLine > 2) NextLine = 1;
                char text[96];
                std::sprintf(text, "TEXT_QST_B07_SHERIFF_ASSASSIN_FOUND_NOT_KILLED%lu", NextLine);
                if (!SpeakAndWait(text)) { Pause(false); return; }
                NextLine++;
            } else {
                if (g.terminating) { Pause(false); return; }
                if (!SpeakAndWait("TEXT_QST_B07_SHERIFF_ASSASSIN_FOUND_NOT_KILLED")) { Pause(false); return; }
                Flag = true;
            }
            Pause(false);
        }
        if (g.hit || (g.hitAny && !g.hitSpecial14)) {
            if (g.terminating) return;
            ++g.enflameWaits;
            Pause(true);
            if (!SpeakAndWait("TEXT_QST_B07_SHERIFF_HERO_SUSSED")) { Pause(false); return; }
            g.sheriffState = 2;
            Pause(false);
        }
    }
};

int main()
{
    CMurderTwistSheriff s;
    s.NextLine = 7; s.Flag = false;
    std::memset(&g, 0, sizeof(g));
    g.talked = true; g.health = 1.0f;
    s.Run();
    bool ok = s.Flag && s.NextLine == 7 && std::strcmp(g.lastLine, "TEXT_QST_B07_SHERIFF_ASSASSIN_FOUND_NOT_KILLED") == 0 && g.pauseDepth == 0;
    s.Run();
    ok = ok && s.NextLine == 2 && std::strcmp(g.lastLine, "TEXT_QST_B07_SHERIFF_ASSASSIN_FOUND_NOT_KILLED1") == 0;
    std::memset(&g, 0, sizeof(g));
    g.hitAny = true; g.health = 1.0f;
    s.Run();
    ok = ok && g.sheriffState == 2 && g.enflameWaits == 1 && std::strcmp(g.lastLine, "TEXT_QST_B07_SHERIFF_HERO_SUSSED") == 0 && g.pauseDepth == 0;
    if (!ok) {
        std::printf("00eb10c0_TEST FAIL\n");
        return 1;
    }
    std::printf("00eb10c0_TEST PASS\n");
    return 0;
}
