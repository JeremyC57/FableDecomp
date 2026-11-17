// CHitParameters::CHitParameters(const CHitParameters&) -- the memberwise copy constructor (Ego_r.pdb 0x00807C70)
// Leaf: ??0CHitParameters@@QAE@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00812170, 266 bytes; caller NEntityEvents::CEventHitBase::CEventHitBase 0x00811760 (call 0x00811797).
// Port (NF78, round 7; q10948 binder): the class from engine/CHitParameters.h (VECTOR_MEMBERS: packed C3DVector Position +2 / Direction
//   +0x0E; DAMAGEMAP: std::map<EDamageAttribute,float> +0x30; AUGMENTATIONS: std::vector<EObjectAugmentationType> +0x3C) with the NF78
//   overlay opt-in FABLE_RETAIL_CHITPARAMETERS_COPY_CTOR (the declaration); the local CHitParameters / DamageMap / AugmentationVector /
//   ThingPointer / MaterialPointer stand-ins are gone.
// Callees: map<EDamageAttribute,float>::map(const map&) 0x004F97B3 (STLport COMDAT, FABLE_RETAIL_STLP_MAP_COPY_OUTOFLINE),
//   vector<EObjectAugmentationType>::vector(const vector&) 0x00811D40 (out of line, FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE),
//   CBaseIntelligentPointer() 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 (CIntelligentPointer<CThing> copy, inline;
//   vtable 0x01238C6C). CDefPointer<const CMaterialDef> copy inline (refcount +4 when non-null).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_MAP_COPY_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE
#define FABLE_RETAIL_CHITPARAMETERS_VECTOR_MEMBERS
#define FABLE_RETAIL_CHITPARAMETERS_DAMAGEMAP
#define FABLE_RETAIL_CHITPARAMETERS_AUGMENTATIONS
#define FABLE_RETAIL_CHITPARAMETERS_COPY_CTOR
#include "fable_compiler.h"
#include <stddef.h>
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CMaterialDef.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CHitParameters.h"

CHitParameters::CHitParameters(const CHitParameters& other)
    : Localised(other.Localised)
    , Directional(other.Directional)
    , Position(other.Position)
    , Direction(other.Direction)
    , KnockDown(other.KnockDown)
    , Decapitate(other.Decapitate)
    , Blockable(other.Blockable)
    , Flourish(other.Flourish)
    , EpicSpell(other.EpicSpell)
    , BlockCounter(other.BlockCounter)
    , PlayHitResponse(other.PlayHitResponse)
    , PlayHitResponseOverrideSet(other.PlayHitResponseOverrideSet)
    , MoveBack(other.MoveBack)
    , CreateParticleEffectOnHit(other.CreateParticleEffectOnHit)
    , CreateDustParticleEffectOnHit(other.CreateDustParticleEffectOnHit)
    , GuaranteeHit(other.GuaranteeHit)
    , Blocked(other.Blocked)
    , HitNegated(other.HitNegated)
    , CauseRecoil(other.CauseRecoil)
    , ShotBuildupTime(other.ShotBuildupTime)
    , DamageMap(other.DamageMap)
    , Augmentations(other.Augmentations)
    , PHitLocation(other.PHitLocation)
    , PHitMaterialDef(other.PHitMaterialDef)
    , PThingHitting(other.PThingHitting)
    , PWeapon(other.PWeapon)
{
}
