#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_COUNT
#define FABLE_RETAIL_CTCHEROSTATS_BOASTS_COMPLETED_ACCESSORS
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#define FABLE_RETAIL_STLP_VECTOR_CLEAR_OUTOFLINE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CTCQUESTCOMPLETIONUI_GLOBALUPDATE
#define FABLE_RETAIL_CTCQUESTCOMPLETIONUI_DESCRIPTION
#define FABLE_RETAIL_CTCQUESTCOMPLETIONUI_DRAW
#define FABLE_RETAIL_CTCQUESTCOMPLETIONUI_ACTIVATEREAL
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_REMOVEMODE
#define FABLE_RETAIL_CLIST
#define FABLE_RETAIL_CLIST_CLEARPREVIOUSSELECTED
// CTCQuestCompletionUI::GlobalUpdate(bool paused) -- per-frame update of the quest completion screen: a quitting screen restores 3D rendering, the HUD, the sound group volumes and stops the counting sounds, plays the exit sound and posts the completion message; an active one waits for the region load, player control and boast processing (paying out the script's accepted boasts), runs the black fade in and out (activating or tearing down the screen) and counts the displayed renown / gold / experience up to their targets
// Floating grouping: each reward difference undergoes the existing int-to-float conversion; parentheses retain its native no-code PAREN scheduler tuple.
// Flags: none
// Leaf: ?GlobalUpdate@CTCQuestCompletionUI@@UAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005E78F0, 2340 bytes, eight `ret 4`. Vtable slot 26 (CTCBase::GlobalUpdate).
// Callees: CIEngine::Enable3DSceneRendering (slot 76) / CutscenePreload (slot 56) on CMainGameComponent::Get()->GetDisplayEngine()
//   .Get3DEngine(); CTCBase::GetWorld 0x00686A70, CWorld::GetFrame 0x0049D870; CPlayerGui::Open 0x004374B0 (singleton 0x013B8790);
//   CUserProfileManager::Get 0x0040D2A0 (GetSoundVolume / GetDialogueVolume inline); PSoundManager 0x013B8394 slot 46 SetGainFader and
//   slot 9 (the sample owner, whose slot 9 stops a handle); NUISystem::CManager::GetInstance 0x0041E5F2 slots 6 ClearObservers, 29
//   ClearActionParams, 37 ClearBastardChild, 40 PlaySound(CCharString&, long), 42 GetMiscThingsDef (CDefPointer<CUIMiscThingsDef>:
//   SoundExit retail +0x1A8 = Ego_r 0x1B0, DigitCountTime retail +0x214 = Ego_r 0x21C, two 16-byte Ego_r maps before them);
//   CMessageEventManager::AddMessage 0x00687540, CQuestManager (0x013B89FC) GetIndexFromRegisteredQuest 0x004AF740 /
//   AreAllBoastsProcessed 0x004AF9C0 / RemoveAcceptedBoastsForScript 0x004B0030, CMusicManager::ClearUIOverridingMusicSet 0x006BE030;
//   IsHeroControlledByPlayer 0x005E3EA0, ActivateReal 0x005E5F30; list<CBoast> clear 0x004B8C00 / push_back 0x004BA550 / erase 0x004B87F0;
//   CBasicString<char>::Compare 0x00411570; CVectorMap LowerBound 0x0040F020; CTCHeroStats::AddMoney 0x0057B327 / AddRewardMoney 0x0057A59B;
//   vector<NUISystem::CComponent*>::clear 0x00485D25; NUISystem::CList::ClearPreviousSelected 0x0048573D; CTCBase::GetPlayerManager
//   0x00686D40, CPlayerManager::GetMainPlayer 0x00449970, CPlayer::RemoveMode 0x00634190; GFIntToWideString 0x00405F00, __ftol2,
//   ~CWideString 0x0099B510, CCharString copy 0x0099EC30 / operator!=(const char*) 0x0099E960.
// Layout: Ego_r.pdb CTCQuestCompletionUI with retail STLport sizes (as the landed ctor 0x005E4030 / dtor 0x005E41E0).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQ
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_CVECTORMAP_ORIGINAL_FIND
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_CWORLD_QUESTCOMPLETIONUI
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
#define FABLE_RETAIL_CDISPLAYENGINE_GET3DENGINE
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_OPEN
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CUSERPROFILEMANAGER_VOLUME
#define FABLE_RETAIL_CASOUNDMANAGER_SLOT9
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_BOASTS
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_COMPLETIONUI
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCHeroStats.h"
#include "engine/CWorld.h"
#include "engine/CIEngine.h"
#include "engine/CDisplayEngine.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerManager.h"
#include "engine/CUserProfileManager.h"
#include "engine/CASoundManager.h"
#include "engine/CQuestManager.h"
#include "engine/C3DVector.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"
#include "engine/CDefPointer.h"

