// CTCQuestCompletionUI::GlobalUpdate(bool) -- per-frame update of the quest completion screen: a quitting screen restores 3D rendering, the HUD, the sound group volumes and stops the counting sounds, plays the exit sound and posts the completion message; an active one waits for the region load, player control and boast processing (paying out the script's accepted boasts), runs the black fade in and out (activating or tearing down the screen) and counts the displayed renown / gold / experience up to their targets
// Grade: DIFFER(2340v2340) -- 4 diff lines: x87 scheduling in the digit-count block: retail spills TargetExperience and loads Gold in a different order around the fmul / fdivp pair (4 lines); everything else matches; chunk 859: 3 more (16 total): target/time/display-count references into the digit block all grow the body (2326/2334), so the digit-count block is the retail spelling and only the x87/int interleave differs; chunk 859 total 126 grades, cross-product grids of behaviour-identical spellings (step forms (5) x FPS/time operand order (3) x clamp if/ternary x const time x counting-sound test order x Frame++ (110 of 240) + user locals for the three targets (12, 2348-2369 B)): best unchanged
// Next idea: the counting block as one expression per member, or the per-frame step through an inline helper (CTCQuestCompletionUI has no Ego_r inline accessor for it); also try the step computed into a float member before the compare
// Leaf: ?GlobalUpdate@CTCQuestCompletionUI@@UAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005E78F0, 2340 bytes, eight `ret 4`. Vtable slot 26 (CTCBase::GlobalUpdate).
// Callees: CIEngine::Enable3DSceneRendering (slot 76) / CutscenePreload (slot 56) on CMainGameComponent::Get()->GetDisplayEngine()
//   .Get3DEngine(); CTCBase::GetWorld 0x00686A70, CWorld::GetFrame 0x0049D870; CPlayerGui::Open 0x004374B0 (singleton 0x013B8790);
//   CUserProfileManager::Get 0x0040D2A0 (GetSoundVolume / GetDialogueVolume inline); PSoundManager 0x013B8394 slot 46 SetGroupVolume and
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
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
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

class CASoundSampleOwner_Unproven { public:                         // the object CSoundManagerLH's slot 9 returns (its +0x44)
    virtual void CUnprovenSlot0(); virtual void CUnprovenSlot1(); virtual void CUnprovenSlot2(); virtual void CUnprovenSlot3();
    virtual void CUnprovenSlot4(); virtual void CUnprovenSlot5(); virtual void CUnprovenSlot6(); virtual void CUnprovenSlot7();
    virtual void CUnprovenSlot8();
    virtual void StopSample(unsigned long handle);                  // slot 9 (+0x24), descriptive
};
class CMessageEvent;
class CMessageEventManager { public:
    CMessageEvent* AddMessage(long type, unsigned long flags, const C3DVector& position, CThing* from, CThing* to, float value1, float value2, long data);   // 0x00687540
};
class CMusicManager { public:
    void ClearUIOverridingMusicSet();                               // 0x006BE030
};
class CPlayer { public:
    void RemoveMode(EPlayerMode mode);                              // 0x00634190
};
CWideString __fastcall GFIntToWideString(long value);               // 0x00405F00

namespace NUISystem {
class CText : public CComponent { public:                           // vtable 0x01249CCC
    virtual void CUnprovenSlot134(); virtual void CUnprovenSlot135(); virtual void CUnprovenSlot136(); virtual void CUnprovenSlot137();
    virtual void CUnprovenSlot138(); virtual void CUnprovenSlot139(); virtual void CUnprovenSlot140(); virtual void CUnprovenSlot141();
    virtual void CUnprovenSlot142(); virtual void CUnprovenSlot143();
    virtual void SetValue(const CWideString& text, bool look_up_tag, bool keep_size);   // slot 144 (+0x240) 0x0054F410
};
class CList { public:
    static void __fastcall ClearPreviousSelected();                 // 0x0048573D ?ClearPreviousSelected@CList@NUISystem@@SIXXZ
};
}

template <> void std::_List_base<CQuestManager::CBoast, std::allocator<CQuestManager::CBoast> >::clear();
template <> void std::list<CQuestManager::CBoast, std::allocator<CQuestManager::CBoast> >::push_back(const CQuestManager::CBoast&);
template <> std::list<CQuestManager::CBoast, std::allocator<CQuestManager::CBoast> >::iterator
    std::list<CQuestManager::CBoast, std::allocator<CQuestManager::CBoast> >::erase(iterator);
template <> void std::vector<NUISystem::CComponent*, std::allocator<NUISystem::CComponent*> >::clear();

