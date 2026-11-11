// Behaviour test for 0x00D43190 NScript::CQ_GuildTrainingScript::CKillBird::Main(): the row's body copied over test doubles
// (predicate, handle, game interface, thread-termination counter); checks the call order of every exit.
#include <cstdio>
#include <cstring>
class CScriptThing { public: unsigned char body[12]; };
struct CIsThingConscious { CScriptThing Thing; CIsThingConscious(const CScriptThing& t) : Thing(t) { } };
static int g_log[128]; static int g_n = 0;
static void Log(int v) { if (g_n < 128) g_log[g_n++] = v; }
static bool g_valid = true;
class CScriptGameResourceObjectScriptedThingBase { public:
    CScriptGameResourceObjectScriptedThingBase() { Log(1); }
    ~CScriptGameResourceObjectScriptedThingBase() { Log(2); }
    bool IsValid() const { Log(3); return g_valid; }
    void Release() { Log(4); }
};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_HIGHEST = 4 };
static int g_startFailures = 0;
class CGame { public:
    void NewScriptFrame() const { Log(5); }
    bool StartScriptingEntity(const CScriptThing&, CScriptGameResourceObjectScriptedThingBase&, EScriptAIPriority priority) { Log(priority == 4 ? 6 : 99); if (g_startFailures > 0) { --g_startFailures; return false; } return true; }
};
static int g_doneAfter = 0, g_doneCalls = 0;
struct CKillBird {
    CGame* Game; CScriptThing Me;
    void SetPredicate(const CIsThingConscious&) { Log(8); }
    bool IsActiveThreadTerminating() const { Log(9); return ++g_doneCalls >= g_doneAfter; }
    void Main();
};
void CKillBird::Main()
{
    SetPredicate(CIsThingConscious(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase handle;
    if (handle.IsValid())
        handle.Release();
    while (!Game->StartScriptingEntity(Me, handle, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}
static bool Run(bool valid, int failures, int doneAfter, const int* want, int nwant)
{
    CGame game; CKillBird g; g.Game = &game; std::memset(g.Me.body, 0, 12);
    g_n = 0; g_valid = valid; g_startFailures = failures; g_doneAfter = doneAfter; g_doneCalls = 0;
    g.Main();
    if (g_n != nwant) return false;
    for (int i = 0; i < nwant; ++i) if (g_log[i] != want[i]) return false;
    return true;
}
int main()
{
    static const int a[] = { 8, 5, 9 };                                           // terminating after the first frame
    static const int b[] = { 8, 5, 9, 1, 3, 4, 6, 9, 5, 9, 5, 9, 5, 9, 2 };        // start at once, three idle frames
    static const int c[] = { 8, 5, 9, 1, 3, 4, 6, 5, 9, 6, 9, 2 };                 // one failed start, terminating right after start
    static const int d[] = { 8, 5, 9, 1, 3, 6, 5, 9, 2 };                          // no handle to release, terminating inside the wait
    bool ok = Run(true, 0, 1, a, 3) && Run(true, 0, 5, b, 15) && Run(true, 1, 3, c, 12) && Run(false, 5, 2, d, 9);
    if (!ok) { std::printf("00d43190_TEST FAIL\n"); return 1; }
    std::printf("00d43190_TEST PASS\n");
    return 0;
}