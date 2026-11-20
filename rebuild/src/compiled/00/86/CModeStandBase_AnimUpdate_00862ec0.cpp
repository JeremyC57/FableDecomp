// CModeStandBase::AnimUpdate(CAnimateThing& anim) -- per-frame animation update of the stand/walk mode: track how long the creature has idled, play the idle animation when it is not moving, otherwise the walking animation, and occasionally (one in three, once idled long enough at walking pace) add combination idles
// Leaf: ?AnimUpdate@CModeStandBase@@UAEXAAVCAnimateThing@@@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00862EC0, length 331 bytes (padded to 333), `ret 4`; CModeStandBase vtable 0x01276D8C slot 2.
// Ego_r.pdb: `?AnimUpdate@CModeStandBase@@UAEXAAVCAnimateThing@@@Z`.
// Callees: CTCCreatureModeManager::CModeBase::GetWorld() const @ 0x0073A680 and the world's virtual slot 4 PeekThingManager
//   (+0x10; the thing manager's two pause-mode bytes +0x54/+0x55), CModeStandBase::UpdateIdleAnim @ 0x00862780,
//   UpdateWalkingAnim @ 0x00862AA0, CThing::PhysicsTC (+0x60) virtual slot 70 GetFacingVector (+0x118, a C3DVector by value),
//   CThing::GetPos @ 0x004C73D0, the creature's virtual slot 79 GetMaxWalkingSpeed(const C3DVector&) (+0x13c,
//   CThingAICreature 0x00830000), virtual GetType (slot 1), CModeBase::GetNoFramesBeforePlayingCombinationIdleAnims(EMode)
//   @ 0x0073A6F0, CWorld::GetWorldSeed @ 0x0049D810 and GFROR13 @ 0x00497890 (as the landed 0x008EE720),
//   AddCombinationIdles(CThingCreatureBase*, CAnimateThing&) @ 0x008619C0 (`__fastcall`).
// Globals: epsilon 0.0001f @ 0x0129BA3C.
// Layout: engine/CModeStandBase.h (FABLE_RETAIL_CMODESTANDBASE): IdleCounter +0x1c, FramesToKeepUpdatingMovement +0x20; the creature's
//   MovementVector X/Y at +0x10c/+0x110 (engine/CThingCreatureBase.h).
// Shape: the planar speed is sqrt(X*X + Y*Y); the walk branch tests `GetMaxWalkingSpeed(GetPos() + PhysicsTC->GetFacingVector()) >= speed`
//   (member operator+ on the position: the argument call runs first and VC loads the velocity operand first); the random
//   step is the inlined generator `seed = GFROR13(seed * 0x24A1 + 0x24DF)` taken `% 3`.
// Naming (NF130, 2026-11-20; q6453 / q6455): moved off the local CGlobalComponents / CGameState / CWorld / CMovementComponent / CThing /
//   CThingCreatureBase / CModeBase / CModeStandBase / CRandomGenerator models onto the shared headers: CModeBase::GetWorld 0x0073A680
//   (?GetWorld@CModeBase@CTCCreatureModeManager@@QBEAAVCWorld@@XZ, engine/CTCCreatureModeManager.h VIRTUALS), whose slot 4 is
//   CWorld::PeekThingManager 0x0049E1C0 (engine/CWorld.h), and the two "suspend idling" bytes are CThingManager::NonScriptedEntitesPauseMode
//   +0x54 / AllEntitesPauseMode +0x55 read through the inline IsNonScriptedEntitesPauseMode / IsAllEntitesPauseMode (engine/CThingManager.h,
//   FableWin.pdb); the movement component +0x60 is CThing::PhysicsTC, its slot 70 CTCPhysicsBase::GetFacingVector (engine/CTCPhysicsBase.h);
//   the planar speed reads CThingCreatureBase::MovementVector +0x10c through the inline PeekMovementVector (FableWin.pdb; NF130 overlay
//   opt-in FABLE_RETAIL_CTHINGCREATUREBASE_PEEKMOVEMENTVECTOR); the random step is the inline GFRandom (engine/GFRandom.h);
//   engine/CModeStandBase.h (FABLE_RETAIL_CMODESTANDBASE + NF130 overlay opt-in FABLE_RETAIL_CMODESTANDBASE_ANIMUPDATE) declares
//   AnimUpdate and the private UpdateIdleAnim / UpdateWalkingAnim. The creature virtual at +0x13c is slot 79 GetMaxWalkingSpeed
//   (engine/CThingCreatureBase.h ORIGINAL_VTABLE order; CThingPlayerCreature.h slot 79 0x006A98A0, CThingAICreature 0x00830000; HEAD
//   called it GetMaxJoggingSpeed, slot 80 +0x140, which grades DIFFER); a virtual call binds no symbol. AddCombinationIdles 0x008619C0 keeps the HEAD declaration (retail's is
//   in an anonymous namespace of the CModeStandBase TU: Ego_r / FableWin ?AddCombinationIdles@?A0x9eb7a2f3@@...).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_PEEKMOVEMENTVECTOR
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CTHINGMANAGER_ALLPAUSE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODEBASE_GETNOFRAMESBEFOREPLAYINGCOMBINATIONIDLEANIMS
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CMODESTANDBASE_ANIMUPDATE
#include "fable_compiler.h"
#include <math.h>
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/NCreatureMode.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingManager.h"
#include "engine/CWorld.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CModeStandBase.h"
#include "engine/GFRandom.h"

class CAnimateThing;
void __fastcall AddCombinationIdles(CThingCreatureBase* creature, CAnimateThing& anim);   // 0x008619C0

void CModeStandBase::AnimUpdate(CAnimateThing& anim)
{
    CThingCreatureBase* creature = (CThingCreatureBase*)&ParentThing;
    const C3DVector& movement = creature->PeekMovementVector();
    float speed = (float)sqrt(movement.X * movement.X + movement.Y * movement.Y);
    if (GetWorld().PeekThingManager().IsNonScriptedEntitesPauseMode() || GetWorld().PeekThingManager().IsAllEntitesPauseMode())
        IdleCounter = 0;
    else
        ++IdleCounter;
    if (speed > 0.0001f || FramesToKeepUpdatingMovement > 0) {
        UpdateWalkingAnim(anim);
        if (creature->GetMaxWalkingSpeed(creature->GetPos() + creature->GetPhysicsTC()->GetFacingVector()) >= speed) {
            if (IdleCounter > GetNoFramesBeforePlayingCombinationIdleAnims((NCreatureMode::EMode)GetType())) {
                if (GFRandom(3, GetWorld().GetWorldSeed()) == 0) {
                    AddCombinationIdles(creature, anim);
                    IdleCounter = 0;
                }
            }
        }
    } else
        UpdateIdleAnim(anim);
}
