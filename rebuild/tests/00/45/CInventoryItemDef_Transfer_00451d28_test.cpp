// behaviour test for 0x00451D28 CInventoryItemDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x25, +0x34, +0x38, +0x3C, +0x40, +0x44, +0x48, +0x4C, +0x50, +0x54, +0x58, +0x5C, +0x5D, +0x5E, +0x5F, +0x60, +0x61, +0x64, +0x68, +0x6C),
// so a binary save / load round trip restores the def
#include <cstdio>
#include <cstring>
struct Blob14 { unsigned char b[14]; };
struct Context {
    unsigned char buf[1024]; unsigned int pos; int load; const void* order[64]; unsigned int count;
    Context(int l) : pos(0), load(l), count(0) { }
    template <class T> void Transfer(T& v)
    {
        order[count++] = &v;
        if (load) std::memcpy(&v, buf + pos, sizeof(T)); else std::memcpy(buf + pos, &v, sizeof(T));
        pos += (unsigned int)sizeof(T);
    }
};
struct Def {
    Blob14 Graphic;
    unsigned long ItemDescription;
    unsigned long ItemDetails;
    long InventoryCategory;
    long MaxNumberItems;
    long SlotIndex;
    float ActivationTime;
    long UseButtonAction;
    long InventoryType;
    long Orientation;
    long HeroAbilityDef;
    bool IsSellable;
    bool IsBuyable;
    bool IsConfiscatable;
    bool DoNotPersistUntilQuestCompleted;
    bool DoNotAutoPickUp;
    bool AutoPickUpAfterFirstPickUp;
    long ItemToSelectUponRemoval;
    long TutorialCategory;
    long UIInventoryCategory;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.Graphic);
    c.Transfer(d.ItemDescription);
    c.Transfer(d.ItemDetails);
    c.Transfer(d.InventoryCategory);
    c.Transfer(d.MaxNumberItems);
    c.Transfer(d.SlotIndex);
    c.Transfer(d.ActivationTime);
    c.Transfer(d.UseButtonAction);
    c.Transfer(d.InventoryType);
    c.Transfer(d.Orientation);
    c.Transfer(d.HeroAbilityDef);
    c.Transfer(d.IsSellable);
    c.Transfer(d.IsBuyable);
    c.Transfer(d.IsConfiscatable);
    c.Transfer(d.DoNotPersistUntilQuestCompleted);
    c.Transfer(d.DoNotAutoPickUp);
    c.Transfer(d.AutoPickUpAfterFirstPickUp);
    c.Transfer(d.ItemToSelectUponRemoval);
    c.Transfer(d.TutorialCategory);
    c.Transfer(d.UIInventoryCategory);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    d.IsSellable = true;
    d.IsBuyable = true;
    d.IsConfiscatable = true;
    d.DoNotPersistUntilQuestCompleted = true;
    d.DoNotAutoPickUp = true;
    d.AutoPickUpAfterFirstPickUp = true;
    Context save(0); Persist(d, save);
    const void* want[20] = { &d.Graphic, &d.ItemDescription, &d.ItemDetails, &d.InventoryCategory, &d.MaxNumberItems, &d.SlotIndex, &d.ActivationTime, &d.UseButtonAction, &d.InventoryType, &d.Orientation, &d.HeroAbilityDef, &d.IsSellable, &d.IsBuyable, &d.IsConfiscatable, &d.DoNotPersistUntilQuestCompleted, &d.DoNotAutoPickUp, &d.AutoPickUpAfterFirstPickUp, &d.ItemToSelectUponRemoval, &d.TutorialCategory, &d.UIInventoryCategory };
    if (save.count != 20u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 20u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.Graphic, 0, sizeof(e.Graphic));
    std::memset(&e.ItemDescription, 0, sizeof(e.ItemDescription));
    std::memset(&e.ItemDetails, 0, sizeof(e.ItemDetails));
    std::memset(&e.InventoryCategory, 0, sizeof(e.InventoryCategory));
    std::memset(&e.MaxNumberItems, 0, sizeof(e.MaxNumberItems));
    std::memset(&e.SlotIndex, 0, sizeof(e.SlotIndex));
    std::memset(&e.ActivationTime, 0, sizeof(e.ActivationTime));
    std::memset(&e.UseButtonAction, 0, sizeof(e.UseButtonAction));
    std::memset(&e.InventoryType, 0, sizeof(e.InventoryType));
    std::memset(&e.Orientation, 0, sizeof(e.Orientation));
    std::memset(&e.HeroAbilityDef, 0, sizeof(e.HeroAbilityDef));
    std::memset(&e.IsSellable, 0, sizeof(e.IsSellable));
    std::memset(&e.IsBuyable, 0, sizeof(e.IsBuyable));
    std::memset(&e.IsConfiscatable, 0, sizeof(e.IsConfiscatable));
    std::memset(&e.DoNotPersistUntilQuestCompleted, 0, sizeof(e.DoNotPersistUntilQuestCompleted));
    std::memset(&e.DoNotAutoPickUp, 0, sizeof(e.DoNotAutoPickUp));
    std::memset(&e.AutoPickUpAfterFirstPickUp, 0, sizeof(e.AutoPickUpAfterFirstPickUp));
    std::memset(&e.ItemToSelectUponRemoval, 0, sizeof(e.ItemToSelectUponRemoval));
    std::memset(&e.TutorialCategory, 0, sizeof(e.TutorialCategory));
    std::memset(&e.UIInventoryCategory, 0, sizeof(e.UIInventoryCategory));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("00451d28_TEST FAIL %d\n", fail); return 1; }
    std::printf("00451d28_TEST PASS\n");
    return 0;
}