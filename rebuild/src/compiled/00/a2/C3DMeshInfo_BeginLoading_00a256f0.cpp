// C3DMeshInfo::BeginLoading(unsigned long, bool, bool) -- touch an already-loaded LOD, or create the mesh data and start the asynchronous read of the requested LOD's bytes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A256F0, length 640 bytes (`ret 0Ch`; the second argument is unused).
// Callees: CResource::ListRemove @ 0x009FBFB0 / ListInsert @ 0x009FBF90 (the inline Touch), operator new @ 0x00BFEA1A,
//          CResource::CResource @ 0x009FBEC0, CMeshDataBank::CreateEntry @ 0x009D58D0, CBankFileAsyncEntry::
//          BeginReadDataAutoAlloc @ 0x009D6100, CResourceBank::AddResource @ 0x009FC3D0, C3DMesh2::Sizeof @ 0x00A893C0,
//          CBankFileAsyncData::Sizeof @ 0x009D5620, CResource::SetResourceSize @ 0x009FBF20, operator delete @ 0x00BFE9BC.
// Shape: the LOD is clamped to 3 and walked down to one with data; a loaded LOD is only touched (the landed resource-list
// move-to-tail, 0x00A42FC0). Otherwise: the mesh data is created on first use (inline CMeshData ctor: novtable count base,
// four empty LOD pointers and the stats pointer) and numbered from the global mesh counter; the bank entry is opened,
// the offset summed over the lower LODs, a new C3DMeshLODInfo stored in the LOD slot (a reference local), its file read
// started (priority 4 when urgent), the resource added to the bank's list and its size recorded -- the landed
// CPhysicsMeshInfo::BeginLoading 0x00A25BD0 family.
// Layout: C3DMeshInfo Bank +4, Index +8, MeshData +0xc, LODDataSizes[4] +0x50 (engine/C3DMeshInfo.h); CMeshData and
// C3DMeshLODInfo from the landed RemoveLOD 0x00A249A0 / C3DMeshLODInfo ctor 0x00A24430.
// Leaf: BeginLoading
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
        T* p = other.Object; SCountBlock* c = other.Info;
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
    void BeginLoading(unsigned long lod, bool unusedFlag, bool urgent);
};

void C3DMeshInfo::BeginLoading(unsigned long lod, bool unusedFlag, bool urgent)
{
    if (lod > 3)
        lod = 3;
    while (LODDataSizes[lod] == 0)
        --lod;
    if (MeshData.m_pObject && MeshData->LODMeshes[lod].m_pObject) {
        MeshData->LODMeshes[lod]->Touch();
        return;
    }
    unsigned long size = LODDataSizes[lod];
    if (!MeshData.m_pObject) {
        MeshData = new CMeshData;
        unsigned long id = g_NextMeshID++;
        MeshData->MeshID = id;
    }
    CCountedPointer<CBankFileAsyncEntry> entry = Bank->CreateEntry(Index);
    unsigned long offset = 0;
    for (unsigned long i = 0; i < lod; ++i)
        offset += LODDataSizes[i];
    CIVCountedPointer<C3DMeshLODInfo>& slot = MeshData->LODMeshes[lod];
    slot = new C3DMeshLODInfo(this, (unsigned char)lod);
    EThreadedFilePriority priority = g_MeshReadPriority;
    if (urgent)
        priority = THREADED_FILE_PRIORITY_HIGH;
    slot->FileData = entry->BeginReadDataAutoAlloc(offset, size, priority);
    Bank->ResourceBank.AddResource(slot.m_pObject);
    C3DMeshLODInfo* info = slot.m_pObject;
    unsigned long meshSize = info->Mesh.Object ? info->Mesh->Sizeof() + 8 : 8;
    unsigned long dataSize = info->FileData.Object ? info->FileData->Sizeof() + 8 : 8;
    slot->SetResourceSize(dataSize + meshSize + 0x34);
}