// CInventoryItemDef::Transfer(CPersistContext& context) -- persist the CInventoryItemDef's fields (20 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@CInventoryItemDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00451D28, 233 bytes, virtual thiscall `ret 4`; size-optimised def TU (unaligned entry, every member address through eax).
// Identity (proven): CInventoryItemDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x01231DBC, whose RTTI type descriptor is .?AVCInventoryItemDef@@; the Ego_r/FableWin.pdb name sets hold ?Transfer@CInventoryItemDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@CInventoryItemDef@@UAEXAAVCPersistContext@@@Z" agrees; family H053 (one-transfer-per-member bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(CEngineGraphic&) 0x00451E11, Transfer(unsigned long&) 0x00431020, Transfer(long&) 0x00431102, Transfer(float&) 0x00431061, Transfer(EInventoryActionType&) 0x00451E59, Transfer(ETCInterfaceType&) 0x00451EFD, Transfer(EInventoryObjectOrientation&) 0x00451FA1, Transfer(bool&) 0x0043314A, Transfer(ETutorialCategory&) 0x00452045, Transfer(EUIInventoryCategory&) 0x00452086.
// Layout: Ego_r.pdb CInventoryItemDef members at their PDB offsets; every transferred offset hits the PDB member of the asserted type.
//   engine/CInventoryItemDef.h exists but is the flattened data-only view (no virtual Transfer to define), so a local class is used -- Review at landing.
// Shape: context in edi, this in esi, one `lea eax,[esi+N] ; push eax ; mov ecx,edi ; call` per member, the last address formed in place (`add esi,N`).
#pragma optimize("s", on)
class CEngineGraphic {                                             // Ego_r.pdb, 14 bytes (opaque here)
public:
    unsigned char _body[14];
};
enum EInventoryActionType { };                              // enumerators not needed
enum EInventoryObjectOrientation { };                       // enumerators not needed
enum ETCInterfaceType { };                                  // enumerators not needed
enum ETutorialCategory { };                                 // enumerators not needed
enum EUIInventoryCategory { };                              // enumerators not needed
class CPersistContext {
public:
    template <class T> void Transfer(T& value);                        // 0x00451E11
    void Transfer(EInventoryActionType& value);                  // 0x00451E59
    void Transfer(ETCInterfaceType& value);                      // 0x00451EFD
    void Transfer(EInventoryObjectOrientation& value);           // 0x00451FA1
    void Transfer(ETutorialCategory& value);                     // 0x00452045
    void Transfer(EUIInventoryCategory& value);                  // 0x00452086
};
class CInventoryItemDef {
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x01231DBC slot 18
    unsigned char _base[0x25 - 4];                        // def base body
    CEngineGraphic Graphic;                            // +0x25
    unsigned long ItemDescription;                     // +0x34
    unsigned long ItemDetails;                         // +0x38
    long InventoryCategory;                            // +0x3C
    long MaxNumberItems;                               // +0x40
    long SlotIndex;                                    // +0x44
    float ActivationTime;                              // +0x48
    EInventoryActionType UseButtonAction;              // +0x4C
    ETCInterfaceType InventoryType;                    // +0x50
    EInventoryObjectOrientation Orientation;           // +0x54
    long HeroAbilityDef;                               // +0x58
    bool IsSellable;                                   // +0x5C
    bool IsBuyable;                                    // +0x5D
    bool IsConfiscatable;                              // +0x5E
    bool DoNotPersistUntilQuestCompleted;              // +0x5F
    bool DoNotAutoPickUp;                              // +0x60
    bool AutoPickUpAfterFirstPickUp;                   // +0x61
    long ItemToSelectUponRemoval;                      // +0x64
    ETutorialCategory TutorialCategory;                // +0x68
    EUIInventoryCategory UIInventoryCategory;          // +0x6C
};

void CInventoryItemDef::Transfer(CPersistContext& context)
{
    context.Transfer(Graphic);
    context.Transfer(ItemDescription);
    context.Transfer(ItemDetails);
    context.Transfer(InventoryCategory);
    context.Transfer(MaxNumberItems);
    context.Transfer(SlotIndex);
    context.Transfer(ActivationTime);
    context.Transfer(UseButtonAction);
    context.Transfer(InventoryType);
    context.Transfer(Orientation);
    context.Transfer(HeroAbilityDef);
    context.Transfer(IsSellable);
    context.Transfer(IsBuyable);
    context.Transfer(IsConfiscatable);
    context.Transfer(DoNotPersistUntilQuestCompleted);
    context.Transfer(DoNotAutoPickUp);
    context.Transfer(AutoPickUpAfterFirstPickUp);
    context.Transfer(ItemToSelectUponRemoval);
    context.Transfer(TutorialCategory);
    context.Transfer(UIInventoryCategory);
}