#include <cstdio>
#include <cstring>
// behaviour test: CQ_HangingTreeGoodScript::CleanUp -- remove all freedom fighters and audience members, then the
// prisoner and executioner when alive; any terminate check that fires stops the clean-up
static int g_checks, g_endAt, g_removed[16], g_nRemoved;
class CScriptThing { public: int id; bool alive; bool IsAlive() const { return id != 0 && alive; } };
struct CThingList { CScriptThing items[8]; unsigned int n; CThingList() : n(0) {} unsigned int size() const { return n; } CScriptThing& operator[](unsigned int i) { return items[i]; } };
class CGameScriptInterface { public:
    CScriptThing GetThingWithScriptName(const char* name) const
    {
        CScriptThing t; t.id = 0; t.alive = false;
        if (!std::strcmp(name, "HTG_Prisoner")) { t.id = 1; t.alive = true; }
        if (!std::strcmp(name, "HTG_Executioner")) { t.id = 2; t.alive = false; }
        return t;
    }
    long GetAllThingsWithScriptName(const char* name, CThingList& out) const
    {
        int base = !std::strcmp(name, "HTG_FreedomFighter") ? 10 : !std::strcmp(name, "HTG_Audience") ? 20 : 0;
        int count = base == 10 ? 3 : base == 20 ? 2 : 0;
        for (int i = 0; i < count; ++i) { out.items[out.n].id = base + i; out.items[out.n].alive = true; ++out.n; }
        return count;
    }
    void RemoveThing(const CScriptThing& t, bool a, bool b) const { if (!a && b) g_removed[g_nRemoved++] = t.id; }
};
struct CQ_HangingTreeGoodScript {
    CGameScriptInterface* Game;
    bool IsActiveThreadTerminating() { return ++g_checks > g_endAt; }
    void CleanUp();
};
void CQ_HangingTreeGoodScript::CleanUp()
{
    CThingList freedomFighters;
    CThingList audience;
    CScriptThing prisoner = Game->GetThingWithScriptName("HTG_Prisoner");
    CScriptThing executioner = Game->GetThingWithScriptName("HTG_Executioner");
    Game->GetAllThingsWithScriptName("HTG_FreedomFighter", freedomFighters);
    Game->GetAllThingsWithScriptName("HTG_Audience", audience);
    for (unsigned int i = 0; i < freedomFighters.size(); ++i) { if (IsActiveThreadTerminating()) return; Game->RemoveThing(freedomFighters[i], false, true); }
    if (IsActiveThreadTerminating()) return;
    for (unsigned int j = 0; j < audience.size(); ++j) { if (IsActiveThreadTerminating()) return; Game->RemoveThing(audience[j], false, true); }
    if (IsActiveThreadTerminating()) return;
    if (prisoner.IsAlive()) Game->RemoveThing(prisoner, false, true);
    if (executioner.IsAlive()) Game->RemoveThing(executioner, false, true);
}
int main()
{
    CGameScriptInterface game;
    CQ_HangingTreeGoodScript s; s.Game = &game;
    g_checks = 0; g_endAt = 100; g_nRemoved = 0;
    s.CleanUp();
    bool ok = g_nRemoved == 6 && g_removed[0] == 10 && g_removed[2] == 12 && g_removed[3] == 20 && g_removed[4] == 21 && g_removed[5] == 1;
    g_checks = 0; g_endAt = 4; g_nRemoved = 0;          // ends at the check after the fighters loop
    s.CleanUp();
    ok = ok && g_nRemoved == 3 && g_removed[2] == 12;
    if (!ok) { std::printf("00d72b70_TEST FAIL\n"); return 1; }
    std::printf("00d72b70_TEST PASS\n");
    return 0;
}