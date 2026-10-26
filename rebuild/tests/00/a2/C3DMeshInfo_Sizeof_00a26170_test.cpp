// C3DMeshInfo::Sizeof() const -- memory footprint of a mesh info: itself plus each loaded LOD's and the mesh stats' C3DMeshLODInfo footprint
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A26170, 279 bytes (virtual `?Sizeof@C3DMeshInfo@@UBEKXZ`, `ret`).
// Callees: C3DMesh2::Sizeof 0x00A893C0 (this TU's copy), CBankFileAsyncData::Sizeof 0x009D5620 (manifest
// "CVertexBufferWin32::DoSizeof" is propagated).
// Shape: sizeof(C3DMeshInfo) (0x60), then with mesh data: `for (; HasLOD(lod); ++lod)` (a __forceinline early-return
// condition, which keeps the lod<4 test at the loop top with a back-edge jmp) each LOD pointer adds
// `p ? p->Sizeof() + 4 : 4` with C3DMeshLODInfo::Sizeof inlined (the landed out-of-line copy 0x00A42AF0: 0x34 + the two
// counted pointers' `p ? p->Sizeof() + 8 : 8`, the +4 folded into 0x38), then the same for MeshStats (VC duplicates
// the three return tails). The LOD index is strength-reduced to a byte offset into the mesh data (ebx 8..0x18).
// Layout: engine/C3DMeshInfo.h (MeshData +0x0c, LODDataSizes +0x50, 0x60 bytes), engine/C3DMeshLODInfo.h (Mesh +0x1c,
// FileData +0x24, 0x34 bytes); Ego_r.pdb C3DMeshInfo::CMeshData: LODMeshes[4] +8, MeshStats +0x18, MeshID +0x1c.
// Leaf: Sizeof@C3DMeshInfo_Methods
#include "engine/C3DMeshInfo.h"
#include "engine/C3DMeshLODInfo.h"

class C3DMesh2           { public: unsigned long Sizeof() const; };   // 0x00A893C0
class CBankFileAsyncData { public: unsigned long Sizeof() const; };   // 0x009D5620

struct C3DMeshLODInfo_Methods : C3DMeshLODInfo {
    unsigned long Sizeof() const
    {
        unsigned long meshSize = Mesh_Data ? Mesh_Data->Sizeof() + 8 : 8;
        unsigned long fileSize = FileData_Data ? FileData_Data->Sizeof() + 8 : 8;
        return sizeof(C3DMeshLODInfo) + meshSize + fileSize;
    }
};

template <class T> class CIVCountedPointer { public:
    T* Data;
    T* operator->() const { return Data; }
};
template <class T> inline unsigned long SizeofPointer(const CIVCountedPointer<T>& p)
{
    return p.Data ? p->Sizeof() + 4 : 4;
}

struct CMeshData {                                 // C3DMeshInfo::CMeshData (Ego_r.pdb, 0x20 bytes)
    void* __vftable;                               // +0x00 CIVCountedPointeeBase
    unsigned long IVRefCount;                      // +0x04
    CIVCountedPointer<C3DMeshLODInfo_Methods> LODMeshes[4];   // +0x08
    CIVCountedPointer<C3DMeshLODInfo_Methods> MeshStats;      // +0x18
    unsigned long MeshID;                          // +0x1c
};

struct C3DMeshInfo_Methods : C3DMeshInfo {
    const CMeshData* GetMeshData() const { return (const CMeshData*)MeshData_Object; }
    unsigned long Sizeof() const;
    __forceinline bool HasLOD(unsigned long lod) const
    {
        if (lod < 4 && LODDataSizes[lod]) return true;
        return false;
    }
};

unsigned long C3DMeshInfo_Methods::Sizeof() const
{
    unsigned long size = sizeof(C3DMeshInfo);
    if (GetMeshData()) {
        for (unsigned long lod = 0; HasLOD(lod); ++lod)
            size += SizeofPointer(GetMeshData()->LODMeshes[lod]);
        size += SizeofPointer(GetMeshData()->MeshStats);
    }
    return size;
}

// ---- test harness ----
#include <stdio.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)

static C3DMesh2 g_mesh;
static CBankFileAsyncData g_file;
unsigned long C3DMesh2::Sizeof() const { return 100; }
unsigned long CBankFileAsyncData::Sizeof() const { return 50; }

int main()
{
    static unsigned long infoBuf[0x60 / 4];
    static unsigned long lodA[sizeof(C3DMeshLODInfo) / 4 + 1], lodB[sizeof(C3DMeshLODInfo) / 4 + 1];
    static CMeshData data;
    C3DMeshLODInfo_Methods* a = (C3DMeshLODInfo_Methods*)lodA;
    C3DMeshLODInfo_Methods* b = (C3DMeshLODInfo_Methods*)lodB;
    a->Mesh_Data = &g_mesh; a->FileData_Data = &g_file;
    b->Mesh_Data = 0;       b->FileData_Data = 0;
    C3DMeshInfo_Methods* info = (C3DMeshInfo_Methods*)infoBuf;
    CHECK(info->Sizeof() == 0x60);                       // no mesh data
    data.LODMeshes[0].Data = a; data.LODMeshes[1].Data = 0; data.LODMeshes[2].Data = b; data.LODMeshes[3].Data = a;
    data.MeshStats.Data = a;
    info->MeshData_Object = (CIVCountedPointeeBase*)&data;
    info->LODDataSizes[0] = 1; info->LODDataSizes[1] = 1; info->LODDataSizes[2] = 1; info->LODDataSizes[3] = 0;
    unsigned long sa = 0x34 + 108 + 58 + 4, sb = 0x34 + 8 + 8 + 4;
    CHECK(info->Sizeof() == 0x60 + sa + 4 + sb + sa);
    info->LODDataSizes[3] = 7;
    CHECK(info->Sizeof() == 0x60 + sa + 4 + sb + sa + sa);
    info->LODDataSizes[0] = 0;                           // stops at the first empty LOD
    CHECK(info->Sizeof() == 0x60 + sa);
    data.MeshStats.Data = 0;
    CHECK(info->Sizeof() == 0x60 + 4);
    if (fails == 0) printf("00a26170_TEST PASS\n");
    return fails;
}