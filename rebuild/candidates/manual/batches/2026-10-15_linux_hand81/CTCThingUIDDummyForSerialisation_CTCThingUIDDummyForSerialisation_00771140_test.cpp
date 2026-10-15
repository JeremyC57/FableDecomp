// Behaviour test for 0x00771140 CTCThingUIDDummyForSerialisation::CTCThingUIDDummyForSerialisation: the
// base gets the thing and the 64-bit ThingUID starts at zero.
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
#if defined(_MSC_VER)
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
struct CTCThingUIDDummyForSerialisation : CTCBase_Calls {
    u64 ThingUID;
    CTCThingUIDDummyForSerialisation(CThing* thing) : CTCBase_Calls(thing), ThingUID(0) { }
};

int main()
{
    CThing thing;
    CTCThingUIDDummyForSerialisation* p = new (Fresh()) CTCThingUIDDummyForSerialisation(&thing);
    if (g_baseCtor != 1 || p->ParentThing != &thing || p->ThingUID != 0) { std::printf("00771140_TEST FAIL\n"); return 1; }
    std::printf("00771140_TEST PASS\n");
    return 0;
}
