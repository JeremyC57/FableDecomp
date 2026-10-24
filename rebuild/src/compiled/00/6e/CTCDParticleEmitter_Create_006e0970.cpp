// CTCDParticleEmitter::Create(const CCharString& emitterName, const C3DVector& position, bool forceParticleToLoad) -- by-name particle emitter creation: look the emitter template up in the particle database and forward to the typed Create; null while compiling the front-end definitions or for an unknown name
// Leaf: ?Create@CTCDParticleEmitter@@SIPAVCThing@@ABVCCharString@@ABVC3DVector@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E0970, 53 bytes, static `__fastcall` (ecx = name, edx = position), `ret 4`.
// Identity: Ego_r.pdb name set `?Create@CTCDParticleEmitter@@SIPAVCThing@@ABVCCharString@@ABVC3DVector@@_N@Z`; 17 callers.
// Callees: CParticleEmitterDatabase::Get() @ 0x00AEAAE0 (static `__fastcall`, the manifest's GFGetPreMainMemoryUsed is a
//   byte twin), CParticleEmitterDatabase::GetEmitterTemplateHandleFromName(const CCharString&) @ 0x00AE85B0,
//   CTCDParticleEmitter::Create(long, const C3DVector&, bool) @ 0x006E0880.
// Globals: g_bCompileFrontendDefinitions @ 0x013B8648 (bool, as named by the landed boot code).
// Shape: the name is pushed before the database getter (arguments first, then the object expression); the typed
//   overload is called (not tail-jumped) and its result returned; one shared `xor eax,eax` exit.
class C3DVector { public: float X, Y, Z; };
class CCharString { public: void* PStringData; };
class CThing;

class CParticleEmitterDatabase {
public:
    static CParticleEmitterDatabase* __fastcall Get();     // 0x00AEAAE0
    long GetEmitterTemplateHandleFromName(const CCharString& name);   // 0x00AE85B0
};

extern bool g_bCompileFrontendDefinitions;                  // 0x013B8648

class CTCDParticleEmitter {
public:
    static CThing* __fastcall Create(long particleTypeID, const C3DVector& position, bool forceParticleToLoad);   // 0x006E0880
    static CThing* __fastcall Create(const CCharString& emitterName, const C3DVector& position, bool forceParticleToLoad);
};

CThing* __fastcall CTCDParticleEmitter::Create(const CCharString& emitterName, const C3DVector& position, bool forceParticleToLoad)
{
    if (!g_bCompileFrontendDefinitions) {
        long particleTypeID = CParticleEmitterDatabase::Get()->GetEmitterTemplateHandleFromName(emitterName);
        if (particleTypeID > 0)
            return Create(particleTypeID, position, forceParticleToLoad);
    }
    return 0;
}