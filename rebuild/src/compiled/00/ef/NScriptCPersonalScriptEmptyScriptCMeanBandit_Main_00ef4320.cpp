// NScript::CPersonalScript_EmptyScript::CMeanBandit::Main() -- the mean bandit entity script: under an alive predicate on Me, loop while the thread runs: bind Me's hands and wait until the hero is unclothed, then free the hands, let Me steal stealable items with a zero combat break-off range for as long as the hero stays unclothed, then reset both and wait a frame
// Inline-budget: flag /d2inlT28 -- CScriptThing's copy is left implicit (FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY): retail expands it for the two Set* by-value arguments (CBaseClass vptr 0x01231710, then 0x01238C8C, Data/Info, counted ++) and calls the COMDAT 0x004ABE90 for the predicate's Thing and the two Reset* arguments; VC7.1 does exactly that for T23..T28 (T22 calls all four, T29+ expands more), and T28 is also inside the T28..T36 window of the sibling CTestPerson::Main 0x00EF4B40 (same source file)
// Leaf: ?Main@CMeanBandit@CPersonalScript_EmptyScript@NScript@@EAEXXZ
// Flags: /d2inlT28
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EF4320, 420 bytes, thiscall, plain `ret`, no frame pointer, no EH frame.
// Identity (proven): slot 1 (Main) of the CMeanBandit entity vtable 0x012F6488 (only reference .rdata 0x012F648C); Ego_r.pdb and
//   FableWin.pdb ?Main@CMeanBandit@CPersonalScript_EmptyScript@NScript@@EAEXXZ (private virtual).
// Callees: CScriptThing copy 0x004ABE90 (called for the CIsThingAlive temporary and the Reset* arguments, expanded for the Set*
//   arguments), NScript::CIsThingAlive (vtable 0x012C32F0, ctor expanded, ~CIsThingAlive 0x00E241B0 called),
//   CEntityScriptBase::SetPredicate 0x00F35B10 / IsActiveThreadTerminating 0x00F35B30; CGameScriptInterfaceBase slots 7
//   NewScriptFrame (+0x1c), 188 IsHeroNaked (+0x2f0), 494 EntitySetAsHavingBoundHands (+0x7b8), 817 SetCombatNearbyBreakOffRange
//   (+0xcc4), 819 ResetCombatNearbyBreakOffRange (+0xccc), 820 SetStealStealableItems (+0xcd0), 822 ResetStealStealableItems (+0xcd8).
// Shape: &Me (this+8) lives in edi for the whole function; the wait loops are the script macro `while (cond) { NewScriptFrame();
//   if (terminating) return; } if (terminating) return;`; the by-value CScriptThing arguments are built straight in their stack slots.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CPERSONALSCRIPT_EMPTYSCRIPT_ENTITIES
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CPersonalScript_EmptyScript.h"
#include "engine/CIsThingAlive.h"
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }

void NScript::CPersonalScript_EmptyScript::CMeanBandit::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!IsActiveThreadTerminating()) {
        Game->EntitySetAsHavingBoundHands(Me, true);
        while (!Game->IsHeroNaked()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->EntitySetAsHavingBoundHands(Me, false);
        Game->SetStealStealableItems(Me, true);
        Game->SetCombatNearbyBreakOffRange(Me, 0.0f);
        while (Game->IsHeroNaked()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->ResetStealStealableItems(Me);
        Game->ResetCombatNearbyBreakOffRange(Me);
        Game->NewScriptFrame();
    }
}