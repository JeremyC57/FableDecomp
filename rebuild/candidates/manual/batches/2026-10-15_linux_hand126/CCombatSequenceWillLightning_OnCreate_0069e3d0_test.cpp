// Behaviour test for 0069e3d0 (camera manager helper labelled CCombatSequence_WillLightning::OnCreate):
// a new mode is built over (manager, descriptor, &m_pTarget), adopted by a counted pointer, passed
// BY VALUE to SetCameraMode (copy adds a reference, the callee's parameter is released), and the
// local release at the end leaves exactly the reference SetCameraMode kept.
#include <cstdio>

struct SCountBlock { long m_references; void (*m_pfnDestroy)(void*); void* m_pObject; };
static int g_destroyed = 0;
struct CMode { void* m_manager; const void* m_desc; const void* m_target; };
static void DestroyMode(void* p) { ++g_destroyed; delete (CMode*)p; }

struct CCountedPointer {
    CMode* m_pObject; SCountBlock* m_pCount;
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
    CCountedPointer(const CCountedPointer& o) : m_pObject(o.m_pObject), m_pCount(o.m_pCount) { if (m_pCount) ++m_pCount->m_references; }
    void Reset(CMode* object)
    {
        if (m_pCount) { --m_pCount->m_references; if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; } }
        m_pCount = 0; m_pObject = object;
        if (object) { m_pCount = new SCountBlock; m_pCount->m_references = 1; m_pCount->m_pfnDestroy = DestroyMode; m_pCount->m_pObject = object; }
    }
    CCountedPointer& operator=(CMode* o) { Reset(o); return *this; }
    ~CCountedPointer() { if (m_pCount && --m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; } }
};

static const int g_cameraDescriptor = 5;
static SCountBlock* g_kept = 0;
static bool g_keep = true;

struct CManager {
    int m_pTarget;
    void SetCameraMode(CCountedPointer mode) { if (g_keep) { g_kept = mode.m_pCount; ++g_kept->m_references; } }
    void OnCreate()
    {
        CCountedPointer mode;
        CMode* m = new CMode; m->m_manager = this; m->m_desc = &g_cameraDescriptor; m->m_target = &m_pTarget;
        mode = m;
        SetCameraMode(mode);
    }
};

int main()
{
    CManager mgr; mgr.m_pTarget = 3;
    mgr.OnCreate();
    bool ok = g_kept && g_kept->m_references == 1 && g_destroyed == 0
        && ((CMode*)g_kept->m_pObject)->m_manager == &mgr && ((CMode*)g_kept->m_pObject)->m_desc == &g_cameraDescriptor;
    if (g_kept) { g_kept->m_pfnDestroy(g_kept->m_pObject); delete g_kept; }
    g_keep = false; g_destroyed = 0;
    mgr.OnCreate();
    ok = ok && g_destroyed == 1;
    if (!ok) { std::printf("0069e3d0_TEST FAIL\n"); return 1; }
    std::printf("0069e3d0_TEST PASS\n");
    return 0;
}
