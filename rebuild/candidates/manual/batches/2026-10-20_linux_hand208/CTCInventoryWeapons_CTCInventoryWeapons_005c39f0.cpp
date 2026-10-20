// CTCInventoryWeapons::~CTCInventoryWeapons() -- releases both active-weapon pointers, then the menu list, the item map and the primitive handles, then the screen and inventory bases
// Leaf: ??1CTCInventoryWeapons
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C39F0, 165 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`). Label correction: the catalog calls this row
// "~CTCInventoryAbilities"; the vtables it stores, 0x01253FF4 (+0) and 0x01253FE0 (+0x130), are CTCInventoryWeapons'
// (rtti-vtables.tsv), and the members it destroys are CTCInventoryWeapons' PDB members.
// Callees: CIntelligentPointer<CThing>::CIntelligentPointer(CThing*) @ 0x004A96F0, ::operator= @ 0x004A9760,
//          CBaseIntelligentPointer::~CBaseIntelligentPointer() @ 0x00A01C10 (x4), list<NUISystem::CComponent*> dtor
//          @ 0x0042ABCA, map<NUISystem::CComponent*,long> dtor @ 0x004DD1C4 (landed as ~CClothingItemMap),
//          vector<CEnginePrimitiveHandle> dtor @ 0x00440C40 (landed as ~CRetailHandleVector), NInventory::
//          CTCInventoryBase::~CTCInventoryBase() @ 0x005C00A0.
// Shape: both vtables stored first; each weapon pointer is cleared with `PX = 0` (an implicit conversion
// temporary in the 8-byte frame, re-addressed with `lea` after its ctor, assigned out of line and destroyed); members die in reverse order; the
// NHeroInformationScreens::CBase secondary base's inline destructor stores its vtable 0x0123A3E0 just before the
// primary base destructor.
// Layout: Ego_r.pdb CTCInventoryWeapons at retail offsets (CTCInventoryBase is 0x130 bytes in retail):
// PActiveMeleeWeapon +0x134, PActiveRangedWeapon +0x141 (the PDB's odd offset: pack(1) on the pointer classes
// only), ActiveWeaponPrimitiveHandles +0x154, WeaponMenuItemMap +0x170, AvailableWeaponsEntries +0x198.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CThing;

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                 // +0x04
    ~CBaseIntelligentPointer();                      // 0x00A01C10
};
struct CIntelligentPointer_CThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>
    CIntelligentPointer_CThing(CThing* thing);       // 0x004A96F0
    CIntelligentPointer_CThing& operator=(const CIntelligentPointer_CThing& other);   // 0x004A9760
};
#pragma pack(pop)

struct SComponentListNode;
struct CComponentList {                              // std::list<NUISystem::CComponent*> (head pointer)
    SComponentListNode* m_pNode;
    ~CComponentList();                               // 0x0042ABCA
};
struct CClothingItemMap {                            // std::map<NUISystem::CComponent*, long> (12 bytes)
    void* _M_header; unsigned long _M_node_count; unsigned char _M_key_compare;
    ~CClothingItemMap();                             // 0x004DD1C4
};
struct CRetailHandleVector {                         // std::vector<CEnginePrimitiveHandle> (12 bytes)
    void* _M_start; void* _M_finish; void* _M_end_of_storage;
    ~CRetailHandleVector();                          // 0x00440C40
};

namespace NInventory {
struct CTCInventoryBase {                            // retail 0x130 bytes
    unsigned char _pad04[0x12C];
    virtual ~CTCInventoryBase();                     // 0x005C00A0
};
}
namespace NHeroInformationScreens {
struct CBase {                                       // vtable 0x0123A3E0
    virtual void v00();
    virtual ~CBase() {}
};
}

struct NUISystemComponent;
class CTCInventoryWeapons : public NInventory::CTCInventoryBase, public NHeroInformationScreens::CBase {
public:
    CIntelligentPointer_CThing PActiveMeleeWeapon;   // +0x134
    long                       ActiveMeleeWeaponDefIndex;   // +0x13C
    bool                       MeleeWeaponCarriedOnLoad;    // +0x140
    CIntelligentPointer_CThing PActiveRangedWeapon;  // +0x141
    long                       ActiveRangedWeaponDefIndex;  // +0x14C
    bool                       RangedWeaponCarriedOnLoad;   // +0x150
    CRetailHandleVector        ActiveWeaponPrimitiveHandles;   // +0x154
    long                       ConfiscatedActiveMeleeWeaponDefIndex;    // +0x160
    long                       ConfiscatedActiveRangedWeaponDefIndex;   // +0x164
    long                       PreviouslyWieldedMeleeWeaponDefIndex;    // +0x168
    long                       PreviouslyWieldedRangedWeaponDefIndex;   // +0x16C
    CClothingItemMap           WeaponMenuItemMap;    // +0x170
    NUISystemComponent*        WeaponsList;          // +0x17C
    NUISystemComponent*        PAssignedRangedIcon;  // +0x180
    NUISystemComponent*        PAssignedMeleeIcon;   // +0x184
    NUISystemComponent*        PAssignedRangedMenuEntry;   // +0x188
    NUISystemComponent*        PAssignedMeleeMenuEntry;    // +0x18C
    NUISystemComponent*        PRangedList;          // +0x190
    NUISystemComponent*        PMeleeList;           // +0x194
    CComponentList             AvailableWeaponsEntries;   // +0x198
    virtual void v00();
    virtual ~CTCInventoryWeapons();
};

CTCInventoryWeapons::~CTCInventoryWeapons()
{
    PActiveMeleeWeapon = 0;
    PActiveRangedWeapon = 0;
}
