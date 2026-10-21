// Behaviour test for CCombatManager::RegisterCombatant (0x006EDC50): the trace text is built from both def names and
// the frame, then the attacked and attacking registrations run and the attacked record's attacker list is refreshed.
#include <cstdio>
#include <cstring>

struct CCharString { char Buf[96]; CCharString(const char* s = "") { std::strcpy(Buf, s); } };
static CCharString operator+(const CCharString& a, const char* b) { CCharString r(a.Buf); std::strcat(r.Buf, b); return r; }
static CCharString operator+(const CCharString& a, const CCharString& b) { return a + b.Buf; }
static int g_frame = 42;
static CCharString GetWorldFrameString() { CCharString r; std::sprintf(r.Buf, "%d", g_frame); return r; }
static char g_lastTrace[96];
static void CombatTrace(const CCharString& s) { std::strcpy(g_lastTrace, s.Buf); }
struct CThingCreatureBase { const char* DefName; int UID; };
static CCharString GetDefNameString(const CThingCreatureBase& t) { CCharString name(t.DefName); return name; }
struct CCombatantInfo { int Refreshes; bool Forced; };
struct CCombatManager {
    int Calls[2]; CCombatantInfo Info;
    CCombatantInfo& GetCombatantInfo(const CThingCreatureBase&) { return Info; }
    void RegisterAttackedCombatant(CThingCreatureBase&, CThingCreatureBase&) { Calls[0] = Calls[1] + 1; }
    void RegisterAttackingCombatant(CThingCreatureBase&, CThingCreatureBase&, bool flag) { Calls[1] = Calls[0] + (flag ? 10 : 20); }
    void UpdateAttackingCombatantsList(CCombatantInfo& info, bool force) { ++info.Refreshes; info.Forced = force; }
    void RegisterCombatant(CThingCreatureBase& attacker, CThingCreatureBase& attacked, bool flag)
    {
        CCharString attackerName = GetDefNameString(attacker);
        CCharString attackedName = GetDefNameString(attacked);
        CombatTrace(attackerName + " attacking " + attackedName + " on frame " + GetWorldFrameString());
        RegisterAttackedCombatant(attacker, attacked);
        RegisterAttackingCombatant(attacker, attacked, flag);
        UpdateAttackingCombatantsList(GetCombatantInfo(attacked), true);
    }
};

int main()
{
    CThingCreatureBase hero = { "HERO", 1 }, troll = { "CREATURE_TROLL", 2 };
    CCombatManager m; m.Calls[0] = m.Calls[1] = 0; m.Info.Refreshes = 0; m.Info.Forced = false;
    m.RegisterCombatant(troll, hero, true);
    bool ok = std::strcmp(g_lastTrace, "CREATURE_TROLL attacking HERO on frame 42") == 0
        && m.Calls[0] == 1 && m.Calls[1] == 11 && m.Info.Refreshes == 1 && m.Info.Forced;
    if (!ok) { std::printf("006edc50_TEST FAIL\n"); return 1; }
    std::printf("006edc50_TEST PASS\n");
    return 0;
}
