// CEdgeStipVertexArray<CPatchTesselationEdgeStrip::CWaterFanBaseVertex>::Resize(long count, CMovableResource* resource) -- reallocate the vertex array from the landscape memory manager, keeping the common prefix
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFC140, length 185 bytes.
// Leaf: Resize
// Callees: CEngineLandscapeMemoryManager AllocVirtual @ 0x00BD9AF0 (thiscall, size + owning resource),
// FreeVirtual @ 0x00BD9B90 (thiscall); the renderer global @ 0x01436EA8 holds the native memory manager at +0x6B0.
// Label correction: the manifest calls this the CPatchTesselationEdgeStrip::CVertex instantiation, but the
// element stride is 0x38 = 56 bytes, which is the PDB CWaterFanBaseVertex (Z, float ShoreLookUpArray[12],
// VertexBufferIndex; 56 bytes) -- CVertex is 20 bytes (its Resize is landed at 0x00BFBE40), CWaterVertex
// 60 (0x00BFC040), CFanBaseVertex 16 (0x00BFBF40). This is the fourth member of the same family, so the
// model is the landed CWaterVertex Resize body with the 56-byte element.
// Shape: same as 0x00BFC040 -- the `count < Count ? count : Count` select (`cmp ; mov ; jl ; mov`), the
// manager loaded into a named local before the alloc, a `rep movsd/movsb` memcpy under `copied > 0`, the
// old block freed through a fresh global read, Count and Data stored after the free; count == 0 frees and
// zeroes both members.
#include <string.h>
// Callee and global symbols follow the catalog majority for these addresses (CEngineLandscapeMemoryManager::
// AllocVirtual(long, CMovableResource*) / FreeVirtual(void*), SEngine_01436ea8* g_pEngine_01436ea8 with the
// manager at +0x6b0, as in the landed class allocators 0x00BEA7E0 / 0x00BF6530); the generated
// engine/CEngineLandscapeMemoryManager.h carries no methods, so the callee view is local.
class CMovableResource;
class CEngineLandscapeMemoryManager { public:
    void* AllocVirtual(long size, CMovableResource* owner);   // 0x00BD9AF0
    void FreeVirtual(void* block);                            // 0x00BD9B90
};
struct SEngine_01436ea8 {
    unsigned char                  _pad[0x6b0];
    CEngineLandscapeMemoryManager* m_pLandscapeMemory;        // +0x6b0
};
extern SEngine_01436ea8* g_pEngine_01436ea8;                  // 0x01436EA8

struct CWaterFanBaseVertex {
    float          Z;                     // +0x00
    float          ShoreLookUpArray[12];  // +0x04
    unsigned short VertexBufferIndex;     // +0x34
};
typedef char CWaterFanBaseVertex_is_56_bytes[sizeof(CWaterFanBaseVertex) == 56 ? 1 : -1];

struct EdgeArray_CWaterFanBaseVertex {
    CWaterFanBaseVertex* Data;    // +0x00
    int                  Count;   // +0x04
    bool Resize(int count, CMovableResource* resource);
};

bool EdgeArray_CWaterFanBaseVertex::Resize(int count, CMovableResource* resource)
{
    if (Count != count) {
        if (count) {
            int copied = count < Count ? count : Count;
            CEngineLandscapeMemoryManager* manager = g_pEngine_01436ea8->m_pLandscapeMemory;
            CWaterFanBaseVertex* data = static_cast<CWaterFanBaseVertex*>(manager->AllocVirtual(count * sizeof(CWaterFanBaseVertex), resource));
            if (!data)
                return false;
            if (copied > 0)
                memcpy(data, Data, copied * sizeof(CWaterFanBaseVertex));
            if (Data)
                g_pEngine_01436ea8->m_pLandscapeMemory->FreeVirtual(Data);
            Count = count;
            Data = data;
        } else {
            g_pEngine_01436ea8->m_pLandscapeMemory->FreeVirtual(Data);
            Data = 0;
            Count = 0;
        }
    }
    return true;
}