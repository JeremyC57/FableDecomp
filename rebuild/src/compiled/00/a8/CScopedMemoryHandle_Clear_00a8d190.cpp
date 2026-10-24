// CScopedMemoryHandle::Clear() -- give the handle back to its handle-based memory pool (when one is attached), detach the pool and mark the handle invalid (-1)
// Leaf: ?Clear@CScopedMemoryHandle@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A8D190, 33 bytes (+ int3 padding), thiscall `ret`.
// Identity: Ego_r.pdb names `?Clear@CScopedMemoryHandle@@QAEXXZ`; CScopedMemoryHandle (8 bytes: Pool +0, Handle +4) is the
//   object behind CCountedPointer<CScopedMemoryHandle> (its DeleteData 0x00A8D7C0 inlines this body as the destructor).
//   The catalog/BSim label "CBankFileAsyncData::DiscardAsyncData" is propagated from a byte-identical body.
// Callee: CHandleBasedMemoryPool::Free(long handle) @ 0x00A02B30 (thiscall, the pool loaded straight into ecx).
// Shape: `if (Pool) { Pool->Free(Handle); Pool = 0; Handle = -1; }` -- this in esi, two immediate stores after the call.
class CHandleBasedMemoryPool {
public:
    void Free(long handle);                        // 0x00A02B30
};

class CScopedMemoryHandle {
public:
    CHandleBasedMemoryPool* Pool;                  // +0x00
    long                    Handle;                // +0x04
    void Clear();
};

void CScopedMemoryHandle::Clear()
{
    if (Pool) {
        Pool->Free(Handle);
        Pool = 0;
        Handle = -1;
    }
}