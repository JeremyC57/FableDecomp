// C3DMeshLODInfo::~C3DMeshLODInfo() -- mesh LOD resource destructor: restore the C3DMeshLODInfo vtable, release the FileData then the Mesh counted pointers (member order reversed), then the CResource base
// Leaf: ??1C3DMeshLODInfo@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A24470, 101 bytes, thiscall, tail `jmp ??1CResource@@UAE@XZ` (0x009FC550).
// Identity (proven): stores vtable 0x0129CDA4 first (RTTI .?AVC3DMeshLODInfo@@ : CResource : CIVCountedPointeeBase); sole
//   caller is that vtable's slot-0 scalar deleting destructor 0x00A2B690 (call at 0x00A2B693); Ego_r.pdb ??1C3DMeshLODInfo@@UAE@XZ.
//   The catalog label is right for this address; the other row carrying it (0x00A0B9A0) is ~CSoundBankLH.
// Callees: the CCPPointerInfo deleter (fastcall, Data in ecx), operator delete 0x00BFE9BC (count blocks), ~CResource 0x009FC550.
// Layout: Ego_r C3DMeshLODInfo over the retail CResource base (engine/CResource.h, FABLE_RETAIL_DEF_CLASS_BASE; engine/C3DMeshLODInfo.h
//   is the flattened donor layout with a `void* __vftable` word): Mesh +0x1C (CCountedPointer<C3DMesh2>), FileData +0x24
//   (CCountedPointer<CBankFileAsyncData>), MeshInfo +0x2C, LOD +0x30, UsedMeshStats +0x31.
// Shape: a user-declared empty virtual destructor (the vtable store at entry); the two counted pointers' destructors
//   (engine/CCountedPointer.h: `dec [Info] ; reload ; cmp ; call DeleteFunc ; delete Info`, then Data = Info = 0) in reverse
//   member order share one zero register (edi).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CResource.h"
#include "engine/CCountedPointer.h"
class C3DMesh2;
class C3DMeshInfo;
class CBankFileAsyncData;
class C3DMeshLODInfo : public CResource {
public:
    virtual ~C3DMeshLODInfo();
    CCountedPointer<C3DMesh2>           Mesh;               // +0x1c
    CCountedPointer<CBankFileAsyncData> FileData;           // +0x24
    C3DMeshInfo*                        MeshInfo;           // +0x2c
    unsigned char                       LOD;                // +0x30
    bool                                UsedMeshStats;      // +0x31
};
C3DMeshLODInfo::~C3DMeshLODInfo()
{
}