// CTCTargetingPlayer::UpdateTargetedPositionRandomWalk() -- while a ranged weapon is drawn, random-walk the targeted position: a new random second-order direction, the 2D offset advanced by its delta, and the 3D offset rebuilt in the plane across the camera-to-target line, scaled by the pull-back amount and the hero's shaky-cursor stats
// Grade: DIFFER(1108v1108) -- dl=15 (try i1, 30 grades), same length, scheduling only: retail stores the sine copy (`mov [esi+0CCh],edx`) after `fcos`, ours before it; retail issues `lea eax,[esi+9Ch] ; mov ecx,eax ; pop edi` right after the first `fld` of the offset sum, ours three x87 instructions later (try k0, a `C3DVector&` to TargetedPosOffset3D, moves the lea before the 2D offset update instead: dl=14)
// Leaf: ?UpdateTargetedPositionRandomWalk@CTCTargetingPlayer@@IAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00803540, 1,108 bytes, thiscall `ret`, 0x44-byte frame, no EH frame.
// Identity: Ego_r.pdb / FableWin.pdb ?UpdateTargetedPositionRandomWalk@CTCTargetingPlayer@@IAEXXZ (protected). Ego_r donor order
//   GetPBestThingForMeleeAttack 0x007FE510, UpdateTargetThreatening 0x007FE5D0, UpdateTargetedPositionRandomWalk 0x007FE840,
//   PostProcessTargetedThing 0x007FED60 matches retail 0x008031E0 .. 0x00803540 .. 0x00803B20; the body writes the PDB members
//   TargetedPosOffset3D +0x9C, TargetedPosOffset +0xB4 (+= DeltaTargetedPosOffset +0xBC) and TargetedPosOffsetSecondOrder +0xCC and
//   reads TargetedPos +0x84; called from CTCTargetingPlayer::FrameUpdate at 0x00805814. Catalog "CViewBase_UpdateSeaLevelParticles"
//   (forge autoname) is wrong.
// Callees: CVectorMap LowerBound 0x0040F020 (GetTC<CTCHeroRangedAttacks>, interface 129) / const 0x004365B0 (PeekTC<const
//   CTCHeroExperience>, 104), CTCHeroRangedAttacks::GetPullBackAmount 0x00789A70 / GetShotAccuracyFraction 0x00789A80 (its value
//   unused, the call kept), CTCBase::GetWorld 0x00686A70, CAIGameCameraBase::GetCameraManager(const CGameCameraManager**) const
//   (slot 10, +0x28), CThingCreatureBase::GetPHeroCombatDef 0x00663340 (twice; CDefPointer temporaries), CHeroAbilityStatWeight::
//   GetParameter 0x00851BD0, C3DVector::AccurateNormalise 0x00A14440 (twice), CWorld::GetWorldSeed 0x0049D810, GFFloatRandom
//   0x004978A0 (twice). Constants: DELTA_NOTIONAL_ZERO 0x0129BA3C, 1.0f, 1/7 0x012722EC, 0.1f 0x0122EE58, -1.0f 0x0122DEE0,
//   TWO_PI 0x0124F2B8 (double), 0.0f 0x0122DEDC.
// Shape: the distance test is `!(distance < DELTA_NOTIONAL_ZERO)` (`test ah,5 ; jnp` to the exit; `>=` gives `test ah,1 ; jne`);
//   `C3DVector perpendicular(0.0f, 0.0f, 0.0f)` (the dead X/Y zeroes drop, Z's stays ahead of the compare) gives retail's operand
//   order in the inlined operator^; named sine / cosine floats give retail's Delta-first `TargetedPosOffset += DeltaTargetedPosOffset`
//   (a direct C2DVector(sin, cos) temporary flips the X add). The 3D offset's first term is multiplied by a literal 0.0f.
// Headers: overlay branches FABLE_RETAIL_CTCTARGETINGPLAYER (CTCTargetingPlayer : CTCTargetingBase, typed members),
//   FABLE_RETAIL_CTCHERORANGEDATTACKS, FABLE_RETAIL_CHEROCOMBATDEF_RANGED (+ new CFloatRange.h / CHeroAbilityStatWeight.h) and
//   FABLE_RETAIL_CTHINGCREATUREBASE_HEROCOMBATDEF (GetPHeroCombatDef).
// Next idea: the two residues are list-scheduler placements of integer ops among x87 ops in one block; try a C2DVector
//   returning helper for the sine/cosine pair (FableWin has no such member: InitialiseFromAngles(float, float) is the nearest) or
//   a different split of the two TargetedPosOffset3D statements, and compare the x87 dependency chains instruction by instruction.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CGAMECAMERAMANAGER
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_HEROCOMBATDEF
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CHEROCOMBATDEF_RANGED
#define FABLE_RETAIL_CTCHERORANGEDATTACKS
#define FABLE_RETAIL_CTCTARGETINGPLAYER
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CTCBase.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CWorld.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CCamera.h"
#include "engine/CGameCameraManager.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CHeroCombatDef.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CTCHeroRangedAttacks.h"
#include "engine/CTCTargetingPlayer.h"

