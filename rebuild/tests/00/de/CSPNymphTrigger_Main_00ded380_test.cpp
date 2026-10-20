// Behaviour test for 0x00ded380 CSP_NymphTrigger::Main
#include <cstdio>
#include <cstring>

static int g_stringDtors = 0, g_thingDtors = 0;
struct CCharString {
    const char* text;
    CCharString(const char* t, long = -1) : text(t) { }
    ~CCharString() { ++g_stringDtors; }
};
struct C3DVector { float X, Y, Z; };
struct CScriptThing {
    int id; C3DVector pos;
    CScriptThing(int i) : id(i) { pos.X = pos.Y = pos.Z = 0; }
    const C3DVector& GetPos() const { return pos; }
    ~CScriptThing() { if (id == 99) ++g_thingDtors; }
};
static int g_distanceCalls = 0, g_closeAfter = 0;
static bool IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance)
{
    ++g_distanceCalls;
    return a.id == 1 && b.id == 2 && distance == 10.0f && g_distanceCalls > g_closeAfter;
}
struct CGameScriptInterface {
    CScriptThing hero; int frames, creates; const char* defName; const char* scriptName; const C3DVector* where; bool flag;
    CGameScriptInterface() : hero(1), frames(0), creates(0) { }
    void NewScriptFrame() const { ++const_cast<CGameScriptInterface*>(this)->frames; }
    CScriptThing& GetHero() { return hero; }
    CScriptThing CreateCreature(const CCharString& d, const C3DVector& p, const CCharString& s, bool b) const
    {
        CGameScriptInterface* self = const_cast<CGameScriptInterface*>(this);
        ++self->creates; self->defName = d.text; self->scriptName = s.text; self->where = &p; self->flag = b;
        return CScriptThing(99);
    }
};
struct CSP_NymphTrigger {
    CGameScriptInterface* Game;
    CScriptThing Me;
    int terminateAt, polls;
    CSP_NymphTrigger() : Me(2), terminateAt(-1), polls(0) { }
    bool IsActiveThreadTerminating() const { CSP_NymphTrigger* s = const_cast<CSP_NymphTrigger*>(this); return ++s->polls == terminateAt; }
    void Main()
    {
        while (!IsDistanceBetweenThingsUnder(Game->GetHero(), Me, 10.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        CScriptThing nymph = Game->CreateCreature("CREATURE_SUCCUBUS_NYMPH_01", Me.GetPos(), "EnemySeekingHero", true);
    }
};

int main()
{
    CGameScriptInterface gsi; CSP_NymphTrigger t; t.Game = &gsi;
    g_closeAfter = 3;                         // hero arrives on the 4th test
    t.Main();
    if (gsi.frames != 3 || gsi.creates != 1 || std::strcmp(gsi.defName, "CREATURE_SUCCUBUS_NYMPH_01") != 0
        || std::strcmp(gsi.scriptName, "EnemySeekingHero") != 0 || gsi.where != &t.Me.pos || !gsi.flag
        || g_stringDtors != 2 || g_thingDtors != 1) { std::printf("00ded380_TEST FAIL\n"); return 1; }
    // terminated while waiting: no creature
    CGameScriptInterface gsi2; CSP_NymphTrigger t2; t2.Game = &gsi2; t2.terminateAt = 2;
    g_distanceCalls = 0; g_closeAfter = 100;
    t2.Main();
    if (gsi2.frames != 2 || gsi2.creates != 0) { std::printf("00ded380_TEST FAIL\n"); return 1; }
    std::printf("00ded380_TEST PASS\n");
    return 0;
}