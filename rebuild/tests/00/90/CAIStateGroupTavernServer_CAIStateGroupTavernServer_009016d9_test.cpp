// Behaviour test for 0x009016d9 CAIStateGroup_TavernServer::CAIStateGroup_TavernServer(): base constructed, own vtable, every intelligent pointer member built empty.
#include <cstdio>

struct CThing; struct CThingObject; struct CThingBuilding; struct CThingAICreature;

static int g_baseCtorCalls = 0;
static int g_pointerCtorCalls = 0;

struct CAIStateGroupBase {
    unsigned char _base[0x10];
    virtual int Kind() const { return 0; }
    CAIStateGroupBase() { ++g_baseCtorCalls; _base[0] = 0x5a; }
};

struct CBaseIntelligentPointer {
    void* m_pItem;
    virtual int PointerKind() const { return 0; }
    CBaseIntelligentPointer() : m_pItem(0) { ++g_pointerCtorCalls; }
};

template<class T> struct STypeTag { enum { value = 1 }; };
template<> struct STypeTag<const CThing> { enum { value = 2 }; };
template<> struct STypeTag<CThingObject> { enum { value = 3 }; };
template<> struct STypeTag<CThingBuilding> { enum { value = 4 }; };
template<> struct STypeTag<CThingAICreature> { enum { value = 5 }; };

template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual int PointerKind() const { return STypeTag<T>::value; }
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};

struct CAIStateGroup_TavernServer : CAIStateGroupBase {
    CIntelligentPointer<CThingObject>   PJug;
    CIntelligentPointer<CThingObject>   PBar;
    CIntelligentPointer<CThingObject>   PTable;
    CIntelligentPointer<CThingObject>   PTankard;
    CIntelligentPointer<CThingBuilding> PPub;
    long JugIndex; long ServingActionPointIndex;
    virtual int Kind() const { return 9; }
    CAIStateGroup_TavernServer();
};

CAIStateGroup_TavernServer::CAIStateGroup_TavernServer()
    : CAIStateGroupBase(), PJug(), PBar(), PTable(), PTankard(), PPub()
{
}

int main()
{
    CAIStateGroup_TavernServer g;
    int fail = 0;
    if (g_baseCtorCalls != 1 || g_pointerCtorCalls != 5 || g.Kind() != 9) fail = 1;
    if (g.PJug.PointerKind() != 3 || g.PBar.PointerKind() != 3 || g.PTable.PointerKind() != 3 || g.PTankard.PointerKind() != 3) fail = 1;
    if (g.PPub.PointerKind() != 4) fail = 1;
    if (g.PJug.m_pItem || g.PBar.m_pItem || g.PTable.m_pItem || g.PTankard.m_pItem || g.PPub.m_pItem) fail = 1;
    if (fail) { std::printf("009016d9_TEST FAIL\n"); return 1; }
    std::printf("009016d9_TEST PASS\n");
    return 0;
}