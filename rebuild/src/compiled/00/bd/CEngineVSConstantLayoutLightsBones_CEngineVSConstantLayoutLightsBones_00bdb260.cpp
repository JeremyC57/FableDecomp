// CEngineVSConstantLayoutLightsBones::CEngineVSConstantLayoutLightsBones() -- constructor over CEngineVSConstantLayoutLights: own vtable, then the bone range (register 0x26, 0x36 registers) and the particle range (register 0x5c, 4 registers) assigned from CShaderConstantRange temporaries
// Leaf: ??0CEngineVSConstantLayoutLightsBones
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDB260, length 130 bytes (+ int3 padding).
// Callees: CEngineVSConstantLayoutLights::CEngineVSConstantLayoutLights @ 0x00BDB400, CBaseClass::CBaseClass @ 0x0099A2D0 and
// CBaseClass::~CBaseClass @ 0x0099A2E0 (twice each, on the 12-byte stack temporary). Vtables: 0x012A7D04 =
// CEngineVSConstantLayoutLightsBones, 0x0129A04C = CShaderConstantRange (rtti-vtables.tsv).
// Model reused from the landed sibling CEngineVSConstantLayoutParticles::CEngineVSConstantLayoutParticles @ 0x00BDAFF0: a
// CBaseClass-derived {first register, register count} range whose implicit assignment copies the two words interleaved
// with the temporary's own stores, then the temporary's destructor. The ranges live in the base layout at +0xd4 and +0x10;
// the engine header (engine/CEngineVSConstantLayoutLightsBones.h) is a flattened 0x150-byte pad, so they are modelled locally.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy

class CShaderConstantRange : public CBaseClass { public:         // vtable 0x0129A04C
    long m_firstRegister;                          // +0x04
    long m_registerCount;                          // +0x08
    CShaderConstantRange(long firstRegister, long registerCount) : CBaseClass(), m_firstRegister(firstRegister), m_registerCount(registerCount) { }
};

struct CEngineVSConstantLayoutLights {             // 0x150 bytes
    virtual void v00();
    unsigned char        _pad0[0xc];               // +0x04..+0x10
    CShaderConstantRange ParticleRange;            // +0x10 (first +0x14, count +0x18)
    unsigned char        _pad1c[0xb8];             // +0x1c..+0xd4
    CShaderConstantRange BoneRange;                // +0xd4 (first +0xd8, count +0xdc)
    unsigned char        _pad1[0x150 - 0xe0];
    CEngineVSConstantLayoutLights();               // 0x00BDB400
};

class CEngineVSConstantLayoutLightsBones : public CEngineVSConstantLayoutLights { public:   // vtable 0x012A7D04
    virtual void v00();
    CEngineVSConstantLayoutLightsBones();
};

CEngineVSConstantLayoutLightsBones::CEngineVSConstantLayoutLightsBones()
    : CEngineVSConstantLayoutLights()
{
    BoneRange = CShaderConstantRange(0x26, 0x36);
    ParticleRange = CShaderConstantRange(0x5c, 4);
}