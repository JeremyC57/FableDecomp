// C3DMeshInfo::GetMeshStats() -- the mesh's stats: from the stats LOD or any loaded LOD that already has a mesh, else load the stats synchronously (with an "Asset Wait" progress line) and build them
// Leaf: GetMeshStats
// Retail @ 0x00A25530, 439 bytes (`ret 4`, hidden result). Callees: BeginLoadingStats(bool) 0x00A24A80, DisplayProgress
//   0x009E9F40, CCharString 0x0099EBF0 / ~CCharString 0x0099EAE0, CBankFileAsyncData::WaitUntilFinished 0x009D5B60,
//   CResource::AddToResourceList 0x009FC440 (manifest "CTCActivationTrigger::OnKill" is propagated), SetAsUnloadable
//   0x009FBF50, ListRemove 0x009FBFB0 / ListInsert 0x009FBF90, the LOD's virtual Build (slot 5) and Delete (slot 1).
// Shape: both LOD walks test an inline `IsLODPresent(i)` predicate (`i < 4 && LODDataSizes[i]`) -- it keeps retail's
//   unrotated loop (`cmp ecx,4 ; jae` at the top, `jmp` back); the plain `&&` condition is rotated. The resource
//   touch is a `__forceinline` move-to-tail (a plain inline member is called out of line here). The fallback LOD index
//   is an inline returning -1 when nothing is loaded (retail reads LODMeshes[-1]); the build keeps the LOD alive with
//   an explicit ref-count bump and reloads straight into MeshStats when the build released the last reference.
// Layout: engine/C3DMeshInfo.h MeshData +0xc, LODDataSizes[4] +0x50; CMeshData LODMeshes[4] +8, MeshStats +0x18 (landed
//   RemoveLOD 0x00A249A0); engine/C3DMeshLODInfo.h is a CResource (list +8, LastUsedFrame +0x18) with Mesh +0x1c
//   (CCountedPointer<C3DMesh2>), FileData +0x24, UsedMeshStats +0x31 -- modelled locally for the polymorphic base.
struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void __fastcall DisplayProgress(const CCharString& text, bool flag, float fraction, bool flag2);   // 0x009E9F40

struct CResourceList;
struct CResource {
    virtual void Destroy(int flags);
    virtual void Delete();                 // slot 1 (+4)
    virtual void v08(); virtual void v0c(); virtual void v10();
    virtual void Build();                  // slot 5 (+0x14)
    unsigned long IVRefCount;              // +4
    CResourceList* ResourceList;           // +8
    CResource* PrevResource;               // +0xc
    CResource* NextResource;               // +0x10
    unsigned long ResourceSize;            // +0x14
    unsigned long LastUsedFrame;           // +0x18
    void ListRemove();                     // 0x009FBFB0
    void ListInsert(CResource* prev, CResource* next);   // 0x009FBF90
    void SetAsUnloadable();                // 0x009FBF50
    void AddToResourceList();              // 0x009FC440
};
struct CResourceList {
    long m_pad0;
    CResource Head;                        // +4
    unsigned long ResourceCount;           // +0x20
    long AllocatedMemory;                  // +0x24
    long MaximumMemory;                    // +0x28
    unsigned long CurrentFrame;            // +0x2c
};

struct SCountBlock { long m_references; };
template <class T> struct CCountedPointer {
    T* m_pObject;
    SCountBlock* m_pCount;
    template <class U> CCountedPointer(const CCountedPointer<U>& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount) ++m_pCount->m_references;
    }
};

struct CBankFileAsyncData { bool WaitUntilFinished(); };   // 0x009D5B60
struct C3DMeshStats {};
struct C3DMesh2 : C3DMeshStats {};

struct C3DMeshLODInfo : CResource {
    CCountedPointer<C3DMesh2> Mesh;        // +0x1c
    CBankFileAsyncData* FileData;          // +0x24 CCountedPointer<CBankFileAsyncData>::Data
    void* FileDataInfo;                    // +0x28
    void* MeshInfo;                        // +0x2c
    unsigned char LOD;                     // +0x30
    bool UsedMeshStats;                    // +0x31
    __forceinline void Touch()
    {
        if (ResourceList && LastUsedFrame != ResourceList->CurrentFrame) {
            ListRemove();
            ListInsert(ResourceList->Head.PrevResource, &ResourceList->Head);
            LastUsedFrame = ResourceList->CurrentFrame;
        }
    }
};
struct CMeshData {
    virtual void Destroy(int flags);
    unsigned long IVRefCount;              // +4
    C3DMeshLODInfo* LODMeshes[4];          // +8 CIVCountedPointer<C3DMeshLODInfo>
    C3DMeshLODInfo* MeshStats;             // +0x18
};

class C3DMeshInfo {
public:
    virtual void v00();
    long m_04, m_08;
    CMeshData* MeshData;                   // +0xc CIVCountedPointer<CMeshData>
    unsigned char _10[0x40];
    unsigned long LODDataSizes[4];         // +0x50
    void BeginLoadingStats(bool wait);     // 0x00A24A80
    bool IsLODPresent(unsigned long lod) const { return lod < 4 && LODDataSizes[lod] != 0; }
    long FindLoadedLOD() const
    {
        if (MeshData) {
            for (unsigned long i = 0; IsLODPresent(i); ++i)
                if (MeshData->LODMeshes[i])
                    return i;
        }
        return -1;
    }
    CCountedPointer<const C3DMeshStats> GetMeshStats();
};

CCountedPointer<const C3DMeshStats> C3DMeshInfo::GetMeshStats()
{
    bool load = true;
    if (MeshData) {
        C3DMeshLODInfo* stats = MeshData->MeshStats;
        if (stats) {
            if (stats->Mesh.m_pObject) {
                stats->Touch();
                return stats->Mesh;
            }
            load = false;
        }
        for (unsigned long i = 0; IsLODPresent(i); ++i) {
            C3DMeshLODInfo* lod = MeshData->LODMeshes[i];
            if (lod) {
                if (lod->Mesh.m_pObject) {
                    lod->Touch();
                    lod->UsedMeshStats = true;
                    return lod->Mesh;
                }
                load = false;
            }
        }
    }
    if (load)
        BeginLoadingStats(true);
    C3DMeshLODInfo* info = MeshData->MeshStats;
    if (!info)
        info = MeshData->LODMeshes[FindLoadedLOD()];
    DisplayProgress("Asset Wait: Mesh Stats", false, -1.0f, false);
    info->FileData->WaitUntilFinished();
    info->AddToResourceList();
    ++info->IVRefCount;
    info->Build();
    if (--info->IVRefCount == 0) {
        info->Delete();
        BeginLoadingStats(true);
        info = MeshData->MeshStats;
        info->FileData->WaitUntilFinished();
        info->AddToResourceList();
        info->Build();
    }
    info->SetAsUnloadable();
    info->UsedMeshStats = true;
    return info->Mesh;
}