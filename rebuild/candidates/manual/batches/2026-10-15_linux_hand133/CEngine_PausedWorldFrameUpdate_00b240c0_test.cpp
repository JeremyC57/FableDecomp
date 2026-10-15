// Behaviour test for CEngine::PausedWorldFrameUpdate() @ 0x00b240c0
#include <cstdio>
struct CEngineComponentParams { int frame; };
struct CEngineComponent {
    unsigned int m_flags; int m_world; int m_paused; const CEngineComponentParams* m_seen;
    virtual void WorldFrameUpdate(const CEngineComponentParams& params) { ++m_world; m_seen = &params; }
    virtual void PausedWorldFrameUpdate() { ++m_paused; }
    virtual unsigned int GetSceneFilterFlags() { return m_flags; }
};
template <class T> struct CRetailVector { T* m_start; T* m_finish; T* m_end; T* begin() const { return m_start; } T* end() const { return m_finish; } };
struct CEngine {
    bool IsInitialised; long CurrentRenderFrame, CurrentWorldFrame, CurrentPausedWorldFrame;
    double NonInterpolatedWorldTime, NonInterpolatedPausedWorldTime;
    unsigned int WorldFramesPerSecond, SceneFilterFlags;
    CRetailVector<CEngineComponent*> EngineComponents;
    void PausedWorldFrameUpdate()
    {
        if (IsInitialised && CurrentRenderFrame != 0) {
            for (CEngineComponent** it = EngineComponents.begin(); it != EngineComponents.end(); ++it) {
                if ((*it)->GetSceneFilterFlags() & SceneFilterFlags)
                    (*it)->PausedWorldFrameUpdate();
            }
        }
        ++CurrentPausedWorldFrame;
        NonInterpolatedPausedWorldTime += 1.0f / (float)WorldFramesPerSecond;
    }
};
int main()
{
    CEngineComponent c[3];
    CEngineComponent* list[3] = { &c[0], &c[1], &c[2] };
    int i;
    for (i = 0; i < 3; ++i) { c[i].m_world = 0; c[i].m_paused = 0; c[i].m_seen = 0; }
    c[0].m_flags = 1; c[1].m_flags = 2; c[2].m_flags = 3;
    CEngine e;
    e.IsInitialised = true; e.CurrentRenderFrame = 5; e.CurrentWorldFrame = 0; e.CurrentPausedWorldFrame = 0;
    e.NonInterpolatedWorldTime = 0.0; e.NonInterpolatedPausedWorldTime = 0.0; e.WorldFramesPerSecond = 10; e.SceneFilterFlags = 2;
    e.EngineComponents.m_start = list; e.EngineComponents.m_finish = list + 3; e.EngineComponents.m_end = list + 3;
    CEngineComponentParams p; p.frame = 1;
    e.PausedWorldFrameUpdate();
    int hits = c[0].m_world + c[0].m_paused, hits1 = c[1].m_world + c[1].m_paused, hits2 = c[2].m_world + c[2].m_paused;
    if (hits != 0 || hits1 != 1 || hits2 != 1 || e.CurrentPausedWorldFrame != 1 || e.NonInterpolatedPausedWorldTime < 0.0999 || e.NonInterpolatedPausedWorldTime > 0.1001) { std::printf("00b240c0_TEST FAIL\n"); return 1; }
    e.CurrentRenderFrame = 0;
    e.PausedWorldFrameUpdate();
    if (c[1].m_world + c[1].m_paused != 1 || e.CurrentPausedWorldFrame != 2 || e.NonInterpolatedPausedWorldTime < 0.1999 || e.NonInterpolatedPausedWorldTime > 0.2001) { std::printf("00b240c0_TEST FAIL\n"); return 1; }
    std::printf("00b240c0_TEST PASS\n");
    return 0;
}
