// NScript::CV_TravellingHeroesScript::CTH_Trigger_Bowerstone::Main() -- manage Thunder and followers across quest progress and day/night transitions.
// Leaf: ?Main@CTH_Trigger_Bowerstone@CV_TravellingHeroesScript@NScript@@EAEXXZ
// Exact private virtual leaf from both original PDBs; native vtable 012f439c slot 1.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CV_TravellingHeroesScript.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingAlive.h"
#include "engine/C3DVector.h"
#include "engine/fable_script_distance.h"
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
inline const C3DVector& CScriptThing::GetPos() const { if(!PImp.Get()) return NullCoord; return PImp->GetPos(); }

void NScript::CV_TravellingHeroesScript::CTH_Trigger_Bowerstone::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if(IsActiveThreadTerminating()) return;
    Game->SetThingPersistent(Me,true);
    EGameflowPosition GameflowPosition = PMasterData->PostSavePosition;
    if(GameflowPosition > EGP_ARENA || ParentClass.ThunderSceneStarted) {
        if(IsActiveThreadTerminating()) return;
        Game->RemoveRumourCategory("TravHero Thunder");
        Game->RemoveThing(Me,false,true);
    } else if(GameflowPosition > EGP_ORCHARD_FARM) {
        if(IsActiveThreadTerminating()) return;
        ParentClass.GossipSetup("TravHero Thunder","TEXT_AI_GOSSIP_THUNDER_BOWERSTONE","VILLAGE_BOWERSTONE_SLUMS","FACTION_VILLAGERS");
        C3DVector MyPos = Me.GetPos();
        CScriptThing Thunder = Game->GetThingWithScriptName("TH_Thunder");
        CScriptThing Boy = Game->GetThingWithScriptName("TH_ThunderBoy");
        CScriptThing Girl = Game->GetThingWithScriptName("TH_ThunderGirl");
        if(!Thunder.IsAlive() && !IsNightTime()) {
            if(IsActiveThreadTerminating()) return;
            ThunderInPlay = true;
            Thunder = Game->CreateCreature("CREATURE_RIVAL_HERO_THUNDER",MyPos,"TH_Thunder",false);
            Boy = Game->CreateCreature("CREATURE_BS_VILLAGER_BOY",Game->GetThingWithScriptName("TH_ThunderBoyMarker2").GetPos(),"TH_ThunderBoy",false);
            Girl = Game->CreateCreature("CREATURE_BS_VILLAGER_GIRL",Game->GetThingWithScriptName("TH_ThunderGirlMarker2").GetPos(),"TH_ThunderGirl",false);
        } else if(Thunder.IsAlive()) {
            if(IsActiveThreadTerminating()) return;
            ThunderInPlay = true;
        }
        while(!ParentClass.ThunderSceneStarted) {
            Game->NewScriptFrame();
            if(IsActiveThreadTerminating()) return;
            if(IsNightTime()) {
                if(IsActiveThreadTerminating()) return;
                if(ThunderInPlay && !Game->IsCameraPosOnScreen(Thunder.GetPos()) && IsDistanceBetweenThingsOver(Thunder,Game->GetHero(),25.0f)) {
                    if(IsActiveThreadTerminating()) return;
                    Game->RemoveThing(Boy,false,true);
                    Game->RemoveThing(Girl,false,true);
                    Game->RemoveThing(Thunder,false,true);
                    ThunderInPlay = false;
                }
            } else {
                if(IsActiveThreadTerminating()) return;
                if(!ThunderInPlay && !IsNightTime() && !Game->IsCameraPosOnScreen(MyPos) && IsDistanceBetweenThingsOver(Me,Game->GetHero(),25.0f)) {
                    if(IsActiveThreadTerminating()) return;
                    Thunder = Game->CreateCreature("CREATURE_RIVAL_HERO_THUNDER",MyPos,"TH_Thunder",false);
                    Boy = Game->CreateCreature("CREATURE_BS_VILLAGER_BOY",Game->GetThingWithScriptName("TH_ThunderBoyMarker2").GetPos(),"TH_ThunderBoy",false);
                    Girl = Game->CreateCreature("CREATURE_BS_VILLAGER_GIRL",Game->GetThingWithScriptName("TH_ThunderGirlMarker2").GetPos(),"TH_ThunderGirl",false);
                    ThunderInPlay = true;
                }
            }
        }
        if(IsActiveThreadTerminating()) return;
        Game->RemoveRumourCategory("TravHero Thunder");
        Game->RemoveThing(Me,false,true);
    }
}