class CQuestCardDef;
class CTCQuestCompletionUI : public CTCBase { public:
    enum CompletionType { COMPLETION_SUCCESS, COMPLETION_1, COMPLETION_2, COMPLETION_3 };
    bool                                    Active;                       // +0x0C
    bool                                    ReallyActive;                 // +0x0D
    std::vector<NUISystem::CComponent*>     Components;                   // +0x10
    bool                                    Quitting;                     // +0x1C
    bool                                    WaitingForRegionLoad;         // +0x1D
    bool                                    WaitingForPlayerControl;      // +0x1E
    bool                                    WaitingForBoastProcessing;    // +0x1F
    CompletionType                          Completion;                   // +0x20
    unsigned long                           Frame;                        // +0x24
    long                                    FrameStartedBoastProcessing;  // +0x28
    void*                                   PCardDef;                     // +0x2C CDefPointer<const CQuestCardDef>
    std::list<CQuestManager::CBoast>        Boasts;                       // +0x30
    unsigned long                           RenderedObjectDef;            // +0x34
    CWideString                             QuestSummary;                 // +0x38
    CWideString                             SuccessSummary;               // +0x3C
    CWideString                             QuestObjective;               // +0x40
    double                                  LastRenderTime;               // +0x48
    CCharString                             ScriptName;                   // +0x50
    CWideString                             FailureReason;                // +0x54
    long                                    GoldReward;                   // +0x58
    long                                    RenownReward;                 // +0x5C
    long                                    TargetGoldGained;             // +0x60
    long                                    InitialGold;                  // +0x64
    long                                    TargetExperienceGained;       // +0x68
    long                                    InitialExperience;            // +0x6C
    long                                    TargetRenownGained;           // +0x70
    long                                    InitialRenown;                // +0x74
    float                                   ExperienceGainedDisplayed;    // +0x78
    float                                   RenownGainedDisplayed;        // +0x7C
    float                                   GoldGainedDisplayed;          // +0x80
    unsigned char                           StartQuestMaps[0x24];         // +0x84 StartQuestExperience / Gold / Renown (12-byte maps)
    NUISystem::CText*                       PRenownText;                  // +0xA8
    NUISystem::CText*                       PGoldText;                    // +0xAC
    NUISystem::CText*                       PExperienceText;              // +0xB0
    float                                   AccumFadeTime;                // +0xB4
    NUISystem::CComponent*                  PFrame;                       // +0xB8
    NUISystem::CComponent*                  PBlack;                       // +0xBC
    bool                                    QuittingFadeOut;              // +0xC0
    long                                    QuestCardDef;                 // +0xC4
    unsigned long                           ExperienceCountingSound;      // +0xC8
    unsigned long                           GoldCountingSound;            // +0xCC
    unsigned long                           RenownCountingSound;          // +0xD0
    bool                                    CanTeleportToGuildMember;     // +0xD4
    bool IsHeroControlledByPlayer() const;                                // 0x005E3EA0
    void ActivateReal();                                                  // 0x005E5F30
    virtual void GlobalUpdate(bool paused);
};

