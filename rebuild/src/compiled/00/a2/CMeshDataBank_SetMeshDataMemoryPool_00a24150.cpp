// CMeshDataBank::SetMeshDataMemoryPool(CCountedPointer<CMovableResourceMemoryPool>, bool) -- adopt a movable-resource memory pool for the MeshResourceBank and optionally point the pool back at that bank
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A24150, 144 bytes (padding included).
// Leaf: SetMeshDataMemoryPool
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pCount->Info are the header CCountedPointer's (same offsets); the local SCountBlock (= CCPPointerInfo) members m_references->RefCount, m_pfnDestroy->DeleteFunc, m_pObject->Data are the header CCPPointerInfo's (same offsets).
// Label correction: the catalog calls this SetAnimationMemoryPool (byte-twin propagation); it
// writes MeshDataMemoryPool (+0x370) and attaches MeshResourceBank (+0x170), so it is the
// mesh-data twin (PDB order AnimationMemoryPool, ClothMemoryPool, MeshDataMemoryPool).
// Shape: the counted pointer arrives BY VALUE (object, block in the argument slots). The member
// MeshDataMemoryPool (+0x370) is assigned with CCountedPointer::operator=: when the blocks differ the old block
// is released (count re-read after the decrement; at zero the __fastcall deleter at +4 runs on the
// object at +8 and the block is freed by operator delete 0x00BFE9BC), then object and block are copied
// and the new block's count incremented. With `attach` the pool's owner slot (+0x34) gets the
// MeshResourceBank's notification interface (bank +4, i.e. +0x174). The by-value parameter's destructor
// (`dec [edi] ; jne`, the block in a register) ends the function. Retail offsets are the PDB's minus
// 0x74 (MeshDataMemoryPool PDB 0x3e4, MeshResourceBank PDB 0x1e4).
class CMovableResourceMemoryPool;

#include "engine/CCountedPointer.h"


struct CResourceBankOwner { void* m_pFirst; };    // the pool-facing interface at bank +4

class CResourceBank { public:                              // 0x7c bytes in retail
    void*              __vftable;                   // +0
    CResourceBankOwner Owner;                       // +4
    unsigned char      _rest[0x7c - 8];
};

class CMovableResourceMemoryPool { public:
    unsigned char       _pad[0x34];
    CResourceBankOwner* POwner;                     // +0x34
};

class CMeshDataBank { public:
    unsigned char _pad000[0x170];
    CResourceBank MeshResourceBank;                 // +0x170
    CResourceBank MeshStatsResourceBank;            // +0x1ec
    CResourceBank AnimationResourceBank;            // +0x268
    CResourceBank PhysicsMeshResourceBank;          // +0x2e4
    CCountedPointer<CMovableResourceMemoryPool> AnimationMemoryPool;   // +0x360
    CCountedPointer<CMovableResourceMemoryPool> ClothMemoryPool;       // +0x368
    CCountedPointer<CMovableResourceMemoryPool> MeshDataMemoryPool;    // +0x370
    void SetMeshDataMemoryPool(CCountedPointer<CMovableResourceMemoryPool> pool, bool attach);
};

void CMeshDataBank::SetMeshDataMemoryPool(CCountedPointer<CMovableResourceMemoryPool> pool, bool attach)
{
    MeshDataMemoryPool = pool;
    if (attach)
        MeshDataMemoryPool->POwner = &MeshResourceBank.Owner;
}