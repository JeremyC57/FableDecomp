// NScript::CQ_HobbeToothContestScript::CSpawnNewHobbe::Main() -- replenish distant contest hobbes and choose the nearer opponent.
// Leaf: ?Main@CSpawnNewHobbe@CQ_HobbeToothContestScript@NScript@@EAEXXZ
// Grade: DIFFER(1597v1693) -- later handle assignments remain calls; frame0x94 agrees but slots and register roles differ.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CENTITYSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTTHING
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#include "engine/CQ_HobbeToothContestScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "stlport/fable_containers.h"

// Both original PDB name sets; native helpers00cbe2ff and00cbe583.
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing&,const CScriptThing&,float);
float __fastcall GetSquaredDistanceBetweenThings(const CScriptThing&,const CScriptThing&);

inline CScriptThing::CScriptThing() : CBaseClass(),PImp() {}
inline CScriptThing::~CScriptThing() {}

void NScript::CQ_HobbeToothContestScript::CSpawnNewHobbe::Main()
{
    while (ParentClass.DrinkingHobbesNotHit) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    while (!ParentClass.CS_OutroBegin) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        std::vector<CScriptThing> AllSpawnedHobbes;
        Game.GetAllThingsWithScriptName("Q_HobbeToothHobbe",AllSpawnedHobbes);
        if (!IsDistanceBetweenThingsUnder(Game.GetHero(),Me,10.0f)
            && !IsDistanceBetweenThingsUnder(Game.GetThingWithScriptName("Q_HobbeToothWhisper"),Me,10.0f)
            && AllSpawnedHobbes.size()<3) {
            if (IsActiveThreadTerminating()) return;
            CScriptThing NewHobbe;
            if (rand()%4>0) {
                if (IsActiveThreadTerminating()) return;
                NewHobbe=Game.CreateCreature("CREATURE_HOBBE_GRUNT",Me.GetPos(),"Q_HobbeToothHobbe",false);
            } else if (rand()%4>0) {
                if (IsActiveThreadTerminating()) return;
                NewHobbe=Game.CreateCreature("CREATURE_HOBBE_SPELLCASTER",Me.GetPos(),"Q_HobbeToothHobbe",false);
            } else {
                if (IsActiveThreadTerminating()) return;
                NewHobbe=Game.CreateCreature("CREATURE_HOBBE_LIEUTENANT",Me.GetPos(),"Q_HobbeToothHobbe",false);
            }
            float HeroDistance=GetSquaredDistanceBetweenThings(NewHobbe,Game.GetHero());
            float WhisperDistance=GetSquaredDistanceBetweenThings(NewHobbe,Game.GetThingWithScriptName("Q_HobbeToothWhisper"));
            if (WhisperDistance<HeroDistance) {
                if (IsActiveThreadTerminating()) return;
                Game.GiveThingBestEnemyTarget(NewHobbe,Game.GetThingWithScriptName("Q_HobbeToothWhisper"));
            } else {
                if (IsActiveThreadTerminating()) return;
                Game.GiveThingBestEnemyTarget(NewHobbe,Game.GetHero());
            }
            Game.Pause(8.0f);
        }
    }
    if (IsActiveThreadTerminating()) return;
}
