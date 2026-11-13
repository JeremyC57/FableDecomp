// CAIStateGroup_SellToBuyer::Update() -- the shopkeeper sells to a customer: take the other participant of the creature interaction as the buyer, wait while the buyer is speaking; if the creature is further from its shop than the keeper's minimum stall distance, call the buyer to follow (CustomerFollowPhrase, only when further than that plus the creature radius and half a metre) and walk back to the stall; then wait up to 20 seconds for the buyer to come within the keeper's customer distance; if it does, add it as a customer, speak the reply phrase, post the accept message (0x10, value 1), speak the thanking phrase and leave the interaction; otherwise (time out, shop closed, buyer gone) refuse the custom; terminate in every case
// Leaf: ?Update@CAIStateGroup_SellToBuyer@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1 RTM) @ 0x0094C050, 1859 bytes, plain `ret` (extent checked: int3 padding before; the last `ret` is at
//   0x0094C792; 0x0094C793..0x0094C79F is int3 padding and 0x0094C7A0 is a separate 10-byte setter `mov [ecx+0Ch],arg ; ret 4` with no
//   caller or catalog row, so the reserved 1882 bytes = 1859 + 13 padding + that 10-byte function: the true length is 1859).
// Identity: slot 12 of vtable 0x0128BF84 (.?AVCAIStateGroup_SellToBuyer@@ : CAIStateGroup_WorkBase<CAIStateGroupBase>); Ego_r.pdb
//   ?Update@CAIStateGroup_SellToBuyer@@UAEXXZ (donor 0x00970400, after IsRunnable 0x009703D0 = retail 0x0094C010) and FableWin.pdb the same.
// Callees (each checked against retail with tmp/rel.py): CVectorMap::LowerBound 0x0040F020 (the GetTC<CTCShopKeeper> at entry and
//   in the reply loop; the inline LowerBound body is expanded in the walk-back and wait loops), CBaseIntelligentPointer 0x00A01B10 /
//   GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / ~ 0x00A01C10, NCreatureAI::IsSpeaking 0x008644E0 / StopSpeaking 0x00864580,
//   CAIStateGroupBase::AIAction_DoNothing 0x0097E200 / AIAction_SpeakAISpeechToTarget (unsigned long) 0x009812C0 /
//   AIAction_MoveToPosProximity 0x0097E890 / Terminate 0x008FCF40 / GetWorld 0x008FCFC0, CAIStateGroup_SellToBuyer::RefuseCustom
//   0x0094BF60, CAIStateGroup_WorkBase<CAIStateGroupBase>::StayInShop 0x0094BEF0 (the COMDAT this TU emits; expanded at three sites,
//   called in the thanking loop), CTCShopKeeper::GetShopTC 0x00798F50 / GetMinDistFromStall 0x00798D10 / GetMaxDistFromCustomer
//   0x00798D20 / AddNewCustomer 0x00798D90, CTCShop::IsShopKeeperAvailable 0x00773AA0, CThing::GetPos 0x004C73D0,
//   CDefClassBase::GetSubDef<CShopDef> 0x005130A0, CWorld::GetFrame 0x0049D870, CMainGameComponent::ConstantFPS 0x01375550,
//   CMessageEventManager::AddMessage 0x00687540, CThingCreatureBase::RemoveFromCreatureInteraction 0x006629B0; vtables
//   CIntelligentPointer<CThingCreatureBase> 0x0123E7D8 / <const CThing> 0x01237F1C; 0.5f 0x0122F59C.
// Shape (each lever measured; NOTES.md): (1) the participant iterator is declared before its `for` (`it; for (it = begin(); ...)`): the
//   extra node keeps the keeper pointer out of ebx until after the speaking wait and gives the participant the ecx-compare / ebx-copy
//   split (0x0094C0B8..0x0094C0BF); (2) the participant walk is a `for` (its post-loop frequency leak) and the wait for the buyer is
//   `bool waiting` + in-loop refusals + `if (!waiting) {...; return;}` before the final refusal: that puts the StayInShop sites at c2 static
//   frequencies 12.5 / 13.9 (expanded, LowerBound expanded), 3.65 (expanded, LowerBound called, score 74) and 1.22 (called), retail's
//   split, and makes the final RefuseCustom/Terminate the function's last block (0x0094C77D, all refusals cross-jumped into it);
//   (3) `end_frame >= GetFrame()` gives `cmp [esp+20h],eax ; jl`; `dist > max * max` inside the `&&` gives `test ah,5 ; jnp`;
//   (4) the radius is read as the member (`->Radius`): the TC pointer is loaded before the two GetMinDistFromStall calls and the float
//   after them (`fadd dword ptr [ebx+34h]`, 0x0094C216); VC7.1 evaluates the by-value inline GetRadius() ahead of the out-of-line calls
//   (probe f12), and a named product local is aliased against the inline IntelligentPointer stores in the speech arguments (fld/fcompp);
//   (5) AddNewCustomer's by-value pointer uses the user copy constructor (FABLE_RETAIL_CINTELLIGENTPOINTER_COPY: ctor, vptr, GetPItem, SetPItem).
// Layout: shared headers with the chunk 2023 overlay (hdr/README.md): new CAIStateGroup_SellToBuyer.h (PBuyer +0x14) and
//   CCreatureInteraction.h; opt-ins in CAIStateGroupBase.h (MOVETOPOSPROXIMITY), CAIStateGroup_WorkBase.h (STAYINSHOP body),
//   CTCShopKeeper.h (CUSTOMER), CThingCreatureBase.h (INTERACTION), NCreatureAI.h (SPEAKING), CTCPhysicsBase.h (RADIUS_ACCESS).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_INTERACTION
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAISTATEGROUP_WORKBASE_STAYINSHOP
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CTCSHOPKEEPER_SHOP
#define FABLE_RETAIL_CTCSHOPKEEPER_CUSTOMER
#define FABLE_RETAIL_CTCSHOP_AVAILABLE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_RADIUS_ACCESS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_NCREATUREAI_SPEAKING
#include "engine/CAIStateGroupBase.h"
#include "engine/CIntelligentPointer.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBaseDef.h"
#include "engine/CAIBrain.h"
#include "engine/CDefPointer.h"
#include "engine/CDefClassBase.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/CShopDef.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCShop.h"
#include "engine/CTCShopKeeper.h"
#include "engine/CCreatureInteraction.h"
#include "engine/CAIStateGroup_WorkBase.h"
#include "engine/CAIStateGroup_SellToBuyer.h"
#include "engine/CMessageEventManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/NCreatureAI.h"
#include "engine/ThingAlive.h"

