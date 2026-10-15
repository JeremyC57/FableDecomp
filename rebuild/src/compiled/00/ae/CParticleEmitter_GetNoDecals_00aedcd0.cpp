// CParticleEmitter::GetNoDecals() -- sums CParticleSystem::GetNoActiveDecals() over every enabled particle system
// Leaf: GetNoDecals
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AEDCD0, length 48 bytes (`ret`).
// Callees: CParticleSystem::GetNoActiveDecals() @ 0x00AF3F70 (returns NoActiveDecals (+0x4c) plus the components' virtual counts).
// Identity: the manifest names both this body and its twin "CParticleEmitter::GetNoDecals"; the
// callee returns the PDB member NoActiveDecals, so this one is GetNoDecals. The twin is 0x00AEDCA0 GetNoParticles; the
// callers (0x00AEEBC3/0x00AEEBCE, 0x00B0C879/0x00B0C8E9) call both.
// Layout: ParticleSystems (retail 12-byte vector of 8-byte CCountedPointer<CParticleSystem>) at
// +0x08 -- engine/CParticleEmitter.h keeps it opaque and 16 bytes wide (PDB), so the emitter is
// modelled locally. CParticleSystem's bool bit-fields (PDB +108, retail +0x50) put Enabled at bit 1.
// Shape: the total lives in ebp (zeroed after both vector words are loaded), top-tested walk with
// `last` re-read each pass, `add ebp,eax` after each call.

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
    long GetNoActiveDecals();                                             // 0x00AF3F70
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
    long GetNoDecals();
};

long CParticleEmitter::GetNoDecals()
{
    long total = 0;
    for (CParticleSystemPtr* it = ParticleSystems.m_first; it != ParticleSystems.m_last; ++it) {
        CParticleSystem* system = it->m_pObject;
        if (system->Enabled)
            total += system->GetNoActiveDecals();
    }
    return total;
}