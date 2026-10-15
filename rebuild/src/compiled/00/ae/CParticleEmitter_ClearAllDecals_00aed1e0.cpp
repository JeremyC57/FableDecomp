// CParticleEmitter::ClearAllDecals(CIEngine* engine) -- once (guarded by the ClearedDecals bit-field, bit 4 of +0x45): every particle system clears its decals from the emitter's primitive group, then the flag is set
// Leaf: ClearAllDecals
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AED1E0, length 52 bytes (`ret 4`; the engine argument is not read).
// Callees: CParticleSystem::ClearAllDecals(const CEnginePrimitiveHandle&) @ 0x00AF58D0 (walks the system's Decals list (+0x20)).
// Identity: the manifest names both this body and its twin "CParticleEmitter::ClearAllDecals"; the
// callee walks the system's Decals list (+0x20) (PDB CParticleSystem: Particles +32 / Decals +44, retail +0x1c /
// +0x20) and the guard is the PDB bit-field ClearedDecals, so this one is ClearAllDecals. The twin uses bit 3 and ClearAllParticles 0x00AF58A0 (0x00AED140); every
// caller calls both in a row.
// Layout (retail; PDB CParticleEmitter with the 12-byte retail vector and 16-byte CVectorMaps, i.e.
// 0xc below the PDB from PrimitiveGroupHandle on): ParticleSystems +0x08, UpdateParameters +0x14
// (0x24 bytes, see 0x00AF0610), PrimitiveGroupHandle +0x38, the PDB bit-field groups at +0x40
// (ushort: Priority..Emitter2D), +0x42 (Emitter2DLayer), +0x44 and +0x45 (EnableDithering,
// CalcBoundingSphereOnceOnly, BoundingSphereValid, ClearedParticles, ClearedDecals).
// engine/CParticleEmitter.h carries the PDB offsets and opaque bit-field bytes, so the emitter is
// modelled locally.
// Shape: the bit-field test/set are `test byte ptr [edi+45h],10h` / `or byte ptr [edi+45h],10h`; the
// handle's address is formed in ebp once, inside the non-empty path; `last` re-read each pass.

struct CEnginePrimitiveHandle {
    void* m_pGroup;
    long  m_index;
};

struct CParticleSystem {
    void ClearAllDecals(const CEnginePrimitiveHandle& group);                // 0x00AF58D0
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

struct CIEngine;

struct CParticleEmitter {
    virtual void v00();
    unsigned char            _base4[4];                                   // +0x04
    CParticleSystemPtrVector ParticleSystems;                             // +0x08
    unsigned char            UpdateParameters[0x24];                      // +0x14 CParticleUpdateParameters<unsigned long,unsigned long>
    CEnginePrimitiveHandle   PrimitiveGroupHandle;                        // +0x38
    unsigned short           Priority                   : 3;              // +0x40 bits 0..2
    unsigned short           IsScreenDisplacement       : 1;              //       bit 3
    unsigned short           ReadZBuffer                : 1;              //       bit 4
    unsigned short           Dead                       : 1;              //       bit 5
    unsigned short           ParticleCreationDisabled   : 1;              //       bit 6
    unsigned short           ContinuousEmitter          : 1;              //       bit 7
    unsigned short           Initialised                : 1;              //       bit 8
    unsigned short           WaitingToSleep             : 1;              //       bit 9
    unsigned short           Sleeping                   : 1;              //       bit 10
    unsigned short           Emitter2D                  : 1;              //       bit 11
    unsigned short           Emitter2DLayer             : 7;              // +0x42 bits 0..6
    unsigned char            PreWaterEffect             : 1;              // +0x44 bit 0
    unsigned char            WaterEffect                : 1;              //       bit 1
    unsigned char            ZBufferWriteable           : 1;              //       bit 2
    unsigned char            Visible                    : 1;              //       bit 3
    unsigned char            AlwaysUpdate               : 1;              //       bit 4
    unsigned char            DieIfOffsceen              : 1;              //       bit 5
    unsigned char            OffscreenUpdate            : 1;              //       bit 6
    unsigned char            ClipEffectToWeatherMask    : 1;              //       bit 7
    unsigned char            EnableDithering            : 1;              // +0x45 bit 0
    unsigned char            CalcBoundingSphereOnceOnly : 1;              //       bit 1
    unsigned char            BoundingSphereValid        : 1;              //       bit 2
    unsigned char            ClearedParticles           : 1;              //       bit 3
    unsigned char            ClearedDecals              : 1;              //       bit 4
    void ClearAllDecals(CIEngine* engine);
};

void CParticleEmitter::ClearAllDecals(CIEngine* engine)
{
    if (!ClearedDecals) {
        for (CParticleSystemPtr* it = ParticleSystems.m_first; it != ParticleSystems.m_last; ++it)
            it->m_pObject->ClearAllDecals(PrimitiveGroupHandle);
        ClearedDecals = 1;
    }
}