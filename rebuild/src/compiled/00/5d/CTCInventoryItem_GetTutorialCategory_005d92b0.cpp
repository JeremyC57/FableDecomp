// CTCInventoryItem::GetTutorialCategory() const -- the CInventoryItemDef's TutorialCategory, read through an add-ref'd copy of the def pointer
// Leaf: ?GetTutorialCategory@CTCInventoryItem@@QBE?AW4ETutorialCategory@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005D92B0, 32 bytes (+ int3 padding), thiscall, plain `ret`.
// Identity (proven): CTCInventoryItem::GetTutorialCategory() const; evidence: the last of CTCInventoryItem's def accessors, right before the landed GetEngineGraphic 0x005D92D0 (which takes the address of CInventoryItemDef::Graphic +0x25) -- Ego_r.pdb's order ... GetItemToSelectUponRemoval (+0x64, 0x005D9290), GetTutorialCategory, GetEngineGraphic; it reads PDef (+0x0C) -> +0x68 = Ego_r.pdb CInventoryItemDef::TutorialCategory (retail keeps the Ego_r offsets for this def: +0x25 / +0x3C / +0x40 / +0x4C / +0x50 / +0x5C..+0x5F / +0x64 all read by the neighbours); CThingPlayerCreature::LearnExpression 0x006AC430 calls it; both PDB name sets hold ?GetTutorialCategory@CTCInventoryItem@@QBE?AW4ETutorialCategory@@XZ.
// Label: the manifest's "GetCategoryIdentifier" is propagated (family J099: five byte-similar def accessors that differ only in the
//   field offset; retail has no identical-code folding).
// Callees: CInventoryItemDef's virtual Delete (CIVCountedPointeeBase slot 1) when the copy drops the last reference.
// Shape: the landed CTCWeapon::GetWeaponType 0x005DA7D0: a CDefPointer<const CInventoryItemDef> copy of PDef (+0x0C; inc
//   IVRefCount when non-null), the field read, then the copy's inline release (dec; Delete on zero -- its null test folds into
//   the dereference).
// Layout: retail RTTI CTCInventoryItem <- CTCBase (engine/CTCBase.h, data-only, included under a layout name); CInventoryItemDef <- CSubDefClassBase
//   <- CDefClassBase <- CDefPointeeBase <- CResource <- CIVCountedPointeeBase (Ego_r.pdb members; CDefClassBase is 0x25 bytes).
#define CTCBase CTCBasePdb
#include "engine/CTCBase.h"
#undef CTCBase
class CTCBase : public CTCBasePdb { };                      // engine/CTCBase.h is data-only; nothing virtual is needed here
class CIVCountedPointeeBase {                               // Ego_r.pdb 8 bytes: vptr, IVRefCount
public:
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;                            // slot 1 (FableWin.pdb ?Delete@CIVCountedPointeeBase@@UBEXXZ)
    __forceinline void AddRef() const { ++IVRefCount; }      // FableWin.pdb ?AddRef@CIVCountedPointeeBase@@QBEXXZ (inline here)
    __forceinline long Release() const                      // FableWin.pdb ?Release@CIVCountedPointeeBase@@QBEJXZ (inline here)
    {
        if (--IVRefCount == 0) {
            Delete();
            return 0;
        }
        return (long)IVRefCount;
    }
    mutable unsigned long IVRefCount;                       // +0x04
};
class CResourceList;
class CResource : public CIVCountedPointeeBase {            // Ego_r.pdb 0x1C
public:
    CResourceList* ResourceList;                            // +0x08
    CResource*     PrevResource;                            // +0x0C
    CResource*     NextResource;                            // +0x10
    unsigned long  ResourceSize;                            // +0x14
    unsigned long  LastUsedFrame;                           // +0x18
};
class CDefPointeeBase : public CResource { };               // Ego_r.pdb 0x1C, no members of its own
class CDefinitionManager;
#pragma pack(push, 1)
class CDefClassBase : public CDefPointeeBase {              // Ego_r.pdb 0x25 bytes
public:
    CDefinitionManager* PDefManager;                        // +0x1C
    unsigned long       GlobalIndex;                        // +0x20
    unsigned char       Setup : 1;                          // +0x24
    unsigned char       Template : 1;
    unsigned char       DefaultValsApplied : 1;
};
#pragma pack(pop)
class CSubDefClassBase : public CDefClassBase { };          // Ego_r.pdb 0x25 bytes, no members of its own
template <class T> class CDefPointer {                      // Ego_r.pdb 4 bytes
public:
    CDefPointer(const CDefPointer<T>& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    __forceinline ~CDefPointer() { if (Object) Object->Release(); }   // the IV release, inline (as the landed 0x005DA7D0)
    T* operator->() const { return static_cast<T*>(Object); }   // FableWin.pdb ??C?$CDefPointer@$$CB...@@QBEPB...@@XZ
    CDefPointeeBase* Object;                                // +0x00
};
enum EInventoryActionType { };                              // Ego_r.pdb (enumerators not needed)
enum ETCInterfaceType { };
enum EInventoryObjectOrientation { };
enum ETutorialCategory { };
enum EUIInventoryCategory { };
class CInventoryItemDef : public CSubDefClassBase {         // RTTI: CInventoryItemDef <- CSubDefClassBase (vtable 0x01231DBC); Ego_r.pdb 0x70
public:
    unsigned char               Graphic[0xf];               // +0x25 CEngineGraphic (opaque here)
    unsigned long               ItemDescription;            // +0x34
    unsigned long               ItemDetails;                // +0x38
    long                        InventoryCategory;          // +0x3C
    long                        MaxNumberItems;             // +0x40
    long                        SlotIndex;                  // +0x44
    float                       ActivationTime;             // +0x48
    EInventoryActionType        UseButtonAction;            // +0x4C
    ETCInterfaceType            InventoryType;              // +0x50
    EInventoryObjectOrientation Orientation;                // +0x54
    long                        HeroAbilityDef;             // +0x58
    bool                        IsSellable;                 // +0x5C
    bool                        IsBuyable;                  // +0x5D
    bool                        IsConfiscatable;            // +0x5E
    bool                        DoNotPersistUntilQuestCompleted;   // +0x5F
    bool                        DoNotAutoPickUp;            // +0x60
    bool                        AutoPickUpAfterFirstPickUp; // +0x61
    long                        ItemToSelectUponRemoval;    // +0x64
    ETutorialCategory           TutorialCategory;           // +0x68
    EUIInventoryCategory        UIInventoryCategory;        // +0x6C
};
class CTCInventoryItem : public CTCBase {
public:
    CDefPointer<const CInventoryItemDef> PDef;              // +0x0C
    ETutorialCategory GetTutorialCategory() const;
};

ETutorialCategory CTCInventoryItem::GetTutorialCategory() const
{
    CDefPointer<const CInventoryItemDef> def(PDef);
    return def->TutorialCategory;
}