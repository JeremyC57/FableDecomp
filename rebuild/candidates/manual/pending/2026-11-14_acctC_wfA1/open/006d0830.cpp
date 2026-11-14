// CTCWife::GlobalUpdate(bool) -- the wife's global (paused-game) state machine: divorce text, wedding fade-out (movie, fresco, sleep the villages, place wife and hero at home facing each other), the sex cutscene and its sounds, the dowry text after the fade-in
// Grade: DIFFER(2581v2584) -- wedding block: retail pushes the movie call's 8 zeros as immediates and first zeroes ebx (= loop index i) after GetWorld for FastForwardTimeTo; ours zeroes ebx before the pushes (+ `jmp ; xor ebx,ebx` on the no-movie arm), 3 bytes short; dowry block wife_def in ebp (retail ebx). wfA1 port to today's headers.
// Leaf: ?GlobalUpdate@CTCWife@@UAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006D0830, 2584 bytes, `ret 4`; CTCWife vtable 0x0125ED5C slot 26. Ego_r.pdb 0x00705510 / FableWin.pdb
//   0x02483170 ?GlobalUpdate@CTCWife@@UAEX_N@Z.
// Residue: the wedding block's movie call: retail pushes its eight zero arguments as immediates (`push 0`) and first loads ebx = 0
//   after the GetWorld call for FastForwardTimeTo; this build keeps 0 in ebx from the movie call on (xor ebx before the pushes, a
//   `jmp; xor ebx,ebx` on the no-movie arm), 3 bytes shorter, so the village loop head gets a 4-byte `lea esp,[esp]` pad. Tried:
//   default arguments on GFPlayFullScreenXBoxMovie, `/ 24.0f`, declaring the vector first (no change / worse).
// Shape: `if (!WaitingForDivorce) { ... } else { divorce }` and `if (!WaitingForSex) { wedding } else { sex }` / `{ dowry } else
//   { WaitingForSex = false; }` (retail lays the divorce, sex and WaitingForSex blocks after the others); the CCharString
//   arguments are implicit conversions (the movie name, the quest name and the dowry text symbol: VC7.1 builds an implicit temporary
//   before the other arguments and the object expression, an explicit CCharString(...) after); CWorld::GetMessageEventManager() /
//   GetGameScriptInterface() accessors (FableWin.pdb, inline) make VC7.1 evaluate the object before the arguments; GetSubDef<T>'s
//   template body is in the TU (the CWifeDef copy 0x006D3DC0 is this TU's COMDAT), so the dowry def stays in ebx across the calls;
//   `!IsDead()` gives retail's `test byte [x+91h],1 ; jne` (IsAlive's `!Dead` gives `not dl ; test dl,1`); the time manager is
//   bound to a local reference (GetWorld before the def load); the divorce block's PeekTC<const CTCBuyableHouse> (const lookup
//   0x004365B0).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYENGINE
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CSUBTITLERENDERER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCBUYABLEHOUSE
#define FABLE_RETAIL_CTCWIFE
#define FABLE_RETAIL_CHEROMARRIAGEDEF
#define FABLE_RETAIL_CGAMECAMERAMANAGER
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_FINDRECENT
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_LAYOUT
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_ISOPEN
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CDISPLAYENGINE_SCREENFADEOUT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CSUBTITLERENDERER_SETTEXT
#define FABLE_RETAIL_CTCWIFE_MARRY
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS
#define FABLE_RETAIL_CMESSAGEEVENT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCVILLAGE_TCI
#define FABLE_RETAIL_CTCPHYSICSBASE_STATTYPE
#define FABLE_RETAIL_CTHINGAICREATURE_SAVE
#define FABLE_RETAIL_CTHINGCREATUREBASE_GETSEX
#define FABLE_RETAIL_CDISPLAYENGINE_ISSCREENFULLYFADEDOUT
#define FABLE_RETAIL_CFADEINFADEOUTBASE_OPEN
#define FABLE_RETAIL_CWORLD_GAMESCRIPTINTERFACE
#define FABLE_RETAIL_CGAMETIMEMANAGER_FULL
#define FABLE_RETAIL_CTHINGCREATUREBASE_AUTOPLACENEAR
#define FABLE_RETAIL_CTCVILLAGE_ONHEROSLEPT
#define FABLE_RETAIL_CGAMECAMERAMANAGER_RESETCAMERA
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_REMOVE
#define FABLE_RETAIL_CSUBTITLERENDERER_SETTEXT_CHAR
#define FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT_SERVERFRAME_OUTOFLINE
#define FABLE_RETAIL_CTCBASE_WORLDSEED
#define FABLE_RETAIL_CTCSOUNDPLAYER_ATTACHEDDIALOGUE
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS_ISOFTYPE_CTOR
#define FABLE_RETAIL_CMAINGAMECOMPONENT_CUTSCENEFONT
#define FABLE_RETAIL_CDISPLAYENGINE_ENDSCREENFADEOUT
#define FABLE_RETAIL_CSUBTITLERENDERER_CUTSCENEFLAGS
#define FABLE_RETAIL_PLAYER_REMOVEMODE
#define FABLE_RETAIL_CTCUNHOLYPOWER_CANCELFORCUTSCENE
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CMainGameComponent.h"
#include "engine/CTCWife.h"
#include "engine/CHeroMarriageDef.h"
#include "engine/CWifeDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CThingBaseDef.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBuilding.h"
#include "engine/CThingVillage.h"
#include "engine/CThingFilter_None.h"
#include "engine/CThingSearchTools.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCVillage.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CTCBuyableHouse.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/CWorld.h"
#include "engine/CDisplayEngine.h"
#include "engine/CPlayerGui.h"
#include "engine/CSubtitleRenderer.h"
#include "engine/NHeroInformationScreens_CManager.h"
#include "engine/CMessageEvent.h"
#include "engine/CMessageEventManager.h"
#include "engine/CMessageEventFilters.h"
#include "engine/CGameTimeManager.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CGameCameraManager.h"
#include "engine/CGameScriptInterface.h"

