// NScript::CV_HeroDollsScript::CBS_Teacher::Main() -- the Bowerstone schoolteacher of the hero-dolls swap: hide the doll kid's information marker, then until the doll script ends: when the hero carries all the dolls, take the teacher over, wait for the hero to come close, follow him, introduce the swap and ask; yes swaps the dolls for the Jack of Blades doll and ends the script, no ends the loop; when the hero first carries a doll, the doll kid walks up, introduces the dolls and hands over the quest card and objective; otherwise note a newly received doll; finally release the teacher, complete and deactivate the quest
// Inline-budget: header-contract inline devices, no flag and no pragma (ablation/, each removed in turn) -- the row-local `inline CScriptThing::~CScriptThing() { }` (retail expands the dtor of the GetNearestWithDefName / GetScriptThing temporaries at some sites and calls 0x004AA840 at others; without it DIFFER(3379v3619)), the row-local `FABLE_FORCEINLINE CScriptThing::operator=` (retail expands both KidIntro assignments, CCountedPointer ShareData / ReleaseData inline, though an out-of-line copy 0x008AB980 exists; plain `inline` as the 147 landed rows: DIFFER(3532v3619); the header's out-of-line declaration: DIFFER(3468v3619); FABLE_FORCEINLINE as the 24 landed rows such as 0x00D52E90) and the out-of-line `template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer()` declaration (retail calls 0x006E7AB0 inside the expanded movie dtors and the whole dtor 0x006E7B80 elsewhere; without it DIFFER(3507v3619)). FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR is not used: the plain inline pause ctor is expanded at both sites (with the opt-in: the same bytes). No per-row flag was needed (the row matches at default flags).
// Leaf: ?Main@CBS_Teacher@CV_HeroDollsScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E97070, 3619 bytes, thiscall, plain `ret`, no EH frame.
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?Main@CBS_Teacher@CV_HeroDollsScript@NScript@@EAEXXZ and the Ego_d.pdb proc of the same
//   name; slot 1 of the CBS_Teacher entity vtable 0x012ECEB4 (slot 0 scalar deleting dtor 0x00E98430, 2 Init 0x00E97060, 3 GetParentScript 0x00E97050).
// Callees: IsActiveThreadTerminating 0x00F35B30, CCharString 0x0099EBF0 / ~ 0x0099EAE0 / operator!= 0x0099E960, ~CScriptThing 0x004AA840,
//   IsDistanceBetweenThingsUnder 0x00CBE2FF, ScriptedThingBase FollowThing 0x007E7320 / ClearCommands 0x007E7360 / Speak 0x007E7390 /
//   IsPerformingScriptTask 0x007E7450 / GetScriptThing 0x007E7490 / dtor 0x007E74D0 / IsValid 0x00CD23B9 / Release 0x00CD2770 (seh_me and
//   seh_KidIntro are locals, so these are direct calls), movie dtor 0x006E7B80 / ~CCountedPointer<MovieBase> 0x006E7AB0, CBaseObject 0x0099A380 /
//   ~ 0x0099A430, ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC, CV_HeroDollsScript::RemoveHeroDolls 0x00E97EA0 / HeroHasAllDolls 0x00E97FD0 /
//   HeroHasAnyDolls 0x00E98200; CGameScriptInterfaceBase and CScriptThing virtual slots as declared in the shared headers.
// Source shape: the parked chunk 845 draft's statement order (its bytes already matched apart from the entry store), written in the generated
//   quest-script text of the landed Mains: `TRetVoid _Fail_Val;` and `return _Fail_Val.Return();` at every exit, each speak-and-wait macro on
//   one line, movie and pause on one line, each terminate check on its statement's line. Names from Ego_d.pdb's locals: function scope seh_me,
//   _Fail_Val, DoesHeroHaveAnyDolls; blocks {answer, speech_movie, PauseMyEntitiesPlease} and {seh_KidIntro, {speech_movie, PauseMyEntitiesPlease}}.
// Inferred: dead conditional temporary -- (fitted statement; coordinator ruling 2026-11-20 under the owner's delegation, as the landed 0x00E72630 /
//   0x00E54E90) retail zeroes a conditional-temporary flag at entry (the dead store `mov [esp+20h],ebx` 0x00E97081; ebx is zeroed at 0x00E9707A for the seh_me ctor stores) and
//   never tests it. The bytes prove that a conditional temporary existed in dead code but constrain neither its content nor its place. Ego_d has
//   no such statement and no dead line (every Ego_d line of this proc has retail code, and there is no inner endless loop), so it is a new dead
//   statement after the DONT_SWAP arm's `break;`, with an empty body and only calls and literals this function already uses (the
//   GetActiveQuestName string temporary and CCharString::operator!=(""); the shortest of the matching operands, README). Ego_d's prolog line (100, 37 bytes) has no room for a flag init (a flag init is +10 bytes at /Od: this row's
//   VC7.1 /Od prolog line is 16 bytes without the fitted line, 26 with it; Ego_d's flagged siblings are 46 = 36 + 10), so its later revision had
//   no such temporary. The 1 byte over the flag-free 36 is a `push esi`: at /Od this row saves esi, as the Ego_d 47-byte rows 0x00E37E40 /
//   0x00EA59E0 do, and the 36 / 46 rows (0x00E54E90, 0x00E72630, 0x00E740C0, 0x00E71F10, 0x00ECE7F0) do not. Without it DIFFER(3619v3619), 70 diff lines: no entry store, `xor ebx,ebx` after the seh_me CBaseObject call, the code after it shifted.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/CScriptThing.h"
inline CScriptThing::~CScriptThing() { }
FABLE_FORCEINLINE CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CV_HeroDollsScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_distance.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
void NScript::CV_HeroDollsScript::CBS_Teacher::Main()
{
    CScriptGameResourceObjectScriptedThingBase seh_me;
    TRetVoid _Fail_Val;
    if (KidIntro.IsAlive()) {if(IsActiveThreadTerminating())return _Fail_Val.Return();
        Game->ClearThingHasInformation(KidIntro);
    }
    bool DoesHeroHaveAnyDolls = ParentClass->HeroHasAnyDolls();
    Game->Pause(0.5f);
    while (!ParentClass->HeroDollScriptEnded) {
        Game->NewScriptFrame();
        if(IsActiveThreadTerminating())return _Fail_Val.Return();
        if (ParentClass->HeroHasAllDolls()) {if(IsActiveThreadTerminating())return _Fail_Val.Return();
            PMasterData->HeroDollsScriptUsingTeacher = true;
            Game->SetThingHasInformation(Me, false, true, false);
            Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST);
            while (!IsDistanceBetweenThingsUnder(Game->GetHero(), Me, 20.0f)) {
                Game->NewScriptFrame();
                if(IsActiveThreadTerminating())return _Fail_Val.Return();
            }
            if(IsActiveThreadTerminating())return _Fail_Val.Return();
            seh_me.FollowThing(Game->GetHero(), 1.0f, true);
            while (!IsDistanceBetweenThingsUnder(Game->GetHero(), Me, 5.0f)) {
                Game->NewScriptFrame();
                if(IsActiveThreadTerminating())return _Fail_Val.Return();
            }
            if(IsActiveThreadTerminating())return _Fail_Val.Return();
            seh_me.ClearCommands();
            CScriptGameResourceObjectMovieBase speech_movie;Game->StartMovieSequence("",speech_movie);CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if(Game->GetHealth(seh_me.GetScriptThing())>0.0f){seh_me.Speak(Game->GetHero(),"TEXT_QST_083_TEACHER_INTRODUCTION",GROUP_SELECT_NONE,false,true,false);while(seh_me.IsPerformingScriptTask()){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}if(IsActiveThreadTerminating())return _Fail_Val.Return();}
            Game->GiveHeroYesNoQuestion("TEXT_QST_083_TEACHER_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
            long answer;
            while ((answer = Game->MsgIsQuestionAnsweredYesOrNo()) < 0) {
                Game->NewScriptFrame();
                if(IsActiveThreadTerminating())return _Fail_Val.Return();
            }
            if(IsActiveThreadTerminating())return _Fail_Val.Return();
            if (answer == 1) {if(IsActiveThreadTerminating())return _Fail_Val.Return();
                if(Game->GetHealth(seh_me.GetScriptThing())>0.0f){seh_me.Speak(Game->GetHero(),"TEXT_QST_083_TEACHER_DO_SWAP",GROUP_SELECT_NONE,false,true,false);while(seh_me.IsPerformingScriptTask()){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}if(IsActiveThreadTerminating())return _Fail_Val.Return();}
                ParentClass->RemoveHeroDolls();
                Game->GiveHeroObject("OBJECT_DOLL_JACK_OF_BLADES_01", -1, false);
                ParentClass->HeroDollScriptEnded = true;
            } else {if(IsActiveThreadTerminating())return _Fail_Val.Return();
                if(Game->GetHealth(seh_me.GetScriptThing())>0.0f){seh_me.Speak(Game->GetHero(),"TEXT_QST_083_TEACHER_DONT_SWAP",GROUP_SELECT_NONE,false,true,false);while(seh_me.IsPerformingScriptTask()){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}if(IsActiveThreadTerminating())return _Fail_Val.Return();}
                break;
                // Inferred: dead conditional temporary -- an empty-body dead statement after `break;` (see the header): a conditional temporary existed; its content and place are not constrained
                if(IsActiveThreadTerminating()||Game->GetActiveQuestName()!=""){}
            }
        } else if (!ParentClass->KidIntroDone && DoesHeroHaveAnyDolls) {if(IsActiveThreadTerminating())return _Fail_Val.Return();
            CScriptGameResourceObjectScriptedThingBase seh_KidIntro;
            KidIntro = Game->GetNearestWithDefName(Game->GetHero(), "CREATURE_BS_VILLAGER_BOY");
            if (KidIntro.GetName() != "") {if(IsActiveThreadTerminating())return _Fail_Val.Return();
                KidIntro = Game->GetNearestWithDefName(KidIntro, "CREATURE_BS_VILLAGER_BOY");
            }
            Game->SetThingHasInformation(KidIntro, false, true, false);
            Game->StartScriptingEntity(KidIntro, seh_KidIntro, SCRIPT_AI_PRIORITY_HIGHEST);
            seh_KidIntro.FollowThing(Game->GetHero(), 1.0f, true);
            Game->NewScriptFrame();
            if(IsActiveThreadTerminating())return _Fail_Val.Return();
            while (!IsDistanceBetweenThingsUnder(Game->GetHero(), KidIntro, 5.0f)) {
                Game->NewScriptFrame();
                if(IsActiveThreadTerminating())return _Fail_Val.Return();
            }
            if(IsActiveThreadTerminating())return _Fail_Val.Return();
            while (!Game->IsHeroControlledByPlayer() || Game->IsInCutscene() || Game->IsHeroInTavernGame()) {
                Game->NewScriptFrame();
                if(IsActiveThreadTerminating())return _Fail_Val.Return();
            }
            if(IsActiveThreadTerminating())return _Fail_Val.Return();
            seh_KidIntro.ClearCommands();
            {
                CScriptGameResourceObjectMovieBase speech_movie;Game->StartMovieSequence("",speech_movie);CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if(Game->GetHealth(seh_KidIntro.GetScriptThing())>0.0f){seh_KidIntro.Speak(Game->GetHero(),"TEXT_QST_083_DOLLKID_INTRODUCTION",GROUP_SELECT_NONE,false,true,false);while(seh_KidIntro.IsPerformingScriptTask()){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}if(IsActiveThreadTerminating())return _Fail_Val.Return();}
            }
            Game->ClearThingHasInformation(KidIntro);
            if (seh_KidIntro.IsValid())
                seh_KidIntro.Release();
            Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_HERO_DOLLS", Game->GetActiveQuestName(), false);
            Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_HERO_DOLLS_OBJECTIVE_01", "", "");
            ParentClass->KidIntroDone = true;
        } else {if(IsActiveThreadTerminating())return _Fail_Val.Return();
            if (Game->GetHero().MsgReceivedInventoryItem()) {if(IsActiveThreadTerminating())return _Fail_Val.Return();
                if (ParentClass->HeroHasAnyDolls()) {if(IsActiveThreadTerminating())return _Fail_Val.Return();
                    DoesHeroHaveAnyDolls = true;
                }
            }
        }
    }
    if(IsActiveThreadTerminating())return _Fail_Val.Return();
    Game->ClearThingHasInformation(Me);
    if (seh_me.IsValid())
        seh_me.Release();
    PMasterData->HeroDollsScriptUsingTeacher = false;
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), false, false, false);
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}