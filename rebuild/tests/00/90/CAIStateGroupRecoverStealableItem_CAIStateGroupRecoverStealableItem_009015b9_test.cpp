// Behaviour test for 0x009015b9 CAIStateGroup_RecoverStealableItem::CAIStateGroup_RecoverStealableItem(): base constructed, own vtable, every intelligent pointer member built empty.
#include <cstdio>
#include <new>

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

struct CAIStateGroup_RecoverStealableItem : CAIStateGroupBase {
    CIntelligentPointer<CThing>       PStealableItem;
    CIntelligentPointer<const CThing> PTargetPoint;
    CIntelligentPointer<CThing>       PPreviouslyCarriedItem;
    bool GotActionPoint; long ActionPointIndex; bool ReachedDestination; long PickUpActionID; bool NeedsCleanup;
    virtual int Kind() const { return 7; }
    CAIStateGroup_RecoverStealableItem();
};

CAIStateGroup_RecoverStealableItem::CAIStateGroup_RecoverStealableItem()
    : CAIStateGroupBase(), PStealableItem(), PTargetPoint(), PPreviouslyCarriedItem(), NeedsCleanup(false)
{
}

int main()
{
    unsigned char storage[sizeof(CAIStateGroup_RecoverStealableItem)];
    for (unsigned i = 0; i < sizeof(storage); ++i) storage[i] = 0xCD;
    CAIStateGroup_RecoverStealableItem* g = new (storage) CAIStateGroup_RecoverStealableItem;
    int fail = 0;
    if (g_baseCtorCalls != 1 || g_pointerCtorCalls != 3 || g->Kind() != 7 || g->_base[0] != 0x5a) fail = 1;
    if (g->PStealableItem.PointerKind() != 1 || g->PTargetPoint.PointerKind() != 2 || g->PPreviouslyCarriedItem.PointerKind() != 1) fail = 1;
    if (g->PStealableItem.m_pItem || g->PTargetPoint.m_pItem || g->PPreviouslyCarriedItem.m_pItem) fail = 1;
    if (g->NeedsCleanup != false) fail = 1;
    if (fail) { std::printf("009015b9_TEST FAIL\n"); return 1; }
    std::printf("009015b9_TEST PASS\n");
    return 0;
}