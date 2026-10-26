// Behaviour test for 0x007B6610 CTCCrateStack::CTCCrateStack: the base gets the thing, both intelligent pointers are
// constructed empty and CrateDefIndex starts at zero.
#include <cstdio>
#include <cstring>
#include <new>
class CThing { public: long m_id; };

static int g_baseCtor;
struct CTCBase_Calls {
    virtual void v00() { }
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    CTCBase_Calls(CThing* thing) : ParentThing(thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { ++g_baseCtor; }
};

static double g_storage[64];
static void* Fresh() { std::memset(g_storage, 0xAB, sizeof g_storage); return g_storage; }
class CBaseObject;
static int g_ipCtor;
struct CBaseIntelligentPointer {
    virtual void v00() { }
    CBaseObject* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_ipCtor; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};
class CTCCrateStack : public CTCBase_Calls { public:
    CIntelligentPointer FirstCrate;
    CIntelligentPointer SecondCrate;
    long CrateDefIndex;
    CTCCrateStack(CThing* thing) : CTCBase_Calls(thing), CrateDefIndex(0) { }
};

int main()
{
    CThing thing;
    CTCCrateStack* p = new (Fresh()) CTCCrateStack(&thing);
    if (g_baseCtor != 1 || g_ipCtor != 2 || p->ParentThing != &thing || p->FirstCrate.m_pItem != 0 || p->SecondCrate.m_pItem != 0 || p->CrateDefIndex != 0) { std::printf("007b6610_TEST FAIL\n"); return 1; }
    std::printf("007b6610_TEST PASS\n");
    return 0;
}