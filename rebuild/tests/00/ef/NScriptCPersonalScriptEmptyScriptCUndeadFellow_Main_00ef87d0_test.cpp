// Behaviour test for NScript::CPersonalScript_EmptyScript::CUndeadFellow::Main 0x00EF87D0: the alive predicate over Me,
// Marker4 and Marker5, the early exit after the first frame, the spawned "ParentClass.ChangeAlpha" thread on Me with the
// empty region name, the scripting wait (priority 4) and the endless Marker4 / Marker5 walk until the thread terminates,
// plus the reference counts of the things (every copy released at each exit).
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Imp { int refs; char name[16]; };
static Imp g_me, g_m4, g_m5;
struct Thing {
    Imp* p;
    explicit Thing(Imp* i) : p(i) { if (p) ++p->refs; }
    Thing(const Thing& o) : p(o.p) { if (p) ++p->refs; }
    ~Thing() { if (p) --p->refs; }
};
struct Game {
    int scriptFailures;
    void NewScriptFrame() { Log("Frame"); }
    Thing GetThingWithScriptName(const char* name)
    {
        if (std::strcmp(name, "Marker4") == 0) return Thing(&g_m4);
        if (std::strcmp(name, "Marker5") == 0) return Thing(&g_m5);
        return Thing(0);
    }
    bool StartScriptingEntity(const Thing& t, int priority)
    {
        if (scriptFailures-- > 0) { Log("StartFail"); return false; }
        char b[48]; std::sprintf(b, "Start(%s,%d)", t.p->name, priority); Log(b); return true;
    }
};
struct Object {
    void MoveToThing(const Thing& t, float d, int type, bool a, bool b, bool c, bool e)
    {
        char s[64]; std::sprintf(s, "Move(%s,%g,%d,%d%d%d%d)", t.p->name, d, type, a, b, c, e); Log(s);
    }
};
static int g_aliveRefs;
struct Script {
    Game* game; Thing me; int terminateAt, calls;
    Script() : me(&g_me) {}
    bool IsActiveThreadTerminating() { ++calls; return calls == terminateAt; }
    void SetPredicate(const Thing& a, const Thing& b, const Thing& c)
    {
        char s[64]; std::sprintf(s, "Alive(%s&&%s&&%s)", a.p->name, b.p->name, c.p->name); Log(s);
        g_aliveRefs = g_m4.refs + g_m5.refs;
    }
    void Spawn(const char* name, Thing par, const char* region) { char s[64]; std::sprintf(s, "Spawn(%s,%s,%s)", name, par.p->name, region); Log(s); }
    void Main()
    {
        Thing marker4 = game->GetThingWithScriptName("Marker4");
        Thing marker5 = game->GetThingWithScriptName("Marker5");
        SetPredicate(me, marker4, marker5);
        game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        Spawn("ParentClass.ChangeAlpha", me, "");
        Object object;
        while (!game->StartScriptingEntity(me, 4)) {
            game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        for (;;) {
            if (IsActiveThreadTerminating()) return;
            object.MoveToThing(marker4, 0.0f, 0, true, true, false, true);
            object.MoveToThing(marker5, 0.0f, 0, true, true, false, true);
            game->NewScriptFrame();
        }
    }
};
static bool Run(int failures, int terminateAt, const char* expect)
{
    g_log[0] = 0;
    {
        Game g; g.scriptFailures = failures;
        Script s; s.game = &g; s.terminateAt = terminateAt; s.calls = 0;
        s.Main();
    }
    if (std::strcmp(g_log, expect) != 0) { std::printf("got %s\n", g_log); return false; }
    if (g_me.refs != 0 || g_m4.refs != 0 || g_m5.refs != 0 || g_aliveRefs != 2) { std::printf("refs leak\n"); return false; }
    return true;
}
int main()
{
    std::strcpy(g_me.name, "Me"); std::strcpy(g_m4.name, "Marker4"); std::strcpy(g_m5.name, "Marker5");
    const char* head = "Alive(Me&&Marker4&&Marker5);Frame;";
    char spawn[256], loop[512], waited[512];
    std::sprintf(spawn, "%sSpawn(ParentClass.ChangeAlpha,Me,);", head);
    std::sprintf(loop, "%sStart(Me,4);Move(Marker4,0,0,1101);Move(Marker5,0,0,1101);Frame;"
                 "Move(Marker4,0,0,1101);Move(Marker5,0,0,1101);Frame;", spawn);
    std::sprintf(waited, "%sStartFail;Frame;Start(Me,4);Move(Marker4,0,0,1101);Move(Marker5,0,0,1101);Frame;", spawn);
    char failExit[256]; std::sprintf(failExit, "%sStartFail;Frame;", spawn);
    char afterStart[256]; std::sprintf(afterStart, "%sStart(Me,4);", spawn);
    if (!Run(0, 1, head) || !Run(1, 2, failExit) || !Run(0, 2, afterStart) || !Run(0, 5, loop) || !Run(1, 5, waited)) {
        std::printf("00ef87d0_TEST FAIL\n");
        return 1;
    }
    std::printf("00ef87d0_TEST PASS\n");
    return 0;
}