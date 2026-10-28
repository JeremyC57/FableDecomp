// CTCInventoryExperience::~CTCInventoryExperience() -- deletes and erases every UI component, drops the yes/no box and viewport pointers, then destroys the members and both bases
// Leaf: ??1CTCInventoryExperience@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0057846F, 324 bytes, thiscall, no frame; ends `jmp 0x005C00A0`. Size TU: `pop ecx` after free.
// Identity (proven): the two vtable stores are CTCInventoryExperience's RTTI vtables (0x0124F11C primary, 0x0124F108 for the
//   NHeroInformationScreens::CBase subobject at +0x130, rtti_bases.tsv), the base store 0x0123A3E0 is CBase's own vtable (as
//   the landed sibling ~CTCInventoryClothing 0x005B7049), the tail jump is NInventory::CTCInventoryBase::~CTCInventoryBase
//   0x005C00A0, and the only caller is the scalar deleting destructor 0x0057A28B (call, `test [esp+8],1`, operator delete).
//   The member destructors walk Ego_r.pdb CTCInventoryExperience in reverse at the retail offsets of
//   engine/CTCInventoryExperience.h (TutorialsPlayed +0x248 ... TutorialSoundHandles +0x134).
// Callees: vector<NUISystem::CComponent*>::erase(iterator) 0x00485468, vector<CCharString>::~vector 0x00414EA0,
//   vector<CEnginePrimitiveHandle>::~vector 0x00440C40 (x6), the out-of-line _Rb_tree / _List_base destructors 0x00579616
//   (UpgradableAbilities), 0x005796AB (UpgradableStats), 0x005792DA (Animators), 0x00579FC1 (UndoableAbilityBuys), 0x00579FAA
//   (UndoableStatBuys), 0x0057988C (TutorialSoundHandles), free 0x00BFEA14, ~CTCInventoryBase 0x005C00A0.
// Label correction: catalogued as the gap label "CTCActionUseBuyOrSteal_Finalize".
// Layout: engine/CTCInventoryExperience.h is the flattened retail layout (no bases), so a local view carries the bases and
//   the PDB member names at the header's offsets. The POD vectors' `if (start) free(start)` is `__forceinline`, as the landed
//   ~CTCInventoryClothing.
#pragma optimize("s", on)
extern "C" void __cdecl free(void* block);                    // 0x00BFEA14
class CCharString;
class CEnginePrimitiveHandle;
class CSliderAnimator;
class CYesNo;
class CViewport;
struct SExpSpendUndo;
enum EHeroTrainableStatType { };
enum EHeroAbility { };
enum EScreenType { };
namespace NUISystem { class CComponent { public: virtual ~CComponent(); }; }
namespace std {
template <class _Tp> class allocator { };
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Pair> struct _Select1st { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef _Tp* iterator;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    iterator erase(iterator __position);
    __forceinline ~vector()
    {
        if (_M_start)
            free(_M_start);
    }
};
template <> vector<CCharString, allocator<CCharString> >::~vector();                         // 0x00414EA0
template <> vector<CEnginePrimitiveHandle, allocator<CEnginePrimitiveHandle> >::~vector();   // 0x00440C40
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc> class _Rb_tree {
public:
    void* _M_header;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
    ~_Rb_tree();
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > > class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _M_t;
};
template <class _Tp, class _Alloc> class _List_base {
public:
    void* _M_node;
    ~_List_base();
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    ~list() { }
};
template <class _Tp> struct _Nonconst_traits { };
template <class _Tp, class _Traits> struct _List_iterator { void* _M_node; };
}
namespace NInventory {
class CTCInventoryBase {                                      // vtable 0x01253DBC, retail sizeof 0x130
public:
    virtual ~CTCInventoryBase();                              // 0x005C00A0
    unsigned char _pad_0x04[0x8];
    std::vector<NUISystem::CComponent*> Components;          // +0x0C
    unsigned char _pad_0x18[0x118];
};
}
namespace NHeroInformationScreens {
class CBase {                                                 // vtable 0x0123A3E0
public:
    virtual ~CBase() { }
};
}
class CTCInventoryExperience : public NInventory::CTCInventoryBase, public NHeroInformationScreens::CBase {
public:
    struct ExperienceSpendInfo { long _data[3]; };
    struct ExperienceSpendEvent { long _data[2]; };
    virtual ~CTCInventoryExperience();
    std::map<CCharString, unsigned long> TutorialSoundHandles;                  // +0x134
    std::map<EHeroTrainableStatType, SExpSpendUndo> UndoableStatBuys;           // +0x140
    std::map<EHeroAbility, SExpSpendUndo> UndoableAbilityBuys;                  // +0x14C
    std::vector<CEnginePrimitiveHandle> ExperiencePrimitiveHandles;             // +0x158
    std::vector<CEnginePrimitiveHandle> ExperienceStrengthPrimitiveHandles;     // +0x164
    std::vector<CEnginePrimitiveHandle> ExperienceSkillPrimitiveHandles;        // +0x170
    std::vector<CEnginePrimitiveHandle> ExperienceWeaponMagicPrimitiveHandles;  // +0x17C
    std::vector<CEnginePrimitiveHandle> ExperienceAbilityMagicPrimitiveHandles; // +0x188
    std::vector<CEnginePrimitiveHandle> ExperiencePureMagicPrimitiveHandles;    // +0x194
    bool OpenedExperienceSpending;                                              // +0x1A0
    std::list<CSliderAnimator> Animators;                                       // +0x1A4
    std::_List_iterator<CSliderAnimator, std::_Nonconst_traits<CSliderAnimator> > AnimIt;   // +0x1A8
    long OrigGeneral;                                                           // +0x1AC
    long OrigStrength;                                                          // +0x1B0
    long OrigSkill;                                                             // +0x1B4
    long OrigWill;                                                              // +0x1B8
    long statIndicatorPos;                                                      // +0x1BC
    long statIndicatorMax;                                                      // +0x1C0
    CYesNo* pYesNo;                                                             // +0x1C4
    ExperienceSpendEvent ExperienceSpendAdd;                                    // +0x1C8
    ExperienceSpendEvent ExperienceSpendRemove;                                 // +0x1D0
    CViewport* pViewport;                                                       // +0x1D8
    CViewport* pTextViewport;                                                   // +0x1DC
    std::vector<ExperienceSpendInfo> ExperienceSpendValuesPool;                 // +0x1E0
    std::vector<ExperienceSpendInfo> ExperienceSpendValuesStats;                // +0x1EC
    std::vector<ExperienceSpendInfo> ExperienceSpendValuesAbility;              // +0x1F8
    long ExperienceSpendMagnitude;                                              // +0x204
    std::vector<long> MenuDisplayListExperienceItem;                            // +0x208
    bool CreateBecauseOfUndo;                                                   // +0x214
    long NumItems;                                                              // +0x218
    bool DestroyEverything;                                                     // +0x21C
    EScreenType PreviousScreenType;                                             // +0x220
    std::map<EHeroTrainableStatType, NUISystem::CComponent*> UpgradableStats;   // +0x224
    std::map<EHeroAbility, NUISystem::CComponent*> UpgradableAbilities;         // +0x230
    std::vector<NUISystem::CComponent*> ExpSliders;                             // +0x23C
    std::vector<CCharString> TutorialsPlayed;                                   // +0x248
    float AccumFadeTime;                                                        // +0x254
    NUISystem::CComponent* PMousePointer;                                       // +0x258
    float Counter;                                                              // +0x25C
    long PointsGeneral;                                                         // +0x260
    long PointsStrength;                                                        // +0x264
    long PointsSkill;                                                           // +0x268
    long PointsWill;                                                            // +0x26C
};

CTCInventoryExperience::~CTCInventoryExperience()
{
    for (std::vector<NUISystem::CComponent*>::iterator it = Components.begin(); it != Components.end(); ) {
        delete *it;
        it = Components.erase(it);
    }
    pYesNo = 0;
    pViewport = 0;
    pTextViewport = 0;
}