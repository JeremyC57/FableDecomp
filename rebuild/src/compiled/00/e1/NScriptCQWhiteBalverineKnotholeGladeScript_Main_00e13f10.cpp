// NScript::CQ_WhiteBalverineKnotholeGladeScript::Main() -- the Knothole Glade white balverine quest main: wait for KnotholeGlade, activate V_KnotholeGladeGates on a fresh start, set objective 1, bind the six entity scripts, take the chief and the village (limbo on), kick off the quest start screen once, spawn WatchForTermination, then run the BalverineState stages: 0/1 wait for the gates quest to end; 2 respawn the balverine if dead, objective 2, wait for stage 3; 3 play CS_WBK_CHIEF1, objective 3, wait for the hero at the ambush marker, objective 4; 4 respawn the balverine above WB_EscapePoint2, wait for stage 5; 5 play CS_WBK_CHIEF2, objective 5, experience and renown, the tutorial; 6 respawn the balverine and the two guards, wait for stage 7; 7 play CS_WBK_CHIEF3 and mark the mission succeeded
// Leaf: ?Main@CQ_WhiteBalverineKnotholeGladeScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E13F10, 5589 bytes, thiscall, plain `ret`, no EH frame; three exits (before the ebp/edi pushes,
//   before the chief / village locals, and the shared ~CScriptThing pair).
// Identity (proven): slot 2 (Main) of the quest-script vtable 0x012E0768 (RegisterMain 0x00E13E20 spawns it as "Main"); Ego_r.pdb
//   ?Main@CQ_WhiteBalverineKnotholeGladeScript@NScript@@UAEXXZ. Every bound Alloc is a landed CQ_WhiteBalverineKnotholeGladeScript::C*::Alloc
//   row (binding literal = class name less its C); the thread is 0x00E15500 under the literal "WatchForTermination" = Ego_r.pdb
//   ?WatchForTermination@CQ_WhiteBalverineKnotholeGladeScript@NScript@@QAEXXZ (landed under an invented name: NAME_FIXES.tsv).
// Callees: operator new 0x00BFEA1A, CCharString 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0, CScriptBase::AddEntityScriptBinding 0x00CB8230 /
//   PostAddScriptedEntities 0x00CB8930 / AddSpawnedFunction 0x00CB7E50 / IsActiveThreadTerminating 0x00CB7940, CSpawnedFuncBase 0x00CDD450,
//   ~CScriptThing 0x004AA840, scripted-thing handle ctor 0x007E72A0 / dtor 0x007E74D0 / operator= 0x008ABD10 / IsValid 0x00CD23B9 /
//   Release 0x00CD2770, movie ctor 0x006E7B60 / dtor 0x006E7B80, cast map ctor 0x00CDBF70 / dtor 0x00CDBFB0 / operator[] 0x00CD3D2E,
//   RunCutsceneMacro 0x00CBFB7D, IsDistanceBetweenThingsUnder 0x00CBE2FF, __ftol2 0x00BFEA70; pGlobalScriptDef 0x0143E90C
//   (EXPERIENCE_GRANT_SMALL +0x3C, WBK_RenownAwardForSecondDefence +0xFA0); 9.0f 0x0125EF98.
// Layout: engine/CQ_WhiteBalverineKnotholeGladeScript.h (new, chunk 799; Ego_r.pdb offsets - 0x14).
// Shape: the landed quest-Main binding model (CQ_RansomVictimScript::Main 0x00DE0850) with FABLE_RETAIL_CENTITYSCRIPTBINDING_FORCEINLINE_CTOR
//   (a plain inline ctor stays a call in this 5.5 KB body, as in the TU of 0x00E479B0; retail expands all six). The stage tests are
//   plain ifs on BalverineState (VC7.1 keeps the next stage number in a register across each block); the gates-quest wait is
//   spelled `!= false` (test / setne, 0x00D50800 precedent); the escape-point offset is `GetPos() + C3DVector(0, 0, 9.0f)` (the `+ 0`
//   terms fold, X copied as an integer, the 0x00DF48F0 precedent). The CreateCreature results are discarded temporaries.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CENTITYSCRIPTBINDING_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_WhiteBalverineKnotholeGladeScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_cutscene.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
extern CDefPointer<const CScriptDef> pGlobalScriptDef;
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();
namespace NScript {
#define FABLE_WBK_ENTITY(Name) \
    class CQ_WhiteBalverineKnotholeGladeScript::Name : public CEntityScriptBase { \
    public: \
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_WhiteBalverineKnotholeGladeScript&, CScriptDataBase*, const CScriptThing&); \
    };
