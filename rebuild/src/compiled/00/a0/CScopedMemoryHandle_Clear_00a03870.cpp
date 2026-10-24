// CScopedMemoryHandle::Clear() -- free the handle back to its pool (CHandleBasedMemoryPool::Free @ 0x00A02B30) and reset to the empty state (no pool, handle -1)
// Leaf: ?Clear@CScopedMemoryHandle@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A03870, 33 bytes, thiscall, plain `ret`. No direct callers (inlined at its
//   uses, e.g. CScopedMemoryHandle::Initialise @ 0x00A039F0).
// Name/layout: Ego_d.pdb ?Clear@CScopedMemoryHandle@@QAEXXZ; Ego_r.pdb CScopedMemoryHandle (8 bytes): Pool +0, Handle +4.
//   The chunk label "DiscardAsyncData" is descriptive.
class CHandleBasedMemoryPool {
public:
    void Free(long handle);                         // 0x00A02B30
};

class CScopedMemoryHandle {
public:
    CHandleBasedMemoryPool* Pool;                   // +0x00
    long Handle;                                    // +0x04
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