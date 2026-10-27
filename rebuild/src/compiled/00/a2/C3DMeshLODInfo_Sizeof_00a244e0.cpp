// C3DMeshLODInfo::Sizeof() const -- this record's size plus the Mesh and FileData pointees' sizes and their pointer objects
// Leaf: ?Sizeof@C3DMeshLODInfo@@QBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00a244e0, 62 bytes, thiscall, `ret`.
// Identity (probable): Ego_r.pdb ?Sizeof@C3DMeshLODInfo@@QBEKXZ over C3DMeshLODInfo's Mesh (CCountedPointer<C3DMesh2>, +0x1C, C3DMesh2::Sizeof 0x00A893C0) and FileData (CCountedPointer<CBankFileAsyncData>, +0x24), sizeof 0x34 (Ego_r.pdb 52 bytes); no caller or reference (an unreferenced out-of-line copy); a byte twin follows at 0x00A2B1C0.
// Callees: C3DMesh2::Sizeof() const 0x00A893C0, CBankFileAsyncData::Sizeof() const 0x009D5620 (Ego_r.pdb).
// Body: `sizeof(*this) + (Mesh ? Mesh->Sizeof() + sizeof(Mesh) : sizeof(Mesh)) + (FileData ? ... : sizeof(FileData))`.
class C3DMeshInfo;
class C3DMesh2 {
public:
    unsigned long Sizeof() const;                                 // 0x00A893C0
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
class C3DMeshLODInfo {
public:
    unsigned char _pad00[0x1C];
    CCountedPointer<C3DMesh2> Mesh;   // +0x1C
    CCountedPointer<CBankFileAsyncData> FileData;   // +0x24
    C3DMeshInfo*  MeshInfo;                                // +0x2C
    unsigned char LOD;                                     // +0x30
    bool          UsedMeshStats;                           // +0x31
    unsigned long Sizeof() const;
};

unsigned long C3DMeshLODInfo::Sizeof() const
{
    unsigned long size1 = Mesh.Data ? Mesh.Data->Sizeof() + sizeof(Mesh) : sizeof(Mesh);
    return (FileData.Data ? FileData.Data->Sizeof() + sizeof(FileData) : sizeof(FileData)) + size1 + sizeof(*this);
}