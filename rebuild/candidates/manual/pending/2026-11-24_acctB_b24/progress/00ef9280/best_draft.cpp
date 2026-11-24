// NScript::Cjoss_test_scriptsScript::Main() -- Joss's test level script: activate guy_script and bandit_level_cutscene; then every frame look up fireh / summoner and keep the summoner scripted, firing its lightning orb at fireh when idle; poke jim's hit-message queries and the hero's flourish queries; look up bob (combo multiplier on), my_generator (depleted query), pinky and perky; once, set every dan_bandit on the hero
// Grade: DIFFER(2622v3158) 80 hunks (B24S2 port) -- call counts vs retail: ~CScriptThing called 14 = retail; retail keeps TWO expanded exit pads (0x00EF9A67 loop-head T; 0x00EF9C55 the two fireh-branch Ts), ours expands the same sites but tail-merges them into one pad (retail's pads differ in the STB Data/Info zero-store order, so they do not merge); operator= called 6 v 5, IsAlive called 1 v 3
// Leaf: ?Main@Cjoss_test_scriptsScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EF9280, 3158 bytes, thiscall, plain `ret`.
// Shape: FableWin 0x03AF7BD0 (a later build with extra statements: only the shared statements are kept) -- locals (FableWin.pdb)
//   straw_bob, jim, generator, camera, pinky, perky, bandits_done, summoner, fire_heart, sum_stuff, _Fail_Val; sthings / n (long);
//   TRetVoid `_Fail_Val` exits; for-scope openers on the main loop and the bandit loop.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_PHASE1_ARCHER_PASS26
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/Cjoss_test_scriptsScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/EScriptAIPriority.h"
#include "engine/TRetVoid.h"
#include "stlport/fable_containers.h"
template<> std::vector<CScriptThing>::~vector();              // 0x008AC970
inline CScriptThing::CScriptThing() {}
inline CScriptThing::~CScriptThing() {}
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
inline bool CScriptThing::MsgIsHitByWithFlourish(const CCharString& name) const { if (!PImp.Data) return false; return PImp.Data->MsgIsHitByWithFlourish(name); }
inline bool CScriptThing::MsgIsHitByWithProjectileWeapon(const CCharString& name, float& distance) const { if (!PImp.Data) return false; return PImp.Data->MsgIsHitByWithProjectileWeapon(name, distance); }

void NScript::Cjoss_test_scriptsScript::Main()
{
    TRetVoid _Fail_Val;
    CScriptThing straw_bob;
    CScriptThing jim;
    CScriptThing generator;
    CScriptThing camera;
    CScriptThing pinky;
    CScriptThing perky;
    bool bandits_done = false;
    CScriptThing summoner;
    CScriptThing fire_heart;
    CScriptGameResourceObjectScriptedThingBase sum_stuff;
    Game->ActivateQuest("guy_script");
    Game->ActivateQuest("bandit_level_cutscene");
    if (false) {} else for (;; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (!fire_heart.IsAlive()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            fire_heart = Game->GetThingWithScriptName("fireh");
        } else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (!summoner.IsAlive()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                summoner = Game->GetThingWithScriptName("summoner");
            } else if (!sum_stuff.IsValid()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->StartScriptingEntity(summoner, sum_stuff, SCRIPT_AI_PRIORITY_HIGHEST);
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (!sum_stuff.IsPerformingScriptTask()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    sum_stuff.SummonerLightningOrbAttackTarget(fire_heart);
                }
            }
        }
        if (!jim.IsAlive()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            jim = Game->GetThingWithScriptName("jim");
        } else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            jim.MsgIsHitByWithFlourish("SCRIPT_NAME_HERO");
            float pwr = 0.0f;
            jim.MsgIsHitByWithProjectileWeapon("SCRIPT_NAME_HERO", pwr);
        }
        Game->GetHero().MsgPerformedFlourish();
        Game->GetHero().MsgPerformedSuccessfulFlourish();
        if (!straw_bob.IsAlive()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            straw_bob = Game->GetThingWithScriptName("bob");
        } else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntitySetAsToAddToComboMultiplierWhenHit(straw_bob, true);
        }
        if (!generator.IsAlive()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            generator = Game->GetThingWithScriptName("my_generator");
        } else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->CreatureGeneratorIsDepleted(generator);
        }
        if (!pinky.IsAlive()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            pinky = Game->GetThingWithScriptName("pinky");
        }
        if (!perky.IsAlive()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            perky = Game->GetThingWithScriptName("perky");
        }
        if (pinky.IsAlive() && perky.IsAlive()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (!bandits_done) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            std::vector<CScriptThing> sthings;
            Game->GetAllThingsWithScriptName("dan_bandit", sthings);
            if (false) {} else for (long n = 0; n < sthings.size(); ++n) {
                bandits_done = true;
                Game->GiveThingBestEnemyTarget(sthings[n], Game->GetHero());
            }
        }
    }
}
