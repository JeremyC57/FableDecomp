// CPhysicsMeshInfo::CMeshData::UnloadResource() -- when the resource reference at +0x24 is held only once, release the owning CPhysicsMeshInfo's counted pointer to this mesh data and report success
// Leaf: UnloadResource
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A24FC0, 58 bytes (+6 int3 padding); virtual slot 2 of vtable 0x0129CD78 (slot 0 the
//   ??_G 0x00A2B610, slot 4 IsResourceLoaded 0x00A23FF0). FableWin.pdb names the slot: ?UnloadResource@CMeshData@CPhysicsMeshInfo@@UAE_NXZ.
// No direct callees: the pointee is destroyed through its vtable slot 1 (`call [eax+4]`).
// Layout: Ego_r.pdb CPhysicsMeshInfo (16 bytes) holds `CIVCountedPointer<CPhysicsMeshInfo::CMeshData> MeshData` at +0xC; the
//   mesh data keeps its owner at +0x28. CIVCountedPointeeBase = vptr + reference count (+4). The member at +0x24 is a
//   second counted pointer whose pointee type is not recoverable from this body (descriptive name).
// Shape: the guard is the pointer's inline count accessor `Data ? Data->RefCount : 0` compared with 1 (the null arm
//   folds into the false exit, the count is loaded before `cmp eax,1`; a plain `p && p->RefCount == 1` gives
//   `cmp dword ptr [eax+4],1`). The owner's MeshData address is formed once (`add esi,0Ch`); CIVCountedPointer::Release
//   reads Data into a local (kept in ecx across the `dec`) and is `if (data) { if (--data->RefCount == 0)
//   data->Destroy(); Data = 0; } Data = 0;` (retail keeps both zero stores).
class CIVCountedPointeeBase { public:
    virtual ~CIVCountedPointeeBase();
    virtual void Destroy();                               // slot 1
    long RefCount;                                        // +0x04
};

template <class T> class CIVCountedPointer { public:
    T* Data;
    long GetRefCount() const { return Data ? Data->RefCount : 0; }
    void Release()
    {
        T* data = Data;
        if (data) {
            if (--data->RefCount == 0)
                data->Destroy();
            Data = 0;
        }
        Data = 0;
    }
};

class CPhysicsMeshInfo {
public:
    class CMeshData;
    unsigned char _pad00[0xC];
    CIVCountedPointer<CIVCountedPointeeBase> MeshData;    // +0x0C (CIVCountedPointer<CMeshData>)
};

class CPhysicsMeshInfo::CMeshData {
public:
    virtual ~CMeshData();
    virtual void v01();
    virtual bool UnloadResource();                        // slot 2
    unsigned char          _pad04[0x20];
    CIVCountedPointer<CIVCountedPointeeBase> m_resource;  // +0x24
    CPhysicsMeshInfo*      m_pOwner;                      // +0x28
};

bool CPhysicsMeshInfo::CMeshData::UnloadResource()
{
    if (m_resource.GetRefCount() == 1) {
        m_pOwner->MeshData.Release();
        return true;
    }
    return false;
}