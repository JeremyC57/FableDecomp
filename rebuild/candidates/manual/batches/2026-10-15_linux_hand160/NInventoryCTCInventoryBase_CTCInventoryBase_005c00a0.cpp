// NInventory::CTCInventoryBase::~CTCInventoryBase() -- unregister from the global TC list, then the members in reverse (confiscated items, the two display-list vectors, the two serialised counted pointers, the def pointer, the categories, the components vector), then the CTCBase base
// Leaf: ??1CTCInventoryBase@NInventory
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C00A0, length 129 bytes (size-optimised TU: `#pragma optimize("s", on)`).
// Label correction: catalogued as CTCActionUseBuyOrSteal::~CTCActionUseBuyOrSteal (propagated); the vtable stored here,
// 0x01253DBC, is NInventory::CTCInventoryBase (rtti-vtables.tsv), and the member layout is the PDB CTCInventoryBase.
// Callees: CTCBase::RemoveFromGlobalTCs @ 0x00686DF0 (dtor body), vector<NInventory::CItem>::~vector @ 0x005C1E30,
// CCountedPointer<vector<CCharString>>::~CCountedPointer @ 0x005C0563, CCountedPointer<vector<CItem>>::~CCountedPointer
// @ 0x005C0511 (catalog: "_Dest_val<...COptimisedPrimitive>" byte-twins), vector<NInventory::CCategory>::~vector @ 0x005C2C1F
// (catalog "CActiveFile::OnReadFinished" twin), free @ 0x00BFEA14, CTCBase::~CTCBase @ 0x00686830 (tail jump).
// Shape: size TU markers (`and dword ptr [m],0`, `pop ecx` after free); the POD vectors' dtors (just `free(start)`) are inline
// only as `__forceinline`; the CDefPointer dtor calls the def's inline Release() inside its null test.
// Layout: PDB CTCInventoryBase with 12-byte retail vectors: Components +0xC, Categories +0x20, PDef +0x68, SerialisedItems
// +0x78, SerialisedSelectedItems +0x80, MenuDisplayListItem +0x94, MenuDisplayListCategory +0xA0, ConfiscatedItems +0xAC.
#pragma optimize("s", on)
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

class CThing;
template <class T> struct CPODVector {             // STLport vector of a trivial type (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    __forceinline ~CPODVector()
    {
        if (_M_start)
            free(_M_start);
    }
};
namespace NInventory { struct CItem; struct CCategory; }
namespace NUISystem { struct CComponent; }
struct CItemVector {                               // vector<NInventory::CItem>
    void* _M_start; void* _M_finish; void* _M_end_of_storage;
    ~CItemVector();                                // 0x005C1E30
};
struct CCategoryVector {                           // vector<NInventory::CCategory>
    void* _M_start; void* _M_finish; void* _M_end_of_storage;
    ~CCategoryVector();                            // 0x005C2C1F
};
struct CSerialisedItemsPointer {                   // CCountedPointer<vector<NInventory::CItem>>
    void* Data; void* Info;
    ~CSerialisedItemsPointer();                    // 0x005C0511
};
struct CSerialisedSelectedItemsPointer {           // CCountedPointer<vector<CCharString>>
    void* Data; void* Info;
    ~CSerialisedSelectedItemsPointer();            // 0x005C0563
};
struct CInventoryDef {
    virtual void v00();
    virtual void Destroy();                        // slot 1 (+0x04)
    long RefCount;                                 // +0x04
    void Release()
    {
        if (--RefCount == 0)
            Destroy();
    }
};
struct CDefPointer {                               // CDefPointer<CInventoryDef const>
    CInventoryDef* Object;
    __forceinline ~CDefPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
};
struct C2DBoxF { float X0, Y0, X1, Y1; };

class CTCBase {                                    // engine/CTCBase.h offsets
public:
    virtual ~CTCBase();                            // 0x00686830
    CThing* ParentThing;                           // +0x04
    bool    ToBeRemoved;                           // +0x08
    bool    IsInGlobalUpdateTCs;                   // +0x09
    void RemoveFromGlobalTCs();                    // 0x00686DF0
};

namespace NInventory {
class CTCInventoryBase : public CTCBase {          // vtable 0x01253DBC
public:
    CPODVector<NUISystem::CComponent*> Components; // +0x0c
    bool FirstRenderFrame;                         // +0x18
    long InventorySelectionState;                  // +0x1c
    CCategoryVector Categories;                    // +0x20
    long HighlightedItemNumOwned;                  // +0x2c
    long HighlightedItemMaxOwned;                  // +0x30
    bool CanItemBeQuickAssigned;                   // +0x34
    bool CanItemBeUsed;                            // +0x35
    long MenuArrowHighlights[4];                   // +0x38
    long TriggerStateLeft;                         // +0x48
    long TriggerStateRight;                        // +0x4c
    double LastRenderTime;                         // +0x50
    bool InventoryOpen;                            // +0x58
    long OldIntervalDesc[2];                       // +0x5c CPresentationIntervalDesc
    bool OldIntervalDescInitialised;               // +0x64
    CDefPointer PDef;                              // +0x68
    long GlobalDefIndex;                           // +0x6c
    long SelectedCategoryIndex;                    // +0x70
    void* GraphicManager;                          // +0x74
    CSerialisedItemsPointer SerialisedItems;       // +0x78
    CSerialisedSelectedItemsPointer SerialisedSelectedItems;   // +0x80
    bool Dying;                                    // +0x88
    bool CategoryMoving;                           // +0x89
    float CategoryMovement;                        // +0x8c
    float CategoryMovementDirection;               // +0x90
    CPODVector<long> MenuDisplayListItem;          // +0x94
    CPODVector<long> MenuDisplayListCategory;      // +0xa0
    CItemVector ConfiscatedItems;                  // +0xac
    virtual ~CTCInventoryBase();
};

CTCInventoryBase::~CTCInventoryBase()
{
    RemoveFromGlobalTCs();
}
}