#include "engine/ETCInterfaceType.h"
#include "engine/CTCQuestCompletionUI.h"
#include "engine/CMessageEventManager.h"
#include "engine/CMusicManager.h"
#include "engine/CPlayer.h"
#include "engine/CASoundEmitter.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CList.h"
inline CWideString __fastcall GFIntToWideString(long n)
{
    CWideString temp;
    temp.Sprintf(L"%d",n);
    return temp;
}
void CTCQuestCompletionUI::GlobalUpdate(bool paused)
{
    if (Quitting) {
        CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().Enable3DSceneRendering(true);
        GetWorld().SetAsStarting3DRendering();
        Active = false;
        Quitting = false;
        ReallyActive = false;
        delete PBlack;
        PBlack = 0;
        CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().Open();
        if (PSoundManager && Completion != QUEST_START && Completion != QUEST_SUCCESS) {
            PSoundManager->SetGainFader(0x10, CUserProfileManager::Get().GetSoundVolume(), 0.0f);
            PSoundManager->SetGainFader(0x40, CUserProfileManager::Get().GetDialogueVolume(), 0.0f);
            PSoundManager->SetGainFader(0x80, CUserProfileManager::Get().GetSoundVolume(), 0.0f);
        }
        if (ExperienceCountingSound) {
            PSoundManager->Get2DEmitter().StopSpecificSample(ExperienceCountingSound);
            ExperienceCountingSound = 0;
        }
        if (GoldCountingSound) {
            PSoundManager->Get2DEmitter().StopSpecificSample(GoldCountingSound);
            GoldCountingSound = 0;
        }
        if (RenownCountingSound) {
            PSoundManager->Get2DEmitter().StopSpecificSample(RenownCountingSound);
            RenownCountingSound = 0;
        }
        NUISystem::CManager::GetInstance()->PlaySoundW(NUISystem::CManager::GetInstance()->GetMiscThingsDef()->SoundExit, 1);
        if (Completion == QUEST_START) {
            GetWorld().GetMessageEventManager().AddMessage(MESSAGE_EVENT_LEAVING_QUEST_START_SCREEN, 0x32, C3DVector(0.0f, 0.0f, 0.0f), &GetParentThing(), 0, 0.0f, 0.0f, 0);
        } else if (Completion == QUEST_SUCCESS) {
            if (ScriptName != "")
                GetWorld().GetMessageEventManager().AddMessage(MESSAGE_EVENT_QUEST_COMPLETED, 0x32, C3DVector(0.0f, 0.0f, 0.0f), 0, 0, 0.0f, 0.0f,
                    CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().GetIndexFromRegisteredQuest(ScriptName));
        } else if (Completion == QUEST_FAILURE) {
            if (ScriptName != "")
                GetWorld().GetMessageEventManager().AddMessage(MESSAGE_EVENT_QUEST_FAILED, 0x32, C3DVector(0.0f, 0.0f, 0.0f), 0, 0, 0.0f, 0.0f,
                    CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().GetIndexFromRegisteredQuest(ScriptName));
        }
        if (Completion != QUEST_START && Completion != QUEST_SUCCESS)
            GetWorld().GetMusicManager().ClearUIOverridingMusicSet();
        return;
    }
    if (!Active)
        return;
    ++Frame;
    if (WaitingForRegionLoad) {
        if (Frame > 1 && !CMainGameComponent::Get()->GetWorld().IsPerformingRegionLoad())
            WaitingForRegionLoad = false;
        return;
    }
    if (WaitingForPlayerControl && IsHeroControlledByPlayer())
        WaitingForPlayerControl = false;
    if (WaitingForPlayerControl)
        return;
    if (WaitingForBoastProcessing) {
        if (FrameStartedBoastProcessing == -1)
            FrameStartedBoastProcessing = Frame;
        if (CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().AreAllBoastsProcessed(ScriptName)) {
            Boasts.clear();
            WaitingForBoastProcessing = false;
            std::list<CQuestManager::CBoast>& boasts = CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().GetQuestBoasts();
            for (std::list<CQuestManager::CBoast>::iterator it = boasts.begin(); it != boasts.end(); ) {
                if (it->GetScriptName() == ScriptName) {
                    CTCHeroStats* ptc_hero_stats;
                    if (GetParentThing().GetTC(&ptc_hero_stats)) {
                        if (!it->IsFailed()) {
                            ptc_hero_stats->SetNumBoastsCompleted(ptc_hero_stats->GetNumBoastsCompleted()+1);
                            if (Completion == QUEST_SUCCESS) {
                                ptc_hero_stats->AddMoney(it->GetReward(), true);
                                ptc_hero_stats->AddRewardMoney(it->GetReward());
                            }
                        } else if (Completion == QUEST_SUCCESS) {
                            ptc_hero_stats->AddMoney(-it->GetCost(), true);
                            ptc_hero_stats->AddRewardMoney(-it->GetCost());
                        }
                    }
                    Boasts.push_back(*it);
                    it = boasts.erase(it);
                } else {
                    ++it;
                }
            }
        }
        if (Frame - FrameStartedBoastProcessing > (unsigned long)CMainGameComponent::GetConstantFPS()) {
            CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().RemoveAcceptedBoastsForScript(ScriptName);
            WaitingForBoastProcessing = false;
        }
        return;
    }
    if (PBlack->ChangedState() && PBlack->GetCurrentStateIndex() == 1 && !ReallyActive) {
        PBlack->ChangeState(0);
        if (!ReallyActive)
            ActivateReal();
        return;
    }
    if (QuittingFadeOut) {
        if (PBlack->ChangedState() && PBlack->GetCurrentStateIndex() == 1) {
            PBlack->ChangeState(0);
            for (unsigned long count = 0; count < Components.size(); ++count)
                delete Components[count];
            Components.clear();
            PFrame = 0;
            NUISystem::CManager::GetInstance()->ClearObservers();
            NUISystem::CManager::GetInstance()->ClearActionParams();
            NUISystem::CList::ClearPreviousSelected();
            NUISystem::CManager::GetInstance()->ClearBastardChild();
            GetPlayerManager().GetMainPlayer().RemoveMode(PLAYER_MODE_QUEST_COMPLETION_UI);
            CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().CutscenePreload(false, false);
            return;
        }
        if (PBlack->ChangedState() && PBlack->GetCurrentStateIndex() == 0) {
            Quitting = true;
            QuittingFadeOut = false;
        }
        return;
    }
    if (PBlack->ChangedState() && PBlack->GetCurrentStateIndex() == 1) {
        PBlack->ChangeState(0);
    } else if (PBlack->ChangedState() && PBlack->GetCurrentStateIndex() == 0) {
    // Original PDB block1f5cc, donor022c58b2..022c5c53, owns count_time.
    if (PRenownText)
        PRenownText->SetValue(GFIntToWideString((long)RenownGainedDisplayed), true, true);
    if (PGoldText)
        PGoldText->SetValue(GFIntToWideString((long)GoldGainedDisplayed), true, true);
    if (PExperienceText)
        PExperienceText->SetValue(GFIntToWideString((long)ExperienceGainedDisplayed), true, true);
    float count_time = NUISystem::CManager::GetInstance()->GetMiscThingsDef()->DigitCountTime;
    RenownGainedDisplayed += ((float)(TargetRenownGained - InitialRenown)) / (CMainGameComponent::GetConstantFPS() * count_time);
    ExperienceGainedDisplayed += ((float)(TargetExperienceGained - InitialExperience)) / (CMainGameComponent::GetConstantFPS() * count_time);
    GoldGainedDisplayed += ((float)(TargetGoldGained - InitialGold)) / (CMainGameComponent::GetConstantFPS() * count_time);
    if (RenownGainedDisplayed > TargetRenownGained)
        RenownGainedDisplayed = TargetRenownGained;
    if (GoldGainedDisplayed > TargetGoldGained)
        GoldGainedDisplayed = TargetGoldGained;
    if (ExperienceGainedDisplayed > TargetExperienceGained)
        ExperienceGainedDisplayed = TargetExperienceGained;
    if (ExperienceCountingSound && ExperienceGainedDisplayed == TargetExperienceGained) {
        PSoundManager->Get2DEmitter().StopSpecificSample(ExperienceCountingSound);
        ExperienceCountingSound = 0;
    }
    if (GoldCountingSound && GoldGainedDisplayed == TargetGoldGained) {
        PSoundManager->Get2DEmitter().StopSpecificSample(GoldCountingSound);
        GoldCountingSound = 0;
    }
    if (RenownCountingSound && RenownGainedDisplayed == TargetRenownGained) {
        PSoundManager->Get2DEmitter().StopSpecificSample(RenownCountingSound);
        RenownCountingSound = 0;
    }
    }
}