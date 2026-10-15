// CTCInventoryBase::GetSelectedCategoryThing() -- returns the thing associated with the currently selected category
// Leaf: GetSelectedCategoryThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0061A040, length 24 bytes; size TU (sibling `jmp` for the last call).
// No direct caller or vtable reference in the image (xref.py finds none): an unreferenced out-of-line copy. The catalog label
// "GetItemDescription" is propagated; the body is GetCategoryWithIndex(GetSelectedCategoryIndex())->GetAssociatedThing().
// Callees: CTCInventoryBase::GetSelectedCategoryIndex @ 0x005BC4B2 (`mov eax,[ecx+70h] ; ret`, out of line),
// CTCInventoryBase::GetCategoryWithIndex(long) @ 0x005BCE3C (`first + index*2Ch`, out of line),
// CCategory::GetAssociatedThing @ 0x005BD1AE (a `jmp CBaseIntelligentPointer::GetPItem` on PAssociatedThing at +0).
#pragma optimize("s", on)
struct CThing;
struct CCategory_Retail {
    unsigned char _body[0x2C];
    CThing* GetAssociatedThing();                                // 0x005BD1AE (jmp CBaseIntelligentPointer::GetPItem)
};
struct CTCInventoryBase_Retail {
    long              GetSelectedCategoryIndex();                // 0x005BC4B2 (returns SelectedCategoryIndex, +0x70)
    CCategory_Retail* GetCategoryWithIndex(long index);          // 0x005BCE3C
    CThing*           GetSelectedCategoryThing();
};
CThing* CTCInventoryBase_Retail::GetSelectedCategoryThing()
{
    return GetCategoryWithIndex(GetSelectedCategoryIndex())->GetAssociatedThing();
}