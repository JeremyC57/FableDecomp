// C3DMeshInfo::Sizeof() const -- memory footprint of a mesh info: itself plus each loaded LOD's and the mesh stats' C3DMeshLODInfo footprint
// Grade: DIFFER(ours v retail) -- loop rotation only: retail keeps the lod<4 test at the loop top (cmp ebx,18h ; jae, back-edge jmp) where ours drops the first test and moves it to the bottom (jb)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A26170, 279 bytes (virtual `?Sizeof@C3DMeshInfo@@UBEKXZ`, `ret`).
// Callees: C3DMesh2::Sizeof 0x00A893C0 (this TU's copy), CBankFileAsyncData::Sizeof 0x009D5620 (manifest
// "CVertexBufferWin32::DoSizeof" is propagated).
// Shape: sizeof(C3DMeshInfo) (0x60), then with mesh data: `for (lod < 4 && LODDataSizes[lod])` each LOD pointer adds
// `p ? p->Sizeof() + 4 : 4` with C3DMeshLODInfo::Sizeof inlined (the landed out-of-line copy 0x00A42AF0: 0x34 + the two
// counted pointers' `p ? p->Sizeof() + 8 : 8`, the +4 folded into 0x38), then the same for MeshStats (VC duplicates
// the three return tails). The LOD index is strength-reduced to a byte offset into the mesh data (ebx 8..0x18).
// Layout: engine/C3DMeshInfo.h (MeshData +0x0c, LODDataSizes +0x50, 0x60 bytes), engine/C3DMeshLODInfo.h (Mesh +0x1c,
// FileData +0x24, 0x34 bytes); Ego_r.pdb C3DMeshInfo::CMeshData: LODMeshes[4] +8, MeshStats +0x18, MeshID +0x1c.
// Leaf: Sizeof@C3DMeshInfo_Methods
#include "engine/C3DMeshInfo.h"
#include "engine/C3DMeshLODInfo.h"

struct C3DMesh2           { unsigned long Sizeof() const; };   // 0x00A893C0
struct CBankFileAsyncData { unsigned long Sizeof() const; };   // 0x009D5620

struct C3DMeshLODInfo_Methods : C3DMeshLODInfo {
    unsigned long Sizeof() const
    {
        unsigned long meshSize = Mesh_Data ? Mesh_Data->Sizeof() + 8 : 8;
        unsigned long fileSize = FileData_Data ? FileData_Data->Sizeof() + 8 : 8;
        return sizeof(C3DMeshLODInfo) + meshSize + fileSize;
    }
};

template <class T> struct CIVCountedPointer {
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
};

unsigned long C3DMeshInfo_Methods::Sizeof() const
{
    unsigned long size = sizeof(C3DMeshInfo);
    if (GetMeshData()) {
        for (unsigned long lod = 0; lod < 4 && LODDataSizes[lod]; ++lod)
            size += SizeofPointer(GetMeshData()->LODMeshes[lod]);
        size += SizeofPointer(GetMeshData()->MeshStats);
    }
    return size;
}
