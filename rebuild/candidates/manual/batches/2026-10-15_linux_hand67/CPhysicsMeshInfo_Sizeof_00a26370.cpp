// CPhysicsMeshInfo::Sizeof() const -- virtual slot 2: 0xC0 plus the counted mesh-data pointer's footprint (4, plus the inlined CMeshData::Sizeof when present: 0x2C + the physics mesh's and the file data's footprints)
// Leaf: Sizeof
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A26370, length 85 bytes. Mangled
// `?Sizeof@CPhysicsMeshInfo@@UBEKXZ` (Ego_r.pdb); vtable 0x0129CD50 slot 2.
// Callees: CPhysicsMesh::Sizeof @ 0x00A6C900 (on CMeshData::Mesh, +0x24), CBankFileAsyncData::Sizeof
// @ 0x009D5620 (on CMeshData::FileData, +0x1C; the manifest label CVertexBufferWin32::DoSizeof is
// propagated, see the landed C3DMeshLODInfo::Sizeof 0x00A42AF0).
// Shape: the C3DAnimationInfo::Sizeof form (0x00A265B0): `(p ? p->Sizeof() + sizeof(p) : sizeof(p))
// + 0xC0`, VC folding the constant into the call arm and keeping `mov eax,4 ; add eax,0C0h` on the
// null arm; CPhysicsMeshInfo::CMeshData::Sizeof (Ego_r, `QBEKXZ`) is inlined: sizeof(CMeshData) =
// 0x2C plus the CIVCountedPointer<CPhysicsMesh> (4 + Sizeof, kept in edi) and the
// CCountedPointer<CBankFileAsyncData> (8 + Sizeof) footprints, folded into one
// `lea eax,[eax+edi+0F0h]` per arm. Layout: CPhysicsMeshInfo::MeshData +0xC (PDB); CMeshData is
// 0x2C bytes with FileData +0x1C, Mesh +0x24, Owner +0x28 (PDB).
struct CPhysicsMesh       { long Sizeof() const; };            // 0x00A6C900
struct CBankFileAsyncData { unsigned long Sizeof() const; };   // 0x009D5620

struct CMeshData_a26370 {                          // CPhysicsMeshInfo::CMeshData (0x2C bytes)
    unsigned char       _base[0x1c];               // the resource base (vptr, state, ...)
    CBankFileAsyncData* FileData_Data;             // +0x1c CCountedPointer<CBankFileAsyncData>
    void*               FileData_Info;             // +0x20
    CPhysicsMesh*       Mesh;                      // +0x24 CIVCountedPointer<CPhysicsMesh>
    void*               Owner;                     // +0x28
    unsigned long Sizeof() const
    {
        unsigned long meshSize = Mesh ? Mesh->Sizeof() + sizeof(Mesh) : sizeof(Mesh);
        unsigned long fileSize = FileData_Data ? FileData_Data->Sizeof() + 8 : 8;
        return sizeof(CMeshData_a26370) + meshSize + fileSize;
    }
};

struct CPhysicsMeshInfo {
    virtual void v00();
    virtual void v01();
    virtual unsigned long Sizeof() const;          // slot 2
    void*             Bank;                        // +0x04
    unsigned long     Index;                       // +0x08
    CMeshData_a26370* MeshData;                    // +0x0c CIVCountedPointer<CMeshData>
};

unsigned long CPhysicsMeshInfo::Sizeof() const
{
    return (MeshData ? MeshData->Sizeof() + sizeof(MeshData) : sizeof(MeshData)) + 0xc0;
}
