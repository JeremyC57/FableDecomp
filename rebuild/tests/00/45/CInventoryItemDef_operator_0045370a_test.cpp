// Behaviour test for 0x0045370A CInventoryItemDef::operator=: base part first, then every member copied, *this returned
#include <cstdio>
#include <cstring>
static int g_baseAssigns = 0;
class CDefClassBase { public:
    unsigned char _body[0x25];
    CDefClassBase& operator=(const CDefClassBase& other) { std::memcpy(_body, other._body, sizeof _body); ++g_baseAssigns; return *this; }
};
class CEngineGraphic { public: unsigned char _bytes[14]; };
class CInventoryItemDef : public CDefClassBase { public:
    CEngineGraphic Graphic;
    unsigned long ItemDescription, ItemDetails;
    long InventoryCategory, MaxNumberItems, SlotIndex;
    float ActivationTime;
    int UseButtonAction, InventoryType, Orientation;
    long HeroAbilityDef;
    bool IsSellable, IsBuyable, IsConfiscatable, DoNotPersistUntilQuestCompleted, DoNotAutoPickUp, AutoPickUpAfterFirstPickUp;
    long ItemToSelectUponRemoval;
    int TutorialCategory, UIInventoryCategory;
    CInventoryItemDef& operator=(const CInventoryItemDef& other)
    {
        CDefClassBase::operator=(other);
        Graphic = other.Graphic;
        ItemDescription = other.ItemDescription; ItemDetails = other.ItemDetails;
        InventoryCategory = other.InventoryCategory; MaxNumberItems = other.MaxNumberItems; SlotIndex = other.SlotIndex;
        ActivationTime = other.ActivationTime;
        UseButtonAction = other.UseButtonAction; InventoryType = other.InventoryType; Orientation = other.Orientation;
        HeroAbilityDef = other.HeroAbilityDef;
        IsSellable = other.IsSellable; IsBuyable = other.IsBuyable; IsConfiscatable = other.IsConfiscatable;
        DoNotPersistUntilQuestCompleted = other.DoNotPersistUntilQuestCompleted;
        DoNotAutoPickUp = other.DoNotAutoPickUp; AutoPickUpAfterFirstPickUp = other.AutoPickUpAfterFirstPickUp;
        ItemToSelectUponRemoval = other.ItemToSelectUponRemoval;
        TutorialCategory = other.TutorialCategory; UIInventoryCategory = other.UIInventoryCategory;
        return *this;
    }
};
int main()
{
    CInventoryItemDef a, b;
    std::memset(&a, 0, sizeof a);
    std::memset(&b, 0, sizeof b);
    for (int i = 0; i < 0x25; ++i) b._body[i] = (unsigned char)(i + 1);
    for (int i = 0; i < 14; ++i) b.Graphic._bytes[i] = (unsigned char)(0x80 + i);
    b.ItemDescription = 11; b.ItemDetails = 12; b.InventoryCategory = 13; b.MaxNumberItems = 14; b.SlotIndex = 15;
    b.ActivationTime = 2.5f; b.UseButtonAction = 3; b.InventoryType = 4; b.Orientation = 5; b.HeroAbilityDef = 16;
    b.IsSellable = true; b.IsConfiscatable = true; b.AutoPickUpAfterFirstPickUp = true;
    b.ItemToSelectUponRemoval = 17; b.TutorialCategory = 6; b.UIInventoryCategory = 7;
    CInventoryItemDef& r = (a = b);
    bool ok = (&r == &a) && g_baseAssigns == 1;
    ok = ok && std::memcmp(a._body, b._body, 0x25) == 0 && std::memcmp(a.Graphic._bytes, b.Graphic._bytes, 14) == 0;
    ok = ok && a.ItemDescription == 11 && a.ItemDetails == 12 && a.InventoryCategory == 13 && a.MaxNumberItems == 14;
    ok = ok && a.SlotIndex == 15 && a.ActivationTime == 2.5f && a.UseButtonAction == 3 && a.InventoryType == 4;
    ok = ok && a.Orientation == 5 && a.HeroAbilityDef == 16 && a.IsSellable && !a.IsBuyable && a.IsConfiscatable;
    ok = ok && !a.DoNotPersistUntilQuestCompleted && !a.DoNotAutoPickUp && a.AutoPickUpAfterFirstPickUp;
    ok = ok && a.ItemToSelectUponRemoval == 17 && a.TutorialCategory == 6 && a.UIInventoryCategory == 7;
    if (!ok) { std::printf("0045370a_TEST FAIL\n"); return 1; }
    std::printf("0045370a_TEST PASS\n");
    return 0;
}