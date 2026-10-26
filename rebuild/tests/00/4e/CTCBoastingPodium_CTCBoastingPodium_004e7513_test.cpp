// Behaviour test for 0x004E7513 CTCBoastingPodium::CTCBoastingPodium: the vector map empty and unsorted,
// the counters zero, BoastingActive false, PDef null.
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
struct CKeyPairCompareLess { };
struct CBoastingPositionMap {
    void* m_pFirst; void* m_pLast; void* m_pEnd;
    CKeyPairCompareLess m_compare;
    bool m_sorted;
    CBoastingPositionMap() : m_pFirst(0), m_pLast(0), m_pEnd(0), m_sorted(false) { }
};
struct CBoastingPodiumDefPointer { const void* m_pDef; CBoastingPodiumDefPointer() : m_pDef(0) { } };
class CTCBoastingPodium : public CTCBase_Calls { public:
    CBoastingPositionMap      BoastingPositions;
    long                      LastPositionsWithBoastCount;
    unsigned long             FrameBoastListFrom;
    bool                      BoastingActive;
    CBoastingPodiumDefPointer PDef;
    CTCBoastingPodium(CThing* thing) : CTCBase_Calls(thing), LastPositionsWithBoastCount(0), FrameBoastListFrom(0), BoastingActive(false) { }
};

int main()
{
    CThing thing;
    CTCBoastingPodium* p = new (Fresh()) CTCBoastingPodium(&thing);
    if (g_baseCtor != 1 || p->ParentThing != &thing || p->BoastingPositions.m_pFirst != 0 || p->BoastingPositions.m_pLast != 0 || p->BoastingPositions.m_pEnd != 0 || p->BoastingPositions.m_sorted) { std::printf("004e7513_TEST FAIL\n"); return 1; }
    if (p->LastPositionsWithBoastCount != 0 || p->FrameBoastListFrom != 0 || p->BoastingActive || p->PDef.m_pDef != 0) { std::printf("004e7513_TEST FAIL\n"); return 1; }
    std::printf("004e7513_TEST PASS\n");
    return 0;
}