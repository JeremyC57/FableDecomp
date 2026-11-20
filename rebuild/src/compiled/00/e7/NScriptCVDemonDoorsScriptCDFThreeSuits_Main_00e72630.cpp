// NScript::CV_DemonDoorsScript::CDF_ThreeSuits::Main() -- demon-door face (three suits): take scripted control of Me, make it persistent and immune, shut the nearest REGION_EXIT_POINT; each time the hero talks to it, under a paused speech movie, walk the suit sequence (intro, then good knight, evil mage, bandit): say the "seen" line and advance when the hero wears the asked-for suit, else the "want" line; once the bandit suit is seen say the opening, fade the face out, open DD_ThreeSuits and reopen the exit
// Inline-budget: header-contract inline devices, no flag and no pragma (ablation/, each removed in turn) -- the out-of-line `template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer()` declaration (retail expands the movie dtor at the talk block's natural end and calls ~CCountedPointer 0x006E7AB0 inside it, and calls the whole movie dtor 0x006E7B80 at the shared exit; without it the whole movie dtor is called at both: DIFFER(2236v2252), 93 diff lines) and the inline CIsThingAlive ctor (expanded at SetPredicate, the CScriptThing copy 0x004ABE90 called; out of line DIFFER(2236v2252), 194 diff lines). No per-row flag was needed (the row matches at default flags).
// Leaf: ?Main@CDF_ThreeSuits@CV_DemonDoorsScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E72630, 2236 bytes of code + the 4-entry switch table 0x00E72EEC (2252 with the table), thiscall, plain `ret`, no EH frame.
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?Main@CDF_ThreeSuits@CV_DemonDoorsScript@NScript@@EAEXXZ and the Ego_d.pdb proc of the same name;
//   slot 1 of the CDF_ThreeSuits entity vtable stored by the landed Alloc 0x00E72560 (the "DF_ThreeSuits" binding of CV_DemonDoorsScript).
// Callees: CBaseObject 0x0099A380 / ~ 0x0099A430, CIsThingAlive (vtable 0x012C32F0, ~ 0x00E241B0) + CScriptThing copy 0x004ABE90, SetPredicate
//   0x00F35B10, IsActiveThreadTerminating 0x00F35B30, ScriptedThingBase IsValid 0x00CD23B9 / Release 0x00CD2770 / ~ 0x007E74D0 / GetScriptThing
//   0x007E7490 / Speak 0x007E7390 / IsPerformingScriptTask 0x007E7450, movie dtor 0x006E7B80, ~CScriptThing 0x004AA840, CCharString 0x0099EBF0 /
//   ~ 0x0099EAE0, operator delete 0x00BFE9BC, DressedAsBandit 0x00E72F00 / DressedAsGoodKnight 0x00E730A0 / DressedAsEvilMage 0x00E731F0,
//   CV_DemonDoorsScript::FadeOutFaceAndOpenDoor 0x00E6F040; CGameScriptInterfaceBase slots 7, 8, 70 GetHero, 81 GetNearestWithDefName, 264 GetHealth,
//   370 StartMovieSequence, 379 PauseAllNonScriptedEntities, 477 SetThingPersistent, 516 / 517 / 526, 567 SetRegionExitAsActive; CScriptThing slot 27
//   MsgIsTalkedToBy. Text keys TEXT_QST_088_THREE_SUITS_INTRO 0x012E925C .. _OPENING, door "DD_ThreeSuits" 0x012E9140.
// Source shape: Ego_d.pdb line table (lines 672..742, ref egod_lines/) and locals: function scope `region_exit`, `seh_me`, `_Fail_Val`,
//   `WhatSuitIsNext` (an anonymous local enum, LF_ENUM 0x148AD `...::Main::__l16::<unnamed-type-WhatSuitIsNext>`); one block {speech_movie,
//   PauseMyEntitiesPlease}. Generated quest-script text as the landed DemonDoors Mains: the seh macro and each speak-and-wait macro on one line,
//   the movie and the pause on one line (696, 101 bytes), the switch at 698 with fall-through cases (breaks 712 / 722, none after the last arm),
//   `for(;!false;Game->NewScriptFrame()){if(T)return..;` (692; retail duplicates the head check at the bottom 0x00E72D0F), the bandit arm's
//   `return _Fail_Val.Return();` (732) and the post-loop terminate check (741). The static S_LDATA32 of the Ego_d proc is the switch table.
// Inferred (fitted statement; coordinator ruling 2026-11-20 under the owner's delegation): retail zeroes a conditional-temporary flag at entry
//   (`xor ebp,ebp` before the seh_me ctor, then the dead home store `mov [esp+3Ch],ebp` 0x00E72641) and never tests it. The bytes prove that a
//   conditional temporary existed in dead code but constrain neither its content nor its place (the same operand after the bandit return also
//   matches; `&&` matches too). It is written as one extra operand of the dead post-loop check (line 741), with only calls and literals this
//   function already uses. Ego_d's prolog line (672, 36 bytes = the flag-free siblings; 46 with a flag) shows its later source revision had no
//   such temporary. Without it DIFFER(2236v2252): the 4-byte store also moves both loop heads off their 16-byte pads (0x00E72709, 0x00E727DD).
//   Same idiom as the landed 0x00DA0230.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDF_THREESUITS
#include "engine/CV_DemonDoorsScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}

template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0 (out of line, as the landed CDF_Dragon::Main 0x00E6FC40)

