// Behaviour test for 0x00547E40 NUISystem::CMovie::Update: state 3 + playing + current movie -> retry pending play and
// process; stop request outside state 3 -> close, delete the video system, free textures, clear the handle, notify owner.
#include <cstdio>

static int g_attempts, g_process, g_close, g_deleted, g_freed, g_baseUpdates, g_lastRelease, g_destroyed, g_notified;
static bool g_attemptResult;
struct CVideoSys {
    ~CVideoSys() { ++g_deleted; }
    bool AttemptToPlay() { ++g_attempts; return g_attemptResult; }
    bool Process() { ++g_process; return true; }
    void Close() { ++g_close; }
};
static CVideoSys* g_pVideoSys;
struct CPrimitive { long RefCount; void Destroy() { ++g_destroyed; } void OnLastHandleReleased() { ++g_lastRelease; } };
struct CEnginePrimitiveHandle {
    CPrimitive* Object;
    void Clear()
    {
        if (Object != 0 && Object->RefCount == 1)
            Object->OnLastHandleReleased();
        if (Object) { if (--Object->RefCount == 0) Object->Destroy(); Object = 0; }
        Object = 0;
    }
};
struct COwner { void OnChildMovieStopped() { ++g_notified; } };
struct CMovie {
    COwner* m_pOwner; unsigned long CurrentState; bool stopRequested;
    bool Playing; CEnginePrimitiveHandle Handle; bool AttempToPlay;
    static CMovie* PlayingMovie;
    bool IsStopRequested() { return stopRequested; }
    void BaseUpdate(float) { ++g_baseUpdates; }
    void FreeTextures() { ++g_freed; }
    void Update(float dt)
    {
        BaseUpdate(dt);
        if (CurrentState == 3 && g_pVideoSys && Playing == true && PlayingMovie == this) {
            if (AttempToPlay && g_pVideoSys->AttemptToPlay())
                AttempToPlay = false;
            g_pVideoSys->Process();
        }
        if (IsStopRequested() && CurrentState != 3 && Playing == true && PlayingMovie == this) {
            g_pVideoSys->Close();
            Playing = false;
            PlayingMovie = 0;
            FreeTextures();
            delete g_pVideoSys;
            g_pVideoSys = 0;
            Handle.Clear();
            if (m_pOwner)
                m_pOwner->OnChildMovieStopped();
        }
    }
};
CMovie* CMovie::PlayingMovie;
static int Fail(int n) { std::printf("00547e40_TEST FAIL %d\n", n); return 1; }
int main()
{
    COwner owner; CPrimitive prim = { 2 };
    CMovie m; m.m_pOwner = &owner; m.CurrentState = 3; m.stopRequested = false; m.Playing = true; m.Handle.Object = &prim; m.AttempToPlay = true;
    CMovie::PlayingMovie = &m; g_pVideoSys = new CVideoSys;
    g_attemptResult = false; m.Update(0.1f);
    if (g_attempts != 1 || g_process != 1 || !m.AttempToPlay) return Fail(1);
    g_attemptResult = true; m.Update(0.1f);
    if (g_attempts != 2 || g_process != 2 || m.AttempToPlay) return Fail(2);
    m.Update(0.1f);
    if (g_attempts != 2 || g_process != 3) return Fail(3);
    m.stopRequested = true; m.Update(0.1f);                  // state 3: no stop
    if (g_close != 0) return Fail(4);
    m.CurrentState = 2; m.Update(0.1f);
    if (g_close != 1 || g_deleted != 1 || g_pVideoSys != 0 || m.Playing || CMovie::PlayingMovie != 0 || g_freed != 1) return Fail(5);
    if (m.Handle.Object != 0 || prim.RefCount != 1 || g_lastRelease != 0 || g_destroyed != 0 || g_notified != 1) return Fail(6);
    m.Update(0.1f);                                          // not playing any more: nothing
    if (g_close != 1 || g_baseUpdates != 6) return Fail(7);
    std::printf("00547e40_TEST PASS\n");
    return 0;
}
