// CTCQuestCompletionUI::ActivateReal() -- build and open the quest start / completion / failure / death screen: fade the sound
//   groups and set the screen's music set (failure / death), reset the counters, snapshot the hero's renown / gold / experience per
//   quest card on a start screen, create the PC_QUEST_COMPLETION_MENU_* menu and its frame, scroll observers, helpers, title, the
//   renown / gold / experience bars (targets from the snapshots, which are erased), subtitle, quest icon, description viewport and
//   container (GetQuestDescription), the quest card model or (death) the region mini map, then the mouse cursor, the hero mode,
//   the game event, the quest manager notifications and the count-up sounds.
// Leaf: ?ActivateReal@CTCQuestCompletionUI@@QAEXXZ
// Flags: /GS
// Retail: 0x005E5F30, 6580 bytes, thiscall, no arguments (`ret`); /GS cookie, aligned frame, no EH states.
// Identity: Ego_r.pdb 0x00644FA0 / FableWin.pdb 0x022C0DD0 ?ActivateReal@CTCQuestCompletionUI@@QAEXXZ; one caller 0x005E7E25
//   (GlobalUpdate 0x005E78F0).
// Callees: CUserProfileManager::Get 0x0040D2A0, CaptureBackBufferForUIBackground 0x00406CC0, NUISystem::CManager::GetInstance
//   0x0041E5F2, CreateComponent 0x0041DB1D, CVectorMap::LowerBound 0x0040F020 (GetTC), map<long,unsigned long>::operator[] 0x005E9760,
//   _Rb_tree::find 0x005E8EB0, map<long,unsigned long>::erase(iterator) 0x005E8F50 (renown; gold / experience expanded with
//   _Rb_global::_Rebalance_for_erase 0x0042A34E), CCountedPointer<CComponent>(CComponent*) 0x00429C15 / Reset 0x00429C6F,
//   list<CComponent*>::~list 0x0042ABCA / _M_insert_dispatch 0x0042B4C3, vector<CComponent*>::_M_insert_overflow 0x00485DBC,
//   CGameDefinitionManager::Get 0x0044C6B0, CDefinitionManager::GetGlobalIndexFromClassIndex 0x009AD390, GetDef<CUIMiscThingsDef>
//   0x0042B312, GetDef<CThingObjectDef> 0x0042B040, CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2, CAnimationSet::GetAnim
//   0x005DC340, CTCInventoryItem::GetStartingRightHandedSet 0x005D9730, CTCInventoryMap::CreateMiniMapMarkers 0x00603000,
//   CTCInventoryBase::GetMiniMapGraphic 0x005BEB57, CWorldMap::GetRegionNumberMapIsIn 0x004FC190 / GetRegion 0x004FB490, CTCBase
//   GetWorldMap 0x00686D30 / GetPlayerManager 0x00686D40 / GetWorld 0x00686A70 / GetDefinitionManager 0x006869D0,
//   CPlayerManager::GetMainPlayer 0x00449970, CPlayer::AddMode 0x006345C0, CMusicManager::ClearUIOverridingMusicSet 0x006BE030 /
//   SetUIOverridingMusicSet 0x006BDFE0, CQuestManager::OnQuestStartScreenDisplayed 0x004B2FE0 / OnQuestFinishScreenDisplayed
//   0x004B0160, CASoundManager::GetPSoundBank(long) 0x00991840, CMainGameComponent::AddGameEvent 0x0041649C, CGameEvent::AddData
//   0x009F1760, NGameText::CDataBank::GetTextBySymbol 0x009C95E0, GetQuestDescription 0x005E4BE0, CCharString 0x0099EBF0 / 0x0099EAE0,
//   CWideString 0x0099AED0 / 0x0099B6B0 / 0x0099B720 / 0x0099B510 / Sprintf 0x0099BA70, __ftol2 0x00BFEA70; virtual calls through
//   the NUISystem component / manager / text / sprite / mesh / list / viewport, CIEngine::Enable3DSceneRendering (slot 76),
//   CASoundManager::SetGainFader (slot 46) / Get2DEmitter (slot 9) and CASoundEmitter::PlaySoundEvent (slot 4).
// Shape: the picks between two strings (screen, viewport, container) are if / else statements whose calls VC7.1 tail-merges; the
//   quest icon is looked up in each arm; the model's mesh view is a CCountedPointer<CMesh> filled by SetStaticCast (default ctor +
//   ShareData: Info zero-stored, the count bumped only when shared); the menu is held as a CComponent* and cast to CList at
//   AddNonScrollingChild; the cursor name is a named CCharString in its own block; ~list stays out of line (STLP_LIST_DTOR_OUTOFLINE).
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CTCQUESTCOMPLETIONUI_DESCRIPTION
#define FABLE_RETAIL_CTCQUESTCOMPLETIONUI_DRAW
#define FABLE_RETAIL_CTCQUESTCOMPLETIONUI_ACTIVATEREAL
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CWIDESTRING_NULLSTRING
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_STARTSCREEN
#define FABLE_RETAIL_CQUESTMANAGER_FINISHSCREEN
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYENGINE_GET3DENGINE
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CINITBASECLASS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_QUESTCOMPLETIONUI
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCHEROSTATS_TOTALS
#define FABLE_RETAIL_CTCHEROEXPERIENCE_TOTALEXPERIENCE
#define FABLE_RETAIL_CGAMEEVENT_INLINE
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CSOUNDINIT_INLINE_CTOR
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_RIGHTHANDEDSET
#define FABLE_RETAIL_CTCINVENTORYMAP_MINIMAPMARKERS
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SETTERS
#define FABLE_RETAIL_CTCHEROSTATS_SERIALISE
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_ERASE_BODY
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CTHINGOBJECTDEF_VALIDATE
#define FABLE_RETAIL_CANIMFILTER
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_MINIMAPGRAPHIC
#define FABLE_RETAIL_CMAINGAMECOMPONENT_ADDGAMEEVENT
#define FABLE_RETAIL_CASOUNDMANAGER_GETPSOUNDBANK_LONG
#define FABLE_RETAIL_CASOUNDMANAGER_BANKLOOKUP
#include "engine/ETCInterfaceType.h"
#include "engine/CTCQuestCompletionUI.h"
#include "engine/CThing.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCHeroExperience.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CSprite.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CMesh.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"
#include "engine/CMainGameComponent.h"
#include "engine/CDisplayEngine.h"
#include "engine/CIEngine.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingObjectDef.h"
#include "engine/CAppearanceDef.h"
#include "engine/CAnimationSet.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAnimFilterNoGroup.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CTCInventoryMap.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CRegion.h"
#include "engine/CMusicManager.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CGameEvent.h"
#include "engine/CQuestManager.h"
#include "engine/CUserProfileManager.h"
#include "engine/CASoundManager.h"
#include "engine/CASoundEmitter.h"
#include "engine/CSoundInit.h"
inline CWideString FABLE_FASTCALL GFIntToWideString(long n)
{
    CWideString temp;
    temp.Sprintf(L"%d", n);
    return temp;
}
void CTCQuestCompletionUI::ActivateReal()
{
    CUserProfileManager::Get().CaptureBackBufferForUIBackground();
    AccumFadeTime = 0.0f;
    CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().Enable3DSceneRendering(false);
    if (PSoundManager && Completion != 0 && Completion != 1) {
        PSoundManager->SetGainFader(0x10, 0.0f, 500.0f);
        PSoundManager->SetGainFader(0x40, 0.0f, 500.0f);
        PSoundManager->SetGainFader(0x80, 0.0f, 500.0f);
    }
    ExperienceCountingSound = 0;
    GoldCountingSound = 0;
    RenownCountingSound = 0;
    EMusicSetType music;
    switch (Completion) {
    case 0: music = NUISystem::CManager::GetInstance()->GetMiscThingsDef()->QuestStartScreenMusic; break;
    case 1: music = NUISystem::CManager::GetInstance()->GetMiscThingsDef()->QuestCompleteScreenMusic; break;
    case 2: music = NUISystem::CManager::GetInstance()->GetMiscThingsDef()->QuestFailureScreenMusic; break;
    case 3: music = NUISystem::CManager::GetInstance()->GetMiscThingsDef()->DeathScreenMusic; break;
    default: music = NUISystem::CManager::GetInstance()->GetMiscThingsDef()->QuestStartScreenMusic; break;
    }
    if (Completion != 0 && Completion != 1) {
        GetWorld().GetMusicManager().ClearUIOverridingMusicSet();
        GetWorld().GetMusicManager().SetUIOverridingMusicSet(music);
    }
    InitialGold = 0;
    TargetGoldGained = 0;
    GoldGainedDisplayed = 0.0f;
    InitialExperience = 0;
    TargetExperienceGained = 0;
    ExperienceGainedDisplayed = 0.0f;
    InitialRenown = 0;
    TargetRenownGained = 0;
    RenownGainedDisplayed = 0.0f;
    PExperienceText = 0;
    PRenownText = 0;
    PGoldText = 0;
    if (Completion == 0 && QuestCardDef != 0) {
        CTCHeroStats* pstats;
        if (GetParentThing().GetTC(TCI_HERO_STATS, &pstats)) StartQuestRenown[QuestCardDef] = pstats->GetRenownTotal();
        if (GetParentThing().GetTC(TCI_HERO_STATS, &pstats)) StartQuestGold[QuestCardDef] = pstats->GetTotalMoneyEarned();
        CTCHeroExperience* pexperience;
        if (GetParentThing().GetTC(TCI_HERO_EXPERIENCE, &pexperience)) StartQuestExperience[QuestCardDef] = pexperience->GetTotalExperience();
    }
    NUISystem::CManager* pmanager = NUISystem::CManager::GetInstance();
    NUISystem::CComponent* pmenu;
    switch (Completion) {
    case 0:
        pmenu = pmanager->CreateComponent("PC_QUEST_COMPLETION_MENU_START", false);
        PFrame = pmenu->FindChild("PC_UI_QUEST_START_FRAME");
        break;
    case 1:
        pmenu = pmanager->CreateComponent("PC_QUEST_COMPLETION_MENU_COMPLETE", false);
        PFrame = pmenu->FindChild("PC_UI_QUEST_FINISHED_FRAME");
        break;
    case 2:
        pmenu = pmanager->CreateComponent("PC_QUEST_COMPLETION_MENU_FAIL", false);
        PFrame = pmenu->FindChild("PC_UI_QUEST_FAILED_FRAME");
        break;
    case 3:
        pmenu = pmanager->CreateComponent("PC_QUEST_COMPLETION_MENU_DEATH", false);
        PFrame = pmenu->FindChild("PC_UI_QUEST_DEATH_FRAME");
        break;
    default:
        pmenu = pmanager->CreateComponent("PC_QUEST_COMPLETION_MENU_START", false);
        PFrame = pmenu->FindChild("PC_UI_QUEST_START_FRAME");
        break;
    }
    if (Completion != 2 && Completion != 3) {
        NUISystem::CComponent* pscroll = pmenu->FindChild("SCROLL_QUEST_COMPLETION_DESCRIPTION_UP");
        if (pscroll) {
            NUISystem::CManager::GetInstance()->AddObserver(pscroll);
            pscroll->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
        }
        pscroll = pmenu->FindChild("SCROLL_QUEST_COMPLETION_DESCRIPTION_DOWN");
        if (pscroll) {
            NUISystem::CManager::GetInstance()->AddObserver(pscroll);
            pscroll->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
        }
    } else {
        NUISystem::CComponent* pscroll = pmenu->FindChild("SCROLL_DESCRIPTION_UP");
        if (pscroll) {
            NUISystem::CManager::GetInstance()->AddObserver(pscroll);
            pscroll->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
        }
        pscroll = pmenu->FindChild("SCROLL_DESCRIPTION_DOWN");
        if (pscroll) {
            NUISystem::CManager::GetInstance()->AddObserver(pscroll);
            pscroll->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
        }
    }
    CDefPointer<const NUISystem::CUIMiscThingsDef> pmiscthingsdef;
    long misc_thing_def_index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);
    const NGameText::CDataBank& rTextBank = CMainGameComponent::Get()->GetTextBank();
    CGameDefinitionManager::Get().GetDef(misc_thing_def_index, &pmiscthingsdef);
    NUISystem::CComponent* pscreen;
    if (Completion != 0 && Completion != 1) pscreen = pmenu->FindChild("PC_QUEST_COMPLETION_MENU_SCREEN");
    else pscreen = pmenu->FindChild("PC_QUEST_COMPLETION_MENU_SCREEN_START_SUCCESS");
    if (pscreen) {
        CCountedPointer<NUISystem::CComponent> phelpers;
        NUISystem::CComponent* phelper;
        switch (Completion) {
        case 0:
        case 1:
            phelpers.Reset(pmanager->CreateComponent("PC_START_SUCCESS_HELPERS", false));
            pscreen->AddChild(phelpers);
            if (PFrame) PFrame->AddPositionChild(phelpers.Get());
            phelper = phelpers->FindChild("PC_CONTINUE_HELPER");
            if (phelper) phelper->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
            break;
        case 2:
            phelpers.Reset(pmanager->CreateComponent("PC_FAILURE_HELPERS_NO_TELEPORT", false));
            pscreen->AddChild(phelpers);
            if (PFrame) PFrame->AddPositionChild(phelpers.Get());
            phelper = phelpers->FindChild("PC_RESPAWN_HELPER");
            if (phelper) phelper->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
            break;
        case 3:
            phelpers.Reset(pmanager->CreateComponent("PC_DEATH_HELPERS", false));
            pscreen->AddChild(phelpers);
            if (PFrame) PFrame->AddPositionChild(phelpers.Get());
            phelper = phelpers->FindChild("PC_RESPAWN_HELPER");
            if (phelper) phelper->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
            phelper = phelpers->FindChild("PC_QUIT_HELPER");
            if (phelper) phelper->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
            break;
        }
    }
    NUISystem::CText* ptitle = (NUISystem::CText*)pmenu->FindChild("PC_TITLE_QUEST_COMPLETION");
    if (PFrame) PFrame->AddPositionChild(ptitle);
    if (ptitle) {
        if (Completion == 2) ptitle->SetValue(rTextBank.GetTextBySymbol("TEXT_GUI_MENU_QUEST_FAILED"), true, true);
        else if (Completion == 0) ptitle->SetValue(rTextBank.GetTextBySymbol("TEXT_GUI_MENU_QUEST_STARTING"), true, true);
        else if (Completion == 3) ptitle->SetValue(rTextBank.GetTextBySymbol("TEXT_GUI_QUESTCARD_FAILURE_TEXT"), true, true);
    }
    if (Completion == 0 || Completion == 1) {
        CCountedPointer<NUISystem::CComponent> pbar(pmanager->CreateComponent("PC_QUEST_COMPLETION_BAR_RENOWN", false));
        NUISystem::CText* ptext = (NUISystem::CText*)pbar->FindChild("UI_QUEST_COMPLETION_BAR_TEXT");
        if (PFrame) PFrame->AddPositionChild(pbar.Get());
        ((NUISystem::CList*)pmenu)->AddNonScrollingChild(pbar);
        if (PCardDef.Get()) {
            CWideString text = GFIntToWideString((long)RenownGainedDisplayed);
            ptext->SetValue(text, true, true);
        }
        if (ptext) PRenownText = ptext;
        if (Completion == 0) {
            if (PCardDef.Get()) TargetRenownGained = RenownReward;
        } else if (QuestCardDef) {
            std::map<long, unsigned long>::iterator it = StartQuestRenown.find(QuestCardDef);
            if (it != StartQuestRenown.end()) {
                CTCHeroStats* pstats;
                if (GetParentThing().GetTC(TCI_HERO_STATS, &pstats)) TargetRenownGained = pstats->GetRenownTotal() - it->second;
                InitialRenown = 0;
                if (TargetRenownGained < 0) TargetRenownGained = 0;
                StartQuestRenown.erase(it);
            } else {
                CTCHeroStats* pstats;
                if (GetParentThing().GetTC(TCI_HERO_STATS, &pstats)) TargetRenownGained = pstats->GetRenownTotal();
                InitialRenown = 0;
                if (TargetRenownGained < 0) TargetRenownGained = 0;
            }
        }
    }
    if (Completion == 0 || Completion == 1) {
        CCountedPointer<NUISystem::CComponent> pbar(pmanager->CreateComponent("PC_QUEST_COMPLETION_BAR_GOLD", false));
        NUISystem::CText* ptext = (NUISystem::CText*)pbar->FindChild("UI_QUEST_COMPLETION_BAR_TEXT");
        if (PFrame) PFrame->AddPositionChild(pbar.Get());
        ((NUISystem::CList*)pmenu)->AddNonScrollingChild(pbar);
        if (PCardDef.Get()) {
            for (std::list<CQuestManager::CBoast>::iterator it = Boasts.begin(); it != Boasts.end(); ++it) {
            }
            CWideString text = GFIntToWideString((long)GoldGainedDisplayed);
            ptext->SetValue(text, true, true);
        }
        if (ptext) PGoldText = ptext;
        if (Completion == 0) {
            if (PCardDef.Get()) TargetGoldGained = GoldReward;
        } else if (QuestCardDef) {
            std::map<long, unsigned long>::iterator it = StartQuestGold.find(QuestCardDef);
            if (it != StartQuestGold.end()) {
                CTCHeroStats* pstats;
                if (GetParentThing().GetTC(TCI_HERO_STATS, &pstats)) TargetGoldGained = pstats->GetTotalMoneyEarned() - it->second;
                InitialGold = 0;
                if (TargetGoldGained < 0) TargetGoldGained = 0;
                StartQuestGold.erase(it);
            } else {
                CTCHeroStats* pstats;
                if (GetParentThing().GetTC(TCI_HERO_STATS, &pstats)) TargetGoldGained = pstats->GetTotalMoneyEarned();
                InitialGold = 0;
                if (TargetGoldGained < 0) TargetGoldGained = 0;
            }
        }
    }
    if (Completion == 1) {
        CCountedPointer<NUISystem::CComponent> pbar(pmanager->CreateComponent("PC_QUEST_COMPLETION_BAR_EXPERIENCE", false));
        NUISystem::CText* ptext = (NUISystem::CText*)pbar->FindChild("UI_QUEST_COMPLETION_BAR_TEXT");
        if (PFrame) PFrame->AddPositionChild(pbar.Get());
        ((NUISystem::CList*)pmenu)->AddNonScrollingChild(pbar);
        if (ptext) {
            PExperienceText = ptext;
            CWideString text = GFIntToWideString((long)ExperienceGainedDisplayed);
            PExperienceText->SetValue(text, true, true);
        }
        if (QuestCardDef) {
            std::map<long, unsigned long>::iterator it = StartQuestExperience.find(QuestCardDef);
            if (it != StartQuestExperience.end()) {
                CTCHeroExperience* pexperience;
                if (GetParentThing().GetTC(TCI_HERO_EXPERIENCE, &pexperience)) TargetExperienceGained = pexperience->GetTotalExperience() - it->second;
                InitialExperience = 0;
                StartQuestExperience.erase(it);
            } else {
                CTCHeroExperience* pexperience;
                if (GetParentThing().GetTC(TCI_HERO_EXPERIENCE, &pexperience)) TargetExperienceGained = pexperience->GetTotalExperience();
                InitialExperience = 0;
            }
        }
    }
    NUISystem::CText* psubtitle = (NUISystem::CText*)pmenu->FindChild("PC_QUEST_COMPLETION_SUBTITLE");
    if (PFrame) PFrame->AddPositionChild(psubtitle);
    if (Completion == 3) psubtitle->SetValue(rTextBank.GetTextBySymbol("TEXT_QST_DEATH_YOU_HAVE_DIED"), true, true);
    else if (PCardDef.Get()) psubtitle->SetValue(PCardDef->QuestName, L"", true);
    if (Completion != 3) {
        NUISystem::CSprite* picon = (NUISystem::CSprite*)pmenu->FindChild("PC_QUEST_ICON");
        if (PFrame) PFrame->AddPositionChild(picon);
        if (PCardDef.Get()) {
            if (PCardDef->IsCore) picon->SetGraphicIndex(pmiscthingsdef->CoreGraphic);
            else if (PCardDef->IsVignette) picon->SetGraphicIndex(pmiscthingsdef->VignetteGraphic);
            else picon->SetGraphicIndex(pmiscthingsdef->OptionalGraphic);
        }
    } else {
        NUISystem::CComponent* picon = pmenu->FindChild("PC_QUEST_ICON");
        if (PFrame) PFrame->AddPositionChild(picon);
        NUISystem::CDeletion deletion;
        deletion.Method = NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
        picon->SetDeletion(deletion);
    }
    NUISystem::CComponent* pdescription;
    if (Completion != 0 && Completion != 1) pdescription = pmenu->FindChild("PC_DESCRIPTION_VIEWPORT_QUEST_FAILED");
    else pdescription = pmenu->FindChild("PC_DESCRIPTION_VIEWPORT_QUEST_COMPLETION");
    NUISystem::CViewport* pviewport = 0;
    if (pdescription) {
        pviewport = (NUISystem::CViewport*)pdescription;
        PFrame->AddPositionChild(pdescription);
    }
    CCountedPointer<NUISystem::CComponent> pcontainer;
    if (Completion != 0 && Completion != 1) pcontainer.Reset(pmanager->CreateComponent("PC_TEXT_CONTAINER_DESCRIPTION_TEMPLATE_FOR_LIST", false));
    else pcontainer.Reset(pmanager->CreateComponent("PC_COMPONENT_CONTAINER_DESCRIPTION_TEMPLATE_FOR_QUEST_COMPLETION", false));
    if (PFrame) PFrame->AddPositionChild(pcontainer.Get());
    if (pviewport) pviewport->AddFakeChild(pcontainer.Get());
    ((NUISystem::CList*)pmenu)->AddNonScrollingChild(pcontainer);
    GetQuestDescription(pcontainer.Get());
    if (RenderedObjectDef != 0 && Completion != 3) {
        CCountedPointer<NUISystem::CComponent> pmodel(pmanager->CreateComponent("PC_QUEST_CARD_MODEL", false));
        if (PFrame) PFrame->AddPositionChild(pmodel.Get());
        CCountedPointer<NUISystem::CMesh> pmesh;
        pmesh.SetStaticCast(pmodel);
        CDefPointer<const CThingObjectDef> pobjdef;
        GetDefinitionManager().GetDef(RenderedObjectDef, &pobjdef);
        pmesh->SetMeshIndex(pobjdef->Graphic.BankIndex);
        if (RenderedObjectDef != (unsigned long)QuestCardDef) {
            CRightHandedSet rhs = CTCInventoryItem::GetStartingRightHandedSet(RenderedObjectDef);
            pmesh->SetOrientation(rhs);
            if (pobjdef->Graphic.Type == 4) {
                CDefPointer<const CAppearanceDef> pappearance;
                pobjdef->GetSubDef(&pappearance);
                pmesh->SetAnimationIndex(pappearance->Animation.GetAnim("DEFAULT", 0, CAnimFilterNoGroup()).BankIndex);
                pmesh->SetMeshType((EEngineGraphicType)4);
            }
        }
        ((NUISystem::CList*)pmenu)->AddNonScrollingChild(pmodel);
    } else if (Completion == 3) {
        CTCInventoryMap* pmap;
        if (GetParentThing().GetTC(TCI_INVENTORY_MAP, &pmap)) {
            CCountedPointer<NUISystem::CComponent> pminimap(NUISystem::CManager::GetInstance()->CreateComponent("PC_MAIN_MENU_MINI_MAP_TEMPLATE", false));
            CCountedPointer<NUISystem::CComponent> psprite(pmanager->CreateComponent("PC_MAP_MENU_MINI_MAP_TEMPLATE_SPRITE", false));
            if (PFrame) PFrame->AddPositionChild(pminimap.Get());
            if (psprite->GetType() == (NUISystem::EType)0)
                ((NUISystem::CSprite*)psprite.Get())->SetGraphicIndex(pmap->GetMiniMapGraphic(GetWorldMap().GetRegion(GetWorldMap().GetRegionNumberMapIsIn(GetParentThing().GetCurrentMapNumber()))));
            pmap->CreateMiniMapMarkers(pminimap.Get(), GetWorldMap().GetRegion(GetWorldMap().GetRegionNumberMapIsIn(GetParentThing().GetCurrentMapNumber())));
            pminimap->AddChild(psprite);
            NUISystem::CComponent* pviewport_movie = pscreen->FindChild("PC_EXPERIENCE_SPENDING_MOVIE_VIEWPORT");
            if (pviewport_movie) {
                if (PFrame) PFrame->AddPositionChild(pviewport_movie);
                pviewport_movie->AddChild(pminimap);
            } else {
                ((NUISystem::CList*)pmenu)->AddNonScrollingChild(pminimap);
            }
        }
    }
    {
        CCharString cursor_name("UI_MOUSE_POINTER");
        NUISystem::CComponent* pcursor = NUISystem::CManager::GetInstance()->CreateComponent(cursor_name, false);
        Components.push_back(pcursor);
    }
    Components[0]->Initialise();
    Components.push_back(pmenu);
    pmenu->Initialise();
    pmenu->ChangeState(5);
    Active = true;
    GetPlayerManager().GetMainPlayer().AddMode((EPlayerMode)0x25, false);
    CGameEvent event(9);
    event.AddUBYTE(true);
    CMainGameComponent::Get()->AddGameEvent(event);
    if (Completion == 0) CQuestManager::Get().OnQuestStartScreenDisplayed(ScriptName);
    else if (Completion == 1 || Completion == 2) CQuestManager::Get().OnQuestFinishScreenDisplayed(ScriptName);
    ReallyActive = true;
    if (InitialGold != TargetGoldGained) {
        CSoundInit init;
        init.GainFaderBits = 0x20;
        GoldCountingSound = PSoundManager->Get2DEmitter().PlaySoundEvent(*PSoundManager->GetPSoundBank(1), NUISystem::CManager::GetInstance()->GetMiscThingsDef()->CountUpSound, true, init, false);
    }
    if (InitialRenown != TargetRenownGained) {
        CSoundInit init;
        init.GainFaderBits = 0x20;
        RenownCountingSound = PSoundManager->Get2DEmitter().PlaySoundEvent(*PSoundManager->GetPSoundBank(1), NUISystem::CManager::GetInstance()->GetMiscThingsDef()->CountUpSound, true, init, false);
    }
    if (InitialExperience != TargetExperienceGained) {
        CSoundInit init;
        init.GainFaderBits = 0x20;
        ExperienceCountingSound = PSoundManager->Get2DEmitter().PlaySoundEvent(*PSoundManager->GetPSoundBank(1), NUISystem::CManager::GetInstance()->GetMiscThingsDef()->CountUpSound, true, init, false);
    }
}
