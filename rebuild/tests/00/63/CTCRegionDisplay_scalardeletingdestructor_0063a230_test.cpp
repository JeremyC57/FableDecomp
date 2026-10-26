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
class CWideString { public: int m_tag; CWideString() : m_tag(0) {} ~CWideString() { Log(m_tag); } };
class CTCRegionDisplay : public CTCBase { public:
    CWideString CurrentAreaName; CWideString CurrentAreaNameExtra;
    long FrameNameChanged, CurrentAlpha, LastAlpha; float DisplayTime; bool TimeOut, InfoDisplay, UpdateWhilePaused;
    virtual ~CTCRegionDisplay() { RemoveFromGlobalTCs(); }
};
CTCRegionDisplay* NewCTCRegionDisplay() { return new CTCRegionDisplay; }
int main()
{
    int bad = 0;
    CTCRegionDisplay* r = NewCTCRegionDisplay();
    r->CurrentAreaName.m_tag = 12; r->CurrentAreaNameExtra.m_tag = 16;
    CTCBase* p = r;
    delete p;
    static const int want[] = { 1, 16, 12, 100 };
    if (!Expect(want, 4)) bad = 1;
    if (bad) { std::printf("0063a230_TEST FAIL\n"); return 1; }
    std::printf("0063a230_TEST PASS\n");
    return 0;
}