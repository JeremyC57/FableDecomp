// Behaviour test for 0x00901658 CAIStateGroup_CarryCrateAndFollow::CAIStateGroup_CarryCrateAndFollow(): base constructed, own vtable, every intelligent pointer member built empty.
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

struct CAIStateGroup_CarryCrateAndFollow : CAIStateGroupBase {
    CIntelligentPointer<CThing> PCrate;
    CIntelligentPointer<CThing> PPreviouslyCarriedItem;
    CIntelligentPointer<CThing> PThingToFollow;
    bool GotActionPoint; long ActionPointIndex; long PickUpActionID;
    virtual int Kind() const { return 8; }
    CAIStateGroup_CarryCrateAndFollow();
};

CAIStateGroup_CarryCrateAndFollow::CAIStateGroup_CarryCrateAndFollow()
    : CAIStateGroupBase(), PCrate(), PPreviouslyCarriedItem(), PThingToFollow()
{
}

int main()
{
    CAIStateGroup_CarryCrateAndFollow g;
    int fail = 0;
    if (g_baseCtorCalls != 1 || g_pointerCtorCalls != 3 || g.Kind() != 8 || g._base[0] != 0x5a) fail = 1;
    if (g.PCrate.PointerKind() != 1 || g.PPreviouslyCarriedItem.PointerKind() != 1 || g.PThingToFollow.PointerKind() != 1) fail = 1;
    if (g.PCrate.m_pItem || g.PPreviouslyCarriedItem.m_pItem || g.PThingToFollow.m_pItem) fail = 1;
    if (fail) { std::printf("00901658_TEST FAIL\n"); return 1; }
    std::printf("00901658_TEST PASS\n");
    return 0;
}