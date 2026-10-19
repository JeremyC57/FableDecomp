// Behaviour test for CPermanentBarrier::Main (0x00D2D830): filter registration, first update, the barrier effect on Me's position, then updates until done.
#include <cstdio>
#include <cstring>
static char g_log[256];
static void LOG(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct C3DVector { float X, Y, Z; };
struct CCharString { char Text[32]; CCharString(const char* t, long = -1) { std::strcpy(Text, t); } ~CCharString() {} };
class CScriptThing {
public:
    C3DVector Pos; int id;
    CScriptThing(int i) : id(i) { Pos.X = 1; Pos.Y = 2; Pos.Z = 3; }
    CScriptThing(const CScriptThing& o) : Pos(o.Pos), id(o.id) {}
    ~CScriptThing() { if (id == 99) LOG("~effect"); }
    virtual const C3DVector& GetPos() const { return Pos; }
};
struct CGameScriptInterface {
    int updates; const C3DVector* where;
    virtual void Update() { ++updates; LOG("update"); }
    virtual CScriptThing CreateEffect(const CCharString& name, const C3DVector& position, const CCharString& bone,
                                      float angle, bool a, bool b) const
    {
        LOG(std::strcmp(name.Text, "HK_MAGIC_BARRIER") == 0 && bone.Text[0] == 0 && angle == 0.0f && !a && !b ? "effect" : "effect?");
        const_cast<CGameScriptInterface*>(this)->where = &position;
        return CScriptThing(99);
    }
    virtual ~CGameScriptInterface() {}
};
struct CIsThingAlive {
    CScriptThing m_thing;
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) {}
    virtual ~CIsThingAlive() {}
};
class CPermanentBarrier {
public:
    CGameScriptInterface* Game; CScriptThing Me; int doneAfter;
    CPermanentBarrier() : Me(1) {}
    void RegisterFilter(const CIsThingAlive& filter) { LOG(filter.m_thing.id == 1 ? "filter" : "filter?"); }
    bool IsDone() { return Game->updates >= doneAfter; }
    void Main()
    {
        RegisterFilter(CIsThingAlive(Me));
        Game->Update();
        if (IsDone())
            return;
        CScriptThing effect = Game->CreateEffect("HK_MAGIC_BARRIER", Me.GetPos(), "", 0.0f, false, false);
        do {
            Game->Update();
        } while (!IsDone());
    }
};
int main()
{
    CGameScriptInterface game; game.updates = 0; game.where = 0;
    CPermanentBarrier b; b.Game = &game; b.doneAfter = 3;
    g_log[0] = 0; b.Main();
    if (std::strcmp(g_log, "filter;update;effect;update;update;~effect;") != 0 || game.where != &b.Me.Pos) { std::printf("00d2d830_TEST FAIL\n"); return 1; }
    game.updates = 0; b.doneAfter = 1; g_log[0] = 0; b.Main();
    if (std::strcmp(g_log, "filter;update;") != 0) { std::printf("00d2d830_TEST FAIL\n"); return 1; }
    std::printf("00d2d830_TEST PASS\n");
    return 0;
}