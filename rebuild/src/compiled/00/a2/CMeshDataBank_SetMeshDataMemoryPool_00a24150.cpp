// CMeshDataBank::SetMeshDataMemoryPool(CCountedPointer<CMovableResourceMemoryPool>, bool) -- adopt a movable-resource memory pool for the MeshResourceBank and optionally point the pool back at that bank
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A24150, 144 bytes (padding included).
// Leaf: SetMeshDataMemoryPool
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
struct CMovableResourceMemoryPool;

struct SCountBlock {
    long m_references;                              // +0
    void (__fastcall* m_pfnDestroy)(void*);         // +4
    void* m_pObject;                                // +8
};

template <class T> struct CCountedPointer {
    T*           Data;                              // +0
    SCountBlock* m_pCount;                          // +4
    ~CCountedPointer()
    {
        SCountBlock* count = m_pCount;
        if (count) {
            if (--count->m_references == 0) {
                count->m_pfnDestroy(count->m_pObject);
                delete count;
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        if (m_pCount != other.m_pCount) {
            if (m_pCount) {
                --m_pCount->m_references;
                if (m_pCount->m_references == 0) {
                    m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                    delete m_pCount;
                }
            }
            Data = other.Data;
            m_pCount = other.m_pCount;
            if (m_pCount)
                ++m_pCount->m_references;
        }
        return *this;
    }
    T* operator->() const { return Data; }
};

struct CResourceBankOwner { void* m_pFirst; };    // the pool-facing interface at bank +4

struct CResourceBank {                              // 0x7c bytes in retail
    void*              __vftable;                   // +0
    CResourceBankOwner Owner;                       // +4
    unsigned char      _rest[0x7c - 8];
};

struct CMovableResourceMemoryPool {
    unsigned char       _pad[0x34];
    CResourceBankOwner* POwner;                     // +0x34
};

struct CMeshDataBank {
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