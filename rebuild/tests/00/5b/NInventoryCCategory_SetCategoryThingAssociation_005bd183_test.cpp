// Behaviour test for 0x005BD183 NInventory::CCategory::SetCategoryThingAssociation(CThing*).
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("005bd183_TEST FAIL\n"); return 1; } } while (0)

class CBaseObject { public: int Id; };
static int g_ptrSets = 0, g_ptrDtors = 0;
class CBaseIntelligentPointer {                         // test model: vptr + item
public:
    CBaseObject* PItem;
    CBaseIntelligentPointer() : PItem(0) { }
    virtual ~CBaseIntelligentPointer() { ++g_ptrDtors; PItem = 0; }
    CBaseObject* GetPItem() const { return PItem; }
    void SetPItem(CBaseObject* item) { PItem = item; ++g_ptrSets; }
};
template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() { }
    CIntelligentPointer(CBaseObject* item) { SetPItem(item); }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

class CThing : public CBaseObject { };
namespace NInventory {
class CCategory {
public:
    CIntelligentPointer<CThing> PAssociatedThing;
    long                        ItemCount;
    CCategory() : ItemCount(0) { }
    void SetCategoryThingAssociation(CThing* thing)
    {
        CIntelligentPointer<CThing> associated(thing);
        PAssociatedThing = associated;
    }
};
}

int main()
{
    CThing a, b;
    a.Id = 1; b.Id = 2;
    NInventory::CCategory cat;
    CHECK(cat.PAssociatedThing.GetPItem() == 0);
    g_ptrSets = g_ptrDtors = 0;
    cat.SetCategoryThingAssociation(&a);
    CHECK(cat.PAssociatedThing.GetPItem() == &a && g_ptrSets == 2 && g_ptrDtors == 1);
    cat.SetCategoryThingAssociation(&b);
    CHECK(cat.PAssociatedThing.GetPItem() == &b && g_ptrSets == 4 && g_ptrDtors == 2);
    cat.SetCategoryThingAssociation(0);
    CHECK(cat.PAssociatedThing.GetPItem() == 0 && cat.ItemCount == 0);
    std::printf("005bd183_TEST PASS\n");
    return 0;
}