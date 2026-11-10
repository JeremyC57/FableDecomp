// CWorld::EAUseProjectileWeapon(const CGameEvent*) -- toggles first-person projectile-weapon mode for the event's player and plays the zoom-in camera sound
// Grade: DIFFER(483v483) -- frame slot only (masked diff 4 lines): retail packs the camera-manager out-local with `ranged` at F+0 (0x0062C09A `lea eax,[esp+10h]`), ours with the this-spill. Packer trace: ranged, this and camera each have IL weight 2, in FIFO order ranged, this, camera, and camera joins the latest free slot (this). Retail needs this >= 3 or camera >= 3 (then camera joins ranged); no code-identical spelling tried adds that reference (10 probes: named game-camera, function-scope camera, const view, tested bool, do/while, duplicated arm tails)
// Retail: 0x0062BEF0 (483 bytes), FableWin.pdb / Ego_r.pdb ?EAUseProjectileWeapon@CWorld@@QAEXPBVCGameEvent@@@Z
// Leaf: ?EAUseProjectileWeapon@CWorld@@QAEXPBVCGameEvent@@@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_REMOVEMODE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_EVENTACTIONS
#define FABLE_RETAIL_CTCCARRYING
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCSPECIALABILITIES
#define FABLE_RETAIL_CTCHERORANGEDATTACKS
#define FABLE_RETAIL_CTCHERORANGEDATTACKS_PROJECTILEMODE
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CGAMECAMERAMANAGER_CAMERASOUND
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayer.h"
#include "engine/CTCBase.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCWeapon.h"
#include "engine/CTCHeroRangedAttacks.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CGameCameraManager.h"
#include "engine/CGameEvent.h"
#include "engine/EPlayerMode.h"
#include "engine/CWorld.h"

// ??$IS_THING_ALIVE@PAVCThing@@@@YI_NABQAVCThing@@@Z (Ego_r.pdb / FableWin.pdb), out of line in this TU (0x0040DCE0).
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);


void CWorld::EAUseProjectileWeapon(const CGameEvent* event)
{
    {
        CPlayer* viewer = GetPlayer(event->Player);
        if (viewer->HasMode(PLAYER_MODE_VIEW_HERO))
            return;
    }
    CPlayer* player = GetPlayer(event->Player);
    if (!player)
        return;
    CThingPlayerCreature* hero = player->GetPControlledCreature();
    if (!hero || hero->IsDead())
        return;
    {   // the ranged/camera scopes are disjoint: retail packs both into one frame slot
    CTCHeroRangedAttacks* ranged;
    if (!hero->GetTC(TCI_HERO_RANGED_ATTACKS, &ranged))
        return;
    const CTCCarrying* carrying;
    if (!hero->PeekTC(TCI_CARRYING, &carrying))
        return;
    if (!player->IsFirstPersonTargeting() && hero->IsPerformingAnyAction())
        return;
    const CTCSpecialAbilities* abilities;
    if (hero->PeekTC(TCI_SPECIAL_ABILITIES, &abilities) && abilities->GetAbilityMutexLock())
        return;
    CThing* thing = carrying->GetThingInPrimarySlot();
    if (!IS_THING_ALIVE(&*thing))       // a pointer rvalue: retail binds the reference to a temporary (in the dead event slot)
        return;
    const CTCWeapon* weapon;
    if (!thing->PeekTC(TCI_WEAPON, &weapon) || !weapon->IsProjectileWeapon())
        return;
    ranged->IncrementProjectileWeaponMode();
    if (ranged->IsToUseFirstPersonTargetingMode())
        player->AddMode(PLAYER_MODE_USE_PROJECTILE_WEAPON, false);
    else
        player->RemoveMode(PLAYER_MODE_USE_PROJECTILE_WEAPON);
    }
    {
    CGameCameraManager* camera;
    GetGameCamera()->GetCameraManager(&camera);
    camera->PlayCameraSoundThroughObservedThing("PROJECTILE_WEAPON_ZOOM_IN");
    }
}
