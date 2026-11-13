// NScript::CQ_HangingTreeGoodScript::DoCutscene(long) -- the three Hanging Tree (good) cut-scenes: 0 the intro (CS_HANGINGTREE_GOOD, then the quest start screen once), 1 the execution (CS_HANGINGTREE_GOOD_EXECUTION with hero, prisoner and executioner), 2 the escape (the guards lead the prisoner to the region exit while the camera watches)
// Leaf: ?DoCutscene@CQ_HangingTreeGoodScript@NScript@@QAEXJ@Z
// Inline-budget: header opt-in FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR (shared, FABLE_FORCEINLINE ctor) -- retail expands
//   the CWideScreenMagicPauseEntities ctor at all three sites (PauseAllNonScriptedEntities(true) 0x00D77712 / 0x00D77FC9 / 0x00D781A7); the
//   c2 inline trace (tools/inl i01, plain inline) calls the case-1 site (c1 17, f 0.0039, score 636 > 74) while the MovieBase ctor at the
//   same frequency (c1 20, score 748) and the case-1 hero handle ctor (c1 20, score 200) are called in retail too, and the case-0 handle
//   / map dtors at score 600 are called: no single /d2inlT separates 636 from 600 and 748, so the ctor is forced (as in 14 landed NScript
//   TUs). ~CCountedPointer<CScriptGameResourceObjectMovieBase> is declared out of line (retail calls 0x006E7AB0 at 0x00D77A69 /
//   0x00D77C43 inside the expanded movie dtors; the landed f4001 / PreMelee rows declare it the same way).
// Identity: Ego_r.pdb / FableWin.pdb ?DoCutscene@CQ_HangingTreeGoodScript@NScript@@QAEXJ@Z (public, one long); the landed DoMission
//   0x00D74430 calls it at 0x00D74650 (source DoCutscene(0)) and 0x00D74CC7 (DoCutscene(1)); no caller passes 2 in retail.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D77690, 3064 bytes, thiscall `ret 4`, frame 0xD0, no EH frame; switch dispatch `sub eax,0 ; je ;
//   dec ; je ; dec ; jne` with case 2 laid out first (fall-through), case 1 at 0x00D77CC8, case 0 at 0x00D78074 (source order 0, 1, 2).
// Callees (each paired with retail by tools/rel.py + tools/drel.py, one symbol per target): IsActiveThreadTerminating 0x00CB7940;
//   CCharString 0x0099EBF0 / ~ 0x0099EAE0; ~CScriptThing 0x004AA840, ~CBaseClass 0x0099A2E0; CBaseObject() 0x0099A380 / ~ 0x0099A430;
//   scripted-thing handle ctor 0x007E72A0 / dtor 0x007E74D0 / IsValid 0x00CD23B9 / Release 0x00CD2770 / operator= 0x008ABD10 /
//   MoveToPosition 0x007E72F0 / FollowThing 0x007E7320 / IsPerformingScriptTask 0x007E7450 / GetScriptThing 0x007E7490; movie ctor
//   0x006E7B60 / dtor 0x006E7B80 / ~CCountedPointer<MovieBase> 0x006E7AB0; cast map ctor 0x00CDBF70 / dtor 0x00CDBFB0 / operator[]
//   0x00CD3D2E; RunCutsceneMacro 0x00CBFB7D; IsDistanceFromThingToPositionOver 0x00CBE45C (Ego_r / FableWin, __fastcall); operator
//   delete 0x00BFE9BC; CScriptThing::NullCoord 0x0143E8E0; interface slots 7, 8, 70, 72, 81, 309, 370, 371, 379, 407, 416.
// Layout: engine/CQ_HangingTreeGoodScript.h (FABLE_RETAIL_CQ_HANGINGTREEGOODSCRIPT): QuestStartScreenShown +0x52, CurrentGuardVector
//   +0xAC (elements 0 and 1 copied inline).
// Shape: generated script code (`if (IsActiveThreadTerminating()) return;` after each wait loop and in each conditional block); the
//   cut-scene blocks of the landed CV_HeroDuelScript::FirstFleeCutscene 0x00E9A970 (handle IsValid / Release, StartScriptingEntity wait,
//   cast map, movie, CWideScreenMagicPauseEntities, FixMovieSequenceCamera around RunCutsceneMacro). The rest of the call / expand split
//   (handle ctors expanded except case 1's hero handle, movie ctor called in case 1, every dtor called at the natural exits and at the
//   case 0 / 1 returns, expanded at case 2's inner wait-loop exit, partly at its outer loop exit) is VC7.1's own with plain inline.
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_SCRIPT_MOVE_ENUM
#define FABLE_RETAIL_CQ_HANGINGTREEGOODSCRIPT
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0 (out of line in retail)
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_cutscene.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_HangingTreeGoodScript.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST = 0, SCRIPT_AI_PRIORITY_LOW = 1, SCRIPT_AI_PRIORITY_MEDIUM = 2, SCRIPT_AI_PRIORITY_HIGH = 3, SCRIPT_AI_PRIORITY_HIGHEST = 4 };
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing::~CScriptThing() { }
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
inline const C3DVector& CScriptThing::GetPos() const { if (!PImp.Data) return NullCoord; return PImp.Data->GetPos(); }
bool FABLE_FASTCALL IsDistanceFromThingToPositionOver(const CScriptThing&, const C3DVector&, float);   // 0x00CBE45C

