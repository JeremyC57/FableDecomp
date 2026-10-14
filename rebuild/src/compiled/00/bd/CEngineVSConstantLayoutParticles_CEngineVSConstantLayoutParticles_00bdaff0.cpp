// CEngineVSConstantLayoutParticles::CEngineVSConstantLayoutParticles() -- constructor over CEngineVSConstantLayoutLights: own vtable, then the particle constant range (register 0x26, 0x3a registers) assigned from a CShaderConstantRange temporary
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDAFF0, length 74 bytes (the oracle row carries 6 bytes of int3 padding).
// Callees: CEngineVSConstantLayoutLights::CEngineVSConstantLayoutLights @ 0x00BDB400, CBaseClass::CBaseClass @ 0x0099A2D0 and CBaseClass::~CBaseClass
// @ 0x0099A2E0 on the 12-byte stack temporary (`sub esp,0Ch`, `lea ecx,[esp+4]`). This class's vtable is 0x012A7CC8 (rtti-vtables.tsv:
// CEngineVSConstantLayoutParticles); the temporary's vtable 0x0129A04C is CShaderConstantRange (rtti-vtables.tsv) -- a CBaseClass-derived
// {start, count} record whose implicit assignment copies the two words (`mov [esi+14h],ecx ; mov [esi+18h],eax`) interleaved with the
// temporary's own stores, before its destructor (inline: the dead derived vtable store, then the out-of-line CBaseClass destructor).
// The range being assigned lives in the base layout at +0x10 (the base constructor builds the one at +0x104 the same way); the engine
// header (CEngineVSConstantLayoutParticles.h) is a flattened 0x150-byte pad, so the members are modelled locally.
struct CBaseClass {
    virtual void v00();
    CBaseClass();                                  // 0x0099A2D0
    ~CBaseClass();                                 // 0x0099A2E0
};

struct CShaderConstantRange : CBaseClass {         // vtable 0x0129A04C: a register range {first register, register count}
    long m_firstRegister;                          // +0x04
    long m_registerCount;                          // +0x08
    virtual void v00();
    CShaderConstantRange(long firstRegister, long registerCount) : CBaseClass(), m_firstRegister(firstRegister), m_registerCount(registerCount) { }
};

struct CEngineVSConstantLayoutLights {             // 0x150 bytes; the named ranges of the vertex-shader constant layout
    virtual void v00();
    unsigned char        _pad0[0xc];               // +0x04..+0x10
    CShaderConstantRange ParticleRange;            // +0x10 (vptr +0x10, first +0x14, count +0x18)
    unsigned char        _pad1[0x150 - 0x1c];
    CEngineVSConstantLayoutLights();               // 0x00BDB400
};

struct CEngineVSConstantLayoutParticles : CEngineVSConstantLayoutLights {   // vtable 0x012A7CC8
    virtual void v00();
    CEngineVSConstantLayoutParticles();
};

CEngineVSConstantLayoutParticles::CEngineVSConstantLayoutParticles()
    : CEngineVSConstantLayoutLights()
{
    ParticleRange = CShaderConstantRange(0x26, 0x3a);
}