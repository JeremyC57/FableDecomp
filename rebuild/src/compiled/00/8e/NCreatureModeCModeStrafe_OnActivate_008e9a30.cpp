// NCreatureMode::CModeStrafe::OnActivate() -- activate the strafe mode: no active weapon, the STAND group filter, one own FrameUpdate, the
// base OnActivate, then the strafing animation for walking (name, distance and index), force no blend-in when an aiming combination anim
// (type 0x24) is playing, remove the combination anims of types 0x23 / 0x24 / 0x25 / 0x1F, and a player re-reads its movement speeds
// Leaf: ?OnActivate@CModeStrafe@NCreatureMode@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E9A30, 297 bytes, thiscall `ret`, virtual: slot 5 of the CModeStrafe vtable 0x01268588 (slot 3 is
//   its own FrameUpdate 0x008EA220).
// Identity: Ego_r.pdb ?OnActivate@CModeStrafe@NCreatureMode@@UAEXXZ (0x0090FFC0), FableWin.pdb 0x02AD8350.
// Callees: CTCCreatureModeManager::CModeBase::OnActivate 0x0073AAC0, NCreatureMode::GetStrafingAnimationName 0x008E8FF0 /
//   GetAnimDistanceAndIndexFromAnimName 0x008E9080 (Ego_r 0x0090F4A0 / 0x0090F530, FableWin 0x02AD6AC0 / 0x02AD6BA0), CAnimFilterOfGroup::
//   SetCurrentGroupName (inline; out of line at 0x0073DFF0), CVectorMap LowerBound 0x0040F020 (inline GetTC TCI_ANIMATION),
//   CTCAnimationComplex::GetAnimation 0x0070B460, CAnimateThing::GetFirstCombinationAnimFromType 0x0070C4E0 / RemoveCombinationAnimsOfType
//   0x0070D8C0, CThingPlayerCreature::InitialiseMovementSpeeds 0x006A9A00, CCharString 0x0099EBF0 / = 0x0099EFB0 / dtor 0x0099EAE0.
// Naming (NF128, 2026-11-20; q6394 / q6395 / q6454): moved off the BaseModeNative / ThingNative / AnimationGroupFilter / NativeStringValue
//   stand-ins (and the quarantine CThing included under a renaming macro) onto engine/NCreatureMode_CModeStrafe.h (overlay opt-in
//   FABLE_RETAIL_NCREATUREMODE_CMODESTRAFE_ONACTIVATE), the shared nested CTCCreatureModeManager::CModeBase (FABLE_RETAIL_CMODEBASE_VIRTUALS),
//   engine/CThing.h, engine/CThingPlayerCreature.h, engine/CAnimFilterOfGroup.h (overlay opt-in
//   FABLE_RETAIL_CANIMFILTEROFGROUP_SETCURRENTGROUPNAME_INLINE), engine/CTCAnimationComplex.h and engine/CAnimateThing.h.
// One definition (NF130, 2026-11-20; GOTCHAS nf80 / nf86 / nf88): this object emits the pick-any COMDAT of the inline
//   CAnimFilterOfGroup::SetCurrentGroupName; the owner 0x0073DFF0 now emits the same pick-any COMDAT (anonymous-namespace scaffolding
//   on the header body) instead of a strong copy. Proven by linking the two objects with LINK= cleared: LNK2005 before, none after.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE_MOVEMENTSPEEDS
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_NCREATUREMODE_CMODESTRAFE_PASS24
#define FABLE_RETAIL_NCREATUREMODE_CMODESTRAFE_ONACTIVATE
#define FABLE_RETAIL_CANIMFILTEROFGROUP_SETCURRENTGROUPNAME_INLINE
#define FABLE_RETAIL_LOOK_HEAD_ANIM
#define FABLE_RETAIL_CANIMSET_TYPE
#define FABLE_RETAIL_CANIMATETHING_MAINANIM
#define FABLE_RETAIL_CANIMATETHING_UPDATEANIMATION
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CAnimateThing.h"
#include "engine/CTCAnimationComplex.h"
#include "engine/NCreatureMode_CModeStrafe.h"

namespace NCreatureMode {

void CModeStrafe::OnActivate()
{
    ActiveWeapon = -2;
    {
        CCharString group("STAND");
        Filter.SetCurrentGroupName(group);
    }
    FrameUpdate();
    CTCCreatureModeManager::CModeBase::OnActivate();
    CCharString name = GetStrafingAnimationName(ParentThing, AI_MOVEMENT_NULL);
    CThing& parent = ParentThing;
    StrafeAnimName = name;
    GetAnimDistanceAndIndexFromAnimName((const CThingCreatureBase&)parent, name, AnimIndex, AnimMovementDistance);
    CTCAnimationComplex* animComplex;
    ParentThing.GetTC(TCI_ANIMATION, &animComplex);
    CAnimateThing& animation = animComplex->GetAnimation();
    if (animation.GetFirstCombinationAnimFromType(0x24))
        ForceAnimNoBlendIn = true;
    animation.RemoveCombinationAnimsOfType(0x23);
    animation.RemoveCombinationAnimsOfType(0x24);
    animation.RemoveCombinationAnimsOfType(0x25);
    animation.RemoveCombinationAnimsOfType(0x1f);
    if (ParentThing.GetType() == 2)
        ((CThingPlayerCreature&)ParentThing).InitialiseMovementSpeeds();
}

}
