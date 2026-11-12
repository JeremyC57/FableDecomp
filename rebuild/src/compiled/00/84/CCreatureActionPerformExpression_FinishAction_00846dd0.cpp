// CCreatureAction_PerformExpression::FinishAction() -- end of an expression: once the expression has run its time, an extended expression whose button is still held is replaced by a PerformExpressionExtended (moving the removed item across); otherwise the removed-item pointer is cleared. Then the base finish, and (unless replaced) emote/sound/script clean-up; the "expression performed" event to the target and the hero's FOLLOW/WAIT opinion deeds; finally the out-of animation
// Grade: RELOCATION_MATCH (VC7.1 RTM, default flags; graded with FABLE_HDR_OVERLAY = an overlay dir without fable_string.h, as Q25)
// Leaf: FinishAction
// Retail: Fable.exe (TLC, VC7.1) @ 0x00846DD0, 1685 bytes (`ret`); vtable slot of CCreatureAction_PerformExpression (Ego_r / FableWin
//   ?FinishAction@CCreatureAction_PerformExpression@@MAEXXZ). Sibling of the Q25 row 0x00847520 (PerformExpressionExtended::FinishAction).
// Callees: CCreatureActionBase::FinishAction 0x00694430, GetFramesRunning 0x006925C0, IsToReplaceItemIfRemoved (virtual), IS_THING_ALIVE
//   0x004CC340, CIntelligentPointer<CThing>(T*) 0x004A96F0 / operator= 0x004A9760, CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem
//   0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10, CCreatureAction_PerformExpressionExtended ctor 0x00843310 / dtor 0x0084ADC0,
//   SetPItemRemoved 0x0072D360, SetCurrentAction 0x006644F0, CTCEmoteIcon::SetEmoteIcon 0x0071AAB0 / SetEmoteText 0x0071AC70,
//   CTCSoundPlayer::ReleaseAttachedLoopingSound 0x0066AD90, CQuestManager::DeactivateQuest 0x004B43D0, CDefClassBase::GetInstantiationName
//   (vtable slot 10, +0x28), CDefString::operator== 0x005B3440 / operator!= 0x005FA740, NEntityEvents::CEventExpressionPerformedTo ctor
//   0x0084B010, CTCEntityEvents::AddEvent 0x008113C0, CTCFollowed::IsCreatureValidFollower 0x006D4E60 / IsFollower 0x006D4D70 /
//   IsAIFollower 0x006D4DB0, CTCHeroOpinionDeedLog::PostDeed 0x006C6990, CCreatureAction_PlayAnimation ctor 0x008424F0.
// Identities fixed against the draft: the def-name compare is CDefClassBase::GetInstantiationName (slot 10, CDefClassBase.h), not a
//   GetName; 0x006925C0 is GetFramesRunning (Q25); the +0x1C9 bit-2 test is CThingAICreature::IsRespondingToFollowAndWait (Ego_r
//   ?IsRespondingToFollowAndWait@CThingAICreature@@QBE_NXZ, inline); the +0x6C bit tests are CThing::GetTypeFlags() (FableWin, inline).
// Shape: the not-yet-finished arm builds a default CIntelligentPointer<CThing>, sets it to 0 and copies its item into PItemRemoved (inline
//   base ctor + vtable store + SetPItem(0), GetPItem, SetPItem, dtor), unlike the replacement arm's out-of-line (T*) ctor / operator=.
//   The event object is 0x20 bytes (`push 20h` to operator new): CEventExpressionPerformedTo carries its Ego_r members.
// Model: shared engine headers + the Q25 overlays (CCreatureActionBase / PerformExpressionExtended / TCSoundPlayer / TCEmoteIcon /
//   CQuestManager / CPlayer) + this chunk's CThingAICreature.h opt-in (_ISRESPONDINGTOFOLLOWANDWAIT). Local: NEntityEvents::
//   CEventExpressionPerformedTo (no engine header; Ego_r.pdb members).
#define FABLE_RETAIL_EXPRESSION_FRAME
#define FABLE_RETAIL_CCREATUREACTIONBASE_GETFRAMESRUNNING
#define FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGAICREATURE_ISRESPONDINGTOFOLLOWANDWAIT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_OUTOFLINE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTIONBASE_ACCESSORS
#define FABLE_RETAIL_CCREATUREACTIONBASE_SETPITEMREMOVED
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_NCREATUREMODE_QUERIES
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCSOUNDPLAYER_RELEASELOOPINGSOUND
#define FABLE_RETAIL_CTCEMOTEICON_STATTYPE
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_FAILQUEST
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_PDCB_TYPED_MEMBERS
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#define FABLE_RETAIL_CDEFSTRING_EQUALS
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_CTCENTITYEVENTS_ADDEVENT
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTHINGAICREATURE
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureAction_PerformExpressionExtended.h"
#include "engine/CCreatureAction_PlayAnimation.h"
#include "engine/CTCInventory.h"
#include "engine/CPlayer.h"
#include "engine/CGamePlayerInterface.h"
#include "engine/CExpressionDef.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CTCEmoteIcon.h"
#include "engine/CQuestManager.h"
#include "engine/ThingAlive.h"
#include "engine/CCreatureAction_PerformExpression.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCEntityEvents.h"
#include "engine/CTCFollowed.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/NEntityEvents_CEventHitBase.h"

