// C3DMeshInfo::IsLoadedStats() -- true when the mesh stats slot or any present LOD already holds its mesh
// Leaf: ?IsLoadedStats@C3DMeshInfo@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A23E80, 74 bytes. Name: Ego_r/FableWin PDB ?IsLoadedStats@C3DMeshInfo@@QBE_NXZ
//   (catalog row agrees): IsLoaded (0x00A23E40) with the MeshStats slot tested first.
// Layout: as 0x00A23E40 (same local model).
// Shape: the stats test then the HasLOD(i) walk (unrotated, `nop` pad before the loop head); one shared false block.
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

bool C3DMeshInfo::IsLoadedStats() const
{
    if (!MeshData.IsNull()) {
        C3DMeshLODInfo* stats = MeshData->MeshStats.Get();
        if (stats && stats->Mesh.Data)
            return true;
        for (unsigned long lod = 0; HasLOD(lod); ++lod) {
            C3DMeshLODInfo* info = MeshData->LODMeshes[lod].Get();
            if (info && info->Mesh.Data)
                return true;
        }
    }
    return false;
}