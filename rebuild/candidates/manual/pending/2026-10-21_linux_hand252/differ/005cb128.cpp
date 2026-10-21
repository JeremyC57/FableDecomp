// Grade: DIFFER(392v392) -- only the callee-saved pair is swapped: retail keeps this in esi and the weapon-pointer / owner temporaries in edi, ours the reverse; every other byte matches
// CTCInventoryWeapons::RemoveItemFromInventory(long defIndex, long count) -- remove the items through the base, then for each live active weapon (melee, ranged) of which none are left in the inventory: take it out of the owner's carrying component if it is carried, refresh the sheathed weapons and the weapon sheathing, and kill the weapon thing; finally refresh the sheathed weapons and set the active weapons up on the carrier again
// Leaf: RemoveItemFromInventory
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005CB128, 392 bytes, `ret 8` (virtual). Size TU with a frame pointer:
//   `#pragma optimize("y", off)` + `("s", on)` (`push 46h ; pop ebx`).
//   Ego_r.pdb: `?RemoveItemFromInventory@CTCInventoryWeapons@@UAEXJJ@Z`.
// Callees: NInventory::CTCInventoryBase::RemoveItemFromInventory(long, long) @ 0x005BF3C6 (label
//   "CTCInventoryBase::GetItemDescription" is wrong: 43-byte (long, long) body, called qualified here),
//   IS_THING_ALIVE<CIntelligentPointer<...> > @ 0x004CC340 (`__fastcall`), CBaseIntelligentPointer::GetPItem
//   @ 0x00A01B50, CTCInventoryBase::GetNumberOfItemsOfTypeInInventory(long) @ 0x005BDF08, std::bitset<>::test
//   @ 0x00410DE0 and CVectorMap LowerBound @ 0x0040F020 (inlined GetInterface, carrying 0x46), CTCCarrying::
//   IsCarryingThing(const CThing&) @ 0x006A1060 / RemoveThing(CThing&) @ 0x006A19B0, RefreshSheathedWeapons
//   @ 0x005C9962, the weapon-sheathing refresh @ 0x005C552C (unnamed; forge-autoname "CTCCarryable_
//   PrepareWeaponSheathing" is a guess), CThing::Kill(bool) @ 0x004C9B80, SetupActiveWeaponsOnCarrier @ 0x005CA68E.
// Shape: the carrying pointer is the out-parameter GetInterface with no initialiser (melee: in the dead `count` slot,
//   ranged: in ebx once the shared 0x46 key is dead).
// Layout: retail CTCInventoryWeapons PActiveMeleeWeapon +0x134 / PActiveRangedWeapon +0x141 (1-aligned intelligent
//   pointers, as the landed ctor 0x005C3947); CThing def index +0x8C.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CTCBase;
struct CPresenceBits { bool test(long kind) const; };      // bitset<>::test 0x00410DE0
struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    CPresenceBits present;                                  // +0x20
    unsigned char _pad21[0x44 - 0x21];
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad4c[0x8c - 0x4c];
    unsigned short DefIndex;                                // +0x8C
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (present.test(interfaceType)) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    void Kill(bool silent);                                 // 0x004C9B80
};
#pragma pack(push, 1)
struct CBaseIntelligentPointer {                            // 8 bytes, 1-aligned
    virtual void v00();
    void* m_pItem;                                          // +0x04
    CThing* GetPItem() const;                               // 0x00A01B50
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer { };
#pragma pack(pop)
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x004CC340

struct CTCCarrying {
    bool IsCarryingThing(const CThing& thing) const;        // 0x006A1060
    void RemoveThing(CThing& thing);                        // 0x006A19B0
};

namespace NInventory {
struct CTCInventoryBase {
    void*         __vftable;                                // +0x00
    CThing*       ParentThing;                              // +0x04 CTCBase
    unsigned char _pad08[0x134 - 0x08];
    void RemoveItemFromInventory(long defIndex, long count);    // 0x005BF3C6
    long GetNumberOfItemsOfTypeInInventory(long defIndex) const;   // 0x005BDF08
};
}

struct CTCInventoryWeapons : NInventory::CTCInventoryBase {
    CIntelligentPointer<CThing> PActiveMeleeWeapon;         // +0x134
    long          ActiveMeleeWeaponDefIndex;                // +0x13C
    bool          MeleeWeaponCarriedOnLoad;                 // +0x140
    CIntelligentPointer<CThing> PActiveRangedWeapon;        // +0x141
    void RefreshSheathedWeapons();                          // 0x005C9962
    void RefreshWeaponSheathing();                          // 0x005C552C
    void SetupActiveWeaponsOnCarrier();                     // 0x005CA68E
    void RemoveItemFromInventory(long defIndex, long count);
};

void CTCInventoryWeapons::RemoveItemFromInventory(long defIndex, long count)
{
    NInventory::CTCInventoryBase::RemoveItemFromInventory(defIndex, count);
    if (IS_THING_ALIVE(PActiveMeleeWeapon)
        && GetNumberOfItemsOfTypeInInventory(PActiveMeleeWeapon.GetPItem()->DefIndex) == 0) {
        CTCCarrying* carrying;
        if (ParentThing->GetInterface(&carrying, 0x46)) {
            if (carrying->IsCarryingThing(*PActiveMeleeWeapon.GetPItem()))
                carrying->RemoveThing(*PActiveMeleeWeapon.GetPItem());
        }
        RefreshSheathedWeapons();
        RefreshWeaponSheathing();
        PActiveMeleeWeapon.GetPItem()->Kill(false);
    }
    if (IS_THING_ALIVE(PActiveRangedWeapon)
        && GetNumberOfItemsOfTypeInInventory(PActiveRangedWeapon.GetPItem()->DefIndex) == 0) {
        CTCCarrying* carrying;
        if (ParentThing->GetInterface(&carrying, 0x46)) {
            if (carrying->IsCarryingThing(*PActiveRangedWeapon.GetPItem()))
                carrying->RemoveThing(*PActiveRangedWeapon.GetPItem());
        }
        RefreshSheathedWeapons();
        RefreshWeaponSheathing();
        PActiveRangedWeapon.GetPItem()->Kill(false);
    }
    RefreshSheathedWeapons();
    SetupActiveWeaponsOnCarrier();
}
