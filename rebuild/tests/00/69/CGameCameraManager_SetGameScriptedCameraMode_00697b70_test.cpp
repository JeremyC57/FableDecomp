// Behaviour test for 0x00697B70 CGameCameraManager::SetGameScriptedCameraMode.
#include <cstdio>
static int g_destroyed = 0;
static void Destroy(void*) { ++g_destroyed; }
struct SCountBlock { int m_references; void (*m_pfnDestroy)(void*); void* m_pObject; };
struct CScriptModuleBase { bool m_bFromGame; };
struct CP {
    CScriptModuleBase* m_pObject; SCountBlock* m_pCount;
    CP(CScriptModuleBase* o, SCountBlock* c) : m_pObject(o), m_pCount(c) { if (m_pCount) ++m_pCount->m_references; }
    CP(const CP& o) : m_pObject(o.m_pObject), m_pCount(o.m_pCount) { if (m_pCount) ++m_pCount->m_references; }
    CP& operator=(const CP& other)
    {
        if (m_pCount != other.m_pCount) {
            if (m_pCount) { --m_pCount->m_references; if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; } }
            m_pObject = other.m_pObject; m_pCount = other.m_pCount;
            if (m_pCount) ++m_pCount->m_references;
        }
        return *this;
    }
    ~CP() { if (m_pCount) { if (--m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; } } }
};
struct M {
    CP PPendingScriptModule; int ScriptedCameraRemoveTimer; bool CameraPreload, CameraPreloadUseFastMode, ScriptedCameraModeFromGame;
    M() : PPendingScriptModule(0, 0) {}
    void Set(CP module, bool preload)
    {
        module.m_pObject->m_bFromGame = true;
        PPendingScriptModule = module;
        ScriptedCameraModeFromGame = true;
        ScriptedCameraRemoveTimer = -1;
        if (preload) { CameraPreload = preload; CameraPreloadUseFastMode = true; }
    }
};
static int Fail() { std::printf("00697b70_TEST FAIL\n"); return 1; }
int main()
{
    CScriptModuleBase a, b; a.m_bFromGame = b.m_bFromGame = false;
    SCountBlock* ba = new SCountBlock; ba->m_references = 0; ba->m_pfnDestroy = Destroy; ba->m_pObject = &a;
    SCountBlock* bb = new SCountBlock; bb->m_references = 0; bb->m_pfnDestroy = Destroy; bb->m_pObject = &b;
    M m; m.ScriptedCameraRemoveTimer = 5; m.CameraPreload = m.CameraPreloadUseFastMode = m.ScriptedCameraModeFromGame = false;
    {
        CP pa(&a, ba);
        m.Set(pa, false);
        if (!a.m_bFromGame || m.PPendingScriptModule.m_pObject != &a || ba->m_references != 2 || m.ScriptedCameraRemoveTimer != -1 ||
            !m.ScriptedCameraModeFromGame || m.CameraPreload) return Fail();
    }
    if (ba->m_references != 1) return Fail();
    {
        CP pb(&b, bb);
        m.Set(pb, true);
    }
    if (g_destroyed != 1 || m.PPendingScriptModule.m_pObject != &b || bb->m_references != 1 || !m.CameraPreload || !m.CameraPreloadUseFastMode) return Fail();
    std::printf("00697b70_TEST PASS\n");
    return 0;
}