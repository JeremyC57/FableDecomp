// Behaviour test for 007509d0: the model below is a copy of src/007509d0.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
static int g_ctors = 0, g_dtors = 0, g_sets = 0, g_gets = 0;
class CBaseObject { public:};
#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    void* PData;
    CBaseIntelligentPointer() : PData(0) { ++g_ctors; }
    ~CBaseIntelligentPointer() { ++g_dtors; }
    CBaseObject* GetPItem() const { ++g_gets; return (CBaseObject*)PData; }
    void SetPItem(CBaseObject* item) { ++g_sets; PData = item; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(CBaseObject* item = 0) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { CBaseObject* item = other.GetPItem(); SetPItem(item); return *this; }
};
class CTCDiscovery { public:
    unsigned char       _base[0x8];
    bool                Discovered;
    CIntelligentPointer Discoverer;
    void SetDiscovered(bool discovered, CBaseObject* discoverer);
};
#pragma pack(pop)

void CTCDiscovery::SetDiscovered(bool discovered, CBaseObject* discoverer)
{
    if (!discovered && discoverer != 0)
        discoverer = 0;
    Discovered = discovered;
    Discoverer = CIntelligentPointer(discoverer);
}

static bool Run(bool discovered, bool withThing, bool expectKept)
{
    static CBaseObject thing, old;
    CTCDiscovery d; std::memset(d._base, 0, sizeof(d._base)); d.Discovered = !discovered; d.Discoverer.PData = &old;
    g_ctors = g_dtors = g_sets = g_gets = 0;
    d.SetDiscovered(discovered, withThing ? &thing : 0);
    if (d.Discovered != discovered) return false;
    if (d.Discoverer.PData != (expectKept ? (void*)&thing : 0)) return false;
    if (g_ctors != 1 || g_dtors != 1 || g_sets != 2 || g_gets != 1) return false;   // one temporary, built then copied
    return true;
}
int main()
{
    bool ok = Run(true, true, true) && Run(true, false, false) && Run(false, true, false) && Run(false, false, false);
    if (!ok) { std::printf("007509d0_TEST FAIL\n"); return 1; }
    std::printf("007509d0_TEST PASS\n");
    return 0;
}