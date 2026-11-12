// CTCLook::UpdateTurningAnims() -- rebuild the table of turning animations: collect the appearance's "TURNING_" animations for the
//   current creature mode's animation filter and its default (weapon-based) filter, or with no group filter when none match; keep
//   each animation that rotates the creature (|rotation| > DELTA_NOTIONAL_ZERO) with its rotation, and track the smallest signed
//   clamped rotation (|r| clamped to 0.5 - DELTA_NOTIONAL_ZERO) as TurningViaAnimMinAngle (reset to 0.5 first).
// Leaf: ?UpdateTurningAnims@CTCLook@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00754050, 588 bytes, __fastcall, plain `ret`, no EH frame.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*,..>::LowerBound 0x0040F020 (GetTC(TCI_ENTITY_MODE_MANAGER) expanded),
//   CTCCreatureModeManager::GetCurrentMode 0x0073A6E0 (x3), CModeBase vslot 1 GetType (+4, result unused) and vslot 4 GetAnimFilter
//   (+0x10), CModeBase::GetDefaultAnimFilter (inline: mode+8), vector<pair<float,const CAnimationEntry*> >::clear 0x00755320 (out of
//   line) and ::_M_insert_overflow 0x00755350 (__false_type), CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0
//   (one "TURNING_" 0x0125AB08 temporary per call), CAnimationSet::GetAnimsWithNameStartingWith 0x005DCB00 (x3),
//   CAnimationEntry::GetRotationInAnimation 0x005DBFD0, free 0x00BFEA14 (vector dtor). Globals: DELTA_NOTIONAL_ZERO 0x0129BA3C;
//   CAnimFilterNoGroup vtable 0x0122FFDC / CAnimFilterBase 0x0122FFCC (inline temporary filter).
// Layout (engine/CTCLook.h): PAppearanceDef +0x18, TurningAnims +0x8C, TurningViaAnimMinAngle +0x98; CAppearanceDef::Animation +0x34.
// Overlay opt-ins: FABLE_RETAIL_CMODEBASE_DEFAULTANIMFILTER, FABLE_RETAIL_CANIMATIONENTRY_GETROTATION, FABLE_RETAIL_STLP_CONSTRUCT_BYVAL.
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CLOOKDEF
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NOT_EQUAL
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_LOOK_HEAD_ANIM
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CANIMATIONSET_QUERIES
#define FABLE_RETAIL_CANIMFILTER
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODEBASE_DEFAULTANIMFILTER
#define FABLE_RETAIL_CANIMATIONENTRY_GETROTATION
#define FABLE_RETAIL_STLP_CONSTRUCT_BYVAL
#define FABLE_RETAIL_STLP_VECTOR_CLEAR_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
#include <math.h>
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CTCLook.h"
#include "engine/CThing.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAppearanceDef.h"
#include "engine/CAnimFilterNoGroup.h"
#include "engine/CTCCreatureModeManager.h"
extern const float DELTA_NOTIONAL_ZERO;                      // 0x0129BA3C

void CTCLook::UpdateTurningAnims()
{
    CTCCreatureModeManager* modeManager;
    if (!GetParentThing().GetTC(TCI_ENTITY_MODE_MANAGER, &modeManager))
        return;
    if (PAppearanceDef.IsNull())
        return;
    modeManager->GetCurrentMode().GetType();
    TurningAnims.clear();
    TurningViaAnimMinAngle = 0.5f;
    std::vector<const CAnimationEntry*> anims;
    PAppearanceDef->Animation.GetAnimsWithNameStartingWith("TURNING_", anims, modeManager->GetCurrentMode().GetAnimFilter());
    PAppearanceDef->Animation.GetAnimsWithNameStartingWith("TURNING_", anims, modeManager->GetCurrentMode().GetDefaultAnimFilter());
    if (anims.empty())
        PAppearanceDef->Animation.GetAnimsWithNameStartingWith("TURNING_", anims, CAnimFilterNoGroup());
    for (std::vector<const CAnimationEntry*>::iterator it = anims.begin(); it != anims.end(); ++it) {
        const CAnimationEntry* anim = *it;
        float rotation;
        anim->GetRotationInAnimation(rotation);
        if (fabs(rotation) > DELTA_NOTIONAL_ZERO) {
            TurningAnims.push_back(std::pair<float, const CAnimationEntry*>(rotation, anim));
            float size = fabs(rotation);
            float maxRotation = 0.5f - DELTA_NOTIONAL_ZERO;
            if (size > maxRotation)
                size = maxRotation;
            long sign = -1;
            if (!(rotation < 0.0f))
                sign = 1;
            rotation = sign * size;
            if (fabs(rotation) < TurningViaAnimMinAngle)
                TurningViaAnimMinAngle = fabs(rotation);
        }
    }
}