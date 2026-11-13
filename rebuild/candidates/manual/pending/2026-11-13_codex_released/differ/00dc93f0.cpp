// NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageSkeleton::Main() -- handle artifact collection and skeleton reminders
// Grade: DIFFER(4485v4507) -- One missing22-byte helmet cancellation cleanup; same-scope movie and hero slots reversed. Native by-value and nested spawn copy now fixed. Original local identities corrected by PDB and donor use sites.
// Leaf: ?Main@CSecretPassageSkeleton@CQ_OpeningGraveyardSecretPassageScript@NScript@@EAEXXZ
// Retail: 0x00DC93F0, 4507 bytes, thiscall.
// Identity: both original PDB name sets; native entity vtable012DA494 slot1.
// Selected reconstruction: a13;4485bytes against4507. One called movie cleanup
// and two local slots still differ. Complete behavior fixture passes both compilers.
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CISTHINGALIVE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CPREDICATEBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_OpeningGraveyardSecretPassageScript.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc1.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_cutscene.h"
#include "engine/fable_script_def_global.h"
#include "engine/fable_script_distance.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0
inline CScriptThing::~CScriptThing() {}
// Inline-budget: the nested Par1 copy is called in retail, while the outgoing argument copy expands.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
template<> FABLE_FORCEINLINE CSpawnedFunc1<NScript::CQ_OpeningGraveyardSecretPassageScript,CScriptThing>::CSpawnedFunc1(const CCharString& name, unsigned long id, NScript::CQ_OpeningGraveyardSecretPassageScript* pclass, void (NScript::CQ_OpeningGraveyardSecretPassageScript::*func)(CScriptThing), CScriptThing par1)
    : CSpawnedFuncBase(name,id), Func(func), PClass(pclass), Par1(par1) { }
#ifdef _MSC_VER
#pragma inline_depth()
#endif
void NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageSkeleton::Main()
{
    TRetVoid _Fail_Val;
    bool first_time=true;
    bool Triggered=false;
    Game.SetThingAsUsable(Me,true);
    while (!collected_all) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (!Triggered) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            while (!IsDistanceBetweenThingsUnder(Game.GetHero(),Game.GetThingWithScriptName("CryptCutsceneMarker"),2.0f)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Triggered=true;
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            Game.StartScriptingEntity(Game.GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["HERO"]=seh_hero;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            Game.FixMovieSequenceCamera(true);
            RunCutsceneMacro("CS_OPENGRAVE_CRYPT_ENTER",mymap,0,0,false,true);
            Game.FixMovieSequenceCamera(false);
            Game.SetQuestCardObjective(Game.GetActiveQuestName(),"TEXT_QUEST_GRAVEYARD_SECRET_PASSAGE_OBJECTIVE_03","","Graveyard");
        }
        ParentClass.skeleton_visited=true;
        while (!Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        for (int i=0; i<LAST; ++i) {
            if (Game.IsObjectInThingsPossession(ParentClass.hero_object[i],Game.GetHero())) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                returned[i]=true;
                CScriptThing new_item=Game.CreateObjectOnEntity(skeleton_object[i],Me,"");
                Game.SetThingPersistent(new_item,true);
                Game.EntitySetAlpha(new_item,0.0f,true);
                Game.TakeObjectFromHero(ParentClass.hero_object[i]);
                ParentClass.AddSpawnedFunction(new CSpawnedFunc1<CQ_OpeningGraveyardSecretPassageScript,CScriptThing>(CCharString("ParentClass.")+CCharString("FadeObjectUp"),0,&ParentClass,&CQ_OpeningGraveyardSecretPassageScript::FadeObjectUp,new_item),CCharString(""));
                if (!first_time) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
        }
        collected_all=true;
        for (int j=0; j<LAST; ++j) {
            ParentClass.SkeletonStuff[j]=!returned[j];
            if (!returned[j]) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                collected_all=false;
            }
        }
        if (collected_all) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.GiveHeroExperience(pGlobalScriptDef->EXPERIENCE_GRANT_LARGE);
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            ParentClass.DoorChange=true;
            {
                CScriptGameResourceObjectScriptedThingBase seh_Demon;
                CScriptGameResourceObjectScriptedThingBase seh_hero;
                Game.StartScriptingEntity(Game.GetThingWithScriptName("GraveyardDemon"),seh_Demon,SCRIPT_AI_PRIORITY_HIGHEST);
                Game.StartScriptingEntity(Game.GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
                mymap["HERO"]=seh_hero;
                mymap["DOOR"]=seh_Demon;
                Game.FixMovieSequenceCamera(true);
                RunCutsceneMacro("CS_OPENGRAVE_SKELETON_COMPLETE",mymap,0,0,false,true);
                Game.FixMovieSequenceCamera(false);
                CScriptThing door=Game.GetThingWithScriptName("DemonDoorGate");
                Game.SetThingAsUsable(Game.GetThingWithScriptName("DemonDoorGate"),true);
                Game.OpenDoor(door);
                Game.SetThingPersistent(door,true);
            }
            ParentClass.finished_quest=true;
            Game.SetQuestCardObjective(Game.GetActiveQuestName(),"TEXT_QUEST_GRAVEYARD_SECRET_PASSAGE_OBJECTIVE_04","GraveyardCircle","Graveyard");
        }
        else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            Game.StartScriptingEntity(Game.GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["HERO"]=seh_hero;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            Game.FixMovieSequenceCamera(true);
            if (!Game.IsObjectInThingsPossession(ParentClass.hero_object[SWORD],Game.GetHero()) && !returned[SWORD]) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                RunCutsceneMacro("CS_OPENGRAVE_SKELETON_REMIND_SWORD",mymap,0,0,false,true);
            }
            else if (!Game.IsObjectInThingsPossession(ParentClass.hero_object[SHIELD],Game.GetHero()) && !returned[SHIELD]) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                RunCutsceneMacro("CS_OPENGRAVE_SKELETON_REMIND_SHIELD",mymap,0,0,false,true);
            }
            else if (!Game.IsObjectInThingsPossession(ParentClass.hero_object[ARMOUR],Game.GetHero()) && !returned[ARMOUR]) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                RunCutsceneMacro("CS_OPENGRAVE_SKELETON_REMIND_ARMOUR",mymap,0,0,false,true);
            }
            else if (!Game.IsObjectInThingsPossession(ParentClass.hero_object[HELMET],Game.GetHero()) && !returned[HELMET]) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                RunCutsceneMacro("CS_OPENGRAVE_SKELETON_REMIND_HELMET",mymap,0,0,false,true);
            }
            Game.FixMovieSequenceCamera(false);
            Game.SetQuestCardObjective(Game.GetActiveQuestName(),"TEXT_QUEST_GRAVEYARD_SECRET_PASSAGE_OBJECTIVE_03","","Graveyard");
        }
        first_time=false;
    }
    while (!IsActiveThreadTerminating()) Game.NewScriptFrame();
}
