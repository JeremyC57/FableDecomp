// behaviour test for CGameflowScript::CheckBarrowFieldsGuards (0x00CEF550): scripted world; guards enabled only on the evil path
#include <cstdio>
#include <cstring>
struct CCharString { const char* Text; CCharString(const char* t) : Text(t) {} };
struct CScriptThing { const char* Name; };
struct World {
    int frame; bool evilDone, goodDone; int evilActiveAt, goodActiveAt, evilCompleteAt, unloadAt, reloadAt; int terminateAt;
    const char* enabled; bool enableFlag;
};
static World w;
struct CGameScriptInterface {
    void NewScriptFrame() const { ++w.frame; }
    bool IsRegionLoaded(const CCharString& n) const { return std::strcmp(n.Text, "BarrowFields") == 0 && !(w.frame >= w.unloadAt && w.frame < w.reloadAt); }
    bool MsgOnQuestCompleted(const CCharString& n) const { return std::strcmp(n.Text, "Q_TraderConflictEvil") == 0 && w.frame >= w.evilCompleteAt; }
    CScriptThing GetThingWithScriptName(const CCharString& n) const { CScriptThing t; t.Name = n.Text; return t; }
    bool IsQuestActive(const CCharString& n) const
    {
        if (std::strcmp(n.Text, "Q_TraderConflictEvil") == 0) return w.evilActiveAt >= 0 && w.frame >= w.evilActiveAt;
        return w.goodActiveAt >= 0 && w.frame >= w.goodActiveAt;
    }
    bool IsQuestCompleted(const CCharString& n) const { return std::strcmp(n.Text, "Q_TraderConflictEvil") == 0 ? w.evilDone : w.goodDone; }
    void EnableGuards(const CScriptThing& t, bool on) const { w.enabled = t.Name; w.enableFlag = on; }
};
struct CGameflowScript {
    CGameScriptInterface* GSI;
    bool IsActiveThreadTerminating() { return w.terminateAt >= 0 && w.frame >= w.terminateAt; }
    void CheckBarrowFieldsGuards()
    {
        if (!GSI->IsQuestCompleted("Q_TraderConflictEvil") && !GSI->IsQuestCompleted("Q_TraderConflictGood")) {
            if (IsActiveThreadTerminating()) return;
            while (!GSI->IsQuestActive("Q_TraderConflictEvil") && !GSI->IsQuestActive("Q_TraderConflictGood")) {
                GSI->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            if (GSI->IsQuestActive("Q_TraderConflictEvil")) {
                if (IsActiveThreadTerminating()) return;
                while (!GSI->MsgOnQuestCompleted("Q_TraderConflictEvil")) { GSI->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
                if (IsActiveThreadTerminating()) return;
                while (GSI->IsRegionLoaded("BarrowFields") != false) { GSI->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
                if (IsActiveThreadTerminating()) return;
                while (!GSI->IsRegionLoaded("BarrowFields")) { GSI->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
                if (IsActiveThreadTerminating()) return;
                GSI->EnableGuards(GSI->GetThingWithScriptName("VILLAGE_BARROWFIELDS"), true);
            }
        }
    }
};
static void Reset() { std::memset(&w, 0, sizeof(w)); w.evilActiveAt = w.goodActiveAt = -1; w.terminateAt = -1; w.enabled = 0; }
int main()
{
    CGameScriptInterface gsi; CGameflowScript s; s.GSI = &gsi;
    Reset(); w.evilActiveAt = 3; w.evilCompleteAt = 7; w.unloadAt = 7; w.reloadAt = 12; s.CheckBarrowFieldsGuards();
    if (!w.enabled || std::strcmp(w.enabled, "VILLAGE_BARROWFIELDS") != 0 || !w.enableFlag || w.frame != 12) { std::printf("00cef550_TEST FAIL\n"); return 1; }
    Reset(); w.goodActiveAt = 2; s.CheckBarrowFieldsGuards();
    if (w.enabled || w.frame != 2) { std::printf("00cef550_TEST FAIL\n"); return 1; }
    Reset(); w.goodDone = true; s.CheckBarrowFieldsGuards();
    if (w.enabled || w.frame != 0) { std::printf("00cef550_TEST FAIL\n"); return 1; }
    Reset(); w.terminateAt = 4; s.CheckBarrowFieldsGuards();
    if (w.enabled || w.frame != 4) { std::printf("00cef550_TEST FAIL\n"); return 1; }
    std::printf("00cef550_TEST PASS\n");
    return 0;
}
