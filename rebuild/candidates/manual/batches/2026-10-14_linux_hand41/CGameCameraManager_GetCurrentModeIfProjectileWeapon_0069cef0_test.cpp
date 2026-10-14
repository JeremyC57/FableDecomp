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

struct CGameCameraManager {
    CCountedPointer<CBase> CurrentMode;
    CGameCameraManager(CBase* m) : CurrentMode(m, 0) {}
    bool GetCurrentModeIfProjectileWeapon(CBase** outMode) {
        if (CurrentMode.m_pObject->GetMode() == 15) { *outMode = CurrentMode.m_pObject; return true; }
        return false; }
};
int main()
{
    CBase mode; mode.m_mode = 15; CGameCameraManager mgr(&mode); CBase* out = 0;
    if (!mgr.GetCurrentModeIfProjectileWeapon(&out) || out != &mode) { std::printf("0069cef0_TEST FAIL\n"); return 1; }
    mode.m_mode = 3; out = 0;
    if (mgr.GetCurrentModeIfProjectileWeapon(&out) || out != 0) { std::printf("0069cef0_TEST FAIL\n"); return 1; }
    std::printf("0069cef0_TEST PASS\n");
    return 0;
}
