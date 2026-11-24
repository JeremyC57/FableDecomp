// NScript::CQ_GuildTrainingScript::CCombatApprentice::Main() -- the combat apprentice who runs the departure melee test: take scripted control, hero faction, unkillable, friends with everything; then each frame flag information over her head while the departure quest is on and no guild grade has reached 4, walk back to her marker when more than 4 from it, otherwise now and then turn to the hero within 10 and comment on his melee grade; when waiting for a fight and talked to by the hero, refuse while busy or while a guild test runs, mourn a dead Whisper, else ask whether he wants the melee test: on yes teleport both to the arena and fight Whisper (bow / lightning hits and a guild warning abort it; insults on hits), then grade the fight from the health lost, play the departure melee test cut scenes and raise the global melee grade
// Inline-budget: scoped `#pragma inline_depth(0)` ... `#pragma inline_depth()` around four `if (IsActiveThreadTerminating()) return;`
//   statements -- the return sites whose ChatTimer destructor (plain inline CTimer::~CTimer, DeregisterTimer through pDSTGame) retail
//   CALLS through the shared pad (0x00CD4470 at 0x00D4C6E7): 0x00D4A4F1 (HasInformation clear), 0x00D4A5C2 (chat), 0x00D4A7A2 (busy
//   talk) and 0x00D4A8D0 (talk else). c2 inline trace (default threshold): those four ~CTimer sites score 70 / 69 / 65 / 65 and are
//   expanded, while retail expands only the sites scoring 18 / 26 / 33 / 25 (0x00D4A46E, 0x00D4A4DA, 0x00D4A538, 0x00D4A782 -> the
//   expanded pads 0x00D4C4C9 / 0x00D4C4F5 / 0x00D4C521). No /d2inlT value can do it: the START wait-loop seh_me dtor (score 68,
//   0x00D4A2DA -> the expanded release at 0x00D4A3F6) must stay expanded (T>=68) while two of the four score 65 (T<=64);
//   /d2inlS5..55 do not either. Depth 0 is the only level that outlines a depth-1 callee.
// Leaf: ?Main@CCombatApprentice@CQ_GuildTrainingScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D4A270, 9388 bytes = code 0x00D4A270..0x00D4C6FF (`ret`) + the 7-entry jump table of the grade
//   switch at 0x00D4C700 (`jmp [ebp*4+0x00D4C700]` at 0x00D4C03B); thiscall, private virtual (entity vtable 0x012CE094 slot 1).
// Identity (proven, two sources): Ego_r.pdb + FableWin.pdb ?Main@CCombatApprentice@CQ_GuildTrainingScript@NScript@@EAEXXZ; slot 1 of
//   the entity vtable 0x012CE094 that the landed Alloc 0x00D41A30 stores (0 0x00D41AF0, 2 Init 0x00D41AE0, 3 GetParentScript 0x00D41AD0).
// Layout: Ego_r.pdb CCombatApprentice (36 bytes): WaitingForFight +0x1c, BadHit +0x20 (overlay opt-in
//   FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_COMBATAPPRENTICE); master data GlobalMeleeGrade / GlobalSkillGrade / GlobalWillGrade
//   +0xb4..+0xbc, GuildWarningOccuring +0xc0, HeroTakingGuildTest +0xc6; quest StartedMeleeTesting +0x5c, FightFinished +0x64;
//   pGlobalScriptDef 0x0143E90C GUI_MeleeGrades +0xEB4 / GUI_MinHealth +0xED8.
// Local names: the Anniversary (Xbox 360) PDB's S_REGREL32 list of this function (ChatTimer, seh_me, HasInformation,
//   CombatApprenticeTargetMarker, Whisper, InsultTimer, Power, speech_movie, PauseMyEntitiesPlease, seh_Hero, seh_Whisper, mymap);
//   answer / healthBar / heroStartHealth / whisperStartHealth / fightAborted / guildWarning / heroEndHealth / whisperEndHealth /
//   score / grade / i are descriptive (register or packed-slot locals, absent from that list).
// Callees: CGameScriptInterfaceBase slots 7, 8, 70 GetHero, 72 GetThingWithScriptName, 87-90 (pDSTGame: RegisterTimer /
//   DeregisterTimer / SetTimer / GetTimer), 114 GiveHeroYesNoQuestion, 39 MsgIsQuestionAnsweredYesOrNo, 130, 147, 153, 154, 263, 264
//   GetHealth, 265, 284 IsQuestActive, 321, 325, 338, 360, 361, 364-366, 370 StartMovieSequence, 371, 373, 374, 376, 379, 451, 461,
//   472, 475, 506-508, 517, 601, 844; CScriptThing slots 21 / 25 / 41 / 67 (inline through PImp), 27 MsgIsTalkedToBy, 75 IsAlive
//   0x004AB130, ~CScriptThing 0x004AA840; STB ctor 0x007E72A0 / dtor 0x007E74D0 / IsValid 0x00CD23B9 / Release 0x00CD2770 / operator=
//   0x008ABD10 / ClearCommands 0x007E7360 / MoveToPosition 0x007E72F0 / Speak 0x007E7390 / GetScriptThing 0x007E7490 /
//   IsPerformingScriptTask 0x007E7450; movie Construct 0x006E7B60 / DestroyOutOfLine 0x006E7B80; CTimer 0x00CD4450 / 0x00CD4470;
//   map<CCharString,STB> ctor 0x00CDBF70 / operator[] 0x00CD3D2E / dtor 0x00CDBFB0; RunCutsceneMacro 0x00CBFB7D;
//   IsDistanceBetweenThingsOver 0x00CBE3EA / Under 0x00CBE2FF; IsActiveThreadTerminating 0x00F35B30.
#include "fable_compiler.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_MAIN
#define FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_COMBATAPPRENTICE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "stlport/fable_containers.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CQ_GuildTrainingScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CRGBColour.h"
#include "engine/CTimer.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/fable_script_distance.h"
#include "stlport/fable_map.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1, ENTITY_MOVE_SNEAK=2 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_LIGHTNING_SPELL=11 };
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A
extern CDefPointer<const CScriptDef> pGlobalScriptDef;   // 0x0143E90C
void FABLE_FASTCALL RunCutsceneMacro(const CCharString&, std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>&, std::map<CCharString,bool>*, std::map<CCharString,CCharString>*, bool, bool);   // 0x00CBFB7D
inline CScriptThing::~CScriptThing() { }
// NF326 one-definition respell: the body in the shape of the self row 0x004AA980 (retail's out-of-line copy), so this TU's pick-any COMDAT equals it.
FABLE_FORCEINLINE const C3DVector& CScriptThing::GetPos() const { const CScriptThing* imp = PImp.Data; if (imp == 0) return NullCoord; return imp->GetPos(); }
inline bool CScriptThing::MsgIsHitBy(const CCharString& attacker) const { if (!PImp.Data) return false; return PImp.Data->MsgIsHitBy(attacker); }
inline bool CScriptThing::MsgIsHitByWithProjectileWeapon(const CCharString& attacker, float& damage) const { if (!PImp.Data) return false; return PImp.Data->MsgIsHitByWithProjectileWeapon(attacker, damage); }
// NF326 one-definition respell: the body in the shape of the self row 0x004AAD20 (retail's out-of-line copy), so this TU's pick-any COMDAT equals it.
inline bool CScriptThing::MsgIsHitBySpecialAbilityFrom(EHeroAbility ability, const CCharString& attacker) const { const CScriptThing* imp = PImp.Data; if (!imp) return false; return imp->MsgIsHitBySpecialAbilityFrom(ability, attacker); }
inline void CScriptThing::SetFriendsWithEverythingFlag(bool on) { if (PImp.Data) PImp.Data->SetFriendsWithEverythingFlag(on); }
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
#define CA_T if (IsActiveThreadTerminating()) return
#define CA_START_SCRIPTING(thing, seh) \
    if (seh.IsValid()) seh.Release(); \
    while (!Game->StartScriptingEntity(thing, seh, SCRIPT_AI_PRIORITY_HIGHEST)) { \
        Game->NewScriptFrame(); \
        CA_T; \
    } \
    CA_T
