// Behaviour test for CQ_WhiteBalverineWWScript::MonitorBalverine (0x00E18EB0): health bar shown, frames run until the
// balverine is killed, then the objective / alive flag / pause / music / trophy / generators follow in order.
#include <cstdio>
#include <cstring>
class CCharString { public:
    char Text[64];
    CCharString(const char* text) { std::strncpy(Text, text, 63); Text[63] = 0; }
};
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
static int g_frames = 0;
class CScriptThing { public:
    int killedAt;
    bool MsgIsKilledBy(const CCharString& killer) const { return killer.Text[0] == 0 && g_frames >= killedAt; }
};
class CGameScriptInterface { public:
    char log[512];
    void Add(const char* s) { std::strcat(log, s); std::strcat(log, ";"); }
    void NewScriptFrame() { ++g_frames; }
    void GiveHeroObject(const CCharString& o, long n, bool m) { if (n == -1 && !m) Add(o.Text); }
    void SetQuestCardObjective(const CCharString& q, const CCharString& o, const CCharString& r1, const CCharString& r2) {
        Add(q.Text); Add(o.Text); Add(r1.Text); Add(r2.Text);
    }
    void DisplayQuestInfo(bool d) { if (d) Add("display"); }
    long AddQuestInfoBarHealth(const CScriptThing&, const CRGBColour& c, const CCharString& icon, float scale) {
        if (c.R == 255 && c.G == 255 && c.B == 255 && c.A == 255 && scale == 1.0f) Add(icon.Text); return 3;
    }
    void Pause(float s) { if (s == 0.5f) Add("pause"); }
    void SetCreatureGeneratorsEnabled(const CCharString& r, bool e) { if (e) Add(r.Text); }
    void StopOverrideMusic(bool i) { if (!i) Add("music"); }
};
struct CScript {
    CGameScriptInterface* Game; bool WhiteBalverineAlive; CScriptThing WhiteBalverine; int terminateAt;
    bool IsActiveThreadTerminating() { return g_frames >= terminateAt; }
    void MonitorBalverine();
};
void CScript::MonitorBalverine()
{
    Game->AddQuestInfoBarHealth(WhiteBalverine, CRGBColour(255, 255, 255, 255), "HUD_QUEST_ICON_WHITE_BALVERINE", 1.0f);
    Game->DisplayQuestInfo(true);
    while (!WhiteBalverine.MsgIsKilledBy("")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetQuestCardObjective("Q_WhiteBalverineKnotholeGlade", "TEXT_QUEST_WHITE_BALVERINE_KNOTHOLE_GLADE_OBJECTIVE_08",
                                "KnotholeGlade", "KnotholeGlade");
    WhiteBalverineAlive = false;
    Game->Pause(0.5f);
    Game->StopOverrideMusic(false);
    Game->GiveHeroObject("OBJECT_TROPHY_BALVERINE_FM_HEAD_01", -1, false);
    Game->SetCreatureGeneratorsEnabled("Witchwood4", true);
}
int main()
{
    CGameScriptInterface g; g.log[0] = 0;
    CScript s; s.Game = &g; s.WhiteBalverineAlive = true; s.WhiteBalverine.killedAt = 4; s.terminateAt = 100;
    s.MonitorBalverine();
    bool ok = g_frames == 4 && !s.WhiteBalverineAlive && std::strcmp(g.log, "HUD_QUEST_ICON_WHITE_BALVERINE;display;"
        "Q_WhiteBalverineKnotholeGlade;TEXT_QUEST_WHITE_BALVERINE_KNOTHOLE_GLADE_OBJECTIVE_08;KnotholeGlade;KnotholeGlade;"
        "pause;music;OBJECT_TROPHY_BALVERINE_FM_HEAD_01;Witchwood4;") == 0;
    g_frames = 0; g.log[0] = 0;
    CScript t; t.Game = &g; t.WhiteBalverineAlive = true; t.WhiteBalverine.killedAt = 50; t.terminateAt = 2;
    t.MonitorBalverine();
    ok = ok && g_frames == 2 && t.WhiteBalverineAlive && std::strcmp(g.log, "HUD_QUEST_ICON_WHITE_BALVERINE;display;") == 0;
    if (!ok) { std::printf("00e18eb0_TEST FAIL\n"); return 1; }
    std::printf("00e18eb0_TEST PASS\n");
    return 0;
}