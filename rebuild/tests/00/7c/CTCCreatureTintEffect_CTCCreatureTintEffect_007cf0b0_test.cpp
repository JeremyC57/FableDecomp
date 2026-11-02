// Behaviour test for 0x007CF0B0 CTCCreatureTintEffect::~CTCCreatureTintEffect: the TintList elements are destroyed in place
// (each intelligent pointer's virtual dtor), the vector block freed, then the tint def pointer released, then the base.
#include <cstdio>
#include <cstdlib>
#include <new>
static char g_log[32];
static int g_n = 0;
static void Log(char c) { if (g_n < 31) g_log[g_n++] = c; g_log[g_n] = 0; }

struct CDef { unsigned long Ref; void Delete() { Log('D'); } };
struct CDefPointer {
    CDef* Object;
    ~CDefPointer() { if (Object) { CDef* o = Object; if (--o->Ref == 0) o->Delete(); Object = 0; } }
};
struct CIntelligentPointer { void* PData; virtual ~CIntelligentPointer() { Log('P'); } };
struct CVector {
    CIntelligentPointer* Start; CIntelligentPointer* Finish; CIntelligentPointer* End;
    ~CVector()
    {
        for (CIntelligentPointer* p = Start; p != Finish; ++p) p->~CIntelligentPointer();
        if (Start) { Log('F'); std::free(Start); }
    }
};
class CTCBase { public: virtual ~CTCBase() { Log('B'); } };
class CTCCreatureTintEffect : public CTCBase { public:
    CDefPointer PDef;
    long CurTime;
    CVector TintList;
    long Color[4];
    virtual ~CTCCreatureTintEffect() {}
};

int main()
{
    CDef def; def.Ref = 1;
    CTCCreatureTintEffect* e = new CTCCreatureTintEffect;
    e->PDef.Object = &def;
    void* block = std::malloc(3 * sizeof(CIntelligentPointer));
    CIntelligentPointer* items = (CIntelligentPointer*)block;
    for (int i = 0; i < 3; ++i) new (&items[i]) CIntelligentPointer();
    e->TintList.Start = items; e->TintList.Finish = items + 3; e->TintList.End = items + 3;
    delete (CTCBase*)e;
    const char* want = "PPPFDB";
    for (int i = 0; want[i] || g_log[i]; ++i)
        if (want[i] != g_log[i]) { std::printf("007cf0b0_TEST FAIL\n"); return 1; }
    std::printf("007cf0b0_TEST PASS\n");
    return 0;
}