void CTCQuestCompletionUI::GlobalUpdate(bool)
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
        if (PSoundManager && Completion != COMPLETION_SUCCESS && Completion != COMPLETION_1) {
            PSoundManager->SetGroupVolume(0x10, CUserProfileManager::Get().GetSoundVolume(), 0.0f);
            PSoundManager->SetGroupVolume(0x40, CUserProfileManager::Get().GetDialogueVolume(), 0.0f);
            PSoundManager->SetGroupVolume(0x80, CUserProfileManager::Get().GetSoundVolume(), 0.0f);
        }
        if (ExperienceCountingSound) {
            PSoundManager->Virtual09_00a3e3a0()->StopSample(ExperienceCountingSound);
            ExperienceCountingSound = 0;
        }
        if (GoldCountingSound) {
            PSoundManager->Virtual09_00a3e3a0()->StopSample(GoldCountingSound);
            GoldCountingSound = 0;
        }
        if (RenownCountingSound) {
            PSoundManager->Virtual09_00a3e3a0()->StopSample(RenownCountingSound);
            RenownCountingSound = 0;
        }
        NUISystem::CManager::GetInstance()->PlaySoundW(NUISystem::CManager::GetInstance()->GetMiscThingsDef()->SoundExit, 1);
        if (Completion == COMPLETION_SUCCESS) {
            CThing* parent = ParentThing;
            GetWorld().GetMessageEventManager().AddMessage(0x49, 0x32, C3DVector(0.0f, 0.0f, 0.0f), parent, 0, 0.0f, 0.0f, 0);
        } else if (Completion == COMPLETION_1) {
            if (ScriptName != "")
                GetWorld().GetMessageEventManager().AddMessage(0x33, 0x32, C3DVector(0.0f, 0.0f, 0.0f), 0, 0, 0.0f, 0.0f,
                    CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().GetIndexFromRegisteredQuest(ScriptName));
        } else if (Completion == COMPLETION_2) {
            if (ScriptName != "")
                GetWorld().GetMessageEventManager().AddMessage(0x34, 0x32, C3DVector(0.0f, 0.0f, 0.0f), 0, 0, 0.0f, 0.0f,
                    CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().GetIndexFromRegisteredQuest(ScriptName));
        }
        if (Completion != COMPLETION_SUCCESS && Completion != COMPLETION_1)
            GetWorld().GetMusicManager().ClearUIOverridingMusicSet();
        return;
    }
    if (!Active)
        return;
    ++Frame;
    if (WaitingForRegionLoad) {
        if (Frame > 1 && !CMainGameComponent::Get()->PeekWorld().IsPerformingRegionLoad())
            WaitingForRegionLoad = false;
        return;
    }
    if (WaitingForPlayerControl) {
        if (IsHeroControlledByPlayer())
            WaitingForPlayerControl = false;
        if (WaitingForPlayerControl)
            return;
    }
    if (WaitingForBoastProcessing) {
        if (FrameStartedBoastProcessing == -1)
            FrameStartedBoastProcessing = Frame;
        if (CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().AreAllBoastsProcessed(ScriptName)) {
            Boasts.clear();
            WaitingForBoastProcessing = false;
            std::list<CQuestManager::CBoast>& boasts = CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().GetQuestBoasts();
            for (std::list<CQuestManager::CBoast>::iterator it = boasts.begin(); it != boasts.end(); ) {
                if (it->ScriptName == ScriptName) {
                    CTCHeroStats* stats;
                    if (ParentThing->GetTC(TCI_HERO_STATS, &stats)) {
                        if (!it->Failed) {
                            ++stats->NumBoastsCompleted;
                            if (Completion == COMPLETION_1) {
                                stats->AddMoney(it->Reward, true);
                                stats->AddRewardMoney(it->Reward);
                            }
                        } else if (Completion == COMPLETION_1) {
                            stats->AddMoney(-it->Cost, true);
                            stats->AddRewardMoney(-it->Cost);
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
            for (unsigned long i = 0; i < Components.size(); ++i)
                delete Components[i];
            Components.clear();
            PFrame = 0;
            NUISystem::CManager::GetInstance()->ClearObservers();
            NUISystem::CManager::GetInstance()->ClearActionParams();
            NUISystem::CList::ClearPreviousSelected();
            NUISystem::CManager::GetInstance()->ClearBastardChild();
            GetPlayerManager().GetMainPlayer().RemoveMode((EPlayerMode)0x25);
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
        return;
    }
    if (!PBlack->ChangedState() || PBlack->GetCurrentStateIndex() != 0)
        return;
    if (PRenownText)
        PRenownText->SetValue(GFIntToWideString((long)RenownGainedDisplayed), true, true);
    if (PGoldText)
        PGoldText->SetValue(GFIntToWideString((long)GoldGainedDisplayed), true, true);
    if (PExperienceText)
        PExperienceText->SetValue(GFIntToWideString((long)ExperienceGainedDisplayed), true, true);
    float digit_count_time = NUISystem::CManager::GetInstance()->GetMiscThingsDef()->DigitCountTime;
    RenownGainedDisplayed += (TargetRenownGained - InitialRenown) / (CMainGameComponent::GetConstantFPS() * digit_count_time);
    ExperienceGainedDisplayed += (TargetExperienceGained - InitialExperience) / (CMainGameComponent::GetConstantFPS() * digit_count_time);
    GoldGainedDisplayed += (TargetGoldGained - InitialGold) / (CMainGameComponent::GetConstantFPS() * digit_count_time);
    if (RenownGainedDisplayed > TargetRenownGained)
        RenownGainedDisplayed = TargetRenownGained;
    if (GoldGainedDisplayed > TargetGoldGained)
        GoldGainedDisplayed = TargetGoldGained;
    if (ExperienceGainedDisplayed > TargetExperienceGained)
        ExperienceGainedDisplayed = TargetExperienceGained;
    if (ExperienceCountingSound && ExperienceGainedDisplayed == TargetExperienceGained) {
        PSoundManager->Virtual09_00a3e3a0()->StopSample(ExperienceCountingSound);
        ExperienceCountingSound = 0;
    }
    if (GoldCountingSound && GoldGainedDisplayed == TargetGoldGained) {
        PSoundManager->Virtual09_00a3e3a0()->StopSample(GoldCountingSound);
        GoldCountingSound = 0;
    }
    if (RenownCountingSound && RenownGainedDisplayed == TargetRenownGained) {
        PSoundManager->Virtual09_00a3e3a0()->StopSample(RenownCountingSound);
        RenownCountingSound = 0;
    }
}
