// C3DMeshInfo::BeginLoadingStats(bool) -- create the mesh data if needed and, unless a LOD is already loaded, start the asynchronous read of the stats block that follows the LODs
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A24A80, length 592 bytes (`ret 4`; the flag is unused).
// Callees: operator new @ 0x00BFEA1A, CResource::CResource @ 0x009FBEC0, CMeshDataBank::CreateEntry @ 0x009D58D0,
//          CBankFile::GetEntryDataSize @ 0x009CC240, CBankFileAsyncEntry::BeginReadDataAutoAlloc @ 0x009D6100,
//          CResourceBank::AddResource @ 0x009FC3D0, C3DMesh2::Sizeof @ 0x00A893C0, CBankFileAsyncData::Sizeof @ 0x009D5620,
//          CResource::SetResourceSize @ 0x009FBF20, operator delete @ 0x00BFE9BC.
// Shape: the sibling of BeginLoading 0x00A256F0. New mesh data is numbered from the global counter; existing mesh data
// with any loaded LOD (the inline first-loaded-LOD search, -1 when none) needs nothing. The stats block is the entry's
// bytes after every present LOD: offset = the LOD sizes summed, size = entry size minus them; a C3DMeshLODInfo with
// LOD index 4 goes into MeshStats, its read is started at the normal priority, then the bank add and the size record.
// Layout: as BeginLoading (engine/C3DMeshInfo.h; CMeshData MeshStats +0x18, MeshID +0x1c from the landed RemoveLOD).
// Loop/order: both LOD walks test an inline IsLODPresent(i) predicate (the landed GetMeshStats 0x00A25530 idiom keeps them
//   unrotated); `offset = 0` is declared before the size call (retail zeroes edi first); the counted-pointer copy reads
//   Info before Object.
// Leaf: BeginLoadingStats
void* __cdecl operator new(unsigned int size);         // 0x00BFEA1A
struct SCountBlock {
    long m_references;                             // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
};
template <class T> class CCountedPointer { public:
    T* Object;                                     // +0
    SCountBlock* Info;                             // +4
    CCountedPointer() : Object(0), Info(0) {}
    ~CCountedPointer()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                delete Info;
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        SCountBlock* c = other.Info; T* p = other.Object;
        if (Info != c) {
            if (Info) { --Info->m_references; if (!Info->m_references) { Info->m_pfnDestroy(Info->m_pObject); delete Info; } }
            Object = p; Info = c; if (c) ++c->m_references;
        }
        return *this;
    }
    T* operator->() const { return Object; }
};
enum EThreadedFilePriority { THREADED_FILE_PRIORITY_HIGH = 4 };
extern EThreadedFilePriority g_MeshReadPriority;   // 0x01397240
extern unsigned long g_NextMeshID;                 // 0x01397248
class CBankFileAsyncData { public:
    unsigned long Sizeof() const;                  // 0x009D5620
};
class CBankFile { public:
    unsigned long GetEntryDataSize(unsigned long index);   // 0x009CC240
};
class CBankFileAsyncEntry { public:
    void*          __vftable;
    CBankFile*     Bank;                           // +4
    unsigned long  Index;                          // +8
    CCountedPointer<CBankFileAsyncData> BeginReadDataAutoAlloc(unsigned long offset, unsigned long size, EThreadedFilePriority priority);   // 0x009D6100
};
class C3DMesh2 { public: unsigned long Sizeof() const; };   // 0x00A893C0

class CResourceList;
class CResource { public:
    virtual void v00();
    virtual void DeleteThis();                     // slot 1
    long           RefCount;                       // +0x04
    CResourceList* ResourceList;                   // +0x08
    CResource*     PrevResource;                   // +0x0c
    CResource*     NextResource;                   // +0x10
    unsigned long  ResourceSize;                   // +0x14
    unsigned long  LastUsedFrame;                  // +0x18
    CResource();                                   // 0x009FBEC0
    void SetResourceSize(unsigned long size);      // 0x009FBF20
    void ListRemove();                             // 0x009FBFB0
    void ListInsert(CResource* prev, CResource* next);   // 0x009FBF90
    void Touch();
};
class CResourceList { public:
    long m_pad0;                                   // +0
    CResource Head;                                // +4
    unsigned long ResourceCount;                   // +0x20
    long AllocatedMemory;                          // +0x24
    long MaximumMemory;                            // +0x28
    unsigned long CurrentFrame;                    // +0x2c
};
inline void CResource::Touch()
{
    if (ResourceList && LastUsedFrame != ResourceList->CurrentFrame) {
        ListRemove();
        ListInsert(ResourceList->Head.PrevResource, &ResourceList->Head);
        LastUsedFrame = ResourceList->CurrentFrame;
    }
}
class CResourceBank { public:
    void AddResource(CResource* resource);         // 0x009FC3D0
};
class CMeshDataBank { public:
    unsigned char _pad000[0x170];
    CResourceBank ResourceBank;                    // +0x170
    CCountedPointer<CBankFileAsyncEntry> CreateEntry(unsigned long index);   // 0x009D58D0
};

