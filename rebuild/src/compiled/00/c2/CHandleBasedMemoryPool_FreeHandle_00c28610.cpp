// CHandleBasedMemoryPool_012b4098::FreeHandle(long handle) -- clears a released handle's slot and pushes the handle onto the free-handle chain
// Leaf: FreeHandle
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C28610, length 38 bytes (+10 int3 padding). The catalog label
// "CTCInventoryBase::GetCategoryWithIndex" is propagated and wrong: the only caller is CHandleBasedMemoryPool_012b4098::FreeAllocation
// (0x00C28690, which reads the header pointer from the same table at +0x440 before calling this), so the body is the pool's
// handle release; the leaf name is descriptive.
// Model: engine/CHandleBasedMemoryPool.h (PDB) puts the Handles CArray at +0x438 (16 bytes) and FirstUnusedHandle at +0x448;
// the retail body reads the handle table pointer at +0x440 and the free-chain head at +0x44C, so a local retail view is used.
// Shape: the slot is cleared and then overwritten with the old chain head: both stores survive because the slot is a union of
// the header pointer and the free link written through one reference (`lea eax,[eax+edx*4] ; mov [eax],0 ; mov [eax],esi`).
struct CHeader;
union CHandleSlot {
    CHeader* Header;                                             // a live handle's block header
    long     NextFreeHandle;                                     // a free handle's link in the free-handle chain
};
struct CHandleBasedMemoryPool_Retail {
    unsigned char _pad_0x000[0x440];
    CHandleSlot*  Handles;                                       // +0x440 (CArray<CHandleInfo> data pointer)
    unsigned char _pad_0x444[0x8];
    long          FirstUnusedHandle;                             // +0x44C
    void FreeHandle(long handle);
};
void CHandleBasedMemoryPool_Retail::FreeHandle(long handle)
{
    CHandleSlot& slot = Handles[handle];
    slot.Header = 0;
    slot.NextFreeHandle = FirstUnusedHandle;
    FirstUnusedHandle = handle;
}