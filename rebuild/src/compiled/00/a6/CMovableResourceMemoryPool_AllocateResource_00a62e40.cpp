// CMovableResourceMemoryPool::AllocateResource(long*, CMovableResource*, unsigned long, unsigned long) -- allocate a pool block under the pool lock and bind it to its movable owner
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A62E40, 98 bytes (`ret 10h`).
// Callees: CHandleBasedMemoryPool::BeginCritical(bool) @ 0x00A02140, Allocate(long*, ulong, ulong) @ 0x00A03180,
//   InternalSetResource(long, void*) @ 0x00A028B0, EndCritical() @ 0x00A02170.
// Shape: the lock flag is bit 0 of the flags argument handed to BeginCritical as a byte (`mov al,bl ; and eax,0FFFFFF01h`);
//   the whole flags word is then forwarded to Allocate.
struct CMovableResource {
    virtual void v00();
};

struct CHandleBasedMemoryPool {
    bool BeginCritical(bool wait);                                           // 0x00A02140
    void EndCritical();                                                          // 0x00A02170
    bool Allocate(long* handle, unsigned long size, unsigned long flags);       // 0x00A03180
    void InternalSetResource(long handle, void* resource);                      // 0x00A028B0
};

struct CMovableResourceMemoryPool : CHandleBasedMemoryPool {
    bool AllocateResource(long* handle, CMovableResource* owner, unsigned long size, unsigned long flags);
};

bool CMovableResourceMemoryPool::AllocateResource(long* handle, CMovableResource* owner, unsigned long size, unsigned long flags)
{
    if (!BeginCritical((flags & 1) != 0))
        return false;
    if (Allocate(handle, size, flags)) {
        InternalSetResource(*handle, owner);
        EndCritical();
        return true;
    }
    EndCritical();
    return false;
}