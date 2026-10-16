// CTCInventoryClothing::CTCInventoryClothing(CThing* thing) -- construct the clothing inventory: the inventory base, the listener base at +0x130, the menu-entry vector, the three maps, and the mannequin / list members
// Leaf: ??0CTCInventoryClothing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E7CC3, length 120 bytes, `ret 4`.
// Label correction: the catalog row says "CTCHero::CTCHero", but the vtable stores are CTCInventoryClothing's (primary
// 0x0124356C, the secondary listener side 0x01243558 at +0x130; rtti-vtables.tsv).
// Callees: CTCInventoryBase::CTCInventoryBase(CThing*) @ 0x005BFF07, the secondary base ctor @ 0x0066E734 (manifest "CBase::CBase"),
// the out-of-line STLport map ctors @ 0x004DF7FF (CurrentClothingSelected), 0x004E76DF (MorphPrimitiveHandles) and 0x004DF81D
// (ClothingItemMap) (all three labelled "CopyBackBufferToTexture", propagated).
// Layout: PDB CTCInventoryClothing (0x198) with retail 12-byte STLport containers (0x170): listener base +0x130,
// ClothingMenuEntries +0x134, CurrentClothingSelected +0x140, MorphPrimitiveHandles +0x14C, PMannequin +0x158, UseMannequin
// +0x15C, LoadedFromSaveGame +0x15D, ClothingItemMap +0x160, ClothingList +0x16C.
// Size-optimised TU (`#pragma optimize("s", on)`: `push dword ptr [esp+10h]`, unaligned address).
// Shape: initialiser list in declaration order; the vector is the STLport _Vector_base with an _STLP_alloc_proxy
// end-of-storage member (three plain pointers schedule the last zero store before the map's `lea`); the vector zero words share ebx with the later zero members.
#pragma optimize("s", on)
struct CThing;
struct CComponent;

class CTCInventoryBase {                        // 0x130 bytes in retail
public:
    CTCInventoryBase(CThing* thing);            // 0x005BFF07
    virtual ~CTCInventoryBase();
    unsigned char _pad04[0x130 - 0x04];
};
class CInventoryListenerBase {                  // secondary base, vtable stored by its out-of-line ctor
public:
    CInventoryListenerBase();                   // 0x0066E734
    virtual void OnNotify();
};
struct CMenuEntry { long Part; CComponent* Component; };
template <class T> struct CStorageProxy {      // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* _M_data;
    CStorageProxy(T* p) : _M_data(p) {}
};
struct CMenuEntryVector {                       // STLport vector<pair<EClothingSuitPart, NUISystem::CComponent*>> (_Vector_base)
    CMenuEntry*               _M_start;
    CMenuEntry*               _M_finish;
    CStorageProxy<CMenuEntry> _M_end_of_storage;
    CMenuEntryVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
struct CSuitPartMap {                           // map<EClothingSuitPart, long>
    void* _M_header; long _M_node_count; long _M_compare;
    CSuitPartMap();                             // 0x004DF7FF
};
struct CMorphHandleMap {                        // map<long, CEnginePrimitiveHandle>
    void* _M_header; long _M_node_count; long _M_compare;
    CMorphHandleMap();                          // 0x004E76DF
};
struct CComponentItemMap {                      // map<NUISystem::CComponent*, long>
    void* _M_header; long _M_node_count; long _M_compare;
    CComponentItemMap();                        // 0x004DF81D
};

class CTCInventoryClothing : public CTCInventoryBase, public CInventoryListenerBase {   // vtables 0x0124356C / 0x01243558
public:
    CMenuEntryVector  ClothingMenuEntries;      // +0x134
    CSuitPartMap      CurrentClothingSelected;  // +0x140
    CMorphHandleMap   MorphPrimitiveHandles;    // +0x14c
    CThing*           PMannequin;               // +0x158
    bool              UseMannequin;             // +0x15c
    bool              LoadedFromSaveGame;       // +0x15d
    CComponentItemMap ClothingItemMap;          // +0x160
    CComponent*       ClothingList;             // +0x16c
    CTCInventoryClothing(CThing* thing);
    virtual ~CTCInventoryClothing();
    virtual void OnNotify();
};

CTCInventoryClothing::CTCInventoryClothing(CThing* thing)
    : CTCInventoryBase(thing), PMannequin(0), UseMannequin(false), LoadedFromSaveGame(false), ClothingList(0)
{
}
