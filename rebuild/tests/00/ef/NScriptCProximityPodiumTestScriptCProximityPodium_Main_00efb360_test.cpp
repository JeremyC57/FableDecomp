// Behaviour test for CProximityPodium::Main (0x00EFB360): the alive predicate over Me is registered (and destroyed),
// then every time the hero is within 2 units Me's activation trigger is switched on, the script pauses 10 seconds and
// switches it off, until the thread is done.
#include <cstdio>
#include <cstring>

static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static const char* g_near;   // per distance test
static const char* g_done;   // per IsActiveThreadTerminating
class CScriptThing { public:
    int id;
    CScriptThing() : id(0) {}
    CScriptThing(const CScriptThing& o) : id(o.id) {}
    virtual ~CScriptThing() {}
    virtual void SetActivationTriggerStatus(bool on) { Log(on ? "on" : "off"); }
};
namespace N_00ef7ab0 { struct CIsThingAlive {
    CScriptThing m_thing;
    CIsThingAlive(const CScriptThing& t) : m_thing(t) { Log("pred"); }
    ~CIsThingAlive() { Log("~pred"); }
}; } using N_00ef7ab0::CIsThingAlive;
static CScriptThing g_hero;
static bool IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float d)
{ char c = *g_near; if (c) ++g_near; return c == '1' && a.id == 5 && &b == &g_hero && d == 2.0f; }
class CGameScriptInterface { public:
    virtual void NewScriptFrame() { Log("frame"); }
    virtual CScriptThing& GetHero() { return g_hero; }
    virtual void Pause(float s) { Log(s == 10.0f ? "pause10" : "pause?"); }
};
class CProximityPodium {
public:
    CGameScriptInterface* Game;
    CScriptThing Me;
    void SetPredicate(CIsThingAlive& p) { Log(p.m_thing.id == 5 ? "set" : "set?"); }
    bool IsActiveThreadTerminating() { char c = *g_done; if (c) ++g_done; return c != '0'; }
    void Main();
};
void CProximityPodium::Main()
{
    {
        CIsThingAlive predicate(Me);
        SetPredicate(predicate);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!IsActiveThreadTerminating()) {
        while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 2.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Me.SetActivationTriggerStatus(true);
        Game->Pause(10.0f);
        Me.SetActivationTriggerStatus(false);
        Game->NewScriptFrame();
    }
}
int main()
{
    int bad = 0;
    CGameScriptInterface gi;
    CProximityPodium p; p.Game = &gi; p.Me.id = 5;
    // far once, then near: one activation; then done at the loop test
    g_log[0] = 0; g_near = "01"; g_done = "00001";
    p.Main();
    if (std::strcmp(g_log, "pred;set;~pred;frame;frame;on;pause10;off;frame;") != 0 || *g_near || *g_done) bad = 1;
    // done straight away
    g_log[0] = 0; g_near = ""; g_done = "1";
    p.Main();
    if (std::strcmp(g_log, "pred;set;~pred;frame;") != 0 || *g_done) bad = 1;
    // done while waiting for the hero
    g_log[0] = 0; g_near = "0"; g_done = "001";
    p.Main();
    if (std::strcmp(g_log, "pred;set;~pred;frame;frame;") != 0 || *g_done || *g_near) bad = 1;
    if (bad) { std::printf("00efb360_TEST FAIL\n"); return 1; }
    std::printf("00efb360_TEST PASS\n");
    return 0;
}