void CAIStateGroup_SellToBuyer::Update()
{
    const std::list<CThingCreatureBase*>& participants = GetCreature().GetPCreatureInteraction()->GetParticipants();
    CTCShopKeeper* shop_keeper;
    GetCreature().GetTC(&shop_keeper);
    std::list<CThingCreatureBase*>::const_iterator it;
    for (it = participants.begin(); it != participants.end(); ++it)
        if (*it != &GetCreature())
            PBuyer = *it;
    while (NCreatureAI::IsSpeaking(*PBuyer)) {
        if (AIAction_DoNothing() == 1) {
            RefuseCustom();
            Terminate();
            return;
        }
    }
    CTCShop* shop = shop_keeper->GetShopTC();
    C3DVector shop_pos = shop->GetParentThing().GetPos();
    float distance_squared = (shop_pos - GetCreature().GetPos()).GetSquaredMagnitude();
    if (distance_squared > shop_keeper->GetMinDistFromStall() * shop_keeper->GetMinDistFromStall()) {
        CDefPointer<const CShopDef> shop_def;
        shop->GetParentThing().PeekBaseDef().GetSubDef(&shop_def);
        unsigned long phrase = shop_def->CustomerFollowPhrase;
        if (distance_squared > shop_keeper->GetMinDistFromStall() * shop_keeper->GetMinDistFromStall() + GetCreature().GetPhysicsTC()->Radius + 0.5f)
            AIAction_SpeakAISpeechToTarget((EAIActionRepeat)2, &GetCreature(), PBuyer, phrase, true, 0x93);
        EAIActionReturn result;
        do {
            if (!StayInShop() || !IS_THING_ALIVE(PBuyer)) {
                Terminate();
                RefuseCustom();
                return;
            }
            bool reached;
            result = AIAction_MoveToPosProximity((EAIActionRepeat)2, shop_pos, (float)shop_keeper->GetMinDistFromStall(), reached,
                (EMovementType)3, true, false, true, 0, true, 0.0f);
        } while (result == 4);
        NCreatureAI::StopSpeaking(GetCreature());
        if (result != 2) {
            Terminate();
            RefuseCustom();
            return;
        }
    }
    long end_frame = CWorld::GetFrame() + 20 * CMainGameComponent::GetConstantFPS();
    bool waiting = true;
    while (waiting && end_frame >= CWorld::GetFrame()) {
        if (!StayInShop() || !IS_THING_ALIVE(PBuyer)) {
            RefuseCustom();
            Terminate();
            return;
        }
        waiting = AIAction_DoNothing() == 4 && (PBuyer->GetPos() - GetCreature().GetPos()).GetSquaredMagnitude() > shop_keeper->GetMaxDistFromCustomer() * shop_keeper->GetMaxDistFromCustomer();
    }
    if (!waiting) {
        CDefPointer<const CShopDef> shop_def;
        shop->GetParentThing().PeekBaseDef().GetSubDef(&shop_def);
        unsigned long reply_phrase = shop_def->ReplyToRequestPhrase;
        shop_keeper->AddNewCustomer(PBuyer);
        EAIActionReturn result;
        do {
            if (!StayInShop() || !IS_THING_ALIVE(PBuyer)) {
                RefuseCustom();
                Terminate();
                return;
            }
            result = AIAction_SpeakAISpeechToTarget((EAIActionRepeat)2, &GetCreature(), PBuyer, reply_phrase, false, 0x93);
        } while (result == 4);
        CThing* creature = &GetCreature();
        C3DVector position(0.0f, 0.0f, 0.0f);
        CMessageEventManager& messages = GetWorld().GetMessageEventManager();
        messages.AddMessage((EMessageEventType)0x10, 1000, position, creature, PBuyer.Get(), 0.0f, 0.0f, 1);
        unsigned long thanking_phrase = shop_def->ThankingPhrase;
        do {
            if (!StayInShop() || !IS_THING_ALIVE(PBuyer)) {
                Terminate();
                return;
            }
            result = AIAction_SpeakAISpeechToTarget((EAIActionRepeat)2, &GetCreature(), PBuyer, thanking_phrase, false, 0x93);
        } while (result == 4);
        GetCreature().RemoveFromCreatureInteraction();
        Terminate();
        return;
    }
    RefuseCustom();
    Terminate();
}