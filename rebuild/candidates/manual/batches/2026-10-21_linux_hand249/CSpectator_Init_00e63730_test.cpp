#include <cstdio>
#include <cstring>
// behaviour test: CSpectator::Init picks 1 when near Spectator1, 2 when near Spectator2, else 3, unless the script is done
struct CScriptThing { const char* name; };
struct CCharString { const char* text; CCharString(const char* t, long) : text(t) {} };
static const char* g_near = 0;
static bool g_done = false;
static int g_lookups = 0;
static bool IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance)
{ return distance == 1.0f && g_near && std::strcmp(b.name, g_near) == 0; }
struct CGameScriptInterface {
    CScriptThing GetThingWithScriptName(const CCharString& name) const { ++g_lookups; CScriptThing t; t.name = name.text; return t; }
};
static const char g_spectator1[] = "Spectator1";
static const char g_spectator2[] = "Spectator2";
struct CSpectator {
    CGameScriptInterface* Game;
    CScriptThing Me;
    long SpectatorNumber;
    bool IsDone() { return g_done; }
    void Init();
};
void CSpectator::Init()
{
    bool nearFirst;
    {
        CCharString name(g_spectator1, -1);
        nearFirst = IsDistanceBetweenThingsUnder(Me, Game->GetThingWithScriptName(name), 1.0f);
    }
    if (nearFirst) {
        if (IsDone())
            return;
        SpectatorNumber = 1;
        return;
    }
    bool nearSecond;
    {
        CCharString name(g_spectator2, -1);
        nearSecond = IsDistanceBetweenThingsUnder(Me, Game->GetThingWithScriptName(name), 1.0f);
    }
    if (nearSecond) {
        if (IsDone())
            return;
        SpectatorNumber = 2;
        return;
    }
    if (IsDone())
        return;
    SpectatorNumber = 3;
}

int main()
{
    CGameScriptInterface game; CSpectator s; s.Game = &game; s.Me.name = "me";
    bool ok = true;
    g_near = "Spectator1"; s.SpectatorNumber = 0; s.Init(); ok = ok && s.SpectatorNumber == 1 && g_lookups == 1;
    g_near = "Spectator2"; s.SpectatorNumber = 0; s.Init(); ok = ok && s.SpectatorNumber == 2 && g_lookups == 3;
    g_near = 0; s.SpectatorNumber = 0; s.Init(); ok = ok && s.SpectatorNumber == 3;
    g_done = true;
    g_near = "Spectator1"; s.SpectatorNumber = 0; s.Init(); ok = ok && s.SpectatorNumber == 0;
    g_near = "Spectator2"; s.Init(); ok = ok && s.SpectatorNumber == 0;
    g_near = 0; s.Init(); ok = ok && s.SpectatorNumber == 0;
    if (!ok) { std::printf("00e63730_TEST FAIL\n"); return 1; }
    std::printf("00e63730_TEST PASS\n");
    return 0;
}
