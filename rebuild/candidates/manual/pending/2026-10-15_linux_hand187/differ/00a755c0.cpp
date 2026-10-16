// Grade: DIFFER(174v174) -- block placement: retail falls through into the shared return-false block right after the AllocateResource call (jne success), ours places it apart | tried: combined || guard, MemoryPool-> reread for GetPointerFromHandle (fixed regs), split guards | next: forceinline AllocateChecked wrapper (guard + call returning bool) inside one if
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
//   `__int64` 0..0x7fffffff guard as the stream reads; the handle lives in the dead `count` slot.
#include "engine/CContainmentVolume.h"

struct C3DVector {
    float X;
    float Y;
    float Z;
};

struct CPlane {
    C3DVector Normal;
    float     Distance;
    CPlane();                                                      // 0x0063EC80 (empty, out of line)
};

CPlane* __fastcall CopyPlanes(const CPlane* first, const CPlane* last, CPlane* dest);   // 0x00A759E0

struct CHandleBasedMemoryPool {
    void* GetPointerFromHandle(long handle);                       // 0x00A026B0
};

struct CMovableResourceMemoryPool : CHandleBasedMemoryPool {
    bool AllocateResource(long* handle, void* owner, unsigned long size, unsigned long alignment);   // 0x00A62E40
};

static __forceinline bool IsValidSize(__int64 size) { return size >= 0 && size <= 0x7fffffff; }

struct CContainmentVolume_Methods : CContainmentVolume {
    bool Initialise(const CPlane* planes, unsigned long count, CMovableResourceMemoryPool* pool);
};

bool CContainmentVolume_Methods::Initialise(const CPlane* planes, unsigned long count, CMovableResourceMemoryPool* pool)
{
    Size = count;
    if (count == 0)
        return true;
    MemoryPool = (::CMovableResourceMemoryPool*)pool;
    if (pool) {
        unsigned long bytes = count * sizeof(CPlane);
        long handle;
        if (!IsValidSize((__int64)bytes))
            return false;
        if (!pool->AllocateResource(&handle, this, bytes, 0))
            return false;
        ContainmentPlanes = (::CPlane*)((CMovableResourceMemoryPool*)MemoryPool)->GetPointerFromHandle(handle);
    }
    else
        ContainmentPlanes = (::CPlane*)new CPlane[count];
    CopyPlanes(planes, planes + count, (CPlane*)ContainmentPlanes);
    return true;
}
