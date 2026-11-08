// NScript::CPersonalScript_EmptyScript::CTestPerson::Main() -- a test person: under a conscious filter on Me take scripted control and walk to TestPos; then while the thread runs wait to be talked to by the hero, play the seated daydream loop under a paused movie (and send a fresh hero handle to HeroStandPos), wait to be talked to again, clear the actions and, when alive, say the "no camera tags" line under a second paused movie
// Inline-budget: flag /d2inlT36 -- seh_hero's implicit CScriptGameResourceObjectScriptedThingBase ctor: VC7.1 expands it at the default threshold T74, retail calls the COMDAT 0x007E72A0 there and expands only seh_me's (call 0x0099A380 ; vtable 0x0127094C ; zero +8/+C); RELOCATION_MATCH for T28..T36, T27 and T37+ differ; /d2inlS2..S20 never move it
// Leaf: ?Main@CTestPerson@CPersonalScript_EmptyScript@NScript@@EAEXXZ
// Flags: /d2inlT36
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EF4B40, 1268 bytes, thiscall, plain `ret`, no EH frame; ebp saved only on the loop path.
// Identity (proven): slot 1 (Main) of the CTestPerson entity vtable stored by its Alloc (bound "TestPerson" by the landed
//   CPersonalScript_EmptyScript::Main 0x00EF0090); Ego_r.pdb ?Main@CTestPerson@CPersonalScript_EmptyScript@NScript@@EAEXXZ.
// Callees: CScriptThing copy 0x004ABE90 / ~ 0x004AA840 (the CIsThingConscious temporary, vtable 0x012C2FE8, ctor and dtor expanded),
//   CEntityScriptBase::SetPredicate 0x00F35B10 / IsActiveThreadTerminating 0x00F35B30, CBaseObject 0x0099A380 (seh_me's ctor
//   expanded), ScriptedThingBase ctor 0x007E72A0 (seh_hero) / dtor 0x007E74D0 / IsValid 0x00CD23B9 / Release 0x00CD2770 / slots
//   MoveToPosition, MoveToThing, GetScriptThing, Speak(const char*), PlayLoopingAnimation, ClearAllActions, IsPerformingScriptTask,
//   MovieBase ctor 0x006E7B60 / dtor 0x006E7B80 (both called), CCharString 0x0099EBF0 / ~ 0x0099EAE0, CScriptThing::NullCoord
//   0x0143E8E0 (GetPos expanded), NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations 0x01375748; CGameScriptInterfaceBase
//   slots 7, 8, 72 GetThingWithScriptName, 264 GetHealth, 370 StartMovieSequence, 379 PauseAllNonScriptedEntities (the inline
//   CWideScreenMagicPauseEntities); CScriptThing slot 27 MsgIsTalkedToBy on Me.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CPERSONALSCRIPT_EMPTYSCRIPT_ENTITIES
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
// (STB ctor implicit: retail expands it for seh_me and calls the COMDAT for seh_hero)
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CPersonalScript_EmptyScript.h"
#include "engine/CIsThingConscious.h"
#include "engine/fable_script_animation_defaults.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingConscious::~CIsThingConscious() { }
inline const C3DVector& CScriptThing::GetPos() const { if (!PImp.Data) return NullCoord; return PImp.Data->GetPos(); }

void NScript::CPersonalScript_EmptyScript::CTestPerson::Main()
{
    SetPredicate(CIsThingConscious(Me));
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
    CScriptThing test_pos = Game->GetThingWithScriptName("TestPos");
    seh_me.MoveToThing(test_pos, 0.0f, (EScriptEntityMoveType)0, false, true, false, true);
    while (seh_me.IsPerformingScriptTask()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!IsActiveThreadTerminating()) {
        while (!Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        {
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            seh_me.PlayLoopingAnimation("MS18_SEATED_DAYDREAM_LOOP", -1, false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            CScriptThing hero_stand_pos = Game->GetThingWithScriptName("HeroStandPos");
            seh_hero.MoveToPosition(hero_stand_pos.GetPos(), 0.0f, (EScriptEntityMoveType)0, true, true);
        }
        while (!Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        {
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            seh_me.ClearAllActions();
            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Me, "This speech doesn't have any camera tags in it!", (ETextGroupSelectionMethod)0, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
            }
        }
        Game->NewScriptFrame();
    }
}