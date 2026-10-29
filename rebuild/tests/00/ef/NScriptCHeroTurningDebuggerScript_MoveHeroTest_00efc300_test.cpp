// Behaviour test for 0x00EFC300 NScript::CHeroTurningDebuggerScript::MoveHeroTest: each pass (while the thread is
// not terminating) creates a scripted-thing handle, starts scripting the hero into it at priority 4, moves the hero to
// the MarkerToFace marker's position (speed 0, move type 0, false, true), pauses 30 seconds, releases the handle (and
// the marker thing temporary), then waits one script frame.
#include <cstdio>
#include <cstring>

static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }

class C3DVector { public: float X, Y, Z; };
class CScriptThing { public:
    const char* Name; C3DVector Pos; bool Temp;
    CScriptThing(const char* n, float x, float y, bool temp) : Name(n), Temp(temp) { Pos.X = x; Pos.Y = y; Pos.Z = 0.0f; }
    ~CScriptThing() { if (Temp) Log("~thing"); }
    const C3DVector& GetPos() const { return Pos; }
};
class CScriptGameResourceObjectScriptedThingBase { public:
    const CScriptThing* Bound;
    CScriptGameResourceObjectScriptedThingBase() : Bound(0) { Log("handle"); }
    ~CScriptGameResourceObjectScriptedThingBase() { Log("~handle"); }
    void MoveToPosition(const C3DVector& pos, float speed, int type, bool a, bool b)
    {
        char t[96];
        std::sprintf(t, "move(%s,%d,%d,%d,%d,%d,%d)", Bound ? Bound->Name : "-", (int)pos.X, (int)pos.Y, (int)speed, type, a, b);
        Log(t);
    }
};
class CGameScriptInterface { public:
    CScriptThing Hero;
    CGameScriptInterface() : Hero("Hero", 0.0f, 0.0f, false) {}
    void NewScriptFrame() { Log("frame"); }
    bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& object, int priority)
    {
        char t[64]; std::sprintf(t, "start(%s,%d)", thing.Name, priority); Log(t);
        object.Bound = &thing; return true;
    }
    CScriptThing& GetHero() { return Hero; }
    CScriptThing GetThingWithScriptName(const char* name) const
    {
        return CScriptThing(name, 12.0f, 34.0f, true);   // a temporary (VC7.1 has no NRVO)
    }
    void Pause(float seconds) const { char t[32]; std::sprintf(t, "pause(%d)", (int)seconds); Log(t); }
};

struct CHeroTurningDebuggerScript {
    CGameScriptInterface* Game;
    int TermAfter, TermCalls;   // IsActiveThreadTerminating is true from call TermAfter on (1-based)
    bool IsActiveThreadTerminating() { ++TermCalls; return TermCalls >= TermAfter; }
    void MoveHeroTest()
    {
        while (!IsActiveThreadTerminating()) {
            {
                CScriptGameResourceObjectScriptedThingBase hero;
                Game->StartScriptingEntity(Game->GetHero(), hero, 4);
                hero.MoveToPosition(Game->GetThingWithScriptName("MarkerToFace").GetPos(), 0.0f, 0, false, true);
                Game->Pause(30.0f);
            }
            Game->NewScriptFrame();
        }
    }
};

static int Run(int termAfter, const char* expect)
{
    CGameScriptInterface gsi;
    CHeroTurningDebuggerScript s; s.Game = &gsi; s.TermAfter = termAfter; s.TermCalls = 0;
    g_log[0] = 0;
    s.MoveHeroTest();
    if (std::strcmp(g_log, expect) != 0) { std::printf("got   %s\nwant  %s\n", g_log, expect); return 1; }
    return 0;
}

int main()
{
    int bad = 0;
    bad |= Run(1, "");
    bad |= Run(2, "handle;start(Hero,4);move(Hero,12,34,0,0,0,1);~thing;pause(30);~handle;frame;");
    bad |= Run(3, "handle;start(Hero,4);move(Hero,12,34,0,0,0,1);~thing;pause(30);~handle;frame;"
                  "handle;start(Hero,4);move(Hero,12,34,0,0,0,1);~thing;pause(30);~handle;frame;");
    if (bad) { std::printf("00efc300_TEST FAIL\n"); return 1; }
    std::printf("00efc300_TEST PASS\n");
    return 0;
}