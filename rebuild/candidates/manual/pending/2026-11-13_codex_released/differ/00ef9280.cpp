// NScript::Cjoss_test_scriptsScript::Main() -- Joss's test level script: activate guy_script and bandit_level_cutscene; then every frame look up fireh / summoner and keep the summoner scripted, firing its lightning orb at fireh when idle; poke jim's hit-message queries and the hero's flourish queries; look up bob (combo multiplier on), my_generator (depleted query), pinky and perky; once, set every dan_bandit on the hero
// Grade: DIFFER(1972v3158) -- md 795: inline-split residue: retail expands ~CScriptThing at two exit pads only, CScriptThing::operator= at the fireh site only and IsAlive per site; this source calls every dtor (an inline dtor expands every site: 3488 bytes, md 1006; inline operator= expands every assignment)
// Next idea: the chunk 838 idle-forever `while (true) { NSF; if (T) return; }` form with the dtor inline, plus a single-site do{}while(0) on the fireh assignment (with the line-2 Inline-split note)
// Leaf: ?Main@Cjoss_test_scriptsScript@NScript@@UAEXXZ
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/Cjoss_test_scriptsScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "stlport/fable_containers.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
template<> std::vector<CScriptThing>::~vector();              // 0x008AC970
inline CScriptThing::CScriptThing() {}
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
inline bool CScriptThing::IsAlive() const { return !PImp.IsNull() && PImp->IsAlive(); }
inline bool CScriptThing::MsgIsHitByWithFlourish(const CCharString& name) const { if (PImp.IsNull()) return false; return PImp->MsgIsHitByWithFlourish(name); }
inline bool CScriptThing::MsgIsHitByWithProjectileWeapon(const CCharString& name, float& distance) const { if (PImp.IsNull()) return false; return PImp->MsgIsHitByWithProjectileWeapon(name, distance); }

void NScript::Cjoss_test_scriptsScript::Main()
{
    CScriptThing bob;
    CScriptThing jim;
    CScriptThing my_generator;
    CScriptThing dan;
    CScriptThing pinky;
    CScriptThing perky;
    bool bandits_attacking = false;
    CScriptThing summoner;
    CScriptThing fireh;
    CScriptGameResourceObjectScriptedThingBase seh_summoner;
    Game->ActivateQuest("guy_script");
    Game->ActivateQuest("bandit_level_cutscene");
    for (; !IsActiveThreadTerminating(); Game->NewScriptFrame()) {
        if (!fireh.IsAlive()) {
            if (IsActiveThreadTerminating()) return;
            fireh = Game->GetThingWithScriptName("fireh");
        } else {
            if (IsActiveThreadTerminating()) return;
            if (!summoner.IsAlive()) {
                if (IsActiveThreadTerminating()) return;
                summoner = Game->GetThingWithScriptName("summoner");
            } else if (!seh_summoner.IsValid()) {
                if (IsActiveThreadTerminating()) return;
                Game->StartScriptingEntity(summoner, seh_summoner, SCRIPT_AI_PRIORITY_HIGHEST);
            } else {
                if (IsActiveThreadTerminating()) return;
                if (!seh_summoner.IsPerformingScriptTask()) {
                    if (IsActiveThreadTerminating()) return;
                    seh_summoner.SummonerLightningOrbAttackTarget(fireh);
                }
            }
        }
        if (!jim.IsAlive()) {
            if (IsActiveThreadTerminating()) return;
            jim = Game->GetThingWithScriptName("jim");
        } else {
            if (IsActiveThreadTerminating()) return;
            jim.MsgIsHitByWithFlourish("SCRIPT_NAME_HERO");
            float distance = 0.0f;
            jim.MsgIsHitByWithProjectileWeapon("SCRIPT_NAME_HERO", distance);
        }
        Game->GetHero().MsgPerformedFlourish();
        Game->GetHero().MsgPerformedSuccessfulFlourish();
        if (!bob.IsAlive()) {
            if (IsActiveThreadTerminating()) return;
            bob = Game->GetThingWithScriptName("bob");
        } else {
            if (IsActiveThreadTerminating()) return;
            Game->EntitySetAsToAddToComboMultiplierWhenHit(bob, true);
        }
        if (!my_generator.IsAlive()) {
            if (IsActiveThreadTerminating()) return;
            my_generator = Game->GetThingWithScriptName("my_generator");
        } else {
            if (IsActiveThreadTerminating()) return;
            Game->CreatureGeneratorIsDepleted(my_generator);
        }
        if (!pinky.IsAlive()) {
            if (IsActiveThreadTerminating()) return;
            pinky = Game->GetThingWithScriptName("pinky");
        }
        if (!perky.IsAlive()) {
            if (IsActiveThreadTerminating()) return;
            perky = Game->GetThingWithScriptName("perky");
        }
        if (pinky.IsAlive() && perky.IsAlive()) {
            if (IsActiveThreadTerminating()) return;
        }
        if (!bandits_attacking) {
            if (IsActiveThreadTerminating()) return;
            std::vector<CScriptThing> bandits;
            Game->GetAllThingsWithScriptName("dan_bandit", bandits);
            for (unsigned long i = 0; i < bandits.size(); i++) {
                bandits_attacking = true;
                Game->GiveThingBestEnemyTarget(bandits[i], Game->GetHero());
            }
        }
    }
}
