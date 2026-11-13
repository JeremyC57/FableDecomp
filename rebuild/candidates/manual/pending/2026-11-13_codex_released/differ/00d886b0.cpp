// NScript::CQ_HeroSoulsNostroScript::CGuardNearGraveyardPath::Main() -- a graveyard-path guard: live while alive, take scripted control and clear his information marker; each frame: when talked to say TEXT_QST_B04_<data>_ONTALK under a paused speech movie; when hit by the hero (not a heal-life spell) count the hit, pick TEXT_QST_B04_<data>_ONHIT_10 / _20 / _30 and say it (unless in a cut scene or dead); under three hits stay allied with the hero, on the third turn on him for good
// Grade: DIFFER(2128v2064) -- 150 normalised lines: VC7.1 expands the first speech movie's pad dtor and the second movie's ctor and dtor, where retail calls 0x006E7B80-family out-of-line bodies at those sites (the per-site movie split), and the two movie frame slots are swapped; strings, callees and control flow otherwise as retail
// Next idea: make the on-hit movie site colder than the talked-to one without a frequency loop (q1/q2 do-whiles made it hotter): try the on-hit branch as an else-if chain or a separate member-free helper scope, and declare the two movies in retail's slot order (on-hit first) to fix the swapped slots
// Leaf: ?Main@CGuardNearGraveyardPath@CQ_HeroSoulsNostroScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D886B0, 2064 bytes, thiscall, plain `ret` (five epilogue copies), ends on its last `ret`.
// Identity (proven): slot 1 (Main) of the CGuardNearGraveyardPath entity vtable stored by the landed Alloc 0x00D885E0
//   (GetParentScript 0x00D88680 landed); Ego_r.pdb ?Main@CGuardNearGraveyardPath@CQ_HeroSoulsNostroScript@NScript@@EAEXXZ.
// Callees: CIsThingAlive (vtable 0x012C32F0; ctor and dtor inline) + CScriptThing copy 0x004ABE90 / ~ 0x004AA840, SetPredicate
//   0x00F35B10, IsActiveThreadTerminating 0x00F35B30, CBaseObject 0x0099A380 / ~ 0x0099A430, ScriptedThingBase IsValid 0x00CD23B9 /
//   Release 0x00CD2770 / dtor 0x007E74D0 / GetScriptThing 0x007E7490 / Speak(.., const char*, ..) 0x007E7390 / IsPerformingScriptTask
//   0x007E7450, MovieBase ctor 0x006E7B60 / dtor 0x006E7B80 and the PImp dtor CCountedPointer<MovieBase>::~ 0x006E7AB0 (the first
//   movie's ctor and natural-exit dtor expanded, the second movie's called), CCharString() 0x0099E4B0 / (const char*, long) 0x0099EBF0 /
//   ~ 0x0099EAE0 / operator= 0x0099EFB0 / operator const char* 0x0099E4C0, operator+(const char*, const CCharString&) 0x0099F690,
//   operator+(const CCharString&, const char*) 0x0099F600; CGameScriptInterfaceBase slots 7, 8, 70 GetHero, 264 GetHealth, 360
//   SetThingHasInformation, 370 StartMovieSequence, 379 PauseAllNonScriptedEntities, 386 IsInCutscene, 451 GiveThingBestEnemyTarget,
//   599 / 600 EntitySetThingAsAllyOfThing / EntityUnsetThingAsAllyOfThing; CScriptThing slots 3 GetDataString, 21 MsgIsHitBy, 27
//   MsgIsTalkedToBy, 41 / 42 MsgIsHitBySpecialAbilityFrom / MsgIsHitByAnySpecialAbilityFrom.
// Layout: Ego_r.pdb CGuardNearGraveyardPath (28 bytes = retail new size): ParentClass +0x14, PMasterData +0x18; no own members.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();                        // 0x006E7AB0
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CIsThingAlive.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_HEAL_LIFE_SPELL=14 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HeroSoulsNostroScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CGuardNearGraveyardPath;
};
class CQ_HeroSoulsNostroScript::CGuardNearGraveyardPath : public CEntityScriptBase {
public:
    CQ_HeroSoulsNostroScript* ParentClass;          // +0x14
    CQ_SunnyvaleMasterData* PMasterData;            // +0x18
private:
    virtual void Main();
};
}

void NScript::CQ_HeroSoulsNostroScript::CGuardNearGraveyardPath::Main()
{
    {
        CIsThingAlive alive(Me);
        SetPredicate(alive);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid())
        seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetThingHasInformation(Me, false, false, false);
    long times_hit = 0;
    while (!IsActiveThreadTerminating()) {
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game->GetHero(), "TEXT_QST_B04_" + Me.GetDataString() + "_ONTALK", (ETextGroupSelectionMethod)0, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") ||
            (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return;
            ++times_hit;
            CCharString on_hit_text;
            switch (times_hit) {
            case 1:
                on_hit_text = "TEXT_QST_B04_" + Me.GetDataString() + "_ONHIT_10";
                break;
            case 2:
                on_hit_text = "TEXT_QST_B04_" + Me.GetDataString() + "_ONHIT_20";
                break;
            case 3:
                on_hit_text = "TEXT_QST_B04_" + Me.GetDataString() + "_ONHIT_30";
                break;
            default:
                on_hit_text = "TEXT_QST_B04_" + Me.GetDataString() + "_ONHIT_30";
                break;
            }
            if (!Game->IsInCutscene() && Game->GetHealth(Me) > 0.0001f) {
                if (IsActiveThreadTerminating())
                    return;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), on_hit_text, (ETextGroupSelectionMethod)0, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    if (IsActiveThreadTerminating())
                        return;
                }
            }
            if (times_hit < 3) {
                if (IsActiveThreadTerminating())
                    return;
                Game->EntitySetThingAsAllyOfThing(Me, Game->GetHero());
                Game->EntitySetThingAsAllyOfThing(Game->GetHero(), Me);
            }
            else {
                if (IsActiveThreadTerminating())
                    return;
                Game->EntityUnsetThingAsAllyOfThing(Me, Game->GetHero());
                Game->EntityUnsetThingAsAllyOfThing(Game->GetHero(), Me);
                Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
                if (seh_me.IsValid())
                    seh_me.Release();
                for (;;) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
            }
        }
        Game->NewScriptFrame();
    }
}
