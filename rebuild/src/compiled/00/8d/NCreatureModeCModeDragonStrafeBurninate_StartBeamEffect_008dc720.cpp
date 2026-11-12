// NCreatureMode::CModeDragonStrafeBurninate::StartBeamEffect() -- start the dragon's strafing flame: find the mouth dummy of the
//   owner's graphic appearance, look up "BURNINATE_ATTACK" in the owner def's CSpecialEffectsDef sub-def, and when it is there stamp
//   LastExplodeFrame with the world frame, create the particle-emitter thing at the mouth into PBeamWeaponEffect, mark the flame
//   active, and (if the new thing is alive and has a particle emitter) attach the emitter to the owner's mouth dummy.
// Leaf: ?StartBeamEffect@CModeDragonStrafeBurninate@NCreatureMode@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DC720, 478 bytes, plain `ret` (private, void __fastcall, no arguments). Name from Ego_r.pdb and
//   FableWin.pdb (?StartBeamEffect@CModeDragonStrafeBurninate@NCreatureMode@@AAEXXZ). Caller: FrameUpdate 0x008DE080.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; NGraphicAppearance::CTCGraphicAppearance::
//   GetDummyObject 0x004C08C0; CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580 (out of line, real body from
//   engine/CDefClassBase_GetSubDef.h); CSpecialEffectsDef::GetEffectFromString 0x00669D30 (out of line, real body from
//   engine/CSpecialEffectsDef.h); CWorld::GetFrame 0x0049D870; CTCDParticleEmitter::Create(long, const C3DVector&, bool) 0x006E0880;
//   CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10 (CIntelligentPointer<CThing>
//   vtable 0x01238C6C); CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (inside the inline GetTC, type 0x4B);
//   CTCDParticleEmitter::AttachToThing 0x006E0BE0; the def's slot-1 Destroy on the last release.
// Strings: "DRAGON_MOUTH_DUMMY" 0x01285B4C (twice), "BURNINATE_ATTACK" 0x01285BA4.
// Shape: the twin of the landed CModeDragonFireBeam::StartBeamEffect 0x008DB620, without its early `if (active) return;`. No frame
//   pointer, no EH state. The counted copy of the owner's base def (GetPBaseDef by value) and the second name are conditional
//   temporaries of the && chain (ebx bits 1 / 2, the found bool at [esp+0Fh]). GetSubDef and GetEffectFromString are calls, but their
//   bodies are visible (as in retail's TU: the def headers), so the sub-def pointer and the effect index do not escape: effectType
//   shares one slot with the CIntelligentPointer temporary, the GetTC key and the second name. operator= reads the item into a
//   local before SetPItem; FlameActive is set right after the assignment, before the attach test. The sub-def is released at the
//   single exit (no early return).
// Layout: no shared header for the dragon mode classes (Ego_r layout minus 4, as the landed OnActivate 0x008DC6C0 / OnDeactivate
//   0x008DCB00 / UpdateBeam 0x008DC900 / ctor 0x008E1580): CModeBase 0x1c (shared, engine/CTCCreatureModeManager.h) ->
//   CModeDragonFlyingBase 0x38 -> CModeDragonFlying 0x94 -> Timer +0x94, PBeamWeaponEffect +0x98, LastExplodeFrame +0xA0,
//   FlameStartPos +0xA4, FlameEndPos +0xB0, FlameActive +0xBC, FlameStartPercentage +0xC0, FlameStopPercentage +0xC4.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODEBASE_PARENTTHING
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CThingBaseDef.h"
#include "engine/CWorld.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTCCreatureModeManager.h"

namespace NCreatureMode {
class CModeDragonFlyingBase : public CTCCreatureModeManager::CModeBase { public:
    unsigned char FlightSpline[0x14];      // +0x1c CCardinalSpline
    float         SplinePercentageComplete;   // +0x30
    float         LegAnimAlpha;            // +0x34
};
class CModeDragonFlying : public CModeDragonFlyingBase { public:   // retail sizeof 0x94
    unsigned char _flying[0x94 - 0x38];
};
class CModeDragonStrafeBurninate : public CModeDragonFlying { public:   // vtable 0x01268718
    long          Timer;                   // +0x94
    CIntelligentPointer<CThing> PBeamWeaponEffect;    // +0x98
    long          LastExplodeFrame;        // +0xa0
    C3DVector     FlameStartPos;           // +0xa4
    C3DVector     FlameEndPos;             // +0xb0
    bool          FlameActive;             // +0xbc
    float         FlameStartPercentage;    // +0xc0
    float         FlameStopPercentage;     // +0xc4
private:
    void StartBeamEffect();
};
}

void NCreatureMode::CModeDragonStrafeBurninate::StartBeamEffect()
{
    CDefPointer<const CSpecialEffectsDef> effects;
    long effectType = 0;
    C3DVector position;
    CRightHandedSet axes;
    ParentThing.GetTCGraphicAppearance()->GetDummyObject("DRAGON_MOUTH_DUMMY", 0, 0, position, axes, 1.0f);
    if (ParentThing.GetPBaseDef()->GetSubDef(&effects) && effects->GetEffectFromString("BURNINATE_ATTACK", effectType)) {
        LastExplodeFrame = CWorld::GetFrame();
        PBeamWeaponEffect = CIntelligentPointer<CThing>(CTCDParticleEmitter::Create(effectType, position, false));
        FlameActive = true;
        CTCDParticleEmitter* emitter;
        if (PBeamWeaponEffect.Get() && !PBeamWeaponEffect.Get()->IsDead()
            && PBeamWeaponEffect.Get()->GetTC(TCI_PARTICLE_EMITTER, &emitter)) {
            emitter->AttachToThing(ParentThing, 0x2a, "DRAGON_MOUTH_DUMMY", 0, 0.0f);
        }
    }
}