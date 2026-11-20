// NScript::CV_ChapelOfEvilScript::EndMission() -- wait until the quest is complete, then until the hero has left the DarkwoodChapel region, then hide the quest info and mark the quest finished
// Leaf: ?EndMission@CV_ChapelOfEvilScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E6A9E0, length 210 bytes (224 with padding).
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (landed), CCharString::CCharString(const char*, long)
// @ 0x0099EBF0, ~CCharString @ 0x0099EAE0, and through the quest's game script interface (+0x40): NewScriptFrame
// (slot 7, +0x1c), IsRegionLoaded(const CCharString&) (slot 12, +0x30), DisplayQuestInfo(bool) (slot 321, +0x504).
// Identity: the only caller is 0x00E6A929 inside the ChapelOfEvil DoMission; the flags fit Ego_r.pdb
// CV_ChapelOfEvilScript - 0x14 (QuestFinished +0x6c, QuestComplete +0x6d).
// Shape: two script waits written as the usual `while (!cond) { NewScriptFrame(); if (terminating) return; }`
// loops, each followed by a terminating check; the region-name temporary is built and destroyed inside every loop
// test through an inline helper holding a NAMED CCharString local (a bare temporary argument lets VC fetch the
// interface vtable into edi before the string is built) and `? true : false` (the plain bool return gives
// `mov bl,al` where retail re-tests with `test al,al ; setne bl`); the bool survives the destructor in bl.
// Naming (NF162, 2026-11-20, q6273): was a row-local global `struct CV_ChapelOfEvilScript` over a local CQuestScriptBase (leaf
//   ?EndMission@CV_ChapelOfEvilScript@@QAEXXZ, which its caller DoMission 0x00E6A7F0 bound); now on the shared
//   engine/CV_ChapelOfEvilScript.h (Ego_r.pdb ?EndMission@CV_ChapelOfEvilScript@NScript@@QAEXXZ; its only caller DoMission moves in the
//   same hand-back). The region test is a row-local anonymous-namespace inline helper taking the script (it was an invented inline
//   member of the local model, ?IsInDarkwoodChapel@CV_ChapelOfEvilScript@@QAE_NXZ); it is expanded at both sites and its unreferenced
//   COMDAT carries the per-TU ?A0x hash, so nothing binds it (descriptive name). Taking the interface pointer instead reads Game before
//   the string is built: DIFFER(213v210).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CV_ChapelOfEvilScript.h"

namespace {
inline bool IsInDarkwoodChapel(NScript::CV_ChapelOfEvilScript* script) { CCharString region("DarkwoodChapel", -1); return script->Game->IsRegionLoaded(region) ? true : false; }
}

void NScript::CV_ChapelOfEvilScript::EndMission()
{
    while (!QuestComplete) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (IsInDarkwoodChapel(this)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->DisplayQuestInfo(false);
    QuestFinished = true;
}
