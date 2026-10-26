#include <cstdio>
static int g_log[16]; static int g_n = 0;
static void Log(int v) { if (g_n < 16) g_log[g_n++] = v; }
static bool Expect(const int* want, int n) { int i; if (g_n != n) return false; for (i = 0; i < n; ++i) if (g_log[i] != want[i]) return false; return true; }
namespace NUISystem {
struct CComponentRoot { virtual ~CComponentRoot() { Log(300); } };
struct CComponentObserver { virtual void o000() {} unsigned char _data[0x10]; };
struct CComponentListener { virtual void l000() {} };
class CComponent : public CComponentRoot, public CComponentObserver, public CComponentListener { public:
    CComponent() {}
    virtual ~CComponent() { Log(200); }
};
struct CStateList { int m_tag; CStateList() : m_tag(7) {} ~CStateList() { Log(m_tag); } };
class CChangingStateComponent : public CComponent { public:
    long StatesBeingDone; long StatesDone; CStateList StatesToDo;
    CChangingStateComponent() {}
    virtual ~CChangingStateComponent() {}
};
}
NUISystem::CChangingStateComponent* NewChangingStateComponent() { return new NUISystem::CChangingStateComponent; }
int main()
{
    int bad = 0;
    NUISystem::CComponentRoot* p = NewChangingStateComponent();
    delete p;
    static const int want[] = { 7, 200, 300 };
    if (!Expect(want, 3)) bad = 1;
    if (bad) { std::printf("0052d770_TEST FAIL\n"); return 1; }
    std::printf("0052d770_TEST PASS\n");
    return 0;
}