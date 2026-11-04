// Behaviour test (0x00efab40): Cwelcome_guy::Main takes Me under scripted control (retrying each frame, priority 4), walks it to the hero (MoveToThing hero, 3.0, walk, false, true, false, true), then each frame waits for the hero to come within squared 2D distance 3 and turns Me to face the hero; every wait stops when the script thread terminates, and the handle is always destroyed
#include <cstdio>

class C3DVector { public: float X, Y, Z; };

// The mock game: a frame counter, a terminate frame, a start-scripting success frame and a hero path.
static int g_frame = 0;
static int g_terminateFrame = 1000;
static int g_startSucceedsOnCall = 0;
static int g_startCalls = 0;
static int g_startPriority = -1;
static int g_moveCalls = 0;
static float g_moveDistance = 0.0f;
static int g_moveArgs[5];
static int g_faceCalls = 0;
static int g_faceFrames[64];
static int g_faceInstant = -1;
static int g_handleDtors = 0;
static int g_releaseCalls = 0;
static C3DVector g_heroPath[64];
static int g_pathLength = 0;

class CScriptThing { public:
    bool IsHero;
    C3DVector MyPos;
    const C3DVector& GetPos() const
    {
        if (!IsHero) return MyPos;
        int f = g_frame < g_pathLength ? g_frame : g_pathLength - 1;
        return g_heroPath[f];
    }
};
static CScriptThing g_hero;

struct ScriptedHandle {
    bool Scripting;
    ScriptedHandle() : Scripting(false) { }
    ~ScriptedHandle() { ++g_handleDtors; }
    bool IsScripting() { return Scripting; }
    void Release() { ++g_releaseCalls; Scripting = false; }
    void MoveToThing(const CScriptThing& thing, float distance, int moveType, bool a, bool b, bool c, bool d)
    {
        ++g_moveCalls;
        g_moveDistance = distance;
        g_moveArgs[0] = moveType; g_moveArgs[1] = a; g_moveArgs[2] = b; g_moveArgs[3] = c; g_moveArgs[4] = d;
        if (!thing.IsHero) g_moveArgs[0] = -99;
    }
};

struct Game {
    void NewScriptFrame() { ++g_frame; }
    bool StartScriptingEntity(const CScriptThing&, ScriptedHandle& handle, int priority)
    {
        g_startPriority = priority;
        if (++g_startCalls >= g_startSucceedsOnCall) { handle.Scripting = true; return true; }
        return false;
    }
    CScriptThing& GetHero() { return g_hero; }
    void EntitySetFacingAngleTowardsThing(const CScriptThing&, const CScriptThing& target, bool instant)
    {
        if (target.IsHero && g_faceCalls < 64) g_faceFrames[g_faceCalls] = g_frame;
        ++g_faceCalls;
        g_faceInstant = instant ? 1 : 0;
    }
};

static float GetDistanceSquared2D(const C3DVector& a, const C3DVector& b)
{
    float dx = a.X - b.X;
    float dy = a.Y - b.Y;
    return dx * dx + dy * dy;
}

