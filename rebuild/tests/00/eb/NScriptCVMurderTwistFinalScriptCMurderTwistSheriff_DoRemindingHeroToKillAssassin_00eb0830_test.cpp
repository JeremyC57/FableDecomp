// Behaviour test for 0x00EB0830 NScript::CV_MurderTwistFinalScript::CMurderTwistSheriff::DoRemindingHeroToKillAssassin:
//   AssassinState 1 -> "HERO_RETURNED" on talk; AssassinState 0 -> "HERO_LOST" once, then "HERO_LOST" + NextLine; hit (not by
//   special ability 14) in either state -> enflame wait + "ATTACKED_HERO_LOST" + NextLine; other states do nothing.
#include <cstdio>
#include <cstring>

struct Mock {
    bool talked, hit, hitAny, hitSpecial14, terminating;
    float health;
    int pauseDepth, speakCount, framesLeft, enflameWaits;
    char lastLine[96];
};
static Mock g;

struct CMurderTwistSheriff {
    int AssassinState;
    unsigned long NextLine;
    bool HasGivenReminderToHero;
    struct Paused { Paused() { ++g.pauseDepth; } ~Paused() { --g.pauseDepth; } };
    void Speak(const char* text) { std::strcpy(g.lastLine, text); ++g.speakCount; g.framesLeft = 1; }
    bool Say(const char* text)
    {
        if (g.health > 0.0f) {
            Speak(text);
            while (g.framesLeft > 0) {
                --g.framesLeft;
                if (g.terminating) return false;
            }
            if (g.terminating) return false;
        }
        return true;
    }
    bool SayNumbered(const char* prefix)
    {
        if (NextLine > 2) NextLine = 1;
        char text[96];
        std::sprintf(text, "%s%lu", prefix, NextLine);
        if (!Say(text)) return false;
        NextLine++;
        return true;
    }
    void Hit()
    {
        if (g.hit || (g.hitAny && !g.hitSpecial14)) {
            if (g.terminating) return;
            ++g.enflameWaits;
            Paused paused;
            SayNumbered("TEXT_QST_B07_SHERIFF_ATTACKED_HERO_LOST");
        }
    }
    void DoRemindingHeroToKillAssassin()
    {
        if (AssassinState == 1) {
            if (g.terminating) return;
            if (g.talked) {
                if (g.terminating) return;
                Paused paused;
                if (!Say("TEXT_QST_B07_SHERIFF_HERO_RETURNED")) return;
            }
            Hit();
        } else if (AssassinState == 0) {
            if (g.terminating) return;
            if (g.talked) {
                if (g.terminating) return;
                Paused paused;
                if (HasGivenReminderToHero) {
                    if (g.terminating) return;
                    if (!SayNumbered("TEXT_QST_B07_SHERIFF_HERO_LOST")) return;
                } else {
                    if (g.terminating) return;
                    if (!Say("TEXT_QST_B07_SHERIFF_HERO_LOST")) return;
                    HasGivenReminderToHero = true;
                }
            }
            Hit();
        }
    }
};

int main()
{
    CMurderTwistSheriff s;
    s.AssassinState = 1; s.NextLine = 0; s.HasGivenReminderToHero = false;
    std::memset(&g, 0, sizeof(g));
    g.talked = true; g.health = 1.0f;
    s.DoRemindingHeroToKillAssassin();
    bool ok = std::strcmp(g.lastLine, "TEXT_QST_B07_SHERIFF_HERO_RETURNED") == 0 && g.pauseDepth == 0 && s.NextLine == 0;
    s.AssassinState = 0;
    s.DoRemindingHeroToKillAssassin();
    ok = ok && std::strcmp(g.lastLine, "TEXT_QST_B07_SHERIFF_HERO_LOST") == 0 && s.HasGivenReminderToHero && s.NextLine == 0;
    s.NextLine = 5;
    s.DoRemindingHeroToKillAssassin();
    ok = ok && std::strcmp(g.lastLine, "TEXT_QST_B07_SHERIFF_HERO_LOST1") == 0 && s.NextLine == 2;
    std::memset(&g, 0, sizeof(g));
    g.hit = true; g.health = 1.0f;
    s.DoRemindingHeroToKillAssassin();
    ok = ok && std::strcmp(g.lastLine, "TEXT_QST_B07_SHERIFF_ATTACKED_HERO_LOST2") == 0 && s.NextLine == 3 && g.enflameWaits == 1;
    std::memset(&g, 0, sizeof(g));
    g.hitAny = true; g.hitSpecial14 = true; g.health = 1.0f;
    s.DoRemindingHeroToKillAssassin();
    ok = ok && g.speakCount == 0 && g.enflameWaits == 0;
    std::memset(&g, 0, sizeof(g));
    g.talked = true; g.hit = true; g.health = 1.0f;
    s.AssassinState = 2;
    s.DoRemindingHeroToKillAssassin();
    ok = ok && g.speakCount == 0;
    if (!ok) {
        std::printf("00eb0830_TEST FAIL\n");
        return 1;
    }
    std::printf("00eb0830_TEST PASS\n");
    return 0;
}