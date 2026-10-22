// C3DMeshInfo::GetMesh(CCountedPointer<const C3DMesh2>&, unsigned long lod, unsigned long flags) const -- hand out the mesh of the requested LOD (clamped and walked down to one with data), touching it; otherwise start loading it (unless flag 4), optionally wait for it (flag 0x10 + 1|2), fall back to any lower then higher LOD that already has a mesh, or (flag 1 without 4) block on the load and build it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A25E70, length 755 bytes (`ret 0Ch`, returns bool).
// Callees: BeginLoading 0x00A256F0 (urgent = flag 0x20), CCountedPointer<const C3DMesh2>::operator= 0x00A2ACC0 (byte-identical
//   COMDAT, manifest "CCountedPointer<CDiskFileWin32>::operator="), CCharString 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   DisplayProgress 0x009E9F40, CBankFileAsyncData::WaitUntilFinished 0x009D5B60, CResource::AddToResourceList 0x009FC440
//   (manifest "CTCActivationTrigger::OnKill" is propagated), C3DMeshLODInfo::BuildMesh(bool, bool) 0x00A24520,
//   CResource::SetAsUnloadable 0x009FBF50, ListRemove 0x009FBFB0 / ListInsert 0x009FBF90, operator delete 0x00BFE9BC.
// Shape: the sibling of the landed GetMeshStats 0x00A25530 (same local model). GetLODInfo is the inline
//   `MeshData ? LODMeshes[lod] : 0`; the upward walk tests the inline IsLODPresent(i) predicate (unrotated). Retail
//   calls the counted-pointer assignment out of line at four sites and inlines it (Info read first) only at the final
//   blocking-load site, so the type is modelled non-template with a same-TU auto_inline(off) operator= stand-in and an
//   inline Assign for the last site (letting VC choose inlined the last two sites).
// Layout: engine/C3DMeshInfo.h MeshData +0xc, LODDataSizes[4] +0x50; C3DMeshLODInfo Mesh +0x1c, FileData +0x24 (as
//   GetMeshStats, modelled locally for the polymorphic CResource base).
// Leaf: GetMesh
struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void __fastcall DisplayProgress(const CCharString& text, bool flag, float fraction, bool flag2);   // 0x009E9F40

struct CResourceList;
struct CResource {
    virtual void v00();
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
void __cdecl operator delete(void* p);     // 0x00BFE9BC
struct SCountBlock {
    long m_references;                             // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
};
struct C3DMesh2;
struct CMeshPointer {                              // CCountedPointer<const C3DMesh2>
    C3DMesh2* Object;                              // +0
    SCountBlock* Info;                             // +4
    CMeshPointer& operator=(const CMeshPointer& other);   // 0x00A2ACC0 (out of line at four sites)
    __forceinline void Assign(const CMeshPointer& other)
    {
        SCountBlock* c = other.Info; C3DMesh2* p = other.Object;
        if (Info != c) {
            if (Info) { --Info->m_references; if (!Info->m_references) { Info->m_pfnDestroy(Info->m_pObject); delete Info; } }
            Object = p; Info = c; if (c) ++c->m_references;
        }
    }
};
template <class T> struct CCountedPointer {
    T* Object;
    SCountBlock* Info;
    T* operator->() const { return Object; }
};
struct CBankFileAsyncData { bool WaitUntilFinished(); };   // 0x009D5B60
struct C3DMesh2 {};

struct C3DMeshLODInfo : CResource {
    CMeshPointer Mesh;        // +0x1c
    CCountedPointer<CBankFileAsyncData> FileData;   // +0x24
    void* MeshInfo;                        // +0x2c
    unsigned char LOD;                     // +0x30
    bool UsedMeshStats;                    // +0x31
    bool BuildMesh(bool a, bool b);        // 0x00A24520
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
    virtual void v00();
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
    void BeginLoading(unsigned long lod, bool unusedFlag, bool urgent) const;   // 0x00A256F0
    bool IsLODPresent(unsigned long lod) const { return lod < 4 && LODDataSizes[lod] != 0; }
    C3DMeshLODInfo* GetLODInfo(unsigned long lod) const { return MeshData ? MeshData->LODMeshes[lod] : 0; }
    bool GetMesh(CMeshPointer& out, unsigned long lod, unsigned long flags) const;
};

bool C3DMeshInfo::GetMesh(CMeshPointer& out, unsigned long lod, unsigned long flags) const
{
    if (lod > 3)
        lod = 3;
    while (LODDataSizes[lod] == 0)
        --lod;
    C3DMeshLODInfo* info = GetLODInfo(lod);
    if (info) {
        info->Touch();
        if (info->Mesh.Object) {
            out = info->Mesh;
            return true;
        }
    }
    if (!(flags & 4)) {
        BeginLoading(lod, true, ((flags >> 5) & 1) != 0);
        info = GetLODInfo(lod);
        if (flags & 0x10) {
            if (flags & 3) {
                DisplayProgress("Asset Wait: Mesh 1", false, -1.0f, false);
                if (info->FileData->WaitUntilFinished()) {
                    info->AddToResourceList();
                    info->BuildMesh(true, false);
                }
                if (info->Mesh.Object) {
                    out = info->Mesh;
                    return true;
                }
            }
            return false;
        }
    }
    if (MeshData) {
        for (long i = lod - 1; i >= 0; --i) {
            C3DMeshLODInfo* lower = MeshData->LODMeshes[i];
            if (lower && lower->Mesh.Object) {
                lower->Touch();
                if (lower->Mesh.Object) {
                    out = lower->Mesh;
                    return true;
                }
                return false;
            }
        }
        for (unsigned long j = lod + 1; IsLODPresent(j); ++j) {
            C3DMeshLODInfo* upper = MeshData->LODMeshes[j];
            if (upper && upper->Mesh.Object) {
                upper->Touch();
                if (upper->Mesh.Object) {
                    out = upper->Mesh;
                    return true;
                }
                return false;
            }
        }
    }
    if ((flags & 5) == 1) {
        DisplayProgress("Asset Wait: Mesh 2", false, -1.0f, false);
        info->FileData->WaitUntilFinished();
        info->AddToResourceList();
        info->BuildMesh(true, ((flags >> 6) & 1) != 0);
        if (flags & 8)
            info->SetAsUnloadable();
        if (info->Mesh.Object) {
            out.Assign(info->Mesh);
            return true;
        }
    }
    return false;
}

#pragma auto_inline(off)
CMeshPointer& CMeshPointer::operator=(const CMeshPointer& other)
{
    C3DMesh2* p = other.Object; SCountBlock* c = other.Info;
    if (Info != c) {
        if (Info) { --Info->m_references; if (!Info->m_references) { Info->m_pfnDestroy(Info->m_pObject); delete Info; } }
        Object = p; Info = c; if (c) ++c->m_references;
    }
    return *this;
}
#pragma auto_inline(on)
