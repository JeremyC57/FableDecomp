// CEngineClothSimulationMeshBase::ClearParticleSims() -- before dropping the cloth's particle simulations, park every simulation whose render data is still busy in the global pending-release vector, then clear ParticleSims
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B81D60, length 148 bytes (+ int3 padding).
// Callees: CParticleSimulationRenderData virtual slot 1 (+0x04, IsBusy -- cf. the landed CEngineParticleSimulationRenderData::IsBusy 0x00BB6420),
// the global vector's _M_insert_overflow(pos, x, __false_type, n, atEnd) @ 0x00B83970 (thiscall on the vector @ 0x014378B8), ParticleSims.clear()
// @ 0x00B83D30 (thiscall, out of line). Globals: the vector of CCountedPointer<CParticleSimulation> @ 0x014378B8 (start / finish +4 / end of
// storage +8); its name is descriptive.
// Layout: CEngineClothSimulationMeshBase::ParticleSims (CArray<CCountedPointer<CParticleSimulation>>, a 12-byte STLport vector in retail)
// +0x0c; CParticleSimulation::RenderData (CCountedPointer<CParticleSimulationRenderData>) object word at +4.
// Shape: an index loop over the re-computed unsigned size (`jbe` / `jb`), the element reached through `*(begin() + i)` (its address kept in
// esi for the push_back argument); STLport push_back inlined: `finish != end_of_storage` -> placement copy (null-checked `new (finish)`,
// the counted-pointer copy constructor bumps the count under a null test of the copied block) and `++finish` re-read from the global;
// else _M_insert_overflow with the empty __false_type tag homed in the frame word's high byte (`lea edx,[esp+17h]`), n = 1, atEnd = true.
inline void* operator new(unsigned int, void* where) { return where; }

struct SCountBlock {
    long m_references;
};

template <class T>
struct CCountedPointer {
    T*           Object;
    SCountBlock* Count;
    CCountedPointer(const CCountedPointer& other) : Object(other.Object), Count(other.Count)
    {
        if (Count)
            ++Count->m_references;
    }
    T* operator->() const { return Object; }
};

struct CParticleSimulationRenderData {
    virtual void v00();
    virtual bool IsBusy();                   // slot 1 (+0x04)
};

struct CParticleSimulation {
    void*                                          __vftable;    // +0x00
    CCountedPointer<CParticleSimulationRenderData> RenderData;   // +0x04
};

struct __false_type { __false_type() {} };

template <class T>
struct CRetailVector {                       // STLport vector: start, finish, end of storage
    T* m_start;
    T* m_finish;
    T* m_endOfStorage;
    T* begin() const { return m_start; }
    unsigned long size() const { return m_finish - m_start; }
    T& operator[](unsigned long n) { return *(begin() + n); }
    void InsertOverflow(T* position, const T& x, const __false_type& tag, unsigned long count, bool atEnd);   // 0x00B83970
    void clear();                            // 0x00B83D30
    void push_back(const T& x)
    {
        if (m_finish != m_endOfStorage) {
            new (m_finish) T(x);
            ++m_finish;
        } else {
            InsertOverflow(m_finish, x, __false_type(), 1, true);
        }
    }
};

extern CRetailVector<CCountedPointer<CParticleSimulation> > g_PendingReleaseParticleSims;   // 0x014378B8

struct CEngineClothSimulationMeshBase {
    void*         __vftable;                 // +0x00
    unsigned char _pad04[0x08];
    CRetailVector<CCountedPointer<CParticleSimulation> > ParticleSims;   // +0x0c
    void ClearParticleSims();
};

void CEngineClothSimulationMeshBase::ClearParticleSims()
{
    for (unsigned long i = 0; i < ParticleSims.size(); ++i) {
        CCountedPointer<CParticleSimulation>& sim = ParticleSims[i];
        if (sim.Object && sim->RenderData.Object && sim->RenderData->IsBusy())
            g_PendingReleaseParticleSims.push_back(sim);
    }
    ParticleSims.clear();
}
