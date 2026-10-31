// CTCInventoryWeapons::GetFreeAugmentationSlotForWeapon(long weaponDefIndex) -- the first free augmentation slot of the weapon of that def: the live active melee or ranged weapon when it is of that def, otherwise a freshly created inventory object (whose 0x1a component is then told to finish)
// Grade: DIFFER(248v248) -- this esi v retail edi with the active-weapon pointer edi v esi in the two IS_THING_ALIVE tests (9 lines each side); chunk 609: PDB template IS_THING_ALIVE<CIntelligentPointer<CThing> > and const GetFreeSlot (same). Earlier sweeps: GetInterface template (fixed weapon/key regs), active-pointer local, inline def-test helper, same-TU LowerBound bodies, FindTC members, self local, /Oy-, inline_depth(0).
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
class CThing;
#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                 // +0x04
    CThing* GetPItem() const;                        // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
};
#pragma pack(pop)
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);
template <> bool __fastcall IS_THING_ALIVE<CIntelligentPointer<CThing> >(const CIntelligentPointer<CThing>& thing);   // 0x004CC340

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
class CThing {
public:
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
class CTCObjectAugmentations {
public:
    long GetFreeSlot() const;                        // 0x00765D50
};
class CUnprovenTC_005d8e50 {                         // interface 0x1a (method name not proven)
public:
    void Notify();                                   // 0x005D8E50
};

namespace NInventory {
class CTCInventoryBase {                             // retail 0x130 bytes
public:
    virtual void v00();
    unsigned char _pad04[0x130 - 0x04];
protected:
    CThing* CreateInventoryObject(long itemType);    // 0x005BE8F3
};
}
class CTCInventoryWeapons : public NInventory::CTCInventoryBase {
public:
    void*                      CBaseVptr;            // +0x130 (NHeroInformationScreens::CBase)
    CIntelligentPointer<CThing> PActiveMeleeWeapon;   // +0x134
    long                       ActiveMeleeWeaponDefIndex;   // +0x13C
    bool                       MeleeWeaponCarriedOnLoad;    // +0x140
    CIntelligentPointer<CThing> PActiveRangedWeapon;  // +0x141
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
        CUnprovenTC_005d8e50* item = 0;
        weapon->GetInterface(&item, 0x1a);
        item->Notify();
    }
    return slot;
}
