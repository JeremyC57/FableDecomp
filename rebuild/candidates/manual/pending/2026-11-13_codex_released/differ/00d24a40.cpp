// NScript::CQ_BowerstoneTownLifeIntroScript::CBowerstoneIntroGuardToPosh::Main() -- the guard at the posh-district gate: take scripted control, wait for DoneGuardSpeech, face M_BTLI_GuardPoshFaceMe when it is alive; then each frame wait for the hero to come within BowerstoneIntroPoshGuardProximity: before the arena is finished cast HERO and Guard in CS_BOWERSTONE_INTRO_POSH_LEAVE; after it, the first time cast them in CS_BOWERSTONE_INTRO_POSH_ENTER, set PoshGuardComplete, release the guard, drop the alliances and the information marker; once complete, when talked to retake control and say TEXT_QST_037_GUARD_POSH_REMINDER under a paused speech movie
// Grade: DIFFER(2440v2403) -- 175 normalised lines: retail keeps zero in edi and VC7.1 picks ebx (push order and every zero compare follow), the frame slots of the ENTER/LEAVE cut-scene blocks differ, and the REMINDER speech movie dtor is called (0x006E7B80 path) where retail expands it; calls, strings and control flow otherwise as retail
// Next idea: the REMINDER block is the only movie dtor still called: try the reacquire loop out of the movie block (retail order permitting) or a single do-while(0) round the talked-to branch instead of the nested pair, then attack the edi/ebx choice by moving the PoshGuardComplete store
// Inline-split: do { } while (0) round the three movie scopes (ENTER, REMINDER, LEAVE) and one round the REMINDER SAY: the c2 frequency lever of the landed CEndGameScythe::Main 0x00D2ED20, which makes VC7.1 expand the ENTER and LEAVE movie dtors as retail does; retail's construct is unproven
// Leaf: ?Main@CBowerstoneIntroGuardToPosh@CQ_BowerstoneTownLifeIntroScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D24A40, 2403 bytes, thiscall, plain `ret`.
// Identity (proven): slot 1 (Main) of the RTTI-less vtable 0x012CB5E0 stored by the landed Alloc 0x00D24330 (Init 0x00D243E0,
//   GetParentScript 0x00D243D0 landed); Ego_r.pdb ?Main@CBowerstoneIntroGuardToPosh@CQ_BowerstoneTownLifeIntroScript@NScript@@EAEXXZ.
// Callees: CBaseObject 0x0099A380 / operator= 0x0099A3B0 / ~ 0x0099A430, ScriptedThingBase ctor 0x007E72A0 (seh_hero; seh_me's
//   expanded) / IsValid 0x00CD23B9 / Release 0x00CD2770 / dtor 0x007E74D0 / GetScriptThing 0x007E7490 / Speak 0x007E7390 /
//   IsPerformingScriptTask 0x007E7450, CCountedPointer<ScriptedThingBase>::operator= 0x008AB1E0, the cast map
//   std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> ctor 0x00CDBF70 / dtor 0x00CDBFB0 / operator[] 0x00CD3D2E,
//   RunCutsceneMacro 0x00CBFB7D, IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall), the MovieBase PImp dtor
//   CCountedPointer<MovieBase>::~ 0x006E7AB0 (movie ctor and dtor expanded), ~CScriptThing 0x004AA840, CCharString 0x0099EBF0 /
//   ~ 0x0099EAE0, operator delete 0x00BFE9BC, pGlobalScriptDef 0x0143E90C (BowerstoneIntroPoshGuardProximity +0xB80);
//   CGameScriptInterfaceBase slots 7, 8, 70 GetHero, 72 GetThingWithScriptName, 264 GetHealth, 361 ClearThingHasInformation, 370
//   StartMovieSequence, 371 FixMovieSequenceCamera, 379 PauseAllNonScriptedEntities, 475 EntitySetFacingAngleTowardsThing, 600
//   EntityUnsetThingAsAllyOfThing; CScriptThing slots 27 MsgIsTalkedToBy, 75 IsAlive (expanded over PImp).
// Layout: Ego_r.pdb CBowerstoneIntroGuardToPosh (28 bytes = retail new size): ParentClass +0x14, PMasterData +0x18; parent
//   CQ_BowerstoneTownLifeIntroScript ArenaFinished / DoneGuardSpeech / PoshGuardComplete Ego_r +0x5c/+0x5d/+0x5e = retail
//   +0x48/+0x49/+0x4a (CLevelScriptBase 0x48 under FABLE_RETAIL_CLEVELSCRIPTBASE; the landed Init 0x00D243E0 and Guard Main
//   0x00D246C0 read the same offsets).
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CQ_SunnyvaleMasterData.h"
class CScriptGameResourceObjectScriptedThingBase;
template <> CCountedPointer<CScriptGameResourceObjectScriptedThingBase>& CCountedPointer<CScriptGameResourceObjectScriptedThingBase>::operator=(const CCountedPointer<CScriptGameResourceObjectScriptedThingBase>&);   // 0x008AB1E0
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_cutscene.h"
#include "engine/fable_script_def_global.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
inline bool CScriptThing::IsAlive() const { if (PImp.IsNull()) return false; return PImp->IsAlive(); }
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();              // 0x00CDBF70
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();             // 0x00CDBFB0
__forceinline CScriptGameResourceObjectScriptedThingBase& CScriptGameResourceObjectScriptedThingBase::operator=(const CScriptGameResourceObjectScriptedThingBase& other) { CScriptGameResourceObjectBase::operator=(other); PImp = other.PImp; return *this; }
bool FABLE_FASTCALL IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float);   // 0x00CBE2FF

