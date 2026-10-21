// CTCInventoryExperience::CTCInventoryExperience(CThing*) -- builds the CTCInventoryBase and NHeroInformationScreens::CBase bases, stores both vtables, constructs the tutorial-sound / undo / upgradable maps, the primitive-handle and spend vectors and the slider-animator list, and clears the fade time, mouse pointer and the rebuild / undo / opened flags
// Leaf: ??0CTCInventoryExperience@@QAE@PAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057832D, 322 bytes, `ret 4`. Size TU with a frame pointer:
//   `#pragma optimize("y", off)` + `("s", on)` (the list's default allocator argument lives in the argument slot,
//   `lea eax,[ebp+0Bh]`). Vtables 0x0124F11C (primary) and 0x0124F108 (NHeroInformationScreens::CBase at +0x130).
// Callees: CTCInventoryBase::CTCInventoryBase(CThing*) @ 0x005BFF07, NHeroInformationScreens::CBase::CBase @ 0x0066E734
//   (as the landed sibling ctor CTCInventoryStats 0x005CD2A6), the STLport map default ctors @ 0x00579B0A / 0x0057A0E8 /
//   0x0057A106 / 0x0057986E / 0x00579842 (manifest "CDisplayManager::CopyBackBufferToTexture" is propagated) and the
//   list ctor list(const allocator_type&) @ 0x005792BF (manifest "CircularListInitialize").
// Shape: bases, the two vtable stores, the members in declaration order (vectors zeroed inline through ebx); the vector
//   ctor is STLport's (`_Vector_base` + `_STLP_alloc_proxy` end-of-storage built from the allocator) -- with three plain
//   zeroing initialisers the next map ctor's `lea ecx` is scheduled ahead of the last vector's stores; the allocator
//   has a user ctor (else the list's default-argument temporary is zeroed with `stosb`); the three
//   flags DestroyEverything, CreateBecauseOfUndo, OpenedExperienceSpending are cleared in the body, in that order.
// Layout: Ego_r.pdb member order with the retail 12-byte STLport maps/vectors and the 4-byte list (Animators +0x1A4 and
//   AnimIt +0x1A8 as read by GlobalUpdate 0x0056FC5D; the spend vectors +0x1E0/+0x1EC/+0x1F8 as the landed
//   UpdateExperienceSpendLevelUp 0x005721FA). The PDB header is flattened with debug 16-byte containers, so not used.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CThing;
struct CComponent;
struct CViewport;
struct CYesNo;

struct CTCInventoryBase {
    virtual void v00();
    unsigned char _base[0x12c];                             // +0x04 .. +0x130
    CTCInventoryBase(CThing* thing);                        // 0x005BFF07
};
namespace NHeroInformationScreens {
struct CBase {                                              // vtable 0x0123A3E0
    virtual void v00();
    CBase();                                                // 0x0066E734
};
}

struct CTutorialSoundMap { unsigned char _body[0xc]; CTutorialSoundMap(); };         // 0x00579B0A
struct CStatUndoMap { unsigned char _body[0xc]; CStatUndoMap(); };                   // 0x0057A0E8
struct CAbilityUndoMap { unsigned char _body[0xc]; CAbilityUndoMap(); };             // 0x0057A106
struct CStatComponentMap { unsigned char _body[0xc]; CStatComponentMap(); };         // 0x0057986E
struct CAbilityComponentMap { unsigned char _body[0xc]; CAbilityComponentMap(); };   // 0x00579842

template <class T> struct CAllocator { CAllocator() { } };
template <class V, class A> struct CAllocProxy : A {      // STLport _STLP_alloc_proxy
    V _M_data;
    CAllocProxy(const A& a, V p) : A(a), _M_data(p) { }
};
template <class T> struct CVectorBase {                     // STLport _Vector_base
    T* _M_start;
    T* _M_finish;
    CAllocProxy<T*, CAllocator<T> > _M_end_of_storage;
    CVectorBase(const CAllocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0) { }
};
template <class T> struct CVector : CVectorBase<T> {        // STLport vector (release: 0xc bytes)
    explicit CVector(const CAllocator<T>& a = CAllocator<T>()) : CVectorBase<T>(a) { }
};
struct CSliderAnimator;
struct SAnimatorNode;
struct CAnimatorList {                                      // STLport list<CSliderAnimator> (release: node pointer)
    SAnimatorNode* _M_node;
    explicit CAnimatorList(const CAllocator<CSliderAnimator>& a = CAllocator<CSliderAnimator>());   // 0x005792BF
};
struct CAnimatorIterator {
    SAnimatorNode* _M_node;
    CAnimatorIterator() { }
};
struct CEnginePrimitiveHandle { long Handle; };
struct ExperienceSpendEvent { bool enable; bool initiate; long time; };
struct ExperienceSpendInfo { long currentSpend; long initialSpend; long levelUpSpend; };
class CCharString { void* PStringData; };

