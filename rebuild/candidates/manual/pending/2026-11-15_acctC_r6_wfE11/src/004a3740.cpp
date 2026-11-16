// CWorld::UpdateRegionLoad() -- the region-load state machine run from UpdateFrame: quest-region confirmation, fade out, teleport particles and followers, region change, placing the hero and followers, mini-map / camera / weapons set-up, fade in
// Grade: RELOCATION_MATCH (wfE11 eighth session)
// Leaf: ?UpdateRegionLoad@CWorld@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004A3740, 8948 bytes (code to 0x004A5A0F, 9-entry switch table 0x004A5A10), thiscall.
// Inferred: 80 dead release asserts (FABLE_RETAIL_ASSERT, fable_assert.h, device d) at the top of the function. FableWin.pdb's
//   UpdateRegionLoad is 23130 bytes / 5206 instructions v 11.9 KB / 3302 instructions for this source at /Od: ~1900 instructions of
//   debug-only ASSERT code. Their dead arms set c2's caller-size budget: retail calls bitset::test at the last 7 HasTC sites and
//   IS_THING_ALIVE at 0x004A543B / 0x004A56F2 / 0x004A57AE; the call list is retail's for 80..86 asserts (76: one call short, 88: one more).
//   Position (anywhere before case 8) and condition text (a pointer test) are byte-neutral (measured).
// FITTED (chosen by grading, ordinary spellings): (1) case 8 `if (!g_bQuestFinishScreensEnabled) SetSaveGameMarkerPos(..); else if (..)
//   SetSaveGameMarkerPos(..);` -- c2 cross-jumps the two calls into retail's one; the single `a || (b && c)` call schedules `push eax`
//   before `mov ecx,ebp` (retail 0x004A55A4 has it after). (2) case 6 `bool has_navigation = PeekTC(..); if (has_navigation && Directed)
//   {..} else if (has_navigation && Wander) {..} else {..}` with RotateXY(angle_step) in all three arms -- evidence: the frame packer
//   needs angle_step at IL weight 4 (traced) and retail has one ResetPosition(start + direction) temporary; c2 merges the equal arms.
// EVIDENCED: SetText takes implicit CCharString temporaries (retail gives the four strings their own frame slots: exposed);
//   ForceToHaveNoTarget via CThing::GetTCOfLeafType<CTCTargetingPlayer> (Ego_r.pdb; its temporaries share slots as retail's);
//   the two quest-lock tests name the region first (`const CRegion& region = ...; if (Q.M(region))`: retail loads the quest manager
//   at the call, 0x004A3810 / 0x004A4666); case-5 `CTCCreatureOpinionOfHero* opinion; if (GetTC(id, &opinion))` (0x004A4A83).
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#include "fable_assert.h"
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQ
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_HASTC_FORCEINLINE
#define FABLE_RETAIL_CTHING_GETTCOFLEAFTYPE
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
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_REMOVE
#define FABLE_RETAIL_PLAYER_REMOVEMODE
#define FABLE_RETAIL_CTHINGMANAGER_ALLPAUSE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_SQUAREDMAGNITUDE_YX
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C2DVECTOR_GFGET2DDISTANCESQUARED
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR_CTOR4_INITIALISE
#define FABLE_RETAIL_CMESSAGEEVENT
#define FABLE_RETAIL_CCHARSTRING_STARTSWITH
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCFOLLOWED
#define FABLE_RETAIL_CWORLD_UPDATEREGIONLOAD_CALLEES
#define FABLE_RETAIL_CWORLD_SAVEGAMESTATE
#define FABLE_RETAIL_CWORLD_AUTOSAVEQUESTSTART
#define FABLE_RETAIL_CWORLD_TYPED_SAVE
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTHING_STATGETTYPE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTCINVENTORYMAP_RESETWORLDMAPDISPLAY
#define FABLE_RETAIL_CTCSOUNDPLAYER_PLAYUNATTACHEDCRITERIA
#define FABLE_RETAIL_CTCVILLAGE_FINEDIALOGUE
#define FABLE_RETAIL_CTCHEROREMEMBERCRIMES
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_TAKENOUTWEAPON
#define FABLE_RETAIL_CTCCARRYING
#define FABLE_RETAIL_CTCHERO_USEOVERRIDES
#define FABLE_RETAIL_CTCHERO_TITLE
#define FABLE_RETAIL_CREGION_PREGIONDEF
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_METHODS
#define FABLE_RETAIL_CTCWIFE
#define FABLE_RETAIL_CGAMECAMERAMANAGER_FULL
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NEARBYDIRECTED
#define FABLE_RETAIL_CTHINGPLAYERCREATURE_ONREGIONCHANGE
#define FABLE_RETAIL_CTHING_LEVELPERSISTENCE
#define FABLE_RETAIL_CTCWIFE_ONHERORETURNTOREGION
#define FABLE_RETAIL_CGAMECAMERAMANAGER_RESETCAMERA
#define FABLE_RETAIL_CQUESTMANAGER_ONACTIVATEENTITY
#define FABLE_RETAIL_CTCAPPEARANCEMORPHBASE
#define FABLE_RETAIL_CWORLD_QUESTCOMPLETIONUI
#define FABLE_RETAIL_CWORLD_REGIONLOAD_ACCESSORS
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_CWORLD_GAMESCRIPTINTERFACE
#define FABLE_RETAIL_CWORLDMAP_MOVEPLAYERPOS
#define FABLE_RETAIL_CASYNCLOADINGMEMORYPOOL
#define FABLE_RETAIL_CDISPLAYENGINE_GET3DENGINE
#define FABLE_RETAIL_CTHINGAICREATURE_REGIONLOAD
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CTCHEROEXPERIENCE_ONREGIONLOAD
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_PUTWEAPONAWAY
#define FABLE_RETAIL_CTCTRESPASSER
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_REPORTCRIMES
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_MIRROREDCRIMES
#define FABLE_RETAIL_CTCVILLAGE_PEEKVILLAGEPEOPLE
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_LAYOUT
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_REGIONLOAD
#define FABLE_RETAIL_NPROGRESSDISPLAY_STARTPROGRESS
#define FABLE_RETAIL_NPROGRESSDISPLAY_STOPPROGRESS
#define FABLE_RETAIL_CTHINGCREATUREBASE_FINISHACTION
#define FABLE_RETAIL_CPLAYERGUI_MINIMAPREGIONLOAD
#define FABLE_RETAIL_CPLAYERGUI_MINIMAPDRAWING
#define FABLE_RETAIL_CQUESTMANAGER_ANYQUEST_LOCKSTART
#define FABLE_RETAIL_FILTER_BIND3_FUNCTION
#define FABLE_RETAIL_CTCINVENTORYMAP_ISOPENEDVIAGUILDSEAL
#define FABLE_RETAIL_CWORLDMAP_SETSOUNDTHEMESDISABLED
#define FABLE_RETAIL_CTCTROPHY_ISMINIGAMEINPROGRESS
#define FABLE_RETAIL_CSUBTITLERENDERER_ISGAMEPAUSEDFLAGSET
#define FABLE_RETAIL_CTHING_REGIONLOAD_TCS
#define FABLE_RETAIL_CTCHERO_TELEPORTPARTICLE
#define FABLE_RETAIL_CTCFADEOUTANDIN
#define FABLE_RETAIL_CTCREGIONDISPLAY
#define FABLE_RETAIL_CTCREGIONFOLLOWER
#define FABLE_RETAIL_CTCREGIONFOLLOWER_REGIONLOAD
#define FABLE_RETAIL_CTCTARGETINGPLAYER
#define FABLE_RETAIL_CTCTARGETINGPLAYER_FORCENOTARGET
#define FABLE_RETAIL_CTHINGCREATUREBASE_ISFOLLOWINGTELEPORTERS
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CSUBTITLERENDERER_INCUTSCENE
#define FABLE_RETAIL_CWORLD_TYPED_REGIONLOAD
#define FABLE_RETAIL_CDISPLAYENGINE_SCREENFADEOUT
#define FABLE_RETAIL_CDISPLAYENGINE_STARTSCREENFADEOUT
#define FABLE_RETAIL_CDISPLAYENGINE_REGIONLOADFADE
#define FABLE_RETAIL_CFADEINFADEOUTBASE_ISACTIVE
#define FABLE_RETAIL_CFADEINFADEOUTBASE_ISCLOSING
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS_ISOFTYPE_CTOR
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_FINDRECENT_DECL
#define FABLE_RETAIL_CSUBTITLERENDERER_SETTEXT_CHAR4
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
#include "engine/CRegionDef.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CThingManager.h"
#include "engine/CQuestManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CRGBColour.h"
#include "engine/CDisplayEngine.h"
#include "engine/CIEngine.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CGlobalDef.h"
#include "engine/CMessageEventManager.h"
#include "engine/CMessageEvent.h"
#include "engine/CMessageEventFilters.h"
#include "engine/EMessageEventType.h"
#include "engine/CRGBColour.h"
#include "engine/CMusicManager.h"
#include "engine/CASoundManager.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/ThingAlive.h"
#include "engine/CTCHero.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCTrophy.h"
#include "engine/CTCTargetingPlayer.h"
#include "engine/CTCRegionFollower.h"
#include "engine/CTCFadeOutAndIn.h"
#include "engine/CTCRegionDisplay.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCTrespasser.h"
#include "engine/CTCHeroRememberCrimes.h"
#include "engine/CTCWife.h"
#include "engine/CTCAppearanceMorphBase.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CRightHandedSet.h"
#include "engine/CAsyncLoadingMemoryPool.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CTCHeroOpinionDeedLog.h"
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
static bool GAutoSaveQuestStartOnScreenFade;                     // 0x013B89D2 (Anniversary PDB S_LDATA32, CWorld.cpp file static)
static long GExtraFramesToWaitBeforeProgressRender = 2;          // 0x01378880 (Anniversary PDB S_LDATA32 long; retail .data value 2)

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