namespace NEntityEvents {
// Ego_r.pdb NEntityEvents::CEventExpressionPerformedTo (no engine header): PerformedBy +8, PExpressionDef +0x10, DurationInFrames +0x14,
// ExtraData1 +0x18, ExtraData2 +0x1c.
class CEventExpressionPerformedTo : public CEventBase {
public:
    CEventExpressionPerformedTo(CThingCreatureBase* performed_by, long duration_in_frames, const CDefPointer<const CExpressionDef>& expression,
                                long extra_data1, long extra_data2);   // 0x0084B010
private:
    CIntelligentPointer<CThingCreatureBase> PerformedBy;                 // +0x08
    CDefPointer<const CExpressionDef> PExpressionDef;                     // +0x10
    long DurationInFrames;                                                // +0x14
    long ExtraData1;                                                      // +0x18
    long ExtraData2;                                                      // +0x1c
};
}

void CCreatureAction_PerformExpression::FinishAction()
{
    if (ReplacedAction)
        return;
    if (FramesRunning >= TotalTime) {
        if (Extended && ButtonHeld) {
            ReplacedAction = true;
            CThing* item = 0;
            if (IsToReplaceItemIfRemoved() && IS_THING_ALIVE(PItemRemoved)) {
                item = PItemRemoved.Get();
                CIntelligentPointer<CThing> no_item(0);
                PItemRemoved = no_item;
            }
            long interruption_group = InterruptionGroupID;
            CThing* perform_to = PPerformToThing.Get();
            CThingCreatureBase* creature = PCreature.Get();
            CCreatureAction_PerformExpressionExtended action(*creature, perform_to, PExpressionDef, ExpressionItemDefIndex,
                EmoteIconAdded, SoundEffectID, GetFramesRunning(), interruption_group + 1);
            if (item)
                action.SetPItemRemoved(CIntelligentPointer<CThing>(item));
            PCreature->SetCurrentAction(action);
        }
    } else {
        CIntelligentPointer<CThing> no_item;
        no_item.Set(0);
        PItemRemoved.Set(no_item.Get());
    }
    CCreatureActionBase::FinishAction();
    if (!ReplacedAction) {
        if (PCreature.Get() && !PCreature->IsDead()) {
            if (EmoteIconAdded) {
                CTCEmoteIcon* ptc_emote = 0;
                PCreature->GetTC(&ptc_emote);
                ptc_emote->SetEmoteIcon((CTCEmoteIcon::EEmotePriority)5, 0, 0.0f, 1.0f, 255);
                CWideString no_text(L"");
                ptc_emote->SetEmoteText((CTCEmoteIcon::EEmotePriority)7, no_text, 0.0f, 1.0f);
            }
            PCreature->RemoveTC((ETCInterfaceType)0x8f);
        }
        if (SoundEffectID > 0) {
            CTCSoundPlayer* ptc_sound;
            if (PCreature->GetTC(&ptc_sound))
                ptc_sound->ReleaseAttachedLoopingSound(SoundEffectID);
        }
        if (PExpressionDef->DeactivateScriptAtEnd && PExpressionDef->ScriptName != "") {
            CCharString script_name = PExpressionDef->ScriptName.GetString();
            CQuestManager::Get().DeactivateQuest(script_name, 0);
        }
    }
    if (PPerformToThing.Get()) {
        // ThingTypeFlags bit 0x10: the perform-to thing is an AI creature; bit 0x20 (below): the performer is the hero.
        if (PPerformToThing->GetTypeFlags() & 0x10) {
            if (PExpressionDef->GetInstantiationName() == "EXPRESSION_FOLLOW" || PExpressionDef->GetInstantiationName() == "EXPRESSION_WAIT") {
                if (!((CThingAICreature*)PPerformToThing.Get())->IsRespondingToFollowAndWait())
                    goto out_of;
            }
        }
        {
            CTCEntityEvents* ptc_events;
            if (PPerformToThing->GetTC(&ptc_events))
                ptc_events->AddEvent(new NEntityEvents::CEventExpressionPerformedTo(PCreature.Get(), GetFramesRunning(), PExpressionDef, ExtraData1, ExtraData2));
        }
        if ((PPerformToThing->GetTypeFlags() & 0x10) && (PCreature->GetTypeFlags() & 0x20)) {
            const CTCFollowed* ptc_followed;
            if (PCreature->PeekTC(&ptc_followed)) {
                if (PExpressionDef->GetInstantiationName() == "EXPRESSION_FOLLOW") {
                    CThingAICreature* target = (CThingAICreature*)PPerformToThing.Get();
                    CTCHeroOpinionDeedLog* ptc_deed_log;
                    PCreature->GetTC(&ptc_deed_log);
                    bool add_to_front = false;
                    bool valid = ptc_followed->IsCreatureValidFollower(*target, &add_to_front);
                    if (add_to_front && !ptc_followed->IsFollower(target)) {
                        if (valid)
                            ptc_deed_log->PostDeed((EOpinionDeedType)0x2e, PCreature->GetPos(), target, target, (EOpinionPostDeedType)1, false, false, 0, false, 0);
                        else
                            ptc_deed_log->PostDeed((EOpinionDeedType)0x2f, PCreature->GetPos(), target, target, (EOpinionPostDeedType)1, false, false, 0, false, 0);
                    }
                } else if (PExpressionDef->GetInstantiationName() == "EXPRESSION_WAIT") {
                    CThingAICreature* target = (CThingAICreature*)PPerformToThing.Get();
                    if (ptc_followed->IsAIFollower(target)) {
                        CTCHeroOpinionDeedLog* ptc_deed_log;
                        PCreature->GetTC(&ptc_deed_log);
                        ptc_deed_log->PostDeed((EOpinionDeedType)0x30, PCreature->GetPos(), target, target, (EOpinionPostDeedType)1, false, false, 0, false, 0);
                    }
                }
            }
        }
    }
out_of:
    if (PPerformToThing.Get() && PExpressionDef->AnimationToOtherOutOf != "") {
        CCharString animation = PExpressionDef->AnimationToOtherOutOf.GetString();
        CSoundPair sound;
        PCreature->SetCurrentAction(CCreatureAction_PlayAnimation(*PCreature, animation, sound, false, true, false, 0, 0));
    } else if (PExpressionDef->AnimationOutOf != "") {
        CCharString animation = PExpressionDef->AnimationOutOf.GetString();
        CSoundPair sound;
        PCreature->SetCurrentAction(CCreatureAction_PlayAnimation(*PCreature, animation, sound, false, true, false, 0, 0));
    }
}