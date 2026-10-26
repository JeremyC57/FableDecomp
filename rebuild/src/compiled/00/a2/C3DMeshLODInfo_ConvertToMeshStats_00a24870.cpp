// C3DMeshLODInfo::ConvertToMeshStats() -- turns this LOD into the stats-only LOD 4: the mesh drops its geometry, the resource is unlinked, re-sized and re-added to the owner's resource list, which then frees space
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A24870, length 113 bytes.
// Leaf: ConvertToMeshStats
// Callees: 0x00A8B3A0 C3DMesh2::ConvertToMeshStats, 0x009FC440 CResource::RemoveFromList (`if (m_pList)
// m_pList->Remove(this)`, propagated label "CTCActivationTrigger::OnKill"), 0x00A893C0 C3DMesh2::Sizeof,
// 0x009D5620 CBankFileAsyncData::Sizeof, 0x009FBF20 CResource::SetResourceSize, 0x009FBFE0
// CResourceList::AddResource, 0x009FC4F0 CResourceList::MakeFreeSpace(0).
// Layout: engine/C3DMeshLODInfo.h (PDB agrees: Mesh +0x1c, FileData +0x24, MeshInfo +0x2c, LOD +0x30,
// sizeof 0x34); the CResource base sits at +0. The mesh info's word at +4 (the PDB header's `f4`) is its
// owning bank, whose resource list is at +0x1f0.
// Shape: the size is the landed C3DMeshLODInfo::Sizeof (0x00A42AF0) inlined: two `p ? p->Sizeof() + 8 : 8`
// ternaries summed with sizeof(C3DMeshLODInfo) into one `lea eax,[eax+edi+34h]`; the list pointer is a named
// local (kept in edi for both calls).
#include "engine/C3DMeshLODInfo.h"

class C3DMesh2 { public:
    void ConvertToMeshStats();                           // 0x00A8B3A0
    unsigned long Sizeof() const;                        // 0x00A893C0
};
class CBankFileAsyncData { public: unsigned long Sizeof() const; };   // 0x009D5620

class CResource;
class CResourceList { public:
    void AddResource(CResource* resource);               // 0x009FBFE0
    bool MakeFreeSpace(unsigned long bytes);             // 0x009FC4F0
};
struct CMeshInfoOwner {                                  // the bank owning the mesh infos
    unsigned char _pad000[0x1f0];
    CResourceList ResourceList;                          // +0x1f0
};
class C3DMeshInfo { public:
    void*           __vftable;                           // +0x00
    CMeshInfoOwner* m_pOwner;                            // +0x04 (PDB header: `f4`)
};

struct C3DMeshLODInfo_Methods : C3DMeshLODInfo {
    void RemoveFromList();                               // 0x009FC440 CResource
    void SetResourceSize(unsigned long size);            // 0x009FBF20 CResource
    unsigned long Sizeof() const
    {
        unsigned long meshSize = Mesh_Data ? Mesh_Data->Sizeof() + 8 : 8;
        unsigned long fileSize = FileData_Data ? FileData_Data->Sizeof() + 8 : 8;
        return sizeof(C3DMeshLODInfo) + meshSize + fileSize;
    }
    void ConvertToMeshStats();
};

void C3DMeshLODInfo_Methods::ConvertToMeshStats()
{
    LOD = 4;
    Mesh_Data->ConvertToMeshStats();
    RemoveFromList();
    SetResourceSize(Sizeof());
    CResourceList* list = &MeshInfo->m_pOwner->ResourceList;
    list->AddResource((CResource*)this);
    list->MakeFreeSpace(0);
}