// CTCInventoryClothing::~CTCInventoryClothing() -- kills the mannequin thing, clears the clothing menu entries, then destroys the members and both bases
// Leaf: ??1CTCInventoryClothing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005B7049, length 111 bytes. Size TU: `#pragma optimize("s", on)` (`pop ecx` after free).
// Callees: CThing::Kill(bool) @ 0x004C9B80, ClearClothingMenuEntries() @ 0x005B432C, the three STLport map destructors
// @ 0x004DD1C4 (ClothingItemMap), 0x004E6DF5 (MorphPrimitiveHandles), 0x004DD159 (CurrentClothingSelected) (manifest
// "CActiveFile::OnReadFinished", byte twins), free @ 0x00BFEA14, NInventory::CTCInventoryBase::~CTCInventoryBase @ 0x005C00A0 (tail jump).
// Shape: two vtable stores (0x0124356C primary, 0x01243558 for the NHeroInformationScreens::CBase subobject at +0x130),
// the members in reverse order, the POD vector's `if (start) free(start)` inline (`__forceinline`), then the inline empty
// CBase destructor re-stores its own vtable (0x0123A3E0) before the tail jump into the primary base.
// Layout: engine/CTCInventoryClothing.h is the PDB layout (16-byte containers, ClothingMenuEntries at +0x14C); the retail
// base CTCInventoryBase is 0x130 bytes and the containers are 12 bytes, so a local view carries the PDB names at:
// CBase vptr +0x130, ClothingMenuEntries +0x134, CurrentClothingSelected +0x140, MorphPrimitiveHandles +0x14C,
// PMannequin +0x158, UseMannequin +0x15C, LoadedFromSaveGame +0x15D, ClothingItemMap +0x160, ClothingList +0x16C.
#pragma optimize("s", on)
extern "C" void __cdecl free(void* block);                    // 0x00BFEA14
class CThing { public:
    void Kill(bool silent);                                   // 0x004C9B80
};
namespace NUISystem { class CComponent; }
struct SClothingMenuEntry { long Part; NUISystem::CComponent* Component; };
struct CClothingMenuEntryVector {                             // vector<pair<EClothingSuitPart, CComponent*>>
    SClothingMenuEntry* _M_start;
    SClothingMenuEntry* _M_finish;
    SClothingMenuEntry* _M_end_of_storage;
    __forceinline ~CClothingMenuEntryVector()
    {
        if (_M_start)
            free(_M_start);
    }
};
struct CSuitPartMap {                                         // map<EClothingSuitPart, long>
    void* _M_header; unsigned long _M_node_count; long _M_key_compare;
    ~CSuitPartMap();                                          // 0x004DD159
};
struct CMorphPrimitiveHandleMap {                             // map<long, CEnginePrimitiveHandle>
    void* _M_header; unsigned long _M_node_count; long _M_key_compare;
    ~CMorphPrimitiveHandleMap();                              // 0x004E6DF5
};
struct CClothingItemMap {                                     // map<NUISystem::CComponent*, long>
    void* _M_header; unsigned long _M_node_count; long _M_key_compare;
    ~CClothingItemMap();                                      // 0x004DD1C4
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
class CTCInventoryClothing : public NInventory::CTCInventoryBase, public NHeroInformationScreens::CBase {
public:
    CClothingMenuEntryVector ClothingMenuEntries;            // +0x134
    CSuitPartMap             CurrentClothingSelected;        // +0x140
    CMorphPrimitiveHandleMap MorphPrimitiveHandles;          // +0x14C
    CThing*                  PMannequin;                     // +0x158
    bool                     UseMannequin;                   // +0x15C
    bool                     LoadedFromSaveGame;             // +0x15D
    CClothingItemMap         ClothingItemMap;                // +0x160
    NUISystem::CComponent*   ClothingList;                   // +0x16C
    void ClearClothingMenuEntries();                         // 0x005B432C
    virtual ~CTCInventoryClothing();
};
CTCInventoryClothing::~CTCInventoryClothing()
{
    if (PMannequin)
        PMannequin->Kill(false);
    ClearClothingMenuEntries();
}