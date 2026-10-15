// CRegistryLayer_00c2c7b0::Add(CRegisteredItem* item, bool flagA, bool flagB) -- forwards the item to the parent layer's Add, records it in this layer's own list and returns it
// Leaf: Add@CRegistryLayer_00c2c7b0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2C7B0, length 43 bytes (+5 int3 padding); speed-optimised TU.
// The catalog label "CTCInventoryClothing::AddClothing" is propagated and wrong: this is one link of a layered registration
// chain in the 0x00C1..0x00C3 module -- its caller 0x00C383D0 has the identical shape one level up (calls this on its +4
// parent, then its own list at +0x40), and the parent 0x00C14A80 files the item into its lists at +0x78 / +0x84 / +0x90 depending
// on the two flags. Class and method names are descriptive (no RTTI for the owner).
// Callees: the parent's Add(item, bool, bool) @ 0x00C14A80 on the pointer at +4, the per-layer list add @ 0x00C2C670 on +0x38
// (manifest "CClothingItem_ValidateGeometry" is propagated).
// Shape: both flags are re-pushed as whole dwords (`mov ecx,[esp+10h] ; push ecx`), the item stays in edi for the second call
// and the return value.
struct CRegisteredItem;
struct CItemList_00c2c7b0 {
    void Add(CRegisteredItem* item);                             // 0x00C2C670
};
struct CRegistryParent_00c2c7b0 {
    CRegisteredItem* Add(CRegisteredItem* item, bool flagA, bool flagB);   // 0x00C14A80
};
struct CRegistryLayer_00c2c7b0 {
    void*            __vftable;                                  // +0x00
    CRegistryParent_00c2c7b0* m_pParent;                                  // +0x04
    unsigned char    _pad_0x08[0x30];
    CItemList_00c2c7b0        m_items;                                    // +0x38
    CRegisteredItem* Add(CRegisteredItem* item, bool flagA, bool flagB);
};
CRegisteredItem* CRegistryLayer_00c2c7b0::Add(CRegisteredItem* item, bool flagA, bool flagB)
{
    m_pParent->Add(item, flagA, flagB);
    m_items.Add(item);
    return item;
}
