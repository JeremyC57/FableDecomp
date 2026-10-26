#include <cstdio>
static int g_log[16]; static int g_n = 0;
static void Log(int v) { if (g_n < 16) g_log[g_n++] = v; }
static bool Expect(const int* want, int n) { int i; if (g_n != n) return false; for (i = 0; i < n; ++i) if (g_log[i] != want[i]) return false; return true; }
class CThing;
class CTCBase { public:
    CTCBase() : ParentThing(0), ToBeRemoved(false), IsInGlobalUpdateTCs(true) {}
    virtual ~CTCBase() { Log(100); }
    CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs;
    void RemoveFromGlobalTCs() { IsInGlobalUpdateTCs = false; Log(1); }
};
struct CBaseIntelligentPointer {
    virtual void v00() {}
    void* PItem; int m_tag;
    CBaseIntelligentPointer() : PItem(0), m_tag(0) {}
    ~CBaseIntelligentPointer() { Log(m_tag); }
};
struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00() {} };
class C3DVector { public: float X, Y, Z; };
struct CTCHighlightItem : CTCBase {
    bool StartRunning; bool Running; long LifeTimer; C3DVector StartPos; CIntelligentPointer PParticleEmitter;
    void DoStopRunning() { Running = false; Log(2); }
    virtual ~CTCHighlightItem() { DoStopRunning(); }
};
CTCHighlightItem* NewCTCHighlightItem() { return new CTCHighlightItem; }
int main()
{
    int bad = 0;
    CTCHighlightItem* h = NewCTCHighlightItem();
    h->PParticleEmitter.m_tag = 32; h->Running = true;
    CTCBase* p = h;
    delete p;
    static const int want[] = { 2, 32, 100 };
    if (!Expect(want, 3)) bad = 1;
    if (bad) { std::printf("0077d140_TEST FAIL\n"); return 1; }
    std::printf("0077d140_TEST PASS\n");
    return 0;
}