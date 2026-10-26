// CTCInventory::~CTCInventory() -- drop the cached defs and close the PC inventory screen, then destroy the members in reverse order and both bases
// Leaf: ??1CTCInventory
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00589C55, length 219 bytes. Called only by the deleting destructor 0x00594CD2
// (slot 0 of CTCInventory's vtable 0x01250DE4); the catalog label "CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip"
// is propagated. Size TU (`pop ecx` after free), as the landed CTCInventoryClothing destructor 0x005B7049.
// Callees: the cached-def release @ 0x005D9320 (label "ClearCachedDefs"; called with `this` in ecx), ClosePCInventory(bool)
//          @ 0x00587847, the map<long, CComponent*> destructor @ 0x00593AC7 (x2), the vector<CEnginePrimitiveHandle>
//          destructor @ 0x00440C40 (x3), the list<CEditBox*> destructor @ 0x005931F5 (labels "CActiveFile::OnReadFinished",
//          byte twins), free @ 0x00BFEA14 (x6), NInventory::CTCInventoryBase::~CTCInventoryBase @ 0x005C00A0 (tail jump).
// Layout: Ego_r.pdb CTCInventory at retail offsets (base 0x130 bytes, NHeroInformationScreens::CBase vptr +0x130,
// 12-byte vectors/maps, one-word list): ThingToAssign +0x134 .. ScreenToShortcut +0x140, EditBoxes +0x144,
// QuickAccessItems +0x148, ContextSensitiveItems +0x154, ContextSensitiveFadeTimes +0x160, the three primitive-handle
// vectors +0x16c/+0x178/+0x184, the RH sets and flash state +0x190..+0x1d4, the three opinion vectors +0x1d8/+0x1e4/+0x1f0,
// ItemsMenuEntries +0x1fc, AssignedIcons +0x208, the menu pointers +0x214.. .
// Shape: both vtables are stored first; the POD vectors free inline (`__forceinline`), the others are out-of-line
// destructors; the inline empty CBase destructor re-stores 0x0123A3E0 before the tail jump into the primary base.
extern "C" void __cdecl free(void* block);                    // 0x00BFEA14
namespace NUISystem { class CComponent; }

template<class T> struct CPODVector {              // STLport vector of a trivially destructible element (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    __forceinline ~CPODVector()
    {
        if (_M_start)
            free(_M_start);
    }
};

namespace NInventory {
class CTCInventoryBase {                                      // vtable 0x01253DBC, retail sizeof 0x130
public:
    virtual ~CTCInventoryBase();                              // 0x005C00A0
    unsigned char _body[0x12C];
};
}
namespace NHeroInformationScreens {
class CBase {                                                 // vtable 0x0123A3E0
public:
    virtual ~CBase() {}
};
}

class CFadeInFadeOutBase { public: unsigned char _body[0x0c]; };
class CRightHandedSet { public: float m[6]; };

struct CEditBoxList {                                         // list<NUISystem::CEditBox*> (one word)
    void* _M_node;
    ~CEditBoxList();                                          // 0x005931F5
};
struct CRetailHandleVector {                                  // vector<CEnginePrimitiveHandle>
    void* m_pFirst;
    void* m_pLast;
    void* m_pEnd;
    ~CRetailHandleVector();                                   // 0x00440C40
};
struct CComponentByLongMap {                                  // map<long, NUISystem::CComponent*>
    void* _M_header; unsigned long _M_node_count; long _M_key_compare;
    ~CComponentByLongMap();                                   // 0x00593AC7
};

class CTCInventory : public NInventory::CTCInventoryBase, public NHeroInformationScreens::CBase {   // vtables 0x01250DE4 / 0x01250DD0
public:
    long                     ThingToAssign;                   // +0x134
    long                     ThingSlotToSwap;                 // +0x138
    NUISystem::CComponent*   PComponentSlotToSwap;            // +0x13c
    long                     ScreenToShortcut;                // +0x140
    CEditBoxList             EditBoxes;                       // +0x144
    CPODVector<long>         QuickAccessItems;                // +0x148
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
    CPODVector<long>         NumberOfCSOpinionExpressions;    // +0x1d8
    CPODVector<long>         LastCSOpinionExpression;         // +0x1e4
    CPODVector<long>         LastButOneCSOpinionExpression;   // +0x1f0
    CComponentByLongMap      ItemsMenuEntries;                // +0x1fc
    CComponentByLongMap      AssignedIcons;                   // +0x208
    NUISystem::CComponent*   PMouse_Pointer;                  // +0x214
    NUISystem::CComponent*   PTop_Level_List;                 // +0x218
    long                     CurrentOpenMenu;                 // +0x21c
    float                    AccumFadeTime;                   // +0x220
    void ClearCachedDefs_005d9320();                          // 0x005D9320
    void ClosePCInventory(bool immediate);                    // 0x00587847
    virtual ~CTCInventory();
};

CTCInventory::~CTCInventory()
{
    ClearCachedDefs_005d9320();
    ClosePCInventory(false);
}