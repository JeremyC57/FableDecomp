// NScript::CQ_NewOakValeIntroScript::CNOVI_Villager::Main() -- an Oakvale intro villager: live while conscious; each frame release control, then if the hero hits it (a heal spell does not count) turn it and the hero into allies, count a bad deed, flag HeroDidHitMe and, if it still has health, have it complain in its sex's TEXT_QST_048_VILLAGER_ATTACKED line; if the hero talks to it, face him and hold a conversation with the bad-deeds or spoken-to line for its sex; otherwise, now and then (timer clear, 1 in 100, hero within 5 units) restart the 3-second talk timer and say a random line from the quest's villager speech list for its sex and the hero's good / bad deed tally
// Inline-budget: flag /d2inlT65 -- the implicit seh_me (CScriptGameResourceObjectScriptedThingBase) dtor at the hit block's entry
//   terminate check (retail 0x00DAE0FA..0x00DAE103 `jne 0x00DAEA49`, the called ~STB pad 0x007E74D0) scores 65 in the c2 inline trace
//   (c1 60, f 0.625) and is expanded at the default threshold (74); retail expands only the loop-top exit pad 0x00DAE9A5 (score 48).
//   RELOCATION_MATCH for T51..T65 (T50: the nested CCountedPointer dtor, score 50, is called; T66+: the extra 42-byte pad); T65 is
//   the value nearest the default and inside the landed same-TU sibling CNOVI_Guard::Main's working range T63..T72. Row-local
//   CTimer::GetTime / operator=(long) are FABLE_FORCEINLINE as in the 76 landed script rows that expand them (operator= scores 187 here).
// Leaf: ?Main@CNOVI_Villager@CQ_NewOakValeIntroScript@NScript@@EAEXXZ
// Flags: /d2inlT65
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DADF80, 2787 bytes, thiscall, plain `ret` (two epilogue copies), no EH frame; ends at 0x00DAEA63.
// Identity (proven): slot 1 (Main) of the CNOVI_Villager entity vtable 0x012D8678 stored by the landed CNOVI_Villager::Alloc
//   0x00DADE50 (its Init 0x00DADF00 / GetParentScript 0x00DADEF0 are landed); Ego_r.pdb / FableWin.pdb name set
//   ?Main@CNOVI_Villager@CQ_NewOakValeIntroScript@NScript@@EAEXXZ; "NOVI_Villager" is the binding name Main 0x00DABAC0 registers.
// Callees: CIsThingConscious (vtable 0x012C2FE8, inline) + CScriptThing copy 0x004ABE90 / ~ 0x004AA840, SetPredicate 0x00F35B10,
//   IsActiveThreadTerminating 0x00F35B30, CBaseObject 0x0099A380 / ~ 0x0099A430 (the handle and movie ctors and the loop-exit handle
//   dtor inline), ScriptedThingBase dtor 0x007E74D0 / IsValid 0x00CD23B9 / Release 0x00CD2770 / GetScriptThing 0x007E7490 / Speak
//   0x007E7390 / IsPerformingScriptTask 0x007E7450, movie dtor 0x006E7B80, CCharString() 0x0099E4B0 / (const char*, long) 0x0099EBF0 /
//   ~ 0x0099EAE0 / operator=(const char*) 0x0099EFE0 / operator=(const CCharString&) 0x0099EFB0, operator+(const CCharString&, const
//   CCharString&) 0x0099F570, AddBadDeed 0x00DAEA70, GetVillagerSpeechIndex 0x00DAEBF0, rand 0x00BFEB16, operator delete 0x00BFE9BC,
//   IsDistanceBetweenThingsUnder 0x00CBE2FF; pDSTGame 0x0143E8F8 (CTimer::GetTime / operator= inline); CGameScriptInterfaceBase slots
//   7, 8, 70 GetHero, 89 SetTimer, 90 GetTimer, 264 GetHealth, 364 AddNewConversation, 365 AddPersonToConversation, 366
//   AddLineToConversation, 368 IsConversationActive, 370 StartMovieSequence, 379 PauseAllNonScriptedEntities, 475
//   EntitySetFacingAngleTowardsThing, 513 EntityGetSex, 599 EntitySetThingAsAllyOfThing; CScriptThing slots 21 MsgIsHitBy, 27
//   MsgIsTalkedToBy, 41 MsgIsHitBySpecialAbilityFrom, 42 MsgIsHitByAnySpecialAbilityFrom.
// Layout: engine/CQ_NewOakValeIntroScript.h (new): HeroDidHitMe +0x1c; ParentClass GoodDeedsPerformed +0x54, BadDeedsPerformed +0x58,
//   VillagerSpeech* +0x9c..+0xf0, TalkIntermittentTimer +0x104.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/TRetVoid.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_NewOakValeIntroScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CIsThingConscious.h"
#include "engine/fable_script_distance.h"
#include <stdlib.h>
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
extern CGameScriptInterfaceBase* pDSTGame;                                                                   // 0x0143E8F8
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long n) { pDSTGame->SetTimer(TimerIndex, n); return *this; }
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingConscious::~CIsThingConscious() { }

