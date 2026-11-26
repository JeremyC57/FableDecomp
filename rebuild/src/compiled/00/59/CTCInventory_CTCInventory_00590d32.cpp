// CTCInventory::CTCInventory(CThing& owner) -- build the hero inventory component: both bases, the swap/shortcut state, the
// edit-box list, the quick-access / context-sensitive / primitive-handle / opinion vectors and the two menu maps; then size
// the quick-access vectors from the quick-access slot count, the context-sensitive ones from their slot count, and the
// three opinion-expression vectors to six entries
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00590D32, 471 bytes, `ret 4` (frame pointer, `leave`).
// Leaf: ??0CTCInventory
// Naming (NF456, r454b): the constructor takes `CThing&`, ??0CTCInventory@@QAE@AAVCThing@@@Z as both PDB name sets spell it (was ??0CTCInventory@@QAE@PAVCThing@@@Z); a reference passes the same pointer, so the object is unchanged.
// Naming (NF449 gen, 2026-11-25): callee-only respell of this row's base / member construct calls to the PDB constructor spelling
//   (T1 tracker; target self rows' Leaf): the row's own bodiless model ctor takes the PDB parameter types, call sites pass *x:
//   0x005BFF07 ??0CTCInventoryBase@NInventory@@QAE@AAVCThing@@@Z (was ??0CTCInventoryBase@NInventory@@QAE@PAVCThing@@@Z)
// NF64 (round 7, q10639): the four resized members (QuickAccessItems, NumberOfCSOpinionExpressions, LastCSOpinionExpression,
//   LastButOneCSOpinionExpression; Ego_r.pdb vector<long>) retyped from the local CPODVector<unsigned long> to stlport/fable_containers.h
//   std::vector<long> (resize kept out of line, FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE): the calls bind
//   ?resize@?$vector@JV?$allocator@J@std@@@std@@QAEXIABJ@Z at 0x004428F0. The other local stand-ins are left as landed.
// Callees: NInventory::CTCInventoryBase::CTCInventoryBase(CThing*) @ 0x005BFF07, NHeroInformationScreens::CBase::CBase @ 0x0066E734,
// list<CEditBox*>(const allocator&) @ 0x005931DA, map<long, CComponent*>() @ 0x00593ED0 (x2), CTCBase::PeekPlayerManager 0x00686D50 -> CPlayerManager::GetMainPlayer() const 0x00449960 ->
// +0x0C -> the quick-access (0x00445950) / context-sensitive (0x00445960) slot counts, vector<long>::resize(n, const long&)
// @ 0x004428F0 (x4), the handle vectors' 0x00443F90 (x2), 0x004433F0 and 0x00594DF4 (one-argument sizing calls).
// Layout: as the landed destructor 0x00589C55 (Ego_r.pdb CTCInventory at retail offsets); vtables 0x01250DE4 / 0x01250DD0.
// Shape: size TU with a frame pointer; the list's default allocator tag lives in the high byte of the dead `owner` slot
// (`lea eax,[ebp+0Bh]`) and the resize fill value in `owner`'s slot; the POD vectors are STLport vectors built through the
// alloc-proxy third member (`_M_end_of_storage(a, 0)`) -- a plain three-zero initialiser list schedules the map's
// `lea ecx` one store early.
// Naming (NF160, 2026-11-20, q6213): the local CRetailHandleVector stand-in replaced by the shared STLport 4.6 std::vector<CEnginePrimitiveHandle>
//   (stlport/fable_containers.h, FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL; the landed 0x0064E1E4 / 0x0064ED06 recipe), so its reserve call binds
//   0x00443F90 as ?resize@?$vector@VCEnginePrimitiveHandle@@V?$allocator@VCEnginePrimitiveHandle@@@std@@@std@@QAEXI@Z (STLport one-argument
//   resize; the self row and 29 landed callers bind it).
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#include "stlport/fable_containers.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "engine/CEnginePrimitiveHandle.h"
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE_QUICKACCESSBUTTONS
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE_CONTEXTSENSITIVEBUTTONS
#pragma optimize("y", off)
#pragma optimize("s", on)

class CThing;
namespace NUISystem { class CComponent; }
class CPlayerManager;
class CTCBase { public:
    virtual ~CTCBase();                                           // slot 0 (0x00686830)
protected:
    const CPlayerManager& PeekPlayerManager() const;   // 0x00686D50 (Ego_r.pdb CTCBase, protected)
};

