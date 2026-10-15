// CParticleEmitter::DatabaseClone() const -- allocates a new emitter (0x16c bytes, default constructor) and copies this emitter's database state into it; returns the clone
// Leaf: DatabaseClone
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF0650, length 58 bytes (`ret`).
// Callees: operator new @ 0x00BFEA1A, CParticleEmitter::CParticleEmitter() @ 0x00AF0380,
//          CParticleEmitter::DatabaseCopy(const CParticleEmitter&) @ 0x00AEEFE0.
// Layout: sizeof(CParticleEmitter) is 0x16c in the shipped build (PDB 0x178: the 12-byte vector and
// 16-byte CVectorMaps save 0xc), so the emitter is modelled locally with an opaque body.
// Shape: `new` without an EH frame (no /EHsc in this module); the null and non-null arms each get
// their own copy of the DatabaseCopy call and the epilogue (VC7.1 tail-duplicates the join), the
// null arm copying into a null `this` exactly as the source asks.

struct CParticleEmitter {
    virtual void v00();
    unsigned char _body[0x168];                                           // +0x04 .. +0x16c
    CParticleEmitter();                                                   // 0x00AF0380
    void DatabaseCopy(const CParticleEmitter& other);                     // 0x00AEEFE0
    CParticleEmitter* DatabaseClone() const;
};

CParticleEmitter* CParticleEmitter::DatabaseClone() const
{
    CParticleEmitter* clone = new CParticleEmitter;
    clone->DatabaseCopy(*this);
    return clone;
}