void NScript::CQ_NewOakValeIntroScript::CNOVI_Villager::Main()
{
    TRetVoid _Fail_Val;
    {
        CIsThingConscious conscious(Me);
        SetPredicate(conscious);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    CCharString speech;
    for (;; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if (seh_me.IsValid())
            seh_me.Release();
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") ||
            (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            Game->EntitySetThingAsAllyOfThing(Me, Game->GetHero());
            Game->EntitySetThingAsAllyOfThing(Game->GetHero(), Me);
            ParentClass.AddBadDeed((EBadDeeds)2);
            HeroDidHitMe = true;
            if (seh_me.IsValid())
                seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->EntityGetSex(Me) == (ESex)1) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_048_VILLAGER_ATTACKED_MALE", (ETextGroupSelectionMethod)1, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
            } else {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_048_VILLAGER_ATTACKED_FEMALE", (ETextGroupSelectionMethod)1, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
            }
        } else if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            CCharString sex_suffix;
            if (Game->EntityGetSex(Me) == (ESex)1) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                sex_suffix = "_MALE";
            } else {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                sex_suffix = "_FEMALE";
            }
            if (seh_me.IsValid())
                seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            if (HeroDidHitMe) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                Game->AddLineToConversation(conversation, CCharString("TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS") + sex_suffix, false, Me, Game->GetHero());
            } else {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                Game->AddLineToConversation(conversation, CCharString("TEXT_QST_048_VILLAGER_SPOKEN_TO") + sex_suffix, false, Me, Game->GetHero());
            }
            while (Game->IsConversationActive(conversation)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            if (seh_me.IsValid())
                seh_me.Release();
        } else {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            if (ParentClass.TalkIntermittentTimer.GetTime() == 0 && rand() % 100 == 0 && IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 5.0f)) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                ParentClass.TalkIntermittentTimer = 3;
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                if (ParentClass.BadDeedsPerformed == 0 && ParentClass.GoodDeedsPerformed > 0) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    if (Game->EntityGetSex(Me) == (ESex)1) {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        { long count = ParentClass.VillagerSpeechMaleGood.size(); speech = ParentClass.VillagerSpeechMaleGood[GetVillagerSpeechIndex(count)]; }
                    } else {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        { long count = ParentClass.VillagerSpeechFemaleGood.size(); speech = ParentClass.VillagerSpeechFemaleGood[GetVillagerSpeechIndex(count)]; }
                    }
                } else if (ParentClass.BadDeedsPerformed > 0 && ParentClass.GoodDeedsPerformed == 0) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    if (Game->EntityGetSex(Me) == (ESex)1) {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        { long count = ParentClass.VillagerSpeechMaleBad.size(); speech = ParentClass.VillagerSpeechMaleBad[GetVillagerSpeechIndex(count)]; }
                    } else {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        { long count = ParentClass.VillagerSpeechFemaleBad.size(); speech = ParentClass.VillagerSpeechFemaleBad[GetVillagerSpeechIndex(count)]; }
                    }
                } else if (ParentClass.BadDeedsPerformed > 0 && ParentClass.GoodDeedsPerformed > 0) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    if (Game->EntityGetSex(Me) == (ESex)1) {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        { long count = ParentClass.VillagerSpeechMaleBoth.size(); speech = ParentClass.VillagerSpeechMaleBoth[GetVillagerSpeechIndex(count)]; }
                    } else {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        { long count = ParentClass.VillagerSpeechFemaleBoth.size(); speech = ParentClass.VillagerSpeechFemaleBoth[GetVillagerSpeechIndex(count)]; }
                    }
                } else {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    if (Game->EntityGetSex(Me) == (ESex)1) {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        { long count = ParentClass.VillagerSpeechMaleNeither.size(); speech = ParentClass.VillagerSpeechMaleNeither[GetVillagerSpeechIndex(count)]; }
                    } else {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        { long count = ParentClass.VillagerSpeechFemaleNeither.size(); speech = ParentClass.VillagerSpeechFemaleNeither[GetVillagerSpeechIndex(count)]; }
                    }
                }
                Game->AddLineToConversation(conversation, speech, false, Me, Game->GetHero());
            }
        }
    }
}
