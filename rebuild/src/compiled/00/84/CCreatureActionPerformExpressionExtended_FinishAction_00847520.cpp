// CCreatureAction_PerformExpressionExtended::FinishAction() -- when an extended (held-button) expression finishes: if the expression's
// quick-access or context-sensitive item button is still held, set ReplacedAction and start a fresh extended expression (moving a held
// replaced item over to it); otherwise run the base finish, clear the creature's emote icon and text and remove its TC 0x8F, release
// the looping sound, deactivate the expression's quest script, and play the out-of animation (the to-other variant when performing to
// a thing).
// Leaf: ?FinishAction@CCreatureAction_PerformExpressionExtended@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00847520, length 1122 bytes.
// Callees (Ego_r.pdb / FableWin.pdb spellings): CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap LowerBound 0x004365B0 (const)
//   / 0x0040F020, CTCInventory::GetQuickAccessItemButtonIndex 0x00581FD1 / GetContextSensitiveItemButtonIndex 0x00581D4E,
//   CThing::GetPlayer 0x004C7A30, CPlayer::GetJoystickDeviceNumber 0x00487510, CGamePlayerInterface::IsQuickAccessEventInQueue
//   0x004460C0 / IsContextSensitiveEventInQueue 0x004461D0, IsToReplaceItemIfRemoved (vtable +0x94), IS_THING_ALIVE<CIntelligentPointer<CThing> > [FAM13: 0x004460C0 is IsQuickAccessEventInQueue -- it scans QuickAccessItemButtons +0x7c4 (Ego_r.pdb); IsAssignableSpecialMoveButtonInQueue is 0x00445E90 (+0x7b8)]
//   0x004CC340, CIntelligentPointer<CThing>(CThing*) 0x004A96F0 / operator= 0x004A9760, CBaseIntelligentPointer dtor 0x00A01C10,
//   CCreatureActionBase::GetFramesRunning 0x006925C0, the extended ctor 0x00843310 / dtor 0x0084ADC0,
//   CCreatureActionBase::SetPItemRemoved 0x0072D360, CThingCreatureBase::SetCurrentAction 0x006644F0, CCreatureActionBase::FinishAction
//   0x00694430, CTCEmoteIcon::SetEmoteIcon 0x0071AAB0 / SetEmoteText 0x0071AC70, CWideString(const wchar_t*) 0x0099B6B0 / dtor
//   0x0099B510, CThing::RemoveTC 0x004C9840, CTCSoundPlayer::ReleaseAttachedLoopingSound 0x0066AD90, CDefString::operator!= 0x005FA740,
//   CDefStringTable::GetString 0x009D49B0, CQuestManager::DeactivateQuest 0x004B43D0, CCreatureAction_PlayAnimation ctor 0x008424F0,
//   ~CCreatureActionBase 0x00693EF0 (the implicit PlayAnimation dtor), ~CCharString 0x0099EAE0.
// Shape: the replacement is a CCreatureAction_PerformExpressionExtended (8 arguments, J_NKJJ: frames running + FrameCount, then
//   InterruptionGroupID + 1), not a plain PerformExpression; the held item goes over by value through SetPItemRemoved.
#define FABLE_RETAIL_EXPRESSION_FRAME
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
#define FABLE_RETAIL_CCREATUREACTIONBASE_GETFRAMESRUNNING
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

void CCreatureAction_PerformExpressionExtended::FinishAction()
{
    if (!ReplacedAction && Finished) {
        const CTCInventory* ptc_inventory;
        if (PCreature->PeekTC(&ptc_inventory)) {
            long button_index = ptc_inventory->GetQuickAccessItemButtonIndex(ExpressionItemDefIndex);
            if (button_index != -1) {
                if (PCreature->GetPlayer().GetPlayerInterface()->IsQuickAccessEventInQueue(button_index, (EInputButtonEventType)2, PCreature->GetPlayer().GetJoystickDeviceNumber()))
                    ReplacedAction = true;
            }
            if (!ReplacedAction) {
                button_index = ptc_inventory->GetContextSensitiveItemButtonIndex(ExpressionItemDefIndex);
                if (button_index != -1) {
                    if (PCreature->GetPlayer().GetPlayerInterface()->IsContextSensitiveEventInQueue(button_index, (EInputButtonEventType)2, PCreature->GetPlayer().GetJoystickDeviceNumber()))
                        ReplacedAction = true;
                }
            }
            if (ReplacedAction) {
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
                    EmoteIconAdded, SoundEffectID, GetFramesRunning() + FrameCount, interruption_group + 1);
                if (item)
                    action.SetPItemRemoved(CIntelligentPointer<CThing>(item));
                PCreature->SetCurrentAction(action);
            }
        }
    }
    if (ReplacedAction)
        return;
    CCreatureActionBase::FinishAction();
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