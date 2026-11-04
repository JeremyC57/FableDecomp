// NParticleEngine::CParticleSystem::PeekEmitterVelocity() const -- the parent emitter's velocity, by reference
// Leaf: ?PeekEmitterVelocity@CParticleSystem@NParticleEngine@@QBEABVC3DVector@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF3DB0, 7 bytes: mov eax,[ecx+4] (PParentEmitter); add eax,52h; ret (thiscall, const).
// Naming wave 35c: FableWin.pdb + Ego_r.pdb ?PeekEmitterVelocity@CParticleSystem@NParticleEngine@@QBEABVC3DVector@@XZ (engine/CParticleSystem.h
//   declares it under FABLE_RETAIL_CPARTICLESYSTEM); called on the CParticleSystem& in CPSCUpdateNormal::UpdateAddParticle 0x00B0CAD0 /
//   Update 0x00B0DC20. Was a placeholder `long C00af3db0::M()` returning this->f + 82 (old file DoSizeof_DoSizeof_00af3db0.cpp).
// Layout: CParticleSystem PParentEmitter +0x04 (engine/CParticleSystem.h); CParticleEmitter Velocity at retail +0x52 (Ego_r.pdb +0x5E less
//   the 0xC the retail STLport members save), byte-packed here as the offset the old body added. The shared headers are not used: their
//   CParticleEmitter is the native layout (Velocity +0x5E) -- the packed retail emitter model is the open 0x00AF3D80 note.
class C3DVector { public: float X, Y, Z; };
namespace NParticleEngine {
#pragma pack(push, 1)
class CParticleEmitter { public:
    unsigned char _pad00[0x52];
    C3DVector     Velocity;                             // +0x52 (retail)
};
#pragma pack(pop)
class CParticleSystem { public:
    void*                   _vptr;                      // +0x00 (CBaseClass)
    const CParticleEmitter* PParentEmitter;             // +0x04
    const C3DVector& PeekEmitterVelocity() const;
};
}

const C3DVector& NParticleEngine::CParticleSystem::PeekEmitterVelocity() const
{
    return PParentEmitter->Velocity;
}
