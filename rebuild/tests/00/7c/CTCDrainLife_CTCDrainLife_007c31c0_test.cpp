// Behaviour test for 0x007C31C0 ~CTCDrainLife (empty body): each Shots element destroyed in order and the block freed, then
// PItemRemoved, then ~CTCBase.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
static char g_log[32];
static void Log(const char* s) { std::strcat(g_log, s); }
static int g_frees = 0;
class CIntelligentPointer { public: CIntelligentPointer(char c) : C(c) { } virtual ~CIntelligentPointer() { char b[2] = { C, 0 }; Log(b); } char C; };
class CShotVector {                                         // STLport 4.6 vector: _Destroy loop, then _Vector_base frees the block
public:
    CShotVector() : Start(0), Finish(0), End(0) { }
    ~CShotVector() { for (CIntelligentPointer* p = Start; p != Finish; ++p) p->~CIntelligentPointer(); if (Start) { std::free(Start); ++g_frees; } }
    void Fill(const char* s) { int n = (int)std::strlen(s); Start = (CIntelligentPointer*)std::malloc(n * sizeof(CIntelligentPointer)); for (int i = 0; i < n; ++i) new (Start + i) CIntelligentPointer(s[i]); Finish = End = Start + n; }
    CIntelligentPointer* Start; CIntelligentPointer* Finish; CIntelligentPointer* End;
};
class CTCBase { public: virtual ~CTCBase() { Log("B"); } };
class CTCDrainLife : public CTCBase {
public:
    CTCDrainLife() : PItemRemoved('R') { }
    virtual ~CTCDrainLife() { }
    const void* PDef;
    long SpellLevel;
    CIntelligentPointer PItemRemoved;
    CShotVector Shots;
    bool JustReleased;
    long SpellDelayFrames;
};
int main()
{
    g_log[0] = 0;
    CTCDrainLife* d = new CTCDrainLife;
    d->Shots.Fill("abc");
    delete (CTCBase*)d;
    bool ok = std::strcmp(g_log, "abcRB") == 0 && g_frees == 1;
    g_log[0] = 0;
    delete (CTCBase*)new CTCDrainLife;
    ok = ok && std::strcmp(g_log, "RB") == 0 && g_frees == 1;
    std::printf(ok ? "007c31c0_TEST PASS\n" : "007c31c0_TEST FAIL\n");
    return ok ? 0 : 1;
}