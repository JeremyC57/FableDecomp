// NScript::CV_ExposeMayorScript::CEM_SuspiciousVillager::Main() -- the Expose-the-Mayor prisoner: under parent scripted control, until stage 5 greet the hero when he comes near (every 7 s), at stage 4 give the outro (path 1) or vanish (path 2), play the attacked cut-scene when hit, and when talked to play the stage's cut-scene (stage 0: intro / ask-again and the yes-no question that starts stage 1)
// Leaf: ?Main@CEM_SuspiciousVillager@CV_ExposeMayorScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E776F0, 2564 bytes, thiscall, plain `ret`, no EH frame; ends in its `ret` at 0x00E780F3.
// Identity (proven): Ego_r.pdb / FableWin.pdb name set ?Main@CEM_SuspiciousVillager@CV_ExposeMayorScript@NScript@@EAEXXZ; the
//   address appears once in .rdata, at slot 1 of the entity vtable stored by the landed CEM_SuspiciousVillager::Alloc 0x00E76360
//   (its Init 0x00E76410 / GetParentScript 0x00E76400 / OnPersist 0x00E77690 are landed); DoneIntro +0x1c / DoneOutro +0x1d are the
//   PDB members, ParentClass->seh_Suspicious +0x58 the PDB handle, and every literal is a B18 prisoner line or CS_MAYOREXPOSE_PRISONER*.
// Callees: CEntityScriptBase::SetPredicate 0x00F35B10 / IsActiveThreadTerminating 0x00F35B30, CScriptThing copy 0x004ABE90 /
//   ~CScriptThing 0x004AA840 (inside the inline CIsThingConscious, vtable 0x012C2FE8 kept in ebx), handle IsValid 0x00CD23B9 /
//   Release 0x00CD2770, CCharString 0x0099EBF0 / ~ 0x0099EAE0, IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall),
//   CV_ExposeMayorScript::PlayCutscene 0x00E78100, ~CTimer 0x00CD4470 (cold exits; the hot ones expand it), movie handle:
//   CBaseObject 0x0099A380 / ~ 0x0099A430, ~CCountedPointer<movie> 0x006E7AB0, the whole movie dtor 0x006E7B80 at the cold exits;
//   CGameScriptInterfaceBase slots 7 NewScriptFrame, 8 StartScriptingEntity, 39 MsgIsQuestionAnsweredYesOrNo, 70 GetHero,
//   87-90 Register/Deregister/Set/GetTimer (through pDSTGame 0x0143E8F8), 108 RemoveThing, 114 GiveHeroYesNoQuestion,
//   364-366 conversation, 370 StartMovieSequence, 379 PauseAllNonScriptedEntities; CScriptThing slots 21 MsgIsHitBy,
//   27 MsgIsTalkedToBy, 41 MsgIsHitBySpecialAbilityFrom, 42 MsgIsHitByAnySpecialAbilityFrom.
// Layout: engine/CV_ExposeMayorScript.h (new, chunk 789): Path +0x48, StageNumber +0x4c, PrisonerTalkedAboutLamp +0x51,
//   seh_Suspicious +0x58; PMasterData->HeroMarriedLadyGrey +0xd7 (engine/CQ_SunnyvaleMasterData.h).
// Shape: generated entity-script code (the landed CMurderTwistBrotherOlderFriend::Main 0x00EACF00 model). Retail keeps
//   &ParentClass.seh_Suspicious in ebx across IsValid / Release with the parent in eax first (`mov eax,[esi+14h]; lea ebx,[eax+58h]`):
//   that is a local reference to the parent class (`CV_ExposeMayorScript& parent = ParentClass;`); a reference or pointer to the
//   handle gives `mov ebx,[esi+14h]; add ebx,58h`, and the plain member path reloads it for Release.
//   The local CTimer has plain inline ctor / dtor and __forceinline operator= / GetTime (the landed 0x00D84D60 model): the two
//   wait loops expand ~CTimer, the other exits call 0x00CD4470.
#include "fable_compiler.h"
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CV_ExposeMayorScript.h"
#include "engine/CIsThingConscious.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CTimer.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
extern CGameScriptInterfaceBase* pDSTGame;               // 0x0143E8F8
inline CTimer::CTimer() { TimerIndex = pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();

inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& thing):Thing(thing) {}
inline NScript::CIsThingConscious::~CIsThingConscious() {}
bool FABLE_FASTCALL IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float);   // 0x00CBE2FF

