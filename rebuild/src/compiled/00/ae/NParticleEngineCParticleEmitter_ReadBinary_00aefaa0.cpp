// NParticleEngine::CParticleEmitter::ReadBinary(CDataInputStream& in) -- load an emitter template in WriteBinary order: version, name, render flags, distances, priority, then every particle system (kept when enabled), each followed by a separator byte
// Leaf: ?ReadBinary@CParticleEmitter@NParticleEngine@@QAEXAAVCDataInputStream@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEFAA0, 2260 bytes, thiscall, `ret 4`.
// Identity: FableWin.pdb / Ego_r.pdb ?ReadBinary@CParticleEmitter@NParticleEngine@@QAEXAAVCDataInputStream@@@Z (non-virtual); the
//   mirror of the landed CParticleEmitter::WriteBinary 0x00AEDAF0 (same field order: FadeInEnd before FadeInStart, Priority as a
//   long, the 0x26 separator byte after each system). FableWin's debug COMDATs right after it: CParticleSystem::IsEnabled,
//   then vector<CCountedPointer<CParticleSystem> >::reserve / push_back and CCountedPointer(CParticleSystem*).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (slow path of the inline Read), the stream's virtual
//   ReadNullTerminatedString (slot 6), CCharString::operator= 0x0099EFB0, ~CCharString 0x0099EAE0, STLport
//   vector<CCountedPointer<CParticleSystem> >::reserve 0x00AF0EF0 and _M_insert_overflow 0x00AF0FA0, operator new 0x00BFEA1A,
//   CParticleSystem::CParticleSystem 0x00AF4E10, CCountedPointer<CParticleSystem>::DeleteData 0x00AF0710 (stored),
//   CParticleSystem::ReadBinary 0x00AF4660, operator delete 0x00BFE9BC.
// Layout: retail CParticleEmitter (engine/CParticleEmitter.h FABLE_RETAIL_CPARTICLEEMITTER: ParticleSystems +0x08, flag word +0x40,
//   flag bytes +0x44/+0x45, Name +0x140, distances +0x158..+0x168); CParticleSystem Enabled = +0x50 bit 1.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_CPARTICLESYSTEM
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_STLP_VECTOR
#include "engine/CParticleEmitter.h"
#include "engine/CParticleSystem.h"
#include "engine/CDataInputStream.h"
namespace NParticleEngine {
void CParticleEmitter::ReadBinary(CDataInputStream& in)
{
    in.ReadULONG();
    Name = in.ReadNullTerminatedString();
    Emitter2D = in.ReadEBOOL();
    PreWaterEffect = in.ReadEBOOL();
    WaterEffect = in.ReadEBOOL();
    ZBufferWriteable = in.ReadEBOOL();
    ContinuousEmitter = in.ReadEBOOL();
    IsScreenDisplacement = in.ReadEBOOL();
    ReadZBuffer = in.ReadEBOOL();
    MaxSpawnDistance = in.ReadFloat();
    MaxDrawDistance = in.ReadFloat();
    FadeOutStart = in.ReadFloat();
    FadeInEnd = in.ReadFloat();
    FadeInStart = in.ReadFloat();
    Priority = in.ReadSLONG();
    DieIfOffsceen = in.ReadEBOOL();
    OffscreenUpdate = in.ReadEBOOL();
    ClipEffectToWeatherMask = in.ReadEBOOL();
    EnableDithering = in.ReadEBOOL();
    CalcBoundingSphereOnceOnly = in.ReadEBOOL();
    unsigned long count = in.ReadULONG();
    ParticleSystems.reserve(count);
    for (unsigned long i = 0; i < count; ++i) {
        CCountedPointer<CParticleSystem> psystem = new CParticleSystem;
        psystem->SetParentEmitter(*this);
        psystem->ReadBinary(in);
        if (psystem->IsEnabled())
            ParticleSystems.push_back(psystem);
        in.ReadEBOOL();
    }
}
}