void NScript::CV_DemonDoorsScript::CDF_ThreeSuits::Main()
{
    CScriptGameResourceObjectScriptedThingBase seh_me;
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();
    if(seh_me.IsValid())seh_me.Release();while(!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}if(IsActiveThreadTerminating())return _Fail_Val.Return();
    Game->SetThingPersistent(Me,true);
    Game->EntitySetAsDamageable(Me,false);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me,false);
    Game->EntitySetAsKillable(Me,false,false);
    CScriptThing region_exit=Game->GetNearestWithDefName(Me,"REGION_EXIT_POINT");
    Game->SetRegionExitAsActive(region_exit,false);
    enum
    {
        BEGINNING,
        GOOD_KNIGHT,
        EVIL_MAGE,
        BANDIT
    } WhatSuitIsNext;
    WhatSuitIsNext=BEGINNING;
    for(;!false;Game->NewScriptFrame()){if(IsActiveThreadTerminating())return _Fail_Val.Return();
        if(Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")){if(IsActiveThreadTerminating())return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;Game->StartMovieSequence("",speech_movie);CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            switch(WhatSuitIsNext){
            case BEGINNING:
                if(Game->GetHealth(seh_me.GetScriptThing())>0.0f){seh_me.Speak(Game->GetHero(),"TEXT_QST_088_THREE_SUITS_INTRO",GROUP_SELECT_NONE,false,true,false);while(seh_me.IsPerformingScriptTask()){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}if(IsActiveThreadTerminating())return _Fail_Val.Return();}
                WhatSuitIsNext=GOOD_KNIGHT;
            case GOOD_KNIGHT:
                if(DressedAsGoodKnight()){if(IsActiveThreadTerminating())return _Fail_Val.Return();
                    if(Game->GetHealth(seh_me.GetScriptThing())>0.0f){seh_me.Speak(Game->GetHero(),"TEXT_QST_088_THREE_SUITS_SEEN_KNIGHT",GROUP_SELECT_NONE,false,true,false);while(seh_me.IsPerformingScriptTask()){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}if(IsActiveThreadTerminating())return _Fail_Val.Return();}
                    WhatSuitIsNext=EVIL_MAGE;
                }
                else{if(IsActiveThreadTerminating())return _Fail_Val.Return();
                    if(Game->GetHealth(seh_me.GetScriptThing())>0.0f){seh_me.Speak(Game->GetHero(),"TEXT_QST_088_THREE_SUITS_WANT_KNIGHT",GROUP_SELECT_NONE,false,true,false);while(seh_me.IsPerformingScriptTask()){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}if(IsActiveThreadTerminating())return _Fail_Val.Return();}
                    break;
                }
            case EVIL_MAGE:
                if(DressedAsEvilMage()){if(IsActiveThreadTerminating())return _Fail_Val.Return();
                    if(Game->GetHealth(seh_me.GetScriptThing())>0.0f){seh_me.Speak(Game->GetHero(),"TEXT_QST_088_THREE_SUITS_SEEN_MAGE",GROUP_SELECT_NONE,false,true,false);while(seh_me.IsPerformingScriptTask()){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}if(IsActiveThreadTerminating())return _Fail_Val.Return();}
                    WhatSuitIsNext=BANDIT;
                }
                else{if(IsActiveThreadTerminating())return _Fail_Val.Return();
                    if(Game->GetHealth(seh_me.GetScriptThing())>0.0f){seh_me.Speak(Game->GetHero(),"TEXT_QST_088_THREE_SUITS_WANT_MAGE",GROUP_SELECT_NONE,false,true,false);while(seh_me.IsPerformingScriptTask()){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}if(IsActiveThreadTerminating())return _Fail_Val.Return();}
                    break;
                }
            case BANDIT:
                if(DressedAsBandit()){if(IsActiveThreadTerminating())return _Fail_Val.Return();
                    if(Game->GetHealth(seh_me.GetScriptThing())>0.0f){seh_me.Speak(Game->GetHero(),"TEXT_QST_088_THREE_SUITS_SEEN_BANDIT",GROUP_SELECT_NONE,false,true,false);while(seh_me.IsPerformingScriptTask()){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}if(IsActiveThreadTerminating())return _Fail_Val.Return();}
                    if(Game->GetHealth(seh_me.GetScriptThing())>0.0f){seh_me.Speak(Game->GetHero(),"TEXT_QST_088_THREE_SUITS_OPENING",GROUP_SELECT_NONE,false,true,false);while(seh_me.IsPerformingScriptTask()){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}if(IsActiveThreadTerminating())return _Fail_Val.Return();}
                    ParentClass.FadeOutFaceAndOpenDoor(Me,"DD_ThreeSuits");
                    Game->SetRegionExitAsActive(region_exit,true);
                    return _Fail_Val.Return();
                }
                else{if(IsActiveThreadTerminating())return _Fail_Val.Return();
                    if(Game->GetHealth(seh_me.GetScriptThing())>0.0f){seh_me.Speak(Game->GetHero(),"TEXT_QST_088_THREE_SUITS_WANT_BANDIT",GROUP_SELECT_NONE,false,true,false);while(seh_me.IsPerformingScriptTask()){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}if(IsActiveThreadTerminating())return _Fail_Val.Return();}
                    break;
                }
            }
        }
    }
    // Inferred: the `||Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")` operand of this dead post-loop check (see the header: a conditional temporary existed; its content and place are not constrained)
    if(IsActiveThreadTerminating()||Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO"))return _Fail_Val.Return();
}