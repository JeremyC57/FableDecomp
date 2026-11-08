// NScript::CQ_BanditCampBossBattleScript::CAssassinMarker::Main() -- the Twinblade assassins' marker: while alive, make the three assassins unkillable; until the assassin cut scene is triggered or they have been attacked: once they are under attack take them under scripted control, let each one still alive shout its ATTACKED line, give each the hero as best enemy target and release them; otherwise, while the hero stands near the marker with no assassin conversation running and has not talked to them, start the next of the six overheard assassin lines; then make them killable again and, if the cut scene was triggered, cast the hero and the assassins (now Twinblade camp bandits) in CS_BANDITCAMP_ASSATTACK, open gate 3 and remove its guard
// Grade: DIFFER(4036v4072) -- md=263: retail expands ~CIsThingAlive at the SetPredicate temporary (vtable 0x012C32F0 held in edi, re-stored, then a call to ~CScriptThing 0x004AA840) and so packs the temporary low in the frame; ours calls 0x00E241B0, the temporary sits at +0xBC and every frame slot after it shifts
// Next idea: make ~CIsThingAlive expand at that site while its nested ~CScriptThing stays a call (a __forceinline row-local definition expands both: md=403; implicit dtor via overlay: md=404; /d2inlT80 with an inline definition: md=170)
// Leaf: ?Main@CAssassinMarker@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D11930, 4072 bytes (the switch table 0x00D12900 inside), thiscall, no EH frame.
// Identity (proven, two sources): the address appears once in .rdata, at slot 1 of the entity vtable 0x012C8B80 the landed
//   CAssassinMarker::Alloc 0x00D048C0 stores (slot 0 the landed scalar deleting dtor 0x00D04970, slot 3 the landed GetParentScript
//   0x00D04960); Ego_r.pdb and FableWin.pdb both hold ?Main@CAssassinMarker@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ.
// Callees: CIsThingAlive vtable 0x012C32F0 / CScriptThing copy 0x004ABE90 / ~CScriptThing 0x004AA840, SetPredicate 0x00F35B10,
//   IsActiveThreadTerminating 0x00F35B30, CCharString 0x0099EBF0 / 0x0099EAE0, CBaseObject ctor / operator= / dtor 0x0099A380 /
//   0x0099A3B0 / 0x0099A430, scripted-thing ctor / dtor 0x007E72A0 / 0x007E74D0 (the cut-scene handles), GetScriptThing 0x007E7490,
//   Speak 0x007E7390, IsPerformingScriptTask 0x007E7450, IsValid 0x00CD23B9, Release 0x00CD2770, counted-pointer assignment
//   0x008AB1E0, movie PImp dtor 0x006E7AB0, cast map ctor / dtor / operator[] 0x00CDBF70 / 0x00CDBFB0 / 0x00CD3D2E, RunCutsceneMacro
//   0x00CBFB7D, IsDistanceBetweenThingsUnder 0x00CBE2FF, ~CTimer 0x00CD4470 (pDSTGame 0x0143E8F8 Register/Set/DeregisterTimer inline).
// Data: engine/CQ_BanditCampBossBattleScript.h Gate3Open +0x4d, AssassinsUnderAttack +0x60, AssassinCutsceneTriggered +0x61,
//   TalkedToAssassin +0x62 (ParentClass at +0x14).
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_ASSASSINMARKER
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_BanditCampBossBattleScript.h"
#include "engine/C3DVector.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CIsThingAlive.h"
#include "engine/CTimer.h"
#include "engine/fable_script_cutscene.h"
#include "engine/fable_script_distance.h"
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
extern CGameScriptInterfaceBase* pDSTGame;
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
inline CScriptThing::~CScriptThing() {}
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();
template<> CCountedPointer<CScriptGameResourceObjectScriptedThingBase>& CCountedPointer<CScriptGameResourceObjectScriptedThingBase>::operator=(const CCountedPointer<CScriptGameResourceObjectScriptedThingBase>& other);
__forceinline CScriptGameResourceObjectScriptedThingBase& CScriptGameResourceObjectScriptedThingBase::operator=(const CScriptGameResourceObjectScriptedThingBase& other) { CScriptGameResourceObjectBase::operator=(other); PImp=other.PImp; return *this; }

