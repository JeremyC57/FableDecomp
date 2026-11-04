// CTCDParticleEmitter::Create(const CCharString& emitterName, const C3DVector& position, bool forceParticleToLoad) -- by-name particle emitter creation: look the emitter template up in the particle database and forward to the typed Create; null while compiling the front-end definitions or for an unknown name
// Leaf: ?Create@CTCDParticleEmitter@@SIPAVCThing@@ABVCCharString@@ABVC3DVector@@_N@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Naming wave 35c (caller respell): calls 0x00AEAAE0 as NParticleEngine::GetParticleEmitterDatabase() (?GetParticleEmitterDatabase@NParticleEngine@@YIAAVCParticleEmitterDatabase@1@XZ, FableWin.pdb + Ego_r.pdb); the database class is NParticleEngine::CParticleEmitterDatabase, its GetEmitterTemplateHandleFromName the const FableWin.pdb ?GetEmitterTemplateHandleFromName@CParticleEmitterDatabase@NParticleEngine@@QBEJABVCCharString@@@Z (was a static CParticleEmitterDatabase::Get()).
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E0970, 53 bytes, static `__fastcall` (ecx = name, edx = position), `ret 4`.
// Identity: Ego_r.pdb name set `?Create@CTCDParticleEmitter@@SIPAVCThing@@ABVCCharString@@ABVC3DVector@@_N@Z`; 17 callers.
// Callees: CParticleEmitterDatabase::Get() @ 0x00AEAAE0 (static `__fastcall`, the manifest's GFGetPreMainMemoryUsed is a
//   byte twin), CParticleEmitterDatabase::GetEmitterTemplateHandleFromName(const CCharString&) @ 0x00AE85B0,
//   CTCDParticleEmitter::Create(long, const C3DVector&, bool) @ 0x006E0880.
// Globals: g_bCompileFrontendDefinitions @ 0x013B8648 (bool, as named by the landed boot code).
// Shape: the name is pushed before the database getter (arguments first, then the object expression); the typed
//   overload is called (not tail-jumped) and its result returned; one shared `xor eax,eax` exit.
#include "engine/C3DVector.h"
class CCharString { public: void* PStringData; };
class CThing;

namespace NParticleEngine {
class CParticleEmitterDatabase {
public:
    long GetEmitterTemplateHandleFromName(const CCharString& name) const;   // 0x00AE85B0
};
CParticleEmitterDatabase& __fastcall GetParticleEmitterDatabase();          // 0x00AEAAE0
}

extern bool g_bCompileFrontendDefinitions;                  // 0x013B8648

class CTCDParticleEmitter {
public:
    static CThing* __fastcall Create(long particleTypeID, const C3DVector& position, bool forceParticleToLoad);   // 0x006E0880
    static CThing* __fastcall Create(const CCharString& emitterName, const C3DVector& position, bool forceParticleToLoad);
};

CThing* __fastcall CTCDParticleEmitter::Create(const CCharString& emitterName, const C3DVector& position, bool forceParticleToLoad)
{
    if (!g_bCompileFrontendDefinitions) {
        long particleTypeID = NParticleEngine::GetParticleEmitterDatabase().GetEmitterTemplateHandleFromName(emitterName);
        if (particleTypeID > 0)
            return Create(particleTypeID, position, forceParticleToLoad);
    }
    return 0;
}