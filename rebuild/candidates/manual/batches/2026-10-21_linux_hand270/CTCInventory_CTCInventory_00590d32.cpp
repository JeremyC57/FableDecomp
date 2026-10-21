// CTCInventory::CTCInventory(CThing* owner) -- build the hero inventory component: both bases, the swap/shortcut state, the
// edit-box list, the quick-access / context-sensitive / primitive-handle / opinion vectors and the two menu maps; then size
// the quick-access vectors from the quick-access slot count, the context-sensitive ones from their slot count, and the
// three opinion-expression vectors to six entries
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00590D32, 471 bytes, `ret 4` (frame pointer, `leave`).
// Leaf: ??0CTCInventory
// Callees: NInventory::CTCInventoryBase::CTCInventoryBase(CThing*) @ 0x005BFF07, NHeroInformationScreens::CBase::CBase @ 0x0066E734,
// list<CEditBox*>(const allocator&) @ 0x005931DA, map<long, CComponent*>() @ 0x00593ED0 (x2), CTCBase 0x00686D50 -> 0x00449960 ->
// +0x0C -> the quick-access (0x00445950) / context-sensitive (0x00445960) slot counts, vector<unsigned>::resize(n, const T&)
// @ 0x004428F0 (x4), the handle vectors' 0x00443F90 (x2), 0x004433F0 and 0x00594DF4 (one-argument sizing calls).
// Layout: as the landed destructor 0x00589C55 (Ego_r.pdb CTCInventory at retail offsets); vtables 0x01250DE4 / 0x01250DD0.
// Shape: size TU with a frame pointer; the list's default allocator tag lives in the high byte of the dead `owner` slot
// (`lea eax,[ebp+0Bh]`) and the resize fill value in `owner`'s slot; the POD vectors are STLport vectors built through the
// alloc-proxy third member (`_M_end_of_storage(a, 0)`) -- a plain three-zero initialiser list schedules the map's
// `lea ecx` one store early.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CThing;
namespace NUISystem { struct CComponent; }

namespace NInventory {
class CTCInventoryBase {                                      // vtable 0x01253DBC, retail sizeof 0x130
public:
    CTCInventoryBase(CThing* owner);                          // 0x005BFF07
    virtual ~CTCInventoryBase();                              // 0x005C00A0
    unsigned char _body[0x12C];
};
}
namespace NHeroInformationScreens {
class CBase {                                                 // vtable 0x0123A3E0
public:
    CBase();                                                  // 0x0066E734
    virtual ~CBase() {}
};
}

template<class T> struct CAllocator { CAllocator() { } };
struct CFadeInFadeOutBase { unsigned char _body[0x0c]; };
struct CRightHandedSet { float m[6]; };

struct CEditBoxList {                                         // list<NUISystem::CEditBox*> (one word)
    void* _M_node;
    explicit CEditBoxList(const CAllocator<void*>& a = CAllocator<void*>());   // 0x005931DA
    ~CEditBoxList();                                          // 0x005931F5
};
template<class T, class A> struct CAllocProxy {             // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* _M_data;
    CAllocProxy(const A&, T* p) : _M_data(p) { }
};
template<class T> struct CPODVector {              // STLport vector of a trivially destructible element (12 bytes)
    T* _M_start;
    T* _M_finish;
    CAllocProxy<T, CAllocator<T> > _M_end_of_storage;
    explicit CPODVector(const CAllocator<T>& a = CAllocator<T>()) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) { }
    ~CPODVector();
    void resize(unsigned long n, const T& x);                 // 0x004428F0 (vector<unsigned int>)
    void reserve(unsigned long n);                            // 0x004433F0 (long) / 0x00594DF4 (fade times)
};
struct CRetailHandleVector {                                  // vector<CEnginePrimitiveHandle>
    void* m_pFirst;
    void* m_pLast;
    void* m_pEnd;
    CRetailHandleVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
    ~CRetailHandleVector();                                   // 0x00440C40
    void reserve(unsigned long n);                            // 0x00443F90
};
struct CComponentByLongMap {                                  // map<long, NUISystem::CComponent*>
    void* _M_header; unsigned long _M_node_count; long _M_key_compare;
    CComponentByLongMap();                                    // 0x00593ED0
    ~CComponentByLongMap();                                   // 0x00593AC7
};

