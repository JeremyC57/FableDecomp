// NParticleEngine::CPSCUpdateNormal::WriteBinary(CDataOutputStream*) -- serialize the normal-update component: fade integers, the flag bits, the decal emitter name, fifteen floats, five vectors and two params
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B0B5C0, 670 bytes. Label correction: the catalog row says
//   CPSCDecalRenderer::WriteBinary, but the only reference is vtable slot 16 of NParticleEngine::CPSCUpdateNormal
//   (0x0129FC1C, rtti-vtables.tsv).
// Callees (lane names, as in the landed CPSCAttractor/CPSCLight WriteBinary): WriteULong 0x00993EA0 (propagated
//   label "WriteCBYTE"), WriteEBOOL 0x00993E30, WriteNullTerminatedString 0x00995AE0, WriteFloat 0x00993EE0,
//   Write3DVector 0x00993FD0.
// Layout: PDB (Ego_r) CPSCUpdateNormal names/types; retail offsets equal the PDB: an unsigned long bit-field word at
//   +0x0c (two 10-bit integers then twelve 1-bit flags), bool bit-fields in +0x10, DecalEmitterName +0x14, floats
//   +0x18..+0x50, vectors +0x54..+0x84, AccelerationParam/OrientFromGameParam +0x90/+0x94.
// Shape: one stream call per member in the order the original wrote them (ParticleCollideWithAnything sits between
//   ParticleCollideWithGround and ParticleDieOnCollision); a 1-bit flag passed as bool extracts as
//   `shr ; and r,0xffffff01`, bit 24 is read as the byte +0x0f.
#include "engine/CDataOutputStream.h"
class C3DVector { public: float X, Y, Z; };
class CCharString { public: void* PStringData; };
namespace NParticleEngine {
class CPSCUpdateNormal { public:
    unsigned char _base[0xc];                           // CPSCBase
    unsigned long FadeInEndInteger : 10;                // +0x0c
    unsigned long FadeOutBeginInteger : 10;
    unsigned long UseParticleLifeSecs : 1;              // bit 20
    unsigned long UseRandomRotationAxis : 1;
    unsigned long UseRandomInitialRotation : 1;
    unsigned long UseAccelerationParam : 1;
    unsigned long StayWithEmitter : 1;                  // bit 24
    unsigned long UseSystemLifeSecs : 1;
    unsigned long SystemAlphaFadeEnable : 1;
    unsigned long EmissionFadeEnable : 1;
    unsigned long ParticleCollideWithGround : 1;
    unsigned long ParticleDieOnCollision : 1;
    unsigned long UseAllAttractors : 1;
    unsigned long RandomisePosEnable : 1;               // bit 31
    bool RandomisePosDistVaryEnable : 1;                // +0x10 bit 0
    bool SetOrientationFromDirection : 1;
    bool SetOrientationFromGame : 1;
    bool ParticleCreateDecal : 1;
    bool ParticleCreateDecalEmitter : 1;
    bool ParticleCollideWithAnything : 1;               // +0x10 bit 5
    CCharString DecalEmitterName;                       // +0x14
    float SystemLifeSecs;                               // +0x18
    float ParticleLifeSecs;
    float WindFactor;
    float GravityFactor;
    float AirResistance;
    float ParticleAccelerationScale;
    float InitialRotationX;
    float InitialRotationY;
    float InitialRotationZ;
    float RotationMinAngleSpeed;
    float RotationMaxAngleSpeed;
    float ParticleBounce;
    float SystemAlphaFadeMinimum;
    float RandomisePosDistVaryScale;
    float EmissionFadeMinimum;                          // +0x50
    C3DVector ParticleSystemOffset;                     // +0x54
    C3DVector ParticleAcceleration;                     // +0x60
    C3DVector RotationAxis;                             // +0x6c
    C3DVector RandomisePosSpeed;                        // +0x78
    C3DVector RandomisePosScale;                        // +0x84
    unsigned long AccelerationParam;                    // +0x90
    unsigned long OrientFromGameParam;                  // +0x94
    void WriteBinary(CDataOutputStream* out);
};
void CPSCUpdateNormal::WriteBinary(CDataOutputStream* destination)
{
    CDataOutputStream* out = (CDataOutputStream*)destination;
    out->WriteULONG(FadeInEndInteger);
    out->WriteULONG(FadeOutBeginInteger);
    out->WriteEBOOL(UseParticleLifeSecs);
    out->WriteEBOOL(UseRandomRotationAxis);
    out->WriteEBOOL(UseRandomInitialRotation);
    out->WriteEBOOL(UseAccelerationParam);
    out->WriteEBOOL(StayWithEmitter);
    out->WriteEBOOL(UseSystemLifeSecs);
    out->WriteEBOOL(SystemAlphaFadeEnable);
    out->WriteEBOOL(EmissionFadeEnable);
    out->WriteEBOOL(ParticleCollideWithGround);
    out->WriteEBOOL(ParticleCollideWithAnything);
    out->WriteEBOOL(ParticleDieOnCollision);
    out->WriteEBOOL(UseAllAttractors);
    out->WriteEBOOL(RandomisePosEnable);
    out->WriteEBOOL(RandomisePosDistVaryEnable);
    out->WriteEBOOL(SetOrientationFromDirection);
    out->WriteEBOOL(SetOrientationFromGame);
    out->WriteEBOOL(ParticleCreateDecal);
    out->WriteEBOOL(ParticleCreateDecalEmitter);
    out->WriteNullTerminatedString(DecalEmitterName);
    out->WriteFloat(SystemLifeSecs);
    out->WriteFloat(ParticleLifeSecs);
    out->WriteFloat(WindFactor);
    out->WriteFloat(GravityFactor);
    out->WriteFloat(AirResistance);
    out->WriteFloat(ParticleAccelerationScale);
    out->WriteFloat(InitialRotationX);
    out->WriteFloat(InitialRotationY);
    out->WriteFloat(InitialRotationZ);
    out->WriteFloat(RotationMinAngleSpeed);
    out->WriteFloat(RotationMaxAngleSpeed);
    out->WriteFloat(ParticleBounce);
    out->WriteFloat(SystemAlphaFadeMinimum);
    out->WriteFloat(RandomisePosDistVaryScale);
    out->WriteFloat(EmissionFadeMinimum);
    out->Write3DVector(ParticleSystemOffset);
    out->Write3DVector(ParticleAcceleration);
    out->Write3DVector(RotationAxis);
    out->Write3DVector(RandomisePosSpeed);
    out->Write3DVector(RandomisePosScale);
    out->WriteULONG(AccelerationParam);
    out->WriteULONG(OrientFromGameParam);
}
}