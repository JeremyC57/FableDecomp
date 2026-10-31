// CTCInventoryWeapons::GetFreeAugmentationSlotForWeapon(long weaponDefIndex) -- the first free augmentation slot of the weapon of that def: the live active melee or ranged weapon when it is of that def, otherwise a freshly created inventory object (whose 0x1a component is then told to finish)
// Grade: DIFFER(248v248) sec1b: 10 difflines (was 23). The two component lookups as the landed CThing::GetInterface<T> template (forceinline, key copied, map pointer local) fix the weapon/0xbf/ key registers (ebx/edi as retail); left: this esi v retail edi with the active-pointer register swapped in the two IS_THING_ALIVE tests. Tried after: named active-pointer local, inline IsActiveWeaponOfDef helper, same-TU LowerBound bodies. Earlier: DIFFER(248v248) -- register permutation only: retail this=edi, the active-weapon pointer esi, the weapon ebx (0xbf in the dead this register); VC7.1 here this=esi, pointer/weapon edi, 0xbf ebx. Tried: FindTC members, a self local, a named active-pointer local, an inline IsActiveWeaponOfDef helper, /Oy-, inline_depth(0).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C4671, length 246 bytes, `ret 4`. Frame-pointer size TU: optimize("y",off) +
// ("s",on). FableWin.pdb ?GetFreeAugmentationSlotForWeapon@CTCInventoryWeapons@@QAEJJ@Z.
// Callees: IS_THING_ALIVE(const CIntelligentPointer&) @ 0x004CC340 (`__fastcall`, out of line), CBaseIntelligentPointer::
// GetPItem @ 0x00A01B50, CTCInventoryBase::CreateInventoryObject(long) @ 0x005BE8F3, std::bitset::test @ 0x00410DE0, the
// component-map LowerBound const @ 0x004365B0 / non-const @ 0x0040F020, CTCObjectAugmentations::GetFreeSlot() @ 0x00765D50,
// the 0x1a component's member @ 0x005D8E50 (no arguments; catalog "CChunkCollectionIndicator::OnPredicateFail" is
// propagated, named after the landed OnShowEnded spelling).
// Shape: the two active-weapon tests through one pointer register (the GetPItem tail cross-jumped), the created flag a frame
// byte and the result a frame word preset to -1; the augmentations lookup is the landed GetInterface shape with the key in
// the dead argument slot; the 0x1a component pointer is preloaded with 0 (`xor esi,esi`) and called without a null test.
// Layout: the landed ~CTCInventoryWeapons (0x005C39F0) model: PActiveMeleeWeapon +0x134, PActiveRangedWeapon +0x141 (pack(1)
// on the pointer classes only); CThing def index short +0x8c, UsingComponents +0x20, components map +0x44.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CTCBase;
struct CThing;
#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                 // +0x04
    CThing* GetPItem() const;                        // 0x00A01B50
};
struct CIntelligentPointer_CThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>
};
#pragma pack(pop)
bool __fastcall IS_THING_ALIVE(const CIntelligentPointer_CThing& thing);   // 0x004CC340

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);            // 0x0040F020
    SEntry* LowerBound(const long* type) const;      // 0x004365B0
};
struct CThing {
    unsigned char _pad00[0x20];
    CPresenceBits UsingComponents;                   // +0x20
    unsigned char _pad21[0x44 - 0x21];
    CInterfaceMap Components;                        // +0x44
    unsigned char _pad4c[0x8c - 0x4c];
    unsigned short DefIndex;                         // +0x8c
    template <class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (UsingComponents.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CTCObjectAugmentations {
    long GetFreeSlot();                              // 0x00765D50
};
struct CTCInventoryItem {
    void OnShowEnded();                              // 0x005D8E50
};

struct CTCInventoryBase_Weapons {                    // retail 0x130 bytes
    virtual void v00();
    unsigned char _pad04[0x130 - 0x04];
    CThing* CreateInventoryObject(long itemType);    // 0x005BE8F3
};
class CTCInventoryWeapons : public CTCInventoryBase_Weapons {
public:
    void*                      CBaseVptr;            // +0x130 (NHeroInformationScreens::CBase)
    CIntelligentPointer_CThing PActiveMeleeWeapon;   // +0x134
    long                       ActiveMeleeWeaponDefIndex;   // +0x13C
    bool                       MeleeWeaponCarriedOnLoad;    // +0x140
    CIntelligentPointer_CThing PActiveRangedWeapon;  // +0x141
    long GetFreeAugmentationSlotForWeapon(long weaponDefIndex);
};

long CTCInventoryWeapons::GetFreeAugmentationSlotForWeapon(long weaponDefIndex)
{
    long slot = -1;
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
    if (weapon->GetInterface(&augmentations, 0xbf))
        slot = augmentations->GetFreeSlot();
    if (created) {
        CTCInventoryItem* item = 0;
        weapon->GetInterface(&item, 0x1a);
        item->OnShowEnded();
    }
    return slot;
}
