// NThingShot::CTCShotDefinitionDrainLife::InitAttachedEffects() -- create the shot def's attached continuous effect at the shot, keep it as the shot's effect, attach its particle emitter to the shot at "weapon_pos_a" and seed the emitter's MOVE_DIRECTION with the shot's facing
// Leaf: ?InitAttachedEffects@CTCShotDefinitionDrainLife@NThingShot@@MAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BE4E0, 256 bytes (plain `ret`). NThingShot::CTCShotDefinitionDrainLife vtable 0x012403F4 slot 34
//   (Ego_r.pdb ?InitAttachedEffects@CTCShotDefinitionDrainLife@NThingShot@@MAEXXZ, protected virtual): the byte twin of
//   CTCShotDefinitionForcePushBall::InitAttachedEffects 0x007BE3E0, the same source. Frame-pointer size TU: optimize("y", off) + ("s", on), as the landed shot rows.
// Callees: CThing::GetPos 0x004C73D0, CTCDParticleEmitter::Create 0x006E0880 (__fastcall), IS_THING_ALIVE<CThing*> 0x0040DCE0,
//   CIntelligentPointer<CThing>(CThing*) 0x004A96F0 / operator= 0x004A9760 / ~ 0x00A01C10, CThing::GetTCDriver<CTCDParticleEmitter>
//   0x004ACB50, CCharString 0x0099EBF0 / ~ 0x0099EAE0, CTCDParticleEmitter::AttachToThing 0x006E0BE0, CCRC::Calc 0x004014A0
//   ("MOVE_DIRECTION", 14), CTCPhysicsBase::GetFacingVector() const (virtual slot 70), CParticleUpdateParameters ctor 0x006E24D0 /
//   dtor 0x006E2500, vector<pair<unsigned long, C3DVector> >::push_back 0x0076B120, CTCDParticleEmitter::SetEmitterUpdateParameters 0x006E11A0.
// Model: the landed sibling SetSoulEffect 0x007BE05C (retail CThing / CTCBase / CIntelligentPointer headers, scoped pointer temporary: the
//   frame and the edi zero register follow); the push_back body is the one the landed CTCShotDefinitionBase::InitAttachedEffects 0x007C25A0
//   defines in its TU (auto_inline(off)): with it visible VC7.1 stores the {key, facing} pair as retail does (X, Y, key, Z).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CARRAY_PUSHBACK_NOINLINE
#define FABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_CTOR_OUTOFLINE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTCDRIVER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTCPHYSICSBASE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCDriverBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CIntelligentPointer.h"
class CCRC { public: static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length); };   // 0x004014A0
// NF55 (2026-11-17): the parameter block is the shared NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long>
//   (engine/NParticleEngine_CParticleUpdateParameters.h: CBaseClass + VectorParams / UpdateParams CVectorMaps; Ego_r.pdb / FableWin.pdb
//   ??0?$CParticleUpdateParameters@KK@NParticleEngine@@QAE@XZ 0x006E24D0) and the parameter pushes CArray<pair<unsigned long, C3DVector> >::push_back
//   (Ego_r.pdb 0x0077A190 + FableWin.pdb 0x025F4840, out of line at 0x0076B120; FABLE_RETAIL_CARRAY_PUSHBACK_NOINLINE keeps its body visible
//   as the old same-TU stand-in body did). They were the local SVectorParam / CVectorParamVector / CVectorParamMap / CParticleUpdateParameters
//   stand-ins.
#include "engine/NParticleEngine_CParticleUpdateParameters.h"
class CTCDParticleEmitter : public CTCDriverBase {                  // no shared header
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_PARTICLE_EMITTER; }
    static CThing* __fastcall Create(long type, const C3DVector& position, bool flag);    // 0x006E0880
    void AttachToThing(CThing& thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
    void SetEmitterUpdateParameters(const NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long>& parameters);   // 0x006E11A0
};
template <class T> inline bool __fastcall IS_THING_ALIVE(const T& thing)
{
    return &*thing && !thing->IsDead();
}
class CThingShotDef { public:                          // retail offsets (PDB minus 4)
    unsigned char _pad00[0x48];
    long          AttachedContinuousEffect;             // +0x48 (PDB 0x4c)
};
namespace NThingShot {
class CTCShotDefinitionDrainLife : public CTCBase {
protected:
    virtual void InitAttachedEffects();
private:
    CIntelligentPointer<CThing> PEffect;                // +0x0c (CTCShotDefinitionBase)
    unsigned char _pad14[0x38 - 0x14];
    const CThingShotDef* PShotDef;                      // +0x38 CDefPointer<const CThingShotDef>
};
}

void NThingShot::CTCShotDefinitionDrainLife::InitAttachedEffects()
{
    long effectType = PShotDef->AttachedContinuousEffect;
    if (effectType) {
        CThing* effect = CTCDParticleEmitter::Create(effectType, GetParentThing().GetPos(), false);
        if (IS_THING_ALIVE(effect)) {
            {
                CIntelligentPointer<CThing> effectPointer(effect);
                PEffect = effectPointer;
            }
            CTCDParticleEmitter* emitter = 0;
            effect->GetTCDriver(&emitter);
            CCharString helperName("weapon_pos_a");
            emitter->AttachToThing(GetParentThing(), 0x3c, helperName, 0, 0.0f);
            unsigned long key = CCRC::Calc(0, "MOVE_DIRECTION", 14);
            std::pair<unsigned long, C3DVector> moveDirection(key, GetParentThing().GetPhysicsTC()->GetFacingVector());
            NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long> parameters;
            parameters.VectorParams.push_back(moveDirection);
            emitter->SetEmitterUpdateParameters(parameters);
        }
    }
}