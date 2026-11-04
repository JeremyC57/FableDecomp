// NScript::CV_OracleScript::CVO_Woman::Main() -- Oracle village woman: take Me under scripted control (not killable, no combo, no information), then each frame: if the hero talks to her speak the on-talk line the first time and the reminder line afterwards inside a paused movie sequence; if the hero hits her speak the on-hit line and make her and the hero allies
// Grade: DIFFER(1500v1482) -- the hit-block movie ctor: retail calls the COMDAT 0x006E7B60 at the second (hit) site, VC7.1 here expands it at both sites (3 stores + base call vs lea/call); 10 grades
// Leaf: ?Main@CVO_Woman@CV_OracleScript@NScript@@EAEXXZ
// Flags: none
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* m_pStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CIsThingAlive.h"
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
using NScript::CIsThingAlive;
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_OracleScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CVO_Woman : public CEntityScriptBase {
    public:
        CV_OracleScript* ParentClass;                   // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        bool SaidIntro;                                 // +0x1C
    private:
        virtual void Main();                            // PDB ?Main@CVO_Woman@CV_OracleScript@NScript@@EAEXXZ (slot 1)
    };
};
}

void NScript::CV_OracleScript::CVO_Woman::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase face;
    if (face.IsValid())
        face.Release();
    while (!Game->StartScriptingEntity(Me, face, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->EntitySetAsKillable(Me, false, false);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
    Game->SetThingHasInformation(Me, false, false, false);
    if (IsActiveThreadTerminating())
        return;
    do {
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CWideScreenMagicPauseEntities pause(*Game);
            if (!SaidIntro) {
                if (IsActiveThreadTerminating()) 
                    return;
                if (Game->GetHealth(face.GetScriptThing()) > 0.0f) {
                    face.Speak(Game->GetHero(), "TEXT_QST_B20_WOMAN_ON_TALK", (ETextGroupSelectionMethod)0, false, true, false);
                    while (face.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) 
                            return;
                    }
                    if (IsActiveThreadTerminating()) 
                        return;
                }
                SaidIntro = true;
            } else {
                if (IsActiveThreadTerminating()) 
                    return;
                if (Game->GetHealth(face.GetScriptThing()) > 0.0f) {
                    face.Speak(Game->GetHero(), "TEXT_QST_B20_WOMAN_REMINDER", (ETextGroupSelectionMethod)0, false, true, false);
                    while (face.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) 
                            return;
                    }
                    if (IsActiveThreadTerminating()) 
                        return;
                }
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom((EHeroAbility)14, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CWideScreenMagicPauseEntities pause(*Game);
            if (Game->GetHealth(face.GetScriptThing()) > 0.0f) {
                face.Speak(Game->GetHero(), "TEXT_QST_B20_WOMAN_ON_HIT", (ETextGroupSelectionMethod)0, false, true, false);
                while (face.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) 
                        return;
                }
                if (IsActiveThreadTerminating()) 
                    return;
            }
            Game->EntitySetThingAsAllyOfThing(Me, Game->GetHero());
            Game->EntitySetThingAsAllyOfThing(Game->GetHero(), Me);
        }
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}
