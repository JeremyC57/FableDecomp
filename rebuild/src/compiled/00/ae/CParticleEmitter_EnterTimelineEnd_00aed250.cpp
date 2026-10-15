// CParticleEmitter::EnterTimelineEnd() -- tells every particle system that is present, enabled and not dead to enter its timeline end
// Leaf: EnterTimelineEnd
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AED250, length 51 bytes (`ret`).
// Callees: CParticleSystem::EnterTimelineEnd() @ 0x00AF4070.
// Layout: ParticleSystems (retail 12-byte vector of 8-byte CCountedPointer<CParticleSystem>) at
// +0x08 -- engine/CParticleEmitter.h keeps it opaque and 16 bytes wide (PDB), so the emitter is
// modelled locally. CParticleSystem's bool bit-fields (PDB +108, retail +0x50) give Enabled bit 1,
// Dead bit 2.
// Shape: top-tested walk over [first, last) (`last` re-read each pass, aligned loop head); the
// counted pointer's object is null-tested, the bit-field byte read once into al for both tests.

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
    void EnterTimelineEnd();                                              // 0x00AF4070
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
    void EnterTimelineEnd();
};

void CParticleEmitter::EnterTimelineEnd()
{
    for (CParticleSystemPtr* it = ParticleSystems.m_first; it != ParticleSystems.m_last; ++it) {
        CParticleSystem* system = it->m_pObject;
        if (system && system->Enabled && !system->Dead)
            system->EnterTimelineEnd();
    }
}