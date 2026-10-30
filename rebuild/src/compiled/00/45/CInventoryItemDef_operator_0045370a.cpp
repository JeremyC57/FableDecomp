// CInventoryItemDef::operator=(const CInventoryItemDef&) -- def copy: CDefClassBase part, the 14-byte CEngineGraphic and the item's scalar fields; returns *this
// Leaf: ??4CInventoryItemDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045370A, 150 bytes, `ret 4`; size-optimised def TU (like the landed def assignments
//         0x00463BA2 CEntitySoundDef and 0x004F914A CHeroExperienceDef).
// Callees: CDefClassBase::operator= @ 0x00431F10 (the catalog label "CThingBuildingDef_CopyAssign" is propagated over every def
//          assignment).
// Identity: the body is reached only through the `jmp` thunk at 0x00453705, which is slot 19 of vtable 0x01231DBC = CInventoryItemDef
//   (rtti-vtables.tsv), the def-assignment slot (CHeroExperienceDef's slot 19 thunk 0x004F9145 reaches its operator= the same way).
//   PDB CInventoryItemDef (Ego_r, 112 bytes) agrees member for member with the offsets the body touches: Graphic +0x25,
//   ItemDescription +0x34 .. ActivationTime +0x48, UseButtonAction/InventoryType/Orientation +0x4c..+0x54, HeroAbilityDef +0x58,
//   the six bools +0x5c..+0x61, ItemToSelectUponRemoval +0x64, TutorialCategory +0x68, UIInventoryCategory +0x6c.
// Shape: the memberwise assignment in declaration order; the 14-byte packed CEngineGraphic (PDB size 14: BankIndex, AnimStep,
//   RenderSizeX, AdditiveAlpha, Type) is the `movsd x3 / movsw` block copy, the dword members go through eax (the float too:
//   a single float assignment is an integer move) and the bools through al.
// The generated engine headers flatten CDefClassBase into CInventoryItemDef (no base operator= to call) and give Graphic 0xf
// bytes, so the classes are modelled locally; CDefClassBase is its 0x25 packed bytes, CEngineGraphic its 14 packed bytes, so the
// natural layout lands every member on the retail offset with no #pragma pack.
#pragma optimize("s", on)   // the def module is size-optimised

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

class CEngineGraphic { public:                 // 14 packed bytes: BankIndex +0, AnimStep +4, RenderSizeX +8, AdditiveAlpha +12, Type +13
    unsigned char _bytes[14];
};

enum EInventoryActionType { INVENTORY_ACTION_TYPE_FORCE_DWORD = 0x7fffffff };
enum ETCInterfaceType { TC_INTERFACE_TYPE_FORCE_DWORD = 0x7fffffff };
enum EInventoryObjectOrientation { INVENTORY_OBJECT_ORIENTATION_FORCE_DWORD = 0x7fffffff };
enum ETutorialCategory { TUTORIAL_CATEGORY_FORCE_DWORD = 0x7fffffff };
enum EUIInventoryCategory { UI_INVENTORY_CATEGORY_FORCE_DWORD = 0x7fffffff };

// def vtable 0x01231DBC: slot 0 0x0044F65A (implicit inline dtor); real overrides from the def vtable scan
class CInventoryItemDef : public CDefClassBase {                   // vtable 0x01231DBC
public:
    CEngineGraphic              Graphic;                            // +0x25
    unsigned long               ItemDescription;                    // +0x34
    unsigned long               ItemDetails;                        // +0x38
    long                        InventoryCategory;                  // +0x3c
    long                        MaxNumberItems;                     // +0x40
    long                        SlotIndex;                          // +0x44
    float                       ActivationTime;                     // +0x48
    EInventoryActionType        UseButtonAction;                    // +0x4c
    ETCInterfaceType            InventoryType;                      // +0x50
    EInventoryObjectOrientation Orientation;                        // +0x54
    long                        HeroAbilityDef;                     // +0x58
    bool                        IsSellable;                         // +0x5c
    bool                        IsBuyable;                          // +0x5d
    bool                        IsConfiscatable;                    // +0x5e
    bool                        DoNotPersistUntilQuestCompleted;    // +0x5f
    bool                        DoNotAutoPickUp;                    // +0x60
    bool                        AutoPickUpAfterFirstPickUp;         // +0x61
    long                        ItemToSelectUponRemoval;            // +0x64
    ETutorialCategory           TutorialCategory;                   // +0x68
    EUIInventoryCategory        UIInventoryCategory;                // +0x6c

    CInventoryItemDef& operator=(const CInventoryItemDef& other);
    virtual void Transfer(CPersistContext& context);         // slot 18 0x00451D28
    virtual void Copy(const CDefClassBase* other);           // slot 19 0x00453705
    virtual unsigned long GetSizeofClass();                  // slot 20 0x0044C12F
};

CInventoryItemDef& CInventoryItemDef::operator=(const CInventoryItemDef& other)
{
    CDefClassBase::operator=(other);
    Graphic = other.Graphic;
    ItemDescription = other.ItemDescription;
    ItemDetails = other.ItemDetails;
    InventoryCategory = other.InventoryCategory;
    MaxNumberItems = other.MaxNumberItems;
    SlotIndex = other.SlotIndex;
    ActivationTime = other.ActivationTime;
    UseButtonAction = other.UseButtonAction;
    InventoryType = other.InventoryType;
    Orientation = other.Orientation;
    HeroAbilityDef = other.HeroAbilityDef;
    IsSellable = other.IsSellable;
    IsBuyable = other.IsBuyable;
    IsConfiscatable = other.IsConfiscatable;
    DoNotPersistUntilQuestCompleted = other.DoNotPersistUntilQuestCompleted;
    DoNotAutoPickUp = other.DoNotAutoPickUp;
    AutoPickUpAfterFirstPickUp = other.AutoPickUpAfterFirstPickUp;
    ItemToSelectUponRemoval = other.ItemToSelectUponRemoval;
    TutorialCategory = other.TutorialCategory;
    UIInventoryCategory = other.UIInventoryCategory;
    return *this;
}