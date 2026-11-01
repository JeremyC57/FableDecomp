// Behaviour test for 0x007C5660 CTCGhostSword::~CTCGhostSword (empty user body): PItemRemoved, CurFadeInTimer (block only),
// MinionEffects, Minions (virtual element dtors + block), the PDef release, then ~CTCBase.
#include <cstdio>
#include <cstddef>
static int g_log[64]; static int g_n = 0;
static void Log(int v) { if (g_n < 64) g_log[g_n++] = v; }
struct CDefPointee { virtual ~CDefPointee() { } virtual void Delete() const { Log(300); } unsigned long IVRefCount; };
struct CDefPointer {
    CDefPointee* Object;
    ~CDefPointer() { if (Object) { CDefPointee* o = Object; if (--o->IVRefCount == 0) o->Delete(); Object = 0; } }
};
struct CIntelligentPointer {
    void* PData; int Id;
    static void* operator new(std::size_t, void* p) { return p; }
    static void operator delete(void*, void*) { }
    static void operator delete(void* p) { ::operator delete(p); }
    virtual ~CIntelligentPointer() { Log(Id); }
};
struct PtrVector {
    CIntelligentPointer* Start; CIntelligentPointer* Finish; int Tag;
    ~PtrVector() { for (CIntelligentPointer* p = Start; p != Finish; ++p) p->~CIntelligentPointer(); if (Start) { Log(Tag); ::operator delete(Start); } }
};
struct LongVector { long* Start; long* Finish; ~LongVector() { if (Start) { Log(705); delete [] Start; } } };
class CTCBase { public: void* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs; virtual ~CTCBase() { Log(9999); } };
class CTCGhostSword : public CTCBase { public:
    CDefPointer PDef; PtrVector Minions; PtrVector MinionEffects; long MinionAliveTimer; long FadeInTimer; LongVector CurFadeInTimer;
    long SpellLevel; CIntelligentPointer PItemRemoved; bool CutSceneCancelled;
    virtual ~CTCGhostSword() { }
};
static void Fill(PtrVector& v, int base, int n, int tag)
{
    v.Tag = tag; v.Start = (CIntelligentPointer*)::operator new(sizeof(CIntelligentPointer) * n);
    for (int i = 0; i < n; ++i) { new (&v.Start[i]) CIntelligentPointer; v.Start[i].Id = base + i; }
    v.Finish = v.Start + n;
}
int main()
{
    CDefPointee def; def.IVRefCount = 1;
    {
        CTCGhostSword g; g.PDef.Object = &def; Fill(g.Minions, 10, 2, 701); Fill(g.MinionEffects, 20, 1, 702);
        g.CurFadeInTimer.Start = new long[3]; g.CurFadeInTimer.Finish = g.CurFadeInTimer.Start + 3; g.PItemRemoved.Id = 40;
    }
    static const int expect[] = { 40, 705, 20, 702, 10, 11, 701, 300, 9999 };
    if (g_n != 9 || def.IVRefCount != 0) { std::printf("007c5660_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 9; ++i) if (g_log[i] != expect[i]) { std::printf("007c5660_TEST FAIL\n"); return 1; }
    std::printf("007c5660_TEST PASS\n");
    return 0;
}