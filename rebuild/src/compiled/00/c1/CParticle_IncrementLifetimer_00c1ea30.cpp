// CParticle::IncrementLifetimer() -- bump the object's 16-bit reference count (+0x1E)
// Leaf: ?IncrementLifetimer@CParticle@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1EA30, 5 bytes: inc word ptr [ecx+1Eh] ; ret
// Identity (descriptive): the catalog row's name, which the landed CParticleRef_00c25c70::Bind 0x00C25C70 binds
//   (?IncrementLifetimer@CParticle@@QAEXXZ). The landed CStreamBuffer::Attach 0x00C262E0 binds a second spelling
//   (?AddRef@CSoundSample@@QAEXXZ) to the same address; the word is the reference count that 0x00C1EA20 tests
//   for zero and 0x00C1EA40 decrements. No PDB covers libpk, so the catalog name is kept.
struct CParticle {
    unsigned char  _pad00[0x1e];
    unsigned short m_lifetimer;                 // +0x1e
    void IncrementLifetimer();
};

void CParticle::IncrementLifetimer()
{
    ++m_lifetimer;
}