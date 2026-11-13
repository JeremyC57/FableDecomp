// NScript::CQ_BreakSiegeScript::CBanditSiegeLeader::Main() -- the bandit leader of the siege: live while alive and take scripted control; unless his demands are known, fade out, take the hero, the chief and the sheriff, play CS_BREAKSIEGE_GIVEN (HERO / LEADER) and CS_BREAKSIEGE_INTRO (with CHIEF / GUARD) with the bodyguards moved for each, block the road with the palisade, activate the Knothole Glade teleporter, kick off the quest start screen and set the objective; then mark the demands known, release himself, wait for the siege and the second wave, and each frame flag the hero's first attack on him and, once the mission succeeds, retake control, wait for the end cut-scene and remove himself
// Grade: DIFFER(3013v2965) -- 66 diff lines (34 structural): the if-block's cut-scene objects take swapped frame slots (retail: movie 0x40, cast map 0x50, hit-expression flag spill 0x5c; ours: map 0x40, spill 0x4c, movie 0x50) and retail expands ~CScriptGameResourceObjectMovieBase at BOTH its sites (the teleporter-loop return pad and the block end) where ours calls 0x006E7B80 at the pad (chunk 814: an every-site movie-dtor expansion is gated by frame-slot sharing and pad cross-jumping)
// Next idea: a spelling that moves the movie below the cast map in the frame (the GM row's for(;; NSF){if (T) return;} and block levers did not); MOVIEBASE_FORCEINLINE / implicit ctor / no-widescreen-forceinline are worse (md152 / 89 / 421)
// Leaf: ?Main@CBanditSiegeLeader@CQ_BreakSiegeScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D21B40, 2965 bytes, thiscall, plain `ret` (two epilogues), frame 0xb0.
// Identity (proven): Ego_r.pdb ?Main@CBanditSiegeLeader@CQ_BreakSiegeScript@NScript@@EAEXXZ; slot 1 of the CBanditSiegeLeader entity
//   vtable 0x012CAEE4 (the address's one .rdata reference).
// Callees: CIsThingAlive (vtable 0x012C32F0; ctor and dtor inline) + CScriptThing copy 0x004ABE90 / ~ 0x004AA840, SetPredicate 0x00F35B10,
//   IsActiveThreadTerminating 0x00F35B30, CCharString 0x0099EBF0 / ~ 0x0099EAE0, CBasicString<char>::Compare 0x00411570, ScriptedThingBase
//   (CBaseObject 0x0099A380 / ~ 0x0099A430 / operator= 0x0099A3B0, ctor 0x007E72A0, IsValid 0x00CD23B9, Release 0x00CD2770, dtor 0x007E74D0,
//   PImp operator= 0x008AB1E0), MovieBase PImp dtor 0x006E7AB0, map<CCharString, STB> ctor 0x00CDBF70 / dtor 0x00CDBFB0 / operator[]
//   0x00CD3D2E, ~vector<CScriptThing> 0x008AC970, RunCutsceneMacro 0x00CBFB7D, parent SendBodyguardsToTheRightPlace 0x00D226E0;
//   CGameScriptInterfaceBase slots 7, 8, 70, 72, 75, 99 CreateObjectOnEntity, 108 RemoveThing, 296 SetQuestCardObjective, 309
//   KickOffQuestStartScreen, 370 / 371 / 372 / 379, 465 EntityAttachToScript, 563 SetTeleporterAsActive, 655 GetActiveQuestName.
// Layout: engine/CQ_BreakSiegeScript.h CBanditSiegeLeader: ParentClass +0x14 (MissionSucceeded +0x48, BanditDemandsKnown +0x4a,
//   BanditSiegeStart +0x4d, StartSecondWave +0x4f, HeroLetInTown +0x51, EndCutsceneDone +0x54, AttackedBanditLeader +0x55,
//   QuestStartScreened +0x56).
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE_CTOR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#include "fable_string.h"
#include "engine/CCountedPointer.h"
class CScriptThing;
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();                        // 0x006E7AB0
class CScriptGameResourceObjectScriptedThingBase;
template <> CCountedPointer<CScriptGameResourceObjectScriptedThingBase>& CCountedPointer<CScriptGameResourceObjectScriptedThingBase>::operator=(const CCountedPointer<CScriptGameResourceObjectScriptedThingBase>&);   // 0x008AB1E0
#include "engine/CScriptThing.h"
#include "engine/CQ_BreakSiegeScript.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CIsThingAlive.h"
#include "engine/C3DVector.h"
#include "engine/fable_script_cutscene.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
template <> std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>::map();                       // 0x00CDBF70
template <> std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>::~map();                      // 0x00CDBFB0
template <> std::vector<CScriptThing>::~vector();                                                           // 0x008AC970
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_HEAL_LIFE_SPELL=14 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }
__forceinline CScriptGameResourceObjectScriptedThingBase& CScriptGameResourceObjectScriptedThingBase::operator=(const CScriptGameResourceObjectScriptedThingBase& other) { CBaseObject::operator=(other); PImp = other.PImp; return *this; }
template<> inline __declspec(noinline) long __fastcall CBasicString<char>::Compare(const char* a, const char* b)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}

