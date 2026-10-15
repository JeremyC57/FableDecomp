// CParticleEmitter::GetParticleStats(CParticleStats* stats) -- adds the emitter's systems to SystemsActive, counts each visible system (else, while the emitter sleeps, each sleeping one) and lets every system add its own stats
// Leaf: GetParticleStats
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AED290, length 88 bytes (`ret 4`).
// Callees: CParticleSystem::GetParticleStats(CParticleStats*) @ 0x00AF43B0.
// Layout: PDB CParticleStats (all longs): SystemsVisible +0x0c, SystemsActive +0x10, SystemsSleeping
// +0x14. Retail emitter: ParticleSystems (12-byte vector of 8-byte counted pointers) +0x08, the
// ushort bit-field group at +0x40 whose bit 10 is Sleeping (PDB order Priority:3,
// IsScreenDisplacement, ReadZBuffer, Dead, ParticleCreationDisabled, ContinuousEmitter, Initialised,
// WaitingToSleep, Sleeping, Emitter2D). CParticleSystem's bool bit-fields at +0x50 put Visible at
// bit 5. engine/CParticleEmitter.h carries the PDB offsets, so the emitter is modelled locally.
// Shape: `size()` is `(last - first)` (`sub ; sar eax,3`) added through a load/add/store; the walk
// re-reads `first` after that store (it may alias), `last` after every call; the visible count is
// `inc [ebp+0Ch]` then a jump over the sleeping test, the stats pointer stays in ebp for the pushes.
// The Sleeping bit is read through an inline bool accessor (`bool IsSleeping() const { return
// Sleeping; }`): that keeps the whole-word `xor edx,edx ; mov dx,[edi+40h] ; shr edx,0Ah ; test dl,1`
// (testing the bit-field or `(word >> 10) & 1` directly folds to `test byte ptr [edi+41h],4`).

struct CParticleStats {
    long EmittersVisible;                                                 // +0x00
    long EmittersActive;                                                  // +0x04
    long EmittersSleeping;                                                // +0x08
    long SystemsVisible;                                                  // +0x0c
    long SystemsActive;                                                   // +0x10
    long SystemsSleeping;                                                 // +0x14
};

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
    void GetParticleStats(CParticleStats* stats);                         // 0x00AF43B0
};

struct CParticleSystemPtr {                                               // CCountedPointer<CParticleSystem> (8 bytes)
    CParticleSystem* m_pObject;
    void*            m_pCount;
};

struct CParticleSystemPtrVector {                                         // retail vector<CCountedPointer<CParticleSystem> > (12 bytes)
    CParticleSystemPtr* m_first;
    CParticleSystemPtr* m_last;
    CParticleSystemPtr* m_end;
    long size() const { return m_last - m_first; }
};

struct CParticleEmitter {
    virtual void v00();
    unsigned char            _base4[4];                                   // +0x04
    CParticleSystemPtrVector ParticleSystems;                             // +0x08
    unsigned char            UpdateParameters[0x24];                      // +0x14
    unsigned char            PrimitiveGroupHandle[8];                     // +0x38
    unsigned short           Priority                 : 3;                // +0x40 bits 0..2
    unsigned short           IsScreenDisplacement     : 1;                //       bit 3
    unsigned short           ReadZBuffer              : 1;                //       bit 4
    unsigned short           Dead                     : 1;                //       bit 5
    unsigned short           ParticleCreationDisabled : 1;                //       bit 6
    unsigned short           ContinuousEmitter        : 1;                //       bit 7
    unsigned short           Initialised              : 1;                //       bit 8
    unsigned short           WaitingToSleep           : 1;                //       bit 9
    unsigned short           Sleeping                 : 1;                //       bit 10
    unsigned short           Emitter2D                : 1;                //       bit 11
    bool IsSleeping() const { return Sleeping; }
    void GetParticleStats(CParticleStats* stats);
};

void CParticleEmitter::GetParticleStats(CParticleStats* stats)
{
    stats->SystemsActive += ParticleSystems.size();
    for (CParticleSystemPtr* it = ParticleSystems.m_first; it != ParticleSystems.m_last; ++it) {
        CParticleSystem* system = it->m_pObject;
        if (system->Visible)
            ++stats->SystemsVisible;
        else if (IsSleeping())
            ++stats->SystemsSleeping;
        system->GetParticleStats(stats);
    }
}