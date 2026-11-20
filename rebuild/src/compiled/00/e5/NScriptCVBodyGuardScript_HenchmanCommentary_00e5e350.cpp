// NScript::CV_BodyGuardScript::HenchmanCommentary() -- the body-guard level script's commentary thread: every frame, once a henchman is hired, clear the master data's commentary state when its timer runs out, re-read every second whether the hero wields a weapon, reset the stood-still counter when the hero moves, and every 5 seconds of idleness let a henchman make an idle comment
// Inline-budget: no flag; one scoped `#pragma inline_depth(0)` region (fitted; retail's construct there is unproven) round the one statement `if(IsActiveThreadTerminating())return _Fail_Val.Return();` inside `if(MakeHenchmanPerformIdleComment())`. What it suppresses: the two implicit ~CTimer calls of that return (CheckExpensiveThingsTimer, then IdleConversationTimer), now out of line to 0x00CD4470; nothing else changes (IsActiveThreadTerminating 0x00CB7940 is never inline, the empty inline TRetVoid::Return emits nothing, and a plain `return;` there gives the same bytes). The bytes prove ~CTimer called at this exit only (0x00E5E580 / 0x00E5E58D) and expanded at the shared exit (DeregisterTimer 0x00E5E564 / 0x00E5E573). In the c2 trace ~CTimer flips from expanded to called between site frequency 0.0346 (expanded) and 0.0313 (called) (formula 0.032 for c 14), while the `=5` operator= after it must stay expanded as in retail, which it is at 0.0692 and not at 0.0346 (formula 0.050 for c 20). Plain, the check is an exit of the main loop at its exit share 0.125 (sc 28), like every in-loop exit; with `do{...}while(0)` at the check its dtors take the operator='s frequency (0.5, 0.0692, 0.0346, ... down to 0.0); graded families: all 256 plain / `do{...}while(0)` placements over the eight terminate checks (186 DIFFER(559v585), 53 DIFFER(553v585), 11 DIFFER(579v585) and 4 DIFFER(526v585) / 1 DIFFER(520v585) with the dtors and the `=5` operator= called together, 1 DIFFER(500v585)), a `{}` block, and an if/else around `IdleConversationTimer=5` (DIFFER(559v585)). Without the region DIFFER(559v585); inline_depth(1) DIFFER(559v585); no /d2inl value splits the exits: /d2inlT20 DIFFER(526v585), T27 DIFFER(532v585), T30 DIFFER(506v585), T100 / S20 / S30 DIFFER(559v585). The wave-8 CTimer bodies defined inline after the include, each expanded by retail (ctor 0x00E5E368 / 0x00E5E37C, GetTime 0x00E5E407 / 0x00E5E433 / 0x00E5E508, operator= 0x00E5E457 / 0x00E5E53E, dtor 0x00E5E564 / 0x00E5E573): without the ctor body DIFFER(588v585), GetTime DIFFER(574v585), operator= DIFFER(584v585), dtor DIFFER(561v585); the header opt-in FABLE_RETAIL_C3DVECTOR_NOT_EQUAL (C3DVector::operator!= with its in-class body, expanded at 0x00E5E47D..0x00E5E4BE; the header has no declaration-only form, SRC_FAIL without it).
// Leaf: ?HenchmanCommentary@CV_BodyGuardScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E5E350, 585 bytes, thiscall, frameless (esp frame 0x14), no EH frame.
// Identity: Ego_r.pdb / FableWin.pdb ?HenchmanCommentary@CV_BodyGuardScript@NScript@@QAEXXZ (public; engine/CV_BodyGuardScript.h names
//   0x00E5E350 under FABLE_RETAIL_CV_BODYGUARDSCRIPT_COMMENTARY). Ego_d.pdb (debug length 956): function-scope locals
//   CheckExpensiveThingsTimer, IdleConversationTimer, _Fail_Val (TRetVoid), HeroLastPosition (C3DVector); no blocks.
// Inferred: the declaration order IdleConversationTimer, CheckExpensiveThingsTimer (Ego_d, a later revision, lists CheckExpensiveThingsTimer
//   first). Retail destroys CheckExpensiveThingsTimer first at both exits, the reverse of declaration: out-of-line exit ~CTimer
//   CheckExpensiveThingsTimer ([esp+0Ch]) 0x00E5E580 before IdleConversationTimer ([esp+10h], the timer set to 5 at 0x00E5E53D and read at
//   0x00E5E503) 0x00E5E58D; shared exit DeregisterTimer of the same two 0x00E5E564 before 0x00E5E573. Swapping them is a killed mutant.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CV_BodyGuardScript::MakeHenchmanPerformIdleComment 0x00E5E5A0,
//   CCounter::operator=(float) 0x00D90550, ~CTimer 0x00CD4470 (the one exit); pDSTGame 0x0143E8F8 timer slots 87 RegisterTimer /
//   88 DeregisterTimer / 89 SetTimer / 90 GetTimer; Game slots 7 NewScriptFrame, 70 GetHero, 195 IsPlayerWieldingWeapon; CScriptThing
//   slot 6 GetPos; the inline C3DVector::operator!= against DELTA_NOTIONAL_ZERO 0x0129BA3C.
// Shape: the quest-script text of the landed Mains: `TRetVoid _Fail_Val;` and `return _Fail_Val.Return();` at every exit, each terminate check
//   on its statement's line (a plain `return;` gives the same bytes here); the timers' ctor / dtor / GetTime / operator= defined inline
//   after the include (engine/CTimer.h, wave 8).
// Inferred: the main loop as a constant-true loop with NewScriptFrame as its last statement (`while(true)`; `for(;;)`, `while(!false)` and
//   `for(;!false;)` give the same bytes). The generated `for(;!false;Game->NewScriptFrame())` of other Mains DIFFER(526v585): with the frame
//   call in the increment c2 does not weight the loop body, so it calls C3DVector::operator!= and the `=5` operator= out of line.
//   Ego_d's revision has the frame call on the loop-head line: no frame-call line before the loop's `}` (286 `IdleConversationTimer=5;` 10 B,
//   290 `}` 5 B), and its head line 254 is 85 B (the generated for header with its check is 89 B at /Od /EHsc, `for(;;Game->NewScriptFrame())`
//   76 B); retail's bytes reject that form (`for(;;Game->NewScriptFrame())` DIFFER(579v585)), so retail's loop differs from Ego_d's here.
// Inferred: dead conditional temporary -- (fitted statement; the coordinator's 2026-11-20 ruling, as the landed 0x00DA0230 / 0x00E72630 /
//   0x00E54E90 / 0x00E97070) retail zeroes a conditional-temporary flag at entry (the dead store `mov dword ptr [esp+0Ch],0` 0x00E5E355, in
//   the slot c2 later gives IdleConversationTimer) and never tests it. The bytes prove a conditional temporary existed in dead code; they
//   constrain neither its content nor its place. It sits on Ego_d line 292, the post-loop check: /Od bytes (52, exactly the plain
//   `if(IsActiveThreadTerminating())return _Fail_Val.Return();` at /Od /EHsc) but no retail bytes (the loop never exits normally). It is the
//   fewest-token operand that matches, `||CTimer().GetTime()`, using only calls this function already uses (CTimer's inline members).
//   Without it, or with the plain check, DIFFER(584v585): no entry store, and the loop heads move onto 16-byte pads (0x00E5E38F `nop`,
//   0x00E5E3AA `lea ebx,[ebx]`).
#define FABLE_RETAIL_CTIMER_ORIGINAL_ACCESS
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCOUNTER
#define FABLE_RETAIL_C3DVECTOR_NOT_EQUAL
#define FABLE_RETAIL_CV_BODYGUARDSCRIPT_COMMENTARY
#include "engine/C3DVector.h"
#include "engine/TRetVoid.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CV_BodyGuardScript.h"
extern CGameScriptInterfaceBase* pDSTGame;
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
inline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex,time); return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
void NScript::CV_BodyGuardScript::HenchmanCommentary()
{
    CTimer IdleConversationTimer;
    CTimer CheckExpensiveThingsTimer;
    TRetVoid _Fail_Val;
    C3DVector HeroLastPosition;
    while(true){if(IsActiveThreadTerminating())return _Fail_Val.Return();
        while(HenchmanList.size()==0){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}
        if(IsActiveThreadTerminating())return _Fail_Val.Return();
        if(MasterScriptData->HenchmanCommentaryState&&CommentaryTimer.GetTime()==0){
            if(IsActiveThreadTerminating())return _Fail_Val.Return();
            MasterScriptData->HenchmanCommentaryState=0;
        }
        if(CheckExpensiveThingsTimer.GetTime()==0){
            if(IsActiveThreadTerminating())return _Fail_Val.Return();
            CheckExpensiveThingsTimer=1;
            HeroWieldingWeapon=Game->IsPlayerWieldingWeapon();
        }
        if(HeroLastPosition!=Game->GetHero().GetPos()){
            if(IsActiveThreadTerminating())return _Fail_Val.Return();
            HeroStoodStillCounter=0.0f;
        }
        HeroLastPosition=Game->GetHero().GetPos();
        if(IdleConversationTimer.GetTime()==0){
            if(IsActiveThreadTerminating())return _Fail_Val.Return();
            if(MakeHenchmanPerformIdleComment()){
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                if(IsActiveThreadTerminating())return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                IdleConversationTimer=5;
            }
        }
        Game->NewScriptFrame();
    }
    // Inferred: dead conditional temporary -- the `||CTimer().GetTime()` operand of this dead post-loop check (Ego_d line 292; see the header:
    // a conditional temporary existed; its content and place are not constrained)
    if(IsActiveThreadTerminating()||CTimer().GetTime())return _Fail_Val.Return();
}