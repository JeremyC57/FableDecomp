// NParticleEngine::CParticleEmitterManager::CreateInterpolatedEmitter(const CParticleEmitter*, long, float, const C3DVector&, const CRightHandedSet&, bool) -- at interpolate >= 1 just create the target emitter; otherwise fetch the target template and create the interpolated emitter between the two (null when the template is missing)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AEB850, length 110 bytes (padded to 112).
// Leaf: ?CreateInterpolatedEmitter@CParticleEmitterManager@NParticleEngine@@QAEPAVCParticleEmitter@2@PBV32@JMABVC3DVector@@ABVCRightHandedSet@@_N@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Callees: CParticleEmitterManager::CreateEmitter(long, const C3DVector&, bool, const CRightHandedSet&) @ 0x00AEB6D0,
// CParticleEmitterDatabase::GetEmitterTemplate(long, bool) @ 0x00AE89E0 on the static counted pointer 0x013D2E14,
// CreateInterpolatedEmitter(CParticleEmitter*, CParticleEmitter*, float, const C3DVector&, const CRightHandedSet&) @ 0x00AEB600.
// Signatures from the manifest (PDB): the three overloads differ in the first two parameters.
// Shape: `interpolate >= 1.0f` (0x0122DED8) gives `fcomp ; test ah,1 ; jne`; the forwarded arguments are re-read from their
// slots (right to left, so CreateEmitter's parameter order shows as the push order a4, a5, a3, a1); the database pointer is
// loaded into ecx through the static counted pointer's inline operator->.
#include "engine/C3DVector.h"
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
// NF158 (q4754 / q4954 family): in-place respell of the row's existing local model (same class token, now `class` in its PDB
//   namespace NParticleEngine) with the Ego_r.pdb / FableWin.pdb overload signatures (the emitters of 0x00AEB600 / 0x00AEB850 are
//   const): 0x00AEB6D0 Ego_r 0x00AAF9C0 / FableWin 0x02EF5EF0, 0x00AEB600 Ego_r 0x00AAF8F0 / FableWin 0x02EF5C00, 0x00AEB850 Ego_r
//   0x00AAFF80 / FableWin 0x02EF5DB0, 0x00AEB8C0 Ego_r 0x00AAFFF0 / FableWin 0x02EF5E50. Was the non-namespaced struct with non-const
//   emitter pointers.
namespace NParticleEngine {
class CParticleEmitterManager { public:
    CParticleEmitter* CreateEmitter(long handle, const C3DVector& position, bool flag, const CRightHandedSet& orientation);   // 0x00AEB6D0
    CParticleEmitter* CreateInterpolatedEmitter(const CParticleEmitter* from, const CParticleEmitter* to, float interpolate,
                                                const C3DVector& position, const CRightHandedSet& orientation);   // 0x00AEB600
    CParticleEmitter* CreateInterpolatedEmitter(const CParticleEmitter* from, long toHandle, float interpolate,
                                                const C3DVector& position, const CRightHandedSet& orientation, bool flag);   // 0x00AEB850
    CParticleEmitter* CreateInterpolatedEmitter(long fromHandle, long toHandle, float interpolate,
                                                const C3DVector& position, const CRightHandedSet& orientation, bool flag);   // 0x00AEB8C0
};
}
CParticleEmitter* NParticleEngine::CParticleEmitterManager::CreateInterpolatedEmitter(const CParticleEmitter* from, long toHandle, float interpolate,
                                                                     const C3DVector& position, const CRightHandedSet& orientation, bool flag)
{
    if (interpolate >= 1.0f)
        return CreateEmitter(toHandle, position, flag, orientation);
    const CParticleEmitter* to = g_ParticleEmitterDatabase->GetEmitterTemplate(toHandle, flag);
    if (to)
        return CreateInterpolatedEmitter(from, to, interpolate, position, orientation);
    return 0;
}