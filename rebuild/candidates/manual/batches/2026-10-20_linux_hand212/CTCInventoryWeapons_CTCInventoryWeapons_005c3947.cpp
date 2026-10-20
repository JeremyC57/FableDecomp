// CTCInventoryWeapons::CTCInventoryWeapons(CThing* thing) -- construct the weapons inventory: the inventory base, the screen base at +0x130, both active-weapon pointers, the primitive handle vector, the menu item map and the entries list
// Leaf: ??0CTCInventoryWeapons
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C3947, 169 bytes, `ret 4`; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`, as the landed dtor @ 0x005C39F0).
// Label correction: the catalog calls this row "CTCWallMountEffects::CTCWallMountEffects"; the vtables it stores,
// 0x01253FF4 (+0) and 0x01253FE0 (+0x130), are CTCInventoryWeapons' (rtti-vtables.tsv), and the members it builds
// are CTCInventoryWeapons' PDB members at retail offsets.
// Callees: NInventory::CTCInventoryBase::CTCInventoryBase(CThing*) @ 0x005BFF07, NHeroInformationScreens::CBase::CBase()
//          @ 0x0066E734, CIntelligentPointer<CThing>::CIntelligentPointer(CThing*) @ 0x004A96F0 (x2),
//          vector<CEnginePrimitiveHandle>::vector(size_type) @ 0x00485D30 (count 2), map<NUISystem::CComponent*,long>
//          ctor @ 0x004DF81D, list<NUISystem::CComponent*>::list(const allocator_type&) @ 0x0042ABAF.
// Shape: initialiser list in declaration order; only WeaponsList of the raw component pointers is zeroed; the list's
// default allocator temporary lives in the dead argument slot (`lea eax,[ebp+0Bh]`).
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CThing;

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                 // +0x04
};
struct CIntelligentPointer_CThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>
    CIntelligentPointer_CThing(CThing* thing);       // 0x004A96F0
};
#pragma pack(pop)

struct SAllocator { SAllocator() { } };
struct SComponentListNode;
struct CComponentList {                              // std::list<NUISystem::CComponent*> (head pointer)
    SComponentListNode* m_pNode;
    CComponentList(const SAllocator& allocator = SAllocator());   // 0x0042ABAF
};
struct CComponentItemMap {                           // std::map<NUISystem::CComponent*, long> (12 bytes)
    void* _M_header; long _M_node_count; long _M_compare;
    CComponentItemMap();                             // 0x004DF81D
};
struct CEnginePrimitiveHandleVector {                // std::vector<CEnginePrimitiveHandle> (12 bytes)
    void* m_first; void* m_last; void* m_end;
    CEnginePrimitiveHandleVector(unsigned long count);   // 0x00485D30
};

namespace NInventory {
class CTCInventoryBase {                             // retail 0x130 bytes
public:
    CTCInventoryBase(CThing* thing);                 // 0x005BFF07
    virtual ~CTCInventoryBase();
    unsigned char _pad04[0x130 - 0x04];
};
}
namespace NHeroInformationScreens {
class CBase {                                        // secondary base, vtable stored by its out-of-line ctor
public:
    CBase();                                         // 0x0066E734
    virtual void v00();
};
}

struct CDefIndex {                                   // a def table index
    long Index;
    CDefIndex(long index) : Index(index) {}
};

struct NUISystemComponent;
class CTCInventoryWeapons : public NInventory::CTCInventoryBase, public NHeroInformationScreens::CBase {
public:
    CIntelligentPointer_CThing   PActiveMeleeWeapon;            // +0x134
    CDefIndex                    ActiveMeleeWeaponDefIndex;     // +0x13C
    bool                         MeleeWeaponCarriedOnLoad;      // +0x140
    CIntelligentPointer_CThing   PActiveRangedWeapon;           // +0x141
    CDefIndex                    ActiveRangedWeaponDefIndex;    // +0x14C
    bool                         RangedWeaponCarriedOnLoad;     // +0x150
    CEnginePrimitiveHandleVector ActiveWeaponPrimitiveHandles;  // +0x154
    long                         ConfiscatedActiveMeleeWeaponDefIndex;    // +0x160
    long                         ConfiscatedActiveRangedWeaponDefIndex;   // +0x164
    long                         PreviouslyWieldedMeleeWeaponDefIndex;    // +0x168
    long                         PreviouslyWieldedRangedWeaponDefIndex;   // +0x16C
    CComponentItemMap            WeaponMenuItemMap;             // +0x170
    NUISystemComponent*          WeaponsList;                   // +0x17C
    NUISystemComponent*          PAssignedRangedIcon;           // +0x180
    NUISystemComponent*          PAssignedMeleeIcon;            // +0x184
    NUISystemComponent*          PAssignedRangedMenuEntry;      // +0x188
    NUISystemComponent*          PAssignedMeleeMenuEntry;       // +0x18C
    NUISystemComponent*          PRangedList;                   // +0x190
    NUISystemComponent*          PMeleeList;                    // +0x194
    CComponentList               AvailableWeaponsEntries;       // +0x198
    CTCInventoryWeapons(CThing* thing);
    virtual ~CTCInventoryWeapons();
    virtual void v00();
};

CTCInventoryWeapons::CTCInventoryWeapons(CThing* thing)
    : NInventory::CTCInventoryBase(thing),
      PActiveMeleeWeapon(0), ActiveMeleeWeaponDefIndex(0), MeleeWeaponCarriedOnLoad(false),
      PActiveRangedWeapon(0), ActiveRangedWeaponDefIndex(0), RangedWeaponCarriedOnLoad(false),
      ActiveWeaponPrimitiveHandles(2),
      ConfiscatedActiveMeleeWeaponDefIndex(0), ConfiscatedActiveRangedWeaponDefIndex(0),
      PreviouslyWieldedMeleeWeaponDefIndex(0), PreviouslyWieldedRangedWeaponDefIndex(0),
      WeaponsList(0)
{
}
