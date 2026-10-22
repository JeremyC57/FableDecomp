#include <cstdio>
#include <cstring>
static char g_log[8192];
static int g_frame = 0;
static int g_termAt = 1000;
static void Log(const char* s) { char b[32]; std::sprintf(b, "@%d;", g_frame); std::strcat(g_log, s); std::strcat(g_log, b); }
static void Reset(int termAt) { g_log[0] = 0; g_frame = 0; g_termAt = termAt; }
static int Fail(const char* addr, const char* what) { std::printf("%s_TEST FAIL %s [%s]\n", addr, what, g_log); return 1; }
#define CHECK(addr, cond) do { if (!(cond)) return Fail(addr, #cond); } while (0)
// behaviour test: CQ_EndGameScript::CheckRegionLeaving -- per Darkwood5 visit: start screen once, undead bar shown while visiting, removed on leaving
static int g_loadFrom = 2, g_loadTo = 5;
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct CGSI {
    void NewScriptFrame() const { ++g_frame; }
    bool IsRegionLoaded(const char*) const { int f = g_frame % 10; return f >= g_loadFrom && f < g_loadTo; }
    void KickOffQuestStartScreen(const char* q, bool, bool) const { Log(q); }
    const char* GetActiveQuestName() const { return "Q_EndGame"; }
    void DisplayQuestInfo(bool) const { Log("display"); }
    long AddQuestInfoBar(float maxV, float v, const CRGBColour& c, const CRGBColour& b, const char* icon, const char*, float) const
    { char x[96]; std::sprintf(x, "bar %g %g %d%d %s", maxV, v, (int)c.G, (int)b.R, icon); Log(x); return 42; }
    void RemoveQuestInfoElement(long id) const { char x[32]; std::sprintf(x, "remove %ld", id); Log(x); }
    void SetCreatureGeneratorsEnabled(const char* n, bool e) const { (void)n; (void)e; }
};
struct CScriptDef { float UndeadBarMaximum; };
static CScriptDef g_def;
static CScriptDef* g_pScriptDef = &g_def;
struct CQ_EndGameScript {
    CGSI* GSI; float UndeadBar; long GuiCounter; bool QuestStartScreened; bool ShowUndeadBar;
    bool IsActiveThreadTerminating() { return g_frame >= g_termAt; }
    void CheckRegionLeaving();
};
void CQ_EndGameScript::CheckRegionLeaving()
{
    while (!IsActiveThreadTerminating()) {
        GSI->SetCreatureGeneratorsEnabled("Darkwood5", false);
        while (!GSI->IsRegionLoaded("Darkwood5")) {
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (!QuestStartScreened) {
            if (IsActiveThreadTerminating())
                return;
            GSI->KickOffQuestStartScreen(GSI->GetActiveQuestName(), true, false);
            QuestStartScreened = true;
        }
        while (!ShowUndeadBar) {
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (UndeadBar < g_pScriptDef->UndeadBarMaximum) {
            if (IsActiveThreadTerminating())
                return;
            GSI->DisplayQuestInfo(true);
            GuiCounter = GSI->AddQuestInfoBar(g_pScriptDef->UndeadBarMaximum, 0.0f, CRGBColour(0, 255, 0, 255), CRGBColour(255, 0, 0, 255), "HUD_QUEST_ICON_UNDEAD", "", 1.0f);
        }
        while (GSI->IsRegionLoaded("Darkwood5") ? true : false) {
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        GSI->RemoveQuestInfoElement(GuiCounter);
        GSI->NewScriptFrame();
    }
}
int main()
{
    CGSI gsi; CQ_EndGameScript q; q.GSI = &gsi; q.UndeadBar = 1.0f; q.GuiCounter = 0; q.QuestStartScreened = false; q.ShowUndeadBar = true;
    g_def.UndeadBarMaximum = 10.0f;
    Reset(16);
    q.CheckRegionLeaving();
    CHECK("00d27950", std::strcmp(g_log, "Q_EndGame@2;display@2;bar 10 0 255255 HUD_QUEST_ICON_UNDEAD@2;remove 42@5;display@12;bar 10 0 255255 HUD_QUEST_ICON_UNDEAD@12;remove 42@15;") == 0 && q.QuestStartScreened);
    Reset(20); q.UndeadBar = 20.0f; q.GuiCounter = 7;
    q.CheckRegionLeaving();
    CHECK("00d27950", std::strcmp(g_log, "remove 7@5;remove 7@15;") == 0);
    std::printf("00d27950_TEST PASS\n");
    return 0;
}
