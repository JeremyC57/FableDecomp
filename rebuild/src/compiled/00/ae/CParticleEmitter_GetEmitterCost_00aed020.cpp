// CParticleEmitter::GetEmitterCost(CParticleSystemCost* cost) -- accumulates the cost of every enabled particle system into *cost
// Leaf: GetEmitterCost
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AED020, length 47 bytes (`ret 4`).
// Callees: CParticleSystem::GetSystemCost(CParticleSystemCost*) @ 0x00AF3DC0.
// Layout: ParticleSystems (retail 12-byte vector of 8-byte CCountedPointer<CParticleSystem>) at
// +0x08 -- engine/CParticleEmitter.h keeps it opaque and 16 bytes wide (PDB), so the emitter is
// modelled locally. CParticleSystem's bool bit-fields (PDB +108, retail +0x50: Initialised, Enabled,
// Dead, NormalisedTimeEnabled, UseAllAttractors, Visible, ...) put Enabled at bit 1.
// Shape: a top-tested walk over [first, last) with `last` re-read after every call; the cost
// argument is loaded into ebp only on the non-empty path (shrink-wrapped push), the system pointer
// goes straight into ecx for the flag test and the call.

struct CParticleSystemCost;

struct CParticleSystem {
    unsigned char _state[0x50];
    bool          Initialised           : 1;                              // +0x50 bit 0
    bool          Enabled               : 1;                              // +0x50 bit 1
    bool          Dead                  : 1;                              // +0x50 bit 2
    bool          NormalisedTimeEnabled : 1;                              // +0x50 bit 3
    bool          UseAllAttractors      : 1;                              // +0x50 bit 4
    bool          Visible               : 1;                              // +0x50 bit 5
    bool          StayWithEmitter       : 1;                              // +0x50 bit 6
    bool          ScaleParticles        : 1;                              // +0x50 bit 7
    void GetSystemCost(CParticleSystemCost* cost);                        // 0x00AF3DC0
};

struct CParticleSystemPtr {                                               // CCountedPointer<CParticleSystem> (8 bytes)
    CParticleSystem* m_pObject;
    void*            m_pCount;
};

struct CParticleSystemPtrVector {                                         // retail vector<CCountedPointer<CParticleSystem> > (12 bytes)
    CParticleSystemPtr* m_first;
    CParticleSystemPtr* m_last;
    CParticleSystemPtr* m_end;
};

struct CParticleEmitter {
    virtual void v00();
    unsigned char            _base4[4];                                   // +0x04
    CParticleSystemPtrVector ParticleSystems;                             // +0x08
    void GetEmitterCost(CParticleSystemCost* cost);
};

void CParticleEmitter::GetEmitterCost(CParticleSystemCost* cost)
{
    for (CParticleSystemPtr* it = ParticleSystems.m_first; it != ParticleSystems.m_last; ++it) {
        CParticleSystem* system = it->m_pObject;
        if (system->Enabled)
            system->GetSystemCost(cost);
    }
}