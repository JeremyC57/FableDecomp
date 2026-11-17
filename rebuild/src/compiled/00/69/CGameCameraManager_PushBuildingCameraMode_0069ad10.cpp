// CGameCameraManager::PushBuildingCameraMode() -- the observed thing entered a building: drop the player's view-hero mode, switch to the building's camera manager set (and back into the combat camera when fighting), and flag the camera as in a building
// Leaf: ?PushBuildingCameraMode@CGameCameraManager@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069AD10, 176 bytes, `ret`.
// Callees: CPlayerManager::GetMainPlayer 0x00449970 (CMainGameComponent::PMainGameComponent 0x013B86A0, PlayerManager +0x1C), CPlayer::RemoveMode
//   (PLAYER_MODE_VIEW_HERO 47) 0x00634190, CBaseIntelligentPointer::GetPItem 0x00A01B50, NCameraModes::GetPBuildingThingOccupying 0x006989D0
//   (__fastcall), CThingBuilding::PeekPCameraManagerSetDefinition 0x0082D3B0 (CDefPointer by hidden pointer), SetCameraManagerSetDef
//   0x0069AC90, the private IsObservedThingInMelee 0x006994F0, PushCombatCameraMode 0x0069A8B0, the CDefPointer release.
// NF62 (2026-11-17): ported onto engine/CGameCameraManager.h (FULL + overlay opt-in BUILDINGMODE) / CThingBuilding.h / CThing.h /
//   CMainGameComponent.h / CPlayerManager.h / CPlayer.h / CDefPointer.h (the dozen local models deleted). Queue entry q10432: the building
//   def is CDefPointer<const CCameraManagerSetDef> (PeekPCameraManagerSetDefinition QBE, SetCameraManagerSetDef ABV?$CDefPointer@$$CBV..).
//   0x0069A8B0 is bound as Ego_r.pdb ?PushCombatCameraMode@CGameCameraManager@@QAEXXZ (its body: the melee camera def, then
//   ObservedThingInMelee = true; the landed stand-in name was AddMeleeCameraMode) -- NOTES.md.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_PEEKDEFINITIONMANAGER
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCAMERA_CTORS
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_CTOR
#define FABLE_RETAIL_CGAMECAMERAMANAGER_FULL
#define FABLE_RETAIL_NCAMERAMODES_CBASE_MEMBERS
#define FABLE_RETAIL_NCAMERAMODES_CBASE_STRING_CAGE
#define FABLE_RETAIL_NCAMERAMODES_CBASE_TYPED_COMPOSITES
#define FABLE_RETAIL_NCAMERAMODES_CSTRINGANDCAGE_INIT
#define FABLE_RETAIL_CGAMECAMERAMANAGER_BUILDINGMODE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGBUILDING_DEFINITION
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHINGBUILDINGDEF
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_PLAYER_REMOVEMODE
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPLAYERMANAGER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_SCOPED_PTR
#include "fable_string.h"
#include "engine/CGameCameraManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CWorld.h"
#include "engine/NCameraModes_CBase.h"
#include "engine/NCameraModes_CBaseInit.h"
#include "engine/NCameraModes_CStringAndCage.h"
#include "engine/CCameraManagerSetDef.h"
#include "engine/EPlayerMode.h"
#include "engine/CThing.h"
#include "engine/CThingBuildingDef.h"
#include "engine/CThingBuilding.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"

void CGameCameraManager::PushBuildingCameraMode()
{
    CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().RemoveMode(PLAYER_MODE_VIEW_HERO);
    if (PObservedThing.Get())
    {
        const CThingBuilding* building = NCameraModes::GetPBuildingThingOccupying(*PObservedThing.Get());
        if (building && !building->IsDead())
        {
            CDefPointer<const CCameraManagerSetDef> camera_manager_set_def = building->PeekPCameraManagerSetDefinition();
            SetCameraManagerSetDef(camera_manager_set_def);
            if (IsObservedThingInMelee())
                PushCombatCameraMode();
        }
    }
    ObservedThingInBuilding = true;
}
