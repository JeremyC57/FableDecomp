#include <cstdio>
static int g_log[16]; static int g_n = 0;
static void Log(int v) { if (g_n < 16) g_log[g_n++] = v; }
static bool Expect(const int* want, int n) { int i; if (g_n != n) return false; for (i = 0; i < n; ++i) if (g_log[i] != want[i]) return false; return true; }
struct CBaseIntelligentPointer {
    virtual void v00() {}
    void* PItem; int m_tag;
    CBaseIntelligentPointer() : PItem(0), m_tag(0) {}
    ~CBaseIntelligentPointer() { Log(m_tag); }
};
struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00() {} };
namespace NCameraModes {
struct CCameraModeBase {
    CCameraModeBase() {}
    virtual ~CCameraModeBase() { Log(200); }
};
struct CCameraPointModeBase : CCameraModeBase {
    CIntelligentPointer PCameraPoint; float FOV; bool CutInto; bool CutOutOf;
    virtual ~CCameraPointModeBase() {}
};
}
NCameraModes::CCameraPointModeBase* NewCameraPointModeBase() { return new NCameraModes::CCameraPointModeBase; }
int main()
{
    int bad = 0;
    NCameraModes::CCameraPointModeBase* c = NewCameraPointModeBase();
    c->PCameraPoint.m_tag = 5;
    NCameraModes::CCameraModeBase* p = c;
    delete p;
    static const int want[] = { 5, 200 };
    if (!Expect(want, 2)) bad = 1;
    if (bad) { std::printf("0056e2f0_TEST FAIL\n"); return 1; }
    std::printf("0056e2f0_TEST PASS\n");
    return 0;
}