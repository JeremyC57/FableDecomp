// CTCInventoryWeapons::AugmentWeapon(long weaponDefIndex, long augmentationDefIndex, bool& augmented) -- find the weapon of that def among the live active melee / ranged weapons (else create an inventory object for it), attach the augmentation to the weapon's first free slot, report whether a free slot is left, remove one augmentation from the owner's inventory of its category, and end the showable display of a freshly created weapon
// Leaf: AugmentWeapon
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C44FA, 375 bytes, `ret 0Ch`. Size TU with a frame pointer:
//   `#pragma optimize("y", off)` + `("s", on)` (`push 1Ah ; pop edi`, `leave`).
//   Ego_r.pdb: `?AugmentWeapon@CTCInventoryWeapons@@QAEXJJAA_N@Z`.
// Callees: IS_THING_ALIVE<CIntelligentPointer<...> > @ 0x004CC340 (`__fastcall`, const ref), CBaseIntelligentPointer::
//   GetPItem @ 0x00A01B50, CTCInventoryBase::CreateInventoryObject(long) @ 0x005BE8F3, std::bitset<>::test @ 0x00410DE0
//   and CVectorMap LowerBound @ 0x0040F020 (inlined GetInterface: 0xBF object augmentations, the item's inventory
//   category, 0x1A), CTCObjectAugmentations::GetFreeSlot @ 0x00765D50 / AttachAugmentationToSlot(long, long)
//   @ 0x00768120, CTCInventoryItem::GetInventoryCategory(long) @ 0x005D99D0 (static `__fastcall`), the inventory's
//   virtual slot 79 (+0x13C, RemoveItem(def, 1)), CTCShowableItem::OnShowEnded @ 0x005D8E50 (propagated label).
// Layout: retail CTCInventoryWeapons PActiveMeleeWeapon +0x134 and PActiveRangedWeapon +0x141 (Ego_r.pdb +0x14C /
//   +0x159: the same 13-byte {pointer, def index, bool} stride, 0x18 lower in the release base; the intelligent pointer
//   is 1-aligned as in the landed 0x007CA670); CThing def index +0x8C.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CTCBase;
struct CPresenceBits { bool test(long kind) const; };      // bitset<>::test 0x00410DE0
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                      // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);                   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
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
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)((it == map->last || interfaceType < it->first) ? map->last : it)->second;
            return true;
        }
        return false;
    }
};
#pragma pack(push, 1)
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public: 
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
#pragma pack(pop)
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x004CC340

class CTCObjectAugmentations { public:
    long GetFreeSlot() const;                               // 0x00765D50
    void AttachAugmentationToSlot(long augmentation, long slot);   // 0x00768120
};
struct CTCShowableItem {
    void OnShowEnded();                                     // 0x005D8E50
};
class CTCInventoryItem { public:
    static long __fastcall GetInventoryCategory(long defIndex);    // 0x005D99D0
};

namespace NInventory {
class CTCInventoryBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); 
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); 
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); 
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); 
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); 
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); 
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); 
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); 
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71(); 
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76(); virtual void v77(); virtual void v78(); 
    virtual void RemoveItem(long item, long count);         // slot 79 (+0x13C)
    CThing*       ParentThing;                              // +0x04 CTCBase
    unsigned char _pad08[0x134 - 0x08];
    CThing* CreateInventoryObject(long defIndex);           // 0x005BE8F3
};
}

class CTCInventoryWeapons : public NInventory::CTCInventoryBase { public:
    CIntelligentPointer<CThing> PActiveMeleeWeapon;         // +0x134
    long          ActiveMeleeWeaponDefIndex;                // +0x13C
    bool          MeleeWeaponCarriedOnLoad;                 // +0x140
    CIntelligentPointer<CThing> PActiveRangedWeapon;        // +0x141 (packed in retail)
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