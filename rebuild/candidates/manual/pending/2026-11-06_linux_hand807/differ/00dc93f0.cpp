// NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageSkeleton::Main() -- the crypt skeleton: plays the crypt-entry cutscene when the hero reaches CryptCutsceneMarker, then each time the hero uses it takes back the artifacts he carries (each fades up on the skeleton), and either plays the completion cutscene that opens the demon door or reminds him of the next missing artifact.
// Grade: DIFFER(4536v4507) -- retail calls the CScriptThing copy inside the CSpawnedFunc1 Par1 mem-initializer but expands the by-value argument copy and calls its ~temp (ours calls both); end-pad movie dtor split; `cmp [esi+edi+0x1c],bl` form of the returned[] test (320 lines, SDIFF 9)
// Next idea: the same split as 0x00ECF7F0: build the by-value argument from a named local with IMPLICIT_COPY and keep CSpawnedFunc1 Par1 copy out of line through a declared-only ctor specialization; else lane gap: per-site inline choice (gotchas 2131/2248)
// Leaf: ?Main@CSecretPassageSkeleton@CQ_OpeningGraveyardSecretPassageScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DC93F0, length 4507 bytes.
// Name: slot 1 of the entity vtable 0x012DA494 (0 scalar deleting dtor 0x00DC3EA0, 2 Init 0x00DC3E50, 3 GetParentScript
//   0x00DC3E40, all landed as CSecretPassageSkeleton); Ego_r.pdb and FableWin.pdb both hold
//   ?Main@CSecretPassageSkeleton@CQ_OpeningGraveyardSecretPassageScript@NScript@@EAEXXZ.
// Data: ParentClass finished_quest +0x48, DoorChange +0x49, skeleton_visited +0x4B, SkeletonStuff[4] +0x51, hero_object[4] +0x58;
//   pGlobalScriptDef EXPERIENCE_GRANT_LARGE +0x44. Generator: probe/gen_dc93f0.py a6 stdtor cpdecl xstr.
// Shape: spawn name an explicit CCharString("") (built after the new expression, as retail); IsDistanceBetweenThingsUnder(hero,
//   marker, 2.0f) argument order from retail's pushes; FABLE_RETAIL_CCHARSTRING_CONCAT_STRING for the thread name.
// Tried (10): see STATUS.tsv.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING
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
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0
inline CScriptThing::~CScriptThing() {}
void NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageSkeleton::Main()
{
    bool crypt_cutscene_done=false;
    bool first_use=true;
    Game->SetThingAsUsable(Me,true);
    while (!collected_all) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (!crypt_cutscene_done) {
            if (IsActiveThreadTerminating()) return;
            while (!IsDistanceBetweenThingsUnder(Game->GetHero(),Game->GetThingWithScriptName("CryptCutsceneMarker"),2.0f)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            crypt_cutscene_done=true;
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            Game->StartScriptingEntity(Game->GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["HERO"]=seh_hero;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("",movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            Game->FixMovieSequenceCamera(true);
            RunCutsceneMacro("CS_OPENGRAVE_CRYPT_ENTER",mymap,0,0,false,true);
            Game->FixMovieSequenceCamera(false);
            Game->SetQuestCardObjective(Game->GetActiveQuestName(),"TEXT_QUEST_GRAVEYARD_SECRET_PASSAGE_OBJECTIVE_03","","Graveyard");
        }
        ParentClass.skeleton_visited=true;
        while (!Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        for (long i=0; i<LAST; ++i) {
            if (Game->IsObjectInThingsPossession(ParentClass.hero_object[i],Game->GetHero())) {
                if (IsActiveThreadTerminating()) return;
                returned[i]=true;
                CScriptThing object=Game->CreateObjectOnEntity(skeleton_object[i],Me,"");
                Game->SetThingPersistent(object,true);
                Game->EntitySetAlpha(object,0.0f,true);
                Game->TakeObjectFromHero(ParentClass.hero_object[i]);
                ParentClass.AddSpawnedFunction(new CSpawnedFunc1<CQ_OpeningGraveyardSecretPassageScript,CScriptThing>(CCharString("ParentClass.")+CCharString("FadeObjectUp"),0,&ParentClass,&CQ_OpeningGraveyardSecretPassageScript::FadeObjectUp,object),CCharString(""));
                if (!first_use) {
                    if (IsActiveThreadTerminating()) return;
                }
            }
        }
        collected_all=true;
        for (long j=0; j<LAST; ++j) {
            ParentClass.SkeletonStuff[j]=!returned[j];
            if (!returned[j]) {
                if (IsActiveThreadTerminating()) return;
                collected_all=false;
            }
        }
        if (collected_all) {
            if (IsActiveThreadTerminating()) return;
            Game->GiveHeroExperience(pGlobalScriptDef->EXPERIENCE_GRANT_LARGE);
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("",movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            ParentClass.DoorChange=true;
            {
                CScriptGameResourceObjectScriptedThingBase seh_demon;
                CScriptGameResourceObjectScriptedThingBase seh_hero;
                Game->StartScriptingEntity(Game->GetThingWithScriptName("GraveyardDemon"),seh_demon,SCRIPT_AI_PRIORITY_HIGHEST);
                Game->StartScriptingEntity(Game->GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
                mymap["HERO"]=seh_hero;
                mymap["DOOR"]=seh_demon;
                Game->FixMovieSequenceCamera(true);
                RunCutsceneMacro("CS_OPENGRAVE_SKELETON_COMPLETE",mymap,0,0,false,true);
                Game->FixMovieSequenceCamera(false);
                CScriptThing gate=Game->GetThingWithScriptName("DemonDoorGate");
                Game->SetThingAsUsable(Game->GetThingWithScriptName("DemonDoorGate"),true);
                Game->OpenDoor(gate);
                Game->SetThingPersistent(gate,true);
            }
            ParentClass.finished_quest=true;
            Game->SetQuestCardObjective(Game->GetActiveQuestName(),"TEXT_QUEST_GRAVEYARD_SECRET_PASSAGE_OBJECTIVE_04","GraveyardCircle","Graveyard");
        }
        else {
            if (IsActiveThreadTerminating()) return;
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            Game->StartScriptingEntity(Game->GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["HERO"]=seh_hero;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("",movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            Game->FixMovieSequenceCamera(true);
            if (!Game->IsObjectInThingsPossession(ParentClass.hero_object[SWORD],Game->GetHero()) && !returned[SWORD]) {
                if (IsActiveThreadTerminating()) return;
                RunCutsceneMacro("CS_OPENGRAVE_SKELETON_REMIND_SWORD",mymap,0,0,false,true);
            }
            else if (!Game->IsObjectInThingsPossession(ParentClass.hero_object[SHIELD],Game->GetHero()) && !returned[SHIELD]) {
                if (IsActiveThreadTerminating()) return;
                RunCutsceneMacro("CS_OPENGRAVE_SKELETON_REMIND_SHIELD",mymap,0,0,false,true);
            }
            else if (!Game->IsObjectInThingsPossession(ParentClass.hero_object[ARMOUR],Game->GetHero()) && !returned[ARMOUR]) {
                if (IsActiveThreadTerminating()) return;
                RunCutsceneMacro("CS_OPENGRAVE_SKELETON_REMIND_ARMOUR",mymap,0,0,false,true);
            }
            else if (!Game->IsObjectInThingsPossession(ParentClass.hero_object[HELMET],Game->GetHero()) && !returned[HELMET]) {
                if (IsActiveThreadTerminating()) return;
                RunCutsceneMacro("CS_OPENGRAVE_SKELETON_REMIND_HELMET",mymap,0,0,false,true);
            }
            Game->FixMovieSequenceCamera(false);
            Game->SetQuestCardObjective(Game->GetActiveQuestName(),"TEXT_QUEST_GRAVEYARD_SECRET_PASSAGE_OBJECTIVE_03","","Graveyard");
        }
        first_use=false;
    }
    while (!IsActiveThreadTerminating()) Game->NewScriptFrame();
}
