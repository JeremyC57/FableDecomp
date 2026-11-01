// CTCInventoryQuests::ProcessButtonAReleased() -- in test-quest mode activate the selected test quest and close the inventory; otherwise, outside the inventory screen in item-selection state, close it when the quest-activate action succeeds; then the base handler
// Leaf: ?ProcessButtonAReleased@CTCInventoryQuests@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0061B590, 93 bytes, thiscall, two tails `jmp` to the base handler.
// Identity (proven): the only reference is slot 58 of vtable 0x012585B4 (RTTI .?AVCTCInventoryQuests@@: CTCInventoryQuests ->
//   NInventory::CTCInventoryBase -> CTCBase -> CBaseClassNonCopyable -> CBaseClass, NHeroInformationScreens::CBase at +0x130, secondary
//   vtable 0x012585A0); slot 58 of the base vtable 0x01253DBC is NInventory::CTCInventoryBase::ProcessButtonAReleased 0x005BC65C, the tail
//   target. The Ego_r donor order ProcessInventoryInputUp, ProcessInventoryInputDown, ProcessButtonAReleased, ProcessButtonBReleased equals
//   the retail order 0x0061B530 (slot 38), 0x0061B560 (39), 0x0061B590 (58), 0x0061B5F0 (59). Name: Ego_r.pdb
//   ?ProcessButtonAReleased@CTCInventoryQuests@@UAEXXZ (the catalog label agrees).
// Callees: CTCInventoryQuests::ActivateSelectedTestQuest 0x0061AB30 (Ego_r donor order GetTestQuestsToDisplay, MoveToNextTestQuest,
//   MoveToPreviousTestQuest, ActivateSelectedTestQuest, DeactivateSelectedTestQuest = retail 0x0061A8A0 .. 0x0061AC60), virtual slot 32
//   AddCloseInventoryEvent (+0x80; base 0x005BC527), NHeroInformationScreens::CBase virtual slot 3 IsInventoryScreen (+0x0C on the +0x130
//   subobject; base 0x004D2E41 `mov al,1`, CTCInventoryQuests 0x00619FD0 `xor al,al`), virtual slot 70 ProcessQuestActivateAction (+0x118;
//   CTCInventoryQuests 0x006223C0), NInventory::CTCInventoryBase::ProcessButtonAReleased 0x005BC65C.
// Virtual slots of NInventory::CTCInventoryBase (base vtable 0x01253DBC): CTCBase 0..27 (the landed CTCBase order), then the class's own
//   virtuals 28..85, named by aligning the base bodies (packed 1-byte-aligned section 0x004E7D3B.., 0x0057A230.., 0x005BC4C0..0x005C0121)
//   with the Ego_r / FableWin donor order and each body's `ret N` (e.g. slots 42-45 are the four 0x23-byte repeat-delay getters in donor
//   order, slots 50-65 the button handlers, 66-76 FableWin's inline header block WhitePressed .. GetCategoryNameFromAssociation).
// Layout (retail, Ego_r.pdb members at STLport sizes; no engine header exists for these classes): CTCInventoryBase 0x130 with
//   InventorySelectionState +0x1C; NHeroInformationScreens::CBase (vptr only) +0x130; CTCInventoryQuests: InventoryQuestCardSelect-
//   PrimitiveHandles +0x134, QuestCardItemList +0x140, IsGuildValid +0x14C, InventoryScreen +0x14D, QuestCardCount +0x150, Teleport
//   +0x154, QuestCardActivatable +0x155, EnableQuestCardListInitialise +0x156, ActiveTestQuestMode +0x157 (the trailing members are not
//   modelled; no size claim).
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
class CThing;
class CCharString;
class CWideString;
class CPersistContext;
class CTCParamsBase;
class CEnginePrimitivePrepareInfo;
class CInventoryDef;
class CGraphicDataBank;
class CEnginePrimitiveHandle;
class CQuestCardItem;
enum ETCInterfaceType { };
enum EPlayerMode { };
namespace NTCMessages { class CMessageBase; }
namespace NUISystem { class CComponent; }
namespace std {
template <class _Tp> class allocator { public: };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
template <class _Tp, class _Alloc> class _Vector_base {
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> { };
}
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };
class C2DBoxF { public: float Left, Top, Right, Bottom; };
class CPresentationIntervalDesc { public: bool PresentImmediate; bool PresentImmediateIfSyncMissed; long VSyncCount; };
enum ESelectionState { };
class CTCBase : public CBaseClassNonCopyable {             // engine/CTCBase.h flattens the vptr: local polymorphic model
protected:
    virtual void OnSerialise(CPersistContext&, bool);      // 1
public:
    virtual void OnRemove();                               // 2
    virtual void ConstructFromParams(const CTCParamsBase&);
    virtual void OnCreate();
    virtual void InitialActivate();
    virtual void OnInitialActivate();
    virtual void RenderUpdate(float);
    virtual bool HasFrameUpdate() const;
    virtual void FrameUpdate();
    virtual void BulletTimeFrameUpdate();                  // 10
    virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&);
    virtual void Copy(const CTCBase&);
    virtual void OnDie();
    virtual void OnKill();
    virtual void OnLevelUnload();
    virtual void OnSetInLimbo(bool);
    virtual bool Validate(CCharString&) const;
    virtual bool IsFreeForDeletion() const;
    virtual bool IsSerialiseCreatable() const;
    virtual bool IsSerialisableForHeroState() const;       // 20
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
    virtual bool IsComponentSerialisable() const;
    virtual bool IsDriver() const;
    virtual bool IsCopyable() const;
    virtual void GlobalUpdate(bool);
    virtual void HandleMessage(const NTCMessages::CMessageBase&);   // 27
    CThing* ParentThing;                                   // +0x04
    bool ToBeRemoved;                                      // +0x08
    bool IsInGlobalUpdateTCs;                              // +0x09
};
namespace NInventory {
class CCategory;
class CItem;
class CTCInventoryBase : public CTCBase {
public:
    enum EInventorySelectionState { };
    virtual void UpdateMenuScrollButtonsCategory();        // 28
    virtual void UpdateMenuScrollButtonsItem();            // 29
    virtual void OpenInventory();                          // 30
    virtual void CloseInventory();                         // 31
    virtual void AddCloseInventoryEvent();                 // 32 (+0x80)  0x005BC527
    virtual void OnItemSelected(long, bool&) = 0;          // 33 (pure)
    virtual void OnItemSelectedInHand(CThing&, bool&);     // 34
    virtual void OnItemDeselected(long, bool&);            // 35
    virtual void OnItemHighlighted(long);                  // 36
    virtual void OnChangeCategory();                       // 37
    virtual void ProcessInventoryInputUp();                // 38
    virtual void ProcessInventoryInputDown();              // 39
    virtual void ProcessInventoryInputLeft();              // 40
    virtual void ProcessInventoryInputRight();             // 41
    virtual float GetInitialCategoryRepeatDelay() const;   // 42
    virtual float GetContinuedCategoryRepeatDelay() const; // 43
    virtual float GetInitialItemRepeatDelay() const;       // 44
    virtual float GetContinuedItemRepeatDelay() const;     // 45
    virtual void SetInventorySelectionState(EInventorySelectionState);   // 46
    virtual CThing* GetItemOfTypeInInventory(long);        // 47
    virtual bool GetInventoryShoppingValidity();           // 48
    virtual bool GetInventoryGuildValidity();              // 49
    virtual void ProcessButtonAPressed();                  // 50
    virtual void ProcessButtonBPressed();
    virtual void ProcessButtonXPressed();
    virtual void ProcessButtonYPressed();
    virtual void ProcessButtonAHeld();                     // 54
    virtual void ProcessButtonBHeld();
    virtual void ProcessButtonXHeld();
    virtual void ProcessButtonYHeld();
    virtual void ProcessButtonAReleased();                 // 58  0x005BC65C
    virtual void ProcessButtonBReleased();
    virtual void ProcessButtonXReleased();
    virtual void ProcessButtonYReleased();
    virtual void ProcessDPadUpReleased();                  // 62
    virtual void ProcessDPadDownReleased();
    virtual void ProcessDPadLeftReleased();
    virtual void ProcessDPadRightReleased();
    virtual void ProcessButtonWhitePressed();              // 66
    virtual void ProcessButtonWhiteReleased();
    virtual void ProcessTradeAction();
    virtual void ProcessAugmentAction();
    virtual bool ProcessQuestActivateAction();             // 70 (+0x118)
    virtual void IncrementQuestCardItemSelection();
    virtual void DecrementQuestCardItemSelection();
    virtual void ReturnAllConfiscatedItems();              // 73
    virtual void RemoveAllConfiscatedItems();
    virtual bool GetIsEnabled() const;                     // 75
    virtual CWideString GetCategoryNameFromAssociation(CThing*) const;
    virtual NUISystem::CComponent* ConstructScreen(NUISystem::CComponent*);
    virtual void GetItemDescription(CWideString&, long, bool) const;    // 78
    virtual void RemoveItemFromInventory(long, long);
    virtual bool AddItemToInventory(long, long, bool, bool, long, bool);
    virtual std::vector<NUISystem::CComponent*>& GetComponents();       // 82
protected:
    virtual bool AddItemToInventory(CThing&, bool, bool, long, bool);
    virtual void OnItemReturnedToInventory(CThing&);
    virtual void OnItemAdded(long, bool, bool);
    virtual void ConfiscateItems(const std::vector<CItem>&);            // 85
public:
    std::vector<NUISystem::CComponent*> Components;        // +0x0C
    bool FirstRenderFrame;                                 // +0x18
    EInventorySelectionState InventorySelectionState;      // +0x1C
    std::vector<CCategory> Categories;                     // +0x20
    long HighlightedItemNumOwned;                          // +0x2C
    long HighlightedItemMaxOwned;
    bool CanItemBeQuickAssigned;
    bool CanItemBeUsed;
    ESelectionState MenuArrowHighlightsCategoryUp;         // +0x38
    ESelectionState MenuArrowHighlightsCategoryDn;
    ESelectionState MenuArrowHighlightsItemUp;
    ESelectionState MenuArrowHighlightsItemDn;
    ESelectionState TriggerStateLeft;
    ESelectionState TriggerStateRight;
    double LastRenderTime;                                 // +0x50
    bool InventoryOpen;
    CPresentationIntervalDesc OldIntervalDesc;             // +0x5C
    bool OldIntervalDescInitialised;
    CDefPointer<const CInventoryDef> PDef;                 // +0x68
    long GlobalDefIndex;
    long SelectedCategoryIndex;
    CGraphicDataBank* GraphicManager;
    CCountedPointer<std::vector<CItem> > SerialisedItems;  // +0x78
    CCountedPointer<std::vector<CCharString> > SerialisedSelectedItems;
    bool Dying;                                            // +0x88
    bool CategoryMoving;
    float CategoryMovement;
    float CategoryMovementDirection;
    std::vector<long> MenuDisplayListItem;                 // +0x94
    std::vector<long> MenuDisplayListCategory;
    std::vector<CItem> ConfiscatedItems;                   // +0xAC
    CThing* pBuildingThing;                                // +0xB8
    C2DBoxF InventoryAnimRenderBox;
    CRightHandedSet InventoryAnimCurrentRHSet;             // +0xCC
    CRightHandedSet InventoryAnimLastRHSet;
    float InventoryAnimCurrentXRotationAngle;              // +0xFC
    float InventoryAnimLastXRotationAngle;
    float InventoryAnimCurrentYRotationAngle;
    float InventoryAnimLastYRotationAngle;
    float InventoryAnimCurrentZRotationAngle;
    float InventoryAnimLastZRotationAngle;
    float InventoryAnimXRotationAngleGain;
    float InventoryAnimYRotationAngleGain;
    float InventoryAnimZRotationAngleGain;
    bool EnableAnimXAxis;                                  // +0x120
    bool EnableAnimZAxis;
    long InventoryAnimThingDefIndex;
    long RHSetInvItemDefIndex;
    long ParentMapNumber;                                  // +0x12C
};
}
typedef char CTCInventoryBaseExtent[sizeof(NInventory::CTCInventoryBase) == 0x130 ? 1 : -1];
namespace NHeroInformationScreens {
enum EScreenType { };
class CBase {
public:
    virtual CWideString GetScreenName() const = 0;         // 0
    virtual EPlayerMode GetPlayerMode() const = 0;         // 1
    virtual EScreenType GetScreenType() const = 0;         // 2
    virtual bool IsInventoryScreen() const;                // 3 (+0x0C)  0x004D2E41
};
}
class CTCInventoryQuests : public NInventory::CTCInventoryBase, public NHeroInformationScreens::CBase {
public:
    virtual bool IsInventoryScreen() const;                // 0x00619FD0
    virtual bool ProcessQuestActivateAction();             // 0x006223C0
    virtual void ProcessButtonAReleased();
    void ActivateSelectedTestQuest();                      // 0x0061AB30
    std::vector<CEnginePrimitiveHandle> InventoryQuestCardSelectPrimitiveHandles;   // +0x134
    std::vector<CQuestCardItem> QuestCardItemList;         // +0x140
    bool IsGuildValid;                                     // +0x14C
    bool InventoryScreen;                                  // +0x14D
    long QuestCardCount;                                   // +0x150
    bool Teleport;                                         // +0x154
    bool QuestCardActivatable;                             // +0x155
    bool EnableQuestCardListInitialise;                    // +0x156
    bool ActiveTestQuestMode;                              // +0x157
};

void CTCInventoryQuests::ProcessButtonAReleased()
{
    if (ActiveTestQuestMode) {
        ActivateSelectedTestQuest();
        AddCloseInventoryEvent();
    } else if (!IsInventoryScreen() && InventorySelectionState == 1 && ProcessQuestActivateAction()) {
        AddCloseInventoryEvent();
    }
    NInventory::CTCInventoryBase::ProcessButtonAReleased();
}