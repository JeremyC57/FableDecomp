#include <cstdio>
// Behaviour test for CTCInventoryWeapons::AugmentWeapon (0x005C44FA): same body as src/, with the liveness test, the
// inventory-object creation, the augmentation slots, the inventory removal and the showable hook stubbed and recorded.
class CTCBase { public: virtual ~CTCBase() { } };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
class CThing {
public:
    unsigned long presentLow, presentHigh;            // interface types 0..63
    CInterfaceMap m_interfaces;
    unsigned short DefIndex;
    bool alive;
    bool Present(long t) const { return t < 32 ? (presentLow >> t) & 1 : (presentHigh >> (t - 32)) & 1; }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        long bit = interfaceType & 63;
        if (Present(bit)) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer { };
template <class T> bool IS_THING_ALIVE(const T& p) { return p.m_pItem && p.m_pItem->alive; }

static int g_attach = 0; static long g_attachDef = 0, g_attachSlot = 0;
struct CTCObjectAugmentations : CTCBase {
    long freeSlots[3]; int used;
    long GetFreeSlot() const { return used < 3 ? freeSlots[used] : -1; }
    void AttachAugmentationToSlot(long a, long slot) { ++g_attach; g_attachDef = a; g_attachSlot = slot; ++used; }
};
static int g_showEnded = 0;
struct CTCShowableItem : CTCBase { void OnShowEnded() { ++g_showEnded; } };
struct CTCInventoryItem { static long GetInventoryCategory(long defIndex) { return defIndex >= 100 ? 0x13 : 0x12; } };

static int g_removes = 0; static long g_removeDef = 0, g_removeCount = 0; static void* g_removeFrom = 0;
static CThing* g_createdThing = 0; static int g_creates = 0; static long g_createDef = 0;
namespace NInventory {
struct CTCInventoryBase : CTCBase {
    CThing* ParentThing;
    virtual void RemoveItem(long item, long count) { ++g_removes; g_removeDef = item; g_removeCount = count; g_removeFrom = this; }
    CThing* CreateInventoryObject(long defIndex) { ++g_creates; g_createDef = defIndex; return g_createdThing; }
};
}
struct CTCInventoryWeapons : NInventory::CTCInventoryBase {
    CIntelligentPointer<CThing> PActiveMeleeWeapon;
    long ActiveMeleeWeaponDefIndex;
    bool MeleeWeaponCarriedOnLoad;
    CIntelligentPointer<CThing> PActiveRangedWeapon;
    void AugmentWeapon(long weaponDefIndex, long augmentationDefIndex, bool& augmented);
};

void CTCInventoryWeapons::AugmentWeapon(long weaponDefIndex, long augmentationDefIndex, bool& augmented)
{
    augmented = false;
    bool created = false;
    CThing* weapon;
    if (IS_THING_ALIVE(PActiveMeleeWeapon) && PActiveMeleeWeapon.GetPItem()->DefIndex == weaponDefIndex)
        weapon = PActiveMeleeWeapon.GetPItem();
    else if (IS_THING_ALIVE(PActiveRangedWeapon) && PActiveRangedWeapon.GetPItem()->DefIndex == weaponDefIndex)
        weapon = PActiveRangedWeapon.GetPItem();
    else {
        weapon = CreateInventoryObject(weaponDefIndex);
        created = true;
    }
    CTCObjectAugmentations* augmentations;
    if (weapon->GetInterface(&augmentations, 0xbf)) {
        augmentations->AttachAugmentationToSlot(augmentationDefIndex, augmentations->GetFreeSlot());
        augmented = augmentations->GetFreeSlot() != -1;
        CThing* owner = ParentThing;
        NInventory::CTCInventoryBase* inventory = 0;
        owner->GetInterface(&inventory, CTCInventoryItem::GetInventoryCategory(augmentationDefIndex));
        inventory->RemoveItem(augmentationDefIndex, 1);
    }
    if (created) {
        CTCShowableItem* showable = 0;
        weapon->GetInterface(&showable, 0x1a);
        showable->OnShowEnded();
    }
}

static void MakeWeapon(CThing& t, unsigned short def, bool alive, CInterfaceMap::SEntry* e, int n, CTCObjectAugmentations* aug,
                       CTCShowableItem* show)
{
    t.DefIndex = def; t.alive = alive; t.presentLow = 0; t.presentHigh = 0;
    for (int i = 0; i < n; ++i) { long b = e[i].type & 63; if (b < 32) t.presentLow |= 1ul << b; else t.presentHigh |= 1ul << (b - 32); }
    t.m_interfaces.first = e; t.m_interfaces.last = e + n;
    (void)aug; (void)show;
}

int main()
{
    int fails = 0;
    CTCObjectAugmentations augMelee; augMelee.freeSlots[0] = 0; augMelee.freeSlots[1] = 1; augMelee.freeSlots[2] = 2; augMelee.used = 1;
    CTCObjectAugmentations augNew; augNew.freeSlots[0] = 0; augNew.freeSlots[1] = 1; augNew.freeSlots[2] = 2; augNew.used = 2;
    CTCShowableItem show;
    CInterfaceMap::SEntry meleeE[1] = { { 0xbf, &augMelee } };
    CInterfaceMap::SEntry rangedE[1] = { { 0x05, 0 } };
    CInterfaceMap::SEntry newE[2] = { { 0x1a, &show }, { 0xbf, &augNew } };
    CThing melee, ranged, fresh;
    MakeWeapon(melee, 10, true, meleeE, 1, &augMelee, 0);
    MakeWeapon(ranged, 20, true, rangedE, 1, 0, 0);
    MakeWeapon(fresh, 30, true, newE, 2, &augNew, &show);
    g_createdThing = &fresh;
    NInventory::CTCInventoryBase invObjects, invAugments;
    CInterfaceMap::SEntry ownerE[2] = { { 0x12, &invObjects }, { 0x13, &invAugments } };
    CThing owner; MakeWeapon(owner, 1, true, ownerE, 2, 0, 0);
    CTCInventoryWeapons tc; tc.ParentThing = &owner;
    tc.PActiveMeleeWeapon.m_pItem = &melee; tc.PActiveRangedWeapon.m_pItem = &ranged;
    bool ok = true;
    // Active melee weapon: slot 1 used, slot 2 still free.
    tc.AugmentWeapon(10, 150, ok);
    if (!ok || g_attach != 1 || g_attachDef != 150 || g_attachSlot != 1 || g_creates != 0) ++fails;
    if (g_removes != 1 || g_removeDef != 150 || g_removeCount != 1 || g_removeFrom != &invAugments || g_showEnded != 0) ++fails;
    // Ranged weapon without augmentation slots: nothing attached or removed, flag cleared.
    ok = true;
    tc.AugmentWeapon(20, 50, ok);
    if (ok || g_attach != 1 || g_removes != 1 || g_creates != 0 || g_showEnded != 0) ++fails;
    // Unknown (or dead) weapon: created; its last free slot is taken, so the flag is false; showable hook runs.
    melee.alive = false;
    ok = true;
    tc.AugmentWeapon(10, 60, ok);
    if (ok || g_creates != 1 || g_createDef != 10 || g_attach != 2 || g_attachSlot != 2 || g_attachDef != 60) ++fails;
    if (g_removes != 2 || g_removeFrom != &invObjects || g_showEnded != 1) ++fails;
    if (fails) { std::printf("005c44fa_TEST FAIL %d\n", fails); return 1; }
    std::printf("005c44fa_TEST PASS\n");
    return 0;
}
