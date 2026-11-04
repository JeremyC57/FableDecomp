// Behaviour test for 0x00EAF810 NScript::CV_MurderTwistFinalScript::CMurderTwistSheriff::DoAskingHeroToKillAssassin:
//   attract line when the timer expired and the hero is near and sees the sheriff; talk -> intro (or fickle) + yes/no question,
//   yes -> SheriffState 1, quest card + objective, TaskDeclined false; no -> TaskDeclined true; hit -> prefix + NextLine.
#include <cstdio>
#include <cstring>

struct Mock {
    bool talked, hit, hitAny, hitSpecial14, terminating, nearAndSeen, assassinTrusts;
    long timer, answer;
    int answerDelay, pauseDepth, speakCount, framesLeft, questCards, objectives, attractLines;
    float health;
    char lines[8][96];
};
static Mock g;

struct Sheriff {
    bool TaskDeclined;
    int SheriffState;
    unsigned long NextLine;
    struct Paused { Paused() { ++g.pauseDepth; } ~Paused() { --g.pauseDepth; } };
    bool Say(const char* text)
    {
        if (g.health > 0.0f) {
            std::strcpy(g.lines[g.speakCount++ & 7], text); g.framesLeft = 1;
            while (g.framesLeft > 0) { --g.framesLeft; if (g.terminating) return false; }
            if (g.terminating) return false;
        }
        return true;
    }
    void DoAskingHeroToKillAssassin()
    {
        if (!TaskDeclined && g.timer == 0 && g.nearAndSeen) {
            if (g.terminating) return;
            g.timer = 10; ++g.attractLines;
        }
        if (g.talked) {
            if (g.terminating) return;
            bool distrusts = !g.assassinTrusts;
            Paused paused;
            if (!TaskDeclined) {
                if (g.terminating) return;
                if (!Say("TEXT_QST_B07_SHERIFF_HERO_INTRO_TITLE")) return;
                if (!Say("TEXT_QST_B07_SHERIFF_HERO_INTRO_AFTERTITLE")) return;
                if (!Say(distrusts ? "TEXT_QST_B07_SHERIFF_INTRO" : "TEXT_QST_B07_SHERIFF_INTRO_SUSS")) return;
            } else {
                if (g.terminating) return;
                if (!Say("TEXT_QST_B07_SHERIFF_HERO_FICKLE")) return;
            }
            long answer;
            while ((answer = (g.answerDelay-- > 0) ? -1 : g.answer) < 0) { if (g.terminating) return; }
            if (g.terminating) return;
            if (answer == 1) {
                if (!Say(distrusts ? "TEXT_QST_B07_SHERIFF_HERO_ACCEPTED" : "TEXT_QST_B07_SHERIFF_HERO_ACCEPTED_SUSS")) return;
                SheriffState = 1; ++g.questCards; ++g.objectives;
                TaskDeclined = false;
            } else {
                if (!Say(distrusts ? "TEXT_QST_B07_SHERIFF_HERO_DECLINED" : "TEXT_QST_B07_SHERIFF_HERO_DECLINED_SUSS")) return;
                TaskDeclined = true;
            }
        }
        if (g.hit || (g.hitAny && !g.hitSpecial14)) {
            if (g.terminating) return;
            const char* prefix = TaskDeclined ? "TEXT_QST_B07_SHERIFF_ATTACKED" : "TEXT_QST_B07_SHERIFF_ATTACKED_BEFORE_INTRO";
            Paused paused;
            if (NextLine > 2) NextLine = 1;
            char text[96]; std::sprintf(text, "%s%lu", prefix, NextLine);
            if (!Say(text)) return;
            NextLine++;
        }
    }
};

int main()
{
    Sheriff s; s.TaskDeclined = false; s.SheriffState = 0; s.NextLine = 7;
    std::memset(&g, 0, sizeof(g));
    g.nearAndSeen = true; g.talked = true; g.health = 1.0f; g.answer = 0; g.answerDelay = 3;
    s.DoAskingHeroToKillAssassin();
    bool ok = g.attractLines == 1 && g.timer == 10 && s.TaskDeclined && g.speakCount == 4
        && std::strcmp(g.lines[3], "TEXT_QST_B07_SHERIFF_HERO_DECLINED") == 0 && g.pauseDepth == 0;
    std::memset(&g, 0, sizeof(g));
    g.talked = true; g.health = 1.0f; g.answer = 1; g.assassinTrusts = true;
    s.DoAskingHeroToKillAssassin();
    ok = ok && g.attractLines == 0 && std::strcmp(g.lines[0], "TEXT_QST_B07_SHERIFF_HERO_FICKLE") == 0
        && std::strcmp(g.lines[1], "TEXT_QST_B07_SHERIFF_HERO_ACCEPTED_SUSS") == 0 && s.SheriffState == 1 && !s.TaskDeclined
        && g.questCards == 1 && g.objectives == 1;
    std::memset(&g, 0, sizeof(g));
    g.hit = true; g.health = 1.0f;
    s.DoAskingHeroToKillAssassin();
    ok = ok && std::strcmp(g.lines[0], "TEXT_QST_B07_SHERIFF_ATTACKED_BEFORE_INTRO1") == 0 && s.NextLine == 2 && g.pauseDepth == 0;
    std::memset(&g, 0, sizeof(g));
    g.talked = true; g.terminating = true; g.health = 1.0f;
    s.DoAskingHeroToKillAssassin();
    ok = ok && g.speakCount == 0 && s.SheriffState == 1;
    if (!ok) { std::printf("00eaf810_TEST FAIL\n"); return 1; }
    std::printf("00eaf810_TEST PASS\n");
    return 0;
}
