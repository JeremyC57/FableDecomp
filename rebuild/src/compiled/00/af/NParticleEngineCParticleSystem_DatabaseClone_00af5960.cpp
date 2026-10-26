// NParticleEngine::CParticleSystem::DatabaseClone() const -- allocates a new particle system (0x70 bytes, default constructor) and copies this system's database state into it; returns the clone
// Leaf: DatabaseClone
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF5960, length 55 bytes (`ret`).
// Callees: operator new @ 0x00BFEA1A, CParticleSystem::CParticleSystem() @ 0x00AF4E10 (manifest
//          label "CHeroMorphDef" is propagated), CParticleSystem::DatabaseCopy(const CParticleSystem&)
//          @ 0x00AF4EE0.
// Identity: the manifest label "CParticleEmitter::DatabaseClone" is propagated from 0x00AF0650. This
// body clones a 0x70-byte object through CParticleSystem::DatabaseCopy, and its caller
// CParticleEmitter::DatabaseCopy (0x00AEF018) runs it on a system taken from the emitter's
// ParticleSystems vector -- CParticleSystem::DatabaseClone. 0x70 is the PDB CParticleSystem (0x8c)
// less the 0x1c the retail lists, CVectorMap and parameter block save.
// Shape: as 0x00AF0650 -- `new` without an EH frame, the DatabaseCopy call and the epilogue
// duplicated into the null and non-null arms.

namespace NParticleEngine {

class CParticleSystem { public:
    unsigned char _body[0x70];
    CParticleSystem();                                                    // 0x00AF4E10
    void DatabaseCopy(const CParticleSystem& other);                      // 0x00AF4EE0
    CParticleSystem* DatabaseClone() const;
};

CParticleSystem* CParticleSystem::DatabaseClone() const
{
    CParticleSystem* clone = new CParticleSystem;
    clone->DatabaseCopy(*this);
    return clone;
}

} // namespace NParticleEngine