// Behaviour test for 0055de40: a copy of the source's model and logic with its out-of-line callees stubbed.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
static int g_fail = 0;
static void Check(bool ok, int line)
{
    if (!ok) {
        g_fail = 1;
        std::printf("check failed at line %d\n", line);
    }
}
#define CHECK(c) Check((c) ? true : false, __LINE__)

static int g_removed = 0, g_wideDtors = 0, g_baseDtors = 0, g_vectorFrees = 0, g_step = 0;
static int g_removeStep = 0, g_vecStep = 0, g_fileStep = 0, g_descStep = 0, g_baseStep = 0;
class CThing;
class CTCBase { public:
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    CTCBase() : ParentThing(0), ToBeRemoved(false), IsInGlobalUpdateTCs(true) { }
    virtual ~CTCBase() { ++g_baseDtors; g_baseStep = ++g_step; }   // stub for 0x00686830
    void RemoveFromGlobalTCs() { ++g_removed; IsInGlobalUpdateTCs = false; g_removeStep = ++g_step; }   // stub for 0x00686DF0
};
class CWideString { public:
    int* Step;
    CWideString() : Step(0) { }
    ~CWideString() { ++g_wideDtors; if (Step) *Step = ++g_step; }
};
struct CComponentVector {
    void** _M_start;
    void** _M_finish;
    void** _M_end_of_storage;
    CComponentVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CComponentVector()
    {
        if (_M_start != 0) {
            std::free(_M_start);
            ++g_vectorFrees;
        }
        g_vecStep = ++g_step;
    }
};
class CTCPhotojournalCapture : public CTCBase { public:
    CWideString      Description;
    bool             DiscardScreenShot;
    long             FrameCounter;
    bool             FirstRenderFrame;
    bool             ScreenOpen;
    double           LastRenderTime;
    CWideString      FileName;
    CComponentVector Components;
    CThing*          PMannequin;
    bool             WasPlayerGuiOpen;
    long             TakeScreenshotAfterFrames;
    virtual ~CTCPhotojournalCapture() { RemoveFromGlobalTCs(); }
};
static void RunTest()
{
    CTCPhotojournalCapture* p = new CTCPhotojournalCapture;
    p->Components._M_start = (void**)std::malloc(16);
    p->FileName.Step = &g_fileStep;
    p->Description.Step = &g_descStep;
    CTCBase* base = p;
    delete base;
    CHECK(g_removed == 1 && g_wideDtors == 2 && g_vectorFrees == 1 && g_baseDtors == 1);
    // the removal first, then members in reverse order (Components, FileName, Description), then the base
    CHECK(g_removeStep == 1 && g_vecStep == 2 && g_fileStep == 3 && g_descStep == 4 && g_baseStep == 5);
}

int main()
{
    RunTest();
    if (g_fail) {
        std::printf("0055de40_TEST FAIL\n");
        return 1;
    }
    std::printf("0055de40_TEST PASS\n");
    return 0;
}