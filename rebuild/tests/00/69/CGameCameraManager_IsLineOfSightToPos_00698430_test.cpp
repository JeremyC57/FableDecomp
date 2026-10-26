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

class CThing { public: int x; };
static const void* g_cam; static const void* g_pos; static long g_filter; static bool g_flag; static bool g_result;
struct CTools {
    unsigned char _body[0x20];
    bool IsLineOfSightFiltered(const void* camera, const void* pos, void* hitPos, void* hitNormal, long* hitIndex, const CThing*& hitThing, long filter, bool flag)
    { g_cam = camera; g_pos = pos; g_filter = filter; g_flag = flag; *hitIndex = 5; hitThing = 0; std::memset(hitPos, 0, 12); std::memset(hitNormal, 0, 12); return g_result; }
};
class CGameCameraManager { public:
    CTools CamTools; CCountedPointer<CBase> CurrentMode;
    CGameCameraManager(CBase* m) : CurrentMode(m, 0) {}
    bool IsLineOfSightToPos(const void* pos) {
        CBase* mode = CurrentMode.m_pObject;
        unsigned char hitPos[0xc]; unsigned char hitNormal[0xc]; long hitIndex; const CThing* hitThing;
        return CamTools.IsLineOfSightFiltered(mode->m_camera, pos, hitPos, hitNormal, &hitIndex, hitThing, 7, true); }
};
int main()
{
    CBase mode; CGameCameraManager mgr(&mode); float pos[3] = { 1.0f, 2.0f, 3.0f };
    g_result = true;
    if (!mgr.IsLineOfSightToPos(pos) || g_cam != (const void*)mode.m_camera || g_pos != (const void*)pos || g_filter != 7 || !g_flag) { std::printf("00698430_TEST FAIL\n"); return 1; }
    g_result = false;
    if (mgr.IsLineOfSightToPos(pos)) { std::printf("00698430_TEST FAIL\n"); return 1; }
    std::printf("00698430_TEST PASS\n");
    return 0;
}