// FableWin.pdb ?GetHomeBuilding@CThingAICreature@@QBEPAVCThingBuilding@@XZ, inline in retail (`lea ecx,[ebp+168h] ; call 0x00A01B50`,
// 0x006D09B6); body as the landed includers (0x00946400, 0x00958840) define it.
inline CThingBuilding* CThingAICreature::GetHomeBuilding() const { return HomeBuilding.Get(); }

namespace NGameText {
class CDataBank {
public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const;   // 0x009C95E0
};
bool __fastcall ConstructString(CWideString& text, long value);          // 0x009C7B70
}
class CXMVPlayerBuffers;
class CIEngine;
bool __fastcall GFPlayFullScreenXBoxMovie(const CCharString& file_name, unsigned long flags, unsigned long width, unsigned long height,
    unsigned long buffer_size, const NGameText::CDataBank* text_bank, CIEngine* engine, bool display_subtitles, float no_skip_duration,
    bool (__fastcall* pcallback)(), CXMVPlayerBuffers* pbuffers, CWideString* perror_message);   // 0x006286F0
float __fastcall GFFloatRandom(float range, unsigned long* seed);                                   // 0x004978A0
extern "C" double __cdecl atan2(double y, double x);
extern "C" double __cdecl fmod(double x, double y);
#pragma intrinsic(atan2, fmod)
inline float __fastcall GFArcTan(double opposite,double adjacent)
{ return (float)atan2(opposite,adjacent)/(2.0f*3.14159265358979323846f); }
FABLE_FORCEINLINE float __fastcall GET_ROUNDED_ANGLE(float angle)
{
    if(angle<0.0f || angle>=1.0f) {
        angle=(float)fmod(angle,1.0f);
        if(angle<0.0f) angle+=1.0f;
    }
    return angle;
}
FABLE_FORCEINLINE float C3DVector::GetAngleXY() const { return GET_ROUNDED_ANGLE(GFArcTan(X,Y)); }
extern const unsigned long INVALID_SAMPLE_ID;                                // 0x0129D25C
// The member template CDefClassBase::GetSubDef<T> (header text; retail instantiates the CWifeDef copy in this TU: 0x006D3DC0, after
// the CTCWife bodies). Its body in the TU lets VC7.1 see the out pointer is not kept, so the dowry def stays in a register.
template <class T> bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(T::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}