#define CA_SPEAK(key) \
    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) { \
        seh_me.Speak(Game->GetHero(), key, GROUP_SELECT_NONE, false, true, false); \
        while (seh_me.IsPerformingScriptTask()) { \
            Game->NewScriptFrame(); \
            CA_T; \
        } \
        CA_T; \
    }
#define CA_MOAN(key) { \
        CScriptGameResourceObjectMovieBase speech_movie; \
        Game->StartMovieSequence("", speech_movie); \
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game); \
        CA_SPEAK(key) \
    }
#define CA_SAY(speaker, text) { \
        long conversation = Game->AddNewConversation(speaker, false, false); \
        Game->AddPersonToConversation(conversation, Game->GetHero()); \
        Game->AddLineToConversation(conversation, text, false, speaker, Game->GetHero()); \
    }

void NScript::CQ_GuildTrainingScript::CCombatApprentice::Main()
{
    CScriptGameResourceObjectScriptedThingBase seh_me;
    CA_START_SCRIPTING(Me, seh_me);
    Game->SetIsPushableByHero(Me, false);
    Game->SetThingHasInformation(Me, false, true, false);
    Game->EntitySetInFaction(Me, "FACTION_HERO");
    Game->EntitySetAsKillable(Me, false, true);
    Me.SetFriendsWithEverythingFlag(true);
    CTimer ChatTimer;
    ChatTimer = 10;
    CScriptThing CombatApprenticeTargetMarker = Game->GetThingWithScriptName("CombatApprenticeTargetMarker");
    bool HasInformation = false;
    for (;; Game->NewScriptFrame()) {
        CA_T;
        if (Game->IsQuestActive("Q_GuildTrainingDeparture")) {
            CA_T;
            if (PMasterData->GlobalMeleeGrade < 4 && PMasterData->GlobalSkillGrade < 4 && PMasterData->GlobalWillGrade < 4) {
                CA_T;
                if (!HasInformation) {
                    CA_T;
                    Game->SetThingHasInformation(Me, false, true, false);
                    HasInformation = true;
                }
            } else {
                CA_T;
                if (HasInformation) {
#pragma inline_depth(0)
                    CA_T;
#pragma inline_depth()
                    Game->ClearThingHasInformation(Me);
                    HasInformation = false;
                }
            }
        }
        if (IsDistanceBetweenThingsOver(Me, CombatApprenticeTargetMarker, 4.0f) && !seh_me.IsPerformingScriptTask()) {
            CA_T;
            seh_me.MoveToPosition(CombatApprenticeTargetMarker.GetPos(), 3.0f, ENTITY_MOVE_RUN, false, true);
        } else if (!seh_me.IsPerformingScriptTask() && IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 10.0f) && ChatTimer.GetTime() <= 0) {
#pragma inline_depth(0)
            CA_T;
#pragma inline_depth()
            Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
            ChatTimer = 20;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            if (PMasterData->GlobalMeleeGrade == 0) {
                CA_T;
                Game->AddLineToConversation(conversation, "TEXT_QST_028_APPRENTICE_MELEE_APPRENTICE_COMMENT", false, Me, Game->GetHero());
            } else if (PMasterData->GlobalMeleeGrade != 7) {
                CA_T;
                Game->AddLineToConversation(conversation, "TEXT_QST_028_APPRENTICE_MELEE_NOT_APLUS_COMMENT", false, Me, Game->GetHero());
            } else {
                CA_T;
                Game->AddLineToConversation(conversation, "TEXT_QST_028_APPRENTICE_MELEE_APLUS_COMMENT", false, Me, Game->GetHero());
            }
        }
        if (WaitingForFight && Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            CA_T;
            if (seh_me.IsPerformingScriptTask()) {
#pragma inline_depth(0)
                CA_T;
#pragma inline_depth()
                seh_me.ClearCommands();
                CA_MOAN("TEXT_QST_028_APPRENTICE_MELEE_APPRENTICE_EARLY");
            } else {
#pragma inline_depth(0)
                CA_T;
#pragma inline_depth()
                if (PMasterData->HeroTakingGuildTest) {
                    CA_T;
                    CA_MOAN("TEXT_QST_028_APPRENTICE_MELEE_OTHER_MELEE_GRADE");
                } else {
                CA_T;
                CScriptThing Whisper = Game->GetThingWithScriptName("MeleeApprentice");
                if (!Whisper.IsAlive()) {
                    CA_T;
                    CA_MOAN("TEXT_QST_028_APPRENTICE_MELEE_NO_WHISPER");
                } else {
                    CA_T;
                    long answer;
                    {
                        CScriptGameResourceObjectMovieBase speech_movie;
                        Game->StartMovieSequence("", speech_movie);
                        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                        CA_SPEAK("TEXT_QST_028_APPRENTICE_MELEE_DEPARTURE_HELLO")
                        Game->GiveHeroYesNoQuestion("TEXT_QST_028_APPRENTICE_MELEE_DEPARTURE_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                        answer = Game->MsgIsQuestionAnsweredYesOrNo();
                        while (answer < 0) {
                            Game->NewScriptFrame();
                            CA_T;
                            answer = Game->MsgIsQuestionAnsweredYesOrNo();
                        }
                        CA_T;
                        if (answer == 0) {
                            CA_T;
                            CA_SPEAK("TEXT_QST_028_APPRENTICE_MELEE_RETURN")
                        } else if (answer == 1) {
                            CA_T;
                            CA_SPEAK("TEXT_QST_028_APPRENTICE_MELEE_START")
                            if (PMasterData->GlobalMeleeGrade != 7) {
                                CA_T;
                                CA_SPEAK("TEXT_QST_028_APPRENTICE_MELEE_START_APLUS")
                            }
                            Game->FadeScreenOut(0.5f, 0.5f, CRGBColour(0, 0, 0, 255));
                            Game->Pause(1.0f);
                            Game->EntityTeleportToThing(Game->GetHero(), Game->GetThingWithScriptName("HeroMeleeStart"), false);
                            Game->EntityTeleportToThing(Whisper, Game->GetThingWithScriptName("WhisperMeleeStart"), false);
                            Game->EntityUnsheatheMeleeWeapon(Game->GetHero(), false);
                            Game->EntityUnsheatheWeapons(Whisper, false);
                        }
                    }
                    if (answer == 1) {
                        CA_T;
                        Game->FadeScreenIn();
                        Game->SetPlayerCreatureOnlyTarget(Whisper);
                        PMasterData->HeroTakingGuildTest = true;
                        ParentClass.StartedMeleeTesting = true;
                        WaitingForFight = false;
                        Game->ChangeHeroHealthBy(1000.0f, true, false);
                        Game->ModifyThingHealth(Whisper, 1000.0f, false);
                        Game->EntitySetAsKillable(Whisper, false, true);
                        Game->EntitySetCombatType(Whisper, "HERO_WHISPER_TUTORIAL_ATTACK_STYLE");
                        Game->EntitySetInFaction(Whisper, "FACTION_BANDITS");
                        Whisper.SetFriendsWithEverythingFlag(false);
                        Game->GiveThingBestEnemyTarget(Whisper, Game->GetHero());
                        ParentClass.FightFinished = false;
                        CTimer InsultTimer;
                        InsultTimer = 15;
                        Game->DisplayQuestInfo(true);
                        long healthBar = Game->AddQuestInfoBarHealth(Whisper, CRGBColour(255, 255, 255, 255), "HUD_WHISPER_ICON", 1.0f);
                        float heroStartHealth = Game->GetHealth(Game->GetHero());
                        float whisperStartHealth = Game->GetHealth(Whisper);
                        bool fightAborted = false;
                        bool guildWarning = false;
                        while (!ParentClass.FightFinished) {
                            Game->NewScriptFrame();
                            CA_T;
                            if (PMasterData->GuildWarningOccuring == true) {
                                CA_T;
                                guildWarning = true;
                                fightAborted = true;
                                ParentClass.FightFinished = true;
                            }
                            float Power = 0.0f;
                            if (Whisper.MsgIsHitByWithProjectileWeapon("SCRIPT_NAME_HERO", Power)) {
                                CA_T;
                                Game->EntitySetInFaction(Whisper, "FACTION_HERO");
                                Whisper.SetFriendsWithEverythingFlag(true);
                                CScriptGameResourceObjectMovieBase speech_movie;
                                Game->StartMovieSequence("", speech_movie);
                                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                                CA_SPEAK("TEXT_QST_028_APPRENTICE_MELEE_NO_BOW")
                                ParentClass.FightFinished = true;
                                fightAborted = true;
                            } else if (Whisper.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_LIGHTNING_SPELL, "SCRIPT_NAME_HERO")) {
                                CA_T;
                                Game->EntitySetInFaction(Whisper, "FACTION_HERO");
                                Whisper.SetFriendsWithEverythingFlag(true);
                                CScriptGameResourceObjectMovieBase speech_movie;
                                Game->StartMovieSequence("", speech_movie);
                                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                                CA_SPEAK("TEXT_QST_028_APPRENTICE_MELEE_NO_LIGHTNING")
                                ParentClass.FightFinished = true;
                                fightAborted = true;
                            }
                            if (Game->GetHealth(Game->GetHero()) < pGlobalScriptDef->GUI_MinHealth) {
                                CA_T;
                                ParentClass.FightFinished = true;
                            } else if (Game->GetHealth(Whisper) < pGlobalScriptDef->GUI_MinHealth) {
                                CA_T;
                                ParentClass.FightFinished = true;
                            }
                            if (Whisper.MsgIsHitBy("SCRIPT_NAME_HERO")) {
                                CA_T;
                                if (InsultTimer.GetTime() <= 8) {
                                    CA_T;
                                    CA_SAY(Whisper, "TEXT_QST_028_WHISPER_MELEE_HIT_INSULT")
                                    InsultTimer = 15;
                                }
                            } else if (Game->IsPlayerCreatureBlocking() && Game->GetHero().MsgIsHitBy("MeleeOpponent")) {
                                CA_T;
                                if (InsultTimer.GetTime() <= 0) {
                                    CA_T;
                                    CA_SAY(Whisper, "TEXT_QST_028_WHISPER_BLOCK_HIT_INSULT")
                                    InsultTimer = 15;
                                }
                            } else if (Game->GetHero().MsgIsHitBy("MeleeOpponent")) {
                                CA_T;
                                if (InsultTimer.GetTime() <= 0) {
                                    CA_T;
                                    CA_SAY(Whisper, "TEXT_QST_028_WHISPER_BATTLE_HIT_INSULT")
                                    InsultTimer = 15;
                                }
                            }
                        }
                        CA_T;
                        Game->ResetPlayerCreatureOnlyTarget();
                        Game->RemoveQuestInfoElement(healthBar);
                        Game->EntitySetInFaction(Whisper, "FACTION_HERO");
                        Whisper.SetFriendsWithEverythingFlag(true);
                        Game->DisplayQuestInfo(false);
                        if (fightAborted == true) {
                            CA_T;
                            if (guildWarning == true) {
                                CA_T;
                                Game->Pause(1.0f);
                                Game->FadeScreenOut(0.5f, 0.5f, CRGBColour(0, 0, 0, 255));
                                Game->SheatheHeroWeapons();
                                Game->EntitySheatheWeapons(Whisper, false);
                                Game->Pause(1.0f);
                            } else {
                                CA_T;
                                Game->FadeScreenOut(0.5f, 0.5f, CRGBColour(0, 0, 0, 255));
                            }
                            Game->ChangeHeroHealthBy(1000.0f, true, false);
                            Game->ModifyThingHealth(Whisper, 1000.0f, false);
                            Game->EntityTeleportToThing(Game->GetHero(), Game->GetThingWithScriptName("M_MeleeHeroStand"), false);
                            Game->EntityTeleportToThing(Whisper, Game->GetThingWithScriptName("M_MeleeOpponentStand"), false);
                            Game->EntitySetInFaction(Whisper, "FACTION_HERO");
                            Game->FadeScreenIn();
                            WaitingForFight = true;
                        } else {
                            CA_T;
                            float heroEndHealth = Game->GetHealth(Game->GetHero());
                            float whisperEndHealth = Game->GetHealth(Whisper);
                            float score = (whisperStartHealth - whisperEndHealth) - (heroStartHealth - heroEndHealth);
                            long grade = 0;
                            for (long i = 0; i < 7; ++i) {
                                grade = i;
                                if (pGlobalScriptDef->GUI_MeleeGrades[i] <= score) {
                                    CA_T;
                                    break;
                                }
                            }
                            CScriptGameResourceObjectScriptedThingBase seh_Whisper;
                            CA_START_SCRIPTING(Whisper, seh_Whisper);
                            CScriptGameResourceObjectScriptedThingBase seh_Hero;
                            CA_START_SCRIPTING(Game->GetHero(), seh_Hero);
                            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                            mymap["ME"] = seh_me;
                            mymap["HERO"] = seh_Hero;
                            mymap["WHISPER"] = seh_Whisper;
                            CScriptGameResourceObjectMovieBase speech_movie;
                            Game->StartMovieSequence("", speech_movie);
                            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                            Game->FixMovieSequenceCamera(true);
                            RunCutsceneMacro("CS_GUILD_DEPARTURE_MELEE_TEST_END", mymap, 0, 0, false, true);
                            mymap["ME"] = seh_me;
                            switch (grade) {
                            case 0:
                                if (PMasterData->GlobalMeleeGrade != 7) {
                                    CA_T;
                                    RunCutsceneMacro("CS_GUILD_DEPARTURE_MELEE_TEST_APLUS_PRIZE", mymap, 0, 0, false, true);
                                    Game->ClearThingHasInformation(Me);
                                } else {
                                    CA_T;
                                    RunCutsceneMacro("CS_GUILD_DEPARTURE_MELEE_TEST_APLUS", mymap, 0, 0, false, true);
                                }
                                break;
                            case 1: RunCutsceneMacro("CS_GUILD_DEPARTURE_MELEE_TEST_A", mymap, 0, 0, false, true); break;
                            case 2: RunCutsceneMacro("CS_GUILD_DEPARTURE_MELEE_TEST_B", mymap, 0, 0, false, true); break;
                            case 3: RunCutsceneMacro("CS_GUILD_DEPARTURE_MELEE_TEST_C", mymap, 0, 0, false, true); break;
                            case 4: RunCutsceneMacro("CS_GUILD_DEPARTURE_MELEE_TEST_D", mymap, 0, 0, false, true); break;
                            case 5: RunCutsceneMacro("CS_GUILD_DEPARTURE_MELEE_TEST_E", mymap, 0, 0, false, true); break;
                            case 6: RunCutsceneMacro("CS_GUILD_DEPARTURE_MELEE_TEST_F", mymap, 0, 0, false, true); break;
                            }
                            if (PMasterData->GlobalMeleeGrade < 7 - grade) {
                                CA_T;
                                PMasterData->GlobalMeleeGrade = 7 - grade;
                            }
                            mymap["ME"] = seh_me;
                            mymap["HERO"] = seh_Hero;
                            mymap["WHISPER"] = seh_Whisper;
                            RunCutsceneMacro("CS_GUILD_DEPARTURE_MELEE_TEST_OVER", mymap, 0, 0, false, true);
                            Game->FixMovieSequenceCamera(false);
                            WaitingForFight = true;
                            Game->ChangeHeroHealthBy(1000.0f, true, false);
                            Game->ModifyThingHealth(Whisper, 1000.0f, false);
                            Game->EntitySetInFaction(Whisper, "FACTION_HERO");
                        }
                        ParentClass.StartedMeleeTesting = false;
                        PMasterData->HeroTakingGuildTest = false;
                    }
                }
                }
            }
        }
    }
}