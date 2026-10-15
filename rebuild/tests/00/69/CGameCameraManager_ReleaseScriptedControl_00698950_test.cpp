// Behaviour test for 0x00698950 CGameCameraManager::ReleaseScriptedControl.
#include <cstdio>
static int g_destroyed = 0, g_pushed = 0;
static void Destroy(void*) { ++g_destroyed; }
struct SCountBlock { int m_references; void (*m_pfnDestroy)(void*); void* m_pObject; };
struct CP {
    void* m_pObject; SCountBlock* m_pCount;
    void Reset()
    {
        if (m_pCount) { --m_pCount->m_references; if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; } }
        m_pCount = 0; m_pObject = 0;
    }
};
struct M {
    bool NewAutoMode; CP PCurrentScriptModule; int ScriptedCameraRemoveTimer; bool CameraPreload, CameraPreloadUseFastMode;
    bool UsingCameraManually, ScriptedCameraModeFromGame;
    void PushCurrentManualMode() { ++g_pushed; }
    void ReleaseScriptedControl()
    {
        ScriptedCameraRemoveTimer = -1;
        PCurrentScriptModule.Reset();
        if (!UsingCameraManually) NewAutoMode = true; else PushCurrentManualMode();
        CameraPreload = true;
        CameraPreloadUseFastMode = ScriptedCameraModeFromGame;
    }
};
static int Fail() { std::printf("00698950_TEST FAIL\n"); return 1; }
int main()
{
    int obj = 0;
    SCountBlock* b = new SCountBlock; b->m_references = 1; b->m_pfnDestroy = Destroy; b->m_pObject = &obj;
    M m; m.NewAutoMode = false; m.PCurrentScriptModule.m_pObject = &obj; m.PCurrentScriptModule.m_pCount = b;
    m.ScriptedCameraRemoveTimer = 9; m.CameraPreload = false; m.CameraPreloadUseFastMode = false; m.UsingCameraManually = false;
    m.ScriptedCameraModeFromGame = true;
    m.ReleaseScriptedControl();
    if (m.ScriptedCameraRemoveTimer != -1 || g_destroyed != 1 || m.PCurrentScriptModule.m_pObject || m.PCurrentScriptModule.m_pCount ||
        !m.NewAutoMode || g_pushed != 0 || !m.CameraPreload || !m.CameraPreloadUseFastMode) return Fail();
    m.NewAutoMode = false; m.UsingCameraManually = true; m.ScriptedCameraModeFromGame = false;
    m.ReleaseScriptedControl();
    if (m.NewAutoMode || g_pushed != 1 || m.CameraPreloadUseFastMode || g_destroyed != 1) return Fail();
    std::printf("00698950_TEST PASS\n");
    return 0;
}