void NScript::CV_ExposeMayorScript::CEM_SuspiciousVillager::Main()
{
    SetPredicate(CIsThingConscious(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    { CV_ExposeMayorScript& parent = ParentClass; if (parent.seh_Suspicious.IsValid()) parent.seh_Suspicious.Release(); }
    while (!Game->StartScriptingEntity(Me, ParentClass.seh_Suspicious, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    CTimer attention_timer;
    while (ParentClass.StageNumber != 5) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (attention_timer.GetTime() == 0 && IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 15.0f)) {
            if (IsActiveThreadTerminating()) return;
            attention_timer = 7;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            if (!DoneIntro) {
                if (IsActiveThreadTerminating()) return;
                Game->AddLineToConversation(conversation, "TEXT_QST_B18_PRISONER_ATTENTION", false, Me, Game->GetHero());
            } else {
                if (IsActiveThreadTerminating()) return;
                Game->AddLineToConversation(conversation, "TEXT_QST_B18_PRISONER_TRAPPED", false, Me, Game->GetHero());
            }
        }
        if (ParentClass.StageNumber == 4) {
            if (IsActiveThreadTerminating()) return;
            if (ParentClass.Path == 1) {
                if (IsActiveThreadTerminating()) return;
                while (!Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
                if (DoneOutro) {
                    if (IsActiveThreadTerminating()) return;
                    long conversation = Game->AddNewConversation(Me, false, false);
                    Game->AddPersonToConversation(conversation, Game->GetHero());
                    Game->AddLineToConversation(conversation, "TEXT_QST_B18_PRISONER_OUTRO", false, Me, Game->GetHero());
                } else {
                    if (IsActiveThreadTerminating()) return;
                    ParentClass.PlayCutscene("CS_MAYOREXPOSE_PRISONEROUTRO", true, "NULL");
                    DoneOutro = true;
                }
            } else if (ParentClass.Path == 2) {
                if (IsActiveThreadTerminating()) return;
                Game->RemoveThing(Me, true, true);
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return;
            ParentClass.PlayCutscene("CS_MAYOREXPOSE_PRISONERATTACKED", true, "NULL");
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return;
            { CV_ExposeMayorScript& parent = ParentClass; if (parent.seh_Suspicious.IsValid()) parent.seh_Suspicious.Release(); }
            while (!Game->StartScriptingEntity(Me, ParentClass.seh_Suspicious, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            if (ParentClass.StageNumber == 0) {
                if (IsActiveThreadTerminating()) return;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (PMasterData->HeroMarriedLadyGrey) {
                    if (IsActiveThreadTerminating()) return;
                    ParentClass.PlayCutscene("CS_MAYOREXPOSE_PRISONERINTRO_MARRIED", false, "NULL");
                } else {
                    if (IsActiveThreadTerminating()) return;
                    if (DoneIntro) {
                        if (IsActiveThreadTerminating()) return;
                        ParentClass.PlayCutscene("CS_MAYOREXPOSE_PRISONERASKAGAIN", false, "NULL");
                    } else {
                        if (IsActiveThreadTerminating()) return;
                        ParentClass.PlayCutscene("CS_MAYOREXPOSE_PRISONERINTRO", false, "NULL");
                        DoneIntro = true;
                    }
                    Game->GiveHeroYesNoQuestion("TEXT_QST_B18_PRISONER_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                    long answer;
                    while ((answer = Game->MsgIsQuestionAnsweredYesOrNo()) < 0) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                    if (answer == 1) {
                        if (IsActiveThreadTerminating()) return;
                        ParentClass.PlayCutscene("CS_MAYOREXPOSE_PRISONERACCEPT", false, "NULL");
                        ParentClass.StageNumber = (STAGE_IN_PATH)1;
                    } else {
                        if (IsActiveThreadTerminating()) return;
                        ParentClass.PlayCutscene("CS_MAYOREXPOSE_PRISONERREFUSE", false, "NULL");
                    }
                }
            } else if (PMasterData->HeroMarriedLadyGrey) {
                if (IsActiveThreadTerminating()) return;
                ParentClass.PlayCutscene("CS_MAYOREXPOSE_PRISONER_MARRIED", true, "NULL");
            } else if (ParentClass.StageNumber == 1) {
                if (IsActiveThreadTerminating()) return;
                ParentClass.PlayCutscene("CS_MAYOREXPOSE_PRISONERRHODRI", true, "NULL");
            } else if (ParentClass.StageNumber == 2) {
                if (IsActiveThreadTerminating()) return;
                if (!ParentClass.PrisonerTalkedAboutLamp) {
                    if (IsActiveThreadTerminating()) return;
                    ParentClass.PlayCutscene("CS_MAYOREXPOSE_PRISONERFLASHING", true, "NULL");
                    ParentClass.PrisonerTalkedAboutLamp = true;
                } else {
                    if (IsActiveThreadTerminating()) return;
                    ParentClass.PlayCutscene("CS_MAYOREXPOSE_PRISONERFLASHING_SHORT", true, "NULL");
                }
            } else if (ParentClass.StageNumber == 3) {
                if (IsActiveThreadTerminating()) return;
                ParentClass.PlayCutscene("CS_MAYOREXPOSE_PRISONERGHOST", true, "NULL");
            }
        }
    }
    if (IsActiveThreadTerminating()) return;
    for (;;) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
}