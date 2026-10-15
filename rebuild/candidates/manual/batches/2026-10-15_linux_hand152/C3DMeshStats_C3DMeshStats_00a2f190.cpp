// C3DMeshStats::~C3DMeshStats() -- return the stats block to its handle-based memory pool, destroy and free the generator objects and the mesh-volume map, destroy the name, then the inlined CMovableResource destructor's vtable store
// Leaf: ??1C3DMeshStats
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2F190, length 121 bytes (+ int3 padding).
// Label correction: catalogued as CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip (propagated); called by the
// scalar deleting destructor @ 0x00A353F0, slot 0 of C3DMeshStats's vtable 0x0129CF04 (the vtable stored first).
// Callees: CHandleBasedMemoryPool::Free(long) @ 0x00A62ED0, CCharString::~CCharString @ 0x0099EAE0 (per generator, then
// Name), free @ 0x00BFEA14 (x2), _Destroy_range of the mesh-volume pairs @ 0x00A33720 (`__fastcall` first/last, the
// iterator tag by reference at `lea [esp+7]`).
// Layout: Ego_r.pdb C3DMeshStats (PDB 132 bytes, retail 0x7c with 12-byte containers): MemoryPool +0x04,
// StatsMemoryHandle +0x08, Name +0x0c, BoundingSphere +0x10, BoundingBox +0x20, HelperPoints +0x38, DummyObjects +0x48
// (pre-allocated vector maps, trivially destroyed), MeshVolumes +0x58 (vector + compare), MeshGenerators +0x68
// (CArray<C3DGeneratorObject>, 0x40-byte elements whose only non-trivial member is ObjectName at +0x34), PackedNames +0x74.
// Shape: the body frees the handle unless it is -1; members then die in reverse order (the generator array's inline
// destroy loop (bounds passed by value, the end cached in ebx) and free, the volume vector's range destroy and free, the name), then the CMovableResource base's inline
// destructor stores its vtable 0x0129B860 and returns.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

struct CCharString {
    void* m_pData;
    ~CCharString();                                // 0x0099EAE0
};
struct CHandleBasedMemoryPool {
    void Free(long handle);                        // 0x00A62ED0
};
struct CMovableResource {                          // vtable 0x0129B860
    virtual void v00();
    ~CMovableResource() { }
};

struct SNonscalarPtrIteratorTag { SNonscalarPtrIteratorTag() { } };
struct SMeshVolumePair;                            // pair<unsigned long, C3DMeshVolume>
void __fastcall DestroyRange(SMeshVolumePair* first, SMeshVolumePair* last, const SNonscalarPtrIteratorTag& tag);   // 0x00A33720

struct C3DGeneratorObject {                        // 0x40 bytes
    unsigned char _pad[0x34];
    CCharString   ObjectName;                      // +0x34
    unsigned long BankIndex;                       // +0x38
    bool          UseLocalOrigin;                  // +0x3c
};
inline void DestroyGenerators(C3DGeneratorObject* first, C3DGeneratorObject* last)   // STLport _Destroy: bounds by value
{
    for (; first != last; ++first)
        first->~C3DGeneratorObject();
}
struct CGeneratorArray {                           // CArray<C3DGeneratorObject>: first / last / end
    C3DGeneratorObject* m_pFirst;
    C3DGeneratorObject* m_pLast;
    C3DGeneratorObject* m_pEnd;
    ~CGeneratorArray()
    {
        DestroyGenerators(m_pFirst, m_pLast);
        if (m_pFirst)
            free(m_pFirst);
    }
};
struct CMeshVolumeVector {
    SMeshVolumePair* _M_start;
    SMeshVolumePair* _M_finish;
    SMeshVolumePair* _M_end_of_storage;
    ~CMeshVolumeVector()
    {
        DestroyRange(_M_start, _M_finish, SNonscalarPtrIteratorTag());
        if (_M_start)
            free(_M_start);
    }
};
struct CMeshVolumeMap {                            // CVectorMap<unsigned long, C3DMeshVolume, ...>
    CMeshVolumeVector m_vector;
    long              m_compare;
};
struct CPreAllocVectorMap { void* m_p[4]; };       // pool-backed, nothing to destroy

struct C3DMeshStats : CMovableResource {           // vtable 0x0129CF04, retail 0x7c bytes
    CHandleBasedMemoryPool* MemoryPool;            // +0x04
    long                    StatsMemoryHandle;     // +0x08
    CCharString             Name;                  // +0x0c
    float                   BoundingSphere[4];     // +0x10
    float                   BoundingBox[6];        // +0x20
    CPreAllocVectorMap      HelperPoints;          // +0x38
    CPreAllocVectorMap      DummyObjects;          // +0x48
    CMeshVolumeMap          MeshVolumes;           // +0x58
    CGeneratorArray         MeshGenerators;        // +0x68
    char*                   PackedNames;           // +0x74
    unsigned short          PackedNamesSize;       // +0x78
    bool                    AnimatedFlag;          // +0x7a
    virtual void v00();
    ~C3DMeshStats();
};

C3DMeshStats::~C3DMeshStats()
{
    if (StatsMemoryHandle != -1)
        MemoryPool->Free(StatsMemoryHandle);
}
