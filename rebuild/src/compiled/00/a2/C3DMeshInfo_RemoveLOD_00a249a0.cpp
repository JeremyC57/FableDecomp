// C3DMeshInfo::RemoveLOD(unsigned long, bool) const -- drop one LOD's mesh info (index 4 = the stats slot); optionally keep a stats-capable LOD as the mesh stats
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A249A0, length 208 bytes (`ret 8`).
// MESH LAYER: Ego_r.pdb ?RemoveLOD@C3DMeshInfo@@ABEXK_N@Z.
// Callees: C3DMeshLODInfo::ConvertToMeshStats @ 0x00A24870, the pointee's virtual Delete (slot 1, +4) at count zero.
// Layout (PDB, agrees with retail): C3DMeshInfo::MeshData +0xc (CIVCountedPointer<CMeshData>); CMeshData LODMeshes[4] +8
// (CIVCountedPointer<C3DMeshLODInfo>), MeshStats +0x18; C3DMeshLODInfo IVRefCount +4, UsedMeshStats (bool) +0x31.
// Shape: the intrusive pointer's inline `= 0` (release, clear, store) and `= other` (skip when equal, release, clear, store,
// add-ref with no null test: the LOD was dereferenced already); a local copy of the LOD keeps it alive across the reset.
// Leaf: RemoveLOD@C3DMeshInfo
struct C3DMeshLODInfo {
    virtual void Destroy(int flags);
    virtual void Delete();                          // slot 1 (+4)
    unsigned long IVRefCount;                       // +4
    unsigned char _08[0x29];
    bool UsedMeshStats;                             // +0x31
    void AddRef() { ++IVRefCount; }
    void Release() { if (--IVRefCount == 0) Delete(); }
    void ConvertToMeshStats();                      // 0x00A24870
};

template<class T> struct CIVCountedPointer {
    T* m_pObject;
    CIVCountedPointer(const CIVCountedPointer& other) : m_pObject(other.m_pObject) { if (m_pObject) m_pObject->AddRef(); }
    ~CIVCountedPointer() { if (m_pObject) m_pObject->Release(); }
    T* operator->() const { return m_pObject; }
    __forceinline CIVCountedPointer& operator=(T* object)
    {
        if (m_pObject) {
            m_pObject->Release();
            m_pObject = 0;
        }
        m_pObject = object;
        return *this;
    }
    __forceinline CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (m_pObject != other.m_pObject) {
            if (m_pObject) {
                m_pObject->Release();
                m_pObject = 0;
            }
            m_pObject = other.m_pObject;
            if (m_pObject)
                m_pObject->AddRef();
        }
        return *this;
    }
};

struct CMeshData {
    virtual void Destroy(int flags);
    unsigned long IVRefCount;                       // +4
    CIVCountedPointer<C3DMeshLODInfo> LODMeshes[4]; // +8
    CIVCountedPointer<C3DMeshLODInfo> MeshStats;    // +0x18
    unsigned long MeshID;                           // +0x1c
};

struct C3DMeshInfo {
    virtual void v00();
    long m_04, m_08;
    CMeshData* MeshData;                            // +0xc CIVCountedPointer<CMeshData>
    void RemoveLOD(unsigned long lod, bool keepAsMeshStats) const;
};

void C3DMeshInfo::RemoveLOD(unsigned long lod, bool keepAsMeshStats) const
{
    if (lod == 4) {
        MeshData->MeshStats = 0;
        return;
    }
    CIVCountedPointer<C3DMeshLODInfo> info = MeshData->LODMeshes[lod];
    MeshData->LODMeshes[lod] = 0;
    if (keepAsMeshStats && info->UsedMeshStats && !MeshData->MeshStats.m_pObject) {
        MeshData->MeshStats = info;
        info->ConvertToMeshStats();
    }
}