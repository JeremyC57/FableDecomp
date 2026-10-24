// CScopedMemoryHandle::Initialise(CHandleBasedMemoryPool*, long) -- release any handle already held (Clear(): Free @ 0x00A02B30, then no pool / handle -1) and take the new pool and handle
// Leaf: ?Initialise@CScopedMemoryHandle@@QAEXPAVCHandleBasedMemoryPool@@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A039F0, 80 bytes, thiscall, `ret 8`.
// Name/layout: Ego_d.pdb ?Initialise@CScopedMemoryHandle@@QAEXPAVCHandleBasedMemoryPool@@J@Z; Ego_r.pdb CScopedMemoryHandle
//   (8 bytes): Pool +0, Handle +4. Caller 0x00A91BA5 (followed by CHandleBasedMemoryPool::GetPointerFromHandle on the same
//   pair). The chunk label "CHandleBuffer_Reset" is descriptive.
// Shape: the inlined Clear() keeps its dead `Pool = 0 ; Handle = -1` stores before the assignment; the two assignment tails
//   are duplicated per arm.
class CHandleBasedMemoryPool {
public:
    void Free(long handle);                         // 0x00A02B30
};

class CScopedMemoryHandle {
public:
    CHandleBasedMemoryPool* Pool;                   // +0x00
    long Handle;                                    // +0x04
    void Clear()
    {
        if (Pool) {
            Pool->Free(Handle);
            Pool = 0;
            Handle = -1;
        }
    }
    void Initialise(CHandleBasedMemoryPool* pool, long handle);
};

void CScopedMemoryHandle::Initialise(CHandleBasedMemoryPool* pool, long handle)
{
    Clear();
    Pool = pool;
    Handle = handle;
}