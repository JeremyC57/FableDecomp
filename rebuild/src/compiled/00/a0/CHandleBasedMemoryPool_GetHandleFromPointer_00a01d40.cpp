// CHandleBasedMemoryPool::GetHandleFromPointer(void*) -- returns the handle stored in the block header before a pointer
// Retail: Fable.exe 0x00A01D40, 10 bytes: `mov eax,[esp+4] ; mov eax,[eax-1Ch] ; ret 4` (this unused).
// Identity: the catalog label CWorld::DrawGetEnvironment is wrong (that PDB function is a const virtual with no
// argument). The only caller, ~CContainmentVolume 0x00A752F0, does `MemoryPool->FreeResource(MemoryPool->X(planes))`
// with MemoryPool the CMovableResourceMemoryPool* at +0x0C (Ego_r.pdb layout): X is Ego_r.pdb
// ?GetHandleFromPointer@CHandleBasedMemoryPool@@QAEJPAX@Z. The 0x20-byte CHeader (Resource +0, Handle +4, ...) sits
// directly before the data, so the handle is at pointer - 0x1C.
// Leaf: ?GetHandleFromPointer@CHandleBasedMemoryPool@@QAEJPAX@Z
#define CHandleBasedMemoryPool DonorCHandleBasedMemoryPool
#include "engine/CHandleBasedMemoryPool.h"
#undef CHandleBasedMemoryPool

class CHandleBasedMemoryPool : public DonorCHandleBasedMemoryPool {
public:
    class CHeader {
    public:
        void*         Resource;          // +0x00
        long          Handle;            // +0x04
        long          Size;              // +0x08
        short         Padding;           // +0x0C
        unsigned char Status;            // +0x0E
        unsigned char Locked;            // +0x0F
        CHeader**     RefAvailable;      // +0x10
        CHeader*      NextAvailable;     // +0x14
        CHeader*      PrevContiguous;    // +0x18
        CHeader*      NextContiguous;    // +0x1C
    };
    long GetHandleFromPointer(void* p);
};

long CHandleBasedMemoryPool::GetHandleFromPointer(void* p)
{
    return (static_cast<CHeader*>(p) - 1)->Handle;
}