// Behaviour test for NScript::CHeroBoastsScript::Main @ 0x00CE1AF0 (copy of the model with a stub interface and thread list).
#include <cstdio>
#include <cstring>
static int g_frame, g_doneAt, g_spawned; static long g_lastBoast; static const char* g_lastQuest;
struct Boast { long first; const char* second; };
struct BoastVector { Boast items[4]; unsigned int count; unsigned int size() const { return count; } Boast& operator[](unsigned int i) { return items[i]; } };
struct CGameScriptInterface {
    void NewScriptFrame() const { ++g_frame; }
    void MsgOnBoastsMade(BoastVector& v) const
    {
        v.count = 0;
        if (g_frame == 1) { v.items[0].first = 8; v.items[0].second = "QuestA"; v.items[1].first = 30; v.items[1].second = "QuestB"; v.count = 2; }
    }
};
struct CSpawnedFunc { const char* name; long a1; const char* a2; };
struct CHeroBoastsScript {
    CGameScriptInterface* GSI;
    bool IsActiveThreadTerminating() { return g_frame >= g_doneAt; }
    void AddSpawnedFunction(CSpawnedFunc* f, const char* region)
    { if (!std::strcmp(f->name, "ControlNewBoast") && !*region) { ++g_spawned; g_lastBoast = f->a1; g_lastQuest = f->a2; } delete f; }
    void Main()
    {
        while (!IsActiveThreadTerminating()) {
            {
                BoastVector boasts;
                GSI->MsgOnBoastsMade(boasts);
                for (unsigned int i = 0; i != boasts.size(); ++i) {
                    if (IsActiveThreadTerminating()) return;
                    CSpawnedFunc* f = new CSpawnedFunc; f->name = "ControlNewBoast"; f->a1 = boasts[i].first; f->a2 = boasts[i].second;
                    AddSpawnedFunction(f, "");
                }
                if (IsActiveThreadTerminating()) return;
            }
            GSI->NewScriptFrame();
        }
    }
};
int main()
{
    int bad = 0; CGameScriptInterface gsi; CHeroBoastsScript s; s.GSI = &gsi;
    g_frame = 0; g_doneAt = 5; g_spawned = 0; s.Main();
    if (g_spawned != 2 || g_lastBoast != 30 || std::strcmp(g_lastQuest, "QuestB") || g_frame != 5) bad++;
    g_frame = 0; g_doneAt = 0; g_spawned = 0; s.Main(); if (g_spawned || g_frame) bad++;
    if (bad) { std::printf("00ce1af0_TEST FAIL\n"); return 1; }
    std::printf("00ce1af0_TEST PASS\n");
    return 0;
}
