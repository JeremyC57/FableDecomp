// CPhysicsMeshInfo::CMeshData::~CMeshData() -- implicit physics-mesh data destructor: release Mesh (intrusive count), release FileData (counted pointer), then ~CResource
// Leaf: ??1CMeshData@CPhysicsMeshInfo@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A2B630, 84 bytes (int3 padding follows), thiscall, tail `jmp ~CResource` 0x009FC550.
// Identity (proven, two sources): (1) the sole caller 0x00A2B613 is the scalar deleting destructor 0x00A2B610, slot 0 of vtable
//   0x0129CD78, RTTI .?AVCMeshData@CPhysicsMeshInfo@@ : CResource : CIVCountedPointeeBase; (2) Ego_r.pdb 0x00B66E60 / FableWin.pdb
//   0x03141980 ??1CMeshData@CPhysicsMeshInfo@@UAE@XZ, and the body walks Ego_r's CPhysicsMeshInfo::CMeshData members FileData
//   (CCountedPointer<CBankFileAsyncData>) +0x1C and Mesh (CIVCountedPointer<CPhysicsMesh>) +0x24. No 0x0129CD78 store: the destructor is
//   compiler-generated. The catalog label "CMeshData::~CMeshData" drops the nesting class.
// Callees: CIVCountedPointeeBase::Delete (virtual slot 1) of the mesh, the counted pointer's DeleteFunc, operator delete 0x00BFE9BC,
//   CResource::~CResource 0x009FC550 (tail).
// Shape: members in reverse order (Owner trivial, Mesh, FileData). The intrusive release is FableWin's ~CIVCountedPointer() -> Release()
//   (object copied into a local, zero store inside the null test; chunk 712); the counted release is engine/CCountedPointer.h's dtor
//   (zeroes Data then Info). The private PDB ctor (body not reconstructed) emits the vtable; DestroyOne (ungraded harness, chunks 656/657/
//   712) destroys the object by name so VC7.1 emits this out-of-line ??1, which the TU's ??_G then calls exactly as retail 0x00A2B610 does
//   (that ??_G grades RELOCATION_MATCH too).
// Layout: engine/CResource.h and engine/CIVCountedPointeeBase.h retail opt-ins (FABLE_RETAIL_DEF_CLASS_BASE); CIVCountedPointer has no
//   shared header (the chunk 712 local template, Ego_r member Object). Retail sizeof 0x2C (Ego_r 44, no STL member).
#include "rebuild_abi.h"
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CIVCountedPointeeBase.h"
#include "engine/CResource.h"

template <class T> class CIVCountedPointer {                // Ego_r.pdb: one CIVCountedPointeeBase* (4 bytes)
public:
    ~CIVCountedPointer() { Release(); }                     // FableWin.pdb ??1?$CIVCountedPointer@...@@QAE@XZ (inline here)
    void Release()                                          // FableWin.pdb ?Release@?$CIVCountedPointer@...@@QAEXXZ (inline here)
    {
        CIVCountedPointeeBase* object = Object;
        if (object) {
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
    CIVCountedPointeeBase* Object;                          // +0x00
};

class CBankFileAsyncData;
class CPhysicsMesh;
class CPhysicsMeshInfo {
public:
    class CMeshData : public CResource {                    // vtable 0x0129CD78
    public:
        CMeshData(const CPhysicsMeshInfo* owner);           // ??0CMeshData@CPhysicsMeshInfo@@QAE@PBV1@@Z (FableWin)
        virtual bool UnloadResource();                      // slot 2 ?UnloadResource@CMeshData@CPhysicsMeshInfo@@UAE_NXZ
        virtual bool IsResourceLoaded() const;              // slot 4 ?IsResourceLoaded@CMeshData@CPhysicsMeshInfo@@UBE_NXZ
        virtual void OnResourceLoaded();                    // slot 5 ?OnResourceLoaded@CMeshData@CPhysicsMeshInfo@@UAEXXZ
        CCountedPointer<CBankFileAsyncData> FileData;        // +0x1c
        CIVCountedPointer<CPhysicsMesh>     Mesh;            // +0x24
        CPhysicsMeshInfo*                   Owner;           // +0x28
    };
};
FABLE_STATIC_ASSERT(sizeof(CPhysicsMeshInfo::CMeshData) == 0x2c);

// Emission harness (not retail code, not graded): destroying the object by name makes VC7.1 emit the out-of-line implicit ??1.
void DestroyOne(CPhysicsMeshInfo::CMeshData* data)
{
    data->CPhysicsMeshInfo::CMeshData::~CMeshData();
}
CPhysicsMeshInfo::CMeshData::CMeshData(const CPhysicsMeshInfo* owner)
{
}