void NScript::CQ_BreakSiegeScript::CBanditSiegeLeader::Main()
{
    {
        CIsThingAlive alive(Me);
        SetPredicate(alive);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid())
        seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (!ParentClass.BanditDemandsKnown) {
        if (IsActiveThreadTerminating())
            return;
        Game->FadeScreenOutUntilNextCallToFadeScreenIn(0.5f, 0.0f);
        CScriptThing chief = Game->GetThingWithScriptName("KG_ChiefBreakSiege");
        CScriptThing guard = Game->GetThingWithScriptName("KG_Sheriff");
        CScriptGameResourceObjectScriptedThingBase seh_hero;
        CScriptGameResourceObjectScriptedThingBase seh_chief;
        CScriptGameResourceObjectScriptedThingBase seh_guard;
        Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
        Game->StartScriptingEntity(chief, seh_chief, SCRIPT_AI_PRIORITY_HIGHEST);
        Game->StartScriptingEntity(guard, seh_guard, SCRIPT_AI_PRIORITY_HIGHEST);
        ParentClass.SendBodyguardsToTheRightPlace("BodyguardTeleportMarker1_BSGIVEN", "BodyguardTeleportMarker2_BSGIVEN");
        std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
        cast["HERO"] = seh_hero;
        cast["LEADER"] = seh_me;
        CScriptGameResourceObjectMovieBase cutscene_movie;
        Game->StartMovieSequence("", cutscene_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_BREAKSIEGE_GIVEN", cast, 0, 0, false, true);
        cast["HERO"] = seh_hero;
        cast["LEADER"] = seh_me;
        cast["CHIEF"] = seh_chief;
        cast["GUARD"] = seh_guard;
        ParentClass.SendBodyguardsToTheRightPlace("BodyguardTeleportMarker1_BSINTRO1", "BodyguardTeleportMarker2_BSINTRO1");
        RunCutsceneMacro("CS_BREAKSIEGE_INTRO", cast, 0, 0, false, true);
        Game->FixMovieSequenceCamera(false);
        ParentClass.SendBodyguardsToTheRightPlace("BodyguardTeleportMarker1_BSINTRO2", "BodyguardTeleportMarker2_BSINTRO2");
        CScriptThing blockage = Game->CreateObjectOnEntity("OBJECT_BANDIT_PALISADES_01", Game->GetThingWithScriptName("M_Blockage"), "Blockage");
        Game->EntityAttachToScript(blockage, Game->GetActiveQuestName());
        std::vector<CScriptThing> teleporters;
        Game->GetAllThingsWithScriptName("TeleportPoint", teleporters);
        for (unsigned long i = 0; i < teleporters.size(); ++i) {
            if (IsActiveThreadTerminating())
                return;
            if (teleporters[i].GetDataString() == "KnotholeGlade") {
                if (IsActiveThreadTerminating())
                    return;
                Game->SetTeleporterAsActive(teleporters[i], true);
                break;
            }
        }
        if (IsActiveThreadTerminating())
            return;
        if (!ParentClass.QuestStartScreened) {
            if (IsActiveThreadTerminating())
                return;
            Game->KickOffQuestStartScreen(Game->GetActiveQuestName(), true, false);
            ParentClass.QuestStartScreened = true;
        }
        ParentClass.HeroLetInTown = true;
        Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_BREAK_SIEGE_OBJECTIVE_02", "KnotholeGlade", "KnotholeGlade");
    }
    if (!ParentClass.QuestStartScreened) {
        if (IsActiveThreadTerminating())
            return;
        Game->KickOffQuestStartScreen(Game->GetActiveQuestName(), true, false);
        ParentClass.QuestStartScreened = true;
    }
    ParentClass.BanditDemandsKnown = true;
    seh_me.Release();
    while (!ParentClass.BanditSiegeStart) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!ParentClass.StartSecondWave) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!IsActiveThreadTerminating()) {
        if (!ParentClass.AttackedBanditLeader &&
            (Me.MsgIsHitBy("SCRIPT_NAME_HERO") ||
             (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO")) ||
             Me.MsgIsHitBy(""))) {
            if (IsActiveThreadTerminating())
                return;
            ParentClass.AttackedBanditLeader = true;
        }
        if (ParentClass.MissionSucceeded) {
            if (IsActiveThreadTerminating())
                return;
            if (seh_me.IsValid())
                seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            while (!ParentClass.EndCutsceneDone) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            Game->RemoveThing(Me, false, true);
        }
        Game->NewScriptFrame();
    }
}
