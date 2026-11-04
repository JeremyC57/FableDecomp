// Behaviour test for 0x00CFCCA0 NScript::CQ_AwakeningTheOracleScript::CPretendGuard::Main(): the row's logic copied over test
// doubles (predicate, handle, game interface, thread-termination counter); checks the call order of every exit.
#include <cstdio>
#include <cstring>
struct CScriptThing { unsigned char body[12]; };
struct CIsThingAlive { CScriptThing Thing; CIsThingAlive(const CScriptThing& t) : Thing(t) { } };
static int g_log[128]; static int g_n = 0;
static void Log(int v) { if (g_n < 128) g_log[g_n++] = v; }
struct CHandle {
    CHandle() { Log(1); }
    ~CHandle() { Log(2); }
    bool IsValid() const { Log(3); return true; }
    void Release() { Log(4); }
};
static int g_startFailures = 0;
struct CGame {
    void NewScriptFrame() const { Log(5); }
    bool StartScriptingEntity(const CScriptThing&, CHandle&, int priority) { Log(priority == 4 ? 6 : 99); if (g_startFailures > 0) { --g_startFailures; return false; } return true; }
    void RemoveThing(const CScriptThing&, bool a, bool b) const { Log(!a && b ? 7 : 98); }
};
struct CParent { bool PlayedOpeningNecroCS; };
static int g_doneAfter = 0, g_doneCalls = 0;
struct CPretendGuard {
    CGame* Game; CScriptThing Me; CParent* ParentClass;
    void SetPredicate(const CIsThingAlive&) { Log(8); }
    bool IsActiveThreadTerminating() const { Log(9); return ++g_doneCalls >= g_doneAfter; }
    void Main();
};
void CPretendGuard::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    if (ParentClass->PlayedOpeningNecroCS) {
        if (IsActiveThreadTerminating())
            return;
        Game->RemoveThing(Me, false, true);
    }
    CHandle handle;
    if (handle.IsValid())
        handle.Release();
    while (!Game->StartScriptingEntity(Me, handle, 4)) {
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
static bool Run(bool played, int failures, int doneAfter, const int* want, int nwant)
{
    CGame game; CParent parent; parent.PlayedOpeningNecroCS = played;
    CPretendGuard g; g.Game = &game; g.ParentClass = &parent; std::memset(g.Me.body, 0, 12);
    g_n = 0; g_startFailures = failures; g_doneAfter = doneAfter; g_doneCalls = 0;
    g.Main();
    if (g_n != nwant) return false;
    for (int i = 0; i < nwant; ++i) if (g_log[i] != want[i]) return false;
    return true;
}
int main()
{
    static const int a[] = { 8, 5, 9 };                                           // done after the first frame
    static const int b[] = { 8, 5, 9, 9, 7, 1, 3, 4, 6, 9, 5, 9, 5, 9, 5, 9, 2 };      // played: remove, start at once, two frames
    static const int c[] = { 8, 5, 9, 1, 3, 4, 6, 5, 9, 6, 9, 2 };                 // one failed start, done right after start
    static const int d[] = { 8, 5, 9, 1, 3, 4, 6, 5, 9, 2 };                       // done inside the start wait
    bool ok = Run(false, 0, 1, a, 3) && Run(true, 0, 6, b, 17) && Run(false, 1, 3, c, 12) && Run(false, 5, 2, d, 10);
    if (!ok) { std::printf("00cfcca0_TEST FAIL\n"); return 1; }
    std::printf("00cfcca0_TEST PASS\n");
    return 0;
}