void CTCWife::GlobalUpdate(bool)
{
    if (!WaitingForDivorce) {
        CMainGameComponent* component = CMainGameComponent::Get();
        if (WaitingForFadeOut && component->GetDisplayEngine().IsScreenFullyFadedOut()) {
            WaitingForFadeOut = false;
            if (!WaitingForSex) {
                if (((CThingCreatureBase*)ParentThing)->GetSex() == (ESex)2) {
                    GFPlayFullScreenXBoxMovie("Data\\Video\\5_wedding_comp.xmv", 0x1b, 0, 0, 0, 0, 0, true, -1.0f, 0, 0, 0);
                    CMainGameComponent::Get()->GetWorld().GetGameScriptInterface().ActivateQuest("Hook_Fresco_11_Wedding");
                }
                InitWifeNonpersistentState();
                InitWifeAtEndOfMarriageFrescoStartFadeIn();
                CGameTimeManager& time_manager = GetWorld().GetGameTimeManager();
                time_manager.FastForwardTimeTo(PHeroMarriageDef->WeddingAfterFrescoTimeOfDayFastForwardTo * (1.0f / 24.0f), 0.0f);
                std::vector<CThingVillage*> villages;
                GetThingSearchTools().GetAllLeafThings<CThingVillage>(CThingFilter_None(), villages);
                unsigned long village_count = villages.size();
                for (unsigned long i = 0; i < village_count; ++i) {
                    CTCVillage* village = 0;
                    villages[i]->GetTC(&village);
                    village->OnHeroSlept();
                }
                CThingAICreature* wife = (CThingAICreature*)ParentThing;
                CThingBuilding* home = wife->GetHomeBuilding();
                if (home) {
                    wife->AutoPlaceNearBuilding(home, 3.0f);
                    CThingPlayerCreature* hero = PeekWorld().PeekMyPlayer()->GetPControlledCreature();
                    if (hero && !hero->IsDead()) {
                        hero->AutoPlaceNear(wife->GetPos(), 1.0f);
                        CTCPhysicsBase* physics;
                        hero->GetTC(&physics);
                        C3DVector to_wife = wife->GetPos() - hero->GetPos();
                        physics->SetFacingAngleXY(to_wife.GetAngleXY());
                        wife->GetTC(&physics);
                        physics->SetFacingAngleXY(C3DVector(-to_wife.X, -to_wife.Y, -to_wife.Z).GetAngleXY());
                    }
                }
                CGameCameraManager* camera_manager;
                if (GetWorld().GetGameCamera()->GetCameraManager(&camera_manager))
                    camera_manager->ResetCamera();
                GetWorld().GetMessageEventManager().AddMessage((EMessageEventType)0x42, CMainGameComponent::GetConstantFPS(), GetParentThing().GetPos(), 0, 0, 0.0f, 0.0f, 0);
                CMainGameComponent::Get()->GetDisplayEngine().EndScreenFadeOut();
                SendPauseEvent(false);
                WaitingForFadeIn = true;
            } else {
                CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().GetSubtitleRenderer().SetAsInCutscene(true);
                CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().GetSubtitleRenderer().SetCutsceneAsSkippable(true);
                CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().GetSubtitleRenderer().SetCutsceneAsSkippableWhilePaused(true);
                GetPlayerManager().GetMainPlayer().AddMode((EPlayerMode)0x1b, false);
                WaitingForSexCutscene = true;
                CurrentSexCutsceneSampleID = INVALID_SAMPLE_ID;
                FrameStartedSexCutscene = CMainGameComponent::Get()->GetCurrentServerFrame();
                FrameEndedSoundInSexCutscene = 0x7fffffff;
                CurrentProportionOfSoundGap = 1.0f;
            }
        } else if (WaitingForSexCutscene) {
            CTCSoundPlayer* sound_player = 0;
            ParentThing->GetTC(&sound_player);
            CMessageEventManager& messages = GetWorld().GetMessageEventManager();
            const CMessageEvent* skipped = messages.FindMostRecentMessage(CMessageEventFilter_IsOfType((EMessageEventType)0x32));
            bool cutscene_skipped = skipped != 0;
            if (skipped)
                messages.RemoveMessage(skipped);
            if (!cutscene_skipped) {
                if (!((float)(CMainGameComponent::Get()->GetCurrentServerFrame() - FrameStartedSexCutscene) >
                      CMainGameComponent::GetConstantFPS() * PHeroMarriageDef->SecsMinSoundDurationInSexCutscene)) {
                    if (!sound_player)
                        return;
                    if (CurrentSexCutsceneSampleID == INVALID_SAMPLE_ID) {
                        if (FrameEndedSoundInSexCutscene != 0x7fffffff &&
                            !((float)(CMainGameComponent::Get()->GetCurrentServerFrame() - FrameEndedSoundInSexCutscene) >
                              PHeroMarriageDef->SecsWaitAfterSoundInSexCutscene * CurrentProportionOfSoundGap * CMainGameComponent::GetConstantFPS()))
                            return;
                        CurrentSexCutsceneSampleID = sound_player->PlayAttachedDialogue(PHeroMarriageDef->DialogueGroupForSexCutscene, false, false, false, true);
                        FrameEndedSoundInSexCutscene = 0x7fffffff;
                    } else if (!sound_player->IsAttachedSoundPlaying(CurrentSexCutsceneSampleID)) {
                        CurrentSexCutsceneSampleID = INVALID_SAMPLE_ID;
                        FrameEndedSoundInSexCutscene = CMainGameComponent::Get()->GetCurrentServerFrame();
                        CurrentProportionOfSoundGap = GFFloatRandom(0.5f, GetPWorldSeed()) + 0.5;
                    }
                    return;
                }
            }
            FrameEndedSoundInSexCutscene = 0x7fffffff;
            if (!sound_player || (CurrentSexCutsceneSampleID != INVALID_SAMPLE_ID && !sound_player->IsAttachedSoundPlaying(CurrentSexCutsceneSampleID))) {
                CurrentSexCutsceneSampleID = INVALID_SAMPLE_ID;
                FrameEndedSoundInSexCutscene = CMainGameComponent::Get()->GetCurrentServerFrame();
            }
            if (!cutscene_skipped &&
                !(FrameEndedSoundInSexCutscene != 0x7fffffff &&
                  (float)(CMainGameComponent::Get()->GetCurrentServerFrame() - FrameEndedSoundInSexCutscene) >
                      CMainGameComponent::GetConstantFPS() * PHeroMarriageDef->SecsWaitAfterSoundInSexCutscene) &&
                !((float)(CMainGameComponent::Get()->GetCurrentServerFrame() - FrameStartedSexCutscene) >
                  (PHeroMarriageDef->SecsMinSoundDurationInSexCutscene + PHeroMarriageDef->SecsWaitAfterSoundInSexCutscene) * CMainGameComponent::GetConstantFPS()))
                return;
            CMainGameComponent::Get()->GetWorld().GetGameScriptInterface().RemoveAllCutSceneSkippedMessages();
            CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().GetSubtitleRenderer().SetAsInCutscene(false);
            CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().GetSubtitleRenderer().SetCutsceneAsSkippable(false);
            CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().GetSubtitleRenderer().SetCutsceneAsSkippableWhilePaused(false);
            GetPlayerManager().GetMainPlayer().RemoveMode((EPlayerMode)0x1b);
            WaitingForSexCutscene = false;
            FrameStartedSexCutscene = 0x7fffffff;
            FrameEndedSoundInSexCutscene = 0x7fffffff;
            CurrentProportionOfSoundGap = 1.0f;
            if (CurrentSexCutsceneSampleID != INVALID_SAMPLE_ID && sound_player)
                sound_player->StopAttachedSound(CurrentSexCutsceneSampleID);
            FrameEnteredLoveWithHusbandPresentAtHome = 0x7fffffff;
            CMainGameComponent::Get()->GetDisplayEngine().EndScreenFadeOut();
            SendPauseEvent(false);
            CThingPlayerCreature* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
            if (hero && !hero->IsDead()) {
                CTCHeroOpinionDeedLog* deed_log;
                if (hero->GetTC(&deed_log))
                    deed_log->PostDeed(OPINION_DEED_TYPE_WIFE_SEX_COMMENT_AFTERWARDS, hero->GetPos(), ParentThing, (CThingAICreature*)ParentThing,
                                       (EOpinionPostDeedType)1, false, false, 0, false, 0);
            }
            WaitingForFadeIn = true;
        } else if (WaitingForFadeIn && !component->GetDisplayEngine().IsScreenFadeOut()) {
            RemoveFromGlobalTCs();
            WaitingForFadeIn = false;
            if (!WaitingForSex) {
                CDefPointer<const CWifeDef> wife_def;
                long dowry = 0;
                const CThingBaseDef* base_def = &ParentThing->PeekBaseDef();
                if (base_def && base_def->GetSubDef(&wife_def))
                    dowry = wife_def->Dowry;
                if (!CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().GetPHeroInfoScreensManager()->IsOpen()) {
                    CWideString text = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_WIFE_MARRIED_RECEIVED_DOWRY");
                    NGameText::ConstructString(text, dowry);
                    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().GetSubtitleRenderer().SetText(
                        CMainGameComponent::Get()->PeekCutSceneMainFont(), text, 0x22, 0.0f, true, true);
                    WaitingForDowry = true;
                    AddToGlobalTCs();
                    SendPauseEvent(true);
                }
            } else {
                WaitingForSex = false;
            }
        } else if (WaitingForDowry) {
            CMessageEventManager& messages = GetWorld().GetMessageEventManager();
            if (messages.FindMostRecentMessage(CMessageEventFilter_IsOfType((EMessageEventType)0x11))) {
                InitWifeAtEndOfMarriageFrescoEndFadeIn();
                RemoveFromGlobalTCs();
                WaitingForDowry = false;
                SendPauseEvent(false);
            }
        }
    } else {
        CMessageEventManager& messages = GetWorld().GetMessageEventManager();
        if (messages.FindMostRecentMessage(CMessageEventFilter_IsOfType((EMessageEventType)0x11)) == 0)
            return;
        CThingAICreature* creature = (CThingAICreature*)ParentThing;
        if (!WaitingForDivorceRenting && creature && !creature->IsDead() && creature->GetHomeBuilding()) {
            const CThingBuilding* home = creature->GetHomeBuilding();
            const CTCBuyableHouse* house = 0;
            if (home->PeekTC(&house) && house->GetOwnedByPlayer()) {
                CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().GetSubtitleRenderer().SetText(
                    CMainGameComponent::Get()->PeekCutSceneMainFont(), PHeroMarriageDef->TextGuiWifeDivorceRenting, 2, 0.0f, true, true);
                WaitingForDivorceRenting = true;
                return;
            }
        }
        WaitingForDivorceRenting = false;
        WaitingForDivorce = false;
        Divorce();
        RemoveFromGlobalTCs();
        WaitingForDivorce = false;
        SendPauseEvent(false);
    }
}
