// CWorld::UpdateRegionLoad() -- the region-load state machine run from UpdateFrame (DRAFT, structure from the disassembly)
// Grade: SRC_FAIL(v8948) -- full structural DRAFT from the disassembly (all 9 cases); header wiring in progress: see NOTES.md + work/ur_missing_members.txt
// Leaf: ?UpdateRegionLoad@CWorld@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004A3740, 8948 bytes (code to 0x004A5A0F, 9-entry switch table 0x004A5A10), thiscall.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_BODY
#define FABLE_RETAIL_CTHINGFILTER_HASTC_OUTOFLINE
#define FABLE_RETAIL_CWORLD_SETKICKPOS
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CSUBTITLERENDERER
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYENGINE
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_GLOBALDEF
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_REGIONLOCK
#define FABLE_RETAIL_CSUBTITLERENDERER_SETTEXT_CHAR
#define FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
#define FABLE_RETAIL_CMAINGAMECOMPONENT_MAINFONT
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_FINDRECENT
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_REMOVE
#define FABLE_RETAIL_PLAYER_REMOVEMODE
#define FABLE_RETAIL_CTHINGMANAGER_ALLPAUSE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_SQUAREDMAGNITUDE_YX
#define FABLE_RETAIL_CCHARSTRING_STARTSWITH
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayerGui.h"
#include "engine/CSubtitleRenderer.h"
#include "engine/CTCBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThingSearchTools.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CFilter_Bind3.h"
#include "engine/CThingFilter_HasTC.h"
#include "engine/CThingCompare_Nearest.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CRegion.h"
#include "engine/CThingManager.h"
#include "engine/CQuestManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CDisplayEngine.h"
#include "engine/CIEngine.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CGlobalDef.h"
#include "engine/CMessageEventManager.h"
#include "engine/CMusicManager.h"
#include "engine/CASoundManager.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/ThingAlive.h"
#include "engine/CTCHero.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCTrophy.h"
#include "engine/CTCTargetingPlayer.h"
#include "engine/CTCRegionFollower.h"
#include "engine/CTCFollowed.h"
#include "engine/CTCVillage.h"
#include "engine/CTCCreatureOpinionOfHero.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CTCInventoryWeapons.h"
#include "engine/CTCInventoryMap.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CTCHeroMorph.h"
#include "engine/CGameCameraManager.h"
#include "engine/CCameraManagerSetDef.h"
#include "engine/CDefPointer.h"
#include "engine/NProgressDisplay.h"
#include "engine/NHeroInformationScreens_CManager.h"

extern bool g_bQuestFinishScreensEnabled;                        // 0x01375741 (descriptive)
extern bool g_bQuestStartAutoSavePending;                        // 0x013B89D2 (descriptive)
extern long g_RegionLoadFramesToWait;                            // 0x01378880 (descriptive)

class CThingFilter_InRegion : public std::unary_function<CThing, bool> {
public:
    CThingFilter_InRegion(const CWorldMap& world_map, long region) : WorldMap(world_map), Region(region) { }
    bool operator()(const CThing& thing) const;
private:
    const CWorldMap& WorldMap;
    long             Region;
};
class CThingFilter_IsRegionEntranceActive : public std::unary_function<CThing, bool> {
public:
    bool operator()(const CThing& thing) const;
};

