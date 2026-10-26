// Behaviour test for CTCCreatureGeneratorCreator::OnInitialActivate (0x007C9920): a saved UID is resolved into PGenerator
// and cleared together with Generated; if the generator is alive it is killed (not immediately) and the pointer cleared.
#include <cstdio>
#ifdef _MSC_VER
typedef unsigned __int64 u64; typedef __int64 s64;
#else
typedef unsigned long long u64; typedef long long s64;
#endif
static int g_temps = 0, g_tempDtors = 0;
class CThing { public:
    bool Dead; int Kills; bool KillFlag;
    void Kill(bool immediately) { ++Kills; KillFlag = immediately; }
};
static CThing* g_lookup = 0; static s64 g_askedUID = 0;
class CThingManager { public:
    CThing* GetThingWithUID(s64 uid) { g_askedUID = uid; return g_lookup; }
};
static CThingManager g_tm;
struct CBaseIntelligentPointer {
    virtual void v00() { }
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_temps; }
    ~CBaseIntelligentPointer() { ++g_tempDtors; }
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { m_pItem = item; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00() { }
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
class CTCBase { public:
    virtual void v00() { }
    CThing* ParentThing;
    CThingManager* GetThingManager() { return &g_tm; }
};
struct CTCCreatureGeneratorCreator : CTCBase {
    bool                Generated;
    CIntelligentPointer PGenerator;
    u64                 GeneratorUID;
    CTCCreatureGeneratorCreator() : PGenerator(0) { }
    bool IsGeneratorAlive() { return PGenerator.GetPItem() && !PGenerator.GetPItem()->Dead; }
    void OnInitialActivate();
};

void CTCCreatureGeneratorCreator::OnInitialActivate()
{
    if (GeneratorUID > 0) {
        PGenerator = CIntelligentPointer(GetThingManager()->GetThingWithUID(GeneratorUID));
        GeneratorUID = 0;
        Generated = false;
        if (IsGeneratorAlive()) {
            PGenerator.GetPItem()->Kill(false);
            PGenerator = CIntelligentPointer(0);
        }
    }
}

int main()
{
    int bad = 0;
    CThing gen; gen.Dead = true; gen.Kills = 0; gen.KillFlag = true;
    CTCCreatureGeneratorCreator c; c.Generated = true; c.GeneratorUID = 0;
    int t0 = g_temps;
    c.OnInitialActivate();                           // no UID: untouched
    if (g_temps != t0 || !c.Generated || c.PGenerator.GetPItem()) bad |= 1;
    c.GeneratorUID = ((u64)1 << 32) | 5; g_lookup = &gen;
    c.OnInitialActivate();                           // dead generator: kept, not killed
    if (g_askedUID != (s64)(((u64)1 << 32) | 5) || c.PGenerator.GetPItem() != &gen || c.GeneratorUID || c.Generated || gen.Kills) bad |= 2;
    if (g_temps - t0 != 1 || g_tempDtors != g_temps - 1) bad |= 4;
    gen.Dead = false; c.GeneratorUID = 9; c.Generated = true;
    c.OnInitialActivate();                           // alive: killed and cleared
    if (gen.Kills != 1 || gen.KillFlag || c.PGenerator.GetPItem() || c.Generated || g_askedUID != 9) bad |= 8;
    if (bad) { std::printf("007c9920_TEST FAIL\n"); return 1; }
    std::printf("007c9920_TEST PASS\n");
    return 0;
}