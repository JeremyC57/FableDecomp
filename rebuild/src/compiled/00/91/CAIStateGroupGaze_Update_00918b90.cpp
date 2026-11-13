// CAIStateGroup_Gaze::Update() -- the gaze state's body: take a free gaze marker (FindFreeGazePos) into GazeMarker, terminate if
// none or if its CTCExclusiveUse already has a user; otherwise claim it (SetUser), read the marker's position and facing, and when
// the creature's work building is this group's owner jitter them (facing rotated by +-0.1 rad, position moved by a random
// unit-sphere offset in XY scaled by 0.25); walk there (run when a random roll is within ChanceOfRunning and the journey is longer
// than MinJourneyForRunning), look along the facing, idle 3..8 seconds of frames, then Terminate.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00918B90, 802 bytes, __fastcall, protected virtual (vtable 0x0128FDEC slot 12). No flags/pragmas.
// Callees: FindFreeGazePos 0x00918880; CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 /
//   dtor 0x00A01C10 (temporary CIntelligentPointer<CThingGameObject>, vtable 0x0127B4D8); CAIStateGroupBase::Terminate 0x008FCF40;
//   CDefClassBase::GetSubDef<CCreatureNavigationDef> 0x006CD2F0; CVectorMap::LowerBound 0x0040F020 (GetTC<CTCExclusiveUse>, type 47);
//   CTCExclusiveUse::SetUser 0x00916640; CTCPhysicsBase::GetFacingVector (slot 70, +0x118); CThingAICreature::GetWorkBuilding
//   (inline, GetPItem on +0x170 at 0x00918CB7, body below); GetOwner (slot 14, +0x38); CAIStateGroupBase::GetWorld 0x008FCFC0; CWorld::GetWorldSeed 0x0049D810;
//   GFFloatRandom 0x004978A0; C3DVector::RotateXY 0x00A14040; C3DVector::GetRandomPointWithinUnitSphere 0x00A146C0;
//   CThing::GetPos 0x004C73D0; AIAction_MoveToPosProximity 0x0097E890; AIAction_LookAtPos 0x0097D580;
//   CAIStateGroupBase::GetPWorldSeed 0x008FCFD0; GFROR13 0x00497890 (GFRandom inline); CWorld::GetFrame 0x0049D870;
//   AIAction_DoNothing 0x0097E200. Data: 0.1f 0x0122EE58, 0.25f 0x0123078C, CMainGameComponent::ConstantFPS 0x01375550.
// Shape (what made it match): (1) GetSubDef<T>'s real body visible in the TU (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE): VC7.1
//   then knows the callee only writes navDef, so the two Terminate exits stay separate and the normal-path release has no null
//   test (`dec dword ptr [ecx+4]`, 0x00918EA0) while the busy path keeps `test ecx,ecx` (0x00918C6A). (2) The XY jitter is a 3D
//   add of a vector whose Z was zeroed: `jitter.Z = 0; jitter *= 0.25f; pos += jitter;` -- VC folds Z += 0*0.25 into the integer
//   self-copy `mov ecx,[esp+28h] ; mov [esp+28h],ecx` (0x00918D24 / 0x00918D2C) and keeps X/Y in the x87 stack (no temporary).
//   (3) The work building is read into a local before the GetOwner vcall (`cmp eax,edi`), the angle is a named float local
//   (spilled to [esp+18h]), the frame rate is read after GFRandom and before GetFrame, `moving` lives in its own block (it shares
//   the HasTC byte slot [esp+13h]).
// Leaf: ?Update@CAIStateGroup_Gaze@@MAEXXZ
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCREATURENAVIGATIONDEF
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_SAVE
#define FABLE_RETAIL_CTHINGAICREATURE_WORKBUILDING
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_HASTC_MATERIALISED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_RANDOMPOINTINSPHERE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_GAZE
#define FABLE_RETAIL_CAISTATEGROUP_GAZE_UPDATE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/CDefPointer.h"
#include "engine/CDefClassBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThingBuilding.h"
#include "engine/CThingAICreature.h"
#include "engine/CCreatureNavigationDef.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/GFRandom.h"
#include "engine/GFFloatRandom.h"
#include "engine/CAIStateGroup_Gaze.h"

