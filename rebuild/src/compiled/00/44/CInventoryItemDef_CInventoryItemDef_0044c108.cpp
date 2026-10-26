// CInventoryItemDef::CInventoryItemDef() -- default constructor of a def whose only constructed member is its CEngineGraphic: the CSubDefClassBase base out of line, this class's vtable, then Graphic zeroed (bank index, animation step, render size, additive-alpha flag, type)
// Leaf: ??0CInventoryItemDef@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0044C108, 39 bytes, thiscall, plain `ret`, returns this (a size-optimised /Os def module: unaligned entry).
// Identity (proven): CInventoryItemDef::CInventoryItemDef() (FableWin.pdb name set: ??0CInventoryItemDef@@QAE@XZ); evidence: RTTI of the stored vtable 0x01231DBC is .?AVCInventoryItemDef@@ (base CSubDefClassBase); the members it zeroes, +0x25..+0x32, are Ego_r.pdb CInventoryItemDef::Graphic (CEngineGraphic at +37)
// Label: the manifest's "CSimVoice" is propagated (family J056: five byte-similar def constructors; the stored vtable names each class).
// Callees: CSubDefClassBase::CSubDefClassBase() 0x0044C0C0 (??0CSubDefClassBase@@QAE@XZ, FableWin.pdb; it stores the CSubDefClassBase vtable 0x01231D54).
// Layout: Ego_r.pdb CInventoryItemDef (Graphic +0x25); CEngineGraphic is a packed 14-byte class (BankIndex, AnimStep, RenderSizeX, AdditiveAlpha, Type).
// Model: local -- engine/CResource.h, CDefClassBase.h and CParentDefClassBase.h flatten the vptr (data-only); engine/CInventoryItemDef.h is data-only as well.
#pragma optimize("s", on)
class CResourceList;
class CResource {                                        // Ego_r.pdb 0x1C (CIVCountedPointeeBase: vptr, IVRefCount)
public:
    CResource();                                         // 0x009FBEC0
    virtual ~CResource();                                // 0x009FC550
    unsigned long  IVRefCount;                           // +0x04
    CResourceList* ResourceList;                         // +0x08
    CResource*     PrevResource;                         // +0x0C
    CResource*     NextResource;                         // +0x10
    unsigned long  ResourceSize;                         // +0x14
    unsigned long  LastUsedFrame;                        // +0x18
};
class CDefPointeeBase : public CResource {               // vtable 0x01230BA0
public:
    virtual ~CDefPointeeBase() { }
};
class CDefinitionManager;
#pragma pack(push, 1)
class CDefClassBase : public CDefPointeeBase {           // vtable 0x01230BCC
public:
    class CSubDefInfo { public:
        long DefIndex;
        long OriginalParentDefIndex;
    };
    CDefinitionManager* PDefManager;                     // +0x1C
    unsigned long       GlobalIndex;                     // +0x20
    unsigned char       Setup : 1;                       // +0x24
    unsigned char       Template : 1;
    unsigned char       DefaultValsApplied : 1;
};
class CEngineGraphic {                                   // Ego_r.pdb 14 bytes, packed
public:
    CEngineGraphic() : BankIndex(0), AnimStep(0.0f), RenderSizeX(0.0f), AdditiveAlpha(false), Type(0) { }
    unsigned long BankIndex;                             // +0x00
    float         AnimStep;                              // +0x04
    float         RenderSizeX;                           // +0x08
    bool          AdditiveAlpha;                         // +0x0C
    unsigned char Type;                                  // +0x0D (EEngineGraphicType)
};
#pragma pack(pop)
class CSubDefClassBase : public CDefClassBase {         // vtable 0x01231D54, Ego_r.pdb 0x25 (packed, no members of its own)
public:
    CSubDefClassBase();                                  // 0x0044C0C0
};
enum EInventoryActionType { };                    // Ego_r.pdb (enumerators not needed)
enum EInventoryObjectOrientation { };             // Ego_r.pdb (enumerators not needed)
enum ETCInterfaceType { };                        // Ego_r.pdb (enumerators not needed)
enum ETutorialCategory { };                       // Ego_r.pdb (enumerators not needed)
enum EUIInventoryCategory { };                    // Ego_r.pdb (enumerators not needed)
class CInventoryItemDef : public CSubDefClassBase {                     // vtable 0x01231DBC
public:
    CInventoryItemDef();
    CEngineGraphic Graphic;                             // +0x25
    unsigned long  ItemDescription;                     // +0x34
    unsigned long  ItemDetails;                         // +0x38
    long           InventoryCategory;                   // +0x3C
    long           MaxNumberItems;                      // +0x40
    long           SlotIndex;                           // +0x44
    float          ActivationTime;                      // +0x48
    EInventoryActionType UseButtonAction;                     // +0x4C
    ETCInterfaceType InventoryType;                       // +0x50
    EInventoryObjectOrientation Orientation;                         // +0x54
    long           HeroAbilityDef;                      // +0x58
    bool           IsSellable;                          // +0x5C
    bool           IsBuyable;                           // +0x5D
    bool           IsConfiscatable;                     // +0x5E
    bool           DoNotPersistUntilQuestCompleted;     // +0x5F
    bool           DoNotAutoPickUp;                     // +0x60
    bool           AutoPickUpAfterFirstPickUp;          // +0x61
    long           ItemToSelectUponRemoval;             // +0x64
    ETutorialCategory TutorialCategory;                    // +0x68
    EUIInventoryCategory UIInventoryCategory;                 // +0x6C
};
CInventoryItemDef::CInventoryItemDef()
{
}