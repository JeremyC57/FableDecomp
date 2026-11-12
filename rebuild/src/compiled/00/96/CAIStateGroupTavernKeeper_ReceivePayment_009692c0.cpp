// CAIStateGroup_TavernKeeper::ReceivePayment() -- the keeper stops being pushable, walks to the jug point, swaps the pending payment request for a fresh MESSAGE_EVENT_PAYMENT_REQUEST aimed at the payee, waits up to 20 seconds for the finished-payment message, then plays RECEIVE_MONEY_AT_BAR; false (bar point freed, pushable again) when the walk fails, the payment is cancelled, the wait times out, the group is stopping or the payee dies
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009692C0, length 426 bytes; __fastcall, no arguments, returns bool.
// Leaf: ?ReceivePayment@CAIStateGroup_TavernKeeper@@QAE_NXZ
// Flags: none
// Name: Ego_r.pdb + FableWin.pdb ?ReceivePayment@CAIStateGroup_TavernKeeper@@QAE_NXZ; caller Update 0x00969470.
// Callees: CAIStateGroupBase::SetPushableByHero 0x0097E330, WalkToPoint 0x00968230, GetPaymentCancelledMessage 0x00968510,
//   FreeBarPoint 0x00968950, CAIStateGroupBase::GetWorld 0x008FCFC0 (+ inline CWorld::GetMessageEventManager, world +0x60),
//   CMessageEventManager::RemoveMessage 0x00493B5B / AddMessage 0x00687540, CBaseIntelligentPointer::GetPItem 0x00A01B50,
//   CWorld::GetFrame 0x0049D870, CMainGameComponent::GetConstantFPS (inline, ConstantFPS 0x01375550), GetPaymentFinishedMessage
//   0x00968460, CAIStateGroupBase::AIAction_DoNothing 0x0097E200, IsStopping 0x008FD010, IS_THING_ALIVE (inline), CCharString
//   ctor 0x0099EBF0 / dtor 0x0099EAE0, CCreatureAction_PlayAnimation ctor 0x008424F0, AIAction_PerformActionAndWait 0x0097DB70,
//   ~CCreatureActionBase 0x00693EF0 (the action's implicit destructor).
// Shape: the two early exits are two separate `if` statements with the same body -- VC7.1 merges them into the one in-place block
//   at 0x009692F0 (the walk test jumps forward into it, the cancel test falls into it), ahead of the delayed ebx/ebp/edi pushes.
//   Written as one `||` condition the block is sunk to the end of the function instead. The position (0,0,0), the creature and the
//   payee are temporaries / inline accessors in the AddMessage call; the success path does not make the keeper pushable again.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_SETPUSHABLEBYHERO
#define FABLE_RETAIL_CAISTATEGROUP_TAVERNKEEPER
#define FABLE_RETAIL_CAISTATEGROUP_TAVERNKEEPER_RECEIVEPAYMENT
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_SUSPENDABLE_BASE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_REMOVE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR
#define FABLE_RETAIL_IS_THING_ALIVE
#include "fable_string.h"
#include "engine/EAIAction.h"
#include "engine/EMessageEventType.h"
class CMessageEvent;
#include "engine/C3DVector.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroup_TavernKeeper.h"
#include "engine/CAIBrain.h"
#include "engine/CWorld.h"
#include "engine/CMessageEventManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_PlayAnimation.h"
#include "engine/ThingAlive.h"

bool CAIStateGroup_TavernKeeper::ReceivePayment()
{
    SetPushableByHero(false);
    if (!WalkToPoint(JugPos, JugRHSet)) {
        FreeBarPoint();
        SetPushableByHero(true);
        return false;
    }
    if (GetPaymentCancelledMessage()) {
        FreeBarPoint();
        SetPushableByHero(true);
        return false;
    }
    GetWorld().GetMessageEventManager().RemoveMessage(PaymentRequestMessage);
    const CMessageEvent* message = GetWorld().GetMessageEventManager().AddMessage(MESSAGE_EVENT_PAYMENT_REQUEST, 1000,
        C3DVector(0.0f, 0.0f, 0.0f), &GetCreature(), &*PPayee, 0.0f, 0.0f, 0);
    long endFrame = CWorld::GetFrame() + CMainGameComponent::GetConstantFPS() * 20;
    while (!GetPaymentFinishedMessage()) {
        AIAction_DoNothing();
        if (CWorld::GetFrame() > endFrame || IsStopping() || !IS_THING_ALIVE(PPayee)) {
            GetWorld().GetMessageEventManager().RemoveMessage(message);
            FreeBarPoint();
            SetPushableByHero(true);
            return false;
        }
    }
    AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), "RECEIVE_MONEY_AT_BAR", CSoundPair(),
        false, true, false, 0, 0), false);
    FreeBarPoint();
    return true;
}