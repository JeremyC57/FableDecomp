// C3DMeshLODInfo::C3DMeshLODInfo(C3DMeshInfo* meshInfo, unsigned char lod) -- CResource base, this class's vtable, two empty counted pointers, owner and LOD index stored, UsedMeshStats false
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A24430, length 64 bytes.
// Callees: 0x009FBEC0 CResource::CResource()
// Local model (the engine header is a flat PDB layout and cannot express the base constructor call); the
// offsets are the header's. The compiler groups the zero stores (four pointers and the bool) ahead of the
// argument stores.
//
#include "rebuild_abi.h"

struct CResource {
    virtual void v0();
    CResource();                                   // 0x009FBEC0
    unsigned char _body[0x18];                     // ResourceList, PrevResource, NextResource, ResourceSize, LastUsedFrame (sizeof 0x1c)
};
struct CCPPointerInfo;
template<class T> struct CCountedPointer {
    T* m_pData; CCPPointerInfo* m_pInfo;
    CCountedPointer() : m_pData(0), m_pInfo(0) {}
};
struct C3DMesh2; struct CBankFileAsyncData; struct C3DMeshInfo;
struct C3DMeshLODInfo : CResource {
    CCountedPointer<C3DMesh2>           Mesh;            // +0x1c
    CCountedPointer<CBankFileAsyncData> FileData;        // +0x24
    C3DMeshInfo*                        MeshInfo;        // +0x2c
    unsigned char                       LOD;             // +0x30
    bool                                UsedMeshStats;   // +0x31   sizeof == 0x34
    C3DMeshLODInfo(C3DMeshInfo* meshInfo, unsigned char lod);
};
FABLE_STATIC_ASSERT(sizeof(C3DMeshLODInfo) == 0x34);
C3DMeshLODInfo::C3DMeshLODInfo(C3DMeshInfo* meshInfo, unsigned char lod)
    : MeshInfo(meshInfo), LOD(lod), UsedMeshStats(false)
{
}