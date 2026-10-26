// CParticleEmitterManager::CreateInterpolatedEmitter(long, long, float, const C3DVector&, const CRightHandedSet&, bool) -- at interpolate <= 0 just create the source emitter; otherwise fetch the source template and hand over to the template overload (null when the template is missing)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AEB8C0, length 118 bytes (padded to 128).
// Leaf: ?CreateInterpolatedEmitter@CParticleEmitterManager@@QAEPAVCParticleEmitter@NParticleEngine@@JJ
// Callees: CParticleEmitterManager::CreateEmitter @ 0x00AEB6D0, CParticleEmitterDatabase::GetEmitterTemplate(long, bool)
// @ 0x00AE89E0 on the static counted pointer 0x013D2E14, the template overload CreateInterpolatedEmitter(CParticleEmitter*,
// long, float, const C3DVector&, const CRightHandedSet&, bool) @ 0x00AEB850.
// Shape: `interpolate <= 0.0f` (0x0129F198) gives `fcomp ; test ah,41h ; jp`; the flag is kept in esi for both calls.
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };
namespace NParticleEngine { class CParticleEmitter; }
typedef NParticleEngine::CParticleEmitter CParticleEmitter;
struct CParticleEmitterDatabase {
    CParticleEmitter* GetEmitterTemplate(long handle, bool flag);   // 0x00AE89E0
};
struct SCountBlock;
struct CEmitterDatabasePointer {           // static CCountedPointer<CParticleEmitterDatabase>
    CParticleEmitterDatabase* Object;
    SCountBlock*              Block;
    CParticleEmitterDatabase* operator->() const { return Object; }
};
extern CEmitterDatabasePointer g_ParticleEmitterDatabase;   // 0x013D2E14
struct CParticleEmitterManager {
    CParticleEmitter* CreateEmitter(long handle, const C3DVector& position, bool flag, const CRightHandedSet& orientation);   // 0x00AEB6D0
    CParticleEmitter* CreateInterpolatedEmitter(CParticleEmitter* from, CParticleEmitter* to, float interpolate,
                                                const C3DVector& position, const CRightHandedSet& orientation);   // 0x00AEB600
    CParticleEmitter* CreateInterpolatedEmitter(CParticleEmitter* from, long toHandle, float interpolate,
                                                const C3DVector& position, const CRightHandedSet& orientation, bool flag);   // 0x00AEB850
    CParticleEmitter* CreateInterpolatedEmitter(long fromHandle, long toHandle, float interpolate,
                                                const C3DVector& position, const CRightHandedSet& orientation, bool flag);   // 0x00AEB8C0
};
CParticleEmitter* CParticleEmitterManager::CreateInterpolatedEmitter(long fromHandle, long toHandle, float interpolate,
                                                                     const C3DVector& position, const CRightHandedSet& orientation, bool flag)
{
    if (interpolate <= 0.0f)
        return CreateEmitter(fromHandle, position, flag, orientation);
    CParticleEmitter* from = g_ParticleEmitterDatabase->GetEmitterTemplate(fromHandle, flag);
    if (from)
        return CreateInterpolatedEmitter(from, toHandle, interpolate, position, orientation, flag);
    return 0;
}