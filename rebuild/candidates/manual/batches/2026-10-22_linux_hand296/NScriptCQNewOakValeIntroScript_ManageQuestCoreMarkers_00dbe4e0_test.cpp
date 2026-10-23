#include <cstdio>
#include <cstring>
// Behaviour test for CQ_NewOakValeIntroScript::ManageQuestCoreMarkers (0x00DBE4E0): the quest-core marker moves
// father -> book trader -> Theresa -> father as the gold / control / tutorial / sweets / chocolates conditions are met.
static char g_log[1024];
static void Log(const char* a, const char* b) { std::strcat(g_log, a); std::strcat(g_log, b); std::strcat(g_log, ";"); }
struct CScriptThing { char Name[32]; };
struct CQ_NewOakValeIntroScript;
struct CGSI {
    int Frames; CQ_NewOakValeIntroScript* Script; bool Tutorial;
    void NewScriptFrame();
    bool MsgIsTutorialClickedPast() { return Frames >= 6; }
    CScriptThing GetThingWithScriptName(const char* n) { CScriptThing t; std::strcpy(t.Name, n); return t; }
    bool DisplayTutorial(int c) { return c == 0x13 && Tutorial; }
    long GetHeroGold() { return Frames >= 2 ? 3 : 0; }
    void MiniMapAddMarker(const CScriptThing& t, const char* icon) { if (std::strcmp(icon, "HUD_ORB_QUEST_CORE") == 0) Log("+", t.Name); }
    void MiniMapRemoveMarker(const CScriptThing& t) { Log("-", t.Name); }
    bool IsHeroControlledByPlayer() { return Frames >= 4; }
};
struct CQ_NewOakValeIntroScript {
    CGSI* GSI; bool GivenSweets; bool GivenTheresaChocs;
    bool IsActiveThreadTerminating() { return GSI->Frames > 100; }
    void ManageQuestCoreMarkers()
    {
        CScriptThing father = GSI->GetThingWithScriptName("NOVI_LiveFather");
        CScriptThing bookTrader = GSI->GetThingWithScriptName("NOVI_BookTrader");
        CScriptThing theresa = GSI->GetThingWithScriptName("NOVI_Theresa");
        GSI->MiniMapAddMarker(father, "HUD_ORB_QUEST_CORE");
        while (GSI->GetHeroGold() <= 2) { GSI->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        while (!GSI->IsHeroControlledByPlayer()) { GSI->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        GSI->MiniMapRemoveMarker(father);
        GSI->MiniMapAddMarker(bookTrader, "HUD_ORB_QUEST_CORE");
        if (GSI->DisplayTutorial(0x13)) {
            if (IsActiveThreadTerminating()) return;
            while (!GSI->MsgIsTutorialClickedPast()) { GSI->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
            if (IsActiveThreadTerminating()) return;
        }
        while (!GivenSweets) { GSI->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        GSI->MiniMapRemoveMarker(bookTrader);
        GSI->MiniMapAddMarker(theresa, "HUD_ORB_QUEST_CORE");
        while (!GivenTheresaChocs) { GSI->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        GSI->MiniMapRemoveMarker(theresa);
        GSI->MiniMapAddMarker(father, "HUD_ORB_QUEST_CORE");
    }
};
void CGSI::NewScriptFrame() { ++Frames; if (Frames == 8) Script->GivenSweets = true; if (Frames == 10) Script->GivenTheresaChocs = true; }
int main()
{
    CQ_NewOakValeIntroScript s; CGSI gsi; gsi.Frames = 0; gsi.Script = &s; gsi.Tutorial = true;
    s.GSI = &gsi; s.GivenSweets = false; s.GivenTheresaChocs = false;
    g_log[0] = 0; s.ManageQuestCoreMarkers();
    bool ok = gsi.Frames == 10 && std::strcmp(g_log, "+NOVI_LiveFather;-NOVI_LiveFather;+NOVI_BookTrader;-NOVI_BookTrader;+NOVI_Theresa;-NOVI_Theresa;+NOVI_LiveFather;") == 0;
    if (!ok) { std::printf("00dbe4e0_TEST FAIL\n"); return 1; }
    std::printf("00dbe4e0_TEST PASS\n");
    return 0;
}