struct Cwelcome_guy {
    Game* GamePtr;
    CScriptThing Me;
    bool IsActiveThreadTerminating() const { return g_frame >= g_terminateFrame; }
    // The function under test, in the source's shape.
    void Main()
    {
        ScriptedHandle scripted;
        if (scripted.IsScripting())
            scripted.Release();
        while (!GamePtr->StartScriptingEntity(Me, scripted, 4)) {
            GamePtr->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        scripted.MoveToThing(GamePtr->GetHero(), 3.0f, 0, false, true, false, true);
        while (!IsActiveThreadTerminating()) {
            while (!(GetDistanceSquared2D(Me.GetPos(), GamePtr->GetHero().GetPos()) < 3.0f)) {
                GamePtr->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
                continue;
            }
            if (IsActiveThreadTerminating())
                return;
            GamePtr->EntitySetFacingAngleTowardsThing(Me, GamePtr->GetHero(), false);
            GamePtr->NewScriptFrame();
            continue;
        }
    }
};

static int fails = 0;
static void Check(bool ok, const char* what) { if (!ok) { std::printf("  fail: %s\n", what); ++fails; } }
static void Reset()
{
    g_frame = 0; g_startCalls = 0; g_startPriority = -1; g_moveCalls = 0; g_moveDistance = 0.0f; g_faceCalls = 0;
    g_faceInstant = -1; g_handleDtors = 0; g_releaseCalls = 0;
}

int main()
{
    Game game;
    g_hero.IsHero = true;
    Cwelcome_guy guy; guy.GamePtr = &game; guy.Me.IsHero = false;
    guy.Me.MyPos.X = 10.0f; guy.Me.MyPos.Y = 20.0f; guy.Me.MyPos.Z = 0.0f;

    // Scenario 1: scripting starts on the third try (frames 0..2); the hero is far until frame 5, within range for frames
    // 5..7 (a height difference is ignored: 2D), far again, then close from frame 10; the thread terminates at frame 12.
    for (int f = 0; f < 20; ++f) {
        C3DVector p; p.X = 10.0f; p.Y = 30.0f; p.Z = 0.0f;
        if ((f >= 5 && f <= 7) || f >= 10) { p.X = 11.0f; p.Y = 20.5f; p.Z = 50.0f; }   // dx 1, dy 0.5 -> 1.25 < 3
        g_heroPath[f] = p;
    }
    g_pathLength = 20;
    Reset(); g_startSucceedsOnCall = 3; g_terminateFrame = 12;
    guy.Main();
    Check(g_startCalls == 3 && g_startPriority == 4, "start scripting retried until it succeeds, priority 4");
    Check(g_moveCalls == 1, "walks to the hero once");
    Check(g_moveDistance > 2.999f && g_moveDistance < 3.001f, "move distance 3");
    Check(g_moveArgs[0] == 0 && g_moveArgs[1] == 0 && g_moveArgs[2] == 1 && g_moveArgs[3] == 0 && g_moveArgs[4] == 1, "move flags");
    // frames: start at 2; waits 2,3,4 (far); faces at 5, 6, 7; waits 8, 9; faces at 10, 11; terminate at 12
    Check(g_faceCalls == 5, "five facings");
    Check(g_faceFrames[0] == 5 && g_faceFrames[1] == 6 && g_faceFrames[2] == 7 && g_faceFrames[3] == 10 && g_faceFrames[4] == 11, "facing frames");
    Check(g_faceInstant == 0, "facing not instant");
    Check(g_frame == 12 && g_handleDtors == 1, "stops on terminate, handle destroyed once");

    // Scenario 2: the thread terminates while the start keeps failing -- no move, no facing.
    Reset(); g_startSucceedsOnCall = 100; g_terminateFrame = 3;
    guy.Main();
    Check(g_startCalls == 3 && g_moveCalls == 0 && g_faceCalls == 0 && g_handleDtors == 1, "terminate during the start wait");

    // Scenario 3: the start succeeds immediately but the thread is already terminating -- no move.
    Reset(); g_startSucceedsOnCall = 0; g_terminateFrame = 0;
    guy.Main();
    Check(g_startCalls == 1 && g_moveCalls == 0 && g_faceCalls == 0 && g_handleDtors == 1, "terminating right after the start");

    // Scenario 4: terminate while waiting for the hero (never close) -- the move happened, no facing.
    for (int f = 0; f < 20; ++f) { g_heroPath[f].X = 40.0f; g_heroPath[f].Y = 20.0f; g_heroPath[f].Z = 0.0f; }
    Reset(); g_startSucceedsOnCall = 1; g_terminateFrame = 6;
    guy.Main();
    Check(g_moveCalls == 1 && g_faceCalls == 0 && g_frame == 6 && g_handleDtors == 1, "terminate in the hero wait");

    // Scenario 5: just outside the threshold (squared distance 3.0625) is not close.
    for (int f = 0; f < 20; ++f) { g_heroPath[f].X = 11.75f; g_heroPath[f].Y = 20.0f; g_heroPath[f].Z = 0.0f; }
    Reset(); g_startSucceedsOnCall = 1; g_terminateFrame = 4;
    guy.Main();
    Check(g_faceCalls == 0, "squared distance over 3 does not count as close");

    if (fails) { std::printf("00efab40_TEST FAIL\n"); return 1; }
    std::printf("00efab40_TEST PASS\n");
    return 0;
}