// Behaviour test for 0x0055D590 ~CTCPhotojournalCapture: the component leaves the global TC list, then the component vector
// block is freed and FileName / Description are destroyed, then ~CTCBase.
#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_log[64];
static void Log(const char* s) { std::strcat(g_log, s); }
class CTCBase { public: CTCBase() : IsInGlobalUpdateTCs(true) { } virtual ~CTCBase() { Log("B"); } void RemoveFromGlobalTCs() { IsInGlobalUpdateTCs = false; Log("R"); } bool IsInGlobalUpdateTCs; };
class CWideString { public: CWideString(const char* t) : Tag(t) { } ~CWideString() { Log(Tag); } const char* Tag; };
class CVector { public: CVector() : Start(0) { } ~CVector() { if (Start) { std::free(Start); Log("V"); } } void** Start; };
class CTCPhotojournalCapture : public CTCBase {
public:
    CTCPhotojournalCapture() : Description("D"), FileName("F") { }
    virtual ~CTCPhotojournalCapture() { RemoveFromGlobalTCs(); }
    CWideString Description;
    bool        DiscardScreenShot;
    long        FrameCounter;
    double      LastRenderTime;
    CWideString FileName;
    CVector     Components;
};
int main()
{
    g_log[0] = 0;
    CTCPhotojournalCapture* capture = new CTCPhotojournalCapture;
    capture->Components.Start = (void**)std::malloc(4 * sizeof(void*));
    CTCBase* base = capture;
    delete base;
    if (std::strcmp(g_log, "RVFDB") != 0) { std::printf("0055d590_TEST FAIL\n"); return 1; }
    std::printf("0055d590_TEST PASS\n");
    return 0;
}