float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
extern const double TWO_PI;                                          // 0x0124F2B8

void CTCTargetingPlayer::UpdateTargetedPositionRandomWalk()
{
    CTCHeroRangedAttacks* ranged_attacks;
    if (GetParentThing().GetTC(&ranged_attacks)) {
        float pull_back_amount = ranged_attacks->GetPullBackAmount();
        float accuracy = ranged_attacks->GetShotAccuracyFraction();
        float shake = pull_back_amount;
        const CGameCameraManager* camera_manager;
        GetWorld().PeekGameCamera()->GetCameraManager(&camera_manager);
        C3DVector direction = TargetedPos - camera_manager->GetCamera().PeekWorldPos();
        float distance = direction.GetMagnitude();
        if (!(distance < DELTA_NOTIONAL_ZERO)) {
            direction /= distance;
            distance *= 1.0f / 7.0f;
            float radius = distance * 0.1f;
            CThing& thing = GetParentThing();
            const CTCHeroExperience* experience;
            if ((thing.GetTypeFlags() & 8) && thing.PeekTC(&experience)) {
                const CThingCreatureBase& creature = static_cast<const CThingCreatureBase&>(thing);
                if (!creature.GetPHeroCombatDef().IsNull()) {
                    const CHeroCombatDef* def = creature.GetPHeroCombatDef().Get();
                    float parameter = def->RangedAccuracyShakeyCursorWeighting.GetParameter(*experience);
                    shake = ((def->RangedAccuracyShakeyCursor.HighValue - def->RangedAccuracyShakeyCursor.LowValue) * parameter
                             + def->RangedAccuracyShakeyCursor.LowValue) * pull_back_amount;
                    radius = distance * def->RangedAccuracyShakeFractionOfRadius;
                }
            }
            C3DVector perpendicular(0.0f, 0.0f, 0.0f);
            if (fabs(direction.X) > fabs(direction.Y)) {
                perpendicular.Y = 1.0f;
                perpendicular.X = direction.Y / direction.X * -1.0f;
            } else {
                perpendicular.X = 1.0f;
                perpendicular.Y = direction.X / direction.Y * -1.0f;
            }
            perpendicular.AccurateNormalise();
            C3DVector up = perpendicular ^ direction;
            up.AccurateNormalise();
            float angle = GFFloatRandom(1.0f, GetWorld().GetWorldSeed());
            float amount = GFFloatRandom(radius, GetWorld().GetWorldSeed());
            float sine = (float)sin(angle * TWO_PI);
            float cosine = (float)cos(angle * TWO_PI);
            TargetedPosOffsetSecondOrder = C2DVector(sine, cosine);
            TargetedPosOffset += DeltaTargetedPosOffset;
            TargetedPosOffset3D = (perpendicular * TargetedPosOffset.X + up * TargetedPosOffset.Y) * distance * pull_back_amount * 0.0f;
            TargetedPosOffset3D += up * TargetedPosOffsetSecondOrder.Y * amount * shake;
        }
    }
}
