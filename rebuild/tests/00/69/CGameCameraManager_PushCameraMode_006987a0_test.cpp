#include <cstdio>
#include <cstring>
#include <new>

struct CBase { virtual long GetMode() { return m_mode; } long m_mode; unsigned char m_camera[0x38]; };
struct SCountBlock { long m_references; void (*m_pfnDestroy)(void*); void* m_pObject; };
static int g_destroyed = 0; static void DestroyBase(void*) { ++g_destroyed; }
template <class T> class CCountedPointer { public:
    T* m_pObject; SCountBlock* m_pCount;
    CCountedPointer(T* o, SCountBlock* c) : m_pObject(o), m_pCount(c) {}
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount) { if (m_pCount) ++m_pCount->m_references; }
    ~CCountedPointer() { if (m_pCount) { if (--m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; } } }
};
class CDefPointeeBase { public: virtual void v00() {} virtual void Destroy() { ++m_destroyed; } long m_refCount; int m_destroyed; CDefPointeeBase() : m_refCount(0), m_destroyed(0) {} };
struct CDefPointer {
    CDefPointeeBase* m_pObject;
    void Release() { CDefPointeeBase* old = m_pObject; if (old) { if (--old->m_refCount == 0) old->Destroy(); m_pObject = 0; } }
    CDefPointer& operator=(const CDefPointer& other) {
        if (m_pObject != other.m_pObject) { Release(); m_pObject = other.m_pObject; if (m_pObject) ++m_pObject->m_refCount; }
        return *this; }
};

static CBase* g_seen = 0; static long g_seenRefs = 0;
class CGameCameraManager { public:
    CCountedPointer<CBase> CurrentMode; bool UsingCameraManually;
    CGameCameraManager() : CurrentMode(0, 0), UsingCameraManually(false) {}
    void SetCameraMode(CCountedPointer<CBase> mode) { g_seen = mode.m_pObject; g_seenRefs = mode.m_pCount->m_references; }
    void PushCameraMode(CCountedPointer<CBase> mode) { SetCameraMode(mode); UsingCameraManually = true; }
};
int main()
{
    CBase mode; SCountBlock* block = new SCountBlock; block->m_references = 1; block->m_pfnDestroy = DestroyBase; block->m_pObject = &mode;
    CGameCameraManager mgr;
    {
        CCountedPointer<CBase> holder(&mode, block);          // refs = 1 (held here)
        mgr.PushCameraMode(holder);                           // copy for the parameter (2), copy for SetCameraMode (3), both destroyed
        if (g_seen != &mode || g_seenRefs != 3 || !mgr.UsingCameraManually || block->m_references != 1 || g_destroyed) { std::printf("006987a0_TEST FAIL\n"); return 1; }
    }
    if (g_destroyed != 1) { std::printf("006987a0_TEST FAIL\n"); return 1; }                              // the last holder destroyed the mode and freed the block
    std::printf("006987a0_TEST PASS\n");
    return 0;
}