// Behaviour test for CV_ArcheryCompetitionScript::AddArcheryQuestInfo (00e33040).
#include <cstdio>
#include <cstring>
class CCharString { public: char t[32]; CCharString(const char* s, long = -1) { std::strcpy(t, s); } };
class CTimer { public: long h; };
struct FakeGSI {
    long next; long upId, upCount, upTarget; float online; bool shown; char icons[3][32]; const CTimer* timer;
    long AddQuestInfoCounter(const CCharString& icon, long c, float s) { if (c != 0 || s != 1.0f) return -99; std::strcpy(icons[next - 1], icon.t); return next++; }
    long AddQuestInfoTimer(const CTimer& tm, const CCharString& icon, float s) { if (s != 1.0f) return -99; timer = &tm; std::strcpy(icons[next - 1], icon.t); return next++; }
    void UpdateQuestInfoCounter(long i, long c, long t) { upId = i; upCount = c; upTarget = t; }
    void UpdateOnlineScore_Archery(float f) { online = f; }
    void DisplayQuestInfo(bool d) { shown = d; }
};
struct Owner { long m_highScore; };
struct Script {
    FakeGSI* GSI; Owner* O; long HighID, GameID, TimerID; CTimer GameTimeLeft;
    void AddArcheryQuestInfo()
    {
        HighID = GSI->AddQuestInfoCounter("HUD_ICON_MULTI_ARROW", 0, 1.0f);
        GameID = GSI->AddQuestInfoCounter("HUD_ICON_ARROW", 0, 1.0f);
        TimerID = GSI->AddQuestInfoTimer(GameTimeLeft, "HUD_CLOCK_ICON", 1.0f);
        long highScore = O->m_highScore;
        GSI->UpdateQuestInfoCounter(HighID, highScore, -1);
        GSI->UpdateOnlineScore_Archery((float)highScore);
        GSI->DisplayQuestInfo(true);
    }
};
int main()
{
    FakeGSI g; std::memset(&g, 0, sizeof(g)); g.next = 1;
    Owner o = { 42 };
    Script s; s.GSI = &g; s.O = &o; s.GameTimeLeft.h = 3;
    s.AddArcheryQuestInfo();
    bool ok = s.HighID == 1 && s.GameID == 2 && s.TimerID == 3 && g.timer == &s.GameTimeLeft && g.upId == 1 && g.upCount == 42
        && g.upTarget == -1 && g.online == 42.0f && g.shown && !std::strcmp(g.icons[0], "HUD_ICON_MULTI_ARROW")
        && !std::strcmp(g.icons[1], "HUD_ICON_ARROW") && !std::strcmp(g.icons[2], "HUD_CLOCK_ICON");
    if (!ok) { std::printf("00e33040_TEST FAIL\n"); return 1; }
    std::printf("00e33040_TEST PASS\n");
    return 0;
}