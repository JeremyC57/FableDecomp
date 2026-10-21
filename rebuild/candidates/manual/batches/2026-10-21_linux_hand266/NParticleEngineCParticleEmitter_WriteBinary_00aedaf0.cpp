// NParticleEngine::CParticleEmitter::WriteBinary(CDataOutputStream*) -- serialize an emitter template: version 100, name, the render flag bits, spawn/draw/fade distances, priority, then every particle system each followed by a 0x26 separator byte
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEDAF0, length 432 bytes (421 + int3 padding).
// Callees (lane names): WriteULong 0x00993EA0 (propagated "WriteCBYTE"), WriteNullTerminatedString 0x00995AE0,
//   WriteEBOOL 0x00993E30, WriteFloat 0x00993EE0, WriteLong 0x00993EB0, WriteUBYTE 0x00993E50,
//   NParticleEngine::CParticleSystem::WriteBinary 0x00AF4240.
// Layout (retail; the landed dtor 0x00AF10E0 shows Ego_r.pdb CParticleEmitter is 0xc larger from +0x18): ParticleSystems
//   vector +0x08 (8-byte CCountedPointer elements), ushort flag word +0x40 (Priority:3, IsScreenDisplacement, ReadZBuffer,
//   Dead, ParticleCreationDisabled, ContinuousEmitter, Initialised, WaitingToSleep, Sleeping, Emitter2D), Emitter2DLayer
//   +0x42, uchar flag bytes +0x44/+0x45, Name +0x140, MaxSpawnDistance +0x158 .. FadeInEnd +0x168. engine/CParticleEmitter.h
//   is the wider PDB view, so the class is modelled locally.
// Shape: one stream call per field in the file order (not declaration order); ushort bits 11 and 7 are read as a word
//   and a byte respectively, exactly as VC7.1 narrows bit-field loads; Priority is passed to WriteLong as `and ecx,7`.
#include "engine/CDataOutputStream.h"
struct CCharString { void* m_pStringData; };
struct CDataOutputStream_Methods : CDataOutputStream {
    void WriteEBOOL(bool value);                                    // 0x00993E30
    void WriteUBYTE(unsigned char value);                           // 0x00993E50
    void WriteLong(long value);                                     // 0x00993EB0
    void WriteULong(unsigned long value);                           // 0x00993EA0
    void WriteFloat(float value);                                   // 0x00993EE0
    void WriteNullTerminatedString(const CCharString& text);        // 0x00995AE0
};
namespace NParticleEngine {
struct CParticleSystem {
    void WriteBinary(CDataOutputStream* out);                       // 0x00AF4240
};
struct CParticleSystemPointer {                                     // CCountedPointer<CParticleSystem>
    CParticleSystem* m_p;
    void* m_count;
    CParticleSystem* operator->() const { return m_p; }
};
struct CParticleEmitter {
    void* __vftable;                                                // +0x00
    void* m_pLink;                                                  // +0x04 CBaseObject
    CParticleSystemPointer* ParticleSystems_start;                  // +0x08
    CParticleSystemPointer* ParticleSystems_finish;                 // +0x0c
    CParticleSystemPointer* ParticleSystems_end;                    // +0x10
    unsigned char UpdateParameters[0x24];                           // +0x14
    unsigned char PrimitiveGroupHandle[8];                          // +0x38
    unsigned short Priority : 3;                                    // +0x40
    unsigned short IsScreenDisplacement : 1;
    unsigned short ReadZBuffer : 1;
    unsigned short Dead : 1;
    unsigned short ParticleCreationDisabled : 1;
    unsigned short ContinuousEmitter : 1;
    unsigned short Initialised : 1;
    unsigned short WaitingToSleep : 1;
    unsigned short Sleeping : 1;
    unsigned short Emitter2D : 1;
    unsigned short Emitter2DLayer : 7;                              // +0x42
    unsigned char PreWaterEffect : 1;                               // +0x44
    unsigned char WaterEffect : 1;
    unsigned char ZBufferWriteable : 1;
    unsigned char Visible : 1;
    unsigned char AlwaysUpdate : 1;
    unsigned char DieIfOffsceen : 1;
    unsigned char OffscreenUpdate : 1;
    unsigned char ClipEffectToWeatherMask : 1;
    unsigned char EnableDithering : 1;                              // +0x45
    unsigned char CalcBoundingSphereOnceOnly : 1;
    unsigned char BoundingSphereValid : 1;
    unsigned char ClearedParticles : 1;
    unsigned char ClearedDecals : 1;
    unsigned char _rest46[0xfa];                                    // +0x46 OldPos, Velocity, interpolators, transforms
    CCharString Name;                                               // +0x140
    unsigned char BSphere[0x10];                                    // +0x144
    float DistanceToCam;                                            // +0x154
    float MaxSpawnDistance;                                         // +0x158
    float MaxDrawDistance;                                          // +0x15c
    float FadeOutStart;                                             // +0x160
    float FadeInStart;                                              // +0x164
    float FadeInEnd;                                                // +0x168
    void WriteBinary(CDataOutputStream* out);
};
void CParticleEmitter::WriteBinary(CDataOutputStream* destination)
{
    CDataOutputStream_Methods* out = (CDataOutputStream_Methods*)destination;
    out->WriteULong(100);
    out->WriteNullTerminatedString(Name);
    out->WriteEBOOL(Emitter2D);
    out->WriteEBOOL(PreWaterEffect);
    out->WriteEBOOL(WaterEffect);
    out->WriteEBOOL(ZBufferWriteable);
    out->WriteEBOOL(ContinuousEmitter);
    out->WriteEBOOL(IsScreenDisplacement);
    out->WriteEBOOL(ReadZBuffer);
    out->WriteFloat(MaxSpawnDistance);
    out->WriteFloat(MaxDrawDistance);
    out->WriteFloat(FadeOutStart);
    out->WriteFloat(FadeInEnd);
    out->WriteFloat(FadeInStart);
    out->WriteLong(Priority);
    out->WriteEBOOL(DieIfOffsceen);
    out->WriteEBOOL(OffscreenUpdate);
    out->WriteEBOOL(ClipEffectToWeatherMask);
    out->WriteEBOOL(EnableDithering);
    out->WriteEBOOL(CalcBoundingSphereOnceOnly);
    out->WriteULong(ParticleSystems_finish - ParticleSystems_start);
    for (CParticleSystemPointer* it = ParticleSystems_start; it != ParticleSystems_finish; ++it) {
        (*it)->WriteBinary(destination);
        out->WriteUBYTE(0x26);
    }
}
}
