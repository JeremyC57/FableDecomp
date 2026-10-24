// Behaviour test for 0x00901783 CAIStateGroup_TavernKeeper::CAIStateGroup_TavernKeeper(): base constructed, own vtable, every intelligent pointer member built empty.
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

struct CAIStateGroup_TavernKeeper : CAIStateGroupBase {
    CIntelligentPointer<CThingObject>     PJug;
    CIntelligentPointer<CThingObject>     PBar;
    CIntelligentPointer<CThingBuilding>   PPub;
    CIntelligentPointer<CThingAICreature> PPayee;
    long JugActionPointIndex;
    virtual int Kind() const { return 10; }
    CAIStateGroup_TavernKeeper();
};

CAIStateGroup_TavernKeeper::CAIStateGroup_TavernKeeper()
    : CAIStateGroupBase(), PJug(), PBar(), PPub(), PPayee()
{
}

int main()
{
    CAIStateGroup_TavernKeeper g;
    int fail = 0;
    if (g_baseCtorCalls != 1 || g_pointerCtorCalls != 4 || g.Kind() != 10) fail = 1;
    if (g.PJug.PointerKind() != 3 || g.PBar.PointerKind() != 3 || g.PPub.PointerKind() != 4 || g.PPayee.PointerKind() != 5) fail = 1;
    if (g.PJug.m_pItem || g.PBar.m_pItem || g.PPub.m_pItem || g.PPayee.m_pItem) fail = 1;
    if (fail) { std::printf("00901783_TEST FAIL\n"); return 1; }
    std::printf("00901783_TEST PASS\n");
    return 0;
}