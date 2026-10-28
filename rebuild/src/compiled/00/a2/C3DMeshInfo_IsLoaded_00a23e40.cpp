// C3DMeshInfo::IsLoaded() -- true when any present LOD of the mesh data already holds its mesh
// Leaf: ?IsLoaded@C3DMeshInfo@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A23E40, 59 bytes. Name: Ego_r/FableWin PDB ?IsLoaded@C3DMeshInfo@@QBE_NXZ (catalog
//   row agrees); its body is the tail of IsLoadedStats 0x00A23E80 (the next row), which adds the stats-slot test first.
// Layout: engine/C3DMeshInfo.h offsets (MeshData +0xC, LODDataSizes[4] +0x50); CMeshData LODMeshes[4] +8, MeshStats +0x18;
//   C3DMeshLODInfo is a CResource (0x1C) with Mesh (CCountedPointer<C3DMesh2>) at +0x1C -- the landed RemoveLOD /
//   GetMeshStats models. Local polymorphic model: the generated headers flatten the vptr and the CResource base.
// Shape: the walk tests the inline HasLOD(i) (`i < 4 && LODDataSizes[i] != 0`), which keeps retail's unrotated loop
//   (`cmp esi,4 ; jae` at the top, `jmp` back); one shared false block.
class CResourceList;
class CResource {                                  // CIVCountedPointeeBase + resource list links (0x1C bytes)
public:
    virtual void Destroy(int flags);
    virtual void Delete();                         // slot 1 (+4): the last reference
    unsigned long  IVRefCount;                     // +0x04
    CResourceList* ResourceList;                   // +0x08
    CResource*     PrevResource;                   // +0x0c
    CResource*     NextResource;                   // +0x10
    unsigned long  ResourceSize;                   // +0x14
    unsigned long  LastUsedFrame;                  // +0x18
};
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
};
template <class T> class CIVCountedPointer {
public:
    T* Object;
    T* Get() const { return Object; }
    T* operator->() const { return Object; }
    bool IsNull() const { return Object == 0; }
};
class C3DMesh2;
class CBankFileAsyncData;
class C3DMeshInfo;
class C3DMeshLODInfo : public CResource {
public:
    CCountedPointer<C3DMesh2>           Mesh;          // +0x1c
    CCountedPointer<CBankFileAsyncData> FileData;      // +0x24
    C3DMeshInfo*                        MeshInfo;      // +0x2c
    unsigned char                       LOD;           // +0x30
    bool                                UsedMeshStats; // +0x31
};
class C3DMeshInfo {
public:
    class CMeshData {
    public:
        virtual ~CMeshData();
        unsigned long                     IVRefCount;     // +0x04
        CIVCountedPointer<C3DMeshLODInfo> LODMeshes[4];   // +0x08
        CIVCountedPointer<C3DMeshLODInfo> MeshStats;      // +0x18
        unsigned long                     MeshID;         // +0x1c
    };
    virtual unsigned long Sizeof() const;
    long                          f4;                     // +0x04 (retail-only, engine/C3DMeshInfo.h)
    long                          f8;                     // +0x08
    CIVCountedPointer<CMeshData>  MeshData;               // +0x0c
    unsigned char                 BoundingBox[0x18];      // +0x10 C3DBoundingBox
    unsigned char                 BoundingSphere[0x10];   // +0x28 C3DBoundingSphere
    float                         SafeBoundingRadius;     // +0x38
    long                          PhysicsMeshIndex;       // +0x3c
    float                         LODErrors[4];           // +0x40
    unsigned long                 LODDataSizes[4];        // +0x50
    bool HasLOD(unsigned long lod) const { return lod < 4 && LODDataSizes[lod] != 0; }
    bool IsLoaded() const;
    bool IsLoadedStats() const;
private:
    virtual void Reset();
};

bool C3DMeshInfo::IsLoaded() const
{
    if (!MeshData.IsNull()) {
        for (unsigned long lod = 0; HasLOD(lod); ++lod) {
            C3DMeshLODInfo* info = MeshData->LODMeshes[lod].Get();
            if (info && info->Mesh.Data)
                return true;
        }
    }
    return false;
}