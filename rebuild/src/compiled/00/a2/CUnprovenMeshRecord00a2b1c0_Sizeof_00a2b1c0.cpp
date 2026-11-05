// CUnprovenMeshRecord_00a2b1c0::Sizeof() const -- this record's size plus the Mesh and FileData pointees' sizes and their pointer objects
// Leaf: ?Sizeof@CUnprovenMeshRecord_00a2b1c0@@QBEKXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00a2b1c0, 62 bytes, thiscall, `ret`.
// Identity (descriptive): a byte twin of 0x00A244E0 (C3DMeshLODInfo's layout: a CCountedPointer<C3DMesh2> at +0x1C, a CCountedPointer<CBankFileAsyncData> at +0x24, sizeof 0x34) in another TU; it cannot be the same symbol as 0x00A244E0 (one name per address) and has no caller or reference, so its owner is unproven.
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
#include "engine/CCountedPointer.h"
template <class T> class CIVCountedPointer {               // intrusive, 4 bytes
public:
    T* Data;                                               // +0x00
};
class CUnprovenMeshRecord_00a2b1c0 {
public:
    unsigned char _pad00[0x1C];
    CCountedPointer<C3DMesh2> Mesh;   // +0x1C
    CCountedPointer<CBankFileAsyncData> FileData;   // +0x24
    C3DMeshInfo*  MeshInfo;                                // +0x2C
    unsigned char LOD;                                     // +0x30
    bool          UsedMeshStats;                           // +0x31
    unsigned long Sizeof() const;
};

unsigned long CUnprovenMeshRecord_00a2b1c0::Sizeof() const
{
    unsigned long size1 = Mesh.Data ? Mesh.Data->Sizeof() + sizeof(Mesh) : sizeof(Mesh);
    return (FileData.Data ? FileData.Data->Sizeof() + sizeof(FileData) : sizeof(FileData)) + size1 + sizeof(*this);
}