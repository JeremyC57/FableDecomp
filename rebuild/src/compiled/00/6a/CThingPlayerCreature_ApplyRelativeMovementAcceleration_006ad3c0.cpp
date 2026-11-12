// CThingPlayerCreature::ApplyRelativeMovementAcceleration(const C3DVector& acceleration, EGameAction action) -- record one game action's camera-relative movement request: the request is rotated into world space by the control angle (the render camera's angle with the free camera, else the current game camera's control angle) and normalised, the creature's current mode gives the movement speed for that direction against the facing vector, and the request scaled by speed / GetMaxAcceleration() (1.0 when the speed is negligible) is added to the action's entry of RelativeMovementAccelerationComponents (sum and count; a new entry starts at count 1); the idle counter is cleared
// Leaf: ?ApplyRelativeMovementAcceleration@CThingPlayerCreature@@QAEXABVC3DVector@@W4EGameAction@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AD3C0, 447 bytes, thiscall `ret 8`, frame 0x68 + ebx/esi/edi (ebp pushed around the camera block only).
// Identity: Ego_r.pdb / FableWin.pdb ?ApplyRelativeMovementAcceleration@CThingPlayerCreature@@QAEXABVC3DVector@@W4EGameAction@@@Z (public, non-virtual);
//   callers CWorld::EAMoveCreature 0x006295C0 and CWorld::EAControlledCreatureStrafe 0x0062BD80.
// Callees: CVectorMap::LowerBound const @ 0x004365B0 (the expanded CThing::PeekTC<const CTCCreatureModeManager>, interface 0x31),
//   CThing::PeekWorld @ 0x004C7AB0, CWorld::PeekMyPlayer @ 0x0049D9D0, CWorld slot 6 (+0x18) GetCurrentRenderCamera() (a 0x38-byte CCamera
//   by hidden pointer -- the reason for the 0x68 frame), CCamera::GetAngleXY @ 0x00A0C380, CAIGameCameraBase slot 7 (+0x1c) GetControlAngleXY on
//   CWorld::PCurrentGameCamera (+0x34, PeekGameCamera expanded), C3DVector::RotateXY @ 0x00A14040, C3DVector::AccurateNormalise @ 0x00A14440,
//   CTCCreatureModeManager::GetCurrentMode @ 0x0073A6E0, CTCPhysicsBase slot 70 (+0x118) GetFacingVector(), CModeBase slot 7 (+0x1c)
//   GetMovementSpeed(direction, facing, EMovementType 0), own slot 87 (+0x15c) GetMaxAcceleration (0x006A9C10),
//   STLport _Rb_tree<EGameAction, pair<const EGameAction, pair<C3DVector,long> > >::find @ 0x006AE3E0 and ::insert_unique @ 0x006AE9F0
//   (map::find / map::insert, both out of line). Global: DELTA_NOTIONAL_ZERO @ 0x0129BA3C (1e-4f).
// Headers: engine/CThingPlayerCreature.h opt-in FABLE_RETAIL_CTHINGPLAYERCREATURE_ACCELERATIONMAP (the +0x1a8 member typed as the Ego_r
//   map<EGameAction,pair<C3DVector,long> >); FABLE_RETAIL_CTHING_ORIGINAL_VTABLE gives GetMaxAcceleration its retail slot 87.
// Shape (wf7): three source facts make the bytes. (1) PhysicsTC is read into a local BEFORE GetCurrentMode() is called (retail
//   `mov ebp,[edi+60h]` ahead of the call 0x0073A6E0, which then leaves no register for the mode's vtable: it is spilled to the dead
//   acceleration argument slot). (2) `acceleration * scale` is written out in BOTH arms of the find test, not as one named vector: VC7.1
//   hoists the three products above the compare, keeps X and Y on the x87 stack and stores only Z (a named `scaled` local stores Y
//   too and grows the function to 471 bytes). (3) The mode manager pointer is left unset when the creature lacks the interface (PeekTC's
//   result ignored): retail's not-present arm loads garbage (`mov ebx,[esp+7Ch]`).
// Grade: RELOCATION_MATCH (wf7, 5 grades).
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
#define FABLE_RETAIL_CTHINGPLAYERCREATURE_RELATIVEMOVEMENT
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE_VSLOTS2
#define FABLE_RETAIL_CTHINGPLAYERCREATURE_ACCELERATIONMAP
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CCAMERA_ANGLES
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_PLAYER
#include "fable_string.h"
#include "engine/fable_nscript_forward.h"
#include "engine/EMovementType.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CWorld.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CCamera.h"
#include "engine/CPlayer.h"

extern const float DELTA_NOTIONAL_ZERO;          // 0x0129BA3C

typedef std::map<EGameAction, std::pair<C3DVector, long> > CComponentMap;
void CThingPlayerCreature::ApplyRelativeMovementAcceleration(const C3DVector& acceleration, EGameAction action)
{
    float scale = 1.0f;
    const CTCCreatureModeManager* mode_manager;
    PeekTC(&mode_manager);
    C3DVector direction = acceleration;
    const CWorld& world = PeekWorld();
    float angle;
    if (world.PeekMyPlayer()->UsingFreeCamera)
        angle = world.GetCurrentRenderCamera().GetAngleXY();
    else
        angle = world.PeekGameCamera()->GetControlAngleXY();
    direction.RotateXY(angle);
    direction.AccurateNormalise();
    const CTCPhysicsBase* physics = PhysicsTC;
    float speed = mode_manager->GetCurrentMode().GetMovementSpeed(direction, physics->GetFacingVector(), (EMovementType)0);
    if (speed > DELTA_NOTIONAL_ZERO)
        scale = speed / GetMaxAcceleration();
    CComponentMap::iterator it = RelativeMovementAccelerationComponents.find(action);
    if (it != RelativeMovementAccelerationComponents.end()) {
        it->second.first += acceleration * scale;
        ++it->second.second;
    } else
        RelativeMovementAccelerationComponents.insert(CComponentMap::value_type(action, std::pair<C3DVector, long>(acceleration * scale, 1)));
    IdleCounter = 0;
}