// CThingPlayerCreature::SetActionForMelee(const CCreatureAbilityDef& ability) -- pick the hero's melee direction and target
// Leaf: ?SetActionForMelee@CThingPlayerCreature@@UAE_NABVCCreatureAbilityDef@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006AABE0, 1278 bytes, `ret 4`; Ego_r ?SetActionForMelee@CThingPlayerCreature@@UAE_NABVCCreatureAbilityDef@@@Z.
// Callees: CCombatManager::GetBestEnemyForCombatant 0x006EB700 / IsTargetOverriddenByAttemptedFacingDirection 0x006EA5F0 /
//   GetDefenderZone 0x006E99B0, C3DVector::AccurateNormalise 0x00A14440 / RotateXY 0x00A14040, NTargeting::GetTargetingPositionOnThing
//   0x007EA2F0, GetCurrentRelativeMovementAngleXY 0x006A9640, SetPlayerCreatureActionForMelee 0x006AA720, CThing::GetPlayer 0x004C7A30,
//   GetWorld 0x004C7AA0, GetPos 0x004C73D0, CTCCarrying::GetThingInPrimarySlot 0x006A1440, CTCWeapon::IsUsableBy 0x005DAB10.
// Levers: C3DVector::GetSquaredMagnitude2D / IsNonZero as running statements (retail loads X, Y and X, Z, Y in source order),
//   CTCPhysicsBase::GetFacingVector() at slot 70 (chunk 718's overload order), the camera vector copied then Z zeroed, a named angle.
// Global 0x01375750 g_MeleeUsesCameraDirection is descriptive (one reader; a console bool initialised to 1; no PDB data name on disk).
// Grade: RELOCATION_MATCH (chunk 732, attempts s1..s10 plus header probes v1..v23).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCTARGETED
#define FABLE_RETAIL_CCOMBATMANAGER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CGAMECAMERAMANAGER
#define FABLE_RETAIL_PLAYER
#include "fable_string.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CTCTargeted.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCWeapon.h"
#include "engine/CCombatManager.h"
#include "engine/CWorld.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CGameCameraManager.h"
#include "engine/CCamera.h"
#include "engine/CPlayer.h"
#include "engine/NTargeting.h"

extern bool g_MeleeUsesCameraDirection;          // 0x01375750 descriptive (one reader, this row; a console bool, initialised 1)
extern const float DELTA_NOTIONAL_ZERO;          // 0x0129BA3C ?DELTA_NOTIONAL_ZERO@@3MB (1e-4f)

template<class T> inline bool __fastcall IS_THING_ALIVE(const T& thing) { return thing && !thing->IsDead(); }   // FableWin ??$IS_THING_ALIVE@PAVCThing@@@@YI_NABQAVCThing@@@Z

bool CThingPlayerCreature::SetActionForMelee(const CCreatureAbilityDef& ability)
{
    const CTCTargetingBase* targeting;
    PeekTC(&targeting);
    CThing* target = targeting->GetPBestThingForMeleeAttack();
    C3DVector last_acceleration = LastRelativeMovementAcceleration;
    ECombatZone zone = (ECombatZone)2;
    C3DVector facing = PhysicsTC->GetFacingVector();
    bool enemy_in_facing = false;
    if (IS_THING_ALIVE(GetCombatManager().GetBestEnemyForCombatant(*this, facing)))
        enemy_in_facing = true;
    C3DVector direction = facing;
    if (g_MeleeUsesCameraDirection) {
        CGameCameraManager* manager;
        if (GetWorld().GetGameCamera()->GetCameraManager(&manager)) {
            C3DVector camera_direction = manager->GetCamera().PeekViewDirection();
            camera_direction.Z = 0.0f;
            if (camera_direction.GetSquaredMagnitude2D() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO) {
                camera_direction.AccurateNormalise();
                if (IS_THING_ALIVE(GetCombatManager().GetBestEnemyForCombatant(*this, camera_direction)) || !enemy_in_facing)
                    direction = camera_direction;
            }
        }
    }
    if (GetPlayer().IsZTargeting() && IS_THING_ALIVE(target)) {
        direction = target->GetPos() - GetPos();
        direction.Z = 0.0f;
        if (direction.GetSquaredMagnitude2D() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO)
            direction.AccurateNormalise();
    } else if (!GetPlayer().IsZTargeting()) {
        if (last_acceleration.IsNonZero()) {
            direction = last_acceleration;
            float angle = GetCurrentRelativeMovementAngleXY();
            direction.RotateXY(angle);
        }
    }
    CThing* best = GetCombatManager().GetBestEnemyForCombatant(*this, direction);
    if (!IS_THING_ALIVE(best) && IS_THING_ALIVE(target)) {
        const CTCTargeted* targeted;
        if (!GetCombatManager().IsTargetOverriddenByAttemptedFacingDirection(*this, *target, direction)
            || (target->PeekTC(&targeted) && targeted->IsZTargetable()))
            best = target;
    }
    const CTCCarrying* carrying;
    PeekTC(&carrying);
    CThing* weapon = carrying->GetThingInPrimarySlot();
    C3DVector final_direction;
    CTCWeapon* weapon_tc;
    if (IS_THING_ALIVE(weapon) && weapon->GetTC(&weapon_tc) && !weapon_tc->IsUsableBy(*this)) {
        final_direction = PhysicsTC->GetFacingVector();
        final_direction.Z = 0.0f;
        if (final_direction.GetSquaredMagnitude2D() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO)
            final_direction.AccurateNormalise();
    } else if (IS_THING_ALIVE(best)) {
        if (best->GetTypeFlags() & 8)
            zone = GetCombatManager().GetDefenderZone(*static_cast<CThingCreatureBase*>(best), *this, 0);
        C3DVector target_pos;
        if (best != targeting->GetPTargetedThing() || !targeting->GetTargetedPos(target_pos, 1.0f))
            target_pos = NTargeting::GetTargetingPositionOnThing(GetWorld(), *best, 1.0f);
        final_direction = target_pos - GetPos();
        final_direction.Z = 0.0f;
        if (final_direction.GetSquaredMagnitude2D() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO)
            final_direction.AccurateNormalise();
    } else
        final_direction = direction;
    SetPlayerCreatureActionForMelee(zone, final_direction, best, ability);
    return true;
}