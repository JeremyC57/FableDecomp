// CThingDeadCreature::Create(long defIndex, const C3DVector& position, long a3, const CCharString& scriptName, const CAnimFilterBase& filter) -- allocate a dead creature, construct it from the parameters, add its five standard components, take a clone of the animation filter, place its physics and activate it; null on any failure
// Flags: /O2 /Oy /W3 /Gr
// Leaf: ?Create@CThingDeadCreature@@SIPAV1@JABVC3DVector@@JABVCCharString@@ABVCAnimFilterBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00835C80, 419 bytes, static __fastcall, `ret 0Ch`. Ego_r.pdb / FableWin.pdb
//   ?Create@CThingDeadCreature@@SIPAV1@JABVC3DVector@@JABVCCharString@@ABVCAnimFilterBase@@@Z.
// Callees: CThing::operator new(size, bool) 0x004C7380 (0xF0 bytes), CThingDeadCreature::CThingDeadCreature 0x00835C20, the inline
//   CThingDeadCreature::ConstructFromParams over CThingGameObject::ConstructFromParams 0x008388D0 (0, 0, 0, true), CThing::AddTC 0x004C9D60
//   x5 (CTCPhysicsStandard, CTCGraphicAppearance, CTCAnimationComplex, CTCHitLocations, CTCSkeletalMorph), the inline SetAnimationFilter
//   (CAnimFilterBase::Clone slot 1, CCountedPointer<CAnimFilterBase>::Reset 0x00836580), CTCParamsPhysicsBase 0x006A06E0 with the
//   default (identity) CRightHandedSet, the physics component's ConstructFromParams (CTCBase slot 3), ~CBaseClassNonCopyable 0x0099A300,
//   CThing::DoActivationSequence 0x004C9CA0.
// NF80 (q10920): moved off the stand-in namespace ExternalRetry_00835c80 (local C3DVector, CCharString, CRightHandedSet, CBaseClass /
//   CBaseClassNonCopyable, CTCParamsPhysicsBase, CAnimFilterBase, CCountedPointer, CCPPointerInfo and CThingDeadCreature models) onto
//   engine/CThingDeadCreature.h (the overlay's FABLE_RETAIL_CTHINGDEADCREATURE_CREATE), CThingGameObject.h (_LEVELSCRIPT), CThing.h
//   (_OPERATORNEW), CTCPhysicsBase.h, CTCParamsPhysicsBase.h and CRightHandedSet.h (_DEFAULT_IDENTITY): the 0x00703210 CThingObject::Create
//   recipe.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_OPERATORNEW
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGGAMEOBJECT_LEVELSCRIPT
#define FABLE_RETAIL_CTHINGDEADCREATURE_ANIMATION
#define FABLE_RETAIL_CTHINGDEADCREATURE_CREATE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_DEFAULT_IDENTITY
#define FABLE_RETAIL_CTCPARAMSPHYSICSBASE_CTOR_BODY
#include "engine/CThing.h"
#include "engine/CThingDeadCreature.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCParamsPhysicsBase.h"

CThingDeadCreature* __fastcall CThingDeadCreature::Create(long defIndex, const C3DVector& position, long a3, const CCharString& scriptName,
                                                          const CAnimFilterBase& filter)
{
    CThingDeadCreature* result = 0;
    CThingDeadCreature* creature = new (false) CThingDeadCreature;
    if (creature && creature->ConstructFromParams(defIndex, position, a3, scriptName)) {
        creature->AddTC("CTCPhysicsStandard", false, 0);
        creature->AddTC("CTCGraphicAppearance", false, 0);
        creature->AddTC("CTCAnimationComplex", false, 0);
        creature->AddTC("CTCHitLocations", false, 0);
        creature->AddTC("CTCSkeletalMorph", false, 0);
        creature->SetAnimationFilter(filter);
        creature->GetPhysicsTC()->ConstructFromParams(CTCParamsPhysicsBase(position, CRightHandedSet()));
        if (creature->DoActivationSequence())
            result = creature;
    }
    return result;
}