namespace NInventory {
class CTCInventoryBase : public ::CTCBase {                                      // vtable 0x01253DBC, retail sizeof 0x130
public:
    CTCInventoryBase(CThing& owner);                          // 0x005BFF07
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
class CFadeInFadeOutBase { public: unsigned char _body[0x0c]; };
class CRightHandedSet { public: float m[6]; };

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
    void resize(unsigned long n, const T& x);                 // unused now (the four resized members are std::vector<long>)
    void reserve(unsigned long n);                            // 0x004433F0 (long) / 0x00594DF4 (fade times)
};
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#include "stlport/fable_containers.h"
struct CComponentByLongMap {                                  // map<long, NUISystem::CComponent*>
    void* _M_header; unsigned long _M_node_count; long _M_key_compare;
    CComponentByLongMap();                                    // 0x00593ED0
    ~CComponentByLongMap();                                   // 0x00593AC7
};

// NF94 (q11291): the +0xC object is CPlayer::PlayerInterface (engine/CPlayer.h); 0x00445950 / 0x00445960 are Ego_r.pdb + FableWin.pdb
//   ?GetNumberOfQuickAccessItemButtons@ / ?GetNumberOfContextSensitiveItemButtons@CGamePlayerInterface@@QBEJXZ (engine/CGamePlayerInterface.h
//   QUICKACCESSBUTTONS / overlay CONTEXTSENSITIVEBUTTONS); the row-local CGameProfile / CPlayer models are replaced by the shared headers.
#include "engine/CGamePlayerInterface.h"
#include "engine/CPlayer.h"
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"

class CTCInventory : public NInventory::CTCInventoryBase, public NHeroInformationScreens::CBase {   // vtables 0x01250DE4 / 0x01250DD0
public:
    long                     ThingToAssign;                   // +0x134
    long                     ThingSlotToSwap;                 // +0x138
    NUISystem::CComponent*   PComponentSlotToSwap;            // +0x13c
    long                     ScreenToShortcut;                // +0x140
    CEditBoxList             EditBoxes;                       // +0x144
    std::vector<long>         QuickAccessItems;               // +0x148 (Ego_r.pdb vector<long>)
    CPODVector<long>         ContextSensitiveItems;           // +0x154
    CPODVector<CFadeInFadeOutBase> ContextSensitiveFadeTimes; // +0x160
    std::vector<CEnginePrimitiveHandle>      InventoryMainPrimitiveHandles;   // +0x16c
    std::vector<CEnginePrimitiveHandle>      QuickAccessPrimitiveHandles;     // +0x178
    std::vector<CEnginePrimitiveHandle>      QuickAccessUnusableIconPrimitiveHandles;   // +0x184
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
    std::vector<long>         NumberOfCSOpinionExpressions;   // +0x1d8 (Ego_r.pdb vector<long>)
    std::vector<long>         LastCSOpinionExpression;        // +0x1e4 (Ego_r.pdb vector<long>)
    std::vector<long>         LastButOneCSOpinionExpression;  // +0x1f0 (Ego_r.pdb vector<long>)
    CComponentByLongMap      ItemsMenuEntries;                // +0x1fc
    CComponentByLongMap      AssignedIcons;                   // +0x208
    NUISystem::CComponent*   PMouse_Pointer;                  // +0x214
    NUISystem::CComponent*   PTop_Level_List;                 // +0x218
    long                     CurrentOpenMenu;                 // +0x21c
    float                    AccumFadeTime;                   // +0x220
    CTCInventory(CThing& owner);
    virtual ~CTCInventory();
};

CTCInventory::CTCInventory(CThing& owner)
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
    unsigned long quickAccessSlots = PeekPlayerManager().GetMainPlayer().PlayerInterface->GetNumberOfQuickAccessItemButtons();
    QuickAccessItems.resize(quickAccessSlots, 0);
    QuickAccessUnusableIconPrimitiveHandles.resize(quickAccessSlots);
    QuickAccessPrimitiveHandles.resize(quickAccessSlots * 2);
    unsigned long contextSensitiveSlots = PeekPlayerManager().GetMainPlayer().PlayerInterface->GetNumberOfContextSensitiveItemButtons();
    ContextSensitiveItems.reserve(contextSensitiveSlots);
    ContextSensitiveFadeTimes.reserve(contextSensitiveSlots);
    NumberOfCSOpinionExpressions.resize(6, 0);
    LastCSOpinionExpression.resize(6, 0);
    LastButOneCSOpinionExpression.resize(6, 0);
    PMouse_Pointer = 0;
    PTop_Level_List = 0;
    CurrentOpenMenu = 0;
}