void CWorld::UpdateRegionLoad()
{
    CThingPlayerCreature* phero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    switch (RegionLoadStatus) {
    case 1: {
        if (g_bQuestFinishScreensEnabled) {
            if (!RegionLoadWaitingForConfirmation && CQuestManager::Get().IsBetweenQuestStartAndFinishScreens() &&
                !CPlayerGui::Get().IsInventoryOpen() && !PGameScriptInterface_ptr->IsInMovieSequence() &&
                !CQuestManager::Get().IsRegionValidInRegionLockingMode(
                    PeekWorldMap().GetRegion(PeekWorldMap().GetRegionNumberMapIsIn(PeekWorldMap().GetMapNumberAtPos(RegionLoadStartPos))))) {
                CCharString button_stay("TEXT_GUI_STAY");
                CCharString button_reload("TEXT_GUI_RELOAD");
                CCharString empty("");
                CCharString text("TEXT_GUI_LEAVE_QUEST_REGION");
                CPlayerGui::Get().GetSubtitleRenderer().SetText(Component->GetFontBank(), text, empty, button_reload, button_stay, 0x104, 0.0f, false, true);
                RegionLoadWaitingForConfirmation = true;
            }
            bool confirmed = false;
            if (RegionLoadWaitingForConfirmation) {
                if (!Component->GetDisplayEngine().IsScreenFadeOutActive()) {
                    const CMessageEvent* pevent = PMessageEventManager_Data->FindMostRecentMessage(CMessageEventFilter_IsOfType(MESSAGE_EVENT_SUBTITLE_BUTTON_PRESSED));
                    if (pevent) {
                        if (pevent->GetExtraData() == 1) {
                            RegionLoadWaitingForConfirmation = false;
                            RegionLoadStatus = (ERegionLoadStatus)0;
                            AutoLoadRegionLeaving();
                        } else if (pevent->GetExtraData() == 0) {
                            confirmed = true;
                            if (!RegionLoadViaTeleporter) {
                                if (PlayerManager->GetMainPlayer().GetCurrentMode() != PLAYER_MODE_FREEZE_CONTROLS)
                                    PlayerManager->GetMainPlayer().AddMode(PLAYER_MODE_FREEZE_CONTROLS, false);
                                Component->GetDisplayEngine().StartScreenFadeOut(1.0f, 1.0f, CRGBColour(0, 0, 0, 255), -1.0f);
                            }
                        }
                        PMessageEventManager_Data->RemoveMessage(pevent);
                    }
                }
                if (RegionLoadViaTeleporter) {
                    if (!confirmed)
                        return;
                    RegionLoadStartPos = C3DVector(0.0f, 0.0f, 0.0f);
                    RegionLoadWaitingForConfirmation = false;
                    RegionLoadStatus = (ERegionLoadStatus)0;
                    RegionLoadStartAngleXY = 0.0f;
                    return;
                }
                if (!Component->GetDisplayEngine().IsScreenFullyFadedOut())
                    return;
                long region = PeekWorldMap().GetRegionNumberMapIsIn(phero->GetMapNumber());
                CThing* pregion_entrance = 0;
                if (GetThingSearchTools().GetPBestThing(GFFilter_Bind2(CThingFilter_HasTC(TCI_REGION_ENTRANCE), CThingFilter_InRegion(PeekWorldMap(), region)),
                                                        CThingCompare_Nearest(phero->GetPos()), &pregion_entrance))
                    phero->GetPhysicsTC()->SetPos(pregion_entrance->GetPos());
                RegionLoadStartPos = C3DVector(0.0f, 0.0f, 0.0f);
                RegionLoadWaitingForConfirmation = false;
                RegionLoadStatus = (ERegionLoadStatus)0;
                RegionLoadStartAngleXY = 0.0f;
                Component->GetDisplayEngine().EndScreenFadeOut();
                PlayerManager->GetMainPlayer().RemoveMode(PLAYER_MODE_FREEZE_CONTROLS);
                return;
            }
        }
        if (!RegionLoadViaTeleporter) {
            SetAsPaused(true);
            phero->RemoveTC(TCI_FADE_OUT_AND_IN);
        }
        PutIntoPauseModeOnRegionChange = false;
        if (RegionLoadViaTeleporter) {
            CTCFadeOutAndIn* pfade;
            if (phero->HasTC(TCI_FADE_OUT_AND_IN))
                phero->GetTC(TCI_FADE_OUT_AND_IN, &pfade);
            else
                pfade = (CTCFadeOutAndIn*)phero->AddTC("CTCFadeOutAndIn", false, 0);
            pfade->Fade(DefinitionManager->PeekGlobalDef().TeleportFadeOutTimeInSeconds, false);
            CThing* pteleport_out_particle = CTCDParticleEmitter::Create(DefinitionManager->PeekGlobalDef().TeleportOutParticleEffect, phero->GetPos(), false);
            if (IS_THING_ALIVE(pteleport_out_particle)) {
                CTCDParticleEmitter* ptcd_particle_emitter = 0;
                pteleport_out_particle->GetTCDriver(&ptcd_particle_emitter);
                ptcd_particle_emitter->AttachToThing(phero, 8, "", 0, 0.0f);
                CTCHero* phero_tc;
                if (phero->GetTC(TCI_HERO, &phero_tc))
                    phero_tc->AddTeleportParticle(pteleport_out_particle);
            }
            CTCRegionFollower* pregion_follower;
            if (phero->GetTC(TCI_REGION_FOLLOWER, &pregion_follower)) {
                std::vector<CThing*> followers;
                pregion_follower->GetFollowers(followers);
                for (unsigned long i = 0; i < followers.size(); ++i) {
                    CThing* pfollower = followers[i];
                    if (pfollower->IsInWorld() &&
                        PeekWorldMap().GetRegionNumberMapIsIn(pfollower->GetMapNumber()) == PeekWorldMap().GetRegionNumberMapIsIn(phero->GetMapNumber()) &&
                        pfollower->IsFollowingHero() && pregion_follower->IsCreatureFollowingHeroAcrossRegions(pfollower) &&
                        pregion_follower->IsCreatureAllowedInRegionAt(pfollower, RegionLoadStartPos)) {
                        pfade->AddThingToFade(pfollower);
                        CThing* pfollower_teleport_out_particle = CTCDParticleEmitter::Create(DefinitionManager->PeekGlobalDef().TeleportOutParticleEffect, pfollower->GetPos(), false);
                        if (IS_THING_ALIVE(pfollower_teleport_out_particle)) {
                            CTCDParticleEmitter* ptcd_particle_emitter = 0;
                            pfollower_teleport_out_particle->GetTCDriver(&ptcd_particle_emitter);
                            ptcd_particle_emitter->SetAlwaysUpdate(true);
                        }
                    }
                }
            }
            CTCTargetingPlayer* ptargeting;
            if (phero->HasTC<CTCTargetingPlayer>(&ptargeting))
                ptargeting->SetTargetingLocked(true);
            if (!GetThingManager().IsAllEntitesPauseMode()) {
                PGameScriptInterface_ptr->PauseAllEntities(true);
                PutIntoPauseModeOnRegionChange = true;
            }
            if (Paused)
                Paused = false;
            PlayerManager->GetMainPlayer().AddMode(PLAYER_MODE_FREEZE_CONTROLS, false);
            ++NumberOfTimesFreezeControlsModeAddedDuringRegionLoad;
        }
        RegionLoadStatus = (ERegionLoadStatus)2;
        return;
    }
    case 2: {
        if (RegionLoadWaitingForConfirmation)
            break;
        CTCCarrying* pcarrying = 0;
        phero->GetTC(TCI_CARRYING, &pcarrying);
        CThing* pthing_carried = pcarrying->GetThingInPrimarySlot();
        if (IS_THING_ALIVE(pthing_carried)) {
            CTCTrophy* ptrophy;
            if (pthing_carried->GetTC(TCI_TROPHY, &ptrophy) && ptrophy->IsInMinigame()) {
                ptrophy->EndMinigame();
                RegionLoadWaitingForConfirmation = true;
            }
        }
        if (RegionLoadWaitingForConfirmation)
            break;
        if (CPlayerGui::Get().IsSubtitleBlocking() && CPlayerGui::Get().IsSubtitleDisplaying())
            break;
        RegionLoadStatus = (ERegionLoadStatus)3;
        return;
    }
    case 3: {
        if (PlayerManager->GetMainPlayer().GetCurrentMode() != PLAYER_MODE_FREEZE_CONTROLS) {
            PlayerManager->GetMainPlayer().AddMode(PLAYER_MODE_FREEZE_CONTROLS, false);
            ++NumberOfTimesFreezeControlsModeAddedDuringRegionLoad;
        }
        const CTCFadeOutAndIn* pfade;
        if (phero->PeekTC(TCI_FADE_OUT_AND_IN, &pfade) && pfade->IsFading())
            break;
        PMessageEventManager_Data->AddMessage(MESSAGE_EVENT_REGION_UNLOADING, 50, C3DVector(0.0f, 0.0f, 0.0f), 0, 0, 0.0f, 0.0f, PeekWorldMap().GetLoadedRegion());
        RegionLoadStatus = (ERegionLoadStatus)4;
        return;
    }
    case 4: {
        if (PSoundManager) {
            PMusicManager_Data->SetRegionLoading(true);
            PMusicManager_Data->Update();
            GetWorldMap().SetRegionLoading(true);
            PSoundManager->SetPaused(true);
            PSoundManager->FadeVolume(0x8000, 0.0f, 3000.0f);
        }
        RegionLoadScreenFullyFaded = false;
        g_bQuestStartAutoSavePending = false;
        RegionLoadScreenWasFadedOut = Component->GetDisplayEngine().IsScreenFullyFadedOut();
        if (!RegionLoadScreenWasFadedOut)
            Component->GetDisplayEngine().StartScreenFadeOut(1.0f, 1.0f, CRGBColour(0, 0, 0, 255), -1.0f);
        if (IsDisplayingMiniMapOnRegionLoad())
            RegionLoadScreenWasFadedOut = false;
        if (RegionLoadViaTeleporter)
            SetAsPaused(true);
        if (PlayerManager->GetMainPlayer().GetCurrentMode() != PLAYER_MODE_FREEZE_CONTROLS) {
            PlayerManager->GetMainPlayer().AddMode(PLAYER_MODE_FREEZE_CONTROLS, false);
            ++NumberOfTimesFreezeControlsModeAddedDuringRegionLoad;
        }
        if (!GetThingManager().IsAllEntitesPauseMode()) {
            PGameScriptInterface_ptr->PauseAllEntities(true);
            PutIntoPauseModeOnRegionChange = true;
        }
        if (RegionLoadViaTeleporter) {
            const CTCInventoryMap* pinventory_map;
            if (phero->PeekTC(TCI_INVENTORY_MAP, &pinventory_map) && pinventory_map && pinventory_map->IsGuildSealRecallEnabled()) {
                const CRegion& region = PeekWorldMap().GetRegion(PeekWorldMap().GetRegionNumberMapIsIn(phero->GetMapNumber()));
                if (region.GetName() == "HeroGuildComplexInside" ||
                    (region.GetName().StartsWith(CCharString("Prison")) && region.GetName() != "PrisonPath")) {
                    SetGuildSealRecallPos(C3DVector(0.0f, 0.0f, 0.0f));
                    SetGuildSealRecallAngleXY(0.0f);
                } else {
                    long regionIndex = PeekWorldMap().GetRegionNumberMapIsIn(phero->GetMapNumber());
                    CThing* precall_region_entrance = 0;
                    GetThingSearchTools().GetPBestThing(GFFilter_Bind3(CThingFilter_HasTC(TCI_REGION_ENTRANCE), CThingFilter_InRegion(PeekWorldMap(), regionIndex),
                                                                       CThingFilter_IsRegionEntranceActive()),
                                                        CThingCompare_Nearest(phero->GetPos()), &precall_region_entrance);
                    CThing* precall_teleporter = 0;
                    if (TeleportingEnabled)
                        GetThingSearchTools().GetPBestThing(GFFilter_Bind2(CThingFilter_HasTC(TCI_TELEPORTER), CThingFilter_InRegion(PeekWorldMap(), regionIndex)),
                                                            CThingCompare_Nearest(phero->GetPos()), &precall_teleporter);
                    CThing* precall_thing = 0;
                    float angle = 0.0f;
                    if (precall_region_entrance) {
                        if (precall_teleporter) {
                            C2DVector to_entrance = C2DVector(precall_region_entrance->GetPos()) - C2DVector(phero->GetPos());
                            C2DVector to_teleporter = C2DVector(precall_teleporter->GetPos()) - C2DVector(phero->GetPos());
                            if (to_teleporter.GetSquaredMagnitude() < to_entrance.GetSquaredMagnitude()) {
                                precall_thing = precall_teleporter;
                                angle = phero->GetPhysicsTC()->GetFacingAngleXY();
                            } else {
                                precall_thing = precall_region_entrance;
                                angle = precall_region_entrance->GetPhysicsTC()->GetFacingAngleXY();
                            }
                        } else {
                            precall_thing = precall_region_entrance;
                            angle = precall_region_entrance->GetPhysicsTC()->GetFacingAngleXY();
                        }
                    } else if (precall_teleporter) {
                        precall_thing = precall_teleporter;
                        angle = phero->GetPhysicsTC()->GetFacingAngleXY();
                    } else if (GetThingSearchTools().GetPFirstLeafThing(GFFilter_Bind2(CThingFilter_HasTC(TCI_REGION_ENTRANCE), CThingFilter_InRegion(PeekWorldMap(), regionIndex)),
                                                                          &precall_thing)) {
                        angle = phero->GetPhysicsTC()->GetFacingAngleXY();
                    }
                    if (precall_thing) {
                        SetGuildSealRecallPos(precall_thing->GetPos());
                        if (precall_thing->GetPhysicsTC())
                            SetGuildSealRecallAngleXY(angle);
                    }
                }
            }
        }
        if (g_bQuestFinishScreensEnabled && !CQuestManager::Get().IsBetweenQuestStartAndFinishScreens() && CQuestManager::Get().IsAnyQuestActive() &&
            CQuestManager::Get().IsRegionStartOfRegionLockingMode(
                PeekWorldMap().GetRegion(PeekWorldMap().GetRegionNumberMapIsIn(PeekWorldMap().GetMapNumberAtPos(RegionLoadStartPos))))) {
            long regionIndex = PeekWorldMap().GetRegionNumberMapIsIn(phero->GetMapNumber());
            CThing* pregion_entrance = 0;
            if (GetThingSearchTools().GetPBestThing(GFFilter_Bind2(CThingFilter_HasTC(TCI_REGION_ENTRANCE), CThingFilter_InRegion(PeekWorldMap(), regionIndex)),
                                                    CThingCompare_Nearest(phero->GetPos()), &pregion_entrance)) {
                SetSaveGameMarkerPos(&pregion_entrance->GetPos());
                g_bQuestStartAutoSavePending = true;
            }
        }
        g_RegionLoadFramesToWait = 2;
        PreviousRegion = PeekWorldMap().GetRegionNumberMapIsIn(phero->GetMapNumber());
        RegionLoadStatus = (ERegionLoadStatus)5;
        return;
    }
    case 5: {
        if (!RegionLoadScreenWasFadedOut) {
            CDisplayEngine& display_engine = Component->GetDisplayEngine();
            if (!display_engine.IsScreenFullyFadedOut() && !display_engine.IsScreenFadeOutHeld())
                break;
        }
        if (!RegionLoadScreenFullyFaded) {
            const CRegion& region = PeekWorldMap().GetRegion(PeekWorldMap().GetRegionNumberMapIsIn(PeekWorldMap().GetMapNumberAtPos(RegionLoadStartPos)));
            CPlayerGui::Get().SetRegionLoadRegion(region);
            if (RegionLoadDisplayRegionName && IsDisplayingMiniMapOnRegionLoad()) {
                CTCRegionDisplay* pregion_display = 0;
                phero->GetTC(TCI_REGION_DISPLAY, &pregion_display);
                pregion_display->SetCurrentAreaName(region.GetDisplayName(), -1.0f, false, true, false);
            }
            if (g_bQuestStartAutoSavePending)
                AutoSaveQuestStart();
            RegionLoadScreenFullyFaded = true;
            return;
        }
        if (g_bQuestStartAutoSavePending) {
            g_bQuestStartAutoSavePending = false;
            return;
        }
        if (IsDisplayingMiniMapOnRegionLoad() && !CPlayerGui::Get().IsRegionLoadMiniMapReady())
            break;
        if (g_RegionLoadFramesToWait > 0) {
            --g_RegionLoadFramesToWait;
            return;
        }
        NProgressDisplay::StartProgress(false, false, false, !IsDisplayingMiniMapOnRegionLoad(), false, false);
        phero->FinishCurrentAction();
        CPlayerGui::Get().GetPHeroInfoScreensManager()->RemoveAllPlayerModes();
        if (CPlayerGui::Get().GetPHeroInfoScreensManager()->IsOpen())
            CPlayerGui::Get().GetPHeroInfoScreensManager()->Close();
        std::list<CThing*> villages;
        GetThingSearchTools().GetAllThings<CThing>(CThingFilter_HasTC(TCI_VILLAGE), villages);
        if (phero->HasTC(TCI_TCBE))
            phero->GetTC(TCI_TCBE).OnRegionLoad();
        CTCBase* ptc51;
        if (phero->GetTC(TCI_TC51, &ptc51))
            ((CTCHeroMorph*)ptc51)->FinishWithDynamicTextures();
        CTCBase* ptc68;
        if (phero->GetTC(TCI_TC68, &ptc68))
            ((CTCHeroMorph*)ptc68)->OnRegionLoad();
        for (std::list<CThing*>::iterator it = villages.begin(); it != villages.end(); ++it) {
            CThing* pvillage_thing = *it;
            if (IS_THING_ALIVE(pvillage_thing)) {
                CTCVillage* pvillage = 0;
                pvillage_thing->GetTC(TCI_VILLAGE, &pvillage);
                for (std::list<CIntelligentPointer<CThing> >::iterator person = pvillage->People.begin(); person != pvillage->People.end(); ++person) {
                    if (IS_THING_ALIVE(*person)) {
                        CTCCreatureOpinionOfHero* opinion;
                        if (person->GetPItem()->GetTC(TCI_CREATURE_OPINION_OF_HERO, &opinion))
                            opinion->ReportAllCrimes(0);
                    }
                }
                pvillage->UpdateMirroredCrimeData();
            }
        }
        SetKickPos();
        RegionLoadFollowers.erase(RegionLoadFollowers.begin(), RegionLoadFollowers.end());
        CTCRegionFollower* pregion_follower;
        if (phero->GetTC(TCI_REGION_FOLLOWER, &pregion_follower)) {
            std::vector<CThing*> followers;
            pregion_follower->GetFollowers(followers);
            for (unsigned long i = 0; i < followers.size(); ++i) {
                CThing* pfollower = followers[i];
                if (!pfollower->IsInWorld())
                    continue;
                if ((!RegionLoadViaTeleporter || pfollower->IsFollowingHero()) && pregion_follower->IsCreatureFollowingHeroAcrossRegions(pfollower) &&
                    pregion_follower->IsCreatureAllowedInRegionAt(pfollower, RegionLoadStartPos)) {
                    if (PeekWorldMap().GetRegionNumberMapIsIn(pfollower->GetMapNumber()) == PeekWorldMap().GetRegionNumberMapIsIn(phero->GetMapNumber())) {
                        ((CThingCreature*)pfollower)->NotifyManager();
                        pfollower->SetInLimbo(true);
                        RegionLoadFollowers.push_back(CIntelligentPointer<CThing>(pfollower));
                    }
                } else {
                    CTCFollowed* pfollowed = 0;
                    phero->GetTC(TCI_FOLLOWED, &pfollowed);
                    pfollowed->RemoveFollower(pfollower);
                    if (!pfollower->IsScripted())
                        pregion_follower->RemoveFollower(*pfollower);
                }
            }
        }
        phero->SetInLimbo(true);
        phero->GetPhysicsTC()->ClearMovement();
        GetWorldMap().StartMovePlayerPos(phero->GetLevelNumber(), RegionLoadStartPos, 0);
        Component->GetDisplayEngine().Get3DEngine().SetRendering(false);
        RegionLoadStatus = (ERegionLoadStatus)6;
        return;
    }
    case 6: {
        CAsyncLoadingMemoryPool::ProcessLoading();
        if (PeekWorldMap().IsLoadingAnyMap()) {
            GetWorldMap().UpdateLoading();
            return;
        }
        GetWorldMap().FinishMovePlayer();
        std::list<CThing*> people;
        GetThingSearchTools().GetAllThings<CThing>(CThingFilter_HasTC(TCI_TCD7), people);
        for (std::list<CThing*>::iterator it = people.begin(); it != people.end(); ++it) {
            CThing* pthing = *it;
            if (pthing && !pthing->IsDeadOrDying() && pthing->HasTC(TCI_TCD7))
                pthing->GetTC(TCI_TCD7).OnRegionLoaded();
        }
        phero->GetPhysicsTC()->SetPos(RegionLoadStartPos);
        phero->GetPhysicsTC()->SetFacingAngleXY(RegionLoadStartAngleXY);
        phero->SetInLimbo(false);
        phero->ResetAfterRegionLoad();
        float angle_step = 0.5f / (float)RegionLoadFollowers.size();
        C3DVector direction_to_head_in = phero->GetPhysicsTC()->GetMatrix().Forward;
        direction_to_head_in.RotateAboutZ(0.25f);
        for (unsigned long i = 0; i < RegionLoadFollowers.size(); ++i) {
            CThing* pfollower = RegionLoadFollowers[i].GetPItem();
            if (pfollower && !pfollower->IsDead()) {
                const CTCCreatureNavigation* pnavigation;
                pfollower->PeekTC(TCI_CREATURE_NAVIGATION, &pnavigation);
                C3DVector nearby_pos;
                if (pnavigation->GetNearbyDirectedPosition(RegionLoadStartPos, 2.0f, direction_to_head_in, 0.25f, &nearby_pos) ||
                    pnavigation->GetNearbyWanderPosition(&(RegionLoadStartPos + direction_to_head_in), 2.0f, &nearby_pos, true))
                    pfollower->GetPhysicsTC()->SetPos(nearby_pos);
                else
                    pfollower->GetPhysicsTC()->SetPos(RegionLoadStartPos + direction_to_head_in);
                direction_to_head_in.RotateAboutZ(angle_step);
                pfollower->SetInLimbo(false);
                if (!pfollower->IsActivated())
                    CQuestManager::Get().OnActivateEntity(*pfollower);
            }
        }
        PrepareAtmosBanksForLoadedRegion();
        CGameCameraManager* pcamera_manager = 0;
        PCurrentGameCamera->GetCameraManager(&pcamera_manager);
        pcamera_manager->ResetCamera();
        CPlayerGui::Get().UpdateMiniMapMarkers(true);
        RegionLoadStatus = (ERegionLoadStatus)8;
        CTCHeroMorph* phero_morph;
        if (phero->GetTC(TCI_HERO_MORPH, &phero_morph))
            phero_morph->UploadTextureMorphsToEngine(&Component->GetDisplayEngine().Get3DEngine(), 0, true, 3.0f);
        Component->GetDisplayEngine().Get3DEngine().SetRendering(true);
        SetAsStarting3DRendering();
        Component->GetDisplayEngine().Get3DEngine().StartNewScene(4, 10.0f);
        NProgressDisplay::StopProgress(true, false);
        return;
    }
    case 8: {
        if (IsDisplayingMiniMapOnRegionLoad() && CPlayerGui::Get().IsMiniMapDrawing() && !CPlayerGui::Get().IsRegionLoadMiniMapDone())
            break;
        CTCTargetingPlayer* ptargeting;
        if (phero->HasTC<CTCTargetingPlayer>(&ptargeting))
            ptargeting->SetTargetingLocked(false);
        WeatherMaskingPrimitiveHandles.erase(WeatherMaskingPrimitiveHandles.begin(), WeatherMaskingPrimitiveHandles.end());
        WeatherMaskingPrimitivesSent = false;
        SetAsPaused(false);
        const CRegion& region = PeekWorldMap().GetRegion(PeekWorldMap().GetRegionNumberMapIsIn(PeekWorldMap().GetMapNumberAtPos(RegionLoadStartPos)));
        CPlayerGui::Get().SetRegionLoadRegion(region);
        if (RegionLoadDisplayRegionName && IsDisplayingMiniMapOnRegionLoad()) {
            CTCRegionDisplay* pregion_display = 0;
            phero->GetTC(TCI_REGION_DISPLAY, &pregion_display);
            pregion_display->SetCurrentAreaName(region.GetDisplayName(), -1.0f, false, false, true);
        }
        CTCHero* phero_tc = 0;
        phero->GetTC(TCI_HERO, &phero_tc);
        CDefPointer<CCameraManagerSetDef> pcam_man_def;
        CDefPointer<CRegionDef> pregion_def = region.GetPBaseDef();
        if (pregion_def) {
            if (!phero_tc->WeaponsLocked)
                phero_tc->SetWeaponsAsUsable(!pregion_def->WeaponsDisabled);
            if (!phero_tc->ExpressionsLocked)
                phero_tc->ExpressionsUsable = !pregion_def->ExpressionsDisabled;
            if (pregion_def->CameraManagerSet > 0)
                DefinitionManager->GetDef(pregion_def->CameraManagerSet, &pcam_man_def);
        } else {
            if (!phero_tc->WeaponsLocked)
                phero_tc->SetWeaponsAsUsable(true);
            if (!phero_tc->ExpressionsLocked)
                phero_tc->ExpressionsUsable = true;
        }
        CTCInventoryWeapons* pinventory_weapons = 0;
        phero->GetTC(TCI_INVENTORY_WEAPONS, &pinventory_weapons);
        pinventory_weapons->RefreshSheathedWeapons();
        if (!pcam_man_def)
            DefinitionManager->GetDef(CCharString("CAMERA_MANAGER_SET_DEFAULT"), &pcam_man_def);
        CGameCameraManager* pcam_man = 0;
        if (PCurrentGameCamera->GetCameraManager(&pcam_man))
            pcam_man->SetCameraManagerSetDef(pcam_man_def);
        CTCBase* ptce0;
        if (phero->GetTC(TCI_TCE0, &ptce0) && ptce0->ClearVillageCrimesOnRegionLoad) {
            std::list<CThing*> villages;
            GetThingSearchTools().GetAllThings<CThing>(CThingFilter_HasTC(TCI_VILLAGE), villages);
            for (std::list<CThing*>::iterator it = villages.begin(); it != villages.end(); ++it) {
                CThing* pvillage_thing = *it;
                if (IS_THING_ALIVE(pvillage_thing)) {
                    CTCVillage* pvillage = 0;
                    pvillage_thing->GetTC(TCI_VILLAGE, &pvillage);
                    pvillage->ClearCrimes();
                }
            }
            ptce0->ClearVillageCrimesOnRegionLoad = false;
        }
        SetKickPos();
        if (CTCCarrying::IsThingCarryingWeapon(*phero)) {
            CTCBase* ptc51 = 0;
            phero->GetTC(TCI_TC51, &ptc51);
            ((CTCHeroMorph*)ptc51)->PlayerTakenOutWeapon();
        }
        if (RegionLoadViaTeleporter)
            PMessageEventManager_Data->AddMessage(MESSAGE_EVENT_HERO_TELEPORTED, CMainGameComponent::GetConstantFPS() * 10, C3DVector(0.0f, 0.0f, 0.0f), 0, 0, 0.0f, 0.0f, PreviousRegion);
        DoneExtraFrameUpdateBeforeRegionLoadScreenFadeIn = false;
        if (PutIntoPauseModeOnRegionChange)
            PGameScriptInterface_ptr->PauseAllEntities(false);
        RegionLoadStatus = (ERegionLoadStatus)9;
        if (!g_bQuestFinishScreensEnabled ||
            (!CQuestManager::Get().IsBetweenQuestStartAndFinishScreens() && !CQuestManager::Get().IsRegionStartOfRegionLockingMode(region)))
            SetSaveGameMarkerPos(&RegionLoadStartPos);
        RegionLoadStartPos = C3DVector(0.0f, 0.0f, 0.0f);
        RegionLoadStartAngleXY = 0.0f;
        CTCInventoryMap* pinventory_map = 0;
        phero->GetTC(TCI_INVENTORY_MAP, &pinventory_map);
        pinventory_map->ResetWorldMapDisplay();
        DisplayMiniMapOnRegionLoad = true;
        if (RegionLoadViaTeleporter) {
            CTCFadeOutAndIn* pfade;
            if (phero->HasTC(TCI_FADE_OUT_AND_IN))
                phero->GetTC(TCI_FADE_OUT_AND_IN, &pfade);
            else
                pfade = (CTCFadeOutAndIn*)phero->AddTC("CTCFadeOutAndIn", false, 0);
            pfade->FadeIn(DefinitionManager->PeekGlobalDef().TeleportFadeInTimeInSeconds, true);
            CThing* pteleport_out_particle = CTCDParticleEmitter::Create(DefinitionManager->PeekGlobalDef().TeleportInParticleEffect, phero->GetPos(), false);
            if (IS_THING_ALIVE(pteleport_out_particle)) {
                CTCDParticleEmitter* ptcd_particle_emitter = 0;
                pteleport_out_particle->GetTCDriver(&ptcd_particle_emitter);
                ptcd_particle_emitter->AttachToThing(phero, 8, "", 0, 0.0f);
                phero_tc->AddTeleportParticle(pteleport_out_particle);
            }
            for (unsigned long i = 0; i < RegionLoadFollowers.size(); ++i) {
                CThing* pfollower = RegionLoadFollowers[i].GetPItem();
                if (pfollower && !pfollower->IsDead()) {
                    CThing* pfollower_teleport_out_particle = CTCDParticleEmitter::Create(DefinitionManager->PeekGlobalDef().TeleportInParticleEffect, pfollower->GetPos(), false);
                    if (IS_THING_ALIVE(pfollower_teleport_out_particle)) {
                        CTCDParticleEmitter* ptcd_particle_emitter = 0;
                        pfollower_teleport_out_particle->GetTCDriver(&ptcd_particle_emitter);
                        ptcd_particle_emitter->AttachToThing(pfollower, 8, "", 0, 0.0f);
                    }
                }
            }
        }
        if (!RegionLoadScreenWasFadedOut && ScreenToFadeInOnNextRegionChange)
            Component->GetDisplayEngine().EndScreenFadeOut();
        ScreenToFadeInOnNextRegionChange = true;
        return;
    }
    case 9: {
        if (DoneExtraFrameUpdateBeforeRegionLoadScreenFadeIn) {
            CDisplayEngine& display_engine = Component->GetDisplayEngine();
            if (!display_engine.IsScreenFadingOut() || display_engine.IsScreenFadeOutLocked()) {
                DisplayCollectableRentInformation();
                for (long i = 0; i < NumberOfTimesFreezeControlsModeAddedDuringRegionLoad; ++i)
                    PlayerManager->GetMainPlayer().RemoveMode(PLAYER_MODE_FREEZE_CONTROLS);
                NumberOfTimesFreezeControlsModeAddedDuringRegionLoad = 0;
                if (PutIntoPauseModeOnRegionChange)
                    PGameScriptInterface_ptr->PauseAllEntities(false);
                CTCSoundPlayer* psound_player;
                if (RegionLoadViaDoor && phero->GetTC(TCI_SOUND_PLAYER, &psound_player))
                    psound_player->PlayUnattachedCriteria(CCharString("BIG_DOOR_SLAM_SHUT"), phero->GetPos(), false, false);
                RegionLoadStatus = (ERegionLoadStatus)0;
                PreviousRegion = 0;
                return;
            }
        }
        if (!DoneExtraFrameUpdateBeforeRegionLoadScreenFadeIn) {
            DoneExtraFrameUpdateBeforeRegionLoadScreenFadeIn = true;
            if (PSoundManager) {
                PMusicManager_Data->SetRegionLoading(false);
                GetWorldMap().SetRegionLoading(false);
                PSoundManager->SetPaused(false);
                PSoundManager->FadeVolume(0x8000, 1.0f, 3000.0f);
            }
        } else if (!GetThingManager().IsAllEntitesPauseMode()) {
            PGameScriptInterface_ptr->PauseAllEntities(true);
            PutIntoPauseModeOnRegionChange = true;
        }
        return;
    }
    default:
        break;
    }
}
