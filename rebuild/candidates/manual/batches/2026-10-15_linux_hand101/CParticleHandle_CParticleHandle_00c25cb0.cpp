// CParticleHandle::~CParticleHandle() -- clear the three cached words, drop the particle's reference and null the pointer
// Leaf: ??1CParticleHandle
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C25CB0, length 39 bytes (callers 0x00C11294, 0x00C11A52, 0x00C2169D: each
// calls it on a 16-byte stack object after use, the shape of a local's destructor).
// Callees: the particle's reference drop @ 0x00C1EA40 (`dec word ptr [ecx+1Eh]`; landed as CParticle::IncrementLifetimer).
// Label correction: the chunk calls this CTCStealth::OnCreate (propagated); the class is named descriptively from the
// callee (no PDB class is established for the 16-byte record).
// Shape: the pointer is loaded first (ecx) and the zero stores are scheduled around it; no null test before the call.
struct CParticle {
    unsigned char  _pad[0x1e];
    unsigned short m_refCount;                              // +0x1e
    void DecRef();                                          // 0x00C1EA40
};

struct CParticleHandle {
    CParticle* m_pParticle;                                 // +0x00
    long       m_a;                                         // +0x04
    long       m_b;                                         // +0x08
    long       m_c;                                         // +0x0c
    ~CParticleHandle();
};

CParticleHandle::~CParticleHandle()
{
    m_a = 0;
    m_b = 0;
    m_c = 0;
    m_pParticle->DecRef();
    m_pParticle = 0;
}
