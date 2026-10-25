// CRegistryLayer_00c383d0::Add(CRegisteredItem* item, bool flagA, bool flagB) -- forwards the item to the parent layer's Add (the 0x00C2C7B0 layer on +4), records it in this layer's own list at +0x40 and returns it
// Leaf: ?Add@CRegistryLayer_00c383d0@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C383D0, 43 bytes (+5 int3 padding), __fastcall `ret 0xc`.
// Library: libpk audio (no PDB/RTTI); descriptive names. One level above the landed 0x00C2C7B0
//   (CRegistryLayer_00c2c7b0::Add, same shape with its list at +0x38), which in turn calls the root 0x00C14A80.
//   Callers 0x00C44B02, 0x00C61E3C, 0x00C6226E, 0x00C62668. The manifest's "CTCInventoryClothing_AddClothing" is
//   propagated.
// Callees: the parent's Add(item, bool, bool) @ 0x00C2C7B0 on +4, the per-layer list add @ 0x00C2C670 on +0x40.
struct CRegisteredItem;

struct CItemList_00c383d0 {
    void Add(CRegisteredItem* item);                             // 0x00C2C670
};

struct CRegistryParent_00c383d0 {
    CRegisteredItem* Add(CRegisteredItem* item, bool flagA, bool flagB);   // 0x00C2C7B0
};

struct CRegistryLayer_00c383d0 {
    void* __vftable;                                             // +0
    CRegistryParent_00c383d0* m_pParent;                         // +4
    unsigned char _pad_0x08[0x38];
    CItemList_00c383d0 m_items;                                  // +0x40
    CRegisteredItem* Add(CRegisteredItem* item, bool flagA, bool flagB);
};

CRegisteredItem* CRegistryLayer_00c383d0::Add(CRegisteredItem* item, bool flagA, bool flagB)
{
    m_pParent->Add(item, flagA, flagB);
    m_items.Add(item);
    return item;
}