// CPhysicsMeshInfo::CMeshData::OnResourceLoaded() -- builds the physics mesh once its source data is present
// Retail: Fable.exe 0x00A25E60, 13 bytes: `mov eax,[ecx+1Ch] ; test eax,eax ; je ; jmp BuildMesh ; ret`.
// Identity: vtable 0x0129CD78 (RTTI .?AVCMeshData@CPhysicsMeshInfo@@, bases CResource / CIVCountedPointeeBase) slot 5.
// CMeshData overrides exactly three CResource virtuals besides its destructor (Ego_r.pdb: UnloadResource,
// IsResourceLoaded, OnResourceLoaded) and its vtable has exactly three own entries besides slot 0: slot 2 0x00A24FC0
// (releases data, the UnloadResource body), slot 4 0x00A23FF0 (returns a bool: IsResourceLoaded) and this void slot 5,
// whose tail call to ?BuildMesh@CMeshData@CPhysicsMeshInfo@@QAEXXZ (0x00A25D20) makes it OnResourceLoaded.
// Layout: CResource is 0x1C bytes (engine/CResource.h); CMeshData's first own member at +0x1C is the source-data
// pointer BuildMesh reads (name not in the PDB layouts: descriptive). engine/CResource.h flattens the vptr, so a
// local polymorphic base model is used (Header debt in NOTES.md).
// Leaf: ?OnResourceLoaded@CMeshData@CPhysicsMeshInfo@@UAEXXZ
class CResourceList;

class CResource {
public:
    virtual ~CResource();
    unsigned char  _pad_0x04[0x4];   // +0x04
    CResourceList* ResourceList;     // +0x08
    CResource*     PrevResource;     // +0x0C
    CResource*     NextResource;     // +0x10
    unsigned long  ResourceSize;     // +0x14
    unsigned long  LastUsedFrame;    // +0x18
};

class CPhysicsMeshInfo {
public:
    class CMeshData : public CResource {
    public:
        virtual void OnResourceLoaded();
        void BuildMesh();            // 0x00A25D20
        void* m_pSourceData;         // +0x1C (descriptive)
    };
};

void CPhysicsMeshInfo::CMeshData::OnResourceLoaded()
{
    if (m_pSourceData)
        BuildMesh();
}