template <class T> class CIVCountedPointer { public:
    T* m_pObject;
    CIVCountedPointer() : m_pObject(0) {}
    __forceinline CIVCountedPointer& operator=(T* object)
    {
        if (T* old = m_pObject) {
            if (--old->RefCount == 0)
                old->DeleteThis();
            m_pObject = 0;
        }
        m_pObject = object;
        return *this;
    }
    T* operator->() const { return m_pObject; }
};

class C3DMeshInfo;
class C3DMeshLODInfo : public CResource { public:                // vtable 0x0129CDA4
    CCountedPointer<C3DMesh2>           Mesh;            // +0x1c
    CCountedPointer<CBankFileAsyncData> FileData;        // +0x24
    C3DMeshInfo*                        MeshInfo;        // +0x2c
    unsigned char                       LOD;             // +0x30
    bool                                UsedMeshStats;   // +0x31
    virtual void v00();
    C3DMeshLODInfo(C3DMeshInfo* meshInfo, unsigned char lod) : MeshInfo(meshInfo), LOD(lod), UsedMeshStats(false) {}
};
struct __declspec(novtable) CCountedBase {
    virtual void v00();
    virtual void DeleteThis();                     // slot 1
    long RefCount;                                 // +0x04
    CCountedBase() : RefCount(1) {}
};
struct CMeshData : CCountedBase {                  // C3DMeshInfo::CMeshData, vtable 0x0129CDD0
    CIVCountedPointer<C3DMeshLODInfo> LODMeshes[4];   // +0x08
    CIVCountedPointer<C3DMeshLODInfo> MeshStats;      // +0x18
    unsigned long MeshID;                             // +0x1c
    virtual void v00();
};
class C3DMeshInfo { public:
    virtual void v00();
    CMeshDataBank*               Bank;             // +0x04
    unsigned long                Index;            // +0x08
    CIVCountedPointer<CMeshData> MeshData;         // +0x0c
    unsigned char _pad10[0x40];
    unsigned long LODDataSizes[4];                 // +0x50
    bool IsLODPresent(unsigned long lod) const { return lod < 4 && LODDataSizes[lod] != 0; }
    __forceinline long GetFirstLoadedLOD() const
    {
        for (unsigned long i = 0; IsLODPresent(i); ++i)
            if (MeshData->LODMeshes[i].m_pObject)
                return i;
        return -1;
    }
    void BeginLoadingStats(bool unusedFlag);
};

void C3DMeshInfo::BeginLoadingStats(bool unusedFlag)
{
    if (!MeshData.m_pObject) {
        MeshData = new CMeshData;
        unsigned long id = g_NextMeshID++;
        MeshData->MeshID = id;
    }
    else if (GetFirstLoadedLOD() != -1)
        return;
    CCountedPointer<CBankFileAsyncEntry> entry = Bank->CreateEntry(Index);
    unsigned long offset = 0;
    unsigned long size = entry->Bank->GetEntryDataSize(entry->Index);
    for (unsigned long i = 0; IsLODPresent(i); ++i) {
        offset += LODDataSizes[i];
        size -= LODDataSizes[i];
    }
    CIVCountedPointer<C3DMeshLODInfo>& slot = MeshData->MeshStats;
    slot = new C3DMeshLODInfo(this, 4);
    slot->FileData = entry->BeginReadDataAutoAlloc(offset, size, g_MeshReadPriority);
    Bank->ResourceBank.AddResource(slot.m_pObject);
    C3DMeshLODInfo* info = slot.m_pObject;
    unsigned long meshSize = info->Mesh.Object ? info->Mesh->Sizeof() + 8 : 8;
    unsigned long dataSize = info->FileData.Object ? info->FileData->Sizeof() + 8 : 8;
    slot->SetResourceSize(dataSize + meshSize + 0x34);
}