FABLE_WBK_ENTITY(CWB_Villager)          // Alloc 0x00E16B50
FABLE_WBK_ENTITY(CWB_WhiteBalverine)    // Alloc 0x00E15750
FABLE_WBK_ENTITY(CWB_ScaredVillager)    // Alloc 0x00E16990
FABLE_WBK_ENTITY(CWB_Guard1)            // Alloc 0x00E16D10
FABLE_WBK_ENTITY(CWB_Guard2)            // Alloc 0x00E17230
FABLE_WBK_ENTITY(CKG_Chief)             // Alloc 0x00E17750
#undef FABLE_WBK_ENTITY
}

void NScript::CQ_WhiteBalverineKnotholeGladeScript::Main()
{
    while (!Game->IsLevelLoaded("KnotholeGlade")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (BalverineState == 0) {
        if (IsActiveThreadTerminating())
            return;
        Game->ActivateQuest("V_KnotholeGladeGates");
    }
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_WHITE_BALVERINE_KNOTHOLE_GLADE_OBJECTIVE_01", "KnotholeGlade", "KnotholeGlade");
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_WhiteBalverineKnotholeGladeScript>("WB_Villager", this, &CWB_Villager::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_WhiteBalverineKnotholeGladeScript>("WB_WhiteBalverine", this, &CWB_WhiteBalverine::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_WhiteBalverineKnotholeGladeScript>("WB_ScaredVillager", this, &CWB_ScaredVillager::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_WhiteBalverineKnotholeGladeScript>("WB_Guard1", this, &CWB_Guard1::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_WhiteBalverineKnotholeGladeScript>("WB_Guard2", this, &CWB_Guard2::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_WhiteBalverineKnotholeGladeScript>("KG_Chief", this, &CKG_Chief::Alloc, 1));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    while (!Game->IsLevelLoaded("KnotholeGlade")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing chief = Game->GetThingWithScriptName("KG_Chief");
    CScriptThing village = Game->GetThingWithScriptName("VILLAGE_KNOTHOLEGLADE");
    Game->SetVillageLimbo(village, true);
    if (!QuestStartScreenShown) {
        if (IsActiveThreadTerminating())
            return;
        QuestStartScreenShown = true;
        Game->KickOffQuestStartScreen("Q_WhiteBalverineKnotholeGlade", true, false);
    }
    AddSpawnedFunction(new CSpawnedFunc<CQ_WhiteBalverineKnotholeGladeScript>("WatchForTermination", 0, this, &CQ_WhiteBalverineKnotholeGladeScript::WatchForTermination), CCharString(""));
    if (BalverineState == 0 || BalverineState == 1) {
        if (IsActiveThreadTerminating())
            return;
        BalverineState = 1;
        while (Game->IsQuestActive("V_KnotholeGladeGates") != false) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        BalverineState = 2;
    }
    if (BalverineState == 2) {
        if (IsActiveThreadTerminating())
            return;
        if (!Game->GetThingWithScriptName("WB_WhiteBalverine").IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            Game->CreateCreature("CREATURE_FULLMOON_BALVERINE", Game->GetThingWithScriptName("WB_WhiteBalverineSpawnMarker").GetPos(), "WB_WhiteBalverine", false);
        }
        Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_WHITE_BALVERINE_KNOTHOLE_GLADE_OBJECTIVE_02", "KnotholeGlade", "KnotholeGlade");
        while (BalverineState != 3) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->StopOverrideMusic(false);
    }
    if (BalverineState == 3) {
        if (IsActiveThreadTerminating())
            return;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        while (!Game->IsHeroControlledByPlayer()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        {
            CScriptGameResourceObjectScriptedThingBase seh_chief;
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            Game->StartScriptingEntity(chief, seh_chief, SCRIPT_AI_PRIORITY_HIGHEST);
            Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["HERO"] = seh_hero;
            mymap["CHIEF"] = seh_chief;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            Game->FixMovieSequenceCamera(true);
            RunCutsceneMacro("CS_WBK_CHIEF1", mymap, 0, 0, false, true);
            Game->FixMovieSequenceCamera(false);
            GreenWife = true;
            if (seh_chief.IsValid()) seh_chief.Release();
        }
        Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_WHITE_BALVERINE_KNOTHOLE_GLADE_OBJECTIVE_03", "KnotholeGlade", "KnotholeGlade");
        while (!IsDistanceBetweenThingsUnder(Game->GetThingWithScriptName("WhiteBalverineAmbushMarker"), Game->GetHero(), 10.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_WHITE_BALVERINE_KNOTHOLE_GLADE_OBJECTIVE_04", "KnotholeGlade", "KnotholeGlade");
        BalverineState = 4;
    }
    if (BalverineState == 4) {
        if (IsActiveThreadTerminating())
            return;
        if (!Game->GetThingWithScriptName("WB_WhiteBalverine").IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            Game->CreateCreature("CREATURE_FULLMOON_BALVERINE", Game->GetThingWithScriptName("WB_EscapePoint2").GetPos() + C3DVector(0.0f, 0.0f, 9.0f), "WB_WhiteBalverine", false);
        }
        while (BalverineState != 5) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->StopOverrideMusic(false);
    }
    if (BalverineState == 5) {
        if (IsActiveThreadTerminating())
            return;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        while (!Game->IsHeroControlledByPlayer()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        WifeCutsceneStart = true;
        {
            CScriptGameResourceObjectScriptedThingBase seh_chief;
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            Game->StartScriptingEntity(chief, seh_chief, SCRIPT_AI_PRIORITY_HIGHEST);
            Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["HERO"] = seh_hero;
            mymap["CHIEF"] = seh_chief;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            Game->FixMovieSequenceCamera(true);
            RunCutsceneMacro("CS_WBK_CHIEF2", mymap, 0, 0, false, true);
            Game->FixMovieSequenceCamera(false);
            WifeCutsceneStop = true;
            if (seh_chief.IsValid()) seh_chief.Release();
        }
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_WHITE_BALVERINE_KNOTHOLE_GLADE_OBJECTIVE_05", "KnotholeGlade", "KnotholeGlade");
        Game->GiveHeroExperience(pGlobalScriptDef->EXPERIENCE_GRANT_SMALL);
        Game->GiveHeroRenownPoints((long)pGlobalScriptDef->WBK_RenownAwardForSecondDefence);
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        Game->Pause(2.0f);
        if (Game->DisplayTutorial((ETutorialCategory)40)) {
            if (IsActiveThreadTerminating())
                return;
            while (!Game->MsgIsTutorialClickedPast()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
        BalverineState = 6;
    }
    if (BalverineState == 6) {
        if (IsActiveThreadTerminating())
            return;
        if (!Game->GetThingWithScriptName("WB_WhiteBalverine").IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            Game->CreateCreature("CREATURE_FULLMOON_BALVERINE", Game->GetThingWithScriptName("WB_BalvEscape2").GetPos(), "WB_WhiteBalverine", false);
        }
        Game->CreateCreature("CREATURE_KN_GUARD", Game->GetThingWithScriptName("WB_Guard1Pos").GetPos(), "WB_Guard1", false);
        Game->CreateCreature("CREATURE_KN_GUARD", Game->GetThingWithScriptName("WB_Guard2Pos").GetPos(), "WB_Guard2", false);
        while (BalverineState != 7) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->StopOverrideMusic(false);
    }
    if (BalverineState == 7) {
        if (IsActiveThreadTerminating())
            return;
        while (!Game->IsHeroControlledByPlayer()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        {
            CScriptGameResourceObjectScriptedThingBase seh_chief;
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            Game->StartScriptingEntity(chief, seh_chief, SCRIPT_AI_PRIORITY_HIGHEST);
            Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["HERO"] = seh_hero;
            mymap["CHIEF"] = seh_chief;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            Game->FixMovieSequenceCamera(true);
            RunCutsceneMacro("CS_WBK_CHIEF3", mymap, 0, 0, false, true);
            Game->FixMovieSequenceCamera(false);
            if (seh_chief.IsValid()) seh_chief.Release();
        }
        MissionSucceeded = true;
    }
}