void NScript::CQ_BanditCampBossBattleScript::CAssassinMarker::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    CScriptThing assassin1 = Game->GetThingWithScriptName("Assassin1");
    CScriptThing assassin2 = Game->GetThingWithScriptName("Assassin2");
    CScriptThing assassin3 = Game->GetThingWithScriptName("Assassin3");
    Game->EntitySetAsKillable(assassin1, false, true);
    Game->EntitySetAsKillable(assassin2, false, true);
    Game->EntitySetAsKillable(assassin3, false, true);
    CScriptGameResourceObjectScriptedThingBase seh_assassin1;
    CScriptGameResourceObjectScriptedThingBase seh_assassin2;
    CScriptGameResourceObjectScriptedThingBase seh_assassin3;
    bool attacked = false;
    long conversation = 0;
    long line = 0;
    CTimer timer;
    timer = 0;
    while (!ParentClass.AssassinCutsceneTriggered && !attacked) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (ParentClass.AssassinsUnderAttack) {
            if (IsActiveThreadTerminating()) return;
            Game->StartScriptingEntity(assassin1, seh_assassin1, SCRIPT_AI_PRIORITY_HIGHEST);
            Game->StartScriptingEntity(assassin2, seh_assassin2, SCRIPT_AI_PRIORITY_HIGHEST);
            Game->StartScriptingEntity(assassin3, seh_assassin3, SCRIPT_AI_PRIORITY_HIGHEST);
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(seh_assassin1.GetScriptThing()) > 0.0f) {
                seh_assassin1.Speak(Game->GetHero(), "TEXT_QST_009_ASSASSIN1_ATTACKED", GROUP_SELECT_NONE, false, true, false);
                while (seh_assassin1.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
            }
            if (Game->GetHealth(seh_assassin2.GetScriptThing()) > 0.0f) {
                seh_assassin2.Speak(Game->GetHero(), "TEXT_QST_009_ASSASSIN2_ATTACKED", GROUP_SELECT_NONE, false, true, false);
                while (seh_assassin2.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
            }
            if (Game->GetHealth(seh_assassin3.GetScriptThing()) > 0.0f) {
                seh_assassin3.Speak(Game->GetHero(), "TEXT_QST_009_ASSASSIN3_ATTACKED", GROUP_SELECT_NONE, false, true, false);
                while (seh_assassin3.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
            }
            Game->GiveThingBestEnemyTarget(assassin1, Game->GetHero());
            Game->GiveThingBestEnemyTarget(assassin2, Game->GetHero());
            Game->GiveThingBestEnemyTarget(assassin3, Game->GetHero());
            attacked = true;
            if (seh_assassin1.IsValid()) seh_assassin1.Release();
            if (seh_assassin2.IsValid()) seh_assassin2.Release();
            if (seh_assassin3.IsValid()) seh_assassin3.Release();
        } else if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 4.0f) && !Game->IsConversationActive(conversation)
                   && !ParentClass.TalkedToAssassin) {
            if (IsActiveThreadTerminating()) return;
            switch (line) {
            case 0:
                conversation = Game->AddNewConversation(assassin1, false, false);
                Game->AddPersonToConversation(conversation, assassin2);
                Game->AddLineToConversation(conversation, "TEXT_QST_009_ASSASSIN1_LISTENED_ONE", false, assassin1, assassin2);
                break;
            case 1:
                conversation = Game->AddNewConversation(assassin2, false, false);
                Game->AddPersonToConversation(conversation, assassin1);
                Game->AddLineToConversation(conversation, "TEXT_QST_009_ASSASSIN2_LISTENED_ONE", false, assassin2, assassin1);
                break;
            case 2:
                conversation = Game->AddNewConversation(assassin1, false, false);
                Game->AddPersonToConversation(conversation, assassin2);
                Game->AddLineToConversation(conversation, "TEXT_QST_009_ASSASSIN1_LISTENED_TWO", false, assassin1, assassin2);
                break;
            case 3:
                conversation = Game->AddNewConversation(assassin2, false, false);
                Game->AddPersonToConversation(conversation, assassin1);
                Game->AddLineToConversation(conversation, "TEXT_QST_009_ASSASSIN2_LISTENED_TWO", false, assassin2, assassin1);
                break;
            case 4:
                conversation = Game->AddNewConversation(assassin1, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_009_ASSASSIN1_LISTENED_THREE", false, assassin1, Game->GetHero());
                break;
            case 5:
                conversation = Game->AddNewConversation(assassin1, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_009_ASSASSIN1_LISTENED_FOUR", false, assassin1, Game->GetHero());
                break;
            }
            ++line;
        }
    }
    if (IsActiveThreadTerminating()) return;
    Game->EntitySetAsKillable(assassin1, true, true);
    Game->EntitySetAsKillable(assassin2, true, true);
    Game->EntitySetAsKillable(assassin3, true, true);
    if (ParentClass.AssassinCutsceneTriggered) {
        if (IsActiveThreadTerminating()) return;
        CScriptGameResourceObjectScriptedThingBase seh_hero;
        CScriptGameResourceObjectScriptedThingBase seh_ass1;
        CScriptGameResourceObjectScriptedThingBase seh_ass2;
        CScriptGameResourceObjectScriptedThingBase seh_ass3;
        Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
        Game->StartScriptingEntity(assassin1, seh_ass1, SCRIPT_AI_PRIORITY_HIGHEST);
        Game->StartScriptingEntity(assassin2, seh_ass2, SCRIPT_AI_PRIORITY_HIGHEST);
        Game->StartScriptingEntity(assassin3, seh_ass3, SCRIPT_AI_PRIORITY_HIGHEST);
        Game->EntitySetInFaction(assassin1, "FACTION_TWINBLADE_CAMP_BANDITS");
        Game->EntitySetInFaction(assassin2, "FACTION_TWINBLADE_CAMP_BANDITS");
        Game->EntitySetInFaction(assassin3, "FACTION_TWINBLADE_CAMP_BANDITS");
        std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> mymap;
        mymap["HERO"] = seh_hero;
        mymap["ASS1"] = seh_ass1;
        mymap["ASS2"] = seh_ass2;
        mymap["ASS3"] = seh_ass3;
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_BANDITCAMP_ASSATTACK", mymap, 0, 0, false, true);
        Game->FixMovieSequenceCamera(false);
        ParentClass.Gate3Open = true;
        Game->RemoveThing(Game->GetThingWithScriptName("Gate3Guard"), false, true);
    }
    for (;;) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
}
