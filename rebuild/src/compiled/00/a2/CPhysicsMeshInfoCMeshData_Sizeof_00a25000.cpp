// CPhysicsMeshInfo::CMeshData::Sizeof() const -- this record's size plus the Mesh and FileData pointees' sizes and their pointer objects
// Leaf: ?Sizeof@CMeshData@CPhysicsMeshInfo@@QBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00a25000, 62 bytes, thiscall, `ret`.
// Identity (probable): Ego_r.pdb ?Sizeof@CMeshData@CPhysicsMeshInfo@@QBEKXZ over CPhysicsMeshInfo::CMeshData's Mesh (CIVCountedPointer<CPhysicsMesh>, +0x24, CPhysicsMesh::Sizeof 0x00A6C900) and FileData (CCountedPointer<CBankFileAsyncData>, +0x1C), sizeof 0x2C (Ego_r.pdb 44 bytes); no caller or reference; a byte twin follows at 0x00A2B200.
// Callees: CPhysicsMesh::Sizeof() const 0x00A6C900, CBankFileAsyncData::Sizeof() const 0x009D5620 (Ego_r.pdb).
// Body: `sizeof(*this) + (Mesh ? Mesh->Sizeof() + sizeof(Mesh) : sizeof(Mesh)) + (FileData ? ... : sizeof(FileData))`.
class CPhysicsMeshInfo;
class CPhysicsMesh {
public:
    long Sizeof() const;                                 // 0x00A6C900
};
class CBankFileAsyncData {
public:
    unsigned long Sizeof() const;                                 // 0x009D5620
};
template <class T> class CCountedPointer {                 // 8 bytes
public:
    T*    Data;                                            // +0x00
    void* Info;                                            // +0x04
};
template <class T> class CIVCountedPointer {               // intrusive, 4 bytes
public:
    T* Data;                                               // +0x00
};
class CPhysicsMeshInfo {
public:
    class CMeshData {
    public:
        unsigned char _pad00[0x1C];
        CCountedPointer<CBankFileAsyncData> FileData;   // +0x1C
        CIVCountedPointer<CPhysicsMesh> Mesh;   // +0x24
        CPhysicsMeshInfo* Owner;                               // +0x28
        unsigned long Sizeof() const;
    };
};

unsigned long CPhysicsMeshInfo::CMeshData::Sizeof() const
{
    unsigned long size1 = Mesh.Data ? Mesh.Data->Sizeof() + sizeof(Mesh) : sizeof(Mesh);
    return (FileData.Data ? FileData.Data->Sizeof() + sizeof(FileData) : sizeof(FileData)) + size1 + sizeof(*this);
}