struct CGameProfile {
    unsigned long GetNumQuickAccessSlots() const;             // 0x00445950
    unsigned long GetNumContextSensitiveSlots() const;        // 0x00445960
};
struct CPlayer {
    unsigned char _pad00[0x0C];
    CGameProfile* PProfile;                                   // +0x0C
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                                 // 0x00449960
};

class CTCInventory : public NInventory::CTCInventoryBase, public NHeroInformationScreens::CBase {   // vtables 0x01250DE4 / 0x01250DD0
public:
    long                     ThingToAssign;                   // +0x134
    long                     ThingSlotToSwap;                 // +0x138
    NUISystem::CComponent*   PComponentSlotToSwap;            // +0x13c
    long                     ScreenToShortcut;                // +0x140
    CEditBoxList             EditBoxes;                       // +0x144
    CPODVector<unsigned long> QuickAccessItems;               // +0x148
    CPODVector<long>         ContextSensitiveItems;           // +0x154
    CPODVector<CFadeInFadeOutBase> ContextSensitiveFadeTimes; // +0x160
    CRetailHandleVector      InventoryMainPrimitiveHandles;   // +0x16c
    CRetailHandleVector      QuickAccessPrimitiveHandles;     // +0x178
    CRetailHandleVector      QuickAccessUnusableIconPrimitiveHandles;   // +0x184
    CRightHandedSet          OldObjectRHSet;                  // +0x190
    float                    OldRotationAngle;                // +0x1a8
    CRightHandedSet          CurrentObjectRHSet;              // +0x1ac
    float                    CurrentRotationAngle;            // +0x1c4
    long                     ItemRHSetsApplyTo;               // +0x1c8
    bool                     EnableQuickAccessAssign;         // +0x1cc
    bool                     EnableQuickAccessRemove;         // +0x1cd
    bool                     QuickAccessFlashState;           // +0x1ce
    long                     QuickAccessFlashTimer;           // +0x1d0
    bool                     ContextSensitiveUpdate;          // +0x1d4
    CPODVector<unsigned long> NumberOfCSOpinionExpressions;   // +0x1d8
    CPODVector<unsigned long> LastCSOpinionExpression;        // +0x1e4
    CPODVector<unsigned long> LastButOneCSOpinionExpression;  // +0x1f0
    CComponentByLongMap      ItemsMenuEntries;                // +0x1fc
    CComponentByLongMap      AssignedIcons;                   // +0x208
    NUISystem::CComponent*   PMouse_Pointer;                  // +0x214
    NUISystem::CComponent*   PTop_Level_List;                 // +0x218
    long                     CurrentOpenMenu;                 // +0x21c
    float                    AccumFadeTime;                   // +0x220
    CPlayerManager* GetPlayerManager() const;                 // 0x00686D50 (CTCBase)
    CTCInventory(CThing* owner);
    virtual ~CTCInventory();
};

CTCInventory::CTCInventory(CThing* owner)
    : NInventory::CTCInventoryBase(owner),
      ThingToAssign(0),
      ThingSlotToSwap(0),
      PComponentSlotToSwap(0),
      ScreenToShortcut(-1),
      OldRotationAngle(0.0f),
      CurrentRotationAngle(0.0f),
      ItemRHSetsApplyTo(0),
      ContextSensitiveUpdate(false),
      AccumFadeTime(0.0f)
{
    unsigned long quickAccessSlots = GetPlayerManager()->GetMainPlayer()->PProfile->GetNumQuickAccessSlots();
    QuickAccessItems.resize(quickAccessSlots, 0);
    QuickAccessUnusableIconPrimitiveHandles.reserve(quickAccessSlots);
    QuickAccessPrimitiveHandles.reserve(quickAccessSlots * 2);
    unsigned long contextSensitiveSlots = GetPlayerManager()->GetMainPlayer()->PProfile->GetNumContextSensitiveSlots();
    ContextSensitiveItems.reserve(contextSensitiveSlots);
    ContextSensitiveFadeTimes.reserve(contextSensitiveSlots);
    NumberOfCSOpinionExpressions.resize(6, 0);
    LastCSOpinionExpression.resize(6, 0);
    LastButOneCSOpinionExpression.resize(6, 0);
    PMouse_Pointer = 0;
    PTop_Level_List = 0;
    CurrentOpenMenu = 0;
}