namespace NScript {
class CQ_BowerstoneTownLifeIntroScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool ArenaFinished;                             // +0x48 (Ego_r +0x5c)
    bool DoneGuardSpeech;                           // +0x49 (Ego_r +0x5d)
    bool PoshGuardComplete;                         // +0x4a (Ego_r +0x5e)
    class CBowerstoneIntroGuardToPosh;
};
class CQ_BowerstoneTownLifeIntroScript::CBowerstoneIntroGuardToPosh : public CEntityScriptBase {
public:
    CQ_BowerstoneTownLifeIntroScript* ParentClass;  // +0x14
    CQ_SunnyvaleMasterData* PMasterData;            // +0x18
private:
    virtual void Main();
};
}

void NScript::CQ_BowerstoneTownLifeIntroScript::CBowerstoneIntroGuardToPosh::Main()
{
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    while (!ParentClass->DoneGuardSpeech) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    CScriptThing face_me = Game->GetThingWithScriptName("M_BTLI_GuardPoshFaceMe");
    if (face_me.IsAlive()) {
        if (IsActiveThreadTerminating()) return;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        Game->EntitySetFacingAngleTowardsThing(Me, face_me, true);
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    while (!IsActiveThreadTerminating()) {
        while (!IsDistanceBetweenThingsUnder(Game->GetHero(), Me, pGlobalScriptDef->BowerstoneIntroPoshGuardProximity)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        if (ParentClass->ArenaFinished) {
            if (IsActiveThreadTerminating()) return;
            if (!ParentClass->PoshGuardComplete) {
                if (IsActiveThreadTerminating()) return;
                std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                CScriptGameResourceObjectScriptedThingBase seh_hero;
                while (!Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
                mymap["HERO"] = seh_hero;
                mymap["Guard"] = seh_me;
                do {
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    Game->FixMovieSequenceCamera(true);
                    RunCutsceneMacro("CS_BOWERSTONE_INTRO_POSH_ENTER", mymap, 0, 0, false, true);
                    Game->FixMovieSequenceCamera(false);
                    ParentClass->PoshGuardComplete = true;
                    if (seh_me.IsValid()) seh_me.Release();
                    Game->EntityUnsetThingAsAllyOfThing(Me, Game->GetHero());
                    Game->EntityUnsetThingAsAllyOfThing(Game->GetHero(), Me);
                    Game->ClearThingHasInformation(Me);
                } while (0);
            } else {
                if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                    if (IsActiveThreadTerminating()) return;
                    do {
                        CScriptGameResourceObjectMovieBase speech_movie;
                        Game->StartMovieSequence("", speech_movie);
                        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                        if (seh_me.IsValid()) seh_me.Release();
                        while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return;
                        }
                        if (IsActiveThreadTerminating()) return;
                        do {
                            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                                seh_me.Speak(Game->GetHero(), "TEXT_QST_037_GUARD_POSH_REMINDER", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game->NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return;
                                }
                                if (IsActiveThreadTerminating()) return;
                            }
                        } while (0);
                        if (seh_me.IsValid()) seh_me.Release();
                    } while (0);
                }
            }
        } else {
            if (IsActiveThreadTerminating()) return;
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            while (!Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            mymap["HERO"] = seh_hero;
            mymap["Guard"] = seh_me;
            do {
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                Game->FixMovieSequenceCamera(true);
                RunCutsceneMacro("CS_BOWERSTONE_INTRO_POSH_LEAVE", mymap, 0, 0, false, true);
                Game->FixMovieSequenceCamera(false);
            } while (0);
        }
        Game->NewScriptFrame();
    }
}