// Same-TU callees defined above the row in retail's World.cpp (bodies = the landed rows' text / the retail bytes), so VC7.1 can expand
// them as retail does: GetFrame 0x0049D870 (landed text; expanded in SetAsStarting3DRendering, 0x004A5058), SetGuildSealRecallPos
// 0x0049C9B0 (expanded at 0x004A42xx, called at 0x004A45E7), SetGuildSealRecallAngleXY 0x0049C9D0 (expanded at 0x004A45F7).
long __fastcall CWorld::GetFrame()
{
    return Frame;
}
void CWorld::SetGuildSealRecallPos(const C3DVector& pos)
{
    GuildSealRecallPos = pos;
}
void CWorld::SetGuildSealRecallAngleXY(float angle)
{
    GuildSealRecallAngleXY = angle;
}

void CWorld::UpdateRegionLoad()
{
    CThingPlayerCreature* phero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    FABLE_RETAIL_ASSERT(phero);
    switch (RegionLoadStatus) {
    case 1: {
        if (g_bQuestFinishScreensEnabled) {
            if (!RegionLoadWaitingForConfirmation && CQuestManager::Get().IsBetweenQuestStartAndFinishScreens() &&
                !CPlayerGui::Get().GetSubtitleRenderer().IsInCutscene() && !PGameScriptInterface_ptr->IsInMovieSequence()) {
                const CRegion& region = PeekWorldMap().GetRegion(PeekWorldMap().GetRegionNumberMapIsIn(PeekWorldMap().GetMapNumberAt(RegionLoadStartPos)));
                if (!CQuestManager::Get().IsRegionValidInRegionLockingMode(region)) {
                    CPlayerGui::Get().GetSubtitleRenderer().SetText(Component->PeekMainFont(), "TEXT_GUI_LEAVE_QUEST_REGION", "", "TEXT_GUI_RELOAD", "TEXT_GUI_STAY", 0x104, 0.0f, false, true);
                    RegionLoadWaitingForConfirmation = true;
                }
            }
            bool confirmed = false;
            if (RegionLoadWaitingForConfirmation) {
                if (!Component->GetDisplayEngine().IsScreenFadeOut()) {
                    const CMessageEvent* pevent = GetMessageEventManager().FindMostRecentMessage(CMessageEventFilter_IsOfType(MESSAGE_EVENT_CONVERSATION_ANSWER_YES_OR_NO));
                    if (pevent) {
                        if (pevent->ExtraData == 1) {
                            RegionLoadWaitingForConfirmation = false;
                            RegionLoadStatus = (ERegionLoadStatus)0;
                            AutoLoadRegionLeaving();
                        } else if (pevent->ExtraData == 0) {
                            confirmed = true;
                            if (!RegionLoadViaTeleporter) {
                                if (PlayerManager->GetMainPlayer().GetCurrentMode() != PLAYER_MODE_FREEZE_CONTROLS)
                                    PlayerManager->GetMainPlayer().AddMode(PLAYER_MODE_FREEZE_CONTROLS, false);
                                Component->GetDisplayEngine().StartScreenFadeOut(1.0f, 1.0f, CRGBColour(0, 0, 0, 255), -1.0f);
                            }
                        }
                        GetMessageEventManager().RemoveMessage(pevent);
                    }
                }
                if (RegionLoadViaTeleporter) {
                    if (!confirmed)
                        return;
                    RegionLoadWaitingForConfirmation = false;
                    RegionLoadStatus = (ERegionLoadStatus)0;
                    RegionLoadStartPos = C3DVector(0.0f, 0.0f, 0.0f);
                    RegionLoadStartAngleXY = 0.0f;
                    return;
                }
                if (!Component->GetDisplayEngine().IsScreenFullyFadedOut())
                    return;
                long region = PeekWorldMap().GetRegionNumberMapIsIn(phero->GetCurrentMapNumber());
                CThing* pregion_entrance = 0;
                if (GetThingSearchTools().GetPBestThing(GFFilter_Bind2(CThingFilter_HasTC(TCI_REGION_ENTRANCE), CThingFilter_InRegion(PeekWorldMap(), region)),
                                                        CThingCompare_Nearest(phero->GetPos()), &pregion_entrance))
                    phero->GetPhysicsTC()->ResetPosition(pregion_entrance->GetPos());
                RegionLoadWaitingForConfirmation = false;
                RegionLoadStatus = (ERegionLoadStatus)0;
                RegionLoadStartPos = C3DVector(0.0f, 0.0f, 0.0f);
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
            pfade->FadeOut(DefinitionManager->PeekGlobalDef().TeleportFadeOutTimeInSeconds, false);
            CThing* pteleport_out_particle = CTCDParticleEmitter::Create(DefinitionManager->PeekGlobalDef().TeleportOutParticleEffect, phero->GetPos(), false);
            if (IS_THING_ALIVE(pteleport_out_particle)) {
                CTCDParticleEmitter* pparticle_tcd = 0;
                pteleport_out_particle->GetTCDriver(&pparticle_tcd);
                pparticle_tcd->AttachToThing(*phero, 8, "", 0, 0);
                CTCHero* phero_tc;
                if (phero->GetTC(TCI_HERO, &phero_tc))
                    phero_tc->SetTeleportParticle(*pteleport_out_particle);
            }
            CTCRegionFollower* pregion_follower;
            if (phero->GetTC(TCI_REGION_FOLLOWER, &pregion_follower)) {
                std::vector<CThing*> followers;
                pregion_follower->GetFollowers(followers);
                for (unsigned long i = 0; i < followers.size(); ++i) {
                    CThingAICreature* pfollower = static_cast<CThingAICreature*>(followers[i]);
                    if (pfollower->GetType() == 1 &&
                        PeekWorldMap().GetRegionNumberMapIsIn(pfollower->GetCurrentMapNumber()) == PeekWorldMap().GetRegionNumberMapIsIn(phero->GetCurrentMapNumber()) &&
                        pfollower->IsFollowingHeroThroughTeleporters() && pregion_follower->IsCreatureFollowingHeroAcrossRegions(*pfollower) &&
                        pregion_follower->IsCreatureAllowedInRegionAt(*pfollower, RegionLoadStartPos)) {
                        pfade->AddThingToFade(*pfollower);
                        CThing* pfollower_teleport_out_particle = CTCDParticleEmitter::Create(DefinitionManager->PeekGlobalDef().TeleportOutParticleEffect, pfollower->GetPos(), false);
                        if (IS_THING_ALIVE(pfollower_teleport_out_particle)) {
                            CTCDParticleEmitter* pparticle_tcd = 0;
                            pfollower_teleport_out_particle->GetTCDriver(&pparticle_tcd);
                            pparticle_tcd->SetAlwaysUpdate(true);
                        }
                    }
                }
            }
            {
                CTCTargetingPlayer* ptargeting;
                if (phero->GetTCOfLeafType(&ptargeting))
                    ptargeting->ForceToHaveNoTarget(true);
            }
            if (!GetThingManager().IsAllEntitesPauseMode()) {
                PGameScriptInterface_ptr->PauseAllEntities(true);
                PutIntoPauseModeOnRegionChange = true;
            }
            SetAsPaused(false);
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
            if (pthing_carried->GetTC(TCI_TROPHY, &ptrophy) && ptrophy->IsMiniGameInProgress()) {
                ptrophy->EndMinigame();
                RegionLoadWaitingForConfirmation = true;
            }
        }
        if (RegionLoadWaitingForConfirmation)
            break;
        if (CPlayerGui::Get().GetSubtitleRenderer().IsDisplayingText() && CPlayerGui::Get().GetSubtitleRenderer().IsGamePausedFlagSet())
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
        GetMessageEventManager().AddMessage(MESSAGE_EVENT_REGION_PREUNLOAD, 50, C3DVector(0.0f, 0.0f, 0.0f), 0, 0, 0.0f, 0.0f, PeekWorldMap().GetLoadedRegion());
        RegionLoadStatus = (ERegionLoadStatus)4;
        return;
    }
    case 4: {
        if (PSoundManager) {
            PMusicManager_Data->PauseMusic(true);
            PMusicManager_Data->Update();
            GetWorldMap().SetSoundThemesDisabled(true);
            PSoundManager->FreezeAtmosMaps(true);
            PSoundManager->SetGainFader(0x8000, 0.0f, 3000.0f);
        }
        RegionLoadScreenFullyFaded = false;
        GAutoSaveQuestStartOnScreenFade = false;
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
            if (phero->PeekTC(TCI_INVENTORY_MAP, &pinventory_map) && pinventory_map && pinventory_map->IsOpenedViaGuildSeal()) {
                const CRegion& region = PeekWorldMap().GetRegion(PeekWorldMap().GetRegionNumberMapIsIn(phero->GetCurrentMapNumber()));
                if (region.GetName() == "HeroGuildComplexInside" ||
                    (region.GetName().StartsWith("Prison") && region.GetName() != "PrisonPath")) {
                    SetGuildSealRecallPos(C3DVector(0.0f, 0.0f, 0.0f));
                    SetGuildSealRecallAngleXY(0.0f);
                } else {
                    long regionIndex = PeekWorldMap().GetRegionNumberMapIsIn(phero->GetCurrentMapNumber());
                    CThing* precall_region_entrance = 0;
                    GetThingSearchTools().GetPBestThing(GFFilter_Bind3(CThingFilter_HasTC(TCI_REGION_ENTRANCE), CThingFilter_InRegion(PeekWorldMap(), regionIndex),
                                                                       CThingFilter_IsRegionEntranceActive()),
                                                        CThingCompare_Nearest(phero->GetPos()), &precall_region_entrance);
                    CThing* precall_teleporter = 0;
                    if (IsTeleportingEnabled())
                        GetThingSearchTools().GetPBestThing(GFFilter_Bind2(CThingFilter_HasTC(TCI_TELEPORTER), CThingFilter_InRegion(PeekWorldMap(), regionIndex)),
                                                            CThingCompare_Nearest(phero->GetPos()), &precall_teleporter);
                    CThing* precall_thing = 0;
                    float angle = 0.0f;
                    if (precall_region_entrance) {
                        if (precall_teleporter) {
                            if (GFGet2DDistanceSquared(phero->GetPos(), precall_region_entrance->GetPos()) > GFGet2DDistanceSquared(phero->GetPos(), precall_teleporter->GetPos())) {
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
        if (g_bQuestFinishScreensEnabled && !CQuestManager::Get().IsBetweenQuestStartAndFinishScreens() && CQuestManager::Get().IsAnyQuestActive()) {
            const CRegion& region = PeekWorldMap().GetRegion(PeekWorldMap().GetRegionNumberMapIsIn(PeekWorldMap().GetMapNumberAt(RegionLoadStartPos)));
            if (CQuestManager::Get().IsRegionStartOfRegionLockingMode(region)) {
                long regionIndex = PeekWorldMap().GetRegionNumberMapIsIn(phero->GetCurrentMapNumber());
                CThing* pregion_entrance = 0;
                if (GetThingSearchTools().GetPBestThing(GFFilter_Bind2(CThingFilter_HasTC(TCI_REGION_ENTRANCE), CThingFilter_InRegion(PeekWorldMap(), regionIndex)),
                                                        CThingCompare_Nearest(phero->GetPos()), &pregion_entrance)) {
                    SetSaveGameMarkerPos(pregion_entrance->GetPos());
                    GAutoSaveQuestStartOnScreenFade = true;
                }
            }
        }
        GExtraFramesToWaitBeforeProgressRender = 2;
        PreviousRegion = PeekWorldMap().GetRegionNumberMapIsIn(phero->GetCurrentMapNumber());
        RegionLoadStatus = (ERegionLoadStatus)5;
        return;
    }
    case 5: {
        if (!RegionLoadScreenWasFadedOut) {
            CDisplayEngine& display_engine = Component->GetDisplayEngine();
            if (!display_engine.IsScreenFullyFadedOut() && !display_engine.IsScreenFadeOutClosing())
                break;
        }
        if (!RegionLoadScreenFullyFaded) {
            const CRegion& region = PeekWorldMap().GetRegion(PeekWorldMap().GetRegionNumberMapIsIn(PeekWorldMap().GetMapNumberAt(RegionLoadStartPos)));
            CPlayerGui::Get().SetMiniMapRegion(region);
            if (IsRegionLoadToDisplayRegionName() && IsDisplayingMiniMapOnRegionLoad()) {
                CTCRegionDisplay* pregion_display = 0;
                phero->GetTC(TCI_REGION_DISPLAY, &pregion_display);
                pregion_display->SetCurrentAreaName(region.GetDisplayName(), -1.0f, false, true, false);
            }
            if (GAutoSaveQuestStartOnScreenFade)
                AutoSaveQuestStart();
            RegionLoadScreenFullyFaded = true;
            return;
        }
        if (GAutoSaveQuestStartOnScreenFade) {
            GAutoSaveQuestStartOnScreenFade = false;
            return;
        }
        if (IsDisplayingMiniMapOnRegionLoad() && !CPlayerGui::Get().IsMiniMapFinishedOpening())
            break;
        if (GExtraFramesToWaitBeforeProgressRender > 0) {
            --GExtraFramesToWaitBeforeProgressRender;
            return;
        }
        NProgressDisplay::StartProgress(false, false, false, !IsDisplayingMiniMapOnRegionLoad(), false, false);
        phero->FinishCurrentAction();
        CPlayerGui::Get().GetPHeroInfoScreensManager()->RemoveAllPlayerModes();
        if (CPlayerGui::Get().GetPHeroInfoScreensManager()->IsOpen())
            CPlayerGui::Get().GetPHeroInfoScreensManager()->Close();
        std::list<CThing*> villages;
        GetThingSearchTools().GetAllThings<CThing>(CThingFilter_HasTC(TCI_VILLAGE), villages);
        if (phero->HasTC(TCI_TRESPASSER))
            ((CTCTrespasser&)phero->GetTC(TCI_TRESPASSER)).LeavingRegion();
        if (phero->HasTC(TCI_HERO_OPINION_DEED_LOG))
            ((CTCHeroOpinionDeedLog&)phero->GetTC(TCI_HERO_OPINION_DEED_LOG)).PlayerPutWeaponAway();
        if (phero->HasTC(TCI_HERO_EXPERIENCE))
            ((CTCHeroExperience&)phero->GetTC(TCI_HERO_EXPERIENCE)).OnRegionLoad();
        for (std::list<CThing*>::iterator it = villages.begin(); it != villages.end(); ++it) {
            CThing* pvillage_thing = *it;
            if (IS_THING_ALIVE(pvillage_thing)) {
                CTCVillage* pvillage = 0;
                pvillage_thing->GetTC(TCI_VILLAGE, &pvillage);
                for (std::list<CIntelligentPointer<CThingAICreature> >::const_iterator person = pvillage->PeekVillagePeople().begin();
                     person != pvillage->PeekVillagePeople().end(); ++person) {
                    if (IS_THING_ALIVE(*person)) {
                        CThingAICreature* pperson = person->Get();
                        CTCCreatureOpinionOfHero* opinion;
                        if (pperson->GetTC(TCI_CREATURE_OPINION_OF_HERO, &opinion))
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
                CThingAICreature* pfollower = static_cast<CThingAICreature*>(followers[i]);
                if (pfollower->GetType() != 1)
                    continue;
                if ((!RegionLoadViaTeleporter || pfollower->IsFollowingHeroThroughTeleporters()) && pregion_follower->IsCreatureFollowingHeroAcrossRegions(*pfollower) &&
                    pregion_follower->IsCreatureAllowedInRegionAt(*pfollower, RegionLoadStartPos)) {
                    if (PeekWorldMap().GetRegionNumberMapIsIn(pfollower->GetCurrentMapNumber()) == PeekWorldMap().GetRegionNumberMapIsIn(phero->GetCurrentMapNumber())) {
                        pfollower->ForceAIReset();
                        pfollower->SetInLimbo(true);
                        RegionLoadFollowers.push_back(pfollower);
                    }
                } else {
                    if (phero->HasTC(TCI_FOLLOWED))
                        ((CTCFollowed&)phero->GetTC(TCI_FOLLOWED)).RemoveFollower(*pfollower);
                    if (!pfollower->IsRegionFollowingOverriddenFromScript())
                        pregion_follower->RemoveFollower(*pfollower);
                }
            }
        }
        phero->SetInLimbo(true);
        phero->GetPhysicsTC()->ResetMovementState();
        GetWorldMap().StartMovePlayerPos(phero->GetPlayerNumber(), RegionLoadStartPos, CWorldMap::WAIT_FOR_LOAD);
        Component->GetDisplayEngine().Get3DEngine().Enable3DSceneRendering(false);
        RegionLoadStatus = (ERegionLoadStatus)6;
        return;
    }
    case 6: {
        CAsyncLoadingMemoryPool::ProcessLoading();
        if (PeekWorldMap().IsLoadingAnyMap()) {
            GetWorldMap().Update();
            return;
        }
        GetWorldMap().EndMovePlayerPos();
        std::list<CThingAICreature*> people;
        GetThingSearchTools().GetAllThings<CThingAICreature>(CThingFilter_HasTC(TCI_WIFE), people);
        for (std::list<CThingAICreature*>::iterator it = people.begin(); it != people.end(); ++it) {
            CThingAICreature* pthing = *it;
            if (pthing && !pthing->IsDead() && !pthing->IsInLimbo() && pthing->HasTC(TCI_WIFE))
                ((CTCWife&)pthing->GetTC(TCI_WIFE)).OnHeroReturnToRegion();
        }
        phero->GetPhysicsTC()->ResetPosition(RegionLoadStartPos);
        phero->GetPhysicsTC()->SetFacingAngleXY(RegionLoadStartAngleXY);
        phero->SetInLimbo(false);
        phero->OnRegionChange();
        float angle_step = 0.5f / (float)RegionLoadFollowers.size();
        C3DVector direction_to_head_in = phero->GetPhysicsTC()->GetRHSet().GetForward();
        direction_to_head_in.RotateXY(0.25f);
        for (unsigned long i = 0; i < RegionLoadFollowers.size(); ++i) {
            CThing* pfollower = RegionLoadFollowers[i].Get();
            if (pfollower && !pfollower->IsDead()) {
                const CTCCreatureNavigation* pnavigation;
                C3DVector nearby_pos;
                bool has_navigation = pfollower->PeekTC(TCI_NAVIGATION, &pnavigation);
                if (has_navigation && pnavigation->GetNearbyDirectedPosition(RegionLoadStartPos, 2.0f, direction_to_head_in, 0.25f, nearby_pos)) {
                    pfollower->GetPhysicsTC()->ResetPosition(nearby_pos);
                    direction_to_head_in.RotateXY(angle_step);
                } else if (has_navigation && pnavigation->GetNearbyWanderPosition(RegionLoadStartPos + direction_to_head_in, 2.0f, nearby_pos, true)) {
                    pfollower->GetPhysicsTC()->ResetPosition(nearby_pos);
                    direction_to_head_in.RotateXY(angle_step);
                } else {
                    pfollower->GetPhysicsTC()->ResetPosition(RegionLoadStartPos + direction_to_head_in);
                    direction_to_head_in.RotateXY(angle_step);
                }
                pfollower->SetInLimbo(false);
                if (!pfollower->IsToKillOnLevelUnload())
                    CQuestManager::Get().OnActivateEntity(*pfollower);
            }
        }
        PrepareAtmosBanksForLoadedRegion();
        CGameCameraManager* pcamera_manager = 0;
        PCurrentGameCamera->GetCameraManager(&pcamera_manager);
        pcamera_manager->ResetCamera();
        CPlayerGui::Get().UpdateMiniMapMarkers(true);
        RegionLoadStatus = (ERegionLoadStatus)8;
        CTCAppearanceMorphBase* pappearance_morph;
        if (phero->GetTC(TCI_APPEARANCE_MORPH, &pappearance_morph))
            pappearance_morph->UploadTextureMorphsToEngine(Component->GetDisplayEngine().Get3DEngine(), 0, true, 3.0f);
        Component->GetDisplayEngine().Get3DEngine().Enable3DSceneRendering(true);
        SetAsStarting3DRendering();
        Component->GetDisplayEngine().Get3DEngine().StartNewScene(4, 10.0f);
        NProgressDisplay::StopProgress(true, false);
        return;
    }
    case 8: {
        if (IsDisplayingMiniMapOnRegionLoad() && CPlayerGui::Get().IsMiniMapDrawing() && !CPlayerGui::Get().IsMiniMapFinishedClosing())
            break;
        {
            CTCTargetingPlayer* ptargeting;
            if (phero->GetTCOfLeafType(&ptargeting))
                ptargeting->ForceToHaveNoTarget(false);
        }
        WeatherMaskingPrimitiveHandles.erase(WeatherMaskingPrimitiveHandles.begin(), WeatherMaskingPrimitiveHandles.end());
        WeatherMaskingPrimitivesSent = false;
        SetAsPaused(false);
        const CRegion& region = PeekWorldMap().GetRegion(PeekWorldMap().GetRegionNumberMapIsIn(PeekWorldMap().GetMapNumberAt(RegionLoadStartPos)));
        CPlayerGui::Get().SetMiniMapRegion(region);
        if (IsRegionLoadToDisplayRegionName() && IsDisplayingMiniMapOnRegionLoad()) {
            CTCRegionDisplay* pregion_display = 0;
            phero->GetTC(TCI_REGION_DISPLAY, &pregion_display);
            pregion_display->SetCurrentAreaName(region.GetDisplayName(), -1.0f, false, false, true);
        }
        CTCHero* phero_tc = 0;
        phero->GetTC(TCI_HERO, &phero_tc);
        CDefPointer<const CCameraManagerSetDef> pcam_man_def;
        CDefPointer<const CRegionDef> pregion_def = region.GetPRegionDef();
        if (!pregion_def.IsNull()) {
            if (!phero_tc->IsWeaponUseOverridden())
                phero_tc->SetWeaponsAsUsable(!pregion_def->PreventHeroWeaponUse);
            if (!phero_tc->IsWillUseOverridden())
                phero_tc->SetWillAsUsable(!pregion_def->PreventHeroWillUse);
            if (pregion_def->CameraManagerSetIndex > 0)
                DefinitionManager->GetDef(pregion_def->CameraManagerSetIndex, &pcam_man_def);
        } else {
            if (!phero_tc->IsWeaponUseOverridden())
                phero_tc->SetWeaponsAsUsable(true);
            if (!phero_tc->IsWillUseOverridden())
                phero_tc->SetWillAsUsable(true);
        }
        CTCInventoryWeapons* pinventory_weapons = 0;
        phero->GetTC(TCI_INVENTORY_WEAPONS, &pinventory_weapons);
        pinventory_weapons->RefreshSheathedWeapons();
        if (pcam_man_def.IsNull())
            DefinitionManager->GetDef("CAMERA_MANAGER_SET_DEFAULT", &pcam_man_def);
        CGameCameraManager* pcam_man = 0;
        if (PCurrentGameCamera->GetCameraManager(&pcam_man))
            pcam_man->SetCameraManagerSetDef(pcam_man_def);
        if (phero->HasTC(TCI_HERO_REMEMBER_CRIMES)) {
            CTCHeroRememberCrimes* phero_remember_crimes = (CTCHeroRememberCrimes*)&phero->GetTC(TCI_HERO_REMEMBER_CRIMES);
            if (phero_remember_crimes->GetHasPaidTheFine()) {
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
                phero_remember_crimes->SetHasPaidTheFine(false);
            }
        }
        SetKickPos();
        if (CTCCarrying::IsThingCarryingWeapon(*phero)) {
            CTCHeroOpinionDeedLog* pdeed_log = 0;
            phero->GetTC(TCI_HERO_OPINION_DEED_LOG, &pdeed_log);
            pdeed_log->PlayerTakenOutWeapon();
        }
        if (RegionLoadViaTeleporter)
            GetMessageEventManager().AddMessage(MESSAGE_EVENT_TELEPORTER_USED, CMainGameComponent::GetConstantFPS() * 10, C3DVector(0.0f, 0.0f, 0.0f), 0, 0, 0.0f, 0.0f, PreviousRegion);
        DoneExtraFrameUpdateBeforeRegionLoadScreenFadeIn = false;
        if (PutIntoPauseModeOnRegionChange)
            PGameScriptInterface_ptr->PauseAllEntities(false);
        RegionLoadStatus = (ERegionLoadStatus)9;
        if (!g_bQuestFinishScreensEnabled)
            SetSaveGameMarkerPos(RegionLoadStartPos);
        else if (!CQuestManager::Get().IsBetweenQuestStartAndFinishScreens() && !CQuestManager::Get().IsRegionStartOfRegionLockingMode(region))
            SetSaveGameMarkerPos(RegionLoadStartPos);
        RegionLoadStartPos = C3DVector(0.0f, 0.0f, 0.0f);
        RegionLoadStartAngleXY = 0.0f;
        CTCInventoryMap* pinventory_map = 0;
        phero->GetTC(TCI_INVENTORY_MAP, &pinventory_map);
        pinventory_map->ResetWorldMapDisplay();
        DisplayMiniMapOnRegionLoad = true;
        if (RegionLoadViaTeleporter) {
            CTCFadeOutAndIn* pfade;
            if (!phero->GetTC(TCI_FADE_OUT_AND_IN, &pfade))
                pfade = (CTCFadeOutAndIn*)phero->AddTC("CTCFadeOutAndIn", false, 0);
            pfade->FadeIn(DefinitionManager->PeekGlobalDef().TeleportFadeInTimeInSeconds, true);
            CThing* pteleport_out_particle = CTCDParticleEmitter::Create(DefinitionManager->PeekGlobalDef().TeleportInParticleEffect, phero->GetPos(), false);
            if (IS_THING_ALIVE(pteleport_out_particle)) {
                CTCDParticleEmitter* pparticle_tcd = 0;
                pteleport_out_particle->GetTCDriver(&pparticle_tcd);
                pparticle_tcd->AttachToThing(*phero, 8, "", 0, 0);
                phero_tc->SetTeleportParticle(*pteleport_out_particle);
            }
            for (unsigned long i = 0; i < RegionLoadFollowers.size(); ++i) {
                CThing* pfollower = RegionLoadFollowers[i].Get();
                if (pfollower && !pfollower->IsDead()) {
                    CThing* pfollower_teleport_out_particle = CTCDParticleEmitter::Create(DefinitionManager->PeekGlobalDef().TeleportInParticleEffect, pfollower->GetPos(), false);
                    if (IS_THING_ALIVE(pfollower_teleport_out_particle)) {
                        CTCDParticleEmitter* pparticle_tcd = 0;
                        pfollower_teleport_out_particle->GetTCDriver(&pparticle_tcd);
                        pparticle_tcd->AttachToThing(*pfollower, 8, "", 0, 0);
                    }
                }
            }
        }
        if (!RegionLoadScreenWasFadedOut && IsScreenToFadeInOnNextRegionChange())
            Component->GetDisplayEngine().EndScreenFadeOut();
        SetScreenToFadeInOnNextRegionChange(true);
        return;
    }
    case 9: {
        if (DoneExtraFrameUpdateBeforeRegionLoadScreenFadeIn) {
            CDisplayEngine& display_engine = Component->GetDisplayEngine();
            if (!display_engine.IsScreenFadeOut() || display_engine.IsScreenFadeOutLockFlagSet()) {
                DisplayCollectableRentInformation();
                for (long i = 0; i < NumberOfTimesFreezeControlsModeAddedDuringRegionLoad; ++i)
                    PlayerManager->GetMainPlayer().RemoveMode(PLAYER_MODE_FREEZE_CONTROLS);
                NumberOfTimesFreezeControlsModeAddedDuringRegionLoad = 0;
                if (PutIntoPauseModeOnRegionChange)
                    PGameScriptInterface_ptr->PauseAllEntities(false);
                CTCSoundPlayer* psound_player;
                if (RegionLoadViaDoor && phero->GetTC(TCI_SOUND_PLAYER, &psound_player))
                    psound_player->PlayUnattachedCriteria("BIG_DOOR_SLAM_SHUT", phero->GetPos(), false, false);
                RegionLoadStatus = (ERegionLoadStatus)0;
                PreviousRegion = 0;
                return;
            }
        }
        if (!DoneExtraFrameUpdateBeforeRegionLoadScreenFadeIn) {
            DoneExtraFrameUpdateBeforeRegionLoadScreenFadeIn = true;
            if (PSoundManager) {
                PMusicManager_Data->PauseMusic(false);
                GetWorldMap().SetSoundThemesDisabled(false);
                PSoundManager->FreezeAtmosMaps(false);
                PSoundManager->SetGainFader(0x8000, 1.0f, 3000.0f);
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
