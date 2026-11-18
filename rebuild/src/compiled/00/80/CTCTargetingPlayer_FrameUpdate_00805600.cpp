// CTCTargetingPlayer::FrameUpdate() -- base update; with no forced loss of target: count Z-lock selection presses, drop a targeted thing that is dead-free but in limbo, untargetable or no longer Z-targetable, age the targeted positions, rebuild the target list every fourth frame (staggered by the parent's id) or on request, redo first-person projectile targeting, clear the NPC name display when nothing is targeted, then the nearest-interesting and post-process updates
// Leaf: ?FrameUpdate@CTCTargetingPlayer@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) 0x00805600, 641 bytes, thiscall `ret` (vtable 0x0124280C slot 9). Grade: RELOCATION_MATCH (e06 egod lane).
// Names: Ego_r.pdb ?FrameUpdate@CTCTargetingPlayer@@UAEXXZ; Ego_d tc_targeting_player.obj locals verbatim: function scope pgp_int (const
//   CGamePlayerInterface*), z_targeting, ptargeted_thing (const CThing*), test_slong (long); block {lose_target, ptc_targeted (const
//   CTCTargeted*)}. test_slong = GetFrame() + the 64-bit UniqueID (truncated): the high word's dead spill `mov [esp+10h],eax` (0x00805796)
//   is the 64-bit add's, whose high half dies after allocation.
// Same-TU body: PostProcessTargetedThing (Ego_r ?PostProcessTargetedThing@CTCTargetingPlayer@@IAEXXZ, Ego_d right after FrameUpdate, no
//   locals), defined here; retail expands it at 0x00805851..0x00805877 and the same text grades RELOCATION_MATCH at its own address
//   0x00803B20. Its last statement calls 0x0085EE10 NCreatureMode::IsTransmogrified (Ego_r ?IsTransmogrified@NCreatureMode@@YI_NABVCThing@@@Z:
//   Ego_r 0x00833030 heads the run IsTransmogrified / IsUnconscious / GetStunnedEffectCreationPos / IsPlayHitResponse / IsRolling = retail
//   0x0085EE10 / EE40 / EE50 / EE90 / EEB0; FableWin 0x02831D60; landed self row calls it IsCreatureButNotHero) and drops the result:
//   retail calls it at 0x00805877 (and tail-jumps to it at 0x00803B4B) with nothing reading eax.
// Inferred: `NCreatureMode::IsTransmogrified(GetParentThing());` with its bool dropped -- 1 statement, required by the call at 0x00805877 /
//   0x00803B4B (nothing reads eax after it); a check whose result the release build ignores. The literal (EInputButtonEventType)2 is
//   the button event type pushed at 0x0080565E (no repo header names EInputButtonEventType's enumerators).
// Callees: CTCBase::FrameUpdate 0x006868A0, SetPTargetedThing (slot 34, +0x88), CMainGameComponent::GetPPlayerInterface 0x00416359 (Ego_r /
//   FableWin; landed local models call it GetGamePlayerInterface), CGamePlayerInterface::IsToggleZTarget 0x00445B80 / IsGameActionEventInQueue
//   0x00446B10 (GAME_ACTION_TOGGLE_DEACTIVATE_LOCK_TARGET 74, button event 2), CThing::GetPlayer 0x004C7A30, CPlayer::IsZTargeting (inline,
//   +0x20A) / GetJoystickDeviceNumber 0x00487510 / IsFirstPersonTargeting 0x004874D0, GetPTargetedThing (slot 33, +0x84), PeekTC<const
//   CTCTargeted> (const LowerBound 0x004365B0), HasTC(TCI_WILL_LIGHTNING 148), CTCTargeted::IsZTargetable 0x007EA290 (out of line: Ego_r),
//   CIntelligentPointer base ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10, CWorld::GetFrame 0x0049D870,
//   UpdateTargetList 0x00804020, UpdateProjectileTargeting 0x00803D60, UpdateTargetedPositionRandomWalk 0x00803540, CWideString() 0x0099AED0 /
//   ~CWideString 0x0099B510, CPlayerGui::SetNPCNameDisplay 0x004379D0 (Ego_r / FableWin) on CPlayerGui::Get() (0x013B8790),
//   UpdateNearestInterestingThing (slot 39, +0x9C), UpdateTargetThreatening 0x00803280.
// Headers: the landed UpdateProjectileTargeting 0x00803D60's opt-in / include set (without its /d2inlT68 flag) plus hdr/ opt-ins
//   FABLE_RETAIL_CTCTARGETINGPLAYER_FRAMEUPDATE, FABLE_RETAIL_CTCTARGETED_ISZTARGETABLE_DECL, FABLE_RETAIL_CMAINGAMECOMPONENT_GETPPLAYERINTERFACE,
//   FABLE_RETAIL_CPLAYERGUI_NPCNAMEDISPLAY (+ existing FABLE_RETAIL_CPLAYERGUI). The PostProcess compare order (vcall result on the left) and
//   the `% 4 == 0` test spelling are FITTED.
#define FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
#define FABLE_RETAIL_CBASEOBJECT_ORIGINAL_ACCESS
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SUBTRACT_ASSIGN
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTCENEMY_QUERIES
#define FABLE_RETAIL_EXPRESSION_FRAME
#define FABLE_RETAIL_CTHING_PEEKPLAYER
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCSTOCKITEM_TARGETING
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_RAW_NOT_EQUAL
#define FABLE_RETAIL_CINTELLIGENTPOINTER_EQUAL_RAW
#define FABLE_RETAIL_UPDATE_NEAREST_INTERESTING
#define FABLE_RETAIL_CTCTARGETINGPLAYER
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION
#define FABLE_RETAIL_CTCTARGETED
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CGAMECAMERAMANAGER_LOOKAROUND
#define FABLE_RETAIL_CGAMECAMERAMANAGER_GETCAMERA
#define FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_PROJECTILE
#define FABLE_RETAIL_CCAMERA_FOV
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C2DVECTOR_SQUAREDMAGNITUDE_YX
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#define FABLE_RETAIL_NCAMERAMODES_CBASE_LOOKATPHASE_INLINE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_MAP_VALIDATE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_FILTER_BIND3_FUNCTION
#define FABLE_RETAIL_STLP_FOR_EACH_BODY
#define FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_STATIC_FOCAL
#define FABLE_RETAIL_CTCTARGETINGPLAYER_FRAMEUPDATE
#define FABLE_RETAIL_CTCTARGETED_ISZTARGETABLE_DECL
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPPLAYERINTERFACE
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_NPCNAMEDISPLAY
#define FABLE_RETAIL_NCREATUREMODE_ISTRANSMOGRIFIED
#include "engine/EPlayerMode.h"
#include "engine/CThing.h"
#include "engine/ThingAlive.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCTargetingPlayer.h"
#include "engine/CTargetingDef.h"
#include "engine/CPlayer.h"
#include "engine/CGamePlayerInterface.h"
#include "engine/CWorld.h"
#include "engine/CCamera.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CGameCameraManager.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCTargeted.h"
#include "engine/CTCCarrying.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CTCMapwho.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_EnemyCreature.h"
#include "engine/CTCEnemy.h"
#include "engine/C2DBoxI.h"
#include "engine/CTCAIScratchpad.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTCStockItem.h"
#include "engine/TargetingPlayerSupport.h"
#include "engine/NCameraModes_CBase.h"
#include "engine/CThingFilter_IsInFOV.h"
#include "engine/CThingFilter_In2DRange.h"
#include "engine/C2DVector.h"
#include "stlport/targeting_algorithms.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerGui.h"
#include "fable_string.h"
#include "engine/NCreatureMode.h"