void NScript::CQ_HangingTreeGoodScript::DoCutscene(long cutscene)
{
    switch (cutscene) {
    case 0: {
        CScriptGameResourceObjectScriptedThingBase seh_hero;
        if (seh_hero.IsValid()) seh_hero.Release();
        while (!Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
        cast["HERO"] = seh_hero;
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CWideScreenMagicPauseEntities pause(*Game);
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_HANGINGTREE_GOOD", cast, 0, 0, false, true);
        Game->FixMovieSequenceCamera(false);
        if (!QuestStartScreenShown) {
            if (IsActiveThreadTerminating()) return;
            Game->KickOffQuestStartScreen("Q_HangingTreeGood", true, false);
            QuestStartScreenShown = true;
        }
        break;
    }
    case 1: {
        CScriptThing executioner = Game->GetThingWithScriptName("HTG_Executioner");
        CScriptThing prisoner = Game->GetThingWithScriptName("HTG_Prisoner");
        CScriptGameResourceObjectScriptedThingBase seh_executioner;
        if (seh_executioner.IsValid()) seh_executioner.Release();
        while (!Game->StartScriptingEntity(executioner, seh_executioner, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        CScriptGameResourceObjectScriptedThingBase seh_prisoner;
        if (seh_prisoner.IsValid()) seh_prisoner.Release();
        while (!Game->StartScriptingEntity(prisoner, seh_prisoner, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        CScriptGameResourceObjectScriptedThingBase seh_hero;
        if (seh_hero.IsValid()) seh_hero.Release();
        while (!Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
        cast["HERO"] = seh_hero;
        cast["PRIS"] = seh_prisoner;
        cast["EXEC"] = seh_executioner;
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CWideScreenMagicPauseEntities pause(*Game);
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_HANGINGTREE_GOOD_EXECUTION", cast, 0, 0, false, true);
        Game->FixMovieSequenceCamera(false);
        break;
    }
    case 2: {
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CWideScreenMagicPauseEntities pause(*Game);
        CScriptThing prisoner = Game->GetThingWithScriptName("HTG_Prisoner");
        CScriptThing guard1 = CurrentGuardVector[0];
        CScriptThing guard2 = CurrentGuardVector[1];
        Game->CameraMoveToPosAndLookAtThing(Game->GetHero().GetPos() + C3DVector(0.0f, 0.0f, 2.0f), prisoner, 1.0f);
        CScriptGameResourceObjectScriptedThingBase seh_guard1;
        CScriptGameResourceObjectScriptedThingBase seh_guard2;
        CScriptGameResourceObjectScriptedThingBase seh_prisoner;
        Game->StartScriptingEntity(guard1, seh_guard1, SCRIPT_AI_PRIORITY_HIGHEST);
        if (guard2.IsAlive()) Game->StartScriptingEntity(guard2, seh_guard2, SCRIPT_AI_PRIORITY_HIGHEST);
        Game->StartScriptingEntity(prisoner, seh_prisoner, SCRIPT_AI_PRIORITY_HIGHEST);
        CScriptThing exit_point = Game->GetNearestWithDefName(guard1, "REGION_EXIT_POINT");
        seh_prisoner.FollowThing(guard1, 1.0f, true);
        if (guard2.IsAlive()) seh_guard2.FollowThing(prisoner, 1.0f, true);
        C3DVector exit_pos = exit_point.GetPos();
        while (IsDistanceFromThingToPositionOver(seh_guard1.GetScriptThing(), exit_pos, 2.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            seh_guard1.MoveToPosition(exit_pos, 0.0f, ENTITY_MOVE_WALK, false, true);
            while (seh_guard1.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        Game->CameraDefault();
        break;
    }
    }
}