struct CTCInventoryExperience : CTCInventoryBase, NHeroInformationScreens::CBase {
    CTutorialSoundMap TutorialSoundHandles;                                 // +0x134
    CStatUndoMap UndoableStatBuys;                                          // +0x140
    CAbilityUndoMap UndoableAbilityBuys;                                    // +0x14C
    CVector<CEnginePrimitiveHandle> ExperiencePrimitiveHandles;             // +0x158
    CVector<CEnginePrimitiveHandle> ExperienceStrengthPrimitiveHandles;     // +0x164
    CVector<CEnginePrimitiveHandle> ExperienceSkillPrimitiveHandles;        // +0x170
    CVector<CEnginePrimitiveHandle> ExperienceWeaponMagicPrimitiveHandles;  // +0x17C
    CVector<CEnginePrimitiveHandle> ExperienceAbilityMagicPrimitiveHandles; // +0x188
    CVector<CEnginePrimitiveHandle> ExperiencePureMagicPrimitiveHandles;    // +0x194
    bool OpenedExperienceSpending;                                          // +0x1A0
    CAnimatorList Animators;                                                // +0x1A4
    CAnimatorIterator AnimIt;                                               // +0x1A8
    long OrigGeneral;                                                       // +0x1AC
    long OrigStrength;                                                      // +0x1B0
    long OrigSkill;                                                         // +0x1B4
    long OrigWill;                                                          // +0x1B8
    long statIndicatorPos;                                                  // +0x1BC
    long statIndicatorMax;                                                  // +0x1C0
    CYesNo* pYesNo;                                                         // +0x1C4
    ExperienceSpendEvent ExperienceSpendAdd;                                // +0x1C8
    ExperienceSpendEvent ExperienceSpendRemove;                             // +0x1D0
    CViewport* pViewport;                                                   // +0x1D8
    CViewport* pTextViewport;                                               // +0x1DC
    CVector<ExperienceSpendInfo> ExperienceSpendValuesPool;                 // +0x1E0
    CVector<ExperienceSpendInfo> ExperienceSpendValuesStats;                // +0x1EC
    CVector<ExperienceSpendInfo> ExperienceSpendValuesAbility;              // +0x1F8
    long ExperienceSpendMagnitude;                                          // +0x204
    CVector<long> MenuDisplayListExperienceItem;                            // +0x208
    bool CreateBecauseOfUndo;                                               // +0x214
    long NumItems;                                                          // +0x218
    bool DestroyEverything;                                                 // +0x21C
    long PreviousScreenType;                                                // +0x220 EScreenType
    CStatComponentMap UpgradableStats;                                      // +0x224
    CAbilityComponentMap UpgradableAbilities;                               // +0x230
    CVector<CComponent*> ExpSliders;                                        // +0x23C
    CVector<CCharString> TutorialsPlayed;                                   // +0x248
    float AccumFadeTime;                                                    // +0x254
    CComponent* PMousePointer;                                              // +0x258
    float Counter;                                                          // +0x25C
    long PointsGeneral;                                                     // +0x260
    long PointsStrength;                                                    // +0x264
    long PointsSkill;                                                       // +0x268
    long PointsWill;                                                        // +0x26C
    virtual void v00();
    CTCInventoryExperience(CThing* thing);
};

CTCInventoryExperience::CTCInventoryExperience(CThing* thing)
    : CTCInventoryBase(thing), AccumFadeTime(0.0f), PMousePointer(0)
{
    DestroyEverything = false;
    CreateBecauseOfUndo = false;
    OpenedExperienceSpending = false;
}