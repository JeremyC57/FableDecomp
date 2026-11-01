// Behaviour test for 0x007AFF70 CTCBoss::~CTCBoss (implicit): Effects then HelperCreatures (each element's virtual dtor, block
// freed), then the PBossDef release (refcount--, Delete() on the last reference, pointer zeroed), then ~CTCBase.
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
class CTCBase { public: void* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs; virtual ~CTCBase() { Log(9999); } };
class CTCBoss : public CTCBase { public:
    CDefPointer PBossDef; long GenericCounter, StrikesSinceHit, currentBossPhase, NextWeapon; bool BCountStrikes, EnablePhaseChanges;
    float PhaseStartPos[3]; PtrVector HelperCreatures; PtrVector Effects;
};
static void Fill(PtrVector& v, int base, int n, int tag)
{
    v.Tag = tag; v.Start = (CIntelligentPointer*)::operator new(sizeof(CIntelligentPointer) * n);
    for (int i = 0; i < n; ++i) { new (&v.Start[i]) CIntelligentPointer; v.Start[i].Id = base + i; }
    v.Finish = v.Start + n;
}
int main()
{
    CDefPointee def; def.IVRefCount = 2;
    {
        CTCBoss b; b.PBossDef.Object = &def; Fill(b.HelperCreatures, 10, 2, 701); Fill(b.Effects, 20, 1, 702);
    }
    if (def.IVRefCount != 1) { std::printf("007aff70_TEST FAIL\n"); return 1; }
    {
        CTCBoss b; b.PBossDef.Object = &def; Fill(b.HelperCreatures, 30, 1, 703); b.Effects.Start = b.Effects.Finish = 0; b.Effects.Tag = 704;
    }
    static const int expect[] = { 20, 702, 10, 11, 701, 9999, 30, 703, 300, 9999 };
    if (g_n != 10 || def.IVRefCount != 0) { std::printf("007aff70_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 10; ++i) if (g_log[i] != expect[i]) { std::printf("007aff70_TEST FAIL\n"); return 1; }
    std::printf("007aff70_TEST PASS\n");
    return 0;
}