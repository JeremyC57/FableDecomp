// C3DMeshLODInfo::Sizeof() const -- sizeof(C3DMeshLODInfo) plus the two counted pointers' footprints (8 bytes of block each, plus the pointee's Sizeof when present)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A42AF0, length 62 bytes.
// Callees: 0x00A43960 C3DMesh2::Sizeof, 0x009D5620 CBankFileAsyncData::Sizeof (InternalAllocatedBufferSize + 0x48)
// The manifest's callee labels (CLipSyncSampleData::Sizeof, CVertexBufferWin32::DoSizeof) are propagated: the
// pointees are the LOD's C3DMesh2 (+0x1c) and CBankFileAsyncData (+0x24).
//
#include "engine/C3DMeshLODInfo.h"   // PDB layout agrees with retail (Mesh +0x1c, FileData +0x24, MeshInfo +0x2c, LOD +0x30)

struct CCPPointerInfo {                                  // PDB: RefCount +0, DeleteFunc +4, Data +8
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
struct C3DMeshInfo {
    void RemoveLOD(unsigned long lod, bool keepStats);   // 0x00A249A0
};

struct C3DMesh2           { unsigned long Sizeof_00a43960() const; };   // 0x00A43960
struct CBankFileAsyncData { unsigned long Sizeof() const; };   // 0x009D5620
struct C3DMeshLODInfo_Methods : C3DMeshLODInfo {
    unsigned long Sizeof() const;
};
unsigned long C3DMeshLODInfo_Methods::Sizeof() const
{
    unsigned long meshSize = Mesh_Data ? Mesh_Data->Sizeof_00a43960() + 8 : 8;
    unsigned long fileSize = FileData_Data ? FileData_Data->Sizeof() + 8 : 8;
    return sizeof(C3DMeshLODInfo) + meshSize + fileSize;
}