// NScript::CV_SwordInTheStoneScript::SwordPullCutscene() -- the hero tries to pull the Sword of Strength: record his strength stats the first time, test each stat for enough gain since then (or its maximum), then inside a paused movie sequence either play CS_SWORDINSTONE_SUCCEED (remove the sword, renown if the crowd is watching, HeroHasTakenSword) or CS_SWORDINSTONE_FAIL with a crowd member naming the weak stat; release the hero, reset the camera and end the cutscene
// Leaf: ?SwordPullCutscene@CV_SwordInTheStoneScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ED5830, 1819 bytes; RELOCATION_MATCH, every relocation bound-same, 11 literals equal.
// Identity: called from CV_SwordInTheStoneScript code at 0x00ED5814; Ego_r.pdb + FableWin.pdb
//   ?SwordPullCutscene@CV_SwordInTheStoneScript@NScript@@QAEXXZ.
// Callees: ~CScriptThing 0x004AA840, CScriptThing::operator= 0x008AB980, ~vector<CScriptThing> 0x008AC970, CBaseClass ctor 0x0099A2D0,
//   CBaseObject 0x0099A380 / 0x0099A3B0 / 0x0099A430, ScriptedThingBase dtor 0x007E74D0 / Release 0x00CD2770, movie dtor 0x006E7B80,
//   movie PImp dtor 0x006E7AB0, cast map operator[] 0x00CD3D2E / dtor 0x00CDBFB0, RunCutsceneMacro 0x00CBFB7D, IsActiveThreadTerminating
//   0x00CB7940, pGlobalScriptDef 0x0143E90C (SwordInTheStone* fields).
// Layout: engine/CV_SwordInTheStoneScript.h (new, 0x80): SwordRemovalAttemptCount +0x48 .. CrowdSwordTimer +0x7C.
// Shape: WaspBoss cutscene model plus the ScriptedThingBase OUTOFLINE_DTOR opt-in; ends with SwordRemovalAttemptCount++.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST,SCRIPT_AI_PRIORITY_LOW,SCRIPT_AI_PRIORITY_MEDIUM,SCRIPT_AI_PRIORITY_HIGH,SCRIPT_AI_PRIORITY_HIGHEST };
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#pragma auto_inline(off)
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer()
{
 if(Info){--Info->RefCount;if(Info->RefCount==0){Info->DeleteFunc(Info->Data);delete Info;}}
 Data=0;Info=0;
}
#pragma auto_inline(on)
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "stlport/fable_map.h"
#include "engine/CV_SwordInTheStoneScript.h"
#include "engine/fable_script_cutscene.h"
#include "engine/fable_script_def_global.h"
__forceinline CScriptGameResourceObjectScriptedThingBase& CScriptGameResourceObjectScriptedThingBase::operator=(const CScriptGameResourceObjectScriptedThingBase& other) { CScriptGameResourceObjectBase::operator=(other); PImp=other.PImp; return *this; }
inline CScriptThing::CScriptThing() { }
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }

void NScript::CV_SwordInTheStoneScript::SwordPullCutscene()
{
    if (!StatsInitialised) {
        if (IsActiveThreadTerminating())
            return;
        HeroStrengthPhysique = Game->GetHeroStatLevel((EHeroTrainableStatType)0);
        HeroStrengthToughness = Game->GetHeroStatLevel((EHeroTrainableStatType)2);
        HeroStrengthHealth = Game->GetHeroStatLevel((EHeroTrainableStatType)1);
        StatsInitialised = true;
    }
    long level = Game->GetHeroStatLevel((EHeroTrainableStatType)0);
    long max = Game->GetHeroStatMax((EHeroTrainableStatType)0);
    bool physique_ok = level - HeroStrengthPhysique >= pGlobalScriptDef->SwordInTheStonePhysiqueChange || level == max;
    level = Game->GetHeroStatLevel((EHeroTrainableStatType)2);
    max = Game->GetHeroStatMax((EHeroTrainableStatType)2);
    bool toughness_ok = level - HeroStrengthToughness >= pGlobalScriptDef->SwordInTheStoneToughnessChange || level == max;
    level = Game->GetHeroStatLevel((EHeroTrainableStatType)1);
    max = Game->GetHeroStatMax((EHeroTrainableStatType)1);
    bool health_ok = level - HeroStrengthHealth >= pGlobalScriptDef->SwordInTheStoneHealthChange || level == max;
    bool success = physique_ok && toughness_ok && health_ok;
    CScriptGameResourceObjectScriptedThingBase hero;
    Game->StartScriptingEntity(Game->GetHero(), hero, SCRIPT_AI_PRIORITY_HIGHEST);
    CScriptGameResourceObjectMovieBase movie;
    Game->StartMovieSequence("", movie);
    CWideScreenMagicPauseEntities pause(*Game);
    std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
    cast["HERO"] = hero;
    if (success) {
        if (IsActiveThreadTerminating())
            return;
        Game->MiniMapRemoveMarker(Game->GetThingWithScriptName("SwordOfStrength"));
        RunCutsceneMacro("CS_SWORDINSTONE_SUCCEED", cast, 0, 0, false, true);
        CScriptThing sword = Game->GetThingWithScriptName("SwordOfStrength");
        if (sword.IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            Game->RemoveThing(sword, false, true);
        }
        std::vector<CScriptThing> crowd;
        long count = Game->GetAllThingsWithScriptName("SwordCrowd", crowd);
        if (count > 0) {
            if (IsActiveThreadTerminating())
                return;
            Game->GiveHeroRenownPoints(pGlobalScriptDef->SwordInTheStoneSuccessRenown);
        }
        HeroHasTakenSword = true;
    } else {
        if (IsActiveThreadTerminating())
            return;
        RunCutsceneMacro("CS_SWORDINSTONE_FAIL", cast, 0, 0, false, true);
        std::vector<CScriptThing> crowd;
        CScriptThing member;
        long count = Game->GetAllThingsWithScriptName("SwordCrowd", crowd);
        if (count > 0) {
            if (IsActiveThreadTerminating())
                return;
            member = crowd[0];
        }
        if (member.IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            long conversation = Game->AddNewConversation(member, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            if (!physique_ok) {
                if (IsActiveThreadTerminating())
                    return;
                Game->AddLineToConversation(conversation, "TEXT_QST_062_FAILURE_PHYSIQUE", false, member, Game->GetHero());
            } else if (!toughness_ok) {
                if (IsActiveThreadTerminating())
                    return;
                Game->AddLineToConversation(conversation, "TEXT_QST_062_FAILURE_TOUGHNESS", false, member, Game->GetHero());
            } else if (!health_ok) {
                if (IsActiveThreadTerminating())
                    return;
                Game->AddLineToConversation(conversation, "TEXT_QST_062_FAILURE_HEALTH", false, member, Game->GetHero());
            }
        }
    }
    hero.Release();
    Game->CameraDefault();
    SwordCutscene = false;
    SwordRemovalAttemptCount++;
}