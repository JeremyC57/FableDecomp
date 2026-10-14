// CParticleRef_00c25c70::Bind(CParticle*, long, long, long) -- bind to a particle (bumping its lifetimer) and record three values
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C25C70, length 37 bytes (+ int3 padding).
// Callees: CParticle::IncrementLifetimer() @ 0x00C1EA30 (`inc word ptr [ecx+1Eh]`, called on the ARGUMENT).
// Shape: a void member on a non-polymorphic 0x10-byte record (no `mov eax,esi` return of `this`, so not a
// constructor): the particle pointer is stored first, the call on the particle follows, then the three dword
// arguments are stored in order (loaded together after the call). `ret 10h`: four stack arguments. The manifest label "CAnimateThing::GetPMeshAnims"
// is propagated (the CAnimateThing layout has nothing at these offsets); the class is named by its address.
struct CParticle {
    unsigned char _pad[0x1e];
    unsigned short m_lifetimer;                 // +0x1e
    void IncrementLifetimer();                  // 0x00C1EA30
};

struct CParticleRef_00c25c70 {
    CParticle* m_pParticle;                     // +0x00
    long m_a;                                   // +0x04
    long m_b;                                   // +0x08
    long m_c;                                   // +0x0c
    void Bind(CParticle* particle, long a, long b, long c);
};

void CParticleRef_00c25c70::Bind(CParticle* particle, long a, long b, long c)
{
    m_pParticle = particle;
    particle->IncrementLifetimer();
    m_a = a;
    m_b = b;
    m_c = c;
}