void CTCTargetingPlayer::PostProcessTargetedThing()
{
    if (GetPTargetedThing() != PLastThreatenedThing.Get())
        UpdateTargetThreatening();
    NCreatureMode::IsTransmogrified(GetParentThing());
}

void CTCTargetingPlayer::FrameUpdate()
{
    CTCTargetingBase::FrameUpdate();
    if (ForcedToHaveNoTarget) {
        SetPTargetedThing(0);
        return;
    }
    const CGamePlayerInterface* pgp_int = CMainGameComponent::Get()->GetPPlayerInterface();
    if (pgp_int->IsToggleZTarget() && GetParentThing().GetPlayer().IsZTargeting()) {
        if (pgp_int->IsGameActionEventInQueue(GAME_ACTION_TOGGLE_DEACTIVATE_LOCK_TARGET, (EInputButtonEventType)2 /* FITTED: Ego_d INPUT_BUTTON_TYPE_HELD, no header names it */,
                                              GetParentThing().GetPlayer().GetJoystickDeviceNumber()))
            ++ZLockTargetSelectionTimer;
        else
            ZLockTargetSelectionTimer = 0;
    }
    bool z_targeting = GetParentThing().GetPlayer().IsZTargeting();
    const CThing* ptargeted_thing = GetPTargetedThing();
    if (IS_THING_ALIVE(ptargeted_thing)) {
        bool lose_target = true;
        const CTCTargeted* ptc_targeted;
        if (!ptargeted_thing->IsInLimbo() && ptargeted_thing->PeekTC(&ptc_targeted)) {
            if (!z_targeting || GetParentThing().HasTC(TCI_WILL_LIGHTNING) || ptc_targeted->IsZTargetable())
                lose_target = false;
        }
        if (lose_target) {
            TargetedPoint.PThing = 0;
            TargetedPoint.Index = -1;
            SetPTargetedThing(0);
            if (TargetNextThing && TargetNextThingMethod == TARGET_NEXT_NULL)
                TargetNextThing = false;
        }
    }
    OldTargetedPos = TargetedPos;
    OldTargetedPosOffset3D = TargetedPosOffset3D;
    long test_slong = CWorld::GetFrame() + GetParentThing().GetUniqueID();
    if (test_slong % 4 == 0 || TargetNextThing)
        UpdateTargetList();
    PProjectileTargetedThing = 0;
    if (GetParentThing().GetPlayer().IsFirstPersonTargeting()) {
        UpdateProjectileTargeting();
        UpdateTargetedPositionRandomWalk();
    }
    if (!GetPTargetedThing())
        CPlayerGui::Get().SetNPCNameDisplay(CWideString());
    UpdateNearestInterestingThing();
    PostProcessTargetedThing();
}