// CHandleBasedMemoryPool::ClearFreeLists() -- empty all 256 size-bucket free lists
// Leaf: ?ClearFreeLists@CHandleBasedMemoryPool@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C28200, 17 bytes (`ret`). The libpk build of the engine's
//   CHandleBasedMemoryPool::ClearFreeLists (Ego_r.pdb name set: `?ClearFreeLists@CHandleBasedMemoryPool@@AAEXXZ`);
//   the manifest's "$E1" (a dynamic-initializer name) is propagated.
// Layout: this library's pool keeps its free lists at +0x3C (InsertFreeArea 0x00C27EF0 indexes `[ecx+edx*4+3Ch]`),
//   one word above engine/CHandleBasedMemoryPool.h (FreeLists +0x38; the libpk pool has an embedded default failure
//   handler at +0x34 and the current handler pointer at +0x38, see AllocateHandle 0x00C22100), so the pool is modelled
//   locally.
// Shape: an intrinsic memset of the 0x400-byte array (`mov ecx,100h ; xor eax,eax ; rep stosd`, this moved to edx).
extern "C" void* __cdecl memset(void* dest, int value, unsigned int count);
#pragma intrinsic(memset)

struct CHeader;

class CHandleBasedMemoryPool { public:                 // this library's pool variant (see Layout)
    void*         __vftable;                    // +0x00
    void*         Buffer;                       // +0x04
    void*         UnalignedBuffer;              // +0x08
    long          PoolSize;                     // +0x0c
    long          AllocatedSize;                // +0x10
    long          NumAllocations;               // +0x14
    long          LockCount;                    // +0x18
    long          LargestFreeArea;              // +0x1c
    unsigned char _pad20[0x34 - 0x20];
    void*         m_defaultHandler;             // +0x34 embedded default handler (one vptr)
    void*         FailureHandler;               // +0x38
    CHeader*      FreeLists[256];               // +0x3c
    void ClearFreeLists();
};

void CHandleBasedMemoryPool::ClearFreeLists()
{
    memset(FreeLists, 0, sizeof(FreeLists));
}