// No shared header: CTCExclusiveUse (TCI_EXCLUSIVE_USE 47); Ego_r.pdb User +0x0C.
class CTCExclusiveUse : public CTCBase {
public:
    void SetUser(CThing* user);                                          // 0x00916640 ?SetUser@CTCExclusiveUse@@QAEXPAVCThing@@@Z
    CIntelligentPointer<CThing> User;                                    // +0x0c
};

// CThingAICreature::GetWorkBuilding (FableWin.pdb ?GetWorkBuilding@CThingAICreature@@QBEPAVCThingBuilding@@XZ), WorkBuilding
// +0x170: its body was visible to retail's TU -- inlined as `lea ecx,[eax+170h] ; call 0x00A01B50` (GetPItem) at
// 0x00918CB7-0x00918CBD, no call to an out-of-line GetWorkBuilding. The same line as the landed rows
// rebuild/src/compiled/00/90/CAIStateGroupMinionWorkInInn_GetFreeBench_0090bc60.cpp:46 and
// CTCVillage_IsTimeToGoToTavern_0065aa6f.cpp:83 (no shared-header opt-in carries it yet).
inline CThingBuilding* CThingAICreature::GetWorkBuilding() const { return WorkBuilding.Get(); }

void CAIStateGroup_Gaze::Update()
{
    GazeMarker = FindFreeGazePos();
    if (GazeMarker.Get() == 0) {
        Terminate();
        return;
    }
    const CThingBaseDef& def = GetCreature().PeekBaseDef();
    CDefPointer<const CCreatureNavigationDef> navDef;
    def.GetSubDef(&navDef);
    CTCExclusiveUse* use;
    GazeMarker->GetTC(TCI_EXCLUSIVE_USE, &use);
    if (use->User.Get()) {
        Terminate();
        return;
    }
    use->SetUser(&GetCreature());
    CTCPhysicsBase* physics = GazeMarker->GetPhysicsTC();
    C3DVector pos = physics->GetPosition();
    C3DVector facing = physics->GetFacingVector();
    CThingBuilding* workBuilding = GetCreature().GetWorkBuilding();
    if (GetOwner() == workBuilding) {
        float angle = GFFloatRandom(0.2f, GetWorld().GetWorldSeed()) - 0.1f;
        facing.RotateXY(angle);
        C3DVector jitter = C3DVector::GetRandomPointWithinUnitSphere(GetWorld().GetWorldSeed());
        jitter.Z = 0.0f;
        jitter *= 0.25f;
        pos += jitter;
    }
    EMovementType movement = AI_MOVEMENT_WALKING;
    if (GFFloatRandom(1.0f, GetWorld().GetWorldSeed()) <= ChanceOfRunning
        && (GetCreature().GetPos() - pos).GetMagnitude() > MinJourneyForRunning)
        movement = AI_MOVEMENT_RUNNING;
    {
        bool moving;
        while (AIAction_MoveToPosProximity(ACTION_REPEAT_CALLBACK, pos, 0.0f, moving, movement, true, false,
                                           navDef->AvoidDynamicObstacles, 0, true, 0.0f) == AI_ACTION_IN_PROGRESS)
            ;
    }
    while (AIAction_LookAtPos(ACTION_REPEAT_CALLBACK, facing + pos, 0x11, 0.0027777f) == AI_ACTION_IN_PROGRESS)
        ;
    long wait = GFRandom(6, GetPWorldSeed());
    long fps = CMainGameComponent::GetConstantFPS();
    long end = CWorld::GetFrame() + (wait + 3) * fps;
    while (CWorld::GetFrame() < end && AIAction_DoNothing() == AI_ACTION_IN_PROGRESS)
        ;
    Terminate();
}