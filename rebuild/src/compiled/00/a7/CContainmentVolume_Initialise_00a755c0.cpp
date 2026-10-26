// CContainmentVolume::Initialise(const CPlane* planes, unsigned long count, CMovableResourceMemoryPool* pool) -- size the plane array (movable pool block when a pool is given, else new CPlane[count]) and copy the planes in
// Leaf: Initialise
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A755C0, 174 bytes, `ret 0xc`.
// Callees: CMovableResourceMemoryPool::AllocateResource(long*, CMovableResource*, ulong, ulong) 0x00A62E40,
//   CHandleBasedMemoryPool::GetPointerFromHandle(long) 0x00A026B0, operator new[] 0x00BFEB22,
//   `vector constructor iterator' 0x004038C0 with the out-of-line empty CPlane::CPlane() 0x0063EC80,
//   the LTCG std::copy<const CPlane*, CPlane*> 0x00A759E0 (first in ecx, last in edx, dest on the stack, `ret 4`
//   = exactly __fastcall).
// Layout: engine/CContainmentVolume.h (ContainmentPlanes +4, Size +8, MemoryPool +0xc); CPlane = { C3DVector Normal; float Distance }.
// Shape: Size and MemoryPool are stored before their tests; the pool block size goes through the same
//   `__int64` 0..0x7fffffff guard as the stream reads; the handle lives in the dead `count` slot. The size guard and
//   the AllocateResource call sit in ONE __forceinline AllocateChecked wrapper tested by a single `if`: VC7.1 then lays
//   the shared `return false` block as the fall-through of the AllocateResource test (`jne success`) with the size guard
//   jumping forward to it; two separate `if (...) return false;` statements keep the first copy and jump back to it.
#include "engine/CContainmentVolume.h"

void* __cdecl operator new[](unsigned int size);   // MSVCR71 import thunk 0x00BFEB22 (the lane <new> lacks it)

class C3DVector { public:
    float X;
    float Y;
    float Z;
};

class CPlane { public:
    C3DVector Normal;
    float     Distance;
    CPlane();                                                      // 0x0063EC80 (empty, out of line)
};

CPlane* __fastcall CopyPlanes(const CPlane* first, const CPlane* last, CPlane* dest);   // 0x00A759E0

class CHandleBasedMemoryPool { public:
    void* GetPointerFromHandle(long handle);                       // 0x00A026B0
};

class CMovableResourceMemoryPool : public CHandleBasedMemoryPool { public:
    bool AllocateResource(long* handle, void* owner, unsigned long size, unsigned long alignment);   // 0x00A62E40
};

static __forceinline bool IsValidSize(__int64 size) { return size >= 0 && size <= 0x7fffffff; }
static __forceinline bool AllocateChecked(CMovableResourceMemoryPool* pool, long* handle, unsigned long bytes, void* owner);

struct CContainmentVolume_Methods : CContainmentVolume {
    bool Initialise(const CPlane* planes, unsigned long count, CMovableResourceMemoryPool* pool);
};

static __forceinline bool AllocateChecked(CMovableResourceMemoryPool* pool, long* handle, unsigned long bytes, void* owner)
{
    if (!IsValidSize((__int64)bytes))
        return false;
    return pool->AllocateResource(handle, owner, bytes, 0);
}
bool CContainmentVolume_Methods::Initialise(const CPlane* planes, unsigned long count, CMovableResourceMemoryPool* pool)
{
    Size = count;
    if (count == 0)
        return true;
    MemoryPool = (::CMovableResourceMemoryPool*)pool;
    if (pool) {
        unsigned long bytes = count * sizeof(CPlane);
        long handle;
        if (!AllocateChecked(pool, &handle, bytes, this))
            return false;
        ContainmentPlanes = (::CPlane*)((CMovableResourceMemoryPool*)MemoryPool)->GetPointerFromHandle(handle);
    }
    else
        ContainmentPlanes = (::CPlane*)new CPlane[count];
    CopyPlanes(planes, planes + count, (CPlane*)ContainmentPlanes);
    return true;
}