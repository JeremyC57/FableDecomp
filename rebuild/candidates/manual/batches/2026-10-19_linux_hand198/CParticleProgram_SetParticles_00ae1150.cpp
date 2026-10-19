// CParticleProgram::SetParticles(const vector<C3DVector>&, CMovableResourceMemoryPool*) -- size the program's non-simulated particle array to the given points, allocate it from the pool and fill it with the points as (x, y, z, 0) vectors; false when the allocation fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AE1150, length 192 bytes (`ret 8`). The manifest's class
// "CParticleSimulation" is propagated: the fields used (+0xbc count, +0xc0 array, and the allocator's +0x138/+0x13c
// handle and pool) are CParticleProgram's NonSimCount / NonSimPositions / MemoryHandle / MemoryPool at their retail
// offsets (PDB +0xcc/+0xd0/+0x168/+0x16c less the 16 bytes of narrower containers).
// Callees: CParticleArrayOwner::AllocateParticles(unsigned long, CMovableResourceMemoryPool*) @ 0x00ADF460 (the landed
//          model of this allocator; the program derives from it here so the call binds to the same symbol).
// Shape: the count is the signed 12-byte element difference stored straight into the member; each point is built as a
// C4DVector temporary (W = 0.0f, hoisted out of the loop as the zero register) and copied as four dwords.
struct C3DVector { float X, Y, Z; };
struct C4DVector {
    float X, Y, Z, W;
    C4DVector(const C3DVector& v, float w) : X(v.X), Y(v.Y), Z(v.Z), W(w) {}
};

struct CMovableResource {
    virtual void v00();
};
struct CMovableResourceMemoryPool;

struct CParticleArrayOwner : CMovableResource {
    unsigned char               _pad04[0xb8];
    unsigned long               NonSimCount;         // +0xbc
    C4DVector*                  m_pParticles;        // +0xc0
    unsigned char               _padC4[0x74];
    long                        m_memoryHandle;      // +0x138
    CMovableResourceMemoryPool* m_pMemoryPool;       // +0x13c
    bool AllocateParticles(unsigned long count, CMovableResourceMemoryPool* pool);   // 0x00ADF460
};

template<class T> struct CRetailVector {
    T* m_first;
    T* m_last;
    T* m_end;
    unsigned long size() const { return m_last - m_first; }
    const T& operator[](unsigned long i) const { return m_first[i]; }
};

struct CParticleProgram : CParticleArrayOwner {
    bool SetParticles(const CRetailVector<C3DVector>& points, CMovableResourceMemoryPool* pool);
};

bool CParticleProgram::SetParticles(const CRetailVector<C3DVector>& points, CMovableResourceMemoryPool* pool)
{
    NonSimCount = points.size();
    if (!AllocateParticles(NonSimCount, pool))
        return false;
    for (unsigned long i = 0; i < NonSimCount; ++i)
        m_pParticles[i] = C4DVector(points[i], 0.0f);
    return true;
}
