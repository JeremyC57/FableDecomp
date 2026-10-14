// C3DMesh2::UnlockPrimitive(long) -- unlocks the mesh, primitive and (when allocated) bone-name memory handles in the handle-based pool at +4; the argument is unused
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A892A0, length 54 bytes.
// Callees: CHandleBasedMemoryPool::Unlock(long) @ 0x00A02710 (three calls on the pool at +4).
// Retail C3DMesh2 sits 8 bytes below the Ego_r PDB layout from the handles on: MemoryHandle +0x7c
// (PDB +0x84), PrimMemoryHandle +0x80, BoneNameMemoryHandle +0x84 (-1 when the mesh has no bone
// names). Modelled locally for that reason.
struct CHandleBasedMemoryPool {
    void Unlock(long handle);              // 0x00A02710
};

struct C3DMesh2 {
    void* m_vptr;
    CHandleBasedMemoryPool* MemoryPool;    // +4
    unsigned char _pad_0x08[0x74];
    long MemoryHandle;                     // +0x7c
    long PrimMemoryHandle;                 // +0x80
    long BoneNameMemoryHandle;             // +0x84
    void UnlockPrimitive(long primitiveIndex);
};

void C3DMesh2::UnlockPrimitive(long primitiveIndex)
{
    MemoryPool->Unlock(MemoryHandle);
    MemoryPool->Unlock(PrimMemoryHandle);
    if (BoneNameMemoryHandle != -1)
        MemoryPool->Unlock(BoneNameMemoryHandle);
}
