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

static bool g_interruptable, g_inMelee; static int g_added, g_removed;
class CGameCameraManager { public:
    CCountedPointer<CBase> CurrentMode; CCountedPointer<CBase> PendingMode; bool ObservedThingInMelee;
    CGameCameraManager() : CurrentMode(0, 0), PendingMode(0, 0), ObservedThingInMelee(false) {}
    static bool IsCameraModeInterruptable(CCountedPointer<CBase>) { return g_interruptable; }
    bool IsObservedThingInMelee() { return g_inMelee; }
    void AddMeleeCameraMode() { ++g_added; }
    void RemoveCombatCameraMode() { ++g_removed; }
    void UpdateObservedThingInMelee() {
        if (!IsCameraModeInterruptable(CurrentMode) && PendingMode.m_pObject == 0) return;
        if (IsObservedThingInMelee()) { if (!ObservedThingInMelee) AddMeleeCameraMode(); }
        else { if (ObservedThingInMelee) RemoveCombatCameraMode(); }
    }
};
int main()
{
    CGameCameraManager mgr; CBase pending;
    g_interruptable = false; g_inMelee = true; mgr.UpdateObservedThingInMelee();
    if (g_added != 0 || g_removed != 0) { std::printf("0069ac30_TEST FAIL\n"); return 1; }                                  // blocked: not interruptable, nothing pending
    mgr.PendingMode.m_pObject = &pending; mgr.UpdateObservedThingInMelee();
    if (g_added != 1 || g_removed != 0) { std::printf("0069ac30_TEST FAIL\n"); return 1; }                                  // pending mode lets it through: add
    mgr.PendingMode.m_pObject = 0; g_interruptable = true; mgr.ObservedThingInMelee = true; mgr.UpdateObservedThingInMelee();
    if (g_added != 1 || g_removed != 0) { std::printf("0069ac30_TEST FAIL\n"); return 1; }                                  // already in melee: nothing
    g_inMelee = false; mgr.UpdateObservedThingInMelee();
    if (g_added != 1 || g_removed != 1) { std::printf("0069ac30_TEST FAIL\n"); return 1; }                                  // left melee: remove
    mgr.ObservedThingInMelee = false; mgr.UpdateObservedThingInMelee();
    if (g_added != 1 || g_removed != 1) { std::printf("0069ac30_TEST FAIL\n"); return 1; }
    std::printf("0069ac30_TEST PASS\n");
    return 0;
}