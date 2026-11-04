// CAIStateGroup_MinionGuardFollowPlayer::Update() -- re-arm the one-second check; while the hero is alive and the village has no live guard following the player, become that guard, walk up to the hero (3 units) and look at it, then terminate
// Leaf: ?Update@CAIStateGroup_MinionGuardFollowPlayer@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0090C540, 299 bytes, plain `ret` (Ego_r.pdb ?Update@CAIStateGroup_MinionGuardFollowPlayer@@UAEXXZ).
// Callees: CWorld::GetFrame() 0x0049D870 (static, after CMainGameComponent::GetConstantFPS() 0x01375550 is read),
//   CWorld::PeekMyPlayer() const 0x0049D9D0 (on the inline CMainGameComponent::PeekWorld of CMainGameComponent::Get() 0x013B86A0),
//   CPlayer::GetPControlledCreature() const 0x00487DC0, CVectorMap lower_bound 0x0040F020 (GetTC<CTCVillageMember>, result
//   unused), CTCVillageMember::GetPVillage() const 0x007EE540 (twice), CBaseIntelligentPointer::GetPItem 0x00A01B50 (the
//   inline CTCVillage::GetGuardFollowingPlayer), CTCVillage::SetGuardFollowingPlayer 0x0090C780,
//   CAIStateGroupBase::AIAction_MoveToThingProximity 0x0097E380 / AIAction_LookAtThing 0x0097D6F0 (CIntelligentPointer<const
//   CThing> by value: CBaseIntelligentPointer() 0x00A01B10, vtable 0x01237F1C, SetPItem 0x00A01B90), Terminate 0x008FCF40.
// Shape: the dead-hero exit is an early `Terminate(); return;` (the if-block form swaps esi/edi for this and the village member);
//   the creature comes through the inline GetCreature() (PBrain->Creature read in place gives other scratch registers).
// Layout: NextCheckFrame +0x18 (after the base's 0x14 bytes and a word at +0x14).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_GUARDFOLLOW
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CPlayer.h"
#include "engine/CTCVillage.h"
#include "engine/CTCVillageMember.h"
enum EAIActionReturn { AI_ACTION_RETURN_SUCCESS = 2 };                            // descriptive values
enum EAIActionRepeat { AI_ACTION_REPEAT_ONCE = 1 };                               // descriptive values
enum EMovementType { MOVEMENT_WALK = 3 };                                         // as the landed 0x00830000
inline bool IS_THING_ALIVE(const CThing* thing) { return thing != 0 && !thing->IsDead(); }

class CAIStateGroup_MinionGuardFollowPlayer : public CAIStateGroupBase {       // no retail-branch shared header
public:
    virtual void Update();
private:
    long          _unk14;                                                        // +0x14
    long          NextCheckFrame;                                                // +0x18
};

void CAIStateGroup_MinionGuardFollowPlayer::Update()
{
    bool reached;
    long oneSecond = CMainGameComponent::GetConstantFPS();
    NextCheckFrame = CWorld::GetFrame() + oneSecond;
    const CThing* hero = CMainGameComponent::Get()->PeekWorld().PeekMyPlayer()->GetPControlledCreature();
    if (!IS_THING_ALIVE(hero)) {
        Terminate();
        return;
    }
    CTCVillageMember* member;
    GetCreature().GetTC(&member);
    if (IS_THING_ALIVE(member->GetPVillage()->GetGuardFollowingPlayer()))
        return;
    member->GetPVillage()->SetGuardFollowingPlayer(&GetCreature());
    AIAction_MoveToThingProximity(AI_ACTION_REPEAT_ONCE, hero, 3.0f, reached, MOVEMENT_WALK, true, false, true, true, 0.0f);
    AIAction_LookAtThing(AI_ACTION_REPEAT_ONCE, hero, 0xb, 0.0027777f);
    Terminate();
}