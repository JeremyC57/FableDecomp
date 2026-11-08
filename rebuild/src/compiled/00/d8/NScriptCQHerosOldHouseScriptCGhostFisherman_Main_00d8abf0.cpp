// NScript::CQ_HerosOldHouseScript::CGhostFisherman::Main() -- the ghost of the old fisherman: takes scripting control, then until the wife is attacked or he is going, cries HELP to a nearby hero every 15 seconds, aborts the mission (fades out) if the booty is dug up before the hero helps, and when talked to plays the CS_GHOSTFISH cutscenes (intro and yes/no question, the where / complaint lines, or the success scene that sends him away); at the end fades out with a ghost effect (wife attacked) or is removed
// Leaf: ?Main@CGhostFisherman@CQ_HerosOldHouseScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D8ABF0, 4306 bytes, thiscall, plain `ret`, no EH frame (sub esp,0x12c; locals seh_me, Booty,
//   GhostTimer, seh_hero, the cast map, movie and pause; every exit runs its own destructor cascade).
// Identity (proven): slot 1 (Main) of the CGhostFisherman entity vtable 0x012D50C0 (the only reference to 0x00D8ABF0 is .rdata
//   0x012D50C4), stored by the ctor inside CGhostFisherman::Alloc 0x00D8A4B0 (0x00D8A4E4); slot 0 sdd 0x00D8A630, 2 Init 0x00D8A560,
//   3 GetParentScript 0x00D8A550, 4 OnPersist 0x00D8AB70; registration "GhostFisherman" 0x00D89F38 in CQ_HerosOldHouseScript::Main
//   0x00D89E60; Ego_r.pdb / FableWin.pdb name set ?Main@CGhostFisherman@CQ_HerosOldHouseScript@NScript@@EAEXXZ (the chunk oracle
//   row still says sub_00d8abf0; ghidra_out/labels_anniversary_2026-11-04_waves23.tsv already has the PDB name).
// Callees: CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30; ScriptedThingBase IsValid 0x00CD23B9 / Release 0x00CD2770 /
//   Speak 0x007E7390 / IsPerformingScriptTask 0x007E7450 / GetScriptThing 0x007E7490 / ~ 0x007E74D0 (the dtor expands at the normal
//   ends, is called at the early returns); CBaseObject ctor 0x0099A380 / operator= 0x0099A3B0 / dtor 0x0099A430;
//   ~CCountedPointer<CScriptGameResourceObjectMovieBase> 0x006E7AB0 (declared-only specialization: retail expands the implicit movie
//   dtor around this call) and ~CScriptGameResourceObjectMovieBase 0x006E7B80 at the early returns; the cast map
//   std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>: ctor expanded (malloc 0x00BFEA0E), operator[] 0x00CD3D2E,
//   _M_erase 0x00CD344A + free 0x00BFEA14 (expanded dtor), ~map 0x00CDBFB0 at the early returns; RunCutsceneMacro 0x00CBFB7D (x6),
//   IsDistanceBetweenThingsUnder 0x00CBE2FF, FadeOutAndKillEntity 0x00CBE716 (x2), ~CTimer 0x00CD4470, ~CScriptThing 0x004AA840,
//   CCharString 0x0099EBF0 / ~ 0x0099EAE0, operator delete 0x00BFE9BC; pDSTGame 0x0143E8F8 for the CTimer bodies (RegisterTimer,
//   SetTimer, GetTimer, DeregisterTimer: slots 87-90); Game slots 7, 8, 27 (Me.MsgIsTalkedToBy), 39, 70, 72, 100, 108, 114, 264, 295,
//   296, 350, 352, 364-366, 370, 371, 379, 576, 655. 40 string literals, same order as retail (strchk: 0 mismatches).
// Layout: engine/CQ_HerosOldHouseScript.h (new, chunk 804): parent Helping +0x48, Helped +0x49, MissionAborted +0x4a, WifeAttacked
//   +0x4b; CGhostFisherman GhostGoing +0x1c, HaveTalked +0x1d.
// Shape: `while (!WifeAttacked && !GhostGoing)`; the talk block owns seh_hero, the cast map, the movie and the pause and ends with
//   FixMovieSequenceCamera(false); every arm opens with its own terminate check (VC7.1 hoists the shared call above `answer == 1`).
#include "fable_compiler.h"
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_MAP
#include <string.h>
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CQ_HerosOldHouseScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_cutscene.h"
#include "engine/CTimer.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
extern CGameScriptInterfaceBase* pDSTGame;
inline CTimer::CTimer() { TimerIndex = pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline const C3DVector& CScriptThing::GetPos() const { if(PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
bool FABLE_FASTCALL IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float);   // 0x00CBE2FF
void FABLE_FASTCALL FadeOutAndKillEntity(const CScriptThing&, float, bool, bool);                    // 0x00CBE716

void NScript::CQ_HerosOldHouseScript::CGhostFisherman::Main()
{
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    CScriptThing Booty = Game->GetThingWithScriptName("HiddenBooty");
    CTimer GhostTimer;
    GhostTimer = 0;
    while (!ParentClass.WifeAttacked && !GhostGoing) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (!HaveTalked && IsDistanceBetweenThingsUnder(Game->GetHero(), Me, 5.5f) && GhostTimer.GetTime() <= 0) {
            if (IsActiveThreadTerminating()) return;
            long Conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(Conversation, Game->GetHero());
            Game->AddLineToConversation(Conversation, "TEXT_QST_032_GHOST_FISHERMAN_HELP", false, Me, Game->GetHero());
            GhostTimer = 15;
        }
        if (!ParentClass.Helping && !Game->IsDiggingSpotEnabled(Booty)) {
            if (IsActiveThreadTerminating()) return;
            ParentClass.MissionAborted = true;
            FadeOutAndKillEntity(Me, 1.0f, true, true);
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return;
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["HERO"] = seh_hero;
            mymap["GHOST"] = seh_me;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            Game->FixMovieSequenceCamera(true);
            RunCutsceneMacro("CS_GHOSTFISH_FISH_INTRO_SETUP", mymap, 0, 0, false, false);
            if (!HaveTalked) {
                if (IsActiveThreadTerminating()) return;
                HaveTalked = true;
                RunCutsceneMacro("CS_GHOSTFISH_FISH_INTRO_1", mymap, 0, 0, false, true);
                Game->GiveHeroYesNoQuestion("TEXT_QST_032_GHOST_FISHERMAN_INTRO_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                long Answer = Game->MsgIsQuestionAnsweredYesOrNo();
                while (Answer < 0) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                    Answer = Game->MsgIsQuestionAnsweredYesOrNo();
                }
                if (IsActiveThreadTerminating()) return;
                if (Answer == 1) {
                    if (IsActiveThreadTerminating()) return;
                    ParentClass.Helping = true;
                    RunCutsceneMacro("CS_GHOSTFISH_FISH_INTRO_2", mymap, 0, 0, false, true);
                    Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_HEROS_OLD_HOUSE", Game->GetActiveQuestName(), false);
                    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_HEROS_OLD_HOUSE_OBJECTIVE_01", "", "OakBay");
                    Game->MiniMapRemoveMarker(Me);
                    Game->MiniMapAddMarker(Game->GetThingWithScriptName("FishermansWife"), "HUD_ORB_QUEST_VIGNETTE");
                } else {
                    if (IsActiveThreadTerminating()) return;
                    RunCutsceneMacro("CS_GHOSTFISH_FISH_INTRO_REFUSAL", mymap, 0, 0, false, true);
                }
            } else {
                if (IsActiveThreadTerminating()) return;
                if (ParentClass.Helping) {
                    if (IsActiveThreadTerminating()) return;
                    if (Game->IsDiggingSpotEnabled(Booty)) {
                        if (IsActiveThreadTerminating()) return;
                        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game->GetHero(), "TEXT_QST_032_GHOST_FISHERMAN_WHERE", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return;
                            }
                            if (IsActiveThreadTerminating()) return;
                        }
                    } else {
                        if (IsActiveThreadTerminating()) return;
                        if (ParentClass.Helped) {
                            if (IsActiveThreadTerminating()) return;
                            RunCutsceneMacro("CS_GHOSTFISH_FISH_SUCCESS", mymap, 0, 0, false, true);
                            Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_HEROS_OLD_HOUSE_OBJECTIVE_04", "", "OakBay");
                            GhostGoing = true;
                        } else {
                            if (IsActiveThreadTerminating()) return;
                            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                                seh_me.Speak(Game->GetHero(), "TEXT_QST_032_GHOST_FISHERMAN_COMPLAINT", GROUP_SELECT_NONE, false, true, false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game->NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return;
                                }
                                if (IsActiveThreadTerminating()) return;
                            }
                        }
                    }
                } else {
                    if (IsActiveThreadTerminating()) return;
                    Game->GiveHeroYesNoQuestion("TEXT_QST_032_GHOST_FISHERMAN_RETURN_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                    long Answer = Game->MsgIsQuestionAnsweredYesOrNo();
                    while (Answer < 0) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                        Answer = Game->MsgIsQuestionAnsweredYesOrNo();
                    }
                    if (IsActiveThreadTerminating()) return;
                    if (Answer == 1) {
                        if (IsActiveThreadTerminating()) return;
                        ParentClass.Helping = true;
                        RunCutsceneMacro("CS_GHOSTFISH_FISH_INTRO_2", mymap, 0, 0, false, true);
                        Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_HEROS_OLD_HOUSE", Game->GetActiveQuestName(), false);
                        Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_HEROS_OLD_HOUSE_OBJECTIVE_01", "", "OakBay");
                        Game->MiniMapRemoveMarker(Me);
                        Game->MiniMapAddMarker(Game->GetThingWithScriptName("FishermansWife"), "HUD_ORB_QUEST_VIGNETTE");
                    } else {
                        if (IsActiveThreadTerminating()) return;
                        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game->GetHero(), "TEXT_QST_032_GHOST_FISHERMAN_REFUSAL", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return;
                            }
                            if (IsActiveThreadTerminating()) return;
                        }
                    }
                }
            }
            Game->FixMovieSequenceCamera(false);
        }
    }
    if (IsActiveThreadTerminating()) return;
    if (ParentClass.WifeAttacked) {
        if (IsActiveThreadTerminating()) return;
        Game->CreateEffect("Ghost_Appear_01", Me.GetPos(), "", 0.0f, false, false);
        FadeOutAndKillEntity(Me, 1.0f, true, true);
    } else {
        if (IsActiveThreadTerminating()) return;
        Game->RemoveThing(Me, false, true);
    }
}