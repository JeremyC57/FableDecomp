#include <cstdio>
#include <cstring>
#include <new>

struct CBase { virtual long GetMode() { return m_mode; } long m_mode; unsigned char m_camera[0x38]; };
struct SCountBlock { long m_references; void (*m_pfnDestroy)(void*); void* m_pObject; };
static int g_destroyed = 0; static void DestroyBase(void*) { ++g_destroyed; }
template <class T> struct CCountedPointer {
    T* m_pObject; SCountBlock* m_pCount;
    CCountedPointer(T* o, SCountBlock* c) : m_pObject(o), m_pCount(c) {}
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount) { if (m_pCount) ++m_pCount->m_references; }
    ~CCountedPointer() { if (m_pCount) { if (--m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; } } }
};
struct CDefPointeeBase { virtual void v00() {} virtual void Destroy() { ++m_destroyed; } long m_refCount; int m_destroyed; CDefPointeeBase() : m_refCount(0), m_destroyed(0) {} };
struct CDefPointer {
    CDefPointeeBase* m_pObject;
    void Release() { CDefPointeeBase* old = m_pObject; if (old) { if (--old->m_refCount == 0) old->Destroy(); m_pObject = 0; } }
    CDefPointer& operator=(const CDefPointer& other) {
        if (m_pObject != other.m_pObject) { Release(); m_pObject = other.m_pObject; if (m_pObject) ++m_pObject->m_refCount; }
        return *this; }
};

static int g_switch, g_data, g_push;
struct CGameCameraManager {
    CDefPointer PDef;
    void PopulateSwitchList() { ++g_switch; } void PopulateListFromData() { ++g_data; } void PushCurrentManualMode() { ++g_push; }
    void SetCameraManagerDef(const CDefPointer& def) { PDef = def; PopulateSwitchList(); PopulateListFromData(); PushCurrentManualMode(); }
};
int main()
{
    CDefPointeeBase a, b; a.m_refCount = 1; b.m_refCount = 1;
    CGameCameraManager mgr; mgr.PDef.m_pObject = &a; CDefPointer arg; arg.m_pObject = &b;
    mgr.SetCameraManagerDef(arg);
    if (mgr.PDef.m_pObject != &b || b.m_refCount != 2 || a.m_refCount != 0 || a.m_destroyed != 1 || g_switch != 1 || g_data != 1 || g_push != 1) { std::printf("0069a850_TEST FAIL\n"); return 1; }
    mgr.SetCameraManagerDef(arg);                                   // same pointee: no count change
    if (b.m_refCount != 2 || b.m_destroyed != 0 || g_switch != 2) { std::printf("0069a850_TEST FAIL\n"); return 1; }
    CDefPointer null; null.m_pObject = 0; mgr.SetCameraManagerDef(null);
    if (mgr.PDef.m_pObject != 0 || b.m_refCount != 1 || b.m_destroyed != 0 || g_push != 3) { std::printf("0069a850_TEST FAIL\n"); return 1; }
    std::printf("0069a850_TEST PASS\n");
    return 0;
}
