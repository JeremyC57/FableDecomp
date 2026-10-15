// Behaviour test for 0x004D975C CTCAvoidRegionExit::CTCAvoidRegionExit: the base gets the thing, both intelligent pointers are
// constructed empty and NextFrameCheck starts at zero.
#include <cstdio>
#include <cstring>
#include <new>
struct CThing { long m_id; };

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
struct CBaseObject;
static int g_ipCtor;
struct CBaseIntelligentPointer {
    virtual void v00() { }
    CBaseObject* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_ipCtor; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};
struct CTCAvoidRegionExit : CTCBase_Calls {
    CIntelligentPointer EntranceMarker;
    CIntelligentPointer ExitMarker;
    unsigned long NextFrameCheck;
    CTCAvoidRegionExit(CThing* thing) : CTCBase_Calls(thing), NextFrameCheck(0) { }
};

int main()
{
    CThing thing;
    CTCAvoidRegionExit* p = new (Fresh()) CTCAvoidRegionExit(&thing);
    if (g_baseCtor != 1 || g_ipCtor != 2 || p->ParentThing != &thing || p->EntranceMarker.m_pItem != 0 || p->ExitMarker.m_pItem != 0 || p->NextFrameCheck != 0) { std::printf("004d975c_TEST FAIL\n"); return 1; }
    std::printf("004d975c_